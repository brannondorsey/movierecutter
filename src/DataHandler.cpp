#include "DataHandler.h"

//--------------------------------------------------------------
DataHandler::DataHandler(string _filePath){
    filePath = _filePath;
    fileName = getFileNameFromPath(filePath);
}

//--------------------------------------------------------------
void DataHandler::loadSettings(int& pixelThreshold, float& imgPercentNeededToPass,
                               int& checkEveryIncrement, int& numCheckEachFrame){
    ofxXmlSettings settings;
    settings.loadFile("settings.xml");
    settings.pushTag("settings");
    //percent similarity each individual pixel needs to pass
    pixelThreshold = settings.getValue("pixel_threshold", 25);
    //percent of pixels that must pass threshold test
    imgPercentNeededToPass = float(settings.getValue("img_percent_needed_to_pass", 80));
    //number of pixels before next pixel comparison
    checkEveryIncrement = float(settings.getValue("check_every_increment", 10));
    //number of movie frames to set each frame of the program
    numCheckEachFrame = settings.getValue("num_check_each_frame", 40);
    settings.popTag();
    //cout<<"The loaded settings are "<<pixelThreshold<<", "<<imgPercentNeededToPass<<", "<<checkEveryIncrement<<", "<<numCheckEachFrame<<endl;
}


//--------------------------------------------------------------
bool DataHandler::seqFileExists(){
    if(movieSequences.loadFile("storage/"+fileName+".sequences")){
        return true; //loaded file
    }
    else return false; //file doesn't exist
}

//--------------------------------------------------------------
void DataHandler::loadSequences(vector<Sequence> & sequences){
    movieSequences.pushTag("reorderedsequences");
    int numSequences = movieSequences.getNumTags("sequence");
    for(int i = 0; i < numSequences; i++){
        movieSequences.pushTag("sequence", i);
        long start = movieSequences.getValue("start", 0);
        long stop = movieSequences.getValue("stop", 0);
        movieSequences.popTag();
        sequences.push_back(Sequence(start, stop));
    }
    movieSequences.popTag();
    cout<<"just loaded the sequence file"<<endl;
}


//--------------------------------------------------------------
void DataHandler::saveSequences(const vector<Sequence> & sequences){
    cout<<"attempting to save the sequences"<<endl;
    cout<<"there are "<<sequences.size()<<" sequences to save"<<endl;
    movieSequences.addTag("reorderedsequences");
    movieSequences.pushTag("reorderedsequences");
    for(int i = 0; i < sequences.size(); i++){
        movieSequences.addTag("sequence");
        movieSequences.pushTag("sequence", i);
        movieSequences.addTag("start");
        movieSequences.addTag("stop");
        movieSequences.setValue("start", ofToString(sequences[i].start));
        movieSequences.setValue("stop", ofToString(sequences[i].stop));
        movieSequences.popTag();
        cout<<"just saved sequence "<<i<<endl;
    }
    movieSequences.popTag();
    movieSequences.saveFile("storage/"+fileName+".sequences");
    cout<<"just saved the sequence file"<<endl;
}


//--------------------------------------------------------------
void DataHandler::savePlayPoint(long currentFrame){
    playPoint.clear();
    playPoint.setValue("playpoint", ofToString(currentFrame), 0);
    playPoint.saveFile("storage/"+fileName+".playpoint");
}

//--------------------------------------------------------------
long DataHandler::getPlayPoint(){
    if(playPoint.loadFile("storage/"+fileName+".playpoint")){
        return playPoint.getValue("playpoint", 0);
    }
    else{
        playPoint.setValue("playPoint", 1);
        return 1;
    }
}

//--------------------------------------------------------------
int DataHandler::getPlayPointSeqIndex(const vector<Sequence> & sequences){
    int loadedPlayPoint = getPlayPoint();
    if(loadedPlayPoint == 0) return loadedPlayPoint; //when movie ends it saves the playpoint to 0. This returns the seqIndex to the begining of sequences.
    for(int i = 0; i < sequences.size(); i++){
        int start = sequences[i].start;
        int stop = sequences[i].stop;
        if((loadedPlayPoint >= start) &&
           (loadedPlayPoint <= stop)){
            cout<<"The playpoint was "<<loadedPlayPoint<<endl;
            cout<<"The playpoint was inside cut number "<<i<<endl; 
            return i;
        }
    }
    return -1; //if there is an error return -1 (which should break something)
}

//--------------------------------------------------------------
void DataHandler::eraseStorageFiles(){
    //delete sequence and playpoint files
    ofFile playPointFile(ofToDataPath("storage/"+fileName+".playpoint"));
    ofFile sequencesFile(ofToDataPath("storage/"+fileName+".sequences"));
    playPointFile.remove();
    sequencesFile.remove();
}

//--------------------------------------------------------------
string DataHandler::getFileNameFromPath(string filePath){
    int startIndex = filePath.find_last_of('/')+1;
    string fileNameWithExt = filePath.substr(startIndex);
    int endIndex = fileNameWithExt.find('.');
    string fileName = fileNameWithExt.substr(0, endIndex);
    return fileName;
}

