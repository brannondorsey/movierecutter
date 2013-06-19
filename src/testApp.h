#pragma once

#include "ofMain.h"
#include "sequence.h"
#include "gui.h"
#include "DataHandler.h"

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
        int fastForward;
        int rewind;
        float imgPercentNeededToPass;
        float aspectRatio;
        bool isPaused;
        bool seqReady;
        bool isFinished;
        bool neededLoading;
        bool movieAlreadySelected;
        string moviePath;
    
        Gui gui;
        DataHandler dataHand;
        Sequence currentSeq;
        vector <Sequence> sequences;
        vector<long> cutFrames;
        ofVideoPlayer myVideo;
        ofPixels prevPixels;

};
