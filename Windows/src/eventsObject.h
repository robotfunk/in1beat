/*
 * ofxEventsAddon.h
 *
 *  Created on: 17-dic-2008
 *      Author: art
 */

#ifndef OFXEVENTSADDON_H_
#define OFXEVENTSADDON_H_

#include "ofMain.h"
#include "ofEvents.h"
//#include "myEventArgs.h"

class eventsObject{
	
public:
	eventsObject(){
	}
	ofEvent<float>				newFloatEvent;
	ofEvent<int>				newIntEvent;
	
private:
	
};
#endif /* OFXEVENTSADDON_H_ */
