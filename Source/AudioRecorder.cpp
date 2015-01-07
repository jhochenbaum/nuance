/*
 *  AudioRecorder.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 11/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 *	Huge thanks and appreciation to Jules / JUCE for much of the code below which has been directly borrowed and/or changed from the JUCE help documention and forums online.
 */

#include "AudioRecorder.h"
#include <iostream>

AudioRecorder::AudioRecorder(AudioDeviceManager& adm) : audioDeviceManager(adm), sampleRate(0), channel(-1)//, savePath("default")
{
    //recorder = new RecOut();
	setOpaque (false);
	recordButtonShape.addEllipse (10.0f, 10.0f, 10.0f, 10.0f);
	addAndMakeVisible (audioLabel = new Label("audioLabel", "Audio Recorder"));	
	addAndMakeVisible (liveAudioDisplayComp = new LiveAudioInputDisplayComp());
    addAndMakeVisible (inputChannelSelector = new ComboBox ("input"));
	addAndMakeVisible (gain = new Slider("Gain"));
	addAndMakeVisible (gainLabel = new Label("gainLabel", "Gain"));
	addAndMakeVisible (inputEnable = new ToggleButton ("input enable"));
	
	audioLabel->setEditable(false, true, false);
	inputChannelSelector->setSelectedItemIndex(0);
	inputChannelSelector->addListener(this);	
	inputChannelSelector->setTextWhenNothingSelected("Select Input Channel");
	
	gain->addListener(this);
	gain->setSliderStyle(Slider::LinearBar);
	gain->toFront(false);
	gain->setRange(0.001, 2.0, .01); //.001 minimum prevents weirdness of multiplying the input gain by a factor of 0.0
	gain->setValue(1.0);
	gain->setTextBoxIsEditable(false); 
	//gain->setDoubleClickReturnValue(true, 1.0); //enable for double-click resetting to 1.0
	
	inputEnable->setButtonText("");
	inputEnable->addListener(this);
	inputEnable->setColour(TextButton::buttonColourId, Colours::white);
	inputEnable->setColour(TextButton::textColourOnId, Colours::white);
	inputEnable->setToggleState(true, false);
	
    Recorder::setFilename( "Audio_"+inputChannelSelector->getItemText(inputChannelSelector->getSelectedId()) ); // initialize fileName for output recording .wav

	setWantsKeyboardFocus(false);
   
    addAudioCallbacks();
    createListOfInputChannels(audioDeviceManager.getCurrentAudioDevice());
}

AudioRecorder::~AudioRecorder(){
    removeAudioCallbacks();
}

void AudioRecorder::paint (Graphics& g){
	
	g.setColour(Colour::fromRGB(61, 61, 61));
    g.fillRect(0, 0, getWidth(), getHeight());
    g.setColour(Colour::fromRGB(147, 147, 147));
    g.fillRect(2, 2, getWidth()-4, getHeight()-4);
	//g.setColour(Colours::darkgrey);
	//g.drawRect (getWidth()/1.9, getHeight()/1.9, getHeight()/4.4, getHeight()/4.6,1);
	
}

void AudioRecorder::resized(){
	
	liveAudioDisplayComp->setBounds (2, 2, getWidth()-4, getHeight()/2); //(x, y, width, height)
	inputChannelSelector->setBounds (3, getHeight()/1.85, getWidth()/2, getHeight()/4.7);	
	gain->setBounds(3, getHeight()/1.32, getWidth()/2, getHeight()/4.7);
	gainLabel->setBounds(getWidth()/1.9, getHeight()/1.6, 40,40);
	inputEnable->setBounds(getWidth()/1.9, getHeight()/2, getHeight()/4.1, getHeight()/4.1);
	audioLabel->setBounds(getWidth()-getWidth()/4, getHeight()/1.3, getWidth()/4, getHeight()/4);
}

void AudioRecorder::stop(){
    removeAudioCallbacks();
	Recorder::recOut->stop();
}

void AudioRecorder::audioDeviceAboutToStart (AudioIODevice* device)
{
    sampleRate = device->getCurrentSampleRate();
}

void AudioRecorder::audioDeviceStopped()
{
    sampleRate = 0;
}

void AudioRecorder::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples)
{
    if (channel >= 0 && channel < numInputChannels) {
        Recorder::setData(inputChannelData[channel]);
    }
    
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != 0)
            zeromem (outputChannelData[i], sizeof (float) * numSamples);
}

void AudioRecorder::createListOfInputChannels(AudioIODevice* _audioIODevice){
	inputChannelSelector->clear(); // clear the current contents in our comboBox's
	inputChannelSelector->addItem("none", -1);	// add "no input"
	// add all input channel names to channel select comboBox
	for (int i = 0; i< _audioIODevice->getInputChannelNames().size(); i++) {
		inputChannelSelector->addItem(_audioIODevice->getInputChannelNames()[i], i+1); // add "1" because and we add "none" input to the first index
	}	
}

const float* AudioRecorder::getData(int numSamples){
    return Recorder::getDataBufferToWrite();
    //return monoChannelArray[1];
}


//-----------------------------------Input Channel Selector--------------------------------//
void AudioRecorder::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
	
	// test to see if selected channel's ID is = -1, if it is, ignore updating filename, if not, update default save filename; always set channel.
	if (inputChannelSelector->getSelectedId() == -1){	
		setChannel(-1);
	}
	else{
		setChannel(inputChannelSelector->getSelectedId()-1);
        Recorder::setFilename( "Audio_"+inputChannelSelector->getItemText(inputChannelSelector->getSelectedId()) ); 	// update fileName for output recording .wav
	}
}
	
void AudioRecorder::buttonStateChanged (Button* buttonThatWasClicked){
	
	if (buttonThatWasClicked == inputEnable){
        if (inputEnable->getToggleState()){
            Recorder::setShouldRecord(true);
        }else{
            Recorder::setShouldRecord(false);
        }
	}
}

void AudioRecorder::buttonClicked (Button* buttonThatWasClicked){
}

void AudioRecorder::sliderValueChanged(Slider* gain){
	setGain(gain->getValue());
	liveAudioDisplayComp->setGainFactor(getGain());
	Recorder::recOut->setGain(getGain());
}

void AudioRecorder::addAudioCallbacks (){
    audioDeviceManager.addAudioCallback(this);    
    audioDeviceManager.addAudioCallback(Recorder::recOut);
    audioDeviceManager.addAudioCallback(liveAudioDisplayComp);
}

void AudioRecorder::removeAudioCallbacks(){
    audioDeviceManager.removeAudioCallback(this);    
    audioDeviceManager.removeAudioCallback(Recorder::recOut);
    audioDeviceManager.removeAudioCallback(liveAudioDisplayComp);
}

void AudioRecorder::setGain(float g){
    Recorder::setGain(g);
    gain->setValue(g);
}

void AudioRecorder::setChannel(int _channel){
	channel = _channel; 
    //Recorder::setChannel(_channel);
	recOut->setChannel(channel);
    liveAudioDisplayComp->setChan(channel);
    inputChannelSelector->setSelectedId(channel+1);
}

int AudioRecorder::getChannel(){
	return channel;
}


LiveAudioInputDisplayComp* AudioRecorder::getWaveformDisplay() {
	return liveAudioDisplayComp;	
}

XmlElement* AudioRecorder::serialize ()
{
    // create an inner element..
    XmlElement* e = new XmlElement ("AudioRecorder");
    
    e->setAttribute ((String)"x", (String)getX());
    e->setAttribute ((String)"y", (String)getY());
    e->setAttribute ((String)"width", getWidth());
    e->setAttribute ((String)"height", getHeight());
    e->setAttribute ((String)"gain", getGain());
    e->setAttribute ((String)"channel", getChannel());
    
    return e;
}

void AudioRecorder::restore (XmlElement* root)
{
}

