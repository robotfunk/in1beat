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
}

void keyboardRecorder::start(){
	clear();
	startTime=ofGetElapsedTimeMillis();
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
	myPair.first=pos;  
	myPair.second=key;
	recordedPresses.push_back(myPair);
	//cout << "Press recorded: " << pos << endl;
}

vector< pair<int, string> > & keyboardRecorder::getRecordedPresses(){
	//printPresses();
	return(recordedPresses);
}

void keyboardRecorder::printPresses(){
	vector< pair<int, string> >::iterator rpi;
	for(rpi=recordedPresses.begin();rpi!=recordedPresses.end();rpi++){
		cout << "Press item. " << ofToString(rpi->first) << ", " << rpi->second << "" << endl;
	}
}