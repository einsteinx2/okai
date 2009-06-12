/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Aug 2008 3:03:23 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEADDEDITIPDIALOG_JUCEADDEDITIPDIALOG_C4139AB3__
#define __JUCER_HEADER_JUCEADDEDITIPDIALOG_JUCEADDEDITIPDIALOG_C4139AB3__

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
class juceAddEditIPDialog  : public Component,
                             public ButtonListener
{
public:
    //==============================================================================
    juceAddEditIPDialog ();
    ~juceAddEditIPDialog();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
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
    Label* label;
    Label* label2;
    TextEditor* txtName;
    TextEditor* txtIP;
    TextButton* btnAdd;
    TextButton* btnCancel;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceAddEditIPDialog (const juceAddEditIPDialog&);
    const juceAddEditIPDialog& operator= (const juceAddEditIPDialog&);
};


#endif   // __JUCER_HEADER_JUCEADDEDITIPDIALOG_JUCEADDEDITIPDIALOG_C4139AB3__
