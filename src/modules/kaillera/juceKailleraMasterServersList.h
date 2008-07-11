/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Jul 2008 5:11:09 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEKAILLERAMASTERSERVERSLIST_JUCEKAILLERAMASTERSERVERSLIST_5D098F1A__
#define __JUCER_HEADER_JUCEKAILLERAMASTERSERVERSLIST_JUCEKAILLERAMASTERSERVERSLIST_5D098F1A__

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
class juceKailleraMasterServersList  : public Component,
                                       public ButtonListener
{
public:
    //==============================================================================
    juceKailleraMasterServersList ();
    ~juceKailleraMasterServersList();

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
    TextEditor* textEditor;
    Label* label;
    TextButton* textButton;
    TextButton* textButton2;
    Component* component;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraMasterServersList (const juceKailleraMasterServersList&);
    const juceKailleraMasterServersList& operator= (const juceKailleraMasterServersList&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERAMASTERSERVERSLIST_JUCEKAILLERAMASTERSERVERSLIST_5D098F1A__
