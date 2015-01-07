/*
 *  AudioSetup.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _AUDIOSETUP_
#define _AUDIOSETUP_

#include "JuceHeader.h"


class AudioSetup  :	public Component
{

public:
	AudioSetup (AudioDeviceManager& audioDeviceManager_);
	~AudioSetup();
		
	void							paint (Graphics& g);
	void							resized();
		
	juce_UseDebuggingNewOperator
		
private:
	AudioDeviceManager&				audioDeviceManager;
	ScopedPointer<AudioDeviceSelectorComponent>	audioSettingsComp;
		
		// (prevent copy constructor and operator= being generated..)
		//AudioSetup (const AudioSetup&);
		//AudioSetup& operator= (const AudioSetup&);
};


#endif   // _AUDIOSETUP_
