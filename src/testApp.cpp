#include "testApp.h"
#include <iostream.h>

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(255, 255, 255);
    ofSetVerticalSync(true);
    isPaused = false;
    
    numSequences = 100;
    seqIndex = 0;
    myVideo.loadMovie("black_dynomite.mp4");
    myVideo.setVolume(1.0);
    myVideo.setLoopState(OF_LOOP_NORMAL);
    setSequences();
    myVideo.play();
    
}

//--------------------------------------------------------------
void testApp::update(){
    if(needsNewSeq()){
        seqIndex++;
        currentSeq = sequences[seqIndex];
        myVideo.setFrame(currentSeq.start);
        if(seqIndex == sequences.size()){
            ofRandomize(sequences);
            seqIndex = 0;
        }
    }
    myVideo.update();  
}

//--------------------------------------------------------------
void testApp::draw(){
    myVideo.draw(0,0);
}

//--------------------------------------------------------------
void testApp::setSequences(){
    
    float seqLength = myVideo.getTotalNumFrames()/numSequences;
    for(int i = 0; i<numSequences; i++){
        long currentStart = seqLength*i;
        long currentStop;
        if(i != numSequences-1){
            currentStop = seqLength*(i+1);
            currentStop--;
        }
        else currentStop = myVideo.getTotalNumFrames(); //if it is the last index
        sequences.push_back(Sequence(currentStart, currentStop));
//        cout << "sequence "<< i <<" is as follows:"<<endl;
//        cout << "start is "<< currentStart <<endl;
//        cout << "stop is "<< currentStop <<endl;
//        cout <<endl;
    }
    ofRandomize(sequences);
    currentSeq = sequences[0];
    myVideo.setFrame(currentSeq.start);
}

bool testApp::needsNewSeq(){
    if(myVideo.getCurrentFrame() == currentSeq.stop) return true;
    else return false;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case ' ':
            isPaused = !isPaused;
            myVideo.setPaused(isPaused);
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}