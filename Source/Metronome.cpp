/*
 *  Metronome.cpp
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 2/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Metronome.h"
#include "GuiCommands.h"

Metronome::Metronome(bool* countedIn) : m_clockPhase(0),  
                                        m_currentBeat(0), 
                                        m_beatsPerBar(4),
                                        bpm(120),
                                        shouldRun(false),
                                        shouldPlay(false)
{
	countIn = countedIn;
    
	audioFormatManager.registerFormat(wavAudioFormat = new WavAudioFormat(), true);
	
    //First we must load out metronome .wav files into a reader
	metroHighReader = audioFormatManager.createReaderFor(new MemoryInputStream (BinaryData::MetronomeHigh_wav,
                                                                                BinaryData::MetronomeHigh_wavSize,
                                                                                false));
	metroLowReader = audioFormatManager.createReaderFor(new MemoryInputStream (BinaryData::MetronomeLow_wav,
                                                                               BinaryData::MetronomeLow_wavSize,
                                                                               false));
	
	 //Define the midi note ranges each sound will be assigned to...
     //
	BigInteger highNotes;
	highNotes.setRange (0, 1, true);
	BigInteger lowNotes;
	lowNotes.setRange (2, 3, true);
	
	//then we must create SamplerSounds from the readers, and add them to our synth
	samplerSynth.addVoice(new SamplerVoice()); //this voice will be used to play the sound-- only one sound will play at a time so we only add one voice (monophonic)
	samplerSynth.addSound(new SamplerSound("metroHigh", *metroHighReader,highNotes,0,0,0,1.0));
	samplerSynth.addSound(new SamplerSound("metroLow",*metroLowReader,lowNotes,2,0,0,1.0));
	samplerSynth.setNoteStealingEnabled(true); //must turn note stealing off
}

Metronome::~Metronome() {	
	samplerSynth.clearSounds();
	samplerSynth.clearVoices();
}

void Metronome::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate){
	midiCollector.reset (sampleRate);
	samplerSynth.setCurrentPlaybackSampleRate (sampleRate);
	m_sampleRate = sampleRate;
}

void Metronome::releaseResources() {
}

void Metronome::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill){
	
	int const numSamples = bufferToFill.numSamples;
	
	// the synth always adds its output so we must first clear (otherwise anything coming in will also output)
	bufferToFill.clearActiveBufferRegion();
	
	MidiBuffer midi;
	
	if (shouldRun)
	{
        double const samplesPerBeat = m_sampleRate * 60 / bpm;
		
		// Adjust phase so the beat is on or after the beginning of the output
		double beat = 0;
		if (m_clockPhase > 0)
			beat = 1 - m_clockPhase;
		else
			beat = 0 - m_clockPhase;
		
		// Set notes in midi buffer
		for (;;beat += 1)
		{
			// Calc beat pos
			int pos = static_cast <int> (beat * samplesPerBeat);
			if (pos < numSamples){
				if(shouldPlay){
					if (m_currentBeat == 0){
						samplerSynth.noteOn(1, 60, 1.0);
						midi.addEvent(MidiMessage::noteOn (1, 0, 1.f), pos);
					}else{
						midi.addEvent(MidiMessage::noteOn (1, 2, 1.f), pos);
					}
				}
				
                sendChangeMessage();
                
                //if we aren't yet counted in, check to see if enough beats have past to set counted in
                if(*countIn == false){
                    if (m_currentBeat == 3) {
                        std::cout << "setting count in" << std::endl;
                        *countIn = true;
                    }
                }
                
				m_currentBeat++;
				m_currentBeat =  m_currentBeat % m_beatsPerBar;
			}
			
			else
			{
				break;
			}
		}
        advanceClock(numSamples);
	}
	
	samplerSynth.renderNextBlock (*bufferToFill.buffer, midi, 0, bufferToFill.numSamples);	
}

void Metronome::setupBar (int startingBeat, int beatsPerBar) {
    *countIn = false;
    m_clockPhase = 0;
	m_currentBeat = startingBeat;
	m_beatsPerBar = beatsPerBar;
}

void Metronome::advanceClock (int numSamples) {
    double const samplesPerBeat = 44100 * 60 / bpm;
	
    jassert (m_clockPhase >= -.5 && m_clockPhase < .5);
	
    if (m_clockPhase < 0)
		m_clockPhase = m_clockPhase + 1;
	
    m_clockPhase = m_clockPhase + numSamples / samplesPerBeat;
	
    if (m_clockPhase >= .5)
		m_clockPhase -= 1;
	
    jassert (m_clockPhase >= -.5 && m_clockPhase < .5);
}



void Metronome::setMetroRunning(bool runMetro) {
    shouldRun = runMetro; 	
    if (shouldRun)
    {
        setupBar (0, 4);
    }
}

void Metronome::setMetroPlaying(bool playMetro) {
	shouldPlay = playMetro; 	
}

bool Metronome::isCountedIn(){
    return countIn;
}

void Metronome::setBPM(float beatsPerMinute){
	bpm = beatsPerMinute;
}

String Metronome::getBeatCount() {
	return String(m_currentBeat);
}

void Metronome::addBeatListener(MetronomeBeatCountListener* mbl) {
	myBeatListeners.add(mbl);	
}

void Metronome::removeBeatListener(MetronomeBeatCountListener* mbl) {
	myBeatListeners.remove(mbl);
}

void Metronome::labelTextChanged (Label* labelThatHasChanged) {
	setBPM(labelThatHasChanged->getText().getFloatValue() );
}
