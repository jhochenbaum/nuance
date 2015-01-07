/*
 *  mac_SerialPort.h
 *  juce_application
 *  Serial Port classes in a Juce stylee, by graffiti
 *
 *  Created by Jordan Hochenbaum on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

//see SerialPort.h for details

#include "stdio.h"
#include "sys/ioctl.h"
#include "termios.h"
#include "IOKit/serial/IOSerialKeys.h"
#include "fcntl.h" //added
#include "IOKit/IOTypes.h" //added
#include "IOKit/IOKitLib.h" //added

#include "JuceHeader.h"
#include "SerialPort.h"





