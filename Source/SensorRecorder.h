/*
 *  SensorRecorder.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 5/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SENSORRECORDER_H_
#define _SENSORRECORDER_H_

#include "JuceHeader.h"
#include "Recorder.h"
#include "SerialDeviceManager.h"

class SerialDeviceManager;

class SensorRecorder  :	public Component,
                        public Recorder,
						public Timer,
						public ButtonListener,
						public ComboBoxListener,
						public TextEditorListener,
                        public ChangeListener

{
	
public:
	SensorRecorder (AudioDeviceManager& adm, SerialDeviceManager& sdm);
	~SensorRecorder();
	
	void paint (Graphics& g);
    void resized();
	void timerCallback();
    void stop();
	
    void changeListenerCallback (ChangeBroadcaster *source);
	void comboBoxChanged        (ComboBox*);	//callback for serialDevice list drop-down menu
	void myCallbackMethod       (void* objectThatHasChanged); //callback for sensor data
	void buttonClicked          (Button* buttonThatWasClicked);
	void buttonStateChanged     (Button* buttonThatWasClicked);
    const float* getData        (int numSamples);
    
	String getProtocol          ();	//returns the String filepath to the selected sensor protocol file
	void setProtocol            (String _protocolPath);	//sets the String filepath to the selected sensor protocol file
	void selectProtocolWindow   (); //opens a file select window, and allows users to select a sensor protocol config file path to pass to loadProtocol
    float getMin                ();	//returns the min value to scale incoming data by
	float getMax                ();	//returns the max value to scale incoming data by
	void setMin                 (float min); //set the min value to scale incoming data by
	void setMax                 (float max); //set the max value to scale incoming data by
    void setSelectedDevice      (String devName);
    void setSelectedSensor      (String selSen);
	
    String getPortPath();

	StringPairArray portList;		
	int selectedSerialDevice;
	
	ScopedPointer<ComboBox> serialDeviceSelector;
	ScopedPointer<ComboBox> sensorSelector;
	void setSensorData(String _temp);
	
    String tempSerialName;
    
    XmlElement* serialize();
    void restore (XmlElement* root);
    
    void addAudioCallbacks();
    void removeAudioCallbacks();
	
private:
    AudioDeviceManager& audioDeviceManager;
    SerialDeviceManager& serialDeviceManager;
    String portPath;
    String deviceName;
	ScopedPointer<Label> sensorLabel;
	Font textEditFont;
	
	String sensorStartMessage;
	String protocolPath;
	
	void updateDeviceList();
	
	ScopedPointer<ToggleButton> inputEnable;
	ScopedPointer<ToggleButton> loadSerialProtocolButton;
	ScopedPointer<Label> minLabel;
	ScopedPointer<Label> maxLabel;
	ScopedPointer<TextEditor> minVal;	//text input field for setting min value
	ScopedPointer<TextEditor> maxVal;	//text input field for setting max value
	
	ScopedPointer<XmlElement> mainElement;
	StringPairArray serialDeviceList;
	
	void textEditorTextChanged (TextEditor &editor){};
	void textEditorReturnKeyPressed (TextEditor &editor);
	void textEditorEscapeKeyPressed (TextEditor &editor){};
	void textEditorFocusLost (TextEditor &editor){};
	
	float* tempData; 
	float scaledData;
	float dataMin;	//min value to offset the incoming data by to put within our audio sample range of 0.0 - 1.0
	float dataMax;	//max value to divide the incoming data by to put within our audio sample range of 0.0 - 1.0
	float sliderWidth;
	
	bool* finishWriting;
	
};



#endif//_AUDIORECORDER_H_