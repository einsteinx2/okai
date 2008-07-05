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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "juceKailleraServerGame.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NewJucerComponent::NewJucerComponent ()
    : textEditor (0),
      textEditor2 (0),
      textButton (0),
      component (0),
      textButton2 (0),
      textButton3 (0),
      textButton4 (0),
      comboBox (0),
      label (0),
      comboBox2 (0),
      label2 (0)
{
    addAndMakeVisible (textEditor = new TextEditor (T("new text editor")));
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (true);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (String::empty);

    addAndMakeVisible (textEditor2 = new TextEditor (T("new text editor")));
    textEditor2->setMultiLine (false);
    textEditor2->setReturnKeyStartsNewLine (false);
    textEditor2->setReadOnly (false);
    textEditor2->setScrollbarsShown (true);
    textEditor2->setCaretVisible (true);
    textEditor2->setPopupMenuEnabled (true);
    textEditor2->setText (String::empty);

    addAndMakeVisible (textButton = new TextButton (T("new button")));
    textButton->setButtonText (T("chat"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButton->addButtonListener (this);

    addAndMakeVisible (component = new Component());
    component->setName (T("new component"));

    addAndMakeVisible (textButton2 = new TextButton (T("new button")));
    textButton2->setButtonText (T("start game"));
    textButton2->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    textButton2->addButtonListener (this);

    addAndMakeVisible (textButton3 = new TextButton (T("new button")));
    textButton3->setButtonText (T("leave game"));
    textButton3->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
    textButton3->addButtonListener (this);

    addAndMakeVisible (textButton4 = new TextButton (T("new button")));
    textButton4->setButtonText (T("kick user"));
    textButton4->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    textButton4->addButtonListener (this);

    addAndMakeVisible (comboBox = new ComboBox (T("new combo box")));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String::empty);
    comboBox->setTextWhenNoChoicesAvailable (T("(no choices)"));
    comboBox->addListener (this);

    addAndMakeVisible (label = new Label (T("new label"),
                                          T("Mode:")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (comboBox2 = new ComboBox (T("new combo box")));
    comboBox2->setEditableText (false);
    comboBox2->setJustificationType (Justification::centredLeft);
    comboBox2->setTextWhenNothingSelected (String::empty);
    comboBox2->setTextWhenNoChoicesAvailable (T("(no choices)"));
    comboBox2->addListener (this);

    addAndMakeVisible (label2 = new Label (T("new label"),
                                           T("Delay:")));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 250);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

NewJucerComponent::~NewJucerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (textEditor);
    deleteAndZero (textEditor2);
    deleteAndZero (textButton);
    deleteAndZero (component);
    deleteAndZero (textButton2);
    deleteAndZero (textButton3);
    deleteAndZero (textButton4);
    deleteAndZero (comboBox);
    deleteAndZero (label);
    deleteAndZero (comboBox2);
    deleteAndZero (label2);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NewJucerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NewJucerComponent::resized()
{
    textEditor->setBounds (8, 8, 328, 208);
    textEditor2->setBounds (8, 216, 288, 24);
    textButton->setBounds (296, 216, 40, 24);
    component->setBounds (344, 8, 296, 176);
    textButton2->setBounds (344, 184, 104, 24);
    textButton3->setBounds (448, 184, 96, 24);
    textButton4->setBounds (544, 184, 96, 24);
    comboBox->setBounds (400, 216, 104, 24);
    label->setBounds (352, 216, 48, 24);
    comboBox2->setBounds (560, 216, 78, 24);
    label2->setBounds (512, 216, 48, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NewJucerComponent::buttonClicked (Button* buttonThatWasClicked)
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
    else if (buttonThatWasClicked == textButton3)
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4)
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void NewJucerComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == comboBox2)
    {
        //[UserComboBoxCode_comboBox2] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox2]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NewJucerComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="650" initialHeight="250">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="new text editor" id="876eff7511524f9e" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 8 328 208" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="7bf7b60c15eeb8b" memberName="textEditor2"
              virtualName="" explicitFocusOrder="0" pos="8 216 288 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="ccfa755ad8e8e362" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="296 216 40 24" buttonText="chat"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="f9436104229f377e" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="344 8 296 176" class="Component"
                    params=""/>
  <TEXTBUTTON name="new button" id="83beae5f7ea8f048" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="344 184 104 24" buttonText="start game"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="4d67e87e8755e8dc" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="448 184 96 24" buttonText="leave game"
              connectedEdges="7" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="90d5c5b1e0a0b2e2" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="544 184 96 24" buttonText="kick user"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="19e29820f9b5e74b" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="400 216 104 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="6e270a333381e674" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="352 216 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="ac49a7cfb8110296" memberName="comboBox2"
            virtualName="" explicitFocusOrder="0" pos="560 216 78 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="2c62b0ee329fcd39" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="512 216 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Delay:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
