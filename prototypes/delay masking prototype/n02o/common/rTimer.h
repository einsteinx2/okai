#include <mmsystem.h>

class rTimer {
protected:
	unsigned int baseTime;

public:

	//set time to read 0
	inline void ResetTime(){
		baseTime = timeGetTime();
	}

	inline unsigned int GetTime(){
		return timeGetTime() - baseTime;
	}

	inline void Adjust(unsigned int _time){

	}
};

