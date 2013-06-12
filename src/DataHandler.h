#include "ofxXmlSettings.h"

class DataHandler{
  public:
    DataHandler(){
    };
    
    DataHandler(string _filePath);
    
    void savePlayPoint(long currentFrame);
    void loadSequences();
    void saveSequences();
    long getPlayPoint();
    
    string fileName;
    string filePath;
    ofxXmlSettings playPoint;
    ofxXmlSettings movieSequences;

  private:
    string getFileNameFromPath(string filePath);
    
};