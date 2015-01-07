/*
 *  OSCRecorder.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "OSCRecorder.h"
#include <iostream>

//==============================================================================

OSCRecorder::OSCRecorder(AudioDeviceManager& adm) : audioDeviceManager(adm), scaledData(0), sliderWidth(0)
{
	textEditFont.setHeight(12.0);
	
	addAndMakeVisible (oscLabel = new Label("oscLabel", "OSC Recorder"));	
	addAndMakeVisible (inputEnable = new ToggleButton ("on/off"));
	addAndMakeVisible(oscAddress = new TextEditor ("oscAddress"));
	addAndMakeVisible (minLabel = new Label("minLabel", "min"));
	addAndMakeVisible(minVal = new TextEditor ("minValue"));
	addAndMakeVisible (maxLabel = new Label("maxLabel", "max"));
	addAndMakeVisible(maxVal = new TextEditor ("maxValue"));
	addAndMakeVisible(argumentSelector = new ComboBox ("argumentSelector"));
	
	oscLabel->setEditable(false, true, false);
	inputEnable->addListener(this);
	oscAddress->setText("/address", false); //set default text to localhost
	//oscAddress->setBorder(0);
	oscAddress->addListener(this);
	minVal->setText("0.0", true); //set default text to 0 for min
	//minVal->setBorder(0);
	minVal->addListener(this);
	maxVal->setText("1.0", true); //set default text to localhost
	//maxVal->setBorder(0);
	maxVal->addListener(this);
	minVal->setFont(textEditFont);
	maxVal->setFont(textEditFont);
	minLabel->setFont(textEditFont);
	maxLabel->setFont(textEditFont);
	argumentSelector->setSelectedItemIndex(0);
	argumentSelector->addListener(this);	
	argumentSelector->setTextWhenNothingSelected("Select OSC Msg");
	oscAddress->setBounds(4, 24, getWidth()-8,getHeight()/6);
    
	startTimer (1000 / 50);  // repaint every 1/50 of a second
	
	dataMin = minVal->getText().getFloatValue();
	dataMax = maxVal->getText().getFloatValue();
    Recorder::setFilename("Osc_");
        
    inputEnable->setToggleState(true, true);
    
    addAudioCallbacks();

}

OSCRecorder::~OSCRecorder()
{
    stop();
    removeAudioCallbacks();
}

//==============================================================================

void OSCRecorder::paint (Graphics& g)
{
	g.setColour(Colour::fromRGB(61, 61, 61) );
	g.fillRoundedRectangle(0,0,getWidth(), getHeight(), 12);
	g.setColour(Colour::fromRGB(147, 147, 147));
	g.fillRoundedRectangle(2,2,getWidth()-4, getHeight()-4, 10);	
	g.setColour(Colour::fromRGB(209, 209, 209));
	g.fillRect (4.0, argumentSelector->getY()+argumentSelector->getHeight()+3.0, getWidth()-8, getWidth()/11.0); //slider
	g.setColour(Colours::yellow);
	g.fillRect (4.0, argumentSelector->getY()+argumentSelector->getHeight()+3.0, (getWidth()-8)*sliderWidth, getWidth()/11.0); //slider
}

void OSCRecorder::resized()
{
	oscLabel->setBounds(4,4,getWidth(),20);
	oscAddress->setBounds(4, 24, getWidth()-8,getHeight()/6);
	argumentSelector->setBounds (4, oscAddress->getY()+oscAddress->getHeight()+2, getWidth()-8,getHeight()/6);
	inputEnable->setBounds(4, argumentSelector->getY()+argumentSelector->getHeight()+getWidth()/8, getHeight()/5.5, getHeight()/5.5);
	inputEnable->changeWidthToFitText();	
	minVal->setBounds(getWidth()/1.8, inputEnable->getY()+2, getWidth()/4, getHeight()/7.5);
	maxVal->setBounds(minVal->getX(), minVal->getY()+minVal->getHeight()+1, minVal->getWidth(), minVal->getHeight());
    minLabel->setBounds(minVal->getX()+minVal->getWidth()-4, minVal->getY(), textEditFont.getHeight()*3, textEditFont.getHeight());
    maxLabel->setBounds(maxVal->getX()+maxVal->getWidth()-4, maxVal->getY(), textEditFont.getHeight()*3, textEditFont.getHeight());
}

void OSCRecorder::timerCallback(){
	//repaint();
    repaint(4.0, argumentSelector->getY()+argumentSelector->getHeight()+3.0, (getWidth()-8), getWidth()/11.0);
}


void OSCRecorder::stop(){
    Recorder::recOut->stop();
}


void OSCRecorder::buttonStateChanged (Button* buttonThatWasClicked){    
    if (buttonThatWasClicked == inputEnable){
        if (inputEnable->getToggleState()){
            Recorder::setShouldRecord( true );
        }else{
            Recorder::setShouldRecord( false );
        }
	}
}

void OSCRecorder::buttonClicked (Button* buttonThatWasClicked){
}

void OSCRecorder::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
    if (comboBoxThatHasChanged == argumentSelector) {
        Recorder::setFilename("Osc_Input "+String(argumentSelector->getSelectedItemIndex()+1));
    }
}

void OSCRecorder::textEditorReturnKeyPressed (TextEditor &editor){
	if (&editor == minVal){
		dataMin = minVal->getText().getFloatValue();
		//std::cout << "minVal: " << dataMin << std::endl;
	}
	else if (&editor == maxVal){
		dataMax = maxVal->getText().getFloatValue();
		//std::cout << "maxVal: " << dataMax << std::endl;
	}
	getParentComponent()->grabKeyboardFocus();
}

void OSCRecorder::myCallbackMethod ( osc::ReceivedMessage objectThatHasChanged){
	osc::ReceivedMessage oscMessage = objectThatHasChanged; //pass in the pointer to the oscMessage
	messageBundle = ""; //clear our string to hold the osc data ////////changed from NULL to ""
	
	//osc::ReceivedMessageArgumentStream args = oscMessage->ArgumentStream();
	osc::ReceivedMessage::const_iterator arg = oscMessage.ArgumentsBegin(); // create and start the osc argument iterator (needed?)
	
	//if the message was sent to the user specified osc address
	if ((oscAddress->getText().compare(oscMessage.AddressPattern())) == 0){
		if (argumentSelector->getNumItems() != oscMessage.ArgumentCount()){
			argumentSelector->clear(false);
			for (int i = 0; i<oscMessage.ArgumentCount(); i++){
				argumentSelector->addItem(String(i+1),i+1);	
			}
		}
		//for each argument sent
		for (int i = 0; i<oscMessage.ArgumentCount(); i++){
			//if the argument is the one we want to record
			if (i == argumentSelector->getSelectedItemIndex()){
				if( arg->IsBool() ){
					bool a = (arg)->AsBoolUnchecked();
					messageBundle.append((String)" ", 10);
					messageBundle.append((String)(i+1), 1);
					messageBundle.append((String)":", 1);
					messageBundle.append((String)a, 10);
				}else if( arg->IsInt32() ){
					int a = (arg)->AsInt32Unchecked();
					messageBundle.append((String)" ", 10);
					messageBundle.append((String)(i+1), 1);
					messageBundle.append((String)":", 1);
					messageBundle.append((String)a, 10);
				}else if( arg->IsFloat() ){
					float a = (arg)->AsFloatUnchecked();
					messageBundle.append((String)" ", 10);
					messageBundle.append((String)(i+1), 1);
					messageBundle.append((String)":", 1);
					messageBundle.append((String)a, 10);
				}else if( arg->IsString() ){
					const char *a = (arg)->AsStringUnchecked();
					messageBundle.append((String)" ", 10);
					messageBundle.append((String)(i+1), 1);
					messageBundle.append((String)":", 1);
					messageBundle.append((String)a, 10);
				}else{
					std::cout << "received" << oscMessage.AddressPattern() <<" message with unexpected argument type\n";
				}
				setOSCData(messageBundle);
			}
			arg++;
		}
		
	}
}

void OSCRecorder::setOSCData(String messageBundle_){
	messageBundle =  messageBundle_.substring(3);
	scaledData = (messageBundle.getFloatValue() - dataMin) / (dataMax - dataMin);
    sliderWidth = scaledData; //update GUI slider width
}

const float* OSCRecorder::getData(int numSamples){
    //create a float*, pack it with our last oscData reading, and pass it into monoChannelArray (Recorder::setData) for our writer to access via Recorder::getDataBufferToWrite()...
    tempData = new float[numSamples];
    std::fill_n(tempData, numSamples, scaledData);
    Recorder::setData(tempData);   
    return Recorder::getDataBufferToWrite();
}

void OSCRecorder::addAudioCallbacks()
{
    audioDeviceManager.addAudioCallback(getRecOut());
}
void OSCRecorder::removeAudioCallbacks()
{
    audioDeviceManager.removeAudioCallback(getRecOut());   
}

float OSCRecorder::getMin(){
	return minVal->getText().getFloatValue();
}

float OSCRecorder::getMax(){
	return maxVal->getText().getFloatValue();
}

void OSCRecorder::setMin(float _minVal){
	minVal->setText((String) _minVal,true);	
}

void OSCRecorder::setMax(float _maxVal){
	maxVal->setText((String) _maxVal,true);
}

void OSCRecorder::setAddress(String ad){
    oscAddress->setText(ad);
}

XmlElement* OSCRecorder::serialize ()
{
    // create an inner element..
    XmlElement* e = new XmlElement ("OSCRecorder");
    
    e->setAttribute ((String)"oscAddress", (String)oscAddress->getText());
    e->setAttribute ((String)"x", (String)getX());
    e->setAttribute ((String)"y", (String)getY());
    e->setAttribute ((String)"width", getWidth());
    e->setAttribute ((String)"height", getHeight());
    e->setAttribute ((String)"minVal", getMin());
    e->setAttribute ((String)"maxVal",  getMax());
    
    return e;
}

void OSCRecorder::restore (XmlElement* root)
{
}
