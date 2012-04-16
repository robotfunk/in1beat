#ifndef _KEYBOARD_RECORDER
#define _KEYBOARD_RECORDER

#include "ofMain.h"

class keyboardRecorder {
public:
	keyboardRecorder();
	void start();
	void start(int ms);


	void stop();
	void recordPress(char key);
	void recordPress(char key, int ms);
	void clear();
	void setCountInOffset(int ms);
    int audioTimerCount;
    void audioTimer();
    int elapsedMillis;
    int oldTime, maxTime;
	vector<pair<int, string> > & getRecordedPresses();
	void printPresses();
private:
	bool active;
	vector<pair<int, string> > recordedPresses;
	int startTime;
	int countInOffset;
};

#endif
