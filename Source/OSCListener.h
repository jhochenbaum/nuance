/*
 *  OSCListener.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OSCLISTENER_
#define _OSCLISTENER_

#include "JuceHeader.h"
#include <iostream>

#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"

#include "OscPrintReceivedElements.h"
#include "OSCMessageListener.h"

//class OSCRecorder; //forward declaration of OSCRecorder (hack for ListenerList?)

class OSCListener : public osc::OscPacketListener,
					public Thread,
					public ChangeBroadcaster //only used to broadcast messags to osc monitor in preferences
{
public:
	OSCListener();
	~OSCListener();
						
	void run(); //thread run method
	void startListening(String address, int port);
	void stopListening();
	bool isListening();
	void addListener(OSCMessageListener* osc);
	void removeListener(OSCMessageListener* osc);
	String getOSCAsString();
	const osc::ReceivedMessage* oscMessage;
private:
	UdpListeningReceiveSocket* socket;
	String messageBundle; //string to hold current incoming osc message as string
	String messageCopy;	//holds a copy of the last messageBundle. Overcomes problem of OSC data coming in so fast that listeners (OSCMonitor) can't read fast enough
	bool isActive;
	//CriticalSection myCriticalSection;
	ListenerList<OSCMessageListener> myListeners;

protected:
	void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );
	//void ProcessPacket( const char *data, int size, const IpEndpointName& remoteEndpoint );

};

#endif //_OSCLISTENER_