/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  7 Jul 2008 11:59:57 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEABORTABLESTATUSWINDOW_JUCEABORTABLESTATUSWINDOW_DC47E865__
#define __JUCER_HEADER_JUCEABORTABLESTATUSWINDOW_JUCEABORTABLESTATUSWINDOW_DC47E865__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
namespace n02 {
	extern bool cancelled;
	extern volatile bool cancelableInitialized;
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class juceAbortableStatusWindow  : public Component,
                                   public ButtonListener
{
public:
    //==============================================================================
    juceAbortableStatusWindow ();
    ~juceAbortableStatusWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateStatus(char * status);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    TextButton* btnCancel;
    Label* lblStatus;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceAbortableStatusWindow (const juceAbortableStatusWindow&);
    const juceAbortableStatusWindow& operator= (const juceAbortableStatusWindow&);
};


#endif   // __JUCER_HEADER_JUCEABORTABLESTATUSWINDOW_JUCEABORTABLESTATUSWINDOW_DC47E865__
