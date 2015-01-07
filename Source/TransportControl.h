/*
 *  TransportControl.h
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 3/6/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _TRANSPORTCTRL_
#define _TRANSPORTCTRL_

#include "JuceHeader.h"
#include "TextBoxSlider.h"
//#include "MetronomeBeatCountListener.h"
#include "Metronome.h"

class TransportControl : public Component,
						 public ApplicationCommandTarget,
                         public ChangeListener
						 //public MetronomeBeatCountListener

{

public:
	TransportControl				(Metronome* metro);
	~TransportControl				();
	
	void paint                      (Graphics& g);
    void resized();
	void setPlayButtonState         (bool s);
	bool getPlayButtonState         ();
	bool getRecordButtonState       ();
	bool getMetroButtonState        ();
	void registerListener           (ButtonListener* bl);
	void registerBPMListener        (LabelListener* ll);
	void setBarCountDisplay         (String barCounter);
	
	//void myCallbackMethod           ( int beatCount );
    
    void changeListenerCallback( ChangeBroadcaster* broadcaster);
	
	//application command methods
	ApplicationCommandTarget*       getNextCommandTarget ();
	void getAllCommands             (Array< CommandID > &commands);
	void getCommandInfo             (CommandID commandID, ApplicationCommandInfo &result);
	bool perform                    (const InvocationInfo &info);
	
	
	DrawableButton* getPlayButton();
	DrawableButton*	getMetroButton();

private:
    ScopedPointer<TextPropertyComponent> barCounter;
	ScopedPointer<TextBoxSlider> bpmEditor;
	ScopedPointer<DrawableButton> playButton;
	ScopedPointer<DrawableButton> recordButton;
	ScopedPointer<DrawableButton> metroButton;
	ScopedPointer<Label> barLabel;
	Path triPath;
	Path circlePath;
	Path metroPath;
	DrawablePath playOffTriangle;
	DrawablePath playOnTriangle;
	DrawablePath recOffCircle;
	DrawablePath recOnCircle;
	DrawablePath metroIcon;
	bool metroState;
	bool firstTime;
	int quarterCount, barCount;
	//Font					textEditFont;
	
};

#endif //_TRANSPORTCTRL_