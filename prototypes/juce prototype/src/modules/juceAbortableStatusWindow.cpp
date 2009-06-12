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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "juceAbortableStatusWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
juceAbortableStatusWindow::juceAbortableStatusWindow ()
    : btnCancel (0),
      lblStatus (0)
{
    addAndMakeVisible (btnCancel = new TextButton (T("new button")));
    btnCancel->setButtonText (T("cancel"));
    btnCancel->addButtonListener (this);
    btnCancel->setColour (TextButton::buttonColourId, Colour (0xffffbbbb));
    btnCancel->setColour (TextButton::buttonOnColourId, Colour (0xffff4444));
    btnCancel->setColour (TextButton::textColourId, Colours::red);

    addAndMakeVisible (lblStatus = new Label (T("new label"),
                                              T("label text")));
    lblStatus->setFont (Font (15.0000f, Font::plain));
    lblStatus->setJustificationType (Justification::centred);
    lblStatus->setEditable (false, false, false);
    lblStatus->setColour (TextEditor::textColourId, Colours::black);
    lblStatus->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 120);

    //[Constructor] You can add your own custom stuff here..
	n02::cancelableInitialized = true;
    //[/Constructor]
}

juceAbortableStatusWindow::~juceAbortableStatusWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (btnCancel);
    deleteAndZero (lblStatus);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceAbortableStatusWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceAbortableStatusWindow::resized()
{
    btnCancel->setBounds (110, 70, 80, 30);
    lblStatus->setBounds (20, 20, 260, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceAbortableStatusWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnCancel)
    {
        //[UserButtonCode_btnCancel] -- add your button handler code here..
		n02::cancelled = true;
        //[/UserButtonCode_btnCancel]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void juceAbortableStatusWindow::updateStatus(char * status) {
	String text(status);
	lblStatus->setText(text,false);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceAbortableStatusWindow"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="10" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="300"
                 initialHeight="120">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="new button" id="cbe549c745624970" memberName="btnCancel"
              virtualName="" explicitFocusOrder="0" pos="110 70 80 30" bgColOff="ffffbbbb"
              bgColOn="ffff4444" textCol="ffff0000" buttonText="cancel" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="1b209b3ef695bfb6" memberName="lblStatus"
         virtualName="" explicitFocusOrder="0" pos="20 20 260 48" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
