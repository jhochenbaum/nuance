/*
  ==============================================================================

    SerialPortOutputStream.h
    Created: 28 Jul 2011 10:33:30am
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#ifndef __SERIALPORTOUTPUTSTREAM_H_5AB4709D__
#define __SERIALPORTOUTPUTSTREAM_H_5AB4709D__

#include "SerialPort.h"

class SerialPortOutputStream : public OutputStream, private Thread
{
public:
	SerialPortOutputStream(SerialPort * port)
    :Thread("SerialOutThread"), port(port),bufferedbytes(0)
	{
		startThread();
	}
	~SerialPortOutputStream()
	{
		signalThreadShouldExit();
		waitForThreadToExit(500);
	}
	virtual void run();
	virtual void flush(){}
	virtual bool setPosition(int64 newPosition){return false;}
	virtual int64 getPosition(){return -1;}
	virtual bool write(const void *dataToWrite, int howManyBytes);
	juce_UseDebuggingNewOperator
private:
	SerialPort * port;
	int bufferedbytes;
	MemoryBlock buffer;
	CriticalSection bufferCriticalSection;
	WaitableEvent triggerWrite;
	static const UInt32 writeBufferSize=128;
};



#endif  // __SERIALPORTOUTPUTSTREAM_H_5AB4709D__
