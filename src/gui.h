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
    void displayButtons(int mx, int my);
    void updateTimeline(int seqNumber, int totalSeqs);
    void displayTimeline();
    void displayPlayIcon();
    void showGui();
    void checkTimer();
    void updateResumeMenu();
    void displayResumeMenu(int mx, int my);
    void selectResumeButton(int selectedButton);

    int loadingRectHeight;
    int resumeMenuNumButtons;
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
    bool resumeMenuShowing;
    string resumeMenuSelection;
    string resumeButtonValues[3];
    
    ofRectangle playButtonRect;
    ofRectangle buttonRects[6];
    ofRectangle resumeMenuButtons[3];
    ofRectangle toolbarRect;
    ofImage buttonImages[6];
    ofxTimer showGuiTimer;
    ofTrueTypeFont font;
};

