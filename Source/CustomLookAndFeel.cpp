/*
 *  CustomLookAndFeel.cpp
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 2/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel() 
{
    Colour backgroundGrey = Colour::fromRGB(209,209,209);
    Colour lightGrey = Colour::fromRGB(99,99,99);
    Colour darkGrey = Colour::fromRGB(61,61,61);
    
    //buttons
	this->setColour(TextButton::buttonColourId, Colours::grey);
	
	//sliders
	this->setColour(Slider::thumbColourId, Colours::yellow);
	this->setColour(Slider::backgroundColourId, backgroundGrey);
	this->setColour(Slider::textBoxTextColourId, Colours::black);
    this->setColour(Slider::textBoxHighlightColourId, Colours::black);
    this->setColour(Slider::textBoxOutlineColourId, Colours::black);
	
	//other
	this->setColour(PopupMenu::backgroundColourId, backgroundGrey);
	this->setColour(ComboBox::buttonColourId, Colours::grey);
    this->setColour(ComboBox::backgroundColourId, backgroundGrey);
    this->setColour(ComboBox::outlineColourId, Colours::black);
    //this->setColour(ComboBox::focusedOutlineColourId, Colours::black);
	this->setColour(TextEditor::outlineColourId, Colours::black);
	this->setColour(TextEditor::focusedOutlineColourId, Colours::black);
    this->setColour(TextEditor::backgroundColourId, backgroundGrey);
    this->setColour(ListBox::backgroundColourId, backgroundGrey);
    this->setColour(ListBox::outlineColourId, Colours::black);
    
}

CustomLookAndFeel::~CustomLookAndFeel()
{

}

void CustomLookAndFeel::drawToggleButton (Graphics& g,
                                          ToggleButton& button,
                                          bool isMouseOverButton,
                                          bool isButtonDown)
{
    float fontSize = jmin (15.0f, button.getHeight() * 0.75f);
    const float tickWidth = fontSize * 1.1f;
    
    drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
                 tickWidth, tickWidth,
                 button.getToggleState(),
                 button.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);
    
    g.setColour (button.findColour (ToggleButton::textColourId));
    g.setFont (fontSize);
    
    if (! button.isEnabled())
        g.setOpacity (0.5f);
    
    const int textX = (int) tickWidth + 5;
    
    g.drawFittedText (button.getButtonText(),
                      textX, 0,
                      button.getWidth() - textX - 2, button.getHeight(),
                      Justification::centredLeft, 10);
}

void CustomLookAndFeel::drawTextEditorOutline (Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (textEditor.isEnabled())
    {
        
            g.setColour (textEditor.findColour (TextEditor::outlineColourId));
            g.drawRect (0, 0, width, height);
            
            g.setOpacity (1.0f);
            const Colour shadowColour (textEditor.findColour (TextEditor::shadowColourId));
            drawBevel (g, 0, 0, width, height + 2, 3, shadowColour, shadowColour);
    }
}

int CustomLookAndFeel::getDefaultScrollbarWidth()
{
    return 14;
}

void CustomLookAndFeel::drawComboBox (Graphics& g, int width, int height,
                                const bool isButtonDown,
                                int buttonX, int buttonY,
                                int buttonW, int buttonH,
                                ComboBox& box)
{    
    g.fillAll (box.findColour (ComboBox::backgroundColourId));
    
    if (box.isEnabled() && box.hasKeyboardFocus (false))
    {
        g.setColour (box.findColour (ComboBox::outlineColourId));
        g.drawRect (0, 0, width, height);
    }
    else
    {
        g.setColour (box.findColour (ComboBox::outlineColourId));
        g.drawRect (0, 0, width, height);
    }
    
    const float outlineThickness = 1;
    //const float outlineThickness = box.isEnabled() ? (isButtonDown ? 1.2f : 0.5f) : 0.3f;
    
    const Colour baseColour (createBaseColour (box.findColour (ComboBox::buttonColourId),
                                                                   box.hasKeyboardFocus (true),
                                                                   false, isButtonDown)
                             .withMultipliedAlpha (box.isEnabled() ? 1.0f : 0.5f));
    
    drawGlassLozenge (g,
                      buttonX + outlineThickness, buttonY + outlineThickness,
                      buttonW - outlineThickness * 2.0f, buttonH - outlineThickness * 2.0f,
                      baseColour, outlineThickness, -1.0f,
                      true, true, true, true);
    
    if (box.isEnabled())
    {
        const float arrowX = 0.3f;
        const float arrowH = 0.2f;
        
        Path p;
        p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.45f - arrowH),
                       buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.45f,
                       buttonX + buttonW * arrowX,          buttonY + buttonH * 0.45f);
        
        p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
                       buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
                       buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);
        
        g.setColour (box.findColour (ComboBox::arrowColourId));
        g.fillPath (p);
    }
}

const Colour CustomLookAndFeel::createBaseColour (const Colour& buttonColour,
                               const bool hasKeyboardFocus,
                               const bool isMouseOverButton,
                               const bool isButtonDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));
    
    if (isButtonDown)
        return baseColour.contrasting (0.2f);
    else if (isMouseOverButton)
        return baseColour.contrasting (0.1f);
    
    return baseColour;
}