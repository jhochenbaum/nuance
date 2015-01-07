/*
  ==============================================================================

    SerialSetup.cpp
    Created: 30 Jul 2011 10:54:11pm
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#include "SerialSetup.h"

SerialSetup::SerialSetup (SerialDeviceManager& sdm) : serialDeviceManager (sdm) 
{
    addAndMakeVisible(serialInputListbox = new SerialInputSelectorComponentListBox(serialDeviceManager, "no serial devices avaialble", 0, 20));
    addAndMakeVisible(inputLabel = new Label());
    inputLabel->setText("active device inputs:", dontSendNotification);
}

SerialSetup::~SerialSetup(){
    serialDeviceManager.sendChangeMessage();  
}

void SerialSetup::paint (Graphics& g){
	g.fillAll(Colours::black);
    g.setColour(Colour::fromRGB(61,61,61));
	g.fillRoundedRectangle (2, 6, getWidth()-8, getHeight()-26, 10);	
	g.setColour(Colour::fromRGB(99,99,99));
	g.fillRoundedRectangle (4, 8, getWidth()-12, getHeight()-30, 10);
    
    const int lx = 140;
    //const int lx = proportionOfWidth (0.35f);
    const int w = proportionOfWidth (0.5f);
    const int h = 48;
    const int space = 48;
	int y = 20;
	
	const int bh = serialInputListbox->getBestHeight (jmin (h * 8, getHeight() - y - space - h));
	serialInputListbox->setBounds (lx, y, w, bh);
}

void SerialSetup::resized(){
    inputLabel->attachToComponent(serialInputListbox, true);
}

