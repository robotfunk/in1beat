#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,688, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofSetDataPathRoot("./");

	ofRunApp( new in1klap());

}