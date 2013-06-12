#include "ofxXmlSettings.h"
#include "sequence.h"

class DataHandler{
  public:
    DataHandler(){
    };
    
    DataHandler(string _filePath);
    
    void savePlayPoint(long currentFrame);
    void saveSequences(const vector<Sequence> & sequences);
    void loadSequences(vector<Sequence> & sequences);
    bool seqFileExists();
    long getPlayPoint();
    
    string fileName;
    string filePath;
    ofxXmlSettings playPoint;
    ofxXmlSettings movieSequences;

  private:
    string getFileNameFromPath(string filePath);
    
};