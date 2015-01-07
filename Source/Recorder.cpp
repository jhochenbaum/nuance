/*
 ==============================================================================
 
 Recorder.cpp
 Created: 12 Jul 2011 1:41:47am
 Author:  Jordan Hochenbaum
 
 ==============================================================================
 */

#include "Recorder.h"
#include <iostream>

Recorder::Recorder() :  Thread("Recorder Thread"), savePath("default"), gain(1.0)
{
    recOut = new RecOut(this); //create our RecOut and pass in 'this' so that it calls our callback to receive new data every Audio Callback
    startThread();
}

Recorder::~Recorder(){
    stopThread(2000);
}

void Recorder::prepareRecording()
{
	//create file for recording and name it according to set name
	if (shouldRecord == true & getRecOut()->getChannel() >=0){
        
        if (savePath != "default"){
            fileToRecord = File(savePath).getNonexistentChildFile (fileName, ".wav");
            Recorder::recOut->startRecording(fileToRecord);
        }else{
            fileToRecord = File(File::getSpecialLocation (File::userDocumentsDirectory) //working on this section to dynamically create/name .wav files
                                     .getNonexistentChildFile (fileName, ".wav"));
            Recorder::recOut->startRecording(fileToRecord);
        }
	}
}

void Recorder::stop(){
    //recOut->stop();
}

void Recorder::run()
{    
    while (!threadShouldExit()) 
    {
        this->wait(-1);
    }
}

void Recorder::setChannel(int chan){
    channel = chan;
}

void Recorder::setData (const float* data){
    monoChannelArray[1] = data;
}

/*const float* Recorder::getData(int numSamples){
    return monoChannelArray[1];
}*/

bool Recorder::isRecording() const{
	return recOut->isRecording();
}

void Recorder::setShouldFinishWriting(bool* finishUp, bool* countIn){
	finishWriting = finishUp;	//set out local bool* to point to our flag set in MainComponent
	countedIn = countIn;
    recOut->countedIn = countIn;
    recOut->finishWriting = finishUp;
}

void Recorder::setShouldRecord(bool sr){
    shouldRecord = sr;
}


//sets the fileName to append to the savePath
void Recorder::setFilename(String _fileName){
	fileName = _fileName;
}

//sets the savePath used for writing the files to disk
void Recorder::setSavePath(String _pathName){
	savePath = _pathName;
}

//method access to the (private String) filename set for the recorder from outside
String Recorder::getFilename(){
	return fileName;
}


float Recorder::getGain(){
	return gain;
}

void Recorder::setGain(float g){
	gain = g;
}


RecOut* Recorder::getRecOut() {
	return recOut;	
}

