/******************************************************************************
***********   .d8888b.   .d8888b    *******************************************
**********   d88P  Y88b d88P  Y88b   ******************************************
*            888    888        888   ******************************************
*    8888b.  888    888      .d88P   ******************************************
*   888 "88b 888    888  .od888P"  ********************************************
*   888  888 888    888 d88P"    **********************************************
*   888  888 Y88b  d88P 888"        *******************************************
*   888  888  "Y8888P"  888888888            Open Kaillera: http://okai.sf.net
******************************************************************************/
#include "p2p_ui.h"

#include "uihlp.h"
#include "resource.h"
//#include "kailleraclient.h"
#include "common/nSTL.h"
#include "common/k_socket.h"
#include "common/nThread.h"
#include "common/nSettings.h"
#include "common/spcharlist.h"
#include "common/rTimer.h"
#include "errr.h"

extern HINSTANCE hx;
#pragma optimize("", off)
#pragma optimize("agt", on)

////void ConnectToServer(char * ip, int port, HWND pDlg, char*name);

rTimer timer;

bool IsLocalIP(char * host) {
	if (strncmp("127.", host, 4)==0||strncmp("10.", host, 3)==0||strncmp("192.168.", host, 8)==0)
		return true;

	if (strncmp("172.", host, 4)==0){
		int secoct = atoi(host+4);
		if (secoct >= 16 && secoct <=31)
			return true;
	}

	if (inet_addr(host) == -1)
		return true;
	else
		return false;
}


#define GETTO(ptr, chr) if (*ptr!=chr) while(*++ptr!=chr);
#define GETTOX(ptr, chr) while(*++ptr!=chr);
#define GETTOR(ptr, chr) if (*ptr!=chr) while(*++ptr!=chr);*ptr++ = 0;
#define GETTORX(ptr, chr) while(*++ptr!=chr);*ptr++ = 0;

HWND kaillera_mslref;
HWND kaillera_mslrefx;
#define KAILLERA_PING_LIMIT 1000
#define KAILLERA_PING_SIMULT 15

class PingSocket: public k_socket {
	DWORD start_time;
	int state;
public:
	unsigned int limit;
	char * ptr;
	char * temp;
	PingSocket(){
		k_socket();
		state = 0;
		start_time = 0;
		limit = 0;
		initialize(0, -1);
	}
	void ping_host(char * host, int p_port, char * DATA_ = "PING", int SIZE_ = 5){
		set_address(host, p_port);
		start_time = timer.GetTime();
		send(DATA_, SIZE_);
		limit = KAILLERA_PING_LIMIT;
	}
	bool is_done(DWORD tiNow){
		if (tiNow - start_time > KAILLERA_PING_LIMIT){
			start_time = -1;
			return true;
		} else {
			if (has_data()){
				start_time = tiNow - start_time;
				return true;
			}
			return false;
		}
	}
	int return_ping(){
		if (start_time != -1){
			return start_time;
		}
		start_time = 0;
		return -1;
	}
	static bool step(int ms){
		return check_sockets(0, ms);
	}
};


int DownloadListToBuffer(char * buffer, int size, char * url){
	rTimer dTimer;

	char urx[2000];
	strcpy(urx, url);
	char * urlbuf = urx;
	char * host;
	char addr[500]; 
	unsigned long ul = 5;
	sockaddr_in server;
	static char RequestBuffer[1024];
	host = strstr(urlbuf, "http://")+7;
	n02_TRACE();
	if(strstr(host, ":")!=NULL){
		char * port = strstr(host, ":")+1;
		server.sin_port = htons(atoi(port));
		strcpy(addr, strstr(port, "/"));
		//host = strstr(port, "/");
		//*host++=0x00;
		*(port-1)=0x00;
	} else{
		server.sin_port = htons(80);
		strcpy(addr, strstr(host, "/"));
		*(strstr(host, "/"))=0x00;
	}
	n02_TRACE();
	wsprintf(RequestBuffer,"GET %s HTTP/1.0\r\nHost: %s\r\nAccept: */*\r\nUser-Agent: n02\r\nConnection: close\r\n\r\n", addr, host);
	//socket
	//MessageBox(0,RequestBuffer,0,0);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(s==SOCKET_ERROR)
		return 0;

	//ioctlsocket(s, FIONBIO, &ul);
	if(*(host)> 0x30 && *(host)<0x3A){//ip address
		server.sin_addr.s_addr = inet_addr(host);
	} else {
		server.sin_addr = *(struct in_addr*)gethostbyname(host)->h_addr_list[0];
	}
	//n02_TRACE();
	server.sin_family = AF_INET;
	if(connect(s, (struct sockaddr *)&server, sizeof(server))!=0){
		if(WSAGetLastError()!=WSAEWOULDBLOCK) return 0;
	}
	n02_TRACE();//Sleep(300);
	//sendto(s, RequestBuffer, strlen(RequestBuffer), 0, (sockaddr*)&server, sizeof(server));
	send(s, RequestBuffer, strlen(RequestBuffer), 0);
	char * bf = buffer;
	int il = size;
	dTimer.ResetTime();
	while (dTimer.GetTime() < 60000){
		//Sleep(25);
		int rec = recv(s, bf, il, 0);
		if(rec<=0) //if(rec==0)
			break;
		//if(rec==SOCKET_ERROR && WSAGetLastError()==10035)
		//	continue;
		bf += rec;
		il -= rec;
	}
	closesocket(s);
	n02_TRACE();
	return size-il;
}



nLVw kaillera_mlv;
int kaillera_mslistColumn;
int kaillera_mslistColumnTypes[7] = {1, 1, 0, 0, 0, 1, 1};
int kaillera_mslistColumnOrder[7];


int CALLBACK kaillera_mslistCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort){
	int ind1 = kaillera_mlv.Find(lParam1);
	int ind2 = kaillera_mlv.Find(lParam2);
	if (ind1 == -1 || ind2 == -1)
		return 0;
	char ItemText1[128];
	char ItemText2[128];
	kaillera_mlv.CheckRow(ItemText1, 128, lParamSort, ind1);
	kaillera_mlv.CheckRow(ItemText2, 128, lParamSort, ind2);
	if (kaillera_mslistColumnTypes[lParamSort]) {
		if (kaillera_mslistColumnOrder[lParamSort])
			return strcmp(ItemText1, ItemText2);
		else
			return -1* strcmp(ItemText1, ItemText2);
	} else {
		ind1 = atoi(ItemText1);
		ind2 = atoi(ItemText2);
		if (kaillera_mslistColumnOrder[lParamSort])
			return (ind1==ind2? 0 : (ind1>ind2? 1 : -1));
		else
			return (ind1==ind2? 0 : (ind1>ind2? -1 : 1));
	}
}

void kaillera_mslistReSort() {
	ListView_SortItems(kaillera_mlv.handle, kaillera_mslistCompareFunc, kaillera_mslistColumn);
}

void kaillera_mslistSort(int column) {
	kaillera_mslistColumn = column;
	if (kaillera_mslistColumnOrder[column])
		kaillera_mslistColumnOrder[column] = 0;
	else
		kaillera_mslistColumnOrder[column] = 5;

	kaillera_mslistReSort();
}


class ListRefresher: public nThread {
public:
	bool running;
	bool runn;
	char buffer[0x8000];
	slist<PingSocket*, KAILLERA_PING_SIMULT> plist;
	int ltype;

	
	void refresh_p2p_wgames() {
		timer.ResetTime();

		n02_TRACE();
		char lurl[256];
		nSettings::get_str("r2wu", lurl, "http://kp2pm.movsq.net:27889/plist.txt");
		int l = DownloadListToBuffer(buffer, 0x8000, lurl);
		n02_TRACE();
		//=====================================================================
		if (runn && l > 7) {
			n02_TRACE();
			int total=0;
			//=================================================================
			char * ptr = strstr(buffer, "\r\n\r\n") + 4;
			if(ptr==NULL)
				return;
			n02_TRACE();
			*((char*)strstr(ptr, "\n")) = 0;
			//=================================================================
			SetWindowText(kaillera_mslref,"Parsing list...");
			n02_TRACE();
			spcharlist * list = static_explode_char('|', ptr);
			n02_TRACE();
			if (runn && list->length > 4) {
				n02_TRACE();
				//=============================================================
				SetWindowText(kaillera_mslref,"Pinging list...");
				int x = 0;
				//=============================================================
				while (runn && (x+4 < list->length || plist.size() > 0)) { 
					if (x+4 < list->length && plist.size() < KAILLERA_PING_SIMULT && plist.size() < total + 1){
						//=====================================================
						char * host = list->items[x+3];
						char * sptr = host;
						while(*sptr!=':' && *sptr!=0) sptr++;
						if (*sptr == ':') {
							*sptr = 0;
							if (!IsLocalIP(host)) {
								int port = atoi(sptr+1);
								PingSocket * ps = new PingSocket;
								ps->ping_host(host, port, "\x00PINGRQ", 8);
								*sptr = ':';
								ps->ptr = (char*)x;
								plist.add(ps);
							}
						}
						//=====================================================
						x+= 4;
						continue;
					}
					PingSocket::step(10);
					DWORD ti = timer.GetTime();
					{
						char xxx[30];
						wsprintf(xxx, "pinged %i games", total);
						SetWindowText(kaillera_mslref,xxx);
					}
					for (int _x = 0; _x < plist.size(); _x++) {
						PingSocket * ps = plist.get(_x);
						if (ps != 0 && ps->limit != 0 && ps->is_done(ti)){
							//=================================================
							int ping = ps->return_ping();
							int bx = (int)ps->ptr;
							plist.remove(ps);
							delete ps;
							_x--;
							//=================================================
							if (ping == -1)
								continue;
							//=================================================
							char * name = list->items[bx];
							char xxxj[20];
							wsprintf(xxxj, "%ims", ping);
							//=================================================
							kaillera_mlv.AddRow(name, (int)name);
							int x = kaillera_mlv.Find((int)name);
							kaillera_mlv.FillRows(x, 4, list->items[bx+1], list->items[bx+2], xxxj, list->items[bx+3]);
							//=================================================							
							total++;
						}
					}
				}
				SetWindowText(kaillera_mslref,"Cleaning up...");
				while (plist.size() > 0){
					PingSocket::step(10);
					DWORD ti = timer.GetTime();
					for (int _x = 0; _x < plist.size(); _x++) {
						PingSocket * ps = plist.get(_x);
						if (ps != 0 && ps->limit != 0 && ps->is_done(ti)){
							//=================================================
							int ping = ps->return_ping();
							int bx = (int)ps->ptr;
							plist.remove(ps);
							delete ps;
							_x--;
							//=================================================
							if (ping == -1)
								continue;
							//=================================================
							char * name = list->items[bx];
							char xxxj[20];
							wsprintf(xxxj, "%ims", ping);
							//=================================================
							kaillera_mlv.AddRow(name, (int)name);
							int x = kaillera_mlv.Find((int)name);
							kaillera_mlv.FillRows(x, 4, list->items[bx+1], list->items[bx+2], xxxj, list->items[bx+3]);
							//=================================================
							total++;
						}
					}
				}
				//=============================================================
				spcharlist_delete(list);
				//=============================================================
				kaillera_mslistColumnOrder[3] = 0;
				kaillera_mslistSort(3);
				char xxx[40];
				wsprintf(xxx, "%i waiting games found", total);
				SetWindowText(kaillera_mslref,xxx);
				//=============================================================
			} else {
				SetWindowText(kaillera_mslref,"No waiting games on the list");
			}
		}
	}

	void run(){
		runn = running = true;
		__try {
			//=====================================================================
			SetWindowText(kaillera_mslref,"Downloading list...");
			SetDlgItemText(kaillera_mslrefx, BTN_REFRESH, "Stop");
			kaillera_mlv.DeleteAllRows();
			memset(buffer, 0, 0x7FFF);
			//=====================================================================
			refresh_p2p_wgames();
			//=====================================================================
		} __except(n02ExceptionFilterFunction(GetExceptionInformation())) {
			running = false;
			eend();
			return;
		}
		running = false;
		SetDlgItemText(kaillera_mslrefx, BTN_REFRESH, "Refresh");
	}
	void start(){
		if (!running)
			create();
		else 
			eend();
	}
	void eend(){
		if (running)
			destroy();
		running = runn = false;
		while (plist.size() > 0){
			delete plist.get(0);
			plist.removei(0);
		}
		SetDlgItemText(kaillera_mslrefx, BTN_REFRESH, "Refresh");
	}
} refresher_thread;

//////////////////////////////////////////////////
////void KLSListAdd(char * name, char * host);

void OKAI_CCNV p2p_connect_peer(char * hostname);

void p2p_wgdlg_connect_selected(HWND hDlg){
	refresher_thread.runn = false;
	int selected = kaillera_mlv.SelectedRow();
	if (selected >= 0) {
		char * buf = (char*)kaillera_mlv.RowNo(selected);
		char * name = buf;
		while(*++buf!= 0){}*buf++ = 0;
		char * emu = buf;
		while(*++buf!= 0){}*buf++ = 0;
		char * usern = buf;
		while(*++buf!= 0){}*buf++ = 0;
		char * host = buf;
		while(*++buf!=0){}*buf++ = 0;
		ShowWindow(hDlg, SW_HIDE);
		p2p_connect_peer(host);
		ShowWindow(hDlg, SW_SHOW);
	}
}

int CALLBACK p2p_WaitingGamesListProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static bool xxx = false;
	switch (uMsg) {
		case WM_INITDIALOG:
			kaillera_mslref = GetDlgItem(kaillera_mslrefx =hDlg, IDC_STATS);
			
			refresher_thread.running = false;
			refresher_thread.ltype = lParam;
			kaillera_mlv.handle = GetDlgItem(hDlg, LV_SLIST);
			kaillera_mlv.FullRowSelect();

			SetWindowText(kaillera_mslref, "waiting games... press refresh to download and ping list");
			kaillera_mlv.AddColumn("Game", 290);
			kaillera_mlv.AddColumn("Emu", 190);
			kaillera_mlv.AddColumn("user", 99);
			kaillera_mlv.AddColumn("ping", 50);
			kaillera_mlv.AddColumn("host", 0);

			//refresher_thread.start();
			xxx = false;
			break;
		case WM_DESTROY:
			refresher_thread.eend();
			break;
		case WM_NOTIFY:
			switch (((NMHDR*)lParam)->code) {
				case NM_DBLCLK:
				if(((NMHDR*)lParam)->hwndFrom == kaillera_mlv.handle){
					p2p_wgdlg_connect_selected(hDlg);
					return TRUE;					
				}
				break;
			}
			if(((LPNMHDR)lParam)->code==LVN_COLUMNCLICK && ((LPNMHDR)lParam)->hwndFrom==kaillera_mlv.handle){
				kaillera_mslistSort(((NMLISTVIEW*)lParam)->iSubItem);
			}
			break;
		case WM_COMMAND:
			if (LOWORD(wParam)==BTN_CONNECT){
				p2p_wgdlg_connect_selected(hDlg);
			} else if (LOWORD(wParam)==BTN_REFRESH) {
				refresher_thread.start();
			}
			break;
		case WM_APP + 11:
		{
			//activate
			break;
		}
		case WM_APP:
		{
			refresher_thread.eend();
			//deactivate
			break;
		}
		case WM_APP + 5:
		{
			RECT r = *((LPRECT)wParam);
			SetWindowPos(hDlg, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER);

			//convert to local coordinates i guess
			r.bottom -= r.top; r.top = 0;
			r.right -= r.left; r.left = 0;

			SetWindowPos(kaillera_mlv.handle, 0, 0, 0, r.right - 20, r.bottom - 70, SWP_NOZORDER|SWP_NOMOVE);
			SetWindowPos(GetDlgItem(hDlg, BTN_REFRESH), 0, 10, r.bottom - 32, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
			SetWindowPos(GetDlgItem(hDlg, BTN_CONNECT), 0, 90, r.bottom - 32, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
			SetWindowPos(GetDlgItem(hDlg, IND_INCDEAD), 0, 90 + 80, r.bottom - 31, 0, 0, SWP_NOZORDER|SWP_NOSIZE);

		}
		break;
		break;
	};
	return 0;
}

void p2p_ShowWaitingGamesList(HWND parent){
	DialogBoxParam(hx, (LPCTSTR)P2P_SDLG_WLIST, parent, (DLGPROC)p2p_WaitingGamesListProc, 2);
}

#pragma optimize("", on)
