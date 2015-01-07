//SerialPort.h
/*Serial port class for accessing serial ports in an asyncronous buffered manner
 
 contributed by graffiti
 
 a typical serialport scenario may be:
 {
 //get a list of serial ports installed on the system, as a StringPairArray containing a friendly name and the port path
 StringPairArray portlist = SerialPort::getSerialPortPaths();
 if(portlist.size())
 {
 //open the first port on the system
 SerialPort * pSP = new SerialPort(portlist.getAllValues()[0], SerialPortConfig(9600, 8, SerialPortConfig::SERIALPORT_PARITY_NONE, SerialPortConfig::STOPBITS_1, SerialPortConfig::FLOWCONTROL_NONE));
 if(pSP->exists())
 {
 //create streams for reading/writing
 SerialPortOutputStream * pOutputStream = new SerialPortOutputStream(pSP);
 SerialPortInputStream * pInputStream = new SerialPortInputStream(pSP);
 
 pOutputStream->write("hello world via serial", 22); //write some bytes
 
 //read chars one at a time:
 char c;
 while(!pInputStream->isExhausted())
 pInputStream->read(&c, 1);
 
 //or, read line by line:
 String s;
 while(pInputStream->canReadLine())
 s = pInputStream->readNextLine();
 
 //or ask to be notified when a new line is available:
 pInputStreams->addChangeListener(this); //we must be a changelistener to receive notifications
 pInputStream->setNotify(SerialPortInputStream::NOTIFY_ON_CHAR, '\n');
 
 //or ask to be notified whenever any character is received
 //NOTE - use with care at high baud rates!!!!
 pInputStream->setNotify(SerialPortInputStream::NOTIFY_ALWAYS);
 
 //please see class definitions for other features/functions etc		
 }
 }
 }
 */

//  TODO: Make sure pInputStream.setNotify(SerialPortInputStream::NOTIFY_ALWAYS)  is needed

#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

//#include "JuceHeader.h"
#include "SensorRecorder.h"
#include "SerialPortInputStream.h"
#include "SerialPortOutputStream.h"

class SensorRecorder;

class SerialPortConfig
	{
	public:
		SerialPortConfig(){};
		~SerialPortConfig(){};
		enum SerialPortStopBits{STOPBITS_1, STOPBITS_1ANDHALF, STOPBITS_2};
		enum SerialPortFlowControl{FLOWCONTROL_NONE, FLOWCONTROL_HARDWARE, FLOWCONTROL_XONXOFF};
		enum SerialPortParity{SERIALPORT_PARITY_NONE, SERIALPORT_PARITY_ODD, SERIALPORT_PARITY_EVEN, SERIALPORT_PARITY_SPACE, SERIALPORT_PARITY_MARK};
		SerialPortConfig(UInt32 bps, UInt32 databits, SerialPortParity parity, SerialPortStopBits stopbits, SerialPortFlowControl flowcontrol)
		:bps(bps), databits(databits), parity(parity), stopbits(stopbits), flowcontrol(flowcontrol)
		{}
		UInt32 bps;
		UInt32 databits;
		SerialPortParity parity;
		SerialPortStopBits stopbits;
		SerialPortFlowControl flowcontrol;
	};


class SerialPort
{
public:
    SerialPort()
    {
        portHandle = 0;
        portDescriptor = -1;
        pInputStream = new SerialPortInputStream(this);
        pInputStream->setNotify(SerialPortInputStream::NOTIFY_ALWAYS);
        //pInputStream->addListener(this);
    }
    SerialPort(const String & portPath)
    {
        SerialPort();
        open(portPath);
        pInputStream = new SerialPortInputStream(this);
        pInputStream->setNotify(SerialPortInputStream::NOTIFY_ALWAYS);
        //pInputStream->addListener(this);
    }
    SerialPort(const String & portPath, const SerialPortConfig & config)
    {
        SerialPort();
        open(portPath);
        setConfig(config);
        pInputStream = new SerialPortInputStream(this);
        pInputStream->setNotify(SerialPortInputStream::NOTIFY_ALWAYS); 
        //pInputStream->addListener(this);
    }
    ~SerialPort()
    {
        std::cout << "Serial Port Closing" << std::endl;
        close();
    }
    
    void addInputStreamListener(SensorRecorder* sr)
    {
        pInputStream->addListener(sr);
    }
    
    void removeInputStreamListener(SensorRecorder* sr)
    {
        sensorRecorderListeners.remove(sr);
        pInputStream->removeListener(sr);
    }
       
    bool open(const String & portPath);
    void close();
    bool setConfig(const SerialPortConfig & config);
    bool getConfig(SerialPortConfig & config);
    String getPortPath(){return portPath;}
    static StringPairArray getSerialPortPaths();
    bool exists();
    
    SerialPortInputStream* pInputStream;
    juce_UseDebuggingNewOperator
private:
    friend class SerialPortInputStream;
    friend class SerialPortOutputStream;
    
    void * portHandle;
    int portDescriptor;
    String portPath;
    ListenerList<SensorRecorder> sensorRecorderListeners;

};

#endif //_SERIALPORT_H_
