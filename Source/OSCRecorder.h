/*
 *  OSCRecorder.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OSCRECORDER_
#define _OSCRRECORDER_

#include "JuceHeader.h"
#include "OscPacketListener.h"
#include "OSCMessageListener.h"
#include "Recorder.h"

class OSCRecorder  :	public Component,
                        public Recorder,
                        public OSCMessageListener, 
                        public Timer,
                        public ComboBoxListener,
                        public ButtonListener,
                        public TextEditorListener
{
	
public:
    
    OSCRecorder                     (AudioDeviceManager& adm);
    ~OSCRecorder                    ();
	
    void paint                      (Graphics& g);
    void resized                    ();
    void timerCallback              ();
   
    void stop                       ();

    double sampleRate;    
    void setOSCData                 (String _data);
    void buttonClicked              (Button* buttonThatWasClicked);
    void buttonStateChanged         (Button* buttonThatWasClicked);
    void myCallbackMethod           ( osc::ReceivedMessage objectThatHasChanged); //callback for osc data

    float getMin                    ();	//returns the min value to scale incoming data by
    float getMax                    ();	//returns the max value to scale incoming data by
    void setMin                     (float min); //set the min value to scale incoming data by
    void setMax                     (float max); //set the max value to scale incoming data by
    void setAddress                 (String ad); //set the osc address prefix to liste to
    const float* getData            (int numSamples);
    
    XmlElement* serialize();
    void restore (XmlElement* root);
    
    void addAudioCallbacks              ();
    void removeAudioCallbacks           ();
    
	juce_UseDebuggingNewOperator
	
private:
    AudioDeviceManager& audioDeviceManager;
    float* tempData;
    Font textEditFont;
    ScopedPointer<Label> oscLabel;
    ScopedPointer<ToggleButton> inputEnable;
    ScopedPointer<TextEditor> oscAddress;	//text input field for listening adress
    ScopedPointer<TextEditor> minVal;	//text input field for setting min value
    ScopedPointer<TextEditor> maxVal;	//text input field for setting max value
    ScopedPointer<Label> minLabel;
    ScopedPointer<Label> maxLabel;
    ScopedPointer<ComboBox> argumentSelector;
	
    bool* finishWriting;
    void comboBoxChanged                (ComboBox*);	
    void textEditorTextChanged          (TextEditor &editor){};
    void textEditorReturnKeyPressed     (TextEditor &editor);
    void textEditorEscapeKeyPressed     (TextEditor &editor){};
    void textEditorFocusLost            (TextEditor &editor){};
	
    String messageBundle;	//string to hold current incoming osc message as string
    float scaledData;
    float dataMin;	//min value to offset the incoming data by to put within our audio sample range of 0.0 - 1.0
    float dataMax;	//max value to divide the incoming data by to put within our audio sample range of 0.0 - 1.0
    float sliderWidth;	//variable to control slider gui fill width
    
	// (prevent copy constructor and operator= being generated..)
	OSCRecorder (const OSCRecorder&);
	OSCRecorder& operator= (const OSCRecorder&);
};


#endif   // _OSCRecorder