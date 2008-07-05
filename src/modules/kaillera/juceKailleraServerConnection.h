/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Jul 2008 4:06:40 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_AEF5E007__
#define __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_AEF5E007__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class juceKailleraServerConnection  : public Component,
                                      public ButtonListener
{
public:
    //==============================================================================
    juceKailleraServerConnection ();
    ~juceKailleraServerConnection();

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
    Component* lstUsers;
    TextEditor* textEditor;
    TextEditor* textEditor2;
    TextButton* textButton;
    Component* component;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraServerConnection (const juceKailleraServerConnection&);
    const juceKailleraServerConnection& operator= (const juceKailleraServerConnection&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_AEF5E007__
