#include "okaimodules.h"

#include "errr.h"

// -------------------------------------------------------------------------
// --  Reel stuff --
int AutoGameMode = 0; //OKAI_GMODE_2PR | OKAI_GMODE_2PS | OKAI_GMODE_8PR | OKAI_GMODE_8PS | OKAI_GMODE_PLAYBACK
int AutoGameDelayMode = 0; // OKAI_AGDM_TRAD | OKAI_AGDM_ZERO | OKAI_AGDM_VARI
bool AutoGameLoaded = false;
int AutoGamePlayerIndex; // index of the player (0 - 7)
int AutoGamePlayerDelay; // delay
int AutoGamePlayerFPTR;
int AutoGameFrameNo = 0;
int AutoGamePredComplete = 0;

#define MAX_AUTO_RESIM 32

typedef struct {

	typedef struct {
		char bytes[16 * MAX_AUTO_RESIM];
	} okaiInput;

	okaiInput reserve;
	okaiInput inputs[MAX_AUTO_RESIM];

	int base;

	void * GetInput(int offset, int slot) {
		int ind = offset + base;
		ind %= MAX_AUTO_RESIM;
		return &inputs[ind].bytes[slot * okai->autoInputSize];
	}

	void * GetReserveInput() {
		return reserve.bytes;
	}

	void * GetReserveInputSlot(int slot) {
		return &reserve.bytes[slot * okai->autoInputSize];
	}
	
	void AdvanceInput() {
		base = (base + 1) % MAX_AUTO_RESIM;
	}

} okaiAutoFrameInputCollection;

okaiAutoFrameInputCollection okaiAuto;



inline void AutoRunFrameDelay(int drawFrame, int final = 1) {
	okai->callbacks.saveStateCallback(AutoGamePlayerFPTR);
	for (int x = 0; x < numplayers; x++) {
		okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(AutoGamePlayerFPTR, x), x);
	}
	AutoGamePlayerFPTR++;
	okai->callbacks.runFrameCallback(drawFrame? OKAI_RUNFRAME_DRAW : OKAI_RUNFRAME_SILENT, final);
}

inline void AutoRunFrameEnd(){
	AutoGameLoaded = false;

	if (isRecorderLoaded()){	
		int secx = AutoGameFrameNo / 60;
		char xxx[30];
		int hrs = secx/3600;
		secx %= 3600;
		wsprintf(xxx, "%02i:%02i:%02i", hrs, secx/ 60, secx%60);
		recorder.recorderSetComment(xxx);
	}

	okaiGameEndCallback();

}

void runFrameAutoTRAD(int drawFrame) {

	if (AutoGameMode != OKAI_BMODE_PB) {
		{
			//send our input first
			void * inpl = okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex);
			memset(inpl, 0, okai->autoInputSize);
			okai->callbacks.getPlayerInputCallback(inpl);
			okai->functions.sendPlayValue(inpl, okai->autoInputSize);

		}

		int r = okai->functions.recvPlayValues(okaiAuto.GetReserveInput(), okai->autoInputSize);
		if (r == 0) {
			n02_TRACE();
			
			// we dont want to run the delay frames koz it'll
			AutoGamePlayerFPTR++;

			//run blank frame
			okai->callbacks.runFrameCallback(OKAI_RUNFRAME_BLANK, 1);
			return;
		} else if (r == -1) {
			n02_TRACE();
			AutoRunFrameEnd();
			return;
		} else {
			n02_TRACE();

			//recorder
			recorder.recorderAddValues(okaiAuto.GetReserveInput(), okai->autoInputSize << 1);

			//set inputs
			for (int x = 0; x < numplayers; x++) {
				okai->callbacks.setPlayerInputCallback(okaiAuto.GetReserveInputSlot(x), x);
			}

			//run frame
			okai->callbacks.runFrameCallback(drawFrame? OKAI_RUNFRAME_DRAW : OKAI_RUNFRAME_SILENT, 1);

		}

	} else {
		n02_TRACE();
		int r = okai->functions.recvPlayValues(okaiAuto.GetReserveInput(), okai->autoInputSize);
		if (r==-1) {
			AutoRunFrameEnd();
			return;
		} else {
			//set inputs
			for (int x = 0; x < numplayers; x++) {
				okai->callbacks.setPlayerInputCallback(okaiAuto.GetReserveInputSlot(x), x);
			}
			//run frame
			okai->callbacks.runFrameCallback(drawFrame? OKAI_RUNFRAME_DRAW : OKAI_RUNFRAME_SILENT, 1);
		}

	}
}
void runFrameAutoZERO(int drawFrame) {
	n02_TRACE();
	{
		//send our input first
		void * inpl = okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex);
		memset(inpl, 0, okai->autoInputSize);
		okai->callbacks.getPlayerInputCallback(inpl);
		okai->functions.sendPlayValue(inpl, okai->autoInputSize);
	}

	int r = okai->functions.recvPlayValues(okaiAuto.GetReserveInput(), okai->autoInputSize);
	
	//firstly, make a prediction of all the inputs
	for (int x = 0; x < numplayers; x++) {
		//there is no need to predict my own input :P
		if (x!=AutoGamePlayerIndex) {
			if (AutoGamePlayerFPTR == 0) {
				//first frame, take prediction from mine...it'll be 0000000000000s anyway
				memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex), okai->autoInputSize);
			} else {
				//get from their previous frame
				memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR-1, x), okai->autoInputSize);
			}
		}
	}

	if (r == -1) { //game not running any more
		AutoRunFrameEnd();
		return;
	} else if (r==0) {
		n02_TRACE();
		AutoRunFrameDelay(drawFrame, 1);
	} else {
		n02_TRACE();

		//recorder
		recorder.recorderAddValues(okaiAuto.GetReserveInput(), okai->autoInputSize << 1);

		// see if we predicted wrong, needs reemulation
		if (memcmp(okaiAuto.GetReserveInput(), okaiAuto.GetInput(0, 0), numplayers * okai->autoInputSize) != 0) {

			{ // update prediction
				for (int x = 0; x <= AutoGamePlayerFPTR; x++) {
					//copy my input's to there
					memcpy(okaiAuto.GetReserveInputSlot(AutoGamePlayerIndex), okaiAuto.GetInput(x, AutoGamePlayerIndex), okai->autoInputSize);
					//replace mine with reserve
					memcpy(okaiAuto.GetInput(x, 0), okaiAuto.GetReserveInput(), r);
				}
			}

			{ //reemulate to current frame
				//load our base state
				okai->callbacks.loadStateCallback(0);

				for (int x = 0; x < AutoGamePlayerFPTR; x++) {
					if (x != 0) {
						okai->callbacks.saveStateCallback(x);
					}
					for (int y = 0; y < numplayers; y++) {
						okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(x, y), y);
					}

					okai->callbacks.runFrameCallback(OKAI_RUNFRAME_SILENT, 0);
				}
			}
		}

		okai->callbacks.saveStateCallback(AutoGamePlayerFPTR);

		for (int y = 0; y < numplayers; y++) {
			okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(AutoGamePlayerFPTR, y), y);
		}

		okai->callbacks.deleteStateCallback();
		okaiAuto.AdvanceInput();

		okai->callbacks.runFrameCallback(drawFrame? OKAI_RUNFRAME_DRAW : OKAI_RUNFRAME_SILENT, 1);

	}

}

inline void AutoRunFrameResimMF(int drawFrame, int r) {

	//kprintf("  - AutoRunFrameResimMF AutoGamePlayerFPTR = %i", AutoGamePlayerFPTR);
	//prediction was false;
	if (memcmp(okaiAuto.GetReserveInput(), okaiAuto.GetInput(0, 0), numplayers * okai->autoInputSize) != 0) { //we predicted wrong, needs reemulation

		//kprintf("   - prediction wrong");

		{ // update prediction
			for (int x = 0; x <= AutoGamePlayerFPTR; x++) {
				//copy my input's to there
				memcpy(okaiAuto.GetReserveInputSlot(AutoGamePlayerIndex), okaiAuto.GetInput(x, AutoGamePlayerIndex), okai->autoInputSize);
				//replace mine with reserve
				memcpy(okaiAuto.GetInput(x, 0), okaiAuto.GetReserveInput(), r);
			}
		}

		{ //reemulate to current frame
			//load our base state
			okai->callbacks.loadStateCallback(0);

			//kprintf("   -- load base");

			for (int x = 0; x < AutoGamePlayerFPTR; x++) {
				if (x != 0) {
					okai->callbacks.saveStateCallback(x);
					//kprintf("   -- save %i", x);
				}
				for (int y = 0; y < numplayers; y++) {
					okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(x, y), y);
				}
				//kprintf("   -- run");
				okai->callbacks.runFrameCallback(OKAI_RUNFRAME_SILENT, 0);
			}
		}
	}

	//recorder
	recorder.recorderAddValues(okaiAuto.GetReserveInput(), okai->autoInputSize << 1);

	okai->callbacks.deleteStateCallback();
	okaiAuto.AdvanceInput();
	AutoGamePlayerFPTR--;
}
void runFrameAutoVARI(int drawFrame) {

	// ok lets start by sending data
	n02_TRACE();
	{
		//send our input first
		void * inpl = okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex);
		memset(inpl, 0, okai->autoInputSize);
		okai->callbacks.getPlayerInputCallback(inpl);
		okai->functions.sendPlayValue(inpl, okai->autoInputSize);
	}

	//now update all our predictions
	activeMod.playDataCheck(0);

	if (activeMod.getLocalQueueLen() > 0){

		int r = okai->functions.recvPlayValues(okaiAuto.GetReserveInput(), okai->autoInputSize);

		//firstly, make a prediction of all the inputs
		for (int x = 0; x < numplayers; x++) {
			//there is no need to predict my own input :P
			if (x!=AutoGamePlayerIndex) {
				if (AutoGamePlayerFPTR == 0) {
					//first frame, take prediction from mine...it'll be 0000000000000s anyway
					memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex), okai->autoInputSize);
				} else {
					//get from their previous frame
					memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR-1, x), okai->autoInputSize);
				}
			}
		}

		if (r == -1) { //game not running any more
			AutoRunFrameEnd();
			return;
		} else if (r==0) {
			n02_TRACE();
			okai->callbacks.saveStateCallback(AutoGamePlayerFPTR);

			for (int x = 0; x < numplayers; x++) {
				okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(AutoGamePlayerFPTR, x), x);
			}

			AutoGamePlayerFPTR++;

			okai->callbacks.runFrameCallback(OKAI_RUNFRAME_DRAW, 1); // will implement autoframeskip later

		} else {
			n02_TRACE();

			//recorder
			recorder.recorderAddValues(okaiAuto.GetReserveInput(), okai->autoInputSize << 1);

			// see if we predicted wrong, needs reemulation
			if (memcmp(okaiAuto.GetReserveInput(), okaiAuto.GetInput(0, 0), numplayers * okai->autoInputSize) != 0) {

				{ // update prediction
					for (int x = 0; x <= AutoGamePlayerFPTR; x++) {
						//copy my input's to there
						memcpy(okaiAuto.GetReserveInputSlot(AutoGamePlayerIndex), okaiAuto.GetInput(x, AutoGamePlayerIndex), okai->autoInputSize);
						//replace mine with reserve
						memcpy(okaiAuto.GetInput(x, 0), okaiAuto.GetReserveInput(), r);
					}
				}

				{ //reemulate to current frame
					//load our base state
					okai->callbacks.loadStateCallback(0);

					for (int x = 0; x < AutoGamePlayerFPTR; x++) {
						if (x != 0) {
							okai->callbacks.saveStateCallback(x);
						}
						for (int y = 0; y < numplayers; y++) {
							okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(x, y), y);
						}

						okai->callbacks.runFrameCallback(OKAI_RUNFRAME_SILENT, 0);
					}
				}
			}

			okai->callbacks.saveStateCallback(AutoGamePlayerFPTR);

			for (int y = 0; y < numplayers; y++) {
				okai->callbacks.setPlayerInputCallback(okaiAuto.GetInput(AutoGamePlayerFPTR, y), y);
			}

			okai->callbacks.deleteStateCallback();
			okaiAuto.AdvanceInput();

			okai->callbacks.runFrameCallback(drawFrame? OKAI_RUNFRAME_DRAW : OKAI_RUNFRAME_SILENT, 1);

		}

	}


	// that was a good start
	
}

/*

void AutoGameUseVariableDelayRun(int drawFrame, bool inRecursion = false){
bool SendDataOnThisFrame = true;

int bd = activeMod.getBaseDelay();
if (AutoGamePlayerFPTR > bd) {
bd += 3;

bd = min(bd, okai->appMaxRsim-1);

if (AutoGamePlayerFPTR > bd) {

SendDataOnThisFrame = false;

//kprintf("- No data but too much delay, gotta control it");

}
}

{
//send our input first
void * inpl = okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex);
memset(inpl, 0, okai->autoInputSize);
okai->callbacks.getPlayerInputCallback(inpl);
okai->functions.sendPlayValue(inpl, okai->autoInputSize);

}

if (!inRecursion)
okai->functions.playDataCheck(2);

//int counter = 1;
// get frame data
//while (okai->functions.getLocalQueueLen() == 0 && counter-->0) {
//	okai->functions.playDataCheck();		
//}

if (okai->functions.getLocalQueueLen() > 0 || !SendDataOnThisFrame) {
//kprintf("- Data in queue, we resim %i", !SendDataOnThisFrame);

StartOfMassacare:

do {

int r = okai->functions.recvPlayValues(okaiAuto.GetReserveInput(), okai->autoInputSize << 1);

//kprintf(" - it returns %i", r);

//firstly, make a prediction of all the inputs
for (int x = 0; x < numplayers; x++) {
//there is no need to predict my own input :P
if (x!=AutoGamePlayerIndex) {
if (AutoGamePlayerFPTR == 0) {
//first frame, take prediction from mine...it'll be 0000000000000s anyway
memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR, AutoGamePlayerIndex), okai->autoInputSize);
} else {
//get from their previous frame
memcpy(okaiAuto.GetInput(AutoGamePlayerFPTR, x), okaiAuto.GetInput(AutoGamePlayerFPTR-1, x), okai->autoInputSize);
}
}
}

if (r == -1) { //game not running any more
//kprintf("  - time to end this torment");
AutoRunFrameEnd();
return;
} else if (r==0) {
//kprintf("  - AutoRunFrameDelay & return AutoGamePlayerFPTR = %i", AutoGamePlayerFPTR);
AutoRunFrameDelay(drawFrame);
return;
} else {
AutoRunFrameResimMF(drawFrame, r);
}

if (!inRecursion)
okai->functions.playDataCheck(0);

} while (okai->functions.getLocalQueueLen() != 0 && AutoGamePlayerFPTR > 0);

//kprintf(" - AutoRunFrameDelay AutoGamePlayerFPTR = %i", AutoGamePlayerFPTR);
AutoRunFrameDelay(drawFrame);

//if (!inRecursion && okai->functions.getLocalQueueLen() > 1)
//AutoGameUseVariableDelayRun(drawFrame, true);
//goto StartOfMassacare;


} else {
if (SendDataOnThisFrame) {
//kprintf("- No data we make delay");
AutoRunFrameDelay(drawFrame);
}
}
//kprintf("- AutoGameUseVariableDelayRun out");

}*/


/*

*/
typedef void (* RFAT)(int drawFrame);

RFAT runFrameAutoFcns [3] =
{
	runFrameAutoTRAD,
	runFrameAutoZERO,
	runFrameAutoVARI
};

void OKAI_CCNV runFrameAuto(int drawFrame) {

	n02_TRACE();

	AutoGameFrameNo++;

	if (!AutoGameLoaded) {
		AutoRunFrameEnd();
		return;
	}

	runFrameAutoFcns[AutoGameDelayMode](drawFrame);
}

