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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "juceKailleraServerConnection.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
juceKailleraServerConnection::juceKailleraServerConnection ()
    : lstUsers (0),
      textEditor (0),
      textEditor2 (0),
      textButton (0),
      component (0)
{
    addAndMakeVisible (lstUsers = new Component());
    lstUsers->setName (T("new component"));

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
    textButton->setButtonText (T("new game"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    textButton->addButtonListener (this);

    addAndMakeVisible (component = new Component());
    component->setName (T("new component"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (750, 500);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

juceKailleraServerConnection::~juceKailleraServerConnection()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (lstUsers);
    deleteAndZero (textEditor);
    deleteAndZero (textEditor2);
    deleteAndZero (textButton);
    deleteAndZero (component);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceKailleraServerConnection::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceKailleraServerConnection::resized()
{
    lstUsers->setBounds (744 - 192, 8, 192, 336);
    textEditor->setBounds (8, 8, (192) + 544, 312);
    textEditor2->setBounds (8, 320, 472, 24);
    textButton->setBounds (480, 320, 71, 24);
    component->setBounds (8, 344, 736, 152);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceKailleraServerConnection::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
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

<JUCER_COMPONENT documentType="Component" className="juceKailleraServerConnection"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="750"
                 initialHeight="500">
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="new component" id="8b78dd76054f6a1a" memberName="lstUsers"
                    virtualName="" explicitFocusOrder="0" pos="744r 8 192 336" class="Component"
                    params=""/>
  <TEXTEDITOR name="new text editor" id="84ef9fa8765d7afd" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 8 544 312" posRelativeW="8b78dd76054f6a1a"
              initialText="" multiline="1" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="5d7f853814619b93" memberName="textEditor2"
              virtualName="" explicitFocusOrder="0" pos="8 320 472 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="ca7fb394f3815c91" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="480 320 71 24" buttonText="new game"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="9340b1da010dd5f9" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="8 344 736 152" class="Component"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
