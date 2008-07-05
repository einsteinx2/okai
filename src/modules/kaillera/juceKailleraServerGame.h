/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Jul 2008 3:10:01 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_NEWJUCERCOMPONENT_JUCEKAILLERASERVERGAME_ACD315B4__
#define __JUCER_HEADER_NEWJUCERCOMPONENT_JUCEKAILLERASERVERGAME_ACD315B4__

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
class NewJucerComponent  : public Component,
                           public ButtonListener,
                           public ComboBoxListener
{
public:
    //==============================================================================
    NewJucerComponent ();
    ~NewJucerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    TextEditor* textEditor;
    TextEditor* textEditor2;
    TextButton* textButton;
    Component* component;
    TextButton* textButton2;
    TextButton* textButton3;
    TextButton* textButton4;
    ComboBox* comboBox;
    Label* label;
    ComboBox* comboBox2;
    Label* label2;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    NewJucerComponent (const NewJucerComponent&);
    const NewJucerComponent& operator= (const NewJucerComponent&);
};


#endif   // __JUCER_HEADER_NEWJUCERCOMPONENT_JUCEKAILLERASERVERGAME_ACD315B4__
