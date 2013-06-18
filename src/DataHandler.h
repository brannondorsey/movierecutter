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
    void eraseStorageFiles();
    int getPlayPointSeqIndex(const vector<Sequence> & sequences);
    long getPlayPoint();
    bool seqFileExists();
    
    string fileName;
    string filePath;
    ofxXmlSettings playPoint;
    ofxXmlSettings movieSequences;

  protected:
    string getFileNameFromPath(string filePath);


    
};