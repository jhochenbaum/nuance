/*
  ==============================================================================
 
 *  MainComponent.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 11/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 
 This is the "MainComponent" class-- which is the main/starting point/display of the application.
 All general intefrace elements & GUI controls are created here.
 
// TODO: - Make a generic DeviceManager class which includes the audioDeviceManager, serialDeviceManager...etc
         - Once this is done, finish implemented their built in addCallbacks and removeCallbacks

 
// TODO: Create one large OwnedArray for all recorders (using their Recorder base class)
         and handle all registering / de-registering in the implementations themselves
         rather than in MainComponent
  
// TODO: Finish reserialization refactoring: Abstract majority of component re-loading into Recorders themselves and remove           
         from MainComponent
 
  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__
#define __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__

#include "JuceHeader.h"
#include "ComponentLayoutEditor.h"
#include "GuiCommands.h"
#include "AudioRecorder.h"
#include "SerialDeviceManager.h"
#include "SensorRecorder.h"
#include "OSCRecorder.h"
#include "MidiRecorder.h"

#include "PreferencesPane.h"
#include "Metronome.h"
#include "TransportControl.h"

#include "OscReceivedElements.h"
#include "UdpSocket.h"
#include "OscPacketListener.h"
#include "OSCListener.h"

class MainComponent  : public Component,
                       public ButtonListener,
					   public ChangeListener,
					   public KeyListener,
					   public ApplicationCommandTarget,
					   public MenuBarModel
					  // public LassoSource <Component*>

{
	
public:
    MainComponent						();
    ~MainComponent						();
    
    void paint							(Graphics& g);
    void resized						();
    void buttonClicked					(Button* buttonThatWasClicked);	//callback for all main GUI buttons
    bool keyPressed						(const KeyPress  &key, Component  *originatingComponent);
    bool keyStateChanged				(bool isKeyDown, Component *originatingComponent);
    void changeListenerCallback			(ChangeBroadcaster *source);	//callback for passing serialData from SerialManager to SensorRecorder
    void chooseSavePath					();	//open dialog for selecting save location
    void setSavePath					(File savePath);	// save save location for output .wav files
    String getSavePath					();	//get save location for output .wav files
    
    StringArray getMenuBarNames         ();	//Returns the names of the menus.
    PopupMenu	getMenuForIndex         (int topLevelMenuIndex, const String &menuName);	//Returns a PopupMenu for the required menu.
    void menuItemSelected				(int menuItemID, int topLevelMenuIndex);	//Called when a menu item has been clicked.
    bool panelToggle					(); //switch between editing object placement and locking object placement
    void setCommandManager				(ApplicationCommandManager* _commandManager);
    //void findLassoItemsInArea (Array <Component*>& results, int x, int y, int width, int height);
    //SelectedItemSet <Component*>& getLassoSelection();
    
    //application command methods
    ApplicationCommandTarget* getNextCommandTarget ();
    void getAllCommands					(Array< CommandID > &commands);
    void getCommandInfo					(CommandID commandID, ApplicationCommandInfo &result);
    bool perform						(const InvocationInfo &info);
	
private:    
    ApplicationCommandManager*          commandManager;
    ScopedPointer<Metronome>            metro;
    AudioSourcePlayer                   metroPlayer;
    ScopedPointer<TransportControl>     transportBar;
    OSCListener	listener;
    LassoComponent<Component*>          lassoComp;
    
    void initialiseAudio                ();
    void startStopTransport				();
    void startStopRecording				();
    
    //main GUI Objects
    ScopedPointer<ComponentLayoutEditor> editor; //enabling this allows sibling components to be repositioned.
    bool isEditable;
    String editText;
    bool finishWritingToDisk;
    bool countedIn;
	
    //device management
    AudioDeviceManager audioDeviceManager;	// this wraps the actual audio device
    SerialDeviceManager serialDeviceManager;
    
    OwnedArray<AudioRecorder, CriticalSection> audioRecorderManager; //array to hold AudioRecorders
    OwnedArray<SensorRecorder, CriticalSection> sensorRecorderManager;	//array to hold SensorRecorders
    OwnedArray<OSCRecorder, CriticalSection> oscRecorderManager;	//array to hold OSCRecorders
    OwnedArray<MidiRecorder, CriticalSection> midiRecorderManager;	//array to hold OSCRecorders
	
    void addAudioRecorder				(int x, int y, int w, int h, float _gain, int _chan);
    void addSensorRecorder				(int x, int y, int w, int h, float min, float max, String protocol, String ss, String sd);
    void addOSCRecorder					(int x, int y, int w, int h, float min, float max, String ad);
    void addMidiRecorder                (int x, int y, int w, int h, String md, String cc);
	
    ScopedPointer<XmlElement> mainElement;
    ScopedPointer<XmlElement> loadAudioConfigElement;
    void saveConfiguration				(File _savePath);
    void loadConfiguration				(File _loadPath);
    void openPreferences				();
	
    File saveFolder;
    
	//*****CALLBACK METHODS*****//
    void mouseDown						(const MouseEvent  &e);
    void mouseDrag						(const MouseEvent  &e);
    void mouseUp						(const MouseEvent  &e);
	
    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainComponent (const MainComponent&);
    const MainComponent& operator= (const MainComponent&);
	
	
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_D0F6CD31__
