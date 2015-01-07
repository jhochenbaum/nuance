/*
  ==============================================================================

    MidiRecorder.cpp
    Created: 23 Jul 2011 1:24:07pm
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#include "MidiRecorder.h"

MidiRecorder::MidiRecorder(AudioDeviceManager& adm) : audioDeviceManager(adm), scaledData(0), selectedMidiDevice(""), sliderWidth(0)
{    
    Recorder::setFilename("MIDI_");
    addAndMakeVisible (midiLabel = new Label("midiLabel", "MIDI Recorder"));
    addAndMakeVisible(midiDeviceSelector = new ComboBox ("midiDeviceSelector"));
    addAndMakeVisible(ccNumberSelector = new ComboBox ("ccNumberSelector"));
    addAndMakeVisible (inputEnable = new ToggleButton ("on/off"));
    midiLabel->setEditable(false, true, false);
    midiDeviceSelector->setSelectedItemIndex(0);
    midiDeviceSelector->addListener(this);
    midiDeviceSelector->setTextWhenNothingSelected("Select Midi Device");
    ccNumberSelector->setSelectedItemIndex(0);
    ccNumberSelector->addListener(this);
    ccNumberSelector->setTextWhenNothingSelected("Select CC Number");
    inputEnable->addListener(this);
    updateMidiDeviceList();
    buildCCList();
    addAudioCallbacks();
    inputEnable->setToggleState(true, true);
    
    startTimer (1000 / 50);  // repaint every 1/50 of a second
    
    addAudioCallbacks();
}

MidiRecorder::~MidiRecorder()
{
    removeAudioCallbacks();
}

void MidiRecorder::stop()
{
    Recorder::recOut->stop();
}

void MidiRecorder::paint (Graphics& g)
{
	g.setColour(Colour::fromRGB(61, 61, 61) );
	g.fillRoundedRectangle(0,0,getWidth(), getHeight(), 12);
	g.setColour(Colour::fromRGB(147, 147, 147));
	g.fillRoundedRectangle(2,2,getWidth()-4, getHeight()-4, 10);	
	g.setColour(Colour::fromRGB(209, 209, 209));
	g.fillRect (4.0, ccNumberSelector->getY()+ccNumberSelector->getHeight()+3.0, getWidth()-8, getWidth()/11.0); //slider
	g.setColour(Colours::yellow);
	g.fillRect (4.0, ccNumberSelector->getY()+ccNumberSelector->getHeight()+3.0, (getWidth()-8)*sliderWidth, getWidth()/11.0); //slider
}

void MidiRecorder::resized()
{
	midiLabel->setBounds(4,4,getWidth(),20);
    midiDeviceSelector->setBounds(4, 24, getWidth() - 8, getHeight() / 6);
    ccNumberSelector->setBounds(4, midiDeviceSelector->getY() + midiDeviceSelector->getHeight() + 2, getWidth() - 8, getHeight() / 6);
    inputEnable->setBounds(4, ccNumberSelector->getY()+ccNumberSelector->getHeight()+getHeight()/7, getHeight()/5.5, getHeight()/5.5);
	inputEnable->changeWidthToFitText();
}

void MidiRecorder::timerCallback()
{
    //repaint();
    repaint(4.0, ccNumberSelector->getY()+ccNumberSelector->getHeight()+3.0, (getWidth()-8), getWidth()/11.0);
}

void MidiRecorder::changeListenerCallback (ChangeBroadcaster *source)
{
    updateMidiDeviceList();
}

const float* MidiRecorder::getData(int numSamples)
{
    //create a float*, pack it with our last oscData reading, and pass it into monoChannelArray (Recorder::setData) for our writer to access via Recorder::getDataBufferToWrite()...
    tempData = new float[numSamples];
    std::fill_n(tempData, numSamples, scaledData);
    Recorder::setData(tempData);   
    return Recorder::getDataBufferToWrite();
}

void MidiRecorder::handleIncomingMidiMessage (MidiInput *source, const MidiMessage &message)
{
   //if the middi message we receive is a CC message
    if(message.isController())
    {
        //if it is the CC number we have selected to record, then we set its value, normalized between 0-1
        if (message.getControllerNumber() == selectedCCNumber.getIntValue() ) {
            scaledData = message.getControllerValue() / 127.0;
            sliderWidth = scaledData;
        }    
    }
}

void MidiRecorder::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == midiDeviceSelector) 
    {
        String lastSelectedMidiDevice = selectedMidiDevice;
            
        //remove current listening device callback, update selectedMidiDevice to newly selected device, and then add newly selected device callback
        audioDeviceManager.removeMidiInputCallback(lastSelectedMidiDevice, this);
        selectedMidiDevice = midiDeviceSelector->getItemText(midiDeviceSelector->getSelectedItemIndex());
        audioDeviceManager.addMidiInputCallback(selectedMidiDevice , this);
    }
    
    else if (comboBoxThatHasChanged == ccNumberSelector)
    {
        selectedCCNumber = ccNumberSelector->getItemText(ccNumberSelector->getSelectedItemIndex());
        Recorder::setFilename("MIDI" + selectedCCNumber+"_");
        dataReset();
    }
}

void MidiRecorder::buttonStateChanged (Button* buttonThatWasClicked){    
    if (buttonThatWasClicked == inputEnable){
        if (inputEnable->getToggleState()){
            Recorder::setShouldRecord( true );
        }else{
            Recorder::setShouldRecord( false );
        }
	}
}

void MidiRecorder::updateMidiDeviceList()
{
    midiDeviceSelector->clear();
    
    midiDeviceList = MidiInput::getDevices();
    
    for (int i=0; i<midiDeviceList.size(); i++) 
    {
        if(audioDeviceManager.isMidiInputEnabled(midiDeviceList[i]))
            midiDeviceSelector->addItem(midiDeviceList[i], i+1);            
    }
    
    for (int i=0; i<midiDeviceSelector->getNumItems(); i++) {
        if (selectedMidiDevice == midiDeviceSelector->getItemText(i) ) {
            midiDeviceSelector->setSelectedItemIndex(i);
            break;
        }
    }
}

void MidiRecorder::buildCCList()
{
    for (int i=0; i< 128; i++)
    {
        ccNumberSelector->addItem(String(i), i+1);
    }
}

void MidiRecorder::dataReset()
{
    scaledData = 0;
    sliderWidth = 0;
}

XmlElement* MidiRecorder::serialize ()
{
    // create an inner element..
    XmlElement* e = new XmlElement ("MIDIRecorder");
    
    e->setAttribute ((String)"x", (String)getX());
    e->setAttribute ((String)"y", (String)getY());
    e->setAttribute ((String)"width", getWidth());
    e->setAttribute ((String)"height", getHeight());
    e->setAttribute ((String)"selectedMidiDevice", selectedMidiDevice);
    e->setAttribute ((String)"selectedCCNumber",  selectedCCNumber);
    
    return e;
}

void MidiRecorder::restore (XmlElement* root)
{
}

void MidiRecorder::setSelectedMidiDevice(String smd){
    selectedMidiDevice = smd;
    updateMidiDeviceList();
}

void MidiRecorder::setSelectedCCNumber(String sccn){
    selectedCCNumber = sccn;
    for (int i=0; i<ccNumberSelector->getNumItems(); i++) {
        if (selectedCCNumber == ccNumberSelector->getItemText(i) ) {
            ccNumberSelector->setSelectedItemIndex(i);
            break;
        }
    }
}

void MidiRecorder::addAudioCallbacks()
{
    audioDeviceManager.addAudioCallback(Recorder::getRecOut());
    audioDeviceManager.addChangeListener(this);
    
}
void MidiRecorder::removeAudioCallbacks()
{
    audioDeviceManager.removeAudioCallback(Recorder::getRecOut());
    audioDeviceManager.removeMidiInputCallback(selectedMidiDevice, this);
    audioDeviceManager.removeChangeListener(this);
    
}


