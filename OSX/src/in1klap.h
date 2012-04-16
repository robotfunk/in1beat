#ifndef _TEST_APP
#define _TEST_APP


#include <fstream>
#include <numeric>
#include "ofxThread.h"
#include "metronome.h"
#include "keyboardRecorder.h"
#include "ofMain.h"

class in1klap : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed  (int key);	
		void audioRequested(float *output, int bufferSize, int numChannels);
	void testCalculate();
	
	pair<string, double> calculateResult(const vector<pair<int, string> >& expected, vector<pair<int, string> >& recieved, const vector<int>& beatvector, bool anyKey);
	vector<pair<int, string> > getExpectedVector(int sequenceNumber,int bpm, bool notes);
	vector<int> getExpectedBeatVector(int sequenceNumber, bool notes);
	void compensate(vector<pair<int, string> >& received, const vector<pair<int, string> >& expected);
	vector<string> getPosOfNearestReceived(double sourcepos, const vector<pair<int, string> >& received, const vector<pair<int, string> >& expected);
	void drawEventVectors(const vector<pair<int, string> >& expectedVector, const vector<pair<int, string> >& receivedVector);

	float toFloat(string str);

	bool detectExtraNote(double position_of_previous_match, double match_position, vector<pair<int, string> >& received);
	double getMedian(vector<double> const input);
	void drawCountIn(int x,int y);
	void drawMetronome(int x,int y);

	void drawText();
	void nextAppState();	
	void startTheTest();	
	
	void subsChars(string & origString);

		
	metronome metro;
	keyboardRecorder recorder;
	int testStartTime;
	vector<vector< string > > sequences;
	vector<pair<int, string> > expectedVector;
	vector<pair<int, string> > receivedVector;
	
	int appState;
	int appTextLeft;
	int appTextTop;
	int imageMargin;
	int chosenBpm;
	int chosenSequence;
	int chosenTestStyle;
	ofImage allSequencesImage;
	ofImage logo;
	ofImage noteskey;
	vector<ofImage> sequenceImages;
	int currentBeat;
	string accuracy;
	string precision;
	string precisionLabel;
	
	ofTrueTypeFont mainFont;
	string translationFileName;
	vector<string> translationLines;

};

//typedef struct{
//	string character;
//	string code;
//}charSubstitution;

#endif
