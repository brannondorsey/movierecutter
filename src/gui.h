#pragma once
#include "ofMain.h"
#include "sequence.h"
#include "ofxTimer.h"

class Gui
{
public:
    //default constructor
    Gui();
    
    void initTotalNumFrames(long _totalNumFrames);
    void updateLoading(long& totalChecked);
    void displayLoading();
    void updateButtons();
    void displayButtons(int mx, int my);
    void updateTimeline(int seqNumber, int totalSeqs);
    void displayTimeline();
    void displayPlayIcon();
    void showGui();
    void checkTimer();

    int loadingRectHeight;
    int numButtons;
    int buttonSize;
    int buttonMargin;
    int buttonsY;
    long totalNumFrames;
    float loadingRectStartX;
    float loadingRectStartY;
    float loadingRectMaxWidth;
    float amountLoadedMapped;
    float timelineStartX;
    float timelineWatchPointX;
    float timelineWidth;
    bool isShowing;
    ofRectangle playButtonRect;
    ofRectangle buttonRects[5];
    ofImage buttonImages[5];
    ofxTimer showGuiTimer;

};

