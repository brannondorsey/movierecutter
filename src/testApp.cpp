#include "testApp.h"
#include <iostream.h>

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(255, 255, 255);
    ofSetVerticalSync(true);
    isPaused = false;
    
    numSequences = 100;
    seqIndex = 0;
    selectMovie();
    myVideo.setVolume(1.0);
    myVideo.setLoopState(OF_LOOP_NORMAL);
    cout<<"the number of frames is "<<ofToString(myVideo.getTotalNumFrames())<<endl;
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
            myVideo.stop();
//            ofRandomize(sequences);
//            seqIndex = 0;
        }
    }
    myVideo.update();  
}

//--------------------------------------------------------------
void testApp::draw(){
    myVideo.draw(0,0);
}

//--------------------------------------------------------------
void testApp::selectMovie(){
    ofFileDialogResult selectedMovie = ofSystemLoadDialog("Select a Movie", false, "");
    string moviePath = selectedMovie.getPath();
    if ((moviePath.find(".mov") != string::npos) ||
        (moviePath.find(".MOV") != string::npos) ||
        (moviePath.find(".mp4") != string::npos) ||
        (moviePath.find(".MP4") != string::npos)){
        myVideo.loadMovie(moviePath);
    }
    else if(moviePath != ""){
        ofSystemAlertDialog("Not a supported movie type");
        selectMovie();
    }
    else{
        ofExit(); //exit if canceled
    }
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
                if(abs(prevLightness-lightness) <= maxPixDiffToPass){
                    numThatPass++;
                }
            }
        }

        //cout<<ofToString(numThatPass/(myVideo.getWidth()*myVideo.getHeight()/(checkEveryIncrement*checkEveryIncrement)))<<" has to be greater than "<<ofToString(imgPercentNeededToPass/100)<<" to pass"<<endl;

        //if too many pixels were different
        if(numThatPass/(myVideo.getWidth()*myVideo.getHeight()/(checkEveryIncrement*checkEveryIncrement)) <= imgPercentNeededToPass/100) cutFrames.push_back(i);

        prevPixels = ofPixels(pixelsRef);
        
    }//go to next frame
    
    sequences.push_back(Sequence(0, cutFrames[0]-1)); //set first sequence before a cut happened
    
    //create sequences from cutFrames vector
    for(int l = 0; l < cutFrames.size(); l++){
        long start = cutFrames[l];
        long stop;
        if(l != cutFrames.size()-1){
            stop = cutFrames[l+1]-1;
        }
        else{
            stop = myVideo.getTotalNumFrames();
        }
        //cout<<"the start frame is "<<start<<", the stop is "<<stop<<endl;
        sequences.push_back(Sequence(start, stop));
    }
    ofRandomize(sequences);
    cout<<"there are "+ofToString(sequences.size())<<" cuts in this movie"<<endl;
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