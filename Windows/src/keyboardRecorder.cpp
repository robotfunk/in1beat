/*
 *  keyboardRecorder.cpp
 *  openFrameworks
 *
 *  Created by mormo on 10/8/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "keyboardRecorder.h"

keyboardRecorder::keyboardRecorder() : active(false) {
	vector<pair<int, char> > recordedPresses;
	elapsedMillis =0;
	audioTimerCount =0;
	oldTime = ofGetSystemTime();
	maxTime =0;
}

void keyboardRecorder::start(){
	clear();
	startTime=ofGetElapsedTimeMillis();
	active=true;

}
void keyboardRecorder::start(int ms){
	clear();
	startTime=ms;
	active=true;

}
void keyboardRecorder::stop(){
	active=false;
}

void keyboardRecorder::clear(){
	recordedPresses.clear();
}

void keyboardRecorder::setCountInOffset(int ms){
	countInOffset=ms;
}

void keyboardRecorder::recordPress(char key){
	if(!active){
		return;
	}
	pair<int, string> myPair;
	int pos=(ofGetElapsedTimeMillis()-startTime)-countInOffset;
// pos=(elapsedMillis)-countInOffset;//hack
	myPair.first=pos;
	myPair.second=key;
	recordedPresses.push_back(myPair);
	cout << "Press recorded: (key) " << pos <<", "<<key<< endl;
}

void keyboardRecorder::recordPress(char key, int ms){
	if(!active){
		return;
	}
	pair<int, string> myPair;
	//int pos=(ofGetElapsedTimeMillis()-startTime)-countInOffset;
	int pos=(ms-startTime)-countInOffset;//hack
	myPair.first=pos;
	myPair.second=key;
	recordedPresses.push_back(myPair);
	cout << "Press recorded: (key, ms) " << pos <<", "<<key<< endl;
}
vector< pair<int, string> > & keyboardRecorder::getRecordedPresses(){
	//printPresses();
	return(recordedPresses);
}
void keyboardRecorder::audioTimer()
{
    audioTimerCount +=128;
    elapsedMillis = (int) (audioTimerCount / 44.100f);
    /*
    int curTime = ofGetSystemTime();
    int deltaTime = curTime - oldTime;
  //  cout << deltaTime<< " "<<maxTime<<"\n";

    maxTime = max(deltaTime, maxTime);
    oldTime = curTime;
    elapsedMillis += deltaTime;
    if (elapsedMillis< 5000) maxTime =0; // lazy reset to ignore hiccup on startup
        //elapsedMillis = (audioTimerCount*128)/44100;
        */
}
void keyboardRecorder::printPresses(){
	vector< pair<int, string> >::iterator rpi;
	for(rpi=recordedPresses.begin();rpi!=recordedPresses.end();rpi++){
		cout << "Press item. " << ofToString(rpi->first) << ", " << rpi->second << "" << endl;
	}
}
