/*
  ==============================================================================

    Recorder.h
    Created: 12 Jul 2011 1:41:47am
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

// TODO: - Remove thread...

#ifndef __RECORDER_H_3629F865__
#define __RECORDER_H_3629F865__

#include "JuceHeader.h"
#include "Serializable.h"
#include "RecOut.h"

class RecOut;

class Recorder : public Serializable<XmlElement>,
                 public Thread
{
    
public:
    Recorder                            ();
    virtual ~Recorder                   ();
    
	void setFilename                    (String _fileName);
	void setSavePath                    (String _pathName);
	String getFilename                  ();
    void setData                        (const float* data);
    virtual const float* getData        (int numSamples) = 0;
    virtual void addAudioCallbacks      () =0;
    virtual void removeAudioCallbacks   () = 0;
    const float* getDataBufferToWrite   () {return monoChannelArray[1];};
	void setGain                        (float g);
	float getGain                       ();
	void prepareRecording               ();
	void stop                           ();
	bool isRecording                    () const;
	void setShouldFinishWriting         (bool* finishUp, bool* countIn);
    void setShouldRecord                (bool sr);
    void setChannel                     (int chan);
    //void setName                        (const String n);

    ScopedPointer<RecOut> recOut;
	RecOut* getRecOut();
    
    void run();
   
private:
    File fileToRecord;
    String name;
    const float* monoChannelArray[1]; //the buffer which stores our current data were recording
    int channel;
	String _inputDeviceName;
	String fileName;
	String savePath;
    bool* finishWriting;
    bool* countedIn;
    bool shouldRecord;
    float gain; //this is the gain amount that will be applied to the output
        
};



#endif  // __RECORDER_H_3629F865__
