#include "ofMain.h"
//#include "testApp.h"
#include "in1klap.h"
#include "ofAppGlutWindow.h"
#include "ofxArgs.h"
//========================================================================
int main(int argc, char *argv[] ){
    ofxArgs* args = new ofxArgs(argc, argv);
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,688, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofSetDataPathRoot("");
	//ofSetDataPathRoot("C:/of_preRelease_v0.06_win32_cb_FAT/apps/examples/in1klap/bin/data/");
//ofSetDataPathRoot("\\bin\data\\");
//ofSetDataPathRoot("\\");
	ofRunApp( new in1klap(args));
    delete args;
}
