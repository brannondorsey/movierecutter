#pragma once
#include "ofMain.h"

class Gui
{
public:
    //default constructor
    Gui();
    
    void initTotalNumFrames(long _totalNumFrames);
    void updateLoading(long& totalChecked);
    void displayLoading();

    float loadingRectStartX;
    float loadingRectStartY;
    float loadingRectMaxWidth;
    float amountLoadedMapped;
    int loadingRectHeight;
    long totalNumFrames;

};

