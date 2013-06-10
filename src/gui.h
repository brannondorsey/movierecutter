#pragma once
#include "ofMain.h"
#include "sequence.h"

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
    ofRectangle playButtonRect;
    ofRectangle buttonRects[5];
    ofImage buttonImages[5];

};

