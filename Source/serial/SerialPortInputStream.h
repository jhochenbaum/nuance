/*
  ==============================================================================

    SerialInputStream.h
    Created: 28 Jul 2011 10:33:20am
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#ifndef __SERIALPORTINPUTSTREAM_H_11C0D022__
#define __SERIALPORTINPUTSTREAM_H_11C0D022__

#include "JuceHeader.h"
#include "SerialPort.h"

class SensorRecorder;
class SerialPort;

class SerialPortInputStream : public InputStream, /*public ChangeBroadcaster,*/ public Thread
{
public:
	SerialPortInputStream(SerialPort * port):Thread("SerialInThread"), port(port), bufferedbytes(0), notify(NOTIFY_OFF),  notifyChar(0)
	{
		startThread();
	}
	~SerialPortInputStream()
	{
        signalThreadShouldExit();
		waitForThreadToExit(500);
	}
	enum notifyflag{NOTIFY_OFF=0, NOTIFY_ON_CHAR, NOTIFY_ALWAYS};
	void setNotify(notifyflag notify=NOTIFY_ON_CHAR, char c=0)
	{
		notifyChar = c;
		this->notify = notify;
	}
	bool canReadString()
	{
		const ScopedLock l(bufferCriticalSection);
		int iNdx=0;
		while(iNdx<bufferedbytes)
			if(((char*)buffer.getData())[iNdx++]==0)return true;  //added.getData()
		return false;
	}
	bool canReadLine()
	{
		const ScopedLock l(bufferCriticalSection);
		int iNdx=0;
		while(iNdx<bufferedbytes)
			if( (((char*)buffer.getData())[iNdx++]=='\n') /*|| (((char*)buffer)[iNdx++]=='\r')*/) //added.getData()
				return true;
		return false;
	}
	
    
	void addListener(SensorRecorder* sr)
	{
        //std::cout << "adding Serial Listener (SensorRecorder)" << std::endl;
        sensorRecorderListeners.add(sr);
	}
	
	void removeListener(SensorRecorder* sr)
	{
		sensorRecorderListeners.remove(sr);
	}
	
	virtual void run();
	virtual int read(void *destBuffer, int maxBytesToRead);
	virtual String readNextLine() //have to override this, because InputStream::readNextLine isn't compatible with SerialPorts (uses setPos) //Jordan H changed from const String return type to String
	{
		String s;
		char c;
		s.preallocateBytes(256); //Jordan H changed to preAllocateBytes assuming each char is 8bits * 32 chars = 256bytes
		//s.preallocateStorage(32); ////Jordan H removed, see line above
		while(read(&c, 1) && (c!='\n'))
			s.append(String::charToString(c), 1);
		s = s.trim();
		return s;
	}
	virtual int64 getTotalLength()
	{
		const ScopedLock l(bufferCriticalSection);
		return bufferedbytes;
	};
	virtual bool isExhausted()
	{
		const ScopedLock l(bufferCriticalSection);
		return bufferedbytes?false:true;
	};
	virtual int64 getPosition(){return -1;};
	virtual bool setPosition(int64 newPosition){return false;};
	juce_UseDebuggingNewOperator
private:
	SerialPort * port;
	int bufferedbytes;
	MemoryBlock buffer;
	CriticalSection bufferCriticalSection;
	notifyflag notify;
	char notifyChar;
	ListenerList<SensorRecorder> sensorRecorderListeners;
};



#endif  // __SERIALPORTINPUTSTREAM_H_11C0D022__
