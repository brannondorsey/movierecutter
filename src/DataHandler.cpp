#include "DataHandler.h"

//--------------------------------------------------------------
DataHandler::DataHandler(string _filePath){
    filePath = _filePath;
    fileName = getFileNameFromPath(filePath);
}

//--------------------------------------------------------------
void DataHandler::loadSequences(){
    
}

//--------------------------------------------------------------
void DataHandler::saveSequences(){
    
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
void DataHandler::savePlayPoint(long currentFrame){
    playPoint.setValue("playpoint", ofToString(currentFrame), 0);
    playPoint.saveFile("storage/"+fileName+".playpoint");
}

//--------------------------------------------------------------
string DataHandler::getFileNameFromPath(string filePath){
    int startIndex = filePath.find_last_of('/')+1;
    string fileNameWithExt = filePath.substr(startIndex);
    int endIndex = fileNameWithExt.find('.');
    string fileName = fileNameWithExt.substr(0, endIndex);
    return fileName;
}

