#pragma once

#include "ofMain.h"
#include "sequence.h"

class testApp : public ofBaseApp{
	public:
    
        void setSequences();
        bool needsNewSeq();
    
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
    
        int numSequences;
        int seqIndex;
        bool isPaused;
    
        Sequence currentSeq;
        vector <Sequence> sequences;
        ofVideoPlayer myVideo;
        
};
