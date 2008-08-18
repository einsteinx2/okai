/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Jul 2008 3:06:03 pm

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
#include "juceModHelpers.h"
//[/Headers]

#include "juceKailleraServerGame.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
juceKailleraServerGame::juceKailleraServerGame ()
    : txtChat (0),
      txtChatInput (0),
      lstPlayers (0),
      btnStartGame (0),
      btnLeaveGame (0),
      btnKick (0),
      cmbRun (0),
      lblRun (0),
      cmbDelay (0),
      lblDelay (0),
      chkRecord (0)
{
    addAndMakeVisible (txtChat = new TextEditor (T("new text editor")));
    txtChat->setTooltip (T("chat messages are displayed here"));
    txtChat->setMultiLine (true);
    txtChat->setReturnKeyStartsNewLine (false);
    txtChat->setReadOnly (true);
    txtChat->setScrollbarsShown (true);
    txtChat->setCaretVisible (true);
    txtChat->setPopupMenuEnabled (true);
    txtChat->setText (String::empty);

    addAndMakeVisible (txtChatInput = new TextEditor (T("new text editor")));
    txtChatInput->setTooltip (T("insert chat input in this box and press return to send chat messages"));
    txtChatInput->setMultiLine (false);
    txtChatInput->setReturnKeyStartsNewLine (false);
    txtChatInput->setReadOnly (false);
    txtChatInput->setScrollbarsShown (true);
    txtChatInput->setCaretVisible (true);
    txtChatInput->setPopupMenuEnabled (true);
    txtChatInput->setText (String::empty);

    addAndMakeVisible (lstPlayers = new TableListBox (T("playersList"), &kailleraPlayers));
    lstPlayers->setName (T("new component"));

    addAndMakeVisible (btnStartGame = new TextButton (T("new button")));
    btnStartGame->setTooltip (T("to start the game"));
    btnStartGame->setButtonText (T("start"));
    btnStartGame->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    btnStartGame->addButtonListener (this);

    addAndMakeVisible (btnLeaveGame = new TextButton (T("new button")));
    btnLeaveGame->setTooltip (T("to leave the game"));
    btnLeaveGame->setButtonText (T("leave"));
    btnLeaveGame->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
    btnLeaveGame->addButtonListener (this);

    addAndMakeVisible (btnKick = new TextButton (T("new button")));
    btnKick->setTooltip (T("to kick someone out of the game"));
    btnKick->setButtonText (T("kick"));
    btnKick->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    btnKick->addButtonListener (this);

    addAndMakeVisible (cmbRun = new ComboBox (T("new combo box")));
    cmbRun->setTooltip (T("switch between different running modes"));
    cmbRun->setEditableText (false);
    cmbRun->setJustificationType (Justification::centredLeft);
    cmbRun->setTextWhenNothingSelected (String::empty);
    cmbRun->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbRun->addListener (this);

    addAndMakeVisible (lblRun = new Label (T("new label"),
                                           T("Mode:")));
    lblRun->setFont (Font (15.0000f, Font::plain));
    lblRun->setJustificationType (Justification::centredLeft);
    lblRun->setEditable (false, false, false);
    lblRun->setColour (TextEditor::textColourId, Colours::black);
    lblRun->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (cmbDelay = new ComboBox (T("new combo box")));
    cmbDelay->setTooltip (T("select the additional delay parameter for the selected running mode"));
    cmbDelay->setEditableText (false);
    cmbDelay->setJustificationType (Justification::centredLeft);
    cmbDelay->setTextWhenNothingSelected (String::empty);
    cmbDelay->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbDelay->addListener (this);

    addAndMakeVisible (lblDelay = new Label (T("new label"),
                                             T("Delay:")));
    lblDelay->setFont (Font (15.0000f, Font::plain));
    lblDelay->setJustificationType (Justification::centredLeft);
    lblDelay->setEditable (false, false, false);
    lblDelay->setColour (TextEditor::textColourId, Colours::black);
    lblDelay->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (chkRecord = new ToggleButton (T("new toggle button")));
    chkRecord->setTooltip (T("check this to enable recording"));
    chkRecord->setButtonText (T("record game"));
    chkRecord->addButtonListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (650, 250);

    //[Constructor] You can add your own custom stuff here..
	// Players LB
	lstPlayers->getHeader()->addColumn("Nick", 1, 110, 30, -1, TableHeaderComponent::notSortable);
	lstPlayers->getHeader()->addColumn("Ping", 2, 40, 30, -1, TableHeaderComponent::notSortable);
	lstPlayers->getHeader()->addColumn("C.S.", 3, 40, 30, -1, TableHeaderComponent::notSortable);
	lstPlayers->getHeader()->addColumn("Delay", 4, 60, 30, -1, TableHeaderComponent::notSortable);
	lstPlayers->setColour (TableListBox::outlineColourId, Colours::grey);
	lstPlayers->setOutlineThickness (1);

	//autorun items
	updateAutorunItems();

	//recorder
	if (n02::modHelper.isRecorderLoaded()) {
		chkRecord->setToggleState(n02::kaillera::recordingEnabled!=0, false);
	} else {
		chkRecord->setToggleState((n02::kaillera::recordingEnabled=0)!=0, false);
		chkRecord->setEnabled(false);
	}

	clearText();

	txtChatInput->addListener(&chatInput);

    //[/Constructor]
}

juceKailleraServerGame::~juceKailleraServerGame()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (txtChat);
    deleteAndZero (txtChatInput);
    deleteAndZero (lstPlayers);
    deleteAndZero (btnStartGame);
    deleteAndZero (btnLeaveGame);
    deleteAndZero (btnKick);
    deleteAndZero (cmbRun);
    deleteAndZero (lblRun);
    deleteAndZero (cmbDelay);
    deleteAndZero (lblDelay);
    deleteAndZero (chkRecord);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceKailleraServerGame::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceKailleraServerGame::resized()
{
    txtChat->setBounds (8, 8, 328, 208);
    txtChatInput->setBounds (8, 216, 328, 24);
    lstPlayers->setBounds (344, 8, 296, 176);
    btnStartGame->setBounds (344, 184, 56, 24);
    btnLeaveGame->setBounds (400, 184, 56, 24);
    btnKick->setBounds (456, 184, 64, 24);
    cmbRun->setBounds (392, 216, 104, 24);
    lblRun->setBounds (344, 216, 48, 24);
    cmbDelay->setBounds (552, 216, 88, 24);
    lblDelay->setBounds (504, 216, 48, 24);
    chkRecord->setBounds (528, 184, 112, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceKailleraServerGame::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnStartGame)
    {
        //[UserButtonCode_btnStartGame] -- add your button handler code here..
		n02::kaillera::uiStartGameCallback();
        //[/UserButtonCode_btnStartGame]
    }
    else if (buttonThatWasClicked == btnLeaveGame)
    {
        //[UserButtonCode_btnLeaveGame] -- add your button handler code here..
		n02::kaillera::uiLeaveGameCallback();
        //[/UserButtonCode_btnLeaveGame]
    }
    else if (buttonThatWasClicked == btnKick)
    {
        //[UserButtonCode_btnKick] -- add your button handler code here..
		n02::kaillera::uiKickGameCallback();
		lstPlayers->deselectAllRows();
        //[/UserButtonCode_btnKick]
    }
    else if (buttonThatWasClicked == chkRecord)
    {
        //[UserButtonCode_chkRecord] -- add your button handler code here..
		n02::kaillera::recordingEnabled = chkRecord->getToggleState()? 1:0;
        //[/UserButtonCode_chkRecord]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void juceKailleraServerGame::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbRun)
    {
        //[UserComboBoxCode_cmbRun] -- add your combo box handling code here..
		n02::kaillera::selectedAutorunIndex = cmbRun->getSelectedId();
        //[/UserComboBoxCode_cmbRun]
    }
    else if (comboBoxThatHasChanged == cmbDelay)
    {
        //[UserComboBoxCode_cmbDelay] -- add your combo box handling code here..
		n02::kaillera::selectedDelayParam = cmbDelay->getSelectedId() -1;
        //[/UserComboBoxCode_cmbDelay]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void juceKailleraServerGame::appendText(String & text) {
	txtChat->setHighlightedRegion(textLength, 0);
	txtChat->insertTextAtCursor (text);
	textLength += text.length();
}

void juceKailleraServerGame::updateAutorunItems() {
	n02::setupAutorunUIItems(n02::kaillera::activeGameCaps, cmbRun, cmbDelay, lblRun, lblDelay);
	n02::kaillera::selectedAutorunIndex = cmbRun->getSelectedId();
	n02::kaillera::selectedDelayParam = cmbDelay->getSelectedId() - 1;
}

void juceKailleraServerGame::clearText() {
	textLength = 0;
	txtChat->setText("", false);
	btnStartGame->setEnabled(n02::kaillera::hosting);
	btnKick->setEnabled(n02::kaillera::hosting);
	updateAutorunItems();
}
void juceKailleraServerGame::handleCommandMessage(int  commandId) {
	TRACE(); n02::kaillera::KailleraListsCommand * cmd = reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId);
	TRACE(); int last = cmd->command;	

	if (last <= LISTCMD_LISTSLIMIT) {
		TRACE();
		n02::kaillera::processCommand(reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId));
		TRACE();
		if (lstPlayers != 0) {
			lstPlayers->updateContent();
			lstPlayers->repaint();
		}
		TRACE();
	} else {
		TRACE();
		if (last == LISTCMD_APPEND) {
			String * s = reinterpret_cast<String*>(reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId)->body.user);
			if (this != 0 && txtChat != 0) {
				TRACE();
				s->append(T("\r"), 2);
				txtChat->setHighlightedRegion(textLength, 0);
				txtChat->insertTextAtCursor (*s);
				textLength += s->length();
			}
			delete s;
		} else if (last == LISTCMD_SHOWGAME) {
			clearText();
		}
	}
	TRACE();
	delete cmd;
	TRACE();
}

void juceKailleraServerGame::sendCommand(n02::kaillera::KailleraListsCommand * cmd) {
	postCommandMessage(reinterpret_cast<int>(cmd));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceKailleraServerGame" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="650" initialHeight="250">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="new text editor" id="876eff7511524f9e" memberName="txtChat"
              virtualName="" explicitFocusOrder="0" pos="8 8 328 208" tooltip="chat messages are displayed here"
              initialText="" multiline="1" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="7bf7b60c15eeb8b" memberName="txtChatInput"
              virtualName="" explicitFocusOrder="0" pos="8 216 328 24" tooltip="insert chat input in this box and press return to send chat messages"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <GENERICCOMPONENT name="new component" id="f9436104229f377e" memberName="lstPlayers"
                    virtualName="" explicitFocusOrder="0" pos="344 8 296 176" class="TableListBox"
                    params="T(&quot;playersList&quot;), &amp;kailleraPlayers"/>
  <TEXTBUTTON name="new button" id="83beae5f7ea8f048" memberName="btnStartGame"
              virtualName="" explicitFocusOrder="0" pos="344 184 56 24" tooltip="to start the game"
              buttonText="start" connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="4d67e87e8755e8dc" memberName="btnLeaveGame"
              virtualName="" explicitFocusOrder="0" pos="400 184 56 24" tooltip="to leave the game"
              buttonText="leave" connectedEdges="7" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="90d5c5b1e0a0b2e2" memberName="btnKick"
              virtualName="" explicitFocusOrder="0" pos="456 184 64 24" tooltip="to kick someone out of the game"
              buttonText="kick" connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="19e29820f9b5e74b" memberName="cmbRun"
            virtualName="" explicitFocusOrder="0" pos="392 216 104 24" tooltip="switch between different running modes"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="6e270a333381e674" memberName="lblRun" virtualName=""
         explicitFocusOrder="0" pos="344 216 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="ac49a7cfb8110296" memberName="cmbDelay"
            virtualName="" explicitFocusOrder="0" pos="552 216 88 24" tooltip="select the additional delay parameter for the selected running mode"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="2c62b0ee329fcd39" memberName="lblDelay"
         virtualName="" explicitFocusOrder="0" pos="504 216 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Delay:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="7868d4f1fc987ede" memberName="chkRecord"
                virtualName="" explicitFocusOrder="0" pos="528 184 112 24" tooltip="check this to enable recording"
                buttonText="record game" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
