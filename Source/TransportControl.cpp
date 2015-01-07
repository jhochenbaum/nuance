/*
 *  TransportControls.cpp
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 3/6/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TransportControl.h"
#include "GuiCommands.h"

TransportControl::TransportControl(Metronome* metro){
    //addAndMakeVisible(barCounter = new TextPropertyComponent(metro->getBeatCounter(), "barCount", 8, false));
    //barCounter->setInterceptsMouseClicks(false, false);

    barCount = 1; quarterCount = 1;
	//define shape paths
	triPath.addTriangle(8, 8, 8, 10, 10, 9);
	circlePath.addEllipse(8, 8, 4, 4);
	metroPath.startNewSubPath(0,0);
	metroPath.addEllipse(2, 8, 2, 2);
	metroPath.closeSubPath();
	metroPath.startNewSubPath(0,0);
	metroPath.addEllipse(10, 8, 2, 2);
	metroPath.closeSubPath();
	//apply path to our DrawablePaths
	playOffTriangle.setPath(triPath);
	playOnTriangle.setPath(triPath);
	playOnTriangle.setStrokeType(PathStrokeType::PathStrokeType(1));
	playOnTriangle.setStrokeFill(FillType::FillType(Colours::lime));
	recOffCircle.setPath(circlePath);
	recOnCircle.setPath(circlePath);
	recOnCircle.setStrokeType(PathStrokeType::PathStrokeType(1));
	recOnCircle.setFill(FillType::FillType(Colours::red));
	recOnCircle.setStrokeFill(FillType::FillType(Colours::red));
	metroIcon.setPath(metroPath);
	metroIcon.setStrokeType(PathStrokeType::PathStrokeType(3));

	addAndMakeVisible(bpmEditor = new TextBoxSlider("bpmEditor"));
	bpmEditor->setValue(120.0, false, false);
	//bpmEditor->setColour(TextBoxSlider::textColourId, Colours::grey);
	//bpmEditor->setColour(TextEditor::highlightColourId, Colours::grey);
	//bpmEditor->setColour(TextEditor::backgroundColourId, Colours::darkgrey);
	//bpmEditor->setColour(TextEditor::outlineColourId, Colours::darkgrey);
	//bpmEditor->setColour(TextEditor::focusedOutlineColourId, Colours::darkgrey);
	//textEditFont.setHeight(10);
	//bpmEditor->setFont(textEditFont);
	//bpmEditor->setText("120", true);

	//create our button and apply our DrawablePaths to the button for various states
	addAndMakeVisible(playButton = new DrawableButton("playButton", DrawableButton::ImageFitted));
	addAndMakeVisible(recordButton = new DrawableButton("recordButton", DrawableButton::ImageFitted));
	addAndMakeVisible(metroButton = new DrawableButton("metroButton", DrawableButton::ImageFitted));
	addAndMakeVisible(barLabel = new Label("barLabel", "0.0"));
	//barLabel->setFont(textEditFont);
	playButton->setImages(&playOffTriangle, &playOffTriangle, &playOffTriangle, 0, &playOnTriangle);
	playButton->setClickingTogglesState(true);
	recordButton->setImages(&recOffCircle, &recOffCircle, &recOffCircle, 0, &recOnCircle);
	recordButton->setClickingTogglesState(true);
	metroButton->setImages(&metroIcon, &metroIcon, &metroIcon, 0, &metroIcon);
	metroButton->setClickingTogglesState(true);
	//listen to spacebar presses
	//playButton->addShortcut(32);
    
    playButton->setColour(DrawableButton::backgroundColourId, Colours::darkgrey);
    playButton->setColour(DrawableButton::backgroundOnColourId, Colours::darkgrey);
    recordButton->setColour(DrawableButton::backgroundColourId, Colours::darkgrey);
    recordButton->setColour(DrawableButton::backgroundOnColourId, Colours::darkgrey);
    metroButton->setColour(DrawableButton::backgroundColourId, Colours::darkgrey);
    metroButton->setColour(DrawableButton::backgroundOnColourId, Colours::yellow);
	//setWantsKeyboardFocus(true);
}

TransportControl::~TransportControl() {
	
}

void TransportControl::paint (Graphics& g) {
	g.setColour(Colours::black);
	g.fillRoundedRectangle(0, 0, 200, 19, 1);
	g.setColour(Colours::darkgrey);
	//g.fillRect(2, 2, 16, 15);
	//g.fillRect(20, 2, 16, 15);
	g.fillRect(38, 2, 48, 15);
	g.fillRect(106, 2, 50, 15);
}	

void TransportControl::resized() {
	playButton->setBounds(2,2,16,15);
	recordButton->setBounds(20,2,16,15);
	barLabel->setBounds(38,2,48,15);
	metroButton->setBounds(88, 2, 16, 15);
	bpmEditor->setBounds(104, 2, 80, 15);
    //barCounter->setBounds(38, 2, 48, 15);
}

void TransportControl::registerListener(ButtonListener* bl) {
	playButton->addListener(bl);
	recordButton->addListener(bl);
	metroButton->addListener(bl);
}

void TransportControl::registerBPMListener(LabelListener* ll) {
	bpmEditor->addListener(ll);	
}

void TransportControl::changeListenerCallback(ChangeBroadcaster* broadcaster )
{
    
	if (firstTime){
        barCount = 1;
        quarterCount = 1;
		firstTime = false;
		
	}else{
		if (quarterCount%4 == 0){
            barCount++;
            quarterCount = 0;
		}
        
        quarterCount++;
	}
	setBarCountDisplay(String(barCount)+ "." + String(quarterCount));
}

/*
void TransportControl::myCallbackMethod( int beatCount){
	

    quarterCount = beatCount+1;
	
	if (!firstTime){
		if (quarterCount%4 == 0){
			barCount++;
		}
	}else{
		barCount = 1;	
		firstTime = false;
	}
	setBarCountDisplay(String(barCount)+ "." + String(quarterCount));
}*/

void TransportControl::setPlayButtonState(bool s) {
	playButton->setToggleState(s, true);
	if (playButton->getToggleState()) {
		firstTime = true;
	}
}

void TransportControl::setBarCountDisplay(String barCounter) {
    //const MessageManagerLock mml;
    barLabel->setText(barCounter, dontSendNotification);
    repaint();
}

bool TransportControl::getPlayButtonState() {
	return playButton->getToggleState();	
}

bool TransportControl::getRecordButtonState() {
	return recordButton->getToggleState();
}

bool TransportControl::getMetroButtonState() {
	return metroButton->getToggleState();
}

DrawableButton* TransportControl::getPlayButton() {
	return playButton;
}

DrawableButton* TransportControl::getMetroButton() {
	return metroButton;
}


ApplicationCommandTarget* TransportControl::getNextCommandTarget (void) {
	return findFirstTargetParentComponent();
}

void TransportControl::getAllCommands (Array< CommandID > &commands) {
	commands.clear();
	
	ShortcutItems itemCur;
	for (itemCur = (ShortcutItems)APP_BASE; itemCur < APP_TOP; itemCur = (ShortcutItems)((int)itemCur+1)) {
		switch (itemCur) {
			case APP_TP_PLAY:
				commands.add(itemCur);
				break;
			case APP_TP_REC:
				commands.add(itemCur);
				break;
			
			default:
				break;
		}
	}
}
void TransportControl::getCommandInfo (CommandID commandID, ApplicationCommandInfo &result) {
	switch ((ShortcutItems)commandID)
    {
       
		case APP_TP_PLAY:
			result.setInfo ("Play", "Start/Stop Play", "Transport", 0);
			break;
        case APP_TP_REC:
			result.setInfo ("Record", "Start/Stop Record", "Transport", 0);
			break;
				
		default:
			break;
    }
}
bool TransportControl::perform (const InvocationInfo &info){

	switch (info.commandID)
    {
        case APP_TP_PLAY:
            setPlayButtonState(!getPlayButtonState());
			break;
		case APP_TP_REC:
			recordButton->setToggleState(!recordButton->getToggleState(), true);
			break;
		
        default:
			return false;
    }
	
	return true;
}
