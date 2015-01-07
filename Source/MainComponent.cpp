#include "MainComponent.h"

MainComponent::MainComponent () : isEditable(false), editText("Edit Panel"), finishWritingToDisk(true), countedIn(false)
{	    
    
    addKeyListener(this);
	this->setWantsKeyboardFocus(true);
	
    initialiseAudio(); //open up and initialise our audio card	

	//Create and set up Metronome and Transport
	metro = new Metronome(&countedIn);
	metroPlayer.setSource(metro);
	metroPlayer.setGain(0.7);
    audioDeviceManager.addAudioCallback(&metroPlayer);
    
    addAndMakeVisible(transportBar = new TransportControl(metro));
	transportBar->registerListener(this);
    transportBar->registerBPMListener(metro);
    metro->addChangeListener(transportBar);
    
}


MainComponent::~MainComponent(){
	
    //Delete all recorders...
	if (audioRecorderManager.size() > 0)
		audioRecorderManager.clear(true);
	
    if (sensorRecorderManager.size() > 0)
		sensorRecorderManager.clear(true);
    
	if (oscRecorderManager.size() > 0){
		for (int i=0; i<oscRecorderManager.size(); i++){
			listener.removeListener(oscRecorderManager[i]);
		}
		oscRecorderManager.clear(true);
	}
	
    if (midiRecorderManager.size() > 0)
		midiRecorderManager.clear(true);
	
	audioDeviceManager.removeAudioCallback(&metroPlayer);
    
    //Save our audio settings so they can be restored upon next time
	ScopedPointer<XmlElement> saveAudioDeviceXmlElement(audioDeviceManager.createStateXml());
	if(saveAudioDeviceXmlElement != 0)
		saveAudioDeviceXmlElement->writeToFile(File(File::getSpecialLocation(File::commonApplicationDataDirectory).getFullPathName() + "/FlipMu/Nuance/lastAudioConfig.xml"), String::empty, "UTF-8", 200);
	
	removeChildComponent (&lassoComp);
}

void MainComponent::paint (Graphics& g){
    g.setColour(Colour::fromRGB(61,61,61));
	g.fillRoundedRectangle (2, 30, getWidth()-8, getHeight()-50, 10);	
	g.setColour(Colour::fromRGB(99,99,99));
	g.fillRoundedRectangle (4, 32, getWidth()-12, getHeight()-54, 10);	
}

void MainComponent::resized(){
	transportBar->setBounds((getWidth()/2)-250, 4, 155,19);
}

void MainComponent::initialiseAudio(){
    //load configuration from file into an XML document
	XmlDocument lastAudioDeviceManagerSettings (File(File::getSpecialLocation(File::commonApplicationDataDirectory).getFullPathName() 
                                                     + "/FlipMu/Nuance/"+ "lastAudioConfig.xml" ));
    
	//get the XML element and pass it into our audio
    loadAudioConfigElement = lastAudioDeviceManagerSettings.getDocumentElement();
	const String error (audioDeviceManager.initialise (2, /* number of input channels */
													   2, /* number of output channels */
													   ScopedPointer<XmlElement>(lastAudioDeviceManagerSettings.getDocumentElement()),
													   true  /* select default device on failure */));
	if (error.isNotEmpty()){
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,"Audio Demo", "Couldn't open an output device!\n\n" + error);
	}else{
	}
}

void MainComponent::setCommandManager(ApplicationCommandManager* _commandManager) {
	commandManager = _commandManager;
	commandManager->registerAllCommandsForTarget(transportBar); //register transportBar's commands
	commandManager->registerAllCommandsForTarget(this);
	//commandManager->getKeyMappings()->resetToDefaultMappings(); //set the default key mappings to those of the registered targets
	commandManager->getKeyMappings()->addKeyPress(APP_TP_PLAY, KeyPress(KeyPress::spaceKey), 0);
	commandManager->getKeyMappings()->addKeyPress(APP_TP_REC, KeyPress::createFromDescription("command + r"), 1);
	commandManager->getKeyMappings()->addKeyPress(APP_GUI_EDITPANEL, KeyPress::createFromDescription("command + l"), 2);
	addKeyListener(commandManager->getKeyMappings());
	commandManager->setFirstCommandTarget(transportBar);
}

void MainComponent::startStopTransport() {
    
    //countedIn = false;

    metro->setMetroRunning(transportBar->getPlayButtonState());
	
	if (transportBar->getRecordButtonState()){
		startStopRecording();	
	}	
}

void MainComponent::startStopRecording() {
	
    //prepare all recorders for recording...
	if(transportBar->getRecordButtonState() && transportBar->getPlayButtonState())
	{
		const ScopedLock sl (audioDeviceManager.getAudioCallbackLock() );
        
        for(int i=0; i< audioRecorderManager.size(); i++){
			audioRecorderManager[i]->prepareRecording();
            //Time::waitForMillisecondCounter(20);
		}
		
		for(int i=0; i< sensorRecorderManager.size(); i++){
			sensorRecorderManager[i]->prepareRecording();
		}
		
		for(int i=0; i< oscRecorderManager.size(); i++){
			oscRecorderManager[i]->prepareRecording();
		}
        
        for(int i=0; i< midiRecorderManager.size(); i++){
			midiRecorderManager[i]->prepareRecording();
		}
        
        finishWritingToDisk = false;

	}
	
    //very briefly lock the audioCallback & then flip our shared bool so that all recorders actually write or stop and cleanup...
    //const ScopedLock sl (audioDeviceManager.getAudioCallbackLock() );

   // if (transportBar->getRecordButtonState() && transportBar->getPlayButtonState() ) {
		//const ScopedLock sl (audioDeviceManager.getAudioCallbackLock() );
 //       finishWritingToDisk = false;
//	}
	else if (transportBar->getRecordButtonState() && !transportBar->getPlayButtonState() ){
        //const ScopedLock sl (audioDeviceManager.getAudioCallbackLock() );
        finishWritingToDisk = true;
	}	
    
}


/*==============================================================================
 **
 * Callback methods
 **
 *=============================================================================*/

void MainComponent::buttonClicked (Button* buttonThatWasClicked){
	
	if (buttonThatWasClicked == transportBar->getPlayButton())
	{
		startStopTransport();
	}
	
	else if (buttonThatWasClicked == transportBar->getMetroButton())
	{
		metro->setMetroPlaying(transportBar->getMetroButtonState());
	}
}


void MainComponent::changeListenerCallback(ChangeBroadcaster* objectThatHasChanged){
	
}

bool MainComponent::keyPressed  (const KeyPress  &key, Component  *originatingComponent) {
	DBG(key.getTextDescription());
	return true;
}

bool MainComponent::keyStateChanged (bool isKeyDown, Component *originatingComponent){
	return true;
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget (void) {
	return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (Array< CommandID > &commands) {
	commands.clear();
	
	ShortcutItems itemCur;
	for (itemCur = (ShortcutItems)APP_BASE; itemCur < APP_TOP; itemCur = (ShortcutItems)((int)itemCur+1)) {
		switch (itemCur) {
			case APP_GUI_EDITPANEL:
				commands.add(itemCur);
				break;
				
			default:
				break;
		}
	}
}

void MainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo &result) {
	switch ((ShortcutItems)commandID)
	{
		case APP_GUI_EDITPANEL:
			result.setInfo ("Lock/Unlock", "Lock/Unlock", "GUI", 0);
			break;	
			
		default:
			break;
	}	
}

bool MainComponent::perform (const InvocationInfo &info){ 
	
	switch (info.commandID)
	{
			
		case APP_TP_PLAY:
			startStopTransport();
			break;
			
		case APP_GUI_EDITPANEL:
			panelToggle();
			break;
	}
	
	return true;
}



/*==============================================================================
 **
 * File saving methods
 **
 *=============================================================================*/

void MainComponent::chooseSavePath(){
	FileChooser chooser ("Choose save location", File::nonexistent, "*.xml");	//create a FileChooser window to select a folder to save recordings in
	if (chooser.browseForDirectory()){
		setSavePath(chooser.getResult());				//setSavePath to the folder selected
	}
}

void MainComponent::setSavePath(File _savePath){
	saveFolder = _savePath;
	for(int i=0; i< audioRecorderManager.size(); i++){
		audioRecorderManager[i]->setSavePath(getSavePath());
	}
	
	for(int i=0; i< sensorRecorderManager.size(); i++){
		sensorRecorderManager[i]->setSavePath(getSavePath());
	} 
	
	for(int i=0; i< oscRecorderManager.size(); i++){
		oscRecorderManager[i]->setSavePath(getSavePath());
	}
    
    for(int i=0; i< midiRecorderManager.size(); i++){
		midiRecorderManager[i]->setSavePath(getSavePath());
	} 
}

String MainComponent::getSavePath(){
	return saveFolder.getFullPathName();
}

/*==============================================================================
 **
 * XML-Based Config Load/Save
 **
 *=============================================================================*/

void MainComponent::saveConfiguration( File _savePath){
	
	//create main XML elements
	ScopedPointer<XmlElement> nuanceConfiguration (new XmlElement("NUANCECONFIGURATION"));
	XmlElement* recorderList = new XmlElement("recorderList");
	XmlElement* deviceList = new XmlElement("deviceList");
	XmlElement* general = new XmlElement("general_settings");
	
	nuanceConfiguration->addChildElement (recorderList);
	nuanceConfiguration->addChildElement (deviceList);
	nuanceConfiguration->addChildElement (general);
	
	//add all sensor recorders
	for (int i = 0; i < sensorRecorderManager.size(); ++i){
		XmlElement* sensorRecorder = sensorRecorderManager[i]->serialize();
		recorderList->addChildElement (sensorRecorder);
	}
	
	//add all audio recorders
	for (int i = 0; i< audioRecorderManager.size(); ++i){
        XmlElement* audioRecorder = audioRecorderManager[i]->serialize();
		recorderList->addChildElement (audioRecorder);
	}
	
	for (int i = 0; i< oscRecorderManager.size(); ++i){
        XmlElement* oscRecorder = oscRecorderManager[i]->serialize();
		recorderList->addChildElement (oscRecorder);
	}
    
    for(int i=0; i<midiRecorderManager.size(); ++i){
        XmlElement* midiRecorder = midiRecorderManager[i]->serialize();
        recorderList->addChildElement(midiRecorder);
    }
	
	XmlElement* window = new XmlElement("WindowSettings");
	window->setAttribute((String)"width", getWidth());
	window->setAttribute((String)"height", getHeight());
	general->addChildElement(window);
	
	// now we can turn the whole thing into a text document and save it to disk...
	String myXmlDoc = nuanceConfiguration->createDocument (String::empty);
	nuanceConfiguration->writeToFile( _savePath, String::empty, (String)"UTF-8", 200);
}

void MainComponent::loadConfiguration(File _loadPath){
	
	XmlDocument layoutFile (_loadPath); //load layout from file to XML document
	mainElement = layoutFile.getDocumentElement();	//pass protocol xml into an XmlElement for parsing
	
	if (mainElement == 0)
	{
		String error = layoutFile.getLastParseError();
	}
	else{
		
		//unregister all component callbacks
		for (int i = 0; i< audioRecorderManager.size(); i++){
			audioRecorderManager[i]->removeAudioCallbacks();
		}
		
		for (int i = 0; i< sensorRecorderManager.size(); i++){
			sensorRecorderManager[i]->removeAudioCallbacks();
		}
		
		for (int i = 0; i< oscRecorderManager.size(); i++){
			listener.removeListener(oscRecorderManager[i]);
            oscRecorderManager[i]->removeAudioCallbacks();
		}
        
        for (int i = 0; i< midiRecorderManager.size(); i++){
            midiRecorderManager[i]->removeAudioCallbacks();
		}
				
		//remove all components from their arrays and delete theme (automatically deleted when removed from the array because it is an OwenedArray
		if (audioRecorderManager.size() > 0) audioRecorderManager.clear(true);
		if (sensorRecorderManager.size() > 0) sensorRecorderManager.clear(true);
		if (oscRecorderManager.size() > 0) oscRecorderManager.clear(true);
        if (midiRecorderManager.size() > 0) midiRecorderManager.clear(true);
		
		//add and register all new components
		//for each childElement of mainElement (IE recorderList & deviceList)
		for (int i=0; i< mainElement->getNumChildElements();i++){
			//for each audio/sensor recorder and serial device (a.k.a childElement of recorderList and deviceList)
			for (int j=0; j<mainElement->getChildElement(i)->getNumChildElements(); j++){ 
				
				//if we are on a audio recorder
				if(mainElement->getChildElement(i)->getChildElement(j)->getTagName() == (String)"AudioRecorder"){
					addAudioRecorder(mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"x").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->
									 getStringAttribute((String)"y").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"width").getIntValue(),mainElement->getChildElement(i)->
									 getChildElement(j)->getStringAttribute((String)"height").getIntValue(), mainElement->getChildElement(i)->
									 getChildElement(j)->getStringAttribute((String)"gain").getFloatValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute("channel").getIntValue());
				}
				
				//if we are on a sensor recorder
				else if(mainElement->getChildElement(i)->getChildElement(j)->getTagName() == (String)"SensorRecorder"){
					//create the sensor recorder and pass in its drawing location and dimensions
					addSensorRecorder(mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"x").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->
									  getStringAttribute((String)"y").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"width").getIntValue(),mainElement->getChildElement(i)->
									  getChildElement(j)->getStringAttribute((String)"height").getIntValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"minVal").getFloatValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"maxVal").getFloatValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)(String)"protocol"),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"selectedSensor"),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"selectedDevice"));					
				}
				
				//if we are on an OSC recorder
				else if(mainElement->getChildElement(i)->getChildElement(j)->getTagName() == (String)"OSCRecorder"){
					//create the sensor recorder and pass in its drawing location and dimensions
					addOSCRecorder(mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"x").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->
								   getStringAttribute((String)"y").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"width").getIntValue(),mainElement->getChildElement(i)->
								   getChildElement(j)->getStringAttribute((String)"height").getIntValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"minVal").getFloatValue(),
								   mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"maxVal").getFloatValue(),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"oscAddress"));					
				}
                
                else if(mainElement->getChildElement(i)->getChildElement(j)->getTagName() == (String)"MIDIRecorder"){
					//create the sensor recorder and pass in its drawing location and dimensions
					addMidiRecorder(mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"x").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->
								   getStringAttribute((String)"y").getIntValue(),mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"width").getIntValue(),mainElement->getChildElement(i)->
								   getChildElement(j)->getStringAttribute((String)"height").getIntValue(), mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"selectedMidiDevice"),
								   mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"selectedCCNumber"));					
				}
				
			
				//if we are on a window setting
				else if(mainElement->getChildElement(i)->getChildElement(j)->getTagName() == (String)"WindowSettings"){
					//change the window dimensions
					setSize(mainElement->getChildElement(i)->getChildElement(j)->getStringAttribute((String)"width").getIntValue(), mainElement->getChildElement(i)->getChildElement(j)->
							getStringAttribute((String)"height").getIntValue());
				}
				
				
			}			
		}
	}
}

/*==============================================================================
 **
 * Methods for Component Editing
 **
 *=============================================================================*/

bool MainComponent::panelToggle(){
	
	isEditable = !isEditable;
	
	if(isEditable){
		//this->setWantsKeyboardFocus(true);
		
		this->setAlpha(0.7);
		editText = "Lock Panel";
		editor = new ComponentLayoutEditor();
		editor->setTargetComponent(this);
		editor->setEnabled(true);
		//editor->setWantsKeyboardFocus(true);
		
	}else{
		this->setAlpha(1.0);
		editText = "Edit Panel";
		//editor->setWantsKeyboardFocus(false);
		editor->setEnabled(false);
		//deleteAndZero(editor); //no longer needed as editor is a scopedPointer
		//setWantsKeyboardFocus(true);
	}
	
	return isEditable;
}

void MainComponent::openPreferences(){
	PreferencesPane prefPane(audioDeviceManager, &listener, serialDeviceManager);
	DialogWindow::showModalDialog("Preferences", &prefPane, this, Colours::black, true);	
}

void MainComponent::addAudioRecorder(int _x, int _y, int _w, int _h, float _gain, int _chan) {
	AudioRecorder* audio;															
	addAndMakeVisible (audio = new AudioRecorder(audioDeviceManager));						
	audio->setShouldFinishWriting(&finishWritingToDisk, &countedIn);
	audioRecorderManager.add(audio);
	audio->setGain(_gain);		
    audio->setChannel(_chan);
	audio->setBounds (_x, _y, _w, _h);
}

void MainComponent::addSensorRecorder(int _x, int _y, int _w, int _h, float _minVal, float _maxVal, String protocol, String ss, String sd) {
	SensorRecorder* sensor;																	
	addAndMakeVisible (sensor = new SensorRecorder( audioDeviceManager, serialDeviceManager ));		
    sensor->setShouldFinishWriting(&finishWritingToDisk, &countedIn);									
	sensorRecorderManager.add(sensor);														
	sensor->setMin(_minVal);
	sensor->setMax(_maxVal);
    if (protocol != "null") {
        sensor->setProtocol(protocol);
    }
    sensor->setSelectedSensor(ss);
    sensor->setSelectedDevice(sd);
	sensor->setBounds(_x, _y, _w, _h);														
}

void MainComponent::addOSCRecorder(int _x, int _y, int _w, int _h, float _minVal, float _maxVal, String ad){
	OSCRecorder* osc;																	
	addAndMakeVisible (osc = new OSCRecorder(audioDeviceManager));										
	osc->setShouldFinishWriting(&finishWritingToDisk, &countedIn);
	oscRecorderManager.add(osc);
    listener.addListener(osc);												
	osc->setMin(_minVal);
	osc->setMax(_maxVal);
    osc->setAddress(ad);
	osc->setBounds(_x, _y, _w, _h);														
}

void MainComponent::addMidiRecorder(int _x, int _y, int _w, int _h, String md, String cc){
	MidiRecorder* midi;																	
	addAndMakeVisible (midi = new MidiRecorder(audioDeviceManager));									
	midi->setShouldFinishWriting(&finishWritingToDisk, &countedIn);
	midiRecorderManager.add(midi);														
	midi->setBounds(_x, _y, _w, _h);	
    midi->setSelectedMidiDevice(md);
    midi->setSelectedCCNumber(cc);
}



/*==============================================================================
 **
 * Mouse events
 **
 *=============================================================================*/

void MainComponent::mouseDown(const MouseEvent  &e){
	if (e.mods.isPopupMenu()){
		PopupMenu main;					//main menu
		PopupMenu addComponent;			//submenu for adding components
		PopupMenu addRecorderMenu;		//sub-submenu for adding recorder components
		PopupMenu addDeviceMenu;		//sub-submenu for adding device (serial...etc) components
		PopupMenu settings;				//sebmenu for settings
		
		main.addItem(103, editText);
		main.addSeparator();
		main.addItem(101, "Open Session");
		main.addItem(102, "Save Session As...");
		main.addSeparator();
		
		addRecorderMenu.addItem(201, "Audio Recorder");
		addRecorderMenu.addItem(202, "Sensor Recorder");
		addRecorderMenu.addItem(203, "OSC Recorder");
        addRecorderMenu.addItem(204, "MIDI Recorder");
		main.addSubMenu("Recorders", addRecorderMenu);
		main.addSeparator();
		
		settings.addItem(401, "Preferences");
		settings.addItem(402, "Save Location...");
		main.addSubMenu("Settings", settings);
		
		int result = main.show();
		//int result = main.showAt(e.getScreenX(), e.getScreenY());
		if (result != 0){
			if (result == 103){
				panelToggle(); //switch between editing the placement of objects, and locking the placement of objects
			}
			else if (result == 101){
				FileChooser chooser ("Choose layout config file", File::nonexistent, "*.xml");
				if (chooser.browseForFileToOpen()){
					loadConfiguration(chooser.getResult());	//setSavePath to the folder selected
				}
			}
			else if (result == 102){
				FileChooser chooser ("Choose save location", File::nonexistent, "*.xml"); //create a FileChooser window to select a folder to save layout XML in
				if (chooser.browseForFileToSave(true)){
					saveConfiguration(chooser.getResult());	
				}
			} 
			//if the menu item selected is an Audio Recorder
			else if (result == 201){
				addAudioRecorder(e.getMouseDownX(), e.getMouseDownY(), getWidth()/1.7, getHeight()/8, 1.0, 0 );
			}
			//if the menu item selected is a Sensor Recorder
			else if (result == 202){
				addSensorRecorder(e.getMouseDownX(),e.getMouseDownY(), getWidth()/5.5, getHeight()/5.6, 0.0, 1024.0,"null", "null","null");
			}	
			
			//if the menu item selected is an OSC Recorder
			else if (result == 203){
				addOSCRecorder(e.getMouseDownX(),e.getMouseDownY(), getWidth()/5.5, getHeight()/5.6, 0.0, 1.0, "/nuance");
			}
			
            //if the menu item selected is a Midi Recorder
			else if (result == 204){
				addMidiRecorder(e.getMouseDownX(),e.getMouseDownY(), getWidth()/5.5, getHeight()/5.6, "0", "0");
			}
            
			else if (result == 401){
				openPreferences();
			}
			
			else if (result == 402){
				chooseSavePath();	
			}
		}
	}
	
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
}

void MainComponent::mouseUp (const MouseEvent& e)
{
	
}

/*void MainComponent::findLassoItemsInArea (Array <Component*>& results, int x, int y, int width, int height)
 {
 
 
 
 }
 
 SelectedItemSet <Component*>& MainComponent::getLassoSelection()
 {
 // return ?.getSelectedSet();
 }*/


/*==============================================================================
 **
 * MenuBar methods
 **
 *=============================================================================*/

StringArray MainComponent::getMenuBarNames()
{
	const char *names[] = {"File",
		"Help"};
	const StringArray retval(names, 2);
	
	return retval;
}

//------------------------------------------------------------------------------
PopupMenu MainComponent::getMenuForIndex(
											   int topLevelMenuIndex,
											   const String &menuName)
{
	PopupMenu retval;
	
	switch(topLevelMenuIndex)
	{
		case 0:
			retval.addItem(FileNew, "New");
			retval.addSeparator();
			retval.addItem(FileOpen, "Open...");
			retval.addItem(FileSave, "Save");
			retval.addItem(FileSaveAs, "Save As...");
			retval.addSeparator();
			retval.addItem(FileQuit, "Quit");
			break;
		case 1:
			retval.addItem(HelpAbout, "About");
			break;
	}
	
	return retval;
}

//------------------------------------------------------------------------------
void MainComponent::menuItemSelected(
									 int menuItemID,
									 int topLevelMenuIndex)
{
	switch(menuItemID)
	{
		case FileNew:
			//fileNew();
			break;
		case FileOpen:
			//fileOpen();
			break;
		case FileSave:
			//fileSave();
			break;
		case FileSaveAs:
			//fileSaveAs();
			break;
		case FileQuit:
			JUCEApplication::quit();
			break;
		case HelpAbout:
		{
			//AboutBox dlg;
			//dlg.setSize(300, 225);
			//ScopedPointer<DialogWindow> dlg(new DialogWindow("About", Colours::lightgrey, true, true));
			//dlg->setSize(300,225);
			//dlg->runModalLoop();
			
			 //DialogWindow::showModalDialog("About",
			 //&dlg,0,Colours::lightblue,true, false);
		}
			break;
	}
}


//==============================================================================






//#endif
