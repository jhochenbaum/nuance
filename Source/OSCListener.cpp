/*
 *  OSCListener.cpp
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 7/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "OSCListener.h"
#include "OSCRecorder.h"

OSCListener::OSCListener() : Thread("Osc Listener"), isActive(false)
{
	//isActive = false;
}

OSCListener::~OSCListener(){
	stopListening();	
}

void OSCListener::startListening(String _address, int _port){
	socket = new UdpListeningReceiveSocket(IpEndpointName( _address.getCharPointer(), _port), this); //changed added toCstring() to .getCharPointer()
	std::cout << "press ctrl-c to end\n";
	isActive = true;
	
	if (!isThreadRunning()){
		startThread();
	}	
}

void OSCListener::stopListening(){
	if (isThreadRunning()){
		std::cout << "closing socket & stopping thread..." << std::endl;
		socket->AsynchronousBreak();
		//socket->Break();
		isActive = false;
		stopThread(2000);
		deleteAndZero(socket);
	}
}

void OSCListener::ProcessMessage( const osc::ReceivedMessage& oscMessage, 
								 const IpEndpointName& remoteEndpoint ){

	//if (!threadShouldExit()){
	messageBundle = ""; ///changed NULL to ""
	myListeners.call(&OSCMessageListener::myCallbackMethod, oscMessage); //broadcast messages to osc recorders

	messageBundle.append((String)oscMessage.AddressPattern(), 20);
	messageBundle.append((String)" ", 1);
	
	try {
		osc::ReceivedMessageArgumentStream args = oscMessage.ArgumentStream(); //need this?
		osc::ReceivedMessage::const_iterator arg = oscMessage.ArgumentsBegin();
		for (int i = 0; i<oscMessage.ArgumentCount(); i++){
			if( arg->IsBool() ){
				bool a = (arg++)->AsBoolUnchecked();
				messageBundle.append((String)" ", 10);
				messageBundle.append((String)(i+1), 1);
				messageBundle.append((String)":", 1);
				messageBundle.append((String)a, 10);
			}else if( arg->IsInt32() ){
				int a = (arg++)->AsInt32Unchecked();
				messageBundle.append((String)" ", 10);
				messageBundle.append((String)(i+1), 1);
				messageBundle.append((String)":", 1);
				messageBundle.append((String)a, 10);
			}else if( arg->IsFloat() ){
				float a = (arg++)->AsFloatUnchecked();
				messageBundle.append((String)" ", 10);
				messageBundle.append((String)(i+1), 1);
				messageBundle.append((String)":", 1);
				messageBundle.append((String)a, 10);
			}else if( arg->IsString() ){
				const char *a = (arg++)->AsStringUnchecked();
				messageBundle.append((String)" ", 10);
				messageBundle.append((String)(i+1), 1);
				messageBundle.append((String)":", 1);
				messageBundle.append((String)a, 10);
			}else{
				std::cout << "received" << oscMessage.AddressPattern() <<" message with unexpected argument type\n";
			}
		}
		
		if( arg != oscMessage.ArgumentsEnd() ) throw osc::ExcessArgumentException();	
		else {
			messageCopy = messageBundle;
			sendChangeMessage();
			//sendSynchronousChangeMessage(); //broadcast message to osc monitor in OSC preferences
		}

		
	}catch( osc::Exception& e ){
		// any parsing errors such as unexpected argument types, or 
		// missing arguments get thrown as exceptions.
		std::cout << "error while parsing message: "
		<< oscMessage.AddressPattern() << ": " << e.what() << "\n";
	}
	//}	
}

String OSCListener::getOSCAsString(){
		return messageCopy;
}


void OSCListener::run(){
	if(isActive){
		socket->Run();
	}
	
	//...keep out thread running
	while (!threadShouldExit()){
		sleep(5000);
	}
}

bool OSCListener::isListening(){
	return isActive;	
}

void OSCListener::addListener(OSCMessageListener* osc){
	myListeners.add(osc);	
}

void OSCListener::removeListener(OSCMessageListener* osc){
	myListeners.remove(osc);
}

