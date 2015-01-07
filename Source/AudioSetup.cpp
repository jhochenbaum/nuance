/*
 *  AudioSetup.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioSetup.h"

AudioSetup::AudioSetup (AudioDeviceManager& audioDeviceManager_) : audioDeviceManager (audioDeviceManager_), audioSettingsComp (0){
	addAndMakeVisible (audioSettingsComp = new AudioDeviceSelectorComponent (audioDeviceManager, 0, 16, 0, 16, true, false, false, false));
    //setSize (500, 450);
}

AudioSetup::~AudioSetup(){

}

void AudioSetup::paint (Graphics& g){
	g.fillAll(Colours::black);
    g.setColour(Colour::fromRGB(61,61,61));
	g.fillRoundedRectangle (2, 6, getWidth()-8, getHeight()-26, 10);	
	g.setColour(Colour::fromRGB(99,99,99));
	g.fillRoundedRectangle (4, 8, getWidth()-12, getHeight()-30, 10);
}

void AudioSetup::resized(){
	audioSettingsComp->setBounds (0, 0, getWidth() , getHeight());
}
	

