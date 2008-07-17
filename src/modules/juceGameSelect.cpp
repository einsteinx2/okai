/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Jul 2008 7:34:06 am

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

#include "juceGameSelect.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace n02 {
	namespace gamelistSel {
		extern int selectedIndex;
	};
}
//[/MiscUserDefs]

//==============================================================================
juceGameSelect::juceGameSelect ()
    : lstGames (0),
      btnOk (0),
      btnCancel (0),
      lblGameInfo (0)
{
    addAndMakeVisible (lstGames = new ListBox (T("gamelist"), &gameListModel));
    lstGames->setName (T("new component"));

    addAndMakeVisible (btnOk = new TextButton (T("new button")));
    btnOk->setButtonText (T("ok"));
    btnOk->setConnectedEdges (Button::ConnectedOnRight);
    btnOk->addButtonListener (this);
    btnOk->setColour (TextButton::buttonColourId, Colour (0xffcaffbb));

    addAndMakeVisible (btnCancel = new TextButton (T("new button")));
    btnCancel->setButtonText (T("cancel"));
    btnCancel->setConnectedEdges (Button::ConnectedOnLeft);
    btnCancel->addButtonListener (this);
    btnCancel->setColour (TextButton::buttonColourId, Colour (0xffffcebb));

    addAndMakeVisible (lblGameInfo = new Label (T("new label"),
                                                String::empty));
    lblGameInfo->setFont (Font (15.0000f, Font::plain));
    lblGameInfo->setJustificationType (Justification::centredLeft);
    lblGameInfo->setEditable (false, false, false);
    lblGameInfo->setColour (TextEditor::textColourId, Colours::black);
    lblGameInfo->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);

    //[Constructor] You can add your own custom stuff here..
	lstGames->setColour (TableListBox::outlineColourId, Colours::grey);
	lstGames->setOutlineThickness (1);
	lstGames->updateContent();
	if (n02::gamelistSel::selectedIndex >= 0)
		lstGames->scrollToEnsureRowIsOnscreen(n02::gamelistSel::selectedIndex);
	n02::gamelistSel::selectedIndex = -1;
    //[/Constructor]
}

juceGameSelect::~juceGameSelect()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (lstGames);
    deleteAndZero (btnOk);
    deleteAndZero (btnCancel);
    deleteAndZero (lblGameInfo);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceGameSelect::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceGameSelect::resized()
{
    lstGames->setBounds (16, 16, 568, 336);
    btnOk->setBounds (448, 360, 72, 24);
    btnCancel->setBounds (520, 360, 64, 24);
    lblGameInfo->setBounds (16, 360, 400, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceGameSelect::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnOk)
    {
        //[UserButtonCode_btnOk] -- add your button handler code here..
		if (lstGames->getNumSelectedRows() == 0) 
			n02::gamelistSel::selectedIndex = -1;
		getCurrentlyModalComponent()->exitModalState(1);
        //[/UserButtonCode_btnOk]
    }
    else if (buttonThatWasClicked == btnCancel)
    {
        //[UserButtonCode_btnCancel] -- add your button handler code here..
		getCurrentlyModalComponent()->exitModalState(0);
        //[/UserButtonCode_btnCancel]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void juceGameSelect::closeUp() {
	getCurrentlyModalComponent()->exitModalState(1);
}
void juceGameSelect::updateGameInfo(String & info) {
	lblGameInfo->setText(info,false);	
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceGameSelect" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="new component" id="a824b51affc5b10d" memberName="lstGames"
                    virtualName="" explicitFocusOrder="0" pos="16 16 568 336" class="ListBox"
                    params="T(&quot;gamelist&quot;), &amp;gameListModel"/>
  <TEXTBUTTON name="new button" id="5e11079d5a885e20" memberName="btnOk" virtualName=""
              explicitFocusOrder="0" pos="448 360 72 24" bgColOff="ffcaffbb"
              buttonText="ok" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="27fed7c0fb2584a1" memberName="btnCancel"
              virtualName="" explicitFocusOrder="0" pos="520 360 64 24" bgColOff="ffffcebb"
              buttonText="cancel" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="abfcb63fc5843c7" memberName="lblGameInfo"
         virtualName="" explicitFocusOrder="0" pos="16 360 400 24" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
