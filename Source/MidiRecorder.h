/*
 ==============================================================================
 
 MidiRecorder.h
 Created: 23 Jul 2011 1:24:07pm
 Author:  Jordan Hochenbaum
 
 ==============================================================================
 */

#ifndef __MIDIRECORDER_H_91ABE3E6__
#define __MIDIRECORDER_H_91ABE3E6__

#include "Recorder.h"

class MidiRecorder  :	public Component,
                        public Recorder,
                        public MidiInputCallback,
                        public ComboBoxListener,
                        public ButtonListener,
                        public Timer,
                        public ChangeListener
{
	
public:
    
    MidiRecorder                    (AudioDeviceManager& adm);
    ~MidiRecorder                   ();
	
    void paint                      (Graphics& g);
    void resized                    ();
    void stop                       ();
    
    double sampleRate;    
    void setMidiData                (String _data);
    
    const float* getData            (int numSamples);
    
    void changeListenerCallback     (ChangeBroadcaster *source);
    void comboBoxChanged            (ComboBox* comboBoxThatHasChanged);
    void buttonClicked              (Button* buttonThatWasClicked){;};
    void buttonStateChanged         (Button* buttonThatWasClicked);
    void handleIncomingMidiMessage  (MidiInput *source, const MidiMessage &message);
    void addAudioCallbacks          ();
    void removeAudioCallbacks       ();
    void timerCallback              ();
    
    XmlElement* serialize();
    void restore (XmlElement* root);
    void setSelectedMidiDevice(String smd);
    void setSelectedCCNumber(String sccn);
    
	juce_UseDebuggingNewOperator
	
private:
    AudioDeviceManager& audioDeviceManager;
    
    float scaledData;
    void updateMidiDeviceList();
    void buildCCList();
    void dataReset();
    ScopedPointer<ToggleButton> inputEnable;
    StringArray midiDeviceList;
    String selectedMidiDevice;
    String selectedCCNumber;
    
    float* tempData;
    float sliderWidth;	//variable to control slider gui fill width
    
    ScopedPointer<Label> midiLabel;
    ScopedPointer<ComboBox> midiDeviceSelector;
    ScopedPointer<ComboBox> ccNumberSelector;
    
	// (prevent copy constructor and operator= being generated..)
	MidiRecorder (const MidiRecorder&);
	MidiRecorder& operator= (const MidiRecorder&);
};


#endif  // __MIDIRECORDER_H_91ABE3E6__
