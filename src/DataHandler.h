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
    int getPlayPointSeqIndex(const vector<Sequence> & sequences);
    bool seqFileExists();
    
    string fileName;
    string filePath;
    ofxXmlSettings playPoint;
    ofxXmlSettings movieSequences;

  protected:
    long getPlayPoint();
    string getFileNameFromPath(string filePath);


    
};