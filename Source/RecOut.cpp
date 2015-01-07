/*
 *  RecOut.cpp
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 4/7/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "RecOut.h"

RecOut::RecOut(Recorder* s) :  Thread("WriterClose Thread"), 
                    backgroundThread ("RecOut Thread"), 
                    sampleRate (0), 
                    activeWriter (0), 
                    channel(0)
{
    setSource(s);
    backgroundThread.startThread();
    startThread();
}

RecOut::~RecOut()
{
    this->notify();
    stopThread(2000);
    backgroundThread.stopThread(2000);
}


void RecOut::audioDeviceAboutToStart (AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

void RecOut::audioDeviceStopped()
{
    sampleRate = 0;
}

void RecOut::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples)
{
    
    if (isRecording())
    {        
        if(*finishWriting == false)
        {
            if( *countedIn == true){
            const float* monoChannelArray[1] = { source->getData(numSamples) };
            activeWriter->write(monoChannelArray, numSamples);
            }
        }else{
            if(*countedIn == false){
                if (fileToWrite.existsAsFile()) {
                    fileToWrite.deleteFile();
                }
            }
            this->notify();
        }
    }
    
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != 0)
            zeromem (outputChannelData[i], sizeof (float) * numSamples);
}

void RecOut::run()
{    
    while (!threadShouldExit()) 
    {
        activeWriter = nullptr;
        writerDeletionArray.add(threadedWriter.release());
        writerDeletionArray.clear(true); //must clear the array, which also deletes the writer
        threadedWriter = nullptr;

        this->wait(-1);
    }
}

void RecOut::startRecording (const File& file)
{
    //stop();
    if (sampleRate > 0)
    {
        fileToWrite = file;
        // Create an OutputStream to write to our destination file...
        //fileToWrite.deleteFile();
        ScopedPointer<FileOutputStream> fileStream (fileToWrite.createOutputStream());
        if (fileStream != nullptr)
        {
            // Now create a WAV writer object that writes to our output stream...
            WavAudioFormat wavFormat;
            AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);
            
            if (writer != nullptr)
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                
                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                
                threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                // And now, swap over our active writer pointer so that the audio callback will start using it..
                //const ScopedLock sl (writerLock);
                activeWriter = threadedWriter;
            }
        }else{
            std::cout << "error opening up filestream... " << std::endl;
        }
    }
}

void RecOut::stop()
{
    // First, clear this pointer to stop the audio callback from using our writer object..
    //{
    //    const ScopedLock sl (writerLock);
    //activeWriter = nullptr;
    //}
    //activeWriter = nullptr;
    // Now we can delete the writer object which happens in our deletion thread
    //writerDeletionArray.add(threadedWriter.release()); //release the ptr and pass the raw ptr into our array to be deleted by the deletion thread
    //this->notify(); //wake up the thread and instruct it to act...
}



void RecOut::setSource (Recorder* s){
    source = s;
}

void RecOut::setChannel(int chan) {
    channel = chan;
}

void RecOut::setGain(float g) {
    gain = g;
}

int RecOut::getChannel(){
    return channel; 
}

bool RecOut::isRecording() const
{
    return activeWriter != nullptr;
}
