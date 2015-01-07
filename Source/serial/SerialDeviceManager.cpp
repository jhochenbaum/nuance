/*
  ==============================================================================

    SerialDeviceManager.cpp
    Created: 27 Jul 2011 5:12:55pm
    Author:  Jordan Hochenbaum

  ==============================================================================
*/

//TODO: Might need to remove listeners--See remove listeners in closeSerialPort() below...

#include "SerialDeviceManager.h"
SerialDeviceManager::SerialDeviceManager()
{
    
}

SerialDeviceManager::~SerialDeviceManager()
{
    if (connectedSerialDevices.size() > 0)
        connectedSerialDevices.clear(true);
}

bool SerialDeviceManager::isSerialPortOpen(String serialDeviceName)
{
    return connectedDeviceList.getAllKeys().contains(serialDeviceName);
}

void SerialDeviceManager::setSerialPortEnabled(String serialDeviceName)
{
    if (connectedDeviceList.getAllKeys().contains(serialDeviceName) ) {
        closeSerialPort(serialDeviceName);
    }
    
    else
    {
        openSerialPort(serialDeviceName);
    }
}

void SerialDeviceManager::openSerialPort(String serialDeviceName)
{
    //attempt to open up a connection with the serial port
    String portPath = getAllDevices().getValue(serialDeviceName, "nul");
    if (portPath != "null") 
    {
        SerialPort* newSerialDevice = new SerialPort(portPath, SerialPortConfig(57600, 8, SerialPortConfig::SERIALPORT_PARITY_NONE, SerialPortConfig::STOPBITS_1, SerialPortConfig::FLOWCONTROL_NONE));
        
        //if successful store the new device in our HashMap
        if(newSerialDevice->exists())
        {
            std::cout << "connected to: " << serialDeviceName << std::endl;
            connectedSerialDevices.add(newSerialDevice);
            connectedDeviceList.set(serialDeviceName, getAllDevices().getValue(serialDeviceName, "null"));
        }
    }
}

void SerialDeviceManager::closeSerialPort(String serialDeviceName)
{
    String portPath = connectedDeviceList.getValue(serialDeviceName, "null");
    
    if (portPath != "null")
    {
        
        for(int i=0; i<connectedSerialDevices.size(); i++)
        {
            if (connectedSerialDevices[i]->getPortPath() == portPath ) {
                connectedSerialDevices[i]->close();
                if(!connectedSerialDevices[i]->exists())
                {
                    connectedSerialDevices.remove(i);
                    connectedDeviceList.remove(StringRef(serialDeviceName));
                    std::cout << "closed: " << serialDeviceName << std::endl;
                }
            }
        }
    }else{
        std::cout<< "trying to disconnect invalid device" <<std::endl;
    }
}

void SerialDeviceManager::addSerialDeviceListener(SensorRecorder* sr)
{
    for(int i=0; i<connectedSerialDevices.size(); i++)
    {
        if ( connectedSerialDevices[i]->getPortPath() == sr->getPortPath()) 
        {
            connectedSerialDevices[i]->addInputStreamListener(sr);
        }
    }
}
void SerialDeviceManager::removeSerialDeviceListener(SensorRecorder* sr)
{
    for(int i=0; i<connectedSerialDevices.size(); i++)
    {
        if ( connectedSerialDevices[i]->getPortPath() == sr->getPortPath()) 
        {
            connectedSerialDevices[i]->removeInputStreamListener(sr);
        }
    }
}

StringPairArray SerialDeviceManager::getAllDevices(){
    return SerialPort::getSerialPortPaths();
}