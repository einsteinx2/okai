/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  28 Jul 2008 10:19:43 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEMODDEFSTRESSTEST_JUCEMODDEFSTRESSTEST_69EA81C8__
#define __JUCER_HEADER_JUCEMODDEFSTRESSTEST_JUCEMODDEFSTRESSTEST_69EA81C8__

//[Headers]     -- You can add your own extra header files here --
#include "juce02.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class juceModDefStressTest  : public Component,
                              public ComboBoxListener,
                              public ButtonListener,
                              public SliderListener
{
public:
    //==============================================================================
    juceModDefStressTest ();
    ~juceModDefStressTest();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateStatus(double fps);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    Label* label;
    ComboBox* cmbGames;
    Label* label2;
    TextButton* btnStart;
    Slider* sldDelay;
    Label* label3;
    TextButton* btnStop;
    Label* label4;
    Label* lblStatus;
    ComboBox* cmbModes;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceModDefStressTest (const juceModDefStressTest&);
    const juceModDefStressTest& operator= (const juceModDefStressTest&);
};


#endif   // __JUCER_HEADER_JUCEMODDEFSTRESSTEST_JUCEMODDEFSTRESSTEST_69EA81C8__
