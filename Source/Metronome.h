/*
 *  Metronome.h
 *
 *	Special thanks to Vincent Falco, a.k.a "The Vinn" on the JUCE Forums for sharing his Metronome clocking code
 *	Which has been adapted and used here.
 * 
 *  Created by Jordan Hochenbaum on 2/4/11.
 *  Copyright 2011 FlipMu. All rights reserved.
 *
 */

#ifndef _METRONOME_
#define _METRONOME_

#include "JuceHeader.h"
#include "MetronomeBeatCountListener.h"

class Metronome	:	public AudioSource,
					public LabelListener,
                    public ChangeBroadcaster
			
{
public:
	Metronome(bool* countedIn);
	~Metronome();
	
	void prepareToPlay						(int /*samplesPerBlockExpected*/, double sampleRate);
    void releaseResources					();
	void getNextAudioBlock					(const AudioSourceChannelInfo& bufferToFill);
	
	void setBPM								(float beatsPerMinute);
	void setMetroRunning					(bool runMetro);
	void setMetroPlaying					(bool playMetro);
	String getBeatCount                     ();
	void addBeatListener                    (MetronomeBeatCountListener* mbl);
	void removeBeatListener                 (MetronomeBeatCountListener* mbl);
    bool isCountedIn                        ();

private:
	void setupBar							(int startingBeat, int beatsPerBar);
	void advanceClock						(int numSamples);
	void labelTextChanged					(Label* labelThatHasChanged);	//used to be notified of things like BPM changes...
	float m_sampleRate, m_clockPhase;
	int m_currentBeat, m_beatsPerBar;
    float bpm;
    Value beatCount;
	bool shouldRun, shouldPlay;
    bool* countIn;
	
	WavAudioFormat*	wavAudioFormat;											//we will add a waveAudioFormat to our audioFormatManager
	AudioFormatManager audioFormatManager;									//manages a list of formats we want to read (currently only .wav files)
	
	ScopedPointer<AudioFormatReader> metroHighReader;						//reader to load metronome .wav #1
	ScopedPointer<AudioFormatReader> metroLowReader;						//reader to load metronome .wav #2
	
	Synthesiser samplerSynth;												//the synth to play the sound
    MidiMessageCollector midiCollector;										//gets out midi message blocks
	
	ListenerList<MetronomeBeatCountListener> myBeatListeners;
};

#endif //_METRONOME_