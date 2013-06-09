#include "gui.h"
#include "ofMain.h"

Gui::Gui(){
    loadingRectStartX = ofGetWidth()/4;
    loadingRectStartY = ofGetHeight()/2;
    loadingRectHeight = 20;
    loadingRectMaxWidth = ofGetWidth()/2;
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
    ofSetColor(100);
    ofFill();
    ofRect(loadingRectStartX, loadingRectStartY, amountLoadedMapped, loadingRectHeight);
    ofNoFill();
    ofRect(loadingRectStartX, loadingRectStartY, loadingRectMaxWidth, loadingRectHeight);
}