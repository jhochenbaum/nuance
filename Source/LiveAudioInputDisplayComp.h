/*
 *  LiveAudioDisplayComp.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "JuceHeader.h"

class LiveAudioInputDisplayComp  :	public Component,
									public AudioIODeviceCallback,
									public Timer
{

public:
    //==============================================================================
    LiveAudioInputDisplayComp();
    ~LiveAudioInputDisplayComp();
	
    void paint (Graphics& g);
    void timerCallback();
	
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, int numSamples);
	void setChan(int channel);
	void setGainFactor(float gain);

private:
    float samples [1024];
    int nextSample, subSample;
    float accumulator;
	float gainFactor;
	
	int channel;
	
    LiveAudioInputDisplayComp (const LiveAudioInputDisplayComp&);
    LiveAudioInputDisplayComp& operator= (const LiveAudioInputDisplayComp&);
};