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
    isCursorShowing = true;
    
    seqIndex = 0;
    checkFrameIndex = 2;
    rwndSpeed = 67;
    origRwndSpeed = rwndSpeed;
    numbRwnds = 0;
    maxRwndSpeed = 3;
    speedIncrease = -21;
    
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
    }
    //if the movie is over
    if((seqReady) &&
       (seqIndex >= sequences.size())){ //changed this from == to >=. Dont know if this messed anything up...
        isFinished = true;
        gui.updateTimeline(seqIndex, sequences.size()); //make sure that the timeline shows that the movie is done
        myVideo.stop();
        myVideo.closeMovie();
        dataHand.eraseStorageFiles();
    }
    //if the sequences are ready
    if(seqReady){
        if((rewinding) || (fastForwarding)){
            changeSpeed();
        }
        myVideo.update();
        if(!isFinished) gui.updateTimeline(seqIndex, sequences.size()); //dont update if movie is over
        gui.checkTimer();
    }
    //if sequence file does not exist set it and show loading
    else if(!dataHand.seqFileExists()){
        setSequences();
        neededLoading = true;
        gui.updateLoading(totalChecked);
    }
    //if sequence file already exists
    else{
        isPaused = true;
        myVideo.setPaused(isPaused);
        //if loading was not needed
        if(!neededLoading) gui.resumeMenuShowing = true; //if no loading occured it is ok to show the resume menu
        
        //if there has been a resume menu selection or the menu never showed because needed loading
        if((gui.resumeMenuSelection != "") ||
           (neededLoading)){
            //if selection was resume or start over or there was loading needed
            if((gui.resumeMenuSelection != gui.resumeButtonValues[2]) ||
               neededLoading){
                sequences.clear(); //make sure that the sequences are empty
                dataHand.loadSequences(sequences); //parse file.sequences and load them into the sequences vector 
                //if selection was resume load previous point and play it
                if(gui.resumeMenuSelection == gui.resumeButtonValues[0]){
                    seqIndex = dataHand.getPlayPointSeqIndex(sequences); //loads seqIndex from info from file.playpoint
                    cout<<"the seqIndex is "<<seqIndex<<endl;
                    currentSeq = sequences[seqIndex]; // set the loaded sequence
                    isPaused = false; //start automatically
                }
                //if selection was start over or loading was needed
                else{
                    currentSeq = sequences[0]; //start at the first sequence
                    dataHand.getPlayPoint(); //long function but used to load playpoint in this case
                    isPaused = true; //pause in case user is away or something
                }
                seqReady = true;
                myVideo.setFrame(currentSeq.start);
                cout<<"the frame was set to "<<currentSeq.start<<endl;
                gui.updateTimeline(seqIndex, sequences.size()); //and update the timeline
                myVideo.play();
                myVideo.setPaused(isPaused);
            }
            //if the selection was recut and start over
            else{
                dataHand.eraseStorageFiles();
                movieAlreadySelected = true;
                gui.resumeMenuSelection = "";
                neededLoading = true;
                gui.resumeMenuShowing = false; //otherwise will not show menu loading
                setup();
                cout<<"just re-ran setup and set neededLoading to true"<<endl;
            }
        }
        //if there has not been a selection set the frame to the playpoint and update the image shown behind resume menu
        else{
            myVideo.setFrame(dataHand.getPlayPoint());
            myVideo.update();
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!isFinished) displayVideo();
    if(seqReady){
        if((gui.isShowing) ||
           (gui.toolbarRect.inside(mouseX, mouseY))){
            gui.displayButtons(mouseX, mouseY);
            if((rewinding) || (fastForwarding)){
                gui.displayRwndNumbs(rewinding, fastForwarding, numbRwnds, mouseX, mouseY);
            }
            gui.displayTimeline();
            //show the cursor
            if((!isCursorShowing)){
                ofShowCursor();
                isCursorShowing = true;
            }
        }
        else{
            //hide the cursor
            if(isCursorShowing){
                ofHideCursor();
                isCursorShowing = false;
            }
        }
        if(isPaused)gui.displayPlayIcon();
    }
    //if the seaquences are not ready
    else{
        gui.displayResumeMenu(mouseX, mouseY);
        if(!gui.resumeMenuShowing){
            gui.displayLoading();
        }
        //show the cursor
        if(!isCursorShowing){
            ofShowCursor();
            isCursorShowing = true;
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
        (moviePath.find(".MP4") != string::npos) ||
        (moviePath.find(".avi") != string::npos) ||
        (moviePath.find(".AVI") != string::npos) ||
        (moviePath.find(".dv") != string::npos) ||
        (moviePath.find(".DV") != string::npos)){
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
    dataHand.loadSettings(pixelThreshold, imgPercentNeededToPass, checkEveryIncrement, numCheckEachFrame);
    maxPixDiffToPass = (pixelThreshold*255)/100; //converts percent to a max pixel change value
    totalChecked = 0;
    myVideo.setFrame(1);
    prevPixels = ofPixels(myVideo.getPixelsRef());
}

//--------------------------------------------------------------
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
        cout<<"just checked frame number "<<checkFrameIndex<<endl;
    }//go to next frame
    cout<<"THE FRAME RATE IS "<<ofGetFrameRate()<<endl;
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

//--------------------------------------------------------------
void testApp::displayVideo(){
    float width = ofGetWidth();
    float height = ofGetWidth()/aspectRatio;
    float x = 0;
    float y = (ofGetHeight()-height)/2;
    if(height > ofGetHeight()){
        height = ofGetHeight();
        width = ofGetHeight()*aspectRatio;
        x = (ofGetWidth()-width)/2;
        y = 0;
    }
    myVideo.draw(x, y, width, height);
}

//--------------------------------------------------------------
void testApp::changeSpeed(){
    if(ofGetFrameNum() % rwndSpeed == 0){
        (fastForwarding) ? seqIndex++ : seqIndex--;
        //skip sequence if it is only one frame long
        if(sequences[seqIndex].start == sequences[seqIndex].stop){
            (fastForwarding) ? seqIndex++ : seqIndex--;
        }
        if(seqIndex <= 0) seqIndex = 0;
        if(seqIndex >= sequences.size()) seqIndex = sequences.size();
        currentSeq = sequences[seqIndex];
        myVideo.setFrame(currentSeq.start);
        }
}

//--------------------------------------------------------------
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
            fastForwarding = false;
            rewinding = false;
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
            rewinding = false;
            fastForwarding = false;
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
                            ofExit();
                            break;
                        case 1: //pause button
                            isPaused = true;
                            myVideo.setPaused(isPaused);
                            rewinding = false;
                            fastForwarding = false;
                            numbRwnds = 0;
                            break;
                        case 2: //play button
                            isPaused = false;
                            myVideo.setPaused(isPaused);
                            rewinding = false;
                            fastForwarding = false;
                            numbRwnds = 0;
                            break;
                        case 3: //rewind button
                            isPaused = true;
                            myVideo.setPaused(isPaused);
                            setSpeedChange(rewinding);
                            fastForwarding = false;
                            break;
                        case 4: //fastforward button
                            isPaused = true;
                            myVideo.setPaused(isPaused);
                            setSpeedChange(fastForwarding);
                            rewinding = false;
                            break;
                        case 5: //fullscreen button
                            ofToggleFullscreen();
                            break;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::setSpeedChange(bool &typeOfSpeedChange){
    if(typeOfSpeedChange){
        rwndSpeed += speedIncrease;
        if(rwndSpeed < maxRwndSpeed){
            rwndSpeed = maxRwndSpeed;
        }
        else numbRwnds++;
    }
    else{
        numbRwnds = 1;
        typeOfSpeedChange = true;
        rwndSpeed = origRwndSpeed; //reset rwnd speed
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