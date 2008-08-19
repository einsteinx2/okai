#include "common.h"
using namespace n02;

int portNo;
CONFIG_START(ssrvConfig)
CONFIG_INTVAR("port", portNo, 27887)
CONFIG_END;

		SocketAddress lastAddress;

class SSRVSocket : public UdpSocket {



public:
	SSRVSocket(int prt) : UdpSocket(prt) {


	}

	void dataArrivalCallback(){
		int bufLen = BIG_RECV_BUFFER_SIZE;



		if (BsdSocket::recvFrom(BsdSocket::bigRecvBuffer, bufLen, lastAddress)) {
//LOG(%s:%i, __FILE__, __LINE__);
			if (bufLen > 0) {
//LOG(%s:%i, __FILE__, __LINE__);
				if (bigRecvBuffer[0]==0) {
//LOG(%s:%i, __FILE__, __LINE__);
					if (strcmp(bigRecvBuffer+1, "WHATISMYIP")==0) {
//LOG(%s:%i, __FILE__, __LINE__);
						char response[1024];
						response[0] = 0;
						sprintf(response+1, "IPIS: Your IP is %s", lastAddress.toString());
						sendTo(response, 2 +strlen(response+1), lastAddress);
LOG(IP Request from %s, lastAddress.toString());
					}
				} else {
					LOG(Invalid packet %s from %s, bigRecvBuffer, lastAddress.toString());
				}
				return;
			}
		}
		LOG(Socket Error occured %i, errno);
	}
} * connection;





int main(int /*argc*/, char * /*args*/[]) {
	// initialize commons
	commonInitialize();

	LOGBASICS("*******************************************************************************");
	LOGBASICS("*******************************************************************************");
	LOGBASICS("p2p ssrv Copyright (c) Open Kaillera Team 2003-2008");
	LOGBASIC ("Built on %s %s", __DATE__, __TIME__);
	// Load config
	{
		ConfigurationManager config(ssrvConfig);
		config.load("config");		
	}

	connection = new SSRVSocket(portNo);

	if (connection != 0) {
		if (connection->isInitialized()) {
			while (true != false) {
				BsdSocket::step(1, 0);
			}
		}
		delete connection;
	}

	LOGBASICS("Cleaning up");
	// save configs
	{
		ConfigurationManager config(ssrvConfig);
		config.save("config");		
	}
	// terminate commons
	commonTerminate();
	return 0;
}













// n02 commons callbacks
const char * n02::n02GetName() {
	return "ssrv";
}

