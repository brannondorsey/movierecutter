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
    int pixelThreshold = 25; //percent similarity each individual pixel needs to pass
    float imgPercentNeededToPass = 80; //percent of pixels that must pass threshold test
    int checkEveryIncrement = 10; //number of pixels before next pixel comparison
    int maxPixDiffToPass = (pixelThreshold*255)/100; //converts percent to a max pixel change value

    vector<long> cutFrames; 
    myVideo.setFrame(1);
    ofPixels prevPixels(myVideo.getPixelsRef());
    
    for(int i = 2; i<myVideo.getTotalNumFrames(); i++){
        int numThatPass = 0; //holds number of pixels that pass
        myVideo.setFrame(i); // start at first frame
        ofPixelsRef pixelsRef = myVideo.getPixelsRef();
            
        //loop through each pixel in the image
        for(int j = 0; j < myVideo.getWidth(); j += checkEveryIncrement){
            for(int k = 0; k < myVideo.getHeight(); k += checkEveryIncrement){
                float prevLightness = prevPixels.getColor(j,k).getLightness();
                float lightness = pixelsRef.getColor(j,k).getLightness();
//                if(prevLightness != lightness){
//                    cout<<"the lightness is "<<lightness<<" and the previous lightness is "<<prevLightness<<endl;
//                    cout<<"that is a difference of "<<ofToString(abs(prevLightness-lightness))<<endl;
//                }
                if(abs(prevLightness-lightness) <= maxPixDiffToPass){
                    //cout<<"this value passes"<<endl;
                    numThatPass++;
                }
            }
        }
        //cout<<"the num that pass is "<<ofToString(numThatPass)<<endl;
        //cout<<"the first pixel of frame "<<ofToString(i)<<" is "<<ofToString(pixelsRef.getColor(1,1).getLightness())<<", the previous pixel was "<<ofToString(prevPixelsRef.getColor(1,1).getLightness())<<endl;
        //cout<<ofToString(numThatPass/(myVideo.getWidth()*myVideo.getHeight()/(checkEveryIncrement*checkEveryIncrement)))<<" has to be greater than "<<ofToString(imgPercentNeededToPass/100)<<" to pass"<<endl;


        if(numThatPass/(myVideo.getWidth()*myVideo.getHeight()/(checkEveryIncrement*checkEveryIncrement)) <= imgPercentNeededToPass/100){
            //if enough pixels passed
            cutFrames.push_back(i);
            cout<<"found a cut at frame "<<ofToString(i)<<endl;
        }
        prevPixels = ofPixels(pixelsRef);
        
    }//go to next frame
    
    cout<<"there are "+ofToString(cutFrames.size())<<" cuts in this movie"<<endl;
}

bool testApp::needsNewSeq(){
    if(myVideo.getCurrentFrame() >= currentSeq.stop) return true;
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