/*
 *  MainWindow.cpp
 *  Nuance
 *
 *  Created by Jordan Hochenbaum on 3/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MainWindow.h"

NuanceWindow::NuanceWindow() : DocumentWindow ("Nuance Audio & Data Recorder", Colours::black, 
											   DocumentWindow::allButtons, true) 
{
	// Create an instance of our main content component, and add it 
	// to our window.
	setResizable(true, true);
	commandManager = new ApplicationCommandManager();
	MainComponent* const contentComponent = new MainComponent();
	setContentOwned(contentComponent, false);
	centreWithSize (700, 650);		
	//setMenuBar(contentComponent, 20);
	//contentComponent->setMacMainMenu(this);
	setUsingNativeTitleBar(true);	
	//ApplicationCommandManager commandManager;
	contentComponent->setCommandManager(commandManager);
	addKeyListener(commandManager->getKeyMappings());
	setVisible (true);
}

NuanceWindow::~NuanceWindow()
{
	// (the content component will be deleted automatically, so no need to do it here)
}