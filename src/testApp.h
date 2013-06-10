#pragma once

#include "ofMain.h"
#include "sequence.h"
#include "gui.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void selectMovie();
        void initSequenceVars();
        void setSequences();
        bool needsNewSeq();
    
        int numSequences;
        int seqIndex;
        int checkFrameIndex;
        int pixelThreshold;
        int checkEveryIncrement;
        int maxPixDiffToPass;
        int numCheckEachFrame;
        long totalChecked;
        float imgPercentNeededToPass;
        float aspectRatio;
    
        int fastForward;
        int rewind;
        bool isPaused;
        bool seqReady;
    
    
        string pathToMovie;
    
        Gui gui;
        Sequence currentSeq;
        vector <Sequence> sequences;
        vector<long> cutFrames;
        ofVideoPlayer myVideo;
        ofPixels prevPixels;
        

};
