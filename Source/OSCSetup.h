/*
 *  OSCSetup.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _OSCSETUP_
#define _OSCSETUP_

#include "JuceHeader.h"
#include "OSCListener.h"
#include "OscPacketListener.h"
#include "OscReceivedElements.h"
#include "OSCMessageListener.h"

//[/Headers]
//==============================================================================

class OSCSetup  :	public OSCMessageListener,
					public Component,
					public ButtonListener,
					public ChangeListener
{
	
public:
	//==============================================================================
	OSCSetup                    (OSCListener* listener_);
	~OSCSetup                   ();
	
	void paint                  (Graphics& g);
	void resized                ();
	
	void buttonClicked          (Button* buttonThatWasClicked);
	void changeListenerCallback (ChangeBroadcaster* source);
	//void myCallbackMethod (void* objectThatHasChanged); //callback for osc data

	//==============================================================================
	juce_UseDebuggingNewOperator
	
private:
	
	ScopedPointer<OSCListener> listener;
	//osc::ReceivedMessage* oscMessage;
	
	ScopedPointer<ToggleButton> connect;		//toggle button to start and stop connection (listening) for OSC messages
	ScopedPointer<TextEditor> recvAddress;	//text input field for listening ip address
	ScopedPointer<TextEditor> recvPort;		//text input field for listening port number
	ScopedPointer<TextEditor> oscMonitor;		//text window to dump all incoming OSC messages
	
	//GUI labels
	ScopedPointer<Label> addressLabel;		
	ScopedPointer<Label> portLabel;
    ScopedPointer<Label> oscMonitorLabel;
	Font textEditFont;
	
	//misc. variables
	String text;
	ScopedPointer<String> textPointer;
	String messageBundle;
	
	int lastTimeinMillis;
	bool print;
	int messageCounter; //counter incremented each time new OSC messages arrives in our callback
	int printCounter; //counter incremented each time we print a message
	// (prevent copy constructor and operator= being generated..)
	OSCSetup (const OSCSetup&);
	OSCSetup& operator= (const OSCSetup&);
};


#endif   // _OSCSETUP_
