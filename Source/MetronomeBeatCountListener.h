/*
 *  MetronomeBeatCountListener.h
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 4/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _METRONOMEBEATCOUNTELISTENER_
#define _METRONOMEBEATCOUNTELISTENER_

class MetronomeBeatCountListener{
	
	
public:
	MetronomeBeatCountListener();
	virtual ~MetronomeBeatCountListener();
	
	virtual void myCallbackMethod( int beatCount);
private:
	
	
};













#endif   // _OSCMESSAGELISTENER_