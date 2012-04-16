#include "in1klap.h"

//--------------------------------------------------------------
void in1klap::setup(){
	
	// Switch for exporting dutch or english version
	translationFileName="translations_en.txt";
	//translationFileName="translations_nl.txt";
	
	// NB. set correct name for app
	// Name for english app : "1 beat"
	// Name for dutch app : "In 1 klap"
	
	mainFont.loadFont("/Library/Fonts/Arial.ttf",10,true,true);
	ofEnableSmoothing();
	
	ofEnableAlphaBlending(); 
	appState=0;
	appTextTop=80;
	appTextLeft=220;
	imageMargin=20;
	currentBeat=0;
	ofSoundStreamSetup(1,0,this,44100,128,1);
	ofSetFrameRate(60);
	ifstream fin; //declare a file stream
	for(int i=0;i!=4;i++){
		
		fin.open(ofToDataPath("sequence_"+ofToString(i)+".txt").c_str() ); //open your text file
		vector<string> thisSequence;
		if(fin==NULL){
			std::cout << "Failed to open sequence:  " << ofToDataPath("sequence_"+ofToString(i)+".txt").c_str() << std::endl;
		}
		while(fin!=NULL) //as long as theres still text to be read
		{
			string str; //declare a string for storage
			getline(fin, str); //get a line from the file, put it in the string
			if(str!=""){
				thisSequence.push_back(str); //push the string onto a vector of strings
			}
		}
		fin.close();
		sequences.push_back(thisSequence);
	}
	// each element in sequences is one sequence.
	// A sequence is stored as a vector of 32 strings.
	// Each string is a single line from the text file:
	// a comman delimited binary representation of notes in one quertar measure
	for(int i=0;i<4;i++){
		ofImage image;
		image.loadImage("sequence_"+ofToString(i)+".png");
		sequenceImages.push_back(image);
	}
	allSequencesImage.loadImage("testoverview.png");
	noteskey.loadImage("noteskey.png");
	logo.setImageType(OF_IMAGE_COLOR_ALPHA);
	logo.loadImage("logo.png");
	
	// Load translations
	fin.open(ofToDataPath(translationFileName).c_str() ); //open your text file
	if(fin==NULL){
		std::cout << "Failed to open translation file:  " << ofToDataPath(translationFileName).c_str() << std::endl;
	}
	while(fin!=NULL) //as long as there's still text to be read
	{
		string str; //declare a string for storage
		getline(fin, str); //get a line from the file, put it in the string
		if(str!=""){
			translationLines.push_back(str); //push the string onto a vector of strings
		}
	}
	fin.close();
}

//--------------------------------------------------------------
void in1klap::update(){
	if(appState==4){
		currentBeat=metro.getBeatNumber();
		if(!metro.isActive()){
			recorder.stop();
			vector<pair<int,string> >& p=recorder.getRecordedPresses();
			vector<pair<int,string> > e=getExpectedVector(chosenSequence,metro.getBpm(),chosenTestStyle==2);
			vector<int> b=getExpectedBeatVector(chosenSequence,chosenTestStyle==2);
			pair<string, double> r;
			expectedVector=e;
			receivedVector=p;
			r=calculateResult(e,p,b,chosenTestStyle==2);
			accuracy=r.first;
			precision=ofToString(r.second,2);
			if(r.second>60){
				precisionLabel=translationLines[22];//"Slecht";
			} else if(r.second>50){
				precisionLabel=translationLines[21];//"Minder dan gemiddeld";
			} else if(r.second>30){
				precisionLabel=translationLines[20];//"Gemiddeld";
			} else if(r.second>15){
				precisionLabel=translationLines[19];//"Goed";
			} else if(r.second<15){
				precisionLabel=translationLines[18];//"Zeer goed";
			}
			appState++;
		}
	}
}

//--------------------------------------------------------------
void in1klap::draw(){
	ofBackground(255,255,255);

	drawText();
	// drawEventVectors(expectedVector,receivedVector); for debugging only
	ofSetColor(230, 230, 230);
	ofRect(0, 0, 190, 1200);
	ofSetColor(0,0,0);
	mainFont.drawString("Esc: "+translationLines[1], 20,appTextTop+80); // Afsluiten
	mainFont.drawString("R: "+translationLines[0], 20,appTextTop+100); // Opnieuw beginnen
	ofSetColor(255,255,255);
	logo.draw(-16,0,160,160);

	ofSetColor(210, 210, 210);
	//mainFont.drawString("Client evaluation preview. Not for distribution. info@redshiftmedia.com", 20, 660);
}


void in1klap::drawText(){
	ofSetColor(0, 0, 0);
	if(appState==0){
		mainFont.drawString(translationLines[2], appTextLeft, appTextTop); // Kies \xE9\xE9n test met \xE9\xE9n numerieke toets
		ofSetColor(255, 255, 255);
		allSequencesImage.draw(appTextLeft,appTextTop+imageMargin);
	}
	else if (appState==1){
		mainFont.drawString(translationLines[3], appTextLeft, appTextTop); // Kies \xE9\xE9n tempo met \xE9\xE9n numerieke toets
		mainFont.drawString("1. Adagio (72 bpm)", appTextLeft, appTextTop+40);
		mainFont.drawString("2. Andante (96 bpm)", appTextLeft, appTextTop+60);
		mainFont.drawString("3. Allegro (126 bpm)", appTextLeft, appTextTop+80);

	}
	else if (appState==2){
		mainFont.drawString(translationLines[4], appTextLeft, appTextTop); // Kies het soort test met \xE9\xE9n numerieke toets
		mainFont.drawString("1.", appTextLeft, appTextTop+40);
		mainFont.drawString(translationLines[5], appTextLeft+15, appTextTop+40); //Vertaal de noten van de test in numerieke toetsen
		mainFont.drawString(translationLines[6], appTextLeft+15, appTextTop+60); //Sla op elke tel een numerieke toets aan. Kies steeds uit 0, 1, 2, 3 of 4.
		ofSetColor(255, 255, 255);

		noteskey.draw(appTextLeft+5,appTextTop+80,153,188);
		ofSetColor(0, 0, 0);

		mainFont.drawString("2.", appTextLeft, appTextTop+300);
		mainFont.drawString(translationLines[7], appTextLeft+15, appTextTop+300); //E\xE9n toetsaanslag per noot

	}
	else if (appState==3){
		mainFont.drawString(translationLines[23], appTextLeft, appTextTop); // Druk op \xE9\xE9n toets om het aftellen voor de test te laten beginnen
		ofSetColor(255, 255, 255);
		sequenceImages[chosenSequence].draw(appTextLeft,appTextTop+50);

	}
	else if (appState==4){
		if (currentBeat<5){
			mainFont.drawString(translationLines[8], appTextLeft, appTextTop);//"Aftellen"
			drawCountIn(appTextLeft+5,appTextTop+20);
		} else {
			mainFont.drawString(translationLines[9], appTextLeft, appTextTop);//"Bezig met de test"
			drawMetronome(appTextLeft+5,appTextTop+20);
		}
		ofSetColor(255, 255, 255);
		sequenceImages[chosenSequence].draw(appTextLeft,appTextTop+50);
	}
	else if (appState==5){
		ofSetColor(0, 0, 0);
		mainFont.drawString(translationLines[10], appTextLeft, appTextTop);//"Test voltooid"
		mainFont.drawString(translationLines[12] + ": " + accuracy, appTextLeft, appTextTop+40);//"Het aantal juiste aanslagen: "
		mainFont.drawString(translationLines[13] + ": " + precisionLabel +" ("+translationLines[17]+ ": " +precision + " ms)", appTextLeft, appTextTop+60); //"Timing van de juiste aanslagen: " Gemiddelde afwijking
		mainFont.drawString(translationLines[14], appTextLeft, appTextTop+100); //"Druk op A om de test opnieuw te doen"
		mainFont.drawString(translationLines[15], appTextLeft, appTextTop+120); //"Druk op R om \xE9\xE9n andere test te proberen"
		mainFont.drawString(translationLines[16], appTextLeft, appTextTop+140); //"Druk op Escape om af te sluiten"
	}
}

void in1klap::drawCountIn(int x, int y){
	int size=12;
	int spacing=3;
	for (int i=0;i<4;i++){
		int nx=(i*size)+((i*spacing)-5)+x;
		ofSetColor(0, 0, 0);
		if (i<currentBeat){
			ofSetColor(0, 0, 0);
		} else {
			ofSetColor(230, 230, 230);
		}
		ofFill();
		ofRect(nx, y, size, size);
	}
}
void in1klap::drawMetronome(int x, int y){
	int size=12;
	if (currentBeat%2!=0){
		ofSetColor(0, 0, 0);
	} else{
		ofSetColor(230, 230, 230);
	}
	ofFill();
	ofRect(x, y, size, size);
}
//--------------------------------------------------------------
void in1klap::nextAppState(){
	appState++;
}

void in1klap::keyPressed(int key){
	recorder.recordPress(key); // try it, will be stored if active
	if(key=='r'){
		recorder.stop();
		metro.stop();
		appState=0;
	}
	
	if(appState==0){
		if(key=='1'){
			chosenSequence=0;
			nextAppState();
		}
		if(key=='2'){
			chosenSequence=1;
			nextAppState();
		}
		if(key=='3'){
			chosenSequence=2;
			nextAppState();
		}
		if(key=='4'){
			chosenSequence=3;
			nextAppState();
		}
	}
	else if (appState==1){
		if(key=='1'){
			chosenBpm=72;
			nextAppState();
		}
		if(key=='2'){
			chosenBpm=96;
			nextAppState();
		}
		if(key=='3'){
			chosenBpm=126;
			nextAppState();
		}
	}
	else if (appState==2){
		// choose a kind of test
		if(key=='1'){
			chosenTestStyle=1;
			nextAppState();
		}
		if(key=='2'){
			chosenTestStyle=2;
			nextAppState();
		}
	}
	else if (appState==3){
		// a key was pressed so start the test
		nextAppState();
		expectedVector.clear();
		receivedVector.clear();
		startTheTest();
	}
	else if (appState==4){
		// test is started, including count in
	}
	else if (appState==5){
		// test ended
		if(key=='a'){
			appState=3;
		}
	}
}
void in1klap::startTheTest(){
	metro.setBpm(chosenBpm);
	int offset=metro.lengthOf16thInMs()*16;
	recorder.setCountInOffset(offset);
	recorder.start();
	metro.start();
}

void in1klap::audioRequested(float *output, int bufferSize, int numChannels) {
	metro.update();
}

float in1klap::toFloat(string str) {
	stringstream stream(str);
	float result;
	stream >> result;
	return result;
}

pair<string, double> in1klap::calculateResult(const vector<pair<int, string> >& expected, vector<pair<int, string> >& received, const vector<int>& beatvector, bool anyKey){

	int beaterrors=0;
	vector<int>::const_iterator beatIt;
	vector<int>::size_type beatvectorSize=beatvector.size();

	int a[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	vector<int> passfail(a, a + 32);
	vector<int>::iterator passfailIt;
	int position_of_previous_match=0;
	

	vector<pair<int, string> >::const_iterator expectedIt;
	vector<pair<int, string> >::size_type expectedSize=expected.size();
	vector<pair<int, string> >::iterator receivedIt;
	vector<pair<int, string> >::size_type receivedSize=received.size();
	string lastReceivedKeycode=" ";
	int lastReceivedEventPos;
	vector<string> r;
	
	//std::cout << "expectedSize=" << expectedSize << std::endl;
	//std::cout << "beatvector size=" << beatvectorSize << std::endl;

	vector<double> timingDeviations;
	int receivedEventsInWindowCount;
	pair<string, double> ret;

	// After each test a median timing offset should be calculated, and applied to all received events.
	compensate(received, expected);
	
	beatIt=beatvector.begin();
	int prevbeat=0;
	bool firstnoteinmeasure=true;
	int measurecount=0;
//	The system looks for the nearest received event (either in the past or future).
//	When found, the system checks whether the nearest event is nearer to a different expected event or not.
//	If not, the absolute timing difference between the expected and received events is stored in a vector.
//	If a different expected event was closer to the received event,
//	then the system ignores this expected event and skips to the next expected event.
	
	// for each expected note
	for(expectedIt=expected.begin();expectedIt!=expected.end();expectedIt++){

		r=getPosOfNearestReceived(expectedIt->first, received, expected); // get a match if possible
		bool match=(r[0]=="true");
		double matchpos=toFloat(r[1]);
		string matchkey=r[2];
		
		// check that the key matches, if necessary
		if(!anyKey){
			if(matchkey==expectedIt->second){
			} else{
				match=false;	
			}
		}
		   
		if(match){
			
			timingDeviations.push_back(fabs(matchpos-expectedIt->first));
			//std::cout<<"Match found in measure "<< ofToString(measurecount) << std::endl;
			// check if we have extra notes
			if(detectExtraNote(double(position_of_previous_match),matchpos, received)){
				//std::cout<<"Extra note found! "<< ofToString(measurecount) << std::endl;

				// there is an extra note,
				if(measurecount==0){
					// if this is the first measure, add the error to the current measure
					passfail[measurecount]=0;
				} else if(firstnoteinmeasure){
					// if the current expected not s the first on in this measure, add the error to the previous measure
					passfail[measurecount-1]=0;
				} else {
					// in normal cases add the error to the current measure
					passfail[measurecount]=0;
				}
			}
			position_of_previous_match=matchpos;
		} else {
			passfail[measurecount]=0;
		}
		
		prevbeat=*beatIt; // rotate the prevbeat variable
		beatIt++;
		
		// did we transition to the next quarter measure?
		if(*beatIt!=prevbeat){
			firstnoteinmeasure=true;
			measurecount++;
		} else {
			firstnoteinmeasure=false;			  
		}
	}
	
	// finally, check if there is a received event after the position of last match
	// if so, fail the last measure.
	if(double(received.back().first) > double(position_of_previous_match)){
		passfail.back()=0;
	}
	
//	for(passfailIt=passfail.begin();passfailIt!=passfail.end();passfailIt++){
//		std::cout<<"Passfail "<< ofToString(*passfailIt) << std::endl;
//	}
	
	//std::cout<<"timing deviations size="<<timingDeviations.size()<< std::endl;;
	vector<double>::iterator tIt;
	
	beaterrors=count(passfail.begin(), passfail.end(), 0);

	//double meanDeviation=std::accumulate(timingDeviations.begin(),timingDeviations.end(), 0.0)/timingDeviations.size();
	//std::cout<<"mean deviation="<<meanDeviation<< std::endl;
	string s=ofToString(32 - beaterrors)+"/32";
	ret.first=s;
	double meanDeviation=std::accumulate(timingDeviations.begin(),timingDeviations.end(), 0.0)/timingDeviations.size();
	ret.second=meanDeviation;
	return(ret);
}

bool in1klap::detectExtraNote(double position_of_previous_match, double match_position, vector<pair<int, string> >& received){
	vector<pair<int, string> >::iterator receivedIt;
	//std::cout<<"Checking for xtras between"<< ofToString(position_of_previous_match) << " and " << ofToString(match_position) << std::endl;
	for(receivedIt=received.begin();receivedIt!=received.end();receivedIt++){
		if((receivedIt->first > position_of_previous_match) && (receivedIt->first < match_position)){
			return true;
		}
	}
	return false;
}

vector<string> in1klap::getPosOfNearestReceived(double sourcepos, const vector<pair<int, string> >& received, const vector<pair<int, string> >& expected){
	vector<string> r;
	r.push_back("");
	r.push_back("");
	r.push_back("");
	vector<pair<int, string> >::const_iterator expectedIt, expectedIt2;
	vector<pair<int, string> >::size_type expectedSize=expected.size();
	vector<pair<int, string> >::const_iterator receivedIt;
	vector<pair<int, string> >::size_type receivedSize=received.size();
	
	bool found=false;
	double closestpos;
	string closestkey;
	double diff;
	
	// find closest received
	for(receivedIt=received.begin();receivedIt!=received.end();receivedIt++){
		double rabsdiff=fabs(receivedIt->first - sourcepos);
		if (!found || rabsdiff < fabs(closestpos-sourcepos)){
			closestpos=receivedIt->first;
			closestkey=receivedIt->second;
			found=true;
		}
	}
	
	// check whether closest pos is in fact closer to a different position in the expected array
	// if so, skip this element
	if(found){
		double diff=closestpos-sourcepos; //positive value indicates that received pos was later than expected
		double candidatecloseness=fabs(closestpos-sourcepos); 
		for(expectedIt2=expected.begin();expectedIt2!=expected.end();expectedIt2++){
			double diff2=expectedIt2->first - closestpos;
			
			// the candidate receives event was actually closer to another expected event, return false
			if(fabs(diff2) < fabs(diff)){
				r[0]="false";
				return(r);
			}
		}
		
		// no closer expected event was found, so return the match 
		r[0]="true";
		r[1]=ofToString(closestpos);
		r[2]=closestkey;
		return(r);
	} else {
		// no received items found
		r[0]="false";
		return(r);
	}
}


// auto-adjust the received vector to correct for latency
void in1klap::compensate(vector<pair<int, string> >& received, const vector<pair<int, string> >& expected){
	vector<double> differences;
	vector<pair<int, string> >::const_iterator expectedIt, expectedIt2;
	vector<pair<int, string> >::size_type expectedSize=expected.size();
	vector<pair<int, string> >::iterator receivedIt;
	vector<pair<int, string> >::size_type receivedSize=received.size();
	// for each item in expected, find the closest item in received (that's not closer to another item in expected)
	// Calculate the position difference and store that.
	for(expectedIt=expected.begin();expectedIt!=expected.end();expectedIt++){
		double sourcepos=expectedIt->first;
		vector<string> r=getPosOfNearestReceived(sourcepos, received, expected);
		bool match=(r[0]=="true");
		double matchpos=toFloat(r[1]);
		string matchkey=r[2];
		if(match){
			differences.push_back(matchpos-sourcepos);
		}
	}
	//std::cout << "differences size" << differences.size() << std::endl;
	if(differences.size() > 0){
		
		// get the mean difference
		double meanDeviationFromExpected=std::accumulate(differences.begin(),differences.end(), 0.0)/differences.size();
		std::cout << "meanDeviationFromExpected: " << ofToString(meanDeviationFromExpected) << endl;
		
		// transform each pos in received events, trying to compensate for system latency
		for(receivedIt=received.begin();receivedIt!=received.end();receivedIt++){
			receivedIt->first = receivedIt->first - meanDeviationFromExpected;
		}
	}
}

double in1klap::getMedian(vector<double> const input){
	vector<double> copy(input);
	sort(copy.begin(), copy.end());
	double midway=copy.size()/2;
	// if midway is a whole number
	if(midway - floor(midway)==0){
		return((copy[midway]+copy[midway+1])/2);
	} else {
		return(copy[floor(midway)]);
	}
}

vector<int> in1klap::getExpectedBeatVector(int sequenceNumber, bool notes){
	//std::cout << "entering getexpectedbeatvector " << std::endl;

	vector<int> r;
	vector<string> s=sequences[sequenceNumber];
	vector<string>::iterator sIt;
	vector<string> split;
	vector<string>::iterator splitIt;
	int beat=0;
	
	//std::cout << "starting text file loop" << std::endl;

	for (sIt=s.begin();sIt!=s.end();sIt++){
		if(notes){
			split=ofSplitString(*sIt,",");
			for(splitIt=split.begin();splitIt!=split.end();splitIt++){
				if(*splitIt != "0"){
					r.push_back(beat);
				}
			}
		} else {
			r.push_back(beat);
		}
		beat++;
	}
	//std::cout << "returning from getexpectedbeatvector " << std::endl;
	return(r);
}

vector<pair<int, string> > in1klap::getExpectedVector(int sequenceNumber,int bpm, bool notes){
	//std::cout << "entering getexpectedvector for seq number " << sequenceNumber << std::endl;

	int sixteenthSize=metro.lengthOf16thInMs();
	int sixteenthsCounter=0;
	vector<string> s=sequences[sequenceNumber];
	vector<string>::iterator sIt;
	for (sIt=s.begin();sIt!=s.end();sIt++){
		std::cout << *sIt << endl;
	}
	vector<pair<int, string> > retVector;
	pair<int, string> retPair;
	
	int quarternoteCounter=0;
	// for each quarter measure in the input file
	for (sIt=s.begin();sIt!=s.end();sIt++){
		//std::cout << std::endl;
		int basePos=sixteenthsCounter*sixteenthSize;
		//std::cout << "basePos is "<< basePos << std::endl;
		vector<string> split=ofSplitString(*sIt,",");
		vector<string>::iterator splitIt;
		int measureCode=0;
		
		// do we return events for each 16th?
		if(notes){
			//std::cout << "note mode is true " << std::endl;

			int c=0;
			for(splitIt=split.begin();splitIt!=split.end();splitIt++){
				if(*splitIt=="1"){
					//std::cout << "note detected in sequence file line " << std::endl;
					retPair.first=basePos+c*sixteenthSize;
					retPair.second=' ';
					//std::cout << "pushing pair to vector " << retPair.first << " " << retPair.second << std::endl;

					retVector.push_back(retPair);
				}
				c++;
			}
			
		} else {
			//std::cout << "note mode not true " << std::endl;
			if(*sIt=="0,0,0,0"){
				measureCode=0;
			} else if (*sIt=="1,0,0,0"){
				measureCode=1;
			} else if(*sIt=="1,0,1,0"){
				measureCode=2; 
			} else if(*sIt=="0,0,1,0"){
				measureCode=3;			
			} else if(*sIt=="1,1,1,1"){
				measureCode=4;
			}
								
			retPair.first=basePos;
			retPair.second=ofToString(measureCode);
			retVector.push_back(retPair);
		}
		sixteenthsCounter+=4;
		//std::cout<< ofToString(quarternoteCounter) << std::endl;
		quarternoteCounter++;
	}
	vector<pair<int, string> >::iterator vI;
	for(vI=retVector.begin();vI!=retVector.end();vI++){
		//std::cout << "Expected item."<< vI->first << ", " << vI->second << std::endl;
	}
	
	return(retVector);
}

void in1klap::drawEventVectors(const vector<pair<int, string> >& v1, const vector<pair<int, string> >& v2){
	vector<pair<int, string> >::const_iterator vI;
	ofSetColor(0, 0, 0);
	double factor=20;
	double margin=30;
	for(vI=v1.begin();vI!=v1.end();vI++){
		ofCircle((vI->first/factor)+margin, 30, 1);
		ofLine((vI->first/factor)+margin, 30, (vI->first/factor)+margin, 50);
	}
	for(vI=v2.begin();vI!=v2.end();vI++){
		ofCircle((vI->first/factor)+margin, 40, 1);
	}
}