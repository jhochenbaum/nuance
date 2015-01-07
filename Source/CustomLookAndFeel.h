/*
 *  CustomLookAndFeel.h
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 2/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CUSTLOOKANDFEEL_
#define _CUSTLOOKANDFEEL_

#include "JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V2
{
public:
	CustomLookAndFeel();
	~CustomLookAndFeel();
    
private:
    void drawToggleButton       (Graphics& g,
                                 ToggleButton& button,
                                 bool isMouseOverButton,
                                 bool isButtonDown);
    
    void drawTextEditorOutline      (Graphics& g, int width, int height, TextEditor& textEditor);
    int getDefaultScrollbarWidth    ();
    void drawComboBox               (Graphics& g, int width, int height,
                                    const bool isButtonDown,
                                    int buttonX, int buttonY,
                                    int buttonW, int buttonH,
                                    ComboBox& box);
    
    
    const Colour createBaseColour (const Colour& buttonColour,
                                   const bool hasKeyboardFocus,
                                   const bool isMouseOverButton,
                                   const bool isButtonDown) noexcept;
};

#endif //_CUSTLOOKANDFEEL_