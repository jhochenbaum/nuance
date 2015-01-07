/*
 ==============================================================================
 
 "Nuance Audio & Sensor Recording Suite" application in JUCE
 Copyright 2009 by Jordan Hochenbaum.
 
 ==============================================================================
 */
#include "JuceHeader.h"
#include "MainWindow.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/** This is the application object that is started up when Juce starts. It handles
 * the initialisation and shutdown of the whole application.
 */

class JUCENuanceApplication : public JUCEApplication
{
    /* Important! NEVER embed objects directly inside your JUCEApplication class! Use
	 ONLY pointers to objects, which you should create during the initialise() method
	 (NOT in the constructor!) and delete in the shutdown() method (NOT in the
	 destructor!)
	 
	 This is because the application object gets created before Juce has been properly
	 initialised, so any embedded objects would also get constructed too soon.
	 */
	
    NuanceWindow* nuanceWindow;
	
	
public:
    CustomLookAndFeel* customLook;
    //==============================================================================
    JUCENuanceApplication() : nuanceWindow (0)
    {
        // NEVER do anything in here that could involve any Juce function being called
        // - leave all your startup tasks until the initialise() method.
    }
	
    ~JUCENuanceApplication()
    {
        // Your shutdown() method should already have done all the things necessary to
        // clean up this app object, so you should never need to put anything in
        // the destructor.
		
        // Making any Juce calls in here could be very dangerous...
    }
	
    //==============================================================================
    void initialise (const String& commandLine)
    {
		
		
		// just create the main window...
        nuanceWindow = new NuanceWindow();
		LookAndFeel::setDefaultLookAndFeel(customLook = new CustomLookAndFeel());
		
		// Check to see if the program settings directory is present on the users machine,
		// if not we must create it...
		
		//search our file path where we will store applications configurations and settings
		File configFolder = File(File::getSpecialLocation(File::commonApplicationDataDirectory).getFullPathName() + "/FlipMu/Nuance/" );
		
		if (!configFolder.isDirectory()) {
			configFolder.createDirectory();
		}
		
		//nuance_app = this;
		
        /*  ..and now return, which will fall into to the main event
		 dispatch loop, and this will run until something calls
		 JUCEAppliction::quit().
		 
		 In this case, JUCEAppliction::quit() will be called by the
		 main window being clicked.
		 */
    }
	
    void shutdown()
    {
        // make sure we delete our window and anything else that needs looking after (look and feel ptrs, etc)
        if (nuanceWindow != 0){
            delete nuanceWindow;
			delete customLook; 
		}
    }
	
    //==============================================================================
    const String getApplicationName()
    {
        return "Nuance Audio and Sensor Recorder";
    }
	
    const String getApplicationVersion()
    {
        return "1.0";
    }
	
    bool moreThanOneInstanceAllowed()
    {
        return true;
    }
	
    void anotherInstanceStarted (const String& commandLine)
    {
    }
		
};


//==============================================================================
// This macro creates the application's main() function..
START_JUCE_APPLICATION (JUCENuanceApplication)
