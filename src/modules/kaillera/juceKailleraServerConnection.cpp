/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Jul 2008 5:18:42 pm

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
using namespace n02;
//[/Headers]

#include "juceKailleraServerConnection.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
juceKailleraServerConnection::juceKailleraServerConnection ()
    : lstUsers (0),
      txtChat (0),
      txtChatInput (0),
      btnNewGame (0),
      lstGames (0)
{
    addAndMakeVisible (lstUsers = new TableListBox (T("usersList"), &kailleraUsers));
    lstUsers->setName (T("new component"));

    addAndMakeVisible (txtChat = new TextEditor (T("new text editor")));
    txtChat->setTooltip (T("chat messages appear here"));
    txtChat->setMultiLine (true);
    txtChat->setReturnKeyStartsNewLine (false);
    txtChat->setReadOnly (true);
    txtChat->setScrollbarsShown (true);
    txtChat->setCaretVisible (true);
    txtChat->setPopupMenuEnabled (true);
    txtChat->setText (String::empty);

    addAndMakeVisible (txtChatInput = new TextEditor (T("new text editor")));
    txtChatInput->setTooltip (T("type here and press return to send chat messages"));
    txtChatInput->setMultiLine (false);
    txtChatInput->setReturnKeyStartsNewLine (false);
    txtChatInput->setReadOnly (false);
    txtChatInput->setScrollbarsShown (true);
    txtChatInput->setCaretVisible (true);
    txtChatInput->setPopupMenuEnabled (true);
    txtChatInput->setText (String::empty);

    addAndMakeVisible (btnNewGame = new TextButton (T("new button")));
    btnNewGame->setTooltip (T("create a game on the server"));
    btnNewGame->setButtonText (T("new game"));
    btnNewGame->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnNewGame->addButtonListener (this);

    addAndMakeVisible (lstGames = new TableListBox (T("gamesList"), &kailleraGames));
    lstGames->setName (T("new component"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (750, 500);

    //[Constructor] You can add your own custom stuff here..
	TRACE();

	// Users List
	lstUsers->getHeader()->addColumn("Nick", 1, 80, 30, -1, TableHeaderComponent::notSortable);
	lstUsers->getHeader()->addColumn("Ping", 2, 30, 30, -1, TableHeaderComponent::notSortable);
	lstUsers->getHeader()->addColumn("C.S.", 3, 30, 30, -1, TableHeaderComponent::notSortable);
	lstUsers->getHeader()->addColumn("Status", 4, 30, 30, -1, TableHeaderComponent::notSortable);

	lstUsers->setColour (TableListBox::outlineColourId, Colours::grey);
	lstUsers->setOutlineThickness (1);

	lstGames->getHeader()->addColumn("Game", 1, 320, 30, -1, TableHeaderComponent::notSortable);
	lstGames->getHeader()->addColumn("App", 2, 180, 30, -1, TableHeaderComponent::notSortable);
	lstGames->getHeader()->addColumn("Owner", 3, 110, 30, -1, TableHeaderComponent::notSortable);
	lstGames->getHeader()->addColumn("Status", 4, 55, 30, -1, TableHeaderComponent::notSortable);
	lstGames->getHeader()->addColumn("Players", 5, 50, 30, -1, TableHeaderComponent::notSortable);

	lstGames->setColour (TableListBox::outlineColourId, Colours::grey);
	lstGames->setOutlineThickness (1);

	txtChatInput->addListener(&chatInputListener);

	textLength = 0;

	TRACE();
    //[/Constructor]
}

juceKailleraServerConnection::~juceKailleraServerConnection()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	TRACE();
    //[/Destructor_pre]

    deleteAndZero (lstUsers);
    deleteAndZero (txtChat);
    deleteAndZero (txtChatInput);
    deleteAndZero (btnNewGame);
    deleteAndZero (lstGames);

    //[Destructor]. You can add your own custom destruction code here..
	TRACE();
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
    lstUsers->setBounds (getWidth() - 6 - 192, 8, 192, proportionOfHeight (0.6720f));
    txtChat->setBounds (8, 8, getWidth() - 206, (proportionOfHeight (0.6720f)) - 24);
    txtChatInput->setBounds (8, (8) + (proportionOfHeight (0.6720f)) - 24, getWidth() - 278, 24);
    btnNewGame->setBounds (getWidth() - 270, (8) + (proportionOfHeight (0.6720f)) - 24, 71, 24);
    lstGames->setBounds (8, (8) + (proportionOfHeight (0.6720f)), getWidth() - 14, proportionOfHeight (0.3040f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceKailleraServerConnection::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnNewGame)
    {
        //[UserButtonCode_btnNewGame] -- add your button handler code here..
		n02::kaillera::uiNewGameCallback();
        //[/UserButtonCode_btnNewGame]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void juceKailleraServerConnection::appendText(String & text) {
	txtChat->setHighlightedRegion(textLength, 0);
	txtChat->insertTextAtCursor (text);
	textLength += text.length();
}

void juceKailleraServerConnection::clearText() {
	textLength = 0;
	txtChat->setText("", false);
}
void juceKailleraServerConnection::handleCommandMessage(int  commandId) {
	//LOG(%i, commandId);
	TRACE(); n02::kaillera::KailleraListsCommand * cmd = reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId);
	TRACE(); int last = cmd->command;	

	if (last <= LISTCMD_LISTSLIMIT) {
		n02::kaillera::processCommand(reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId));
		TRACE();
		if (lstUsers != 0 && lstGames != 0) {
			if (last <= 3) {
				TRACE();
				lstUsers->updateContent();
				lstUsers->repaint();
			} else {
				TRACE();
				lstGames->updateContent();
				lstGames->repaint();
			}
		}
	} else {
		TRACE();
		if (last == LISTCMD_APPEND) {
			String * s = reinterpret_cast<String*>(reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId)->body.user);
			if (this != 0 && txtChat != 0) {
				TRACE();
				LOG(%i, s);
				txtChat->setHighlightedRegion(textLength, 0);
				txtChat->insertTextAtCursor (*s);
				textLength += s->length();
			}
			delete s;
		} else if (last == LISTCMD_SETTITLE) {
			TRACE();
			TCHAR * s = reinterpret_cast<TCHAR*>(reinterpret_cast<n02::kaillera::KailleraListsCommand*>(commandId)->body.user);
			String title;
			title << "Connected to " << s;
			n02::kaillera::uiSetTitleCallback(title);
		}
	}
	TRACE();
	delete cmd;
	TRACE();
}

void juceKailleraServerConnection::sendCommand(n02::kaillera::KailleraListsCommand * cmd) {
	postCommandMessage(reinterpret_cast<int>(cmd));
}

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
                    virtualName="" explicitFocusOrder="0" pos="6Rr 8 192 67.2%" class="TableListBox"
                    params="T(&quot;usersList&quot;), &amp;kailleraUsers"/>
  <TEXTEDITOR name="new text editor" id="84ef9fa8765d7afd" memberName="txtChat"
              virtualName="" explicitFocusOrder="0" pos="8 8 206M 24M" posRelativeH="8b78dd76054f6a1a"
              tooltip="chat messages appear here" initialText="" multiline="1"
              retKeyStartsLine="0" readonly="1" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="5d7f853814619b93" memberName="txtChatInput"
              virtualName="" explicitFocusOrder="0" pos="8 24R 278M 24" posRelativeY="8b78dd76054f6a1a"
              tooltip="type here and press return to send chat messages" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="ca7fb394f3815c91" memberName="btnNewGame"
              virtualName="" explicitFocusOrder="0" pos="270R 24R 71 24" posRelativeY="8b78dd76054f6a1a"
              tooltip="create a game on the server" buttonText="new game" connectedEdges="3"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="9340b1da010dd5f9" memberName="lstGames"
                    virtualName="" explicitFocusOrder="0" pos="8 0R 14M 30.4%" posRelativeY="8b78dd76054f6a1a"
                    class="TableListBox" params="T(&quot;gamesList&quot;), &amp;kailleraGames"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
