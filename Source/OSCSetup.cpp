/*
 *  OSCSetup.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "OSCSetup.h"

//==============================================================================
OSCSetup::OSCSetup (OSCListener* listener_)
{
	
	textEditFont.setHeight(12.0);
	listener = listener_;		  //make new OSCListener reference our global OSCListener
	listener->addChangeListener(this);
	
	//set up gui...
	addAndMakeVisible (connect = new ToggleButton ("Connect"));
	addAndMakeVisible(recvAddress = new TextEditor ("recvAddress"));
	addAndMakeVisible(recvPort = new TextEditor ("recvPort"));
	addAndMakeVisible(addressLabel = new Label ("ipAddress", "ip address:"));
	addAndMakeVisible(portLabel = new Label ("port","port:"));
	addAndMakeVisible(oscMonitor = new TextEditor ("oscMonitor"));
    addAndMakeVisible(oscMonitorLabel = new Label("oscMonitorLavel", "OSC Monitor"));
	
	connect->addListener(this);
	recvAddress->setFont(textEditFont);
	recvAddress->setText("localhost", false);
	recvPort->setFont(textEditFont);
	recvPort->setText("7000", false);
	connect->setToggleState(listener->isListening(), false); //check and set the current OSCListening state (in case we've already enabled OSC Listening)
	oscMonitor->setReadOnly(true);
	oscMonitor->setMultiLine(true,false);
	oscMonitor->setCaretVisible(false);
	//oscMonitor->setScrollToShowCursor(true);
	oscMonitor->setReturnKeyStartsNewLine(true);
	oscMonitor->setScrollbarsShown(true);
	oscMonitor->setScrollBarThickness(10);
	//listener->addListener(this);
    //setSize (500, 450);
}

OSCSetup::~OSCSetup(){
	listener->removeChangeListener(this);
    listener.release();
	//listener->removeListener(this);
	//deleteAllChildren();
}

//==============================================================================
void OSCSetup::paint (Graphics& g){
	g.fillAll(Colours::black);
    g.setColour(Colour::fromRGB(61,61,61));
	g.fillRoundedRectangle (2, 6, getWidth()-8, getHeight()-26, 10);	
	g.setColour(Colour::fromRGB(99,99,99));
	g.fillRoundedRectangle (4, 8, getWidth()-12, getHeight()-30, 10);
}

void OSCSetup::resized(){
	recvAddress->setBounds(100, 20, 100, 20);
	recvPort->setBounds(100, 45, 60, 20);
    connect->setBounds(210, 15, 20, 36);
	connect->changeWidthToFitText();
	oscMonitor->setBounds(100, 70, getWidth()/1.5, getHeight()/2.5-oscMonitor->getTextHeight());
    addressLabel->attachToComponent(recvAddress, true);
	portLabel->attachToComponent(recvPort, true);
    oscMonitorLabel->attachToComponent(oscMonitor, true);
}


void OSCSetup::buttonClicked (Button* buttonThatWasClicked){
    if (buttonThatWasClicked == connect){
		//if connect is true (start listening)
		if(connect->getToggleState()){
			//check to make sure both an ip address and port are specified
			if (recvAddress->getText() == "" || recvPort->getText() == ""){ //changed from NULL to ""
				//if not, deactivate the button and notify the user
				connect->setToggleState(false,false);
				oscMonitor->insertTextAtCaret("\n"); ///changed all of thesse to Caret instead of cursor
				oscMonitor->insertTextAtCaret("please specify an ip address and port!");
			}else{
				//if so, open up a new socket and begin listening for OSC messges
				listener->startListening(recvAddress->getText() , recvPort->getText().getIntValue());
				//listener->addChangeListener(this);
				oscMonitor->insertTextAtCaret("\n");	
				oscMonitor->insertTextAtCaret("connected");
			}
		}else{
			if (listener->isListening()){
				//listener->removeChangeListener(this);
				listener->stopListening();
				oscMonitor->insertTextAtCaret("\n");
				oscMonitor->insertTextAtCaret("not connected");
			}
		}
	}
}

void OSCSetup::changeListenerCallback(ChangeBroadcaster* objectThatHasChanged){
	oscMonitor->insertTextAtCaret("\n");
	oscMonitor->insertTextAtCaret(listener->getOSCAsString());
}

//=============================================================================
