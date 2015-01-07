/*
 *  WavOut.h
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 4/7/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 *  //TODO: - Put writeLocks back??
    //      - Add Gain Multiplier
 */

#ifndef _RECOUT_
#define _RECOUT_

#include "JuceHeader.h"
#include "Recorder.h"

class Recorder;

class RecOut  : public AudioIODeviceCallback, public Thread
{
public:
    
    RecOut                          (Recorder* s);
    ~RecOut                         ();
    
    void startRecording             (const File& file);
    void stop                       ();
    bool isRecording                () const;
    void audioDeviceAboutToStart    (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback      (const float** inputChannelData,
                                       int totalNumInputChannels,
                                       float** outputChannelData,
                                       int totalNumOutputChannels,
                                       int numSamples);
    
    void setSource                  (Recorder* source);
    void setChannel                 (int chan);
    int getChannel                  ();
    void setGain                    (float g);
    CriticalSection& getWriterLock  () {return writerLock;}
    void run                        ();
    bool* finishWriting;
    bool* countedIn;
private:
    
    File fileToWrite;
    Recorder* source; //the Recorder object which will supply us with our data / buffer to write
    TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    OwnedArray<AudioFormatWriter::ThreadedWriter> writerDeletionArray; //this array holds ptrs to the threadedWriters needed to be deleted
    double sampleRate;
    
    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
    
    int channel;
    float gain;
};

#endif //_RECOUT_
