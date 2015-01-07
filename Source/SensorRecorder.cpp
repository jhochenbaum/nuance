/*
 *  SensorRecorder.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 5/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "SensorRecorder.h"
//#include "SerialPort.h" //maybe dont need this?
#include <iostream>

SensorRecorder::SensorRecorder(AudioDeviceManager& adm, SerialDeviceManager& sdm) : audioDeviceManager(adm), serialDeviceManager(sdm),portPath("null"), sensorStartMessage("null"){
    
    Recorder::setFilename("Sensor_");
    addAndMakeVisible (sensorLabel = new Label("sensorLabel", "Sensor Recorder"));	
	addAndMakeVisible (inputEnable = new ToggleButton("on/off") );
	addAndMakeVisible (serialDeviceSelector = new ComboBox ("serial input"));
	addAndMakeVisible (sensorSelector = new ComboBox ("sensor input"));
	addAndMakeVisible (loadSerialProtocolButton = new ToggleButton ("load"));
	addAndMakeVisible (minLabel = new Label("minLabel", "min"));
	addAndMakeVisible(minVal = new TextEditor ("minValue"));
	addAndMakeVisible (maxLabel = new Label("maxLabel", "max"));
	addAndMakeVisible(maxVal = new TextEditor ("maxValue"));
	
	serialDeviceSelector->setSelectedItemIndex(-1);
	serialDeviceSelector->addListener(this);	
	serialDeviceSelector->setTextWhenNothingSelected("Select Device");
	sensorSelector->setSelectedItemIndex(-1);
	sensorSelector->addListener(this);	
	sensorSelector->setTextWhenNothingSelected("Select Sensor");	
	sensorLabel->setEditable(false, true, false);
	inputEnable->addListener(this);
    loadSerialProtocolButton->addListener (this);
	minVal->setText("0.0", true); //set default min value to 0
	minVal->addListener(this);
	maxVal->setText("1024.0", true); //set default max value to 1024
	maxVal->addListener(this);
	
	textEditFont.setHeight(12.0);
	minVal->setFont(textEditFont);
	maxVal->setFont(textEditFont);
	minLabel->setFont(textEditFont);
	maxLabel->setFont(textEditFont);
    
	sliderWidth = 0;
	dataMin = minVal->getText().getFloatValue();
	dataMax = maxVal->getText().getFloatValue();
	
	this->toFront(false);
	//this->setAlwaysOnTop(true);
    updateDeviceList();
    startTimer (1000 / 50);  // repaint every 1/50 of a second
    
    inputEnable->setToggleState(true, true);
    addAudioCallbacks(); //add all callbacks    
	
}


SensorRecorder::~SensorRecorder(){
    removeAudioCallbacks();
}

void SensorRecorder::stop(){
    Recorder::recOut->stop();
}

void SensorRecorder::paint (Graphics& g){
	
	g.setColour(Colour::fromRGB(61, 61, 61) );
	g.fillRoundedRectangle(0,0,getWidth(), getHeight(), 12);
	g.setColour(Colour::fromRGB(147, 147, 147));
	g.fillRoundedRectangle(2,2,getWidth()-4, getHeight()-4, 10);	
	g.setColour(Colour::fromRGB(209, 209, 209));
	g.fillRect (4.0, sensorSelector->getY()+sensorSelector->getHeight()+3.0, getWidth()-8, getWidth()/11.0); //slider
	g.setColour(Colours::yellow);
	g.fillRect (4.0, sensorSelector->getY()+sensorSelector->getHeight()+3.0, (getWidth()-8)*sliderWidth, getWidth()/11.0); //slider
}

void SensorRecorder::resized(){
	sensorLabel->setBounds(4,4,getWidth(),20); //"Sensor Recorder"
	serialDeviceSelector->setBounds (4, 24, getWidth()-8,getHeight()/6);
	sensorSelector->setBounds (4, serialDeviceSelector->getY()+serialDeviceSelector->getHeight()+2, getWidth()-8,getHeight()/6);
	loadSerialProtocolButton->setBounds(1, getHeight()/1.45, getHeight()/5.5, getHeight()/5.5);
	loadSerialProtocolButton->changeWidthToFitText();
	inputEnable->setBounds(1, loadSerialProtocolButton->getY()+loadSerialProtocolButton->getHeight()/1.5,loadSerialProtocolButton->getWidth(), loadSerialProtocolButton->getHeight());
	inputEnable->changeWidthToFitText();
	minVal->setBounds(getWidth()/1.8, loadSerialProtocolButton->getY()+2, getWidth()/4, getHeight()/7.5);
	maxVal->setBounds(minVal->getX(), minVal->getY()+minVal->getHeight()+1, minVal->getWidth(), minVal->getHeight());
    minLabel->setBounds(minVal->getX()+minVal->getWidth()-4, minVal->getY(), textEditFont.getHeight()*3, textEditFont.getHeight());
    maxLabel->setBounds(maxVal->getX()+maxVal->getWidth()-4, maxVal->getY(), textEditFont.getHeight()*3, textEditFont.getHeight());
}

void SensorRecorder::timerCallback(){
	//repaint();
    repaint(4.0, sensorSelector->getY()+sensorSelector->getHeight()+3.0, getWidth()-8, getWidth()/11.0);
}

const float* SensorRecorder::getData(int numSamples)
{
    //create a float*, pack it with our last sensor data reading, and pass it into monoChannelArray (Recorder::setData) for our writer to access via Recorder::getDataBufferToWrite()...
    tempData = new float[numSamples];
    std::fill_n(tempData, numSamples, scaledData);
    Recorder::setData(tempData);   
    return Recorder::getDataBufferToWrite();
}

void SensorRecorder::changeListenerCallback (ChangeBroadcaster *source)
{
    updateDeviceList();
}


void SensorRecorder::buttonStateChanged (Button* buttonThatWasClicked){
	
}

void SensorRecorder::buttonClicked (Button* buttonThatWasClicked){
	if (buttonThatWasClicked == loadSerialProtocolButton){
		selectProtocolWindow();
		loadSerialProtocolButton->setToggleState(false,false);
	}
    
    else if (buttonThatWasClicked == inputEnable){
        if (inputEnable->getToggleState()){
            Recorder::setShouldRecord( true );
        }else{
            Recorder::setShouldRecord( false );
        }
	}
}

void SensorRecorder::comboBoxChanged(ComboBox* comboBoxThatHasChanged){
	if (comboBoxThatHasChanged == sensorSelector){
		sensorStartMessage = mainElement->getChildElement(0)->getChildElement(sensorSelector->getSelectedId()-1)->getAttributeValue(1); //update Start Message 
        Recorder::setFilename(sensorStartMessage+"_");
    }
    
    else if(comboBoxThatHasChanged == serialDeviceSelector){
        serialDeviceManager.removeSerialDeviceListener(this);
        deviceName = serialDeviceSelector->getItemText(comboBoxThatHasChanged->getSelectedItemIndex());
        portPath = portList.getValue(deviceName, "null");
        std::cout << "SensorRecorder portPath: " << portPath <<std::endl;
        serialDeviceManager.addSerialDeviceListener(this);
        selectedSerialDevice = comboBoxThatHasChanged->getSelectedItemIndex();
    }
}

void SensorRecorder::textEditorReturnKeyPressed (TextEditor &editor){
	if (&editor == minVal){
		dataMin = minVal->getText().getFloatValue();
	}
	else if (&editor == maxVal){
		dataMax = maxVal->getText().getFloatValue();
	}
	getParentComponent()->grabKeyboardFocus();
}

void SensorRecorder::myCallbackMethod(void* objectThatHasChanged){
    
    String incomingData = *(String*)objectThatHasChanged;
    //std::cout << incomingData << std::endl;
    if(sensorStartMessage!="null")
    {
        if (incomingData.startsWith(sensorStartMessage) ){
            std::cout << incomingData << std::endl;
            setSensorData(incomingData);
        }
    }
}

String SensorRecorder::getPortPath(){
    return portPath;
}


void SensorRecorder::updateDeviceList()
{
    
    portList = serialDeviceManager.getAllDevices();
    
    serialDeviceSelector->clear(); //first we clear and reset out serialDeviceSelector comboBox
    serialDeviceSelector->addItem("none", 1);
    
    serialDeviceList = serialDeviceManager.getConnectedDeviceList(); //request an updated list of connected devices and add them
    
    for (int i=0; i<serialDeviceList.size(); i++) 
    {
        serialDeviceSelector->addItem(serialDeviceList.getAllKeys()[i] , i+2); //+2 because we must start at 1 and we aleady added "none" as the first item in the list 
    }
    
    setSelectedDevice(deviceName);
    
}

void SensorRecorder::setSensorData(String data){
	data = data.substring(sensorStartMessage.length());
	scaledData = (data.getFloatValue() - dataMin) / (dataMax - dataMin);
    sliderWidth = scaledData;
    //std::cout << scaledData << std::endl;
}


void SensorRecorder::setProtocol(String _protocolPath){
	protocolPath = _protocolPath;
    
    File protocolFile(protocolPath);
  	
    XmlDocument protocol (protocolFile); //load serial protocol
    mainElement = protocol.getDocumentElement();	//pass protocol xml into an XmlElement for parsing
    
    if (mainElement == 0){
        String error = protocol.getLastParseError();
    }
    else{
        sensorSelector->clear(); //clear current contents in list
        
        for (int i=0; i< mainElement->getChildElement(0)->getNumChildElements();i++){
            sensorSelector->addItem((String) mainElement->getChildElement(0)->getChildElement(i)->getAttributeValue(0), i+1);
        }
    }
}

String SensorRecorder::getProtocol(){
	return protocolPath;	
}

void SensorRecorder::selectProtocolWindow(){
	FileChooser chooser ("Select Serial Protocol XML File", File::nonexistent, "*.xml");	//create a FileChooser window to select a serial protocol XML file
	if (chooser.browseForFileToOpen()){
        setProtocol(chooser.getResult().getFullPathName());
	}	
}

void SensorRecorder::setSelectedDevice(String devName){
    //go through each of the added devices, and if they were there previously, reselect the device
    if (devName != "null")
    {
        for (int i=0; i<serialDeviceSelector->getNumItems(); i++) {
            if (devName == serialDeviceSelector->getItemText(i) ) {
                serialDeviceSelector->setSelectedItemIndex(i);
                break;
            }
        }
    }
}

void SensorRecorder::setSelectedSensor(String selSen){
    if(selSen != "null")
    {
        for (int i=0; i<sensorSelector->getNumItems(); i++) {
            if (selSen == sensorSelector->getItemText(i) ) {
                sensorSelector->setSelectedItemIndex(i);
                break;
            }
        }
    }
}

float SensorRecorder::getMin(){
	return minVal->getText().getFloatValue();
}

float SensorRecorder::getMax(){
	return maxVal->getText().getFloatValue();
}

void SensorRecorder::setMin(float _minVal){
	minVal->setText((String) _minVal,true);	
}

void SensorRecorder::setMax(float _maxVal){
	maxVal->setText((String) _maxVal,true);
}

XmlElement* SensorRecorder::serialize ()
{
    // create an inner element..
    XmlElement* e = new XmlElement ("SensorRecorder");
    
    e->setAttribute ((String)"x",(String) getX());
    e->setAttribute ((String)"y", (String)getY());
    e->setAttribute ((String)"width", (String)getWidth());
    e->setAttribute ((String)"height",(String) getHeight());
    e->setAttribute ((String)"protocol", (String) getProtocol());
    e->setAttribute ((String)"minVal", (String) getMin());
    e->setAttribute ((String)"maxVal", (String) getMax());
    e->setAttribute ((String)"selectedSensor", sensorSelector->getItemText(sensorSelector->getSelectedItemIndex()));
    e->setAttribute ((String)"selectedDevice", deviceName);
    
    return e;
}

void SensorRecorder::restore (XmlElement* root)
{
}


void SensorRecorder::addAudioCallbacks (){
    serialDeviceManager.addSerialDeviceListener(this);
    audioDeviceManager.addAudioCallback(Recorder::getRecOut());
    serialDeviceManager.addChangeListener(this);
}

void SensorRecorder::removeAudioCallbacks(){
    audioDeviceManager.removeAudioCallback(Recorder::recOut);
    serialDeviceManager.removeSerialDeviceListener(this);
    serialDeviceManager.removeChangeListener(this);
    
}

