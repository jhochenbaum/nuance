/*
  ==============================================================================

    SerialDeviceManager.h
    Created: 27 Jul 2011 5:12:55pm
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

#ifndef __SERIALDEVICEMANAGER_H_369224D0__
#define __SERIALDEVICEMANAGER_H_369224D0__

//#include "mac_SerialPort.h"
#include "SerialPort.h"
#include "SensorRecorder.h"

class SensorRecorder;
class SerialPort;

class SerialDeviceManager : public ChangeBroadcaster

{
	
public:
	SerialDeviceManager();
	~SerialDeviceManager();
    
    //returns a key-value pair of all serial devices detected by the OS
    StringPairArray getAllDevices();  
    
    //returns a key-value pair of the names & addresses of all connected serial devices
    StringPairArray getConnectedDeviceList(){ return connectedDeviceList; };     
                                                                        
    void openSerialPort(String serialDeviceName);
    void closeSerialPort(String serialDeviceName);
    void addSerialDeviceListener(SensorRecorder* sr);
    void removeSerialDeviceListener(SensorRecorder* sr);
    void setSerialPortEnabled(String serialDeviceName);
    bool isSerialPortOpen(String serialDeviceName);
		
private:
	OwnedArray<SerialPort> connectedSerialDevices; //stores pointers to all connected Serial Devices (opened SerialPorts)
    StringPairArray connectedDeviceList; //stores a key-value pair of the names and addresses of all connected serial devices
};


#endif  // __SERIALDEVICEMANAGER_H_369224D0__
