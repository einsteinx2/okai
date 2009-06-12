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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "juceKailleraMasterServersList.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
juceKailleraMasterServersList::juceKailleraMasterServersList ()
    : textEditor (0),
      label (0),
      textButton (0),
      textButton2 (0),
      component (0)
{
    addAndMakeVisible (textEditor = new TextEditor (T("new text editor")));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (String::empty);

    addAndMakeVisible (label = new Label (T("new label"),
                                          T("URL:")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (textButton = new TextButton (T("new button")));
    textButton->setButtonText (T("go"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    textButton->addButtonListener (this);

    addAndMakeVisible (textButton2 = new TextButton (T("new button")));
    textButton2->setButtonText (T("stop"));
    textButton2->setConnectedEdges (Button::ConnectedOnLeft);
    textButton2->addButtonListener (this);

    addAndMakeVisible (component = new Component());
    component->setName (T("new component"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 400);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

juceKailleraMasterServersList::~juceKailleraMasterServersList()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (textEditor);
    deleteAndZero (label);
    deleteAndZero (textButton);
    deleteAndZero (textButton2);
    deleteAndZero (component);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceKailleraMasterServersList::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceKailleraMasterServersList::resized()
{
    textEditor->setBounds (48, 8, getWidth() - 162, 24);
    label->setBounds (8, 8, 40, 24);
    textButton->setBounds (getWidth() - 114, 8, 48, 24);
    textButton2->setBounds (getWidth() - 66, 8, 56, 24);
    component->setBounds (8, 40, getWidth() - 16, getHeight() - 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceKailleraMasterServersList::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceKailleraMasterServersList"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="650"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="new text editor" id="5b98b6faed99fe16" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="48 8 162M 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="80c59e95e121f503" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 40 24" edTextCol="ff000000" edBkgCol="0"
         labelText="URL:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="new button" id="ec8fc74ddfcb81eb" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="114R 8 48 24" buttonText="go"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="4f979ff6945e65b0" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="66R 8 56 24" buttonText="stop"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="8fa7acd8779b6ec6" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="8 40 16M 48M" class="Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
