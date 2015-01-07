/*
 *  PreferencesPane.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PreferencesPane_H_
#define _PreferencesPane_H_

#include "JuceHeader.h"
#include "OSCListener.h"
#include "AudioSetup.h"
#include "OSCSetup.h"
#include "SerialSetup.h"

class PreferencesPane:  public Component	

{
public:
	//==============================================================================
	PreferencesPane(AudioDeviceManager& audioDeviceManager_ , OSCListener* listener_, SerialDeviceManager& serialDeviceManager_);
	~PreferencesPane();
	
	//void paint (Graphics& g);
	//void resized();		
	
	juce_UseDebuggingNewOperator
	
	
private:
	
	ScopedPointer<TabbedComponent> tabbedComponent;
	ScopedPointer<AudioSetup> audiosetup;
	ScopedPointer<OSCSetup> oscsetup;
    ScopedPointer<SerialSetup> serialsetup;
	
	// (prevent copy constructor and operator= being generated..)
	PreferencesPane (const PreferencesPane&);
	const PreferencesPane& operator= (const PreferencesPane&);
};




#endif  //_PreferencesPane_H_