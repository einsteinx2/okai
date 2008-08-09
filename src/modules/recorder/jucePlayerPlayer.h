/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Aug 2008 9:16:09 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEPLAYERPLAYER_JUCEPLAYERPLAYER_C6709985__
#define __JUCER_HEADER_JUCEPLAYERPLAYER_JUCEPLAYERPLAYER_C6709985__

//[Headers]     -- You can add your own extra header files here --
#include "clientgui.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class jucePlayerPlayer  : public Component
{
public:
    //==============================================================================
    jucePlayerPlayer ();
    ~jucePlayerPlayer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void handleCommandMessage (int commandId);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int textLength;
    //[/UserVariables]

    //==============================================================================
    ProgressBar* pbProgress;
    TextEditor* txtStatus;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    jucePlayerPlayer (const jucePlayerPlayer&);
    const jucePlayerPlayer& operator= (const jucePlayerPlayer&);
};


#endif   // __JUCER_HEADER_JUCEPLAYERPLAYER_JUCEPLAYERPLAYER_C6709985__
