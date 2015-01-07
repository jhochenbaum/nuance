/*
 ==============================================================================
 
 SerialInputSelectorComponent.h
 Created: 31 Jul 2011 2:28:09am
 Author:  Jordan Hochenbaum
 
 ==============================================================================
 */

#ifndef __SERIALINPUTSELECTORCOMPONENT_H_5E7EA0EF__
#define __SERIALINPUTSELECTORCOMPONENT_H_5E7EA0EF__

#include "JuceHeader.h"
#include "SerialDeviceManager.h"

class SerialInputSelectorComponentListBox  :    public ListBox,
                                                public ListBoxModel
{
    
public:
    SerialInputSelectorComponentListBox(SerialDeviceManager& sdm, const String& noItemsMessage_, 
											   const int minNumber_, const int maxNumber_); 	
    
    ~SerialInputSelectorComponentListBox();
        
	void paint (Graphics& g);
    void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected);
	
    void listBoxItemClicked (int row, const MouseEvent& e);	
    void listBoxItemDoubleClicked (int row, const MouseEvent&);	
    void returnKeyPressed (int row);
	
	int getNumRows();
    int getBestHeight (const int preferredHeight);
	
    void refresh();
		
    juce_UseDebuggingNewOperator
	
private:
    SerialDeviceManager& serialDeviceManager;
	
    const String noItemsMessage;
    StringArray items, itemsAvailable;
    int minNumber, maxNumber;
	    
    void flipEnablement (const int row);	
    int getTickX() const;
    	
};



#endif  // __SERIALINPUTSELECTORCOMPONENT_H_5E7EA0EF__
