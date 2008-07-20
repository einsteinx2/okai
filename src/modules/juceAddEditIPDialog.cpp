/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Jul 2008 2:15:49 pm

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
#include "common.h"
//[/Headers]

#include "juceAddEditIPDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

namespace n02 {
	namespace addedit {
		extern int returnValue;
		extern char name[128];
        extern char ip[128];
	};
};
using namespace n02::addedit;
//[/MiscUserDefs]

//==============================================================================
juceAddEditIPDialog::juceAddEditIPDialog ()
    : Component (T("add edit dlg")),
      label (0),
      label2 (0),
      txtName (0),
      txtIP (0),
      btnAdd (0),
      btnCancel (0)
{
    addAndMakeVisible (label = new Label (T("new label"),
                                          T("Name:")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (T("new label"),
                                           T("IP:")));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (txtName = new TextEditor (T("new text editor")));
    txtName->setMultiLine (false);
    txtName->setReturnKeyStartsNewLine (false);
    txtName->setReadOnly (false);
    txtName->setScrollbarsShown (true);
    txtName->setCaretVisible (true);
    txtName->setPopupMenuEnabled (true);
    txtName->setText (String::empty);

    addAndMakeVisible (txtIP = new TextEditor (T("new text editor")));
    txtIP->setMultiLine (false);
    txtIP->setReturnKeyStartsNewLine (false);
    txtIP->setReadOnly (false);
    txtIP->setScrollbarsShown (true);
    txtIP->setCaretVisible (true);
    txtIP->setPopupMenuEnabled (true);
    txtIP->setText (String::empty);

    addAndMakeVisible (btnAdd = new TextButton (T("new button")));
    btnAdd->setButtonText (T("add"));
    btnAdd->setConnectedEdges (Button::ConnectedOnRight);
    btnAdd->addButtonListener (this);
    btnAdd->setColour (TextButton::buttonColourId, Colour (0xffbbffc7));

    addAndMakeVisible (btnCancel = new TextButton (T("new button")));
    btnCancel->setButtonText (T("cancel"));
    btnCancel->setConnectedEdges (Button::ConnectedOnLeft);
    btnCancel->addButtonListener (this);
    btnCancel->setColour (TextButton::buttonColourId, Colour (0xffffd3bb));


    //[UserPreSize]
	if (returnValue==1) {
		btnAdd->setButtonText(T("Edit"));

		txtIP->setText(FROMUTF8(ip));
		txtName->setText(FROMUTF8(name));
	}
    //[/UserPreSize]

    setSize (296, 120);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

juceAddEditIPDialog::~juceAddEditIPDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (txtName);
    deleteAndZero (txtIP);
    deleteAndZero (btnAdd);
    deleteAndZero (btnCancel);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceAddEditIPDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceAddEditIPDialog::resized()
{
    label->setBounds (16, 16, 56, 24);
    label2->setBounds (16, 48, 56, 24);
    txtName->setBounds (80, 16, 200, 24);
    txtIP->setBounds (80, 48, 200, 24);
    btnAdd->setBounds (152, 80, 64, 24);
    btnCancel->setBounds (216, 80, 64, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceAddEditIPDialog::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnAdd)
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
		if (txtIP->getText().length() > 0) {

			strncpy(ip, txtIP->getText().toUTF8(), 127);
			strncpy(name, txtName->getText().toUTF8(), 127);

			if (strlen(name) ==0)
				strcpy(name, ip);

			getCurrentlyModalComponent()->exitModalState(returnValue = 1);
		}
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnCancel)
    {
        //[UserButtonCode_btnCancel] -- add your button handler code here..
		getCurrentlyModalComponent()->exitModalState(returnValue = 0);
        //[/UserButtonCode_btnCancel]
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

<JUCER_COMPONENT documentType="Component" className="juceAddEditIPDialog" componentName="add edit dlg"
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="296" initialHeight="120">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="new label" id="bc6e2dff38f4319e" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 16 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Name:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="2e8bf0c4ebd7172b" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="16 48 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="new text editor" id="a9b27039ff7b269d" memberName="txtName"
              virtualName="" explicitFocusOrder="0" pos="80 16 200 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="34dc5f1ee5e26328" memberName="txtIP"
              virtualName="" explicitFocusOrder="0" pos="80 48 200 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="40e5bf395e8bb212" memberName="btnAdd" virtualName=""
              explicitFocusOrder="0" pos="152 80 64 24" bgColOff="ffbbffc7"
              buttonText="add" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f79a1fcb769fcdd4" memberName="btnCancel"
              virtualName="" explicitFocusOrder="0" pos="216 80 64 24" bgColOff="ffffd3bb"
              buttonText="cancel" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
