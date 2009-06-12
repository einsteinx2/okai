#include "okaimodules.h"

//recorder
static int  OKAI_CCNV recorderInit(char * game, int playerno, int totalPlayers, char * filename, bool fNameReturn){
	return 1;
}
static int   OKAI_CCNV recorderAddData(const void * value, const int len){
	return 1;
}
static int   OKAI_CCNV recorderAddValues(const void * value, const int len){
	return 1;
}
static int   OKAI_CCNV recorderAddChat(const char * username, const char * chat){
	return 1;
}
static void  OKAI_CCNV recorderSetComment(const char * comment){
}
static int   OKAI_CCNV recorderAddDrop(const char * username, int playerNo){
	return 1;
}
static void  OKAI_CCNV recorderTerm(){
}

recorderInterface recorder = 
{
	recorderInit,
	recorderAddData,
	recorderAddValues,
	recorderSetComment,
	recorderAddChat,
	recorderAddDrop,
	recorderTerm
};

static bool recorderLoaded;
bool isRecorderLoaded(){
	return recorderLoaded;	
}

void recorderReset(){//resets recorder interface
	recorderLoaded = false;
	recorder.recorderInit = recorderInit;
	recorder.recorderAddData = recorderAddData;
	recorder.recorderAddValues = recorderAddValues;
	recorder.recorderAddChat = recorderAddChat;
	recorder.recorderSetComment = recorderSetComment;			
	recorder.recorderAddDrop = recorderAddDrop;
	recorder.recorderTerm = recorderTerm;
}

void recorderCheck(){//resets recorder interface
	okaiServiceModule orc;
	recorderReset();

	if (getServiceModule("recorder", &orc)==0) {
		KDBFC(recorder module found);
		orc.getServiceModuleInterface(&recorder);
		recorderLoaded = true;
	}
}
