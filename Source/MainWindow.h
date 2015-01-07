/*
 *  MainWindow.h
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 3/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NuanceWindow_
#define _NuanceWindow_

#include "JuceHeader.h"
#include "MainComponent.h"

class NuanceWindow  : public DocumentWindow
{
	
	void closeButtonPressed() 
	{
        JUCEApplication::quit();
    }

public:
	
	NuanceWindow						();
	~NuanceWindow						();
	
	MainComponent*						contentComponent;
	
private:
	ScopedPointer<ApplicationCommandManager>	commandManager;
	
};

#endif //_NuanceWindow_