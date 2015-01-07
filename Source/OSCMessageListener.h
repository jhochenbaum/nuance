/*
 *  OSCMessageListener.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 9/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OSCMESSAGELISTENER_
#define _OSCMESSAGELISTENER_

#include "OscReceivedElements.h"

class OSCMessageListener{
	
	
public:
	OSCMessageListener();
	virtual ~OSCMessageListener();
	
	virtual void myCallbackMethod( osc::ReceivedMessage objectThatHasChanged);
private:
	
	
};













#endif   // _OSCMESSAGELISTENER_