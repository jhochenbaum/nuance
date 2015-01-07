/*
 *  AudioRecorder.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 11/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 * //  TODO: Finish dynamic file renaming
 * //  TODO: Add Stereo Channel Recording
 * //  TODO: Remove Select Input Channel, default to 1
 */

#ifndef _AUDIORECORDER_H_
#define _AUDIORECORDER_H_

#include "JuceHeader.h"
#include "LiveAudioInputDisplayComp.h"
#include "Recorder.h"

class AudioRecorder  :  public Component,
                        public Recorder,
                        public ComboBoxListener,
						public SliderListener,
						public ButtonListener,
                        public AudioIODeviceCallback

{

public:
	  AudioRecorder                 (AudioDeviceManager& adm);
	 ~AudioRecorder                 ();
	
	void paint                      (Graphics& g);
    void resized                    ();
    void audioDeviceAboutToStart    (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback      (const float** inputChannelData,
                                     int totalNumInputChannels,
                                     float** outputChannelData,
                                     int totalNumOutputChannels,
                                     int numSamples);
	
	void setGain                    (float g);
    void setChannel                 (int channel);
	int	 getChannel                 ();
	void createListOfInputChannels  (AudioIODevice*);
	void comboBoxChanged            (ComboBox*);
	void buttonClicked              (Button* buttonThatWasClicked);
	void buttonStateChanged         (Button* buttonThatWasClicked);
	void sliderValueChanged         (Slider*);

	ScopedPointer<ComboBox> inputChannelSelector;
	    
    void stop                       ();
    
    const float* getData    (int numSamples);
    LiveAudioInputDisplayComp* getWaveformDisplay();
    
    XmlElement* serialize();
    void restore (XmlElement* root);
    
    void addAudioCallbacks          ();
    void removeAudioCallbacks       ();

private:
    AudioDeviceManager& audioDeviceManager;
    ScopedPointer<LiveAudioInputDisplayComp>   liveAudioDisplayComp;
    double sampleRate;
	int	channel;
    //float currentInputLevel;
	String _inputDeviceName;
	//String fileName;
	//String savePath;
    bool* finishWriting;	
    
	ScopedPointer<Label> audioLabel;
	ScopedPointer<Slider> gain;
	ScopedPointer<Label> gainLabel;
	ScopedPointer<ToggleButton>	inputEnable;
	Path recordButtonShape;
    
};



#endif//_AUDIORECORDER_H_