#include "metronome.h"
metronome::metronome() : bpm(120), flipFlop(0), samplePos(0),lengthOfOneBeatInSamples(0), active(false){
	recalculateBeatLengths();
	tick.loadSound("metronome2.aif");
	tock.loadSound("metronome1.aif");
}

void metronome::start(){
	samplePos=0;
	beatCounter=0;
	active=true;
	tickGo=false;
	tockGo=false;
}

bool metronome::isActive(){
	return(active);
}

void metronome::stop(){
	active=false;
}

// this gets called from the testapp audio requested method (ie. once per frame)
void metronome::update(){
	if (!active || lengthOfOneBeatInSamples==0){
		return;
	}
	for(int i = 0; i!=128; i++) {
		if(samplePos % (lengthOfOneBeatInSamples*4) == 0.0) {
			tickGo=true; // resets the tick sound to the beginning, so that its audio will be added to the output
		}
		else if(samplePos % lengthOfOneBeatInSamples == 0.0) {
			tockGo=true; // resets the tock sound to the beginning, so that its audio will be added to the output
		}
		samplePos++; // this gets incremented with every sample
	}
	if(tickGo){
		if(beatCounter>35){
			stop();	
			return;
		};
		tick.play();
		tickGo=false;
		flipFlop=!flipFlop;
		beatCounter++;
		std::cout<<"Beatcounter="<<beatCounter<<std::endl;

	}
	else if(tockGo){
		if(beatCounter>35){
			stop();	
			return;
		};
		tock.play();
		tockGo=false;
		flipFlop=!flipFlop;
		beatCounter++;
		std::cout<<"Beatcounter="<<beatCounter<<std::endl;

	}


}

void metronome::recalculateBeatLengths(){
	int samplespersecond=44100;
	double beatspersecond=bpm/60.0; // 2
	double secondsperbeat=1/beatspersecond; // 0.5
	lengthOfOneBeatInSamples=secondsperbeat*samplespersecond;
}

bool metronome::getFlipFlop(){
	return(flipFlop);
}
int metronome::getBeatNumber(){
	return(beatCounter);
}

void metronome::setBpm(int targetBpm){
	bpm=targetBpm;
	recalculateBeatLengths();
}

int metronome::getBpm(){
	return(bpm);
}

int metronome::lengthOf16thInMs(){
	return(((lengthOfOneBeatInSamples/4.0)/44100.0)*1000.0);
}

