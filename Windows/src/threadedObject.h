#ifndef _THREADED_OBJECT
#define _THREADED_OBJECT

 #include <time.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <windows.h>
 #include <winbase.h>

#include "ofMain.h"
#include "ofxThread.h"
#include "ofEvents.h"
#include "eventsObject.h"



// this is not a very exciting example yet
// but ofThread provides the basis for ofNetwork and other
// operations that require threading.
//
// please be careful - threading problems are notoriously hard
// to debug and working with threads can be quite difficult


class threadedObject : public ofxThread{

	public:

	eventsObject myEventsObject;
	    int count;

	    // threaded fucntions that share data need to use lock (mutex)
	                // and unlock in order to write to that data
	                // otherwise it's possible to get crashes.
	                //
	                // also no opengl specific stuff will work in a thread...
	                // threads can't create textures, or draw stuff on the screen
	                // since opengl is single thread safe

		//--------------------------


        LARGE_INTEGER ticksPerSecond;
        LARGE_INTEGER tick;   // A point in time
        LARGE_INTEGER start_ticks, end_ticks;
        //, cputime;

		threadedObject(){
			count = 0;

		}
        int lastMillis;
        int totalMillis;
        float getElapsedMillis()
        {
           return totalMillis;
        }


		void start(){
		    //THREAD_PRIORITY_IDLE, THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL
            //THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST,  THREAD_PRIORITY_TIME_CRITICAL

            startThread(true, false);   // blocking, verbose
            lastMillis=ofGetElapsedTimeMillis();
            SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_BELOW_NORMAL);//
            QueryPerformanceFrequency(&ticksPerSecond);
            QueryPerformanceCounter(&start_ticks);
            //lastMillis = 0;
        }

        void stop(){
            stopThread();
        }

		//--------------------------
		void threadedFunction(){
            bool doUpdate = false; //false
			while( isThreadRunning() != 0 ){
				if( lock() ){
					count++;
					//if(count > 50000) count = 0;
					//int myInt = 1;
					int deltaMillis = ofGetElapsedTimeMillis() - lastMillis;
					QueryPerformanceCounter(&end_ticks);

					LARGE_INTEGER cputime;
					cputime.QuadPart = end_ticks.QuadPart- start_ticks.QuadPart;

                    totalMillis = ((float)end_ticks.QuadPart/(float) ticksPerSecond.QuadPart)*1000;
                    float elapsedMillis = ((float)cputime.QuadPart/(float)ticksPerSecond.QuadPart) *1000.0f;
                    //start_ticks = end_ticks;.
					//if (deltaMillis >0)
					if (elapsedMillis >= 1.0f)
					{
					       /*
					        printf ("\tElapsed CPU time test:   %.9f  sec  ticks %d\n",
    ((float)cputime.QuadPart/(float)ticksPerSecond.QuadPart),cputime.QuadPart);
    */
					//ofNotifyEvent(myEventsObject.newIntEvent,deltaMillis,this);
					//lastMillis = ofGetElapsedTimeMillis();
					doUpdate = true;
					QueryPerformanceCounter(&start_ticks);
					//start_ticks -= (elapsedMillis - 3.0f);
					lastMillis = ofGetElapsedTimeMillis();



					}
					/*
                    int c = count - lastMillis;
					ofNotifyEvent(myEventsObject.newIntEvent,c,this);
					lastMillis = count;
*/

                unlock();

                   if (doUpdate)
					{
                        cout << "sending event at elapsedMillis " << elapsedMillis <<"\n";//start_ticks.QuadPart;
					float ms = totalMillis;
					ofNotifyEvent(myEventsObject.newFloatEvent,ms,this);
                    ofSleepMillis(1);//5
                  //Sleep(1);//1
					}
                    //ofSleepMillis(1);
                                      //  SwitchToThread();
					//ofSleepMillis(1 * 1000);
                    //Sleep(5);

				}
			}
		}





        void testTime()
        {

     int count,a;

     printf ("\ Value of CLOCKS_PER_SEC is :    %f  ticks/sec\n",CLOCKS_PER_SEC   );

     // get the high resolution counter's accuracy
     // if (!QueryPerformanceFrequency(&ticksPerSecond))
    // printf("\tno go QueryPerformance not present");
    printf ("\tfreq test:   %I64Ld ticks/sec\n",ticksPerSecond    );
     // what time is it?
      if (!QueryPerformanceCounter(&tick) ) printf("no go counter not installed");
    printf ("\QueryPerformanceCounter testpoint :   %I64Ld  ticks\n",tick);

    QueryPerformanceCounter(&start_ticks);
      /* start foo()  */


      /* end foo( ) */

    QueryPerformanceCounter(&end_ticks);
    LARGE_INTEGER cputime;
    cputime.QuadPart = end_ticks.QuadPart- start_ticks.QuadPart;

    printf ("\tElapsed CPU time test:   %.9f  sec  ticks %d\n",
    ((float)cputime.QuadPart/(float)ticksPerSecond.QuadPart),cputime.QuadPart);

}


		//--------------------------
		void draw(){

			string str = "I am a slowly increasing thread. \nmy current count is: ";

			if( lock() ){
				str += ofToString(count);
				unlock();
			}else{
				str = "can't lock!\neither an error\nor the thread has stopped";
			}
			ofDrawBitmapString(str, 50, 56);
		}



};

#endif
