/*
 ==============================================================================
 
 SerialInputSelectorComponent.cpp
 Created: 31 Jul 2011 2:28:09am
 Author:  Jordan Hochenbaum
 
 ==============================================================================
 */

#include "SerialInputSelectorComponentListBox.h"

SerialInputSelectorComponentListBox::SerialInputSelectorComponentListBox (SerialDeviceManager& sdm, const String& noItemsMessage_,
                                                                          const int minNumber_, const int maxNumber_)
                                                                        : ListBox (String::empty, 0), serialDeviceManager(sdm), 
                                                                        noItemsMessage (noItemsMessage_), minNumber (minNumber_),
                                                                        maxNumber (maxNumber_)
{
    refresh();
	setModel (this);
	setOutlineThickness (1);
}

SerialInputSelectorComponentListBox::~SerialInputSelectorComponentListBox()
{
}

//==============================================================================

int SerialInputSelectorComponentListBox::getNumRows()
{
	return itemsAvailable.size();
}

void SerialInputSelectorComponentListBox::paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected)
{
	if (isPositiveAndBelow (row, itemsAvailable.size()))
	{
		if(rowIsSelected)
			g.fillAll (findColour (TextEditor::highlightColourId).withMultipliedAlpha (0.3f));
		
		const String item (itemsAvailable[row]);

		bool enabled = serialDeviceManager.isSerialPortOpen(item);
		const int x = getTickX();
		const float tickW = height * 0.75f;
		
		getLookAndFeel().drawTickBox (g, *this, x - tickW, (height - tickW) / 2, tickW, tickW, enabled, true, true, false);
		
		g.setFont (height * 0.6f);
		g.setColour (findColour (ListBox::textColourId, true).withMultipliedAlpha (enabled ? 1.0f : 0.6f));
		g.drawText (item, x, 0, width - x - 2, height, Justification::centredLeft, true);
	}
}

void SerialInputSelectorComponentListBox::listBoxItemClicked (int row, const MouseEvent& e)
{
	selectRow(row);
	
	if (e.x < getTickX())
		flipEnablement (row);
}

void SerialInputSelectorComponentListBox::listBoxItemDoubleClicked (int row, const MouseEvent&)
{
	flipEnablement (row);
}

void SerialInputSelectorComponentListBox::returnKeyPressed (int row)
{
	flipEnablement (row);

}


void SerialInputSelectorComponentListBox::paint (Graphics& g)
{
	ListBox::paint (g);
	
	if (itemsAvailable.size() == 0)
	{
		g.setColour (Colours::grey);
		g.setFont (13.0f);
		g.drawText (noItemsMessage, 0, 0, getWidth(), getHeight() / 2, Justification::centred, true);
	}
}


int SerialInputSelectorComponentListBox::getBestHeight (const int preferredHeight)
{
	const int extra = getOutlineThickness() * 2;
	
    return jmax (getRowHeight() * 2 + extra, jmin (getRowHeight() * getNumRows() + extra + getRowHeight(), preferredHeight));
	
}


void SerialInputSelectorComponentListBox::flipEnablement (const int row)
{
	if (isPositiveAndBelow (row, itemsAvailable.size()))
	{
		const String item (items [row]);
        serialDeviceManager.setSerialPortEnabled(item);
        repaint();
	}
    
}

int SerialInputSelectorComponentListBox::getTickX() const
{
	return getRowHeight() + 5;
}

void SerialInputSelectorComponentListBox::refresh()
{
    itemsAvailable.clear();
    items.clear();
    
	items = serialDeviceManager.getAllDevices().getAllKeys();
    
    for(int i = 0; i < items.size(); i++)
     {
         itemsAvailable.addIfNotAlreadyThere(items[i]);
     }
       
    updateContent();
    repaint();
}