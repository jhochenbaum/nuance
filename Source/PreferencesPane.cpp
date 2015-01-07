/*
 *  PreferencesPane.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PreferencesPane.h"

PreferencesPane::PreferencesPane (AudioDeviceManager& audioDeviceManager_, OSCListener* listener_, SerialDeviceManager& serialDeviceManager_) : tabbedComponent (0)
{
	//instantiate out tabbed component, and add out audio/osc tabs which are also instantiated when they are added
	addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtLeft));
	tabbedComponent->setTabBarDepth (30);
	tabbedComponent->setOutline(0);
	tabbedComponent->addTab ("audio", Colours::darkgrey, audiosetup = new AudioSetup(audioDeviceManager_), false);
    tabbedComponent->addTab ("osc", Colours::darkgrey, oscsetup = new OSCSetup(listener_), true);
    tabbedComponent->addTab("serial", Colours::darkgrey, serialsetup = new SerialSetup(serialDeviceManager_), false);
	setSize (600, 400);
	tabbedComponent->setBounds (0, 0, getWidth(), getHeight());
}

PreferencesPane::~PreferencesPane()
{
	
}

//==============================================================================
/*void PreferencesPane::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}*/

/*void PreferencesPane::resized()
{
    tabbedComponent->setBounds (0, 0, getWidth(), getHeight());
}*/

//==============================================================================