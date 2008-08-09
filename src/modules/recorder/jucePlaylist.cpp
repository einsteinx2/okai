/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Aug 2008 5:15:33 am

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
#include "module.h"

namespace n02 {
    namespace playback {
		void uiModChangeCallback(int index);
	};
};

//[/Headers]

#include "jucePlaylist.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
jucePlaylist::jucePlaylist ()
    : lstPlayback (0),
      btnBrowse (0),
      cmbModes (0),
      btnPlay (0),
      btnSave (0),
      btnRefresh (0),
      btnDelete (0)
{
    addAndMakeVisible (lstPlayback = new TableListBox (T("playlist"), &playListBox));
    lstPlayback->setName (T("new component"));

    addAndMakeVisible (btnBrowse = new TextButton (T("new button")));
    btnBrowse->setButtonText (T("browse"));
    btnBrowse->setConnectedEdges (Button::ConnectedOnRight);
    btnBrowse->addButtonListener (this);
    btnBrowse->setColour (TextButton::buttonColourId, Colour (0xffbbffbc));

    addAndMakeVisible (cmbModes = new ComboBox (T("new combo box")));
    cmbModes->setEditableText (false);
    cmbModes->setJustificationType (Justification::centredLeft);
    cmbModes->setTextWhenNothingSelected (T("Switch"));
    cmbModes->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbModes->addListener (this);

    addAndMakeVisible (btnPlay = new TextButton (T("new button")));
    btnPlay->setButtonText (T("play"));
    btnPlay->setConnectedEdges (Button::ConnectedOnRight);
    btnPlay->addButtonListener (this);
    btnPlay->setColour (TextButton::buttonColourId, Colour (0xffbbffbc));

    addAndMakeVisible (btnSave = new TextButton (T("new button")));
    btnSave->setButtonText (T("save"));
    btnSave->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnSave->addButtonListener (this);

    addAndMakeVisible (btnRefresh = new TextButton (T("new button")));
    btnRefresh->setButtonText (T("refresh"));
    btnRefresh->setConnectedEdges (Button::ConnectedOnLeft);
    btnRefresh->addButtonListener (this);

    addAndMakeVisible (btnDelete = new TextButton (T("new button")));
    btnDelete->setButtonText (T("delete"));
    btnDelete->setConnectedEdges (Button::ConnectedOnLeft);
    btnDelete->addButtonListener (this);
    btnDelete->setColour (TextButton::buttonColourId, Colour (0xffffbbbb));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (700, 400);

    //[Constructor] You can add your own custom stuff here..
	{ // modes
		ModuleAbstraction02 * mod = n02::modHelper.modHandler->find(MTYPE02_TRANSPORT);
		if (mod != 0) {
			do {
				cmbModes->addItem(mod->name, mod->index);
			} while ((mod = n02::modHelper.modHandler->findNext(mod))!=0);
		}
	}

	lstPlayback->getHeader()->addColumn("Game", 1, 250, 30, -1, TableHeaderComponent::notSortable);
	lstPlayback->getHeader()->addColumn("App/ver", 2, 120, 30, -1, TableHeaderComponent::notSortable);
	lstPlayback->getHeader()->addColumn("Time", 3, 80, 30, -1, TableHeaderComponent::notSortable);
	lstPlayback->getHeader()->addColumn("Size", 4, 60, 30, -1, TableHeaderComponent::notSortable);
	lstPlayback->getHeader()->addColumn("Comments", 5, 100, 30, -1, TableHeaderComponent::notSortable);
	lstPlayback->getHeader()->addColumn("File", 6, 30, 30, -1, TableHeaderComponent::notSortable);

	lstPlayback->setColour (TableListBox::outlineColourId, Colours::grey);
	lstPlayback->setOutlineThickness (1);

	playListBox.RePopulateList();

    //[/Constructor]
}

jucePlaylist::~jucePlaylist()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (lstPlayback);
    deleteAndZero (btnBrowse);
    deleteAndZero (cmbModes);
    deleteAndZero (btnPlay);
    deleteAndZero (btnSave);
    deleteAndZero (btnRefresh);
    deleteAndZero (btnDelete);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void jucePlaylist::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void jucePlaylist::resized()
{
    lstPlayback->setBounds (8, 40, getWidth() - 16, getHeight() - 48);
    btnBrowse->setBounds (getWidth() - 256, 8, 72, 24);
    cmbModes->setBounds (getWidth() - 104, 8, 96, 24);
    btnPlay->setBounds (8, 8, 72, 24);
    btnSave->setBounds (80, 8, 64, 24);
    btnRefresh->setBounds (getWidth() - 184, 8, 72, 24);
    btnDelete->setBounds (144, 8, 72, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void jucePlaylist::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnBrowse)
    {
        //[UserButtonCode_btnBrowse] -- add your button handler code here..
		playListBox.playBrowse();
        //[/UserButtonCode_btnBrowse]
    }
    else if (buttonThatWasClicked == btnPlay)
    {
        //[UserButtonCode_btnPlay] -- add your button handler code here..
		playListBox.playSelected(lstPlayback->getSelectedRow());
        //[/UserButtonCode_btnPlay]
    }
    else if (buttonThatWasClicked == btnSave)
    {
        //[UserButtonCode_btnSave] -- add your button handler code here..
		playListBox.saveSelected(lstPlayback->getSelectedRow());
        //[/UserButtonCode_btnSave]
    }
    else if (buttonThatWasClicked == btnRefresh)
    {
        //[UserButtonCode_btnRefresh] -- add your button handler code here..
		playListBox.RePopulateList();
		lstPlayback->updateContent();
		lstPlayback->repaint();
        //[/UserButtonCode_btnRefresh]
    }
    else if (buttonThatWasClicked == btnDelete)
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
		playListBox.deleteKeyPressed(lstPlayback->getSelectedRow());
        //[/UserButtonCode_btnDelete]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void jucePlaylist::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbModes)
    {
        //[UserComboBoxCode_cmbModes] -- add your combo box handling code here..
		n02::playback::uiModChangeCallback(cmbModes->getSelectedId());
        //[/UserComboBoxCode_cmbModes]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void jucePlaylist::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
	switch (commandId) {
		case 0:
			lstPlayback->updateContent();
			lstPlayback->repaint();
			break;
	};
    //[/UserCode_handleCommandMessage]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="jucePlaylist" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="700" initialHeight="400">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="new component" id="37fd0e5ba034bdba" memberName="lstPlayback"
                    virtualName="" explicitFocusOrder="0" pos="8 40 16M 48M" class="TableListBox"
                    params="T(&quot;playlist&quot;), &amp;playListBox"/>
  <TEXTBUTTON name="new button" id="21bfb622402f4660" memberName="btnBrowse"
              virtualName="" explicitFocusOrder="0" pos="256R 8 72 24" bgColOff="ffbbffbc"
              buttonText="browse" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="c981121cda8f3409" memberName="cmbModes"
            virtualName="" explicitFocusOrder="0" pos="104R 8 96 24" editable="0"
            layout="33" items="" textWhenNonSelected="Switch" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="78507ad39a518560" memberName="btnPlay"
              virtualName="" explicitFocusOrder="0" pos="8 8 72 24" bgColOff="ffbbffbc"
              buttonText="play" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b5f96880e9bc087e" memberName="btnSave"
              virtualName="" explicitFocusOrder="0" pos="80 8 64 24" buttonText="save"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="8dd00496c8431b9" memberName="btnRefresh"
              virtualName="" explicitFocusOrder="0" pos="184R 8 72 24" buttonText="refresh"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="aa921d71c1304508" memberName="btnDelete"
              virtualName="" explicitFocusOrder="0" pos="144 8 72 24" bgColOff="ffffbbbb"
              buttonText="delete" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
