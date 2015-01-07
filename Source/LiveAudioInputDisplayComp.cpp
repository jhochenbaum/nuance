/*
 *  LiveAudioInputDisplayComp.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 3/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "LiveAudioInputDisplayComp.h"

LiveAudioInputDisplayComp::LiveAudioInputDisplayComp(): nextSample(0), subSample(0), accumulator(0), gainFactor(1.0),channel(-1)
{
    zeromem (samples, sizeof (samples));
    setOpaque (true);
    startTimer (1000 / 50); // use a timer to keep repainting this component	
}

LiveAudioInputDisplayComp::~LiveAudioInputDisplayComp()
{
}

void LiveAudioInputDisplayComp::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    g.setColour (Colours::lightgrey);
    const float midY = getHeight() * 0.5f;
    int sampleNum = (nextSample + numElementsInArray (samples) - 1);
	
    for (int x = jmin (getWidth(), numElementsInArray (samples)); --x >= 0;)
    {
        const float sampleSize = midY * samples [sampleNum-- % numElementsInArray (samples)];
        g.drawVerticalLine (x, midY - sampleSize, midY + sampleSize);
    }	
}

void LiveAudioInputDisplayComp::timerCallback()
{
    repaint();
}

void LiveAudioInputDisplayComp::audioDeviceAboutToStart (AudioIODevice* device)
{
    zeromem (samples, sizeof (samples));
}

void LiveAudioInputDisplayComp::audioDeviceStopped()
{
    zeromem (samples, sizeof (samples));
}

void LiveAudioInputDisplayComp::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                                       float** outputChannelData, int numOutputChannels, int numSamples)
{
    if (channel != -1){
		for (int i = 0; i < numSamples; ++i)
		{
			for (int chan = 0; chan < numInputChannels; ++chan)
			{
				if (inputChannelData[channel] != 0)
					accumulator += fabsf (inputChannelData[channel][i]);
				
			}
			
			const int numSubSamples = 100; // how many input samples go onto one pixel.
			const float boost = 2*gainFactor;     // how much to boost the levels to make it more visible.
			
			if (subSample == 0)
			{
				samples[nextSample] = accumulator * boost / numSubSamples;
				nextSample = (nextSample + 1) % numElementsInArray (samples);
				subSample = numSubSamples;
				accumulator = 0;
			}
			else
			{
				--subSample;
			}
		}
	}
	
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != 0)
            zeromem (outputChannelData[i], sizeof (float) * numSamples);
}

void LiveAudioInputDisplayComp::setChan(int _chan){
    channel = _chan;
    std::cout << "audioInputChan: " << channel << std::endl;
}
void LiveAudioInputDisplayComp::setGainFactor(float _gain){
	gainFactor = _gain;
}