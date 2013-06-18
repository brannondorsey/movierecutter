#include "gui.h"
#include "ofMain.h"
#include "sequence.h"

Gui::Gui(){
    font.loadFont("fonts/futura.ttc", 20);
    loadingRectStartX = ofGetWidth()/4;
    loadingRectStartY = ofGetHeight()/2;
    loadingRectHeight = 20;
    loadingRectMaxWidth = ofGetWidth()/2;
    timelineStartX = 0;
    numButtons = 5;
    isShowing = false;
    resumeMenuShowing = false;
    string buttonImageNames [] = {"stop", "pause", "play", "rewind", "fastforward"};
    for(int i = 0; i < numButtons; i++){
        buttonImages[i].loadImage("images/"+buttonImageNames[i]+".png");
    }
    resumeButtonValues[0] = "Resume playing";
    resumeButtonValues[1] = "Start over";
    resumeButtonValues[2] = "Recut and start over";
    resumeMenuSelection = "";
    showGuiTimer.setup(0, false); //setup timer. Give it no delay
    showGuiTimer.setTimer(2000); //set the timer
    displayButtons(0, 0); //these two lines of code are smelly because they don't actually do what they say that they do here. They are used to init two vars used in the updateTimeline() mapping process.
    displayTimeline();
}

//--------------------------------------------------------------

void Gui::displayResumeMenu(int mx, int my){
    
    resumeMenuNumButtons = 3;
    int buttonMarginSides = 40;
    int buttonMarginTop = 15;
    int boxColorDefault = 40;
    int boxColorHovered = 80;
    int boxWidth = 400; //ofGetWidth()/3;
    int boxHeight = (font.getLineHeight()+buttonMarginTop)*3;//ofGetHeight()/4;
    int boxX = ofGetWidth()/2-boxWidth/2;
    int boxY = ofGetHeight()/2-boxHeight/2;
    int amountRounded = 10;
    
    int buttonsX = boxX+buttonMarginSides;
    int buttonsY = boxY;
    
    //draw menu box
    ofSetColor(boxColorDefault);
    ofFill();
    ofRectRounded(boxX, boxY, boxWidth, boxHeight, amountRounded);
    
    for(int i = 0; i < resumeMenuNumButtons; i++){
        if(resumeMenuButtons[i].inside(mx, my)){
            ofSetColor(boxColorHovered);
        }
        else ofSetColor(boxColorDefault);
        resumeMenuButtons[i] = ofRectangle(boxX, buttonsY, boxWidth, font.getLineHeight()+buttonMarginTop);
        if((i == 0) || (i == resumeMenuNumButtons-1)) ofRectRounded(resumeMenuButtons[i], amountRounded);
        else ofRect(resumeMenuButtons[i]);
        ofSetColor(255);
        font.drawString(resumeButtonValues[i], buttonsX, buttonsY+font.getLineHeight());
        buttonsY += buttonMarginTop+font.getLineHeight();
    }
    ofNoFill();
}

//--------------------------------------------------------------

void Gui::selectResumeButton(int selectedButton){
    resumeMenuSelection = resumeButtonValues[selectedButton];
}

//--------------------------------------------------------------

void Gui::initTotalNumFrames(long _totalNumFrames){
    totalNumFrames = _totalNumFrames;
}

//--------------------------------------------------------------
void Gui::updateLoading(long& totalChecked){
    amountLoadedMapped = ofMap(totalChecked, 1, totalNumFrames, 0, loadingRectMaxWidth);
}


//--------------------------------------------------------------
void Gui::displayLoading(){
    ofFill();
    ofSetColor(0);
    ofRect(0, 0, ofGetWidth(), ofGetHeight()); //draw black background
    ofSetColor(255);
    ofRect(loadingRectStartX, loadingRectStartY, amountLoadedMapped, loadingRectHeight);
    ofNoFill();
    ofRect(loadingRectStartX, loadingRectStartY, loadingRectMaxWidth, loadingRectHeight);
}

//--------------------------------------------------------------
void Gui::displayButtons(int mx, int my){
    buttonSize = 50;
    buttonMargin = 10;
    int buttonsMarginTop = 14;
    int buttonsX = buttonMargin;
    buttonsY = ofGetHeight()-buttonSize-buttonMargin;
    ofSetColor(120, 75);
    ofFill();
    ofRect(0, buttonsY-buttonsMarginTop, ofGetWidth(), buttonSize+buttonMargin+buttonsMarginTop);
    
    ofSetColor(255); //reset fill and color
    ofNoFill();
    
    for(int i = 0; i < numButtons; i++){
        buttonRects[i] = ofRectangle(buttonsX, buttonsY, buttonSize, buttonSize);
        //if over [i] button
        if(buttonRects[i].inside(mx, my)){
            buttonRects[i].setY(buttonRects[i].getTop()-4);
        }
        buttonImages[i].draw(buttonRects[i]);
        buttonsX += buttonSize+buttonMargin;
    }
    timelineStartX = buttonsX;
}

//--------------------------------------------------------------
void Gui::updateTimeline(int seqNumber, int totalSeqs){
    timelineWatchPointX = ofMap(seqNumber, 0, totalSeqs, timelineStartX, timelineStartX+timelineWidth);
//    cout<<"the seqNumber is "<<seqNumber<<endl;
//    cout<<"the totalSeqs val is "<<totalSeqs<<endl;
//    cout<<"the min map val is "<<timelineStartX<<endl;
//    cout<<"the max map val is "<<ofToString(timelineStartX+timelineWidth)<<endl;
//    cout<<"the totalSeqs val is "<<totalSeqs<<endl;
//    cout<<"the timelineWatchPointX val is "<<timelineWatchPointX<<endl;
}

//--------------------------------------------------------------
void Gui::displayTimeline(){
    //notice: some of the vars that displayTimeline uses are instantiated in displayButtons
    int timelineBarWidth = 5;
    int timelineBarOverhang = 5; //amount top and bottom overhangs timeline by
    
    
    //draw timeline
    timelineWidth = ofGetWidth()-timelineStartX-buttonMargin;
    ofSetColor(0);
    ofFill();
    ofRectangle timeline = ofRectangle(timelineStartX, buttonsY, timelineWidth, buttonSize);
    ofRectRounded(timeline, 5);
    
    ofSetColor(255); //reset color and fill
    ofNoFill();
    
    //draw watch point bar
    ofFill();
    ofRectangle timelineBar(timelineWatchPointX, buttonsY-timelineBarOverhang, timelineBarWidth, buttonSize+timelineBarOverhang*2);
    ofRectRounded(timelineBar, 2);
    ofNoFill();

}

//--------------------------------------------------------------
void Gui::displayPlayIcon(){
    int playButtonSize = 100;
    ofImage playButtonImage = buttonImages[2];
    playButtonRect = ofRectangle(ofGetWidth()/2-playButtonSize/2, ofGetHeight()/2-playButtonSize/2, playButtonSize, playButtonSize);
    ofSetColor(255, 120);
    playButtonImage.draw(playButtonRect);
    ofSetColor(255);
}

//--------------------------------------------------------------
void Gui::showGui(){
    isShowing = true;
    showGuiTimer.startTimer();
}

//--------------------------------------------------------------
void Gui::checkTimer(){
    if (showGuiTimer.isTimerFinished()){
        isShowing = false;
        showGuiTimer.reset(); //dont forget to reset
    }
}
