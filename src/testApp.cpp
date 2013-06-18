#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0, 0, 0);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    isPaused = false;
    isFinished = false;
    seqReady = false;
    
    seqIndex = 0;
    checkFrameIndex = 2;
    
    if(!movieAlreadySelected) selectMovie();
    dataHand = DataHandler(moviePath);
    aspectRatio = myVideo.getWidth()/myVideo.getHeight();
    myVideo.setVolume(0.0); //change this to 1.0
    myVideo.setLoopState(OF_LOOP_NORMAL);
    gui.initTotalNumFrames(myVideo.getTotalNumFrames());
    initSequenceVars();
}

//--------------------------------------------------------------
void testApp::update(){
    //if the movie is ready, playing, and needs a new sequence
    if((seqReady) &&
       (needsNewSeq()) &&
       (!isFinished)){
        seqIndex++;
        currentSeq = sequences[seqIndex];
        myVideo.setFrame(currentSeq.start);
        dataHand.savePlayPoint(currentSeq.start); //save frame to .playpoint file
        gui.updateTimeline(seqIndex, sequences.size());
    }
    //if the movie is over
    if((seqReady) &&
       (seqIndex == sequences.size())){
        myVideo.stop();
        isFinished = true;
    }
    //if the sequences are ready
    if(seqReady){
        myVideo.update();
        gui.checkTimer();
    }
    //if sequence file does not exist set it and show loading
    else if(!dataHand.seqFileExists()){
        cout<<"the sequence file did not exist. I am loading a new one."<<endl;
        setSequences();
        neededLoading = true;
        gui.updateLoading(totalChecked);
    }
    //if sequence file exists load it, play it, set seq to ready, pause it, and display resume menu
    else{
        isPaused = true;
        myVideo.setPaused(isPaused);
        if(!neededLoading){
            gui.resumeMenuShowing = true;
        }
        //if there has been a resume menu selection or the menu never showed because needed loading
        if((gui.resumeMenuSelection != "") ||
           (neededLoading)){
            //if selection was resume or start over or there was loading needed
            if((gui.resumeMenuSelection != gui.resumeButtonValues[2]) ||
               neededLoading){
                dataHand.loadSequences(sequences);
                //if selection was resume load previous point and play it
                if(gui.resumeMenuSelection == gui.resumeButtonValues[0]){
                    seqIndex = dataHand.getPlayPointSeqIndex(sequences); //loads seqIndex from info from file.playpoint
                    cout<<"the seqIndex is "<<seqIndex<<endl;
                    currentSeq = sequences[seqIndex]; // set the loaded sequence
                }
                //if selection was start over or loading was needed
                else{
                    currentSeq = sequences[0];
                    dataHand.getPlayPoint(); //used to load playpoint
                }
                seqReady = true;
                myVideo.setFrame(currentSeq.start); // and the play point
                cout<<"the frame was set to "<<currentSeq.start<<endl;
                gui.updateTimeline(seqIndex, sequences.size()); //and update the timeline
                myVideo.play();
                isPaused = false;
                myVideo.setPaused(isPaused);
            }
            //if the selection was recut and start over
            else{
                //delete sequence and playpoint files
                ofFile playPointFile(ofToDataPath("storage/"+dataHand.fileName+".playpoint"));
                ofFile sequencesFile(ofToDataPath("storage/"+dataHand.fileName+".sequences"));
                playPointFile.remove();
                sequencesFile.remove();
                
                movieAlreadySelected = true;
                gui.resumeMenuSelection = "";
                neededLoading = true;
                gui.resumeMenuShowing = false; //otherwise will not show menu loading
                setup();
                cout<<"just re-ran setup and set neededLoading to true"<<endl;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    float width = ofGetWidth();
    float height = ofGetWidth()/aspectRatio;
    if(!isFinished) myVideo.draw(0, (ofGetHeight()-height)/2, width, height);
    if(seqReady){
        if(gui.isShowing){
            gui.displayButtons(mouseX, mouseY);
            gui.displayTimeline();
        }
        if(isPaused)gui.displayPlayIcon();
    }
    else{
        gui.displayResumeMenu(mouseX, mouseY);
        if(!gui.resumeMenuShowing){
            gui.displayLoading();
        }
    }
}
//--------------------------------------------------------------
void testApp::selectMovie(){
    ofFileDialogResult selectedMovie = ofSystemLoadDialog("Select a Movie", false, "");
    moviePath = selectedMovie.getPath();
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
void testApp::initSequenceVars(){
    pixelThreshold = 25; //percent similarity each individual pixel needs to pass
    imgPercentNeededToPass = 80; //percent of pixels that must pass threshold test
    checkEveryIncrement = 10; //number of pixels before next pixel comparison
    maxPixDiffToPass = (pixelThreshold*255)/100; //converts percent to a max pixel change value
    numCheckEachFrame = 40; //number of movie frames to set each frame of the program
    totalChecked = 0;
    myVideo.setFrame(1);
    prevPixels = ofPixels(myVideo.getPixelsRef());
}

void testApp::setSequences(){
    
    int stopThisCheck = checkFrameIndex+numCheckEachFrame;
    
    for(; checkFrameIndex<stopThisCheck; checkFrameIndex++){
        int numThatPass = 0; //holds number of pixels that pass
        myVideo.setFrame(checkFrameIndex); // start at first frame
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
        if(numThatPass/(myVideo.getWidth()*myVideo.getHeight()/(checkEveryIncrement*checkEveryIncrement)) <= imgPercentNeededToPass/100) cutFrames.push_back(checkFrameIndex);

        prevPixels = ofPixels(pixelsRef);
        totalChecked++;
    }//go to next frame
    
    if(checkFrameIndex >= myVideo.getTotalNumFrames()){
        cout<<"there are "+ofToString(cutFrames.size())<<" cuts in this movie"<<endl;
        if(cutFrames.size() > 0){
            sequences.push_back(Sequence(1, cutFrames[0]-1)); //set first sequence before a cut happened
            //cout<<"got here! The last frame of the first sequence set is "<<ofToString(cutFrames[0]-1)<<endl;

            //create sequences from cutFrames vector
            for(int l = 0; l < cutFrames.size()-1; l++){
                long start = cutFrames[l];
                long stop = cutFrames[l+1]-1;
                if(l == cutFrames.size()-2) stop = cutFrames[l+1]; //include last frame if sequence is last in movie
                sequences.push_back(Sequence(start, stop));
            }
            //uncomment below for debugging
            //            for(int index = 0; index < sequences.size(); index++){
            //                cout<<"sequence "<<index<<" | start: "<<sequences[index].start<<" stop: "<<sequences[index].stop<<endl;
            //            }
            ofRandomize(sequences);
            dataHand.saveSequences(sequences);
        }
        else{
            ofSystemAlertDialog("No cuts detected. Choose another movie.");
            setup();
        }
    }
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
    gui.showGui();
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //if left click
    if(button == 0){
        if((gui.playButtonRect.inside(x, y)) &&
            (isPaused)){
            isPaused = false;
            myVideo.setPaused(isPaused);
        }
        if(gui.resumeMenuShowing){
            for(int j = 0; j < gui.resumeMenuNumButtons; j++){
                if(gui.resumeMenuButtons[j].inside(x, y)){
                    gui.selectResumeButton(j);
                    gui.resumeMenuShowing = false;
                }
            }
        }
        if(gui.numButtons != 0){
            for(int i = 0; i < gui.numButtons; i++){
                if(gui.buttonRects[i].inside(x, y)){
                    switch(i){
                        case 0: //stop button
                            break;
                        case 1: //pause button
                            isPaused = true;
                            myVideo.setPaused(isPaused);
                            rewind = 0; 
                            fastForward = 0;
                            break;
                        case 2: //play button
                            isPaused = false;
                            myVideo.setPaused(isPaused);
                            rewind = 0;
                            fastForward = 0;
                            break;
                        case 3: //rewind button
                            rewind++;
                            fastForward = 0;
                            break;
                        case 4: //fastforward button
                            fastForward++;
                            rewind = 0;
                            break;
                    }
                }
            }
        }
    }
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