/*
  ==============================================================================

    SerialSetup.h
    Created: 30 Jul 2011 10:54:11pm
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#ifndef __SERIALSETUP_H_A852A856__
#define __SERIALSETUP_H_A852A856__

#include "JuceHeader.h"
#include "SerialDeviceManager.h"
#include "SerialInputSelectorComponentListBox.h"

class SerialSetup  :	public Component
{
    
public:
	SerialSetup             (SerialDeviceManager& sdm);
	~SerialSetup            ();
    
	void paint              (Graphics& g);
	void resized            ();
    
	juce_UseDebuggingNewOperator
    
private:
	SerialDeviceManager& serialDeviceManager;
    ScopedPointer<SerialInputSelectorComponentListBox> serialInputListbox;
    ScopedPointer<Label> inputLabel;
    
    // (prevent copy constructor and operator= being generated..)
    //AudioSetup (const AudioSetup&);
    //AudioSetup& operator= (const AudioSetup&);
};


#endif  // __SERIALSETUP_H_A852A856__
