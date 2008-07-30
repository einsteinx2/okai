/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  29 Jul 2008 10:15:29 am

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

#include "jucep2pSession.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
jucep2pSession::jucep2pSession ()
    : txtChat (0),
      txtChatInput (0),
      chkReady (0),
      cmbRun (0),
      lblRun (0),
      cmbDelay (0),
      lblDelay (0),
      chkRecord (0),
      cmbSmoothing (0),
      lblSmoothing (0),
      btnDisconnect (0),
      btnGetIP (0),
      btnDrop (0),
      btnChangeGame (0),
      btnCpr (0)
{
    addAndMakeVisible (txtChat = new TextEditor (T("new text editor")));
    txtChat->setMultiLine (true);
    txtChat->setReturnKeyStartsNewLine (false);
    txtChat->setReadOnly (true);
    txtChat->setScrollbarsShown (true);
    txtChat->setCaretVisible (false);
    txtChat->setPopupMenuEnabled (true);
    txtChat->setText (String::empty);

    addAndMakeVisible (txtChatInput = new TextEditor (T("new text editor")));
    txtChatInput->setMultiLine (false);
    txtChatInput->setReturnKeyStartsNewLine (false);
    txtChatInput->setReadOnly (false);
    txtChatInput->setScrollbarsShown (true);
    txtChatInput->setCaretVisible (true);
    txtChatInput->setPopupMenuEnabled (true);
    txtChatInput->setText (String::empty);

    addAndMakeVisible (chkReady = new ToggleButton (T("new toggle button")));
    chkReady->setButtonText (T("ready"));
    chkReady->addButtonListener (this);

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

    addAndMakeVisible (cmbSmoothing = new ComboBox (T("new combo box")));
    cmbSmoothing->setTooltip (T("switch between different smoothing for p2p"));
    cmbSmoothing->setEditableText (false);
    cmbSmoothing->setJustificationType (Justification::centredLeft);
    cmbSmoothing->setTextWhenNothingSelected (String::empty);
    cmbSmoothing->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbSmoothing->addListener (this);

    addAndMakeVisible (lblSmoothing = new Label (T("new label"),
                                                 T("Smoothing:")));
    lblSmoothing->setFont (Font (15.0000f, Font::plain));
    lblSmoothing->setJustificationType (Justification::centredLeft);
    lblSmoothing->setEditable (false, false, false);
    lblSmoothing->setColour (TextEditor::textColourId, Colours::black);
    lblSmoothing->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (btnDisconnect = new TextButton (T("new button")));
    btnDisconnect->setButtonText (T("disconnect"));
    btnDisconnect->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    btnDisconnect->addButtonListener (this);

    addAndMakeVisible (btnGetIP = new TextButton (T("new button")));
    btnGetIP->setButtonText (T("get ip/sid"));
    btnGetIP->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnBottom);
    btnGetIP->addButtonListener (this);

    addAndMakeVisible (btnDrop = new TextButton (T("new button")));
    btnDrop->setButtonText (T("drop"));
    btnDrop->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnBottom);
    btnDrop->addButtonListener (this);

    addAndMakeVisible (btnChangeGame = new TextButton (T("new button")));
    btnChangeGame->setButtonText (T("change game"));
    btnChangeGame->setConnectedEdges (Button::ConnectedOnTop | Button::ConnectedOnBottom);
    btnChangeGame->addButtonListener (this);

    addAndMakeVisible (btnCpr = new TextButton (T("new button")));
    btnCpr->setButtonText (T("cpr"));
    btnCpr->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    btnCpr->addButtonListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (576, 232);

    //[Constructor] You can add your own custom stuff here..

	updateAutorunItems();

	// recorder
	if (n02::modHelper.isRecorderLoaded()) {
		chkRecord->setToggleState(n02::p2p::recordingEnabled!=0, false);
	} else {
		chkRecord->setToggleState((n02::p2p::recordingEnabled=0)!=0, false);
		chkRecord->setEnabled(false);
	}

	for (int x = 0; x < 9; x++) {
		String xx;				
		xx.printf(T("+%i frames"), x);
		cmbSmoothing->addItem(xx, x+1);
	}

	cmbSmoothing->setSelectedId(1);
	n02::p2p::selectedSmoothing = 0;

	textLength = 0;

	txtChatInput->addListener(&chatInp);

    //[/Constructor]
}

jucep2pSession::~jucep2pSession()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (txtChat);
    deleteAndZero (txtChatInput);
    deleteAndZero (chkReady);
    deleteAndZero (cmbRun);
    deleteAndZero (lblRun);
    deleteAndZero (cmbDelay);
    deleteAndZero (lblDelay);
    deleteAndZero (chkRecord);
    deleteAndZero (cmbSmoothing);
    deleteAndZero (lblSmoothing);
    deleteAndZero (btnDisconnect);
    deleteAndZero (btnGetIP);
    deleteAndZero (btnDrop);
    deleteAndZero (btnChangeGame);
    deleteAndZero (btnCpr);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void jucep2pSession::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void jucep2pSession::resized()
{
    txtChat->setBounds (8, 8, 376, 192);
    txtChatInput->setBounds (8, 200, 376, 24);
    chkReady->setBounds (392, 8, 112, 24);
    cmbRun->setBounds (472, 168, 96, 24);
    lblRun->setBounds (424, 168, 48, 24);
    cmbDelay->setBounds (472, 200, 96, 24);
    lblDelay->setBounds (424, 200, 44, 24);
    chkRecord->setBounds (392, 32, 112, 24);
    cmbSmoothing->setBounds (472, 136, 96, 24);
    lblSmoothing->setBounds (392, 136, 76, 24);
    btnDisconnect->setBounds (392, 104, 88, 24);
    btnGetIP->setBounds (392, 56, 88, 24);
    btnDrop->setBounds (480, 56, 88, 24);
    btnChangeGame->setBounds (392, 80, 176, 24);
    btnCpr->setBounds (480, 104, 88, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void jucep2pSession::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == chkReady)
    {
        //[UserButtonCode_chkReady] -- add your button handler code here..
		n02::p2p::uiReadynessChange(chkReady->getToggleState());
        //[/UserButtonCode_chkReady]
    }
    else if (buttonThatWasClicked == chkRecord)
    {
        //[UserButtonCode_chkRecord] -- add your button handler code here..
		n02::p2p::recordingEnabled = chkRecord->getToggleState()? 1:0;
        //[/UserButtonCode_chkRecord]
    }
    else if (buttonThatWasClicked == btnDisconnect)
    {
        //[UserButtonCode_btnDisconnect] -- add your button handler code here..
		n02::p2p::uiDisconnect();
        //[/UserButtonCode_btnDisconnect]
    }
    else if (buttonThatWasClicked == btnGetIP)
    {
        //[UserButtonCode_btnGetIP] -- add your button handler code here..
		n02::p2p::uiGetIP();
        //[/UserButtonCode_btnGetIP]
    }
    else if (buttonThatWasClicked == btnDrop)
    {
        //[UserButtonCode_btnDrop] -- add your button handler code here..
		n02::p2p::uiDrop();
        //[/UserButtonCode_btnDrop]
    }
    else if (buttonThatWasClicked == btnChangeGame)
    {
        //[UserButtonCode_btnChangeGame] -- add your button handler code here..
		n02::p2p::uiChangeGame();
        //[/UserButtonCode_btnChangeGame]
    }
    else if (buttonThatWasClicked == btnCpr)
    {
        //[UserButtonCode_btnCpr] -- add your button handler code here..
		n02::p2p::uiCpr();
        //[/UserButtonCode_btnCpr]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void jucep2pSession::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbRun)
    {
        //[UserComboBoxCode_cmbRun] -- add your combo box handling code here..
		n02::p2p::selectedAutorunIndex = cmbRun->getSelectedId();
        //[/UserComboBoxCode_cmbRun]
    }
    else if (comboBoxThatHasChanged == cmbDelay)
    {
        //[UserComboBoxCode_cmbDelay] -- add your combo box handling code here..
		n02::p2p::selectedDelayParam = cmbDelay->getSelectedId() -1;
        //[/UserComboBoxCode_cmbDelay]
    }
    else if (comboBoxThatHasChanged == cmbSmoothing)
    {
        //[UserComboBoxCode_cmbSmoothing] -- add your combo box handling code here..
		n02::p2p::selectedSmoothing = cmbSmoothing->getSelectedId() -1;
        //[/UserComboBoxCode_cmbSmoothing]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void jucep2pSession::updateAutorunItems() {
	n02::setupAutorunUIItems(n02::p2p::activeGameCaps, cmbRun, cmbDelay, lblRun, lblDelay);
	n02::p2p::selectedAutorunIndex = cmbRun->getSelectedId();
	n02::p2p::selectedDelayParam = cmbDelay->getSelectedId() - 1;
}

void jucep2pSession::appendText(String * s) {
	if (txtChat != 0) {
		txtChat->setHighlightedRegion(textLength, 0);
		txtChat->insertTextAtCursor (*s);
		textLength += s->length();
	}
}

void jucep2pSession::P2PSessionMessageListener::handleMessage(const juce::Message &message) {
	if (isValidMessageListener())
	{
		register int param1 = message.intParameter1;
		register void * param2 = (void*)message.intParameter2;
		register int param3 = message.intParameter3;
		jucep2pSession * window = reinterpret_cast<jucep2pSession*>(message.pointerParameter);


		switch (param1) {
			case MSG_SET_READY:
				window->setReady(param2 != 0);
				break;
			case MSG_APPEND:
				{
					String * s = reinterpret_cast<String*>(param2);
					s->append(T("\r"), 1);
					window->appendText(s);
					delete s;
				}
				break;
			case MSG_CGLOCK:
				n02::p2p::uiChangeGameCallBack();
				break;
			case MSG_UPDATE_CAPS:
				n02::p2p::activeGameCaps = param3;
				window->updateAutorunItems();
				break;
		};
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="jucep2pSession" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="576" initialHeight="232">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="new text editor" id="992c0adfba0d2d86" memberName="txtChat"
              virtualName="" explicitFocusOrder="0" pos="8 8 376 192" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="770fa082f007fa8a" memberName="txtChatInput"
              virtualName="" explicitFocusOrder="0" pos="8 200 376 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="new toggle button" id="6ba9c783df525ee1" memberName="chkReady"
                virtualName="" explicitFocusOrder="0" pos="392 8 112 24" buttonText="ready"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="new combo box" id="19e29820f9b5e74b" memberName="cmbRun"
            virtualName="" explicitFocusOrder="0" pos="472 168 96 24" tooltip="switch between different running modes"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="6e270a333381e674" memberName="lblRun" virtualName=""
         explicitFocusOrder="0" pos="424 168 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="ac49a7cfb8110296" memberName="cmbDelay"
            virtualName="" explicitFocusOrder="0" pos="472 200 96 24" tooltip="select the additional delay parameter for the selected running mode"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="2c62b0ee329fcd39" memberName="lblDelay"
         virtualName="" explicitFocusOrder="0" pos="424 200 44 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Delay:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="7868d4f1fc987ede" memberName="chkRecord"
                virtualName="" explicitFocusOrder="0" pos="392 32 112 24" tooltip="check this to enable recording"
                buttonText="record game" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <COMBOBOX name="new combo box" id="274a3a9080097c74" memberName="cmbSmoothing"
            virtualName="" explicitFocusOrder="0" pos="472 136 96 24" tooltip="switch between different smoothing for p2p"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="9dbc5219384c8e35" memberName="lblSmoothing"
         virtualName="" explicitFocusOrder="0" pos="392 136 76 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Smoothing:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="d9940f03bd3fd6ce" memberName="btnDisconnect"
              virtualName="" explicitFocusOrder="0" pos="392 104 88 24" buttonText="disconnect"
              connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="df1198c91fad0b76" memberName="btnGetIP"
              virtualName="" explicitFocusOrder="0" pos="392 56 88 24" buttonText="get ip/sid"
              connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1ce606d2d8d52ff8" memberName="btnDrop"
              virtualName="" explicitFocusOrder="0" pos="480 56 88 24" buttonText="drop"
              connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="aaf89f1605b4a8d4" memberName="btnChangeGame"
              virtualName="" explicitFocusOrder="0" pos="392 80 176 24" buttonText="change game"
              connectedEdges="12" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7af7b9a0d08a50cf" memberName="btnCpr" virtualName=""
              explicitFocusOrder="0" pos="480 104 88 24" buttonText="cpr" connectedEdges="5"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
