#ifndef _METRONOME
#define _METRONOME

#include "ofMain.h"

class metronome {
public:
	metronome();
	void start();
	void stop();
	void update();

	void setBpm(int targetBpm);
	int getBpm();
	bool getFlipFlop();
	int getBeatNumber();
	void audioRequested(float *output, int bufferSize, int numChannels);
	int lengthOf16thInMs();
	bool isActive();

private:
	void recalculateBeatLengths();
	int lengthOfOneBeatInSamples;
	
	int bpm;
	bool flipFlop;
	int samplePos;
	
	bool tickGo;
	bool tockGo;
	
	bool active;
	int beatCounter;

	ofSoundPlayer tick;
	ofSoundPlayer tock;
};

#endif