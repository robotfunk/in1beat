#ifndef _KEYBOARD_RECORDER
#define _KEYBOARD_RECORDER

#include "ofMain.h"

class keyboardRecorder {
public:
	keyboardRecorder();
	void start();
	void stop();
	void recordPress(char key);
	void clear();
	void setCountInOffset(int ms);

	vector<pair<int, string> > & getRecordedPresses();	
	void printPresses();
private:
	bool active;
	vector<pair<int, string> > recordedPresses;
	int startTime;
	int countInOffset;
};

#endif