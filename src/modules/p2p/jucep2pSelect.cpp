/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  29 Jul 2008 12:58:05 pm

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

#include "jucep2pSelect.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
jucep2pSelect::jucep2pSelect ()
    : label (0),
      label2 (0),
      txtNick (0),
      txtPort (0),
      cmbModes (0),
      btnNew (0),
      txtIP (0),
      btnConnect (0),
      label3 (0),
      lstStored (0),
      label4 (0),
      btnAdd (0),
      btnEdit (0),
      btnDelete (0)
{
    addAndMakeVisible (label = new Label (T("new label"),
                                          T("Nick:")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (T("new label"),
                                           T("Port:")));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (txtNick = new TextEditor (T("new text editor")));
    txtNick->setMultiLine (false);
    txtNick->setReturnKeyStartsNewLine (false);
    txtNick->setReadOnly (false);
    txtNick->setScrollbarsShown (true);
    txtNick->setCaretVisible (true);
    txtNick->setPopupMenuEnabled (true);
    txtNick->setText (String::empty);

    addAndMakeVisible (txtPort = new TextEditor (T("new text editor")));
    txtPort->setMultiLine (false);
    txtPort->setReturnKeyStartsNewLine (false);
    txtPort->setReadOnly (false);
    txtPort->setScrollbarsShown (true);
    txtPort->setCaretVisible (true);
    txtPort->setPopupMenuEnabled (true);
    txtPort->setText (String::empty);

    addAndMakeVisible (cmbModes = new ComboBox (T("new combo box")));
    cmbModes->setEditableText (false);
    cmbModes->setJustificationType (Justification::centredLeft);
    cmbModes->setTextWhenNothingSelected (T("Switch"));
    cmbModes->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbModes->addListener (this);

    addAndMakeVisible (btnNew = new TextButton (T("new button")));
    btnNew->setButtonText (T("new game"));
    btnNew->setConnectedEdges (Button::ConnectedOnLeft);
    btnNew->addButtonListener (this);

    addAndMakeVisible (txtIP = new TextEditor (T("new text editor")));
    txtIP->setMultiLine (false);
    txtIP->setReturnKeyStartsNewLine (false);
    txtIP->setReadOnly (false);
    txtIP->setScrollbarsShown (true);
    txtIP->setCaretVisible (true);
    txtIP->setPopupMenuEnabled (true);
    txtIP->setText (String::empty);

    addAndMakeVisible (btnConnect = new TextButton (T("new button")));
    btnConnect->setButtonText (T("Connect"));
    btnConnect->setConnectedEdges (Button::ConnectedOnRight);
    btnConnect->addButtonListener (this);

    addAndMakeVisible (label3 = new Label (T("new label"),
                                           T("IP/SID:")));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lstStored = new TableListBox (T("storedList"), &storedListBoxModel));
    lstStored->setName (T("new component"));

    addAndMakeVisible (label4 = new Label (T("new label"),
                                           T("Stored:")));
    label4->setFont (Font (15.0000f, Font::plain));
    label4->setJustificationType (Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (btnAdd = new TextButton (T("new button")));
    btnAdd->setButtonText (T("add"));
    btnAdd->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    btnAdd->addButtonListener (this);
    btnAdd->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (btnEdit = new TextButton (T("new button")));
    btnEdit->setButtonText (T("edit"));
    btnEdit->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    btnEdit->addButtonListener (this);
    btnEdit->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (btnDelete = new TextButton (T("new button")));
    btnDelete->setButtonText (T("delete"));
    btnDelete->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    btnDelete->addButtonListener (this);
    btnDelete->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 344);

    //[Constructor] You can add your own custom stuff here..

	{ // modes
		ModuleAbstraction02 * mod = n02::modHelper.modHandler->find(MTYPE02_TRANSPORT);
		if (mod != 0) {
			do {
				cmbModes->addItem(mod->name, mod->index);
			} while ((mod = n02::modHelper.modHandler->findNext(mod))!=0);
		}
	}

	txtIP->setText(FROMUTF8(n02::p2p::ip));
	txtNick->setText(FROMUTF8(n02::p2p::nick));
	txtPort->setText(String(n02::p2p::port));

	// List
	lstStored->getHeader()->addColumn("Name", 1, 482/2, 30, -1, TableHeaderComponent::notSortable);
	lstStored->getHeader()->addColumn("IP Address", 2, 482/2 - 20, 30, -1, TableHeaderComponent::notSortable);

	lstStored->setColour (TableListBox::outlineColourId, Colours::grey);
	lstStored->setOutlineThickness (1);

    //[/Constructor]
}

jucep2pSelect::~jucep2pSelect()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (txtNick);
    deleteAndZero (txtPort);
    deleteAndZero (cmbModes);
    deleteAndZero (btnNew);
    deleteAndZero (txtIP);
    deleteAndZero (btnConnect);
    deleteAndZero (label3);
    deleteAndZero (lstStored);
    deleteAndZero (label4);
    deleteAndZero (btnAdd);
    deleteAndZero (btnEdit);
    deleteAndZero (btnDelete);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void jucep2pSelect::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void jucep2pSelect::resized()
{
    label->setBounds (16, 16, 48, 24);
    label2->setBounds (16, 48, 48, 24);
    txtNick->setBounds (72, 16, 240, 24);
    txtPort->setBounds (72, 48, 240, 24);
    cmbModes->setBounds (464, 16, 120, 24);
    btnNew->setBounds (480, 80, 104, 24);
    txtIP->setBounds (72, 80, 328, 24);
    btnConnect->setBounds (408, 80, 72, 24);
    label3->setBounds (8, 80, 56, 24);
    lstStored->setBounds (72, 112, getWidth() - 88, getHeight() - 128);
    label4->setBounds (8, 112, 56, 24);
    btnAdd->setBounds (16, getHeight() - 88, 56, 24);
    btnEdit->setBounds (16, getHeight() - 64, 56, 24);
    btnDelete->setBounds (16, getHeight() - 40, 56, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void jucep2pSelect::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbModes)
    {
        //[UserComboBoxCode_cmbModes] -- add your combo box handling code here..
		n02::p2p::uiModChangeCallback(cmbModes->getSelectedId());
        //[/UserComboBoxCode_cmbModes]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void jucep2pSelect::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
	saveConfig();
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnNew)
    {
        //[UserButtonCode_btnNew] -- add your button handler code here..
		n02::p2p::uiHost();
        //[/UserButtonCode_btnNew]
    }
    else if (buttonThatWasClicked == btnConnect)
    {
        //[UserButtonCode_btnConnect] -- add your button handler code here..
		n02::p2p::uiConnect();
        //[/UserButtonCode_btnConnect]
    }
    else if (buttonThatWasClicked == btnAdd)
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
		n02::p2p::uiAddServer();
		lstStored->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnEdit)
    {
        //[UserButtonCode_btnEdit] -- add your button handler code here..
		n02::p2p::uiEditServer(lstStored->getSelectedRow());
		lstStored->repaint();
        //[/UserButtonCode_btnEdit]
    }
    else if (buttonThatWasClicked == btnDelete)
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
		n02::p2p::uiDeleteServer(lstStored->getSelectedRow());
		lstStored->updateContent();
		lstStored->repaint();
        //[/UserButtonCode_btnDelete]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void jucep2pSelect::saveConfig()
{
	strncpy(n02::p2p::ip, txtIP->getText().toUTF8(), 127);
	strncpy(n02::p2p::nick, txtNick->getText().toUTF8(), 31);
	n02::p2p::port = min(max(txtPort->getText().getIntValue(), 0), 655634);
}
void jucep2pSelect::handleCommandMessage(int  commandId) {
	char * x = reinterpret_cast<char*>(commandId);
	txtIP->setText(FROMUTF8(x));
	saveConfig();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="jucep2pSelect" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="600" initialHeight="344">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="new label" id="fd443000655b7872" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 16 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Nick:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="bf78e7cfe9bad832" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="16 48 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Port:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <TEXTEDITOR name="new text editor" id="5cf2dc4bb5a24cb4" memberName="txtNick"
              virtualName="" explicitFocusOrder="0" pos="72 16 240 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="d687d5b410d5b0d2" memberName="txtPort"
              virtualName="" explicitFocusOrder="0" pos="72 48 240 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <COMBOBOX name="new combo box" id="7faeef2c3e3dd267" memberName="cmbModes"
            virtualName="" explicitFocusOrder="0" pos="464 16 120 24" editable="0"
            layout="33" items="" textWhenNonSelected="Switch" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="635cc93b8c86c85c" memberName="btnNew" virtualName=""
              explicitFocusOrder="0" pos="480 80 104 24" buttonText="new game"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="6a7eda4fa71e245c" memberName="txtIP"
              virtualName="" explicitFocusOrder="0" pos="72 80 328 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="66fb92ddb6861e57" memberName="btnConnect"
              virtualName="" explicitFocusOrder="0" pos="408 80 72 24" buttonText="Connect"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="4028d6aeeb60cf60" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 80 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP/SID:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <GENERICCOMPONENT name="new component" id="4f00277e359b081b" memberName="lstStored"
                    virtualName="" explicitFocusOrder="0" pos="72 112 88M 128M" class="TableListBox"
                    params="T(&quot;storedList&quot;), &amp;storedListBoxModel"/>
  <LABEL name="new label" id="c398f2871db66354" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 112 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Stored:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="new button" id="7820dfae9523cee6" memberName="btnAdd" virtualName=""
              explicitFocusOrder="0" pos="16 88R 56 24" bgColOff="ffbbbbff"
              buttonText="add" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="38bc363dbffd9a1d" memberName="btnEdit"
              virtualName="" explicitFocusOrder="0" pos="16 64R 56 24" bgColOff="ffbbbbff"
              buttonText="edit" connectedEdges="14" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="17e3c0b7c04af57d" memberName="btnDelete"
              virtualName="" explicitFocusOrder="0" pos="16 40R 56 24" bgColOff="ffbbbbff"
              buttonText="delete" connectedEdges="6" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
