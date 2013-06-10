#include "gui.h"
#include "ofMain.h"

Gui::Gui(){
    loadingRectStartX = ofGetWidth()/4;
    loadingRectStartY = ofGetHeight()/2;
    loadingRectHeight = 20;
    loadingRectMaxWidth = ofGetWidth()/2;
    timelineStartX = 0;
    numButtons = 5;
    string buttonImageNames [] = {"stop", "pause", "play", "rewind", "fastforward"};
    for(int i = 0; i < numButtons; i++){
        buttonImages[i].loadImage("images/"+buttonImageNames[i]+".png");
    }
    ofEnableAlphaBlending();
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
    //ofSetColor(100);
    ofFill();
    ofRect(loadingRectStartX, loadingRectStartY, amountLoadedMapped, loadingRectHeight);
    ofNoFill();
    ofRect(loadingRectStartX, loadingRectStartY, loadingRectMaxWidth, loadingRectHeight);
}

//--------------------------------------------------------------
void Gui::updateButtons(){
    
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
void Gui::updateTimeline(){
    
}

//--------------------------------------------------------------
void Gui::displayTimeline(){
    //notice: some of the vars that displayTimeline uses are instantiated in displayButtons
    float timelineWidth = ofGetWidth()-timelineStartX-buttonMargin;
    ofSetColor(0);
    ofFill();
    ofRectangle timeline = ofRectangle(timelineStartX, buttonsY, timelineWidth, buttonSize);
    ofRectRounded(timeline, 5);
    
    ofSetColor(255); //reset color and fill
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