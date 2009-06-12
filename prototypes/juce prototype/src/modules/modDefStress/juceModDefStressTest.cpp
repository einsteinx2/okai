/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  28 Jul 2008 10:19:43 pm

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
using namespace n02;
#include "locid.h"
//[/Headers]

#include "juceModDefStressTest.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace n02 {
	namespace defStressT {
		void uiStartGame(const char * gameName);
		void uiStopGame();
		void uiModChangeCallback(int index);
		extern int delay;
	};
};
//[/MiscUserDefs]

//==============================================================================
juceModDefStressTest::juceModDefStressTest ()
    : label (0),
      cmbGames (0),
      label2 (0),
      btnStart (0),
      sldDelay (0),
      label3 (0),
      btnStop (0),
      label4 (0),
      lblStatus (0),
      cmbModes (0)
{
    addAndMakeVisible (label = new Label (T("new label"),
                                          T("use this module to test the limits of your machine")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (cmbGames = new ComboBox (T("Games combo box")));
    cmbGames->setEditableText (false);
    cmbGames->setJustificationType (Justification::centredLeft);
    cmbGames->setTextWhenNothingSelected (T("select a game from here"));
    cmbGames->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbGames->addListener (this);

    addAndMakeVisible (label2 = new Label (T("new label"),
                                           T("Select a game from the list to start the game:")));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (btnStart = new TextButton (T("new button")));
    btnStart->setButtonText (T("Start"));
    btnStart->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnStart->addButtonListener (this);

    addAndMakeVisible (sldDelay = new Slider (T("new slider")));
    sldDelay->setRange (1, 64, 1);
    sldDelay->setSliderStyle (Slider::IncDecButtons);
    sldDelay->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sldDelay->addListener (this);

    addAndMakeVisible (label3 = new Label (T("new label"),
                                           T("Use this to vary delay:")));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (btnStop = new TextButton (T("new button")));
    btnStop->setButtonText (T("Stop"));
    btnStop->setConnectedEdges (Button::ConnectedOnLeft);
    btnStop->addButtonListener (this);

    addAndMakeVisible (label4 = new Label (T("new label"),
                                           T("framerate:")));
    label4->setFont (Font (15.0000f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (lblStatus = new Label (T("new label"),
                                              T("will appear here")));
    lblStatus->setFont (Font (15.0000f, Font::plain));
    lblStatus->setJustificationType (Justification::centredLeft);
    lblStatus->setEditable (false, false, false);
    lblStatus->setColour (TextEditor::textColourId, Colours::black);
    lblStatus->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (cmbModes = new ComboBox (T("new combo box")));
    cmbModes->setEditableText (false);
    cmbModes->setJustificationType (Justification::centredLeft);
    cmbModes->setTextWhenNothingSelected (T("Switch"));
    cmbModes->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbModes->addListener (this);


    //[UserPreSize]
	cmbModes->setTextWhenNothingSelected (n02::LUTF16(LID_SWIT));
    //[/UserPreSize]

    setSize (450, 145);

    //[Constructor] You can add your own custom stuff here..
	{ // games with herpes
		char * game = modHelper.gameList->start();
		int index = 0;
		if (game) {
			do {
				if (modHelper.gameList->getCaps(game) & GCAPS_STATES) {
					cmbGames->addItem(game, ++index);
				}
				//index++;
			} while ((game = modHelper.gameList->next())!=0);
		}
	}
	{ // modes
		ModuleAbstraction02 * mod = n02::modHelper.modHandler->find(MTYPE02_TRANSPORT);
		if (mod != 0) {
			do {
				cmbModes->addItem(mod->name, mod->index);
			} while ((mod = n02::modHelper.modHandler->findNext(mod))!=0);
		}
	}
    //[/Constructor]
}

juceModDefStressTest::~juceModDefStressTest()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (label);
    deleteAndZero (cmbGames);
    deleteAndZero (label2);
    deleteAndZero (btnStart);
    deleteAndZero (sldDelay);
    deleteAndZero (label3);
    deleteAndZero (btnStop);
    deleteAndZero (label4);
    deleteAndZero (lblStatus);
    deleteAndZero (cmbModes);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceModDefStressTest::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceModDefStressTest::resized()
{
    label->setBounds (8, 8, 320, 16);
    cmbGames->setBounds (16, 56, 256, 24);
    label2->setBounds (8, 32, 312, 16);
    btnStart->setBounds (272, 56, 80, 24);
    sldDelay->setBounds (16, 112, 134, 24);
    label3->setBounds (8, 96, 150, 16);
    btnStop->setBounds (352, 56, 80, 24);
    label4->setBounds (232, 96, 104, 16);
    lblStatus->setBounds (272, 112, 150, 24);
    cmbModes->setBounds (336, 8, 102, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceModDefStressTest::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbGames)
    {
        //[UserComboBoxCode_cmbGames] -- add your combo box handling code here..
        //[/UserComboBoxCode_cmbGames]
    }
    else if (comboBoxThatHasChanged == cmbModes)
    {
        //[UserComboBoxCode_cmbModes] -- add your combo box handling code here..
		n02::defStressT::uiModChangeCallback(cmbModes->getSelectedId());
        //[/UserComboBoxCode_cmbModes]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void juceModDefStressTest::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnStart)
    {
        //[UserButtonCode_btnStart] -- add your button handler code here..
		String game = cmbGames->getText();
		n02::defStressT::uiStartGame(game.toUTF8());
		sldDelay->setValue(1);
        //[/UserButtonCode_btnStart]
    }
    else if (buttonThatWasClicked == btnStop)
    {
        //[UserButtonCode_btnStop] -- add your button handler code here..
		n02::defStressT::uiStopGame();
		sldDelay->setValue(1);
        //[/UserButtonCode_btnStop]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void juceModDefStressTest::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sldDelay)
    {
        //[UserSliderCode_sldDelay] -- add your slider handling code here..
		n02::defStressT::delay = (int)sldDelay->getValue();
        //[/UserSliderCode_sldDelay]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void juceModDefStressTest::updateStatus(double fps) {
	juce::String status (fps);
	lblStatus->setText(status, false);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceModDefStressTest" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="450" initialHeight="145">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="new label" id="8acc7092c4b18498" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 320 16" edTextCol="ff000000"
         edBkgCol="0" labelText="use this module to test the limits of your machine"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Games combo box" id="dd2b4c386ade9c11" memberName="cmbGames"
            virtualName="" explicitFocusOrder="0" pos="16 56 256 24" editable="0"
            layout="33" items="" textWhenNonSelected="select a game from here"
            textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="7fa5d53316bb0c18" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 32 312 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Select a game from the list to start the game:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="4f0c403ebce50525" memberName="btnStart"
              virtualName="" explicitFocusOrder="0" pos="272 56 80 24" buttonText="Start"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="e62b7ae366ddbdc4" memberName="sldDelay"
          virtualName="" explicitFocusOrder="0" pos="16 112 134 24" min="1"
          max="64" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="d174a6e4712ffed1" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 96 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Use this to vary delay:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="6664fba5175af309" memberName="btnStop"
              virtualName="" explicitFocusOrder="0" pos="352 56 80 24" buttonText="Stop"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="d73fd9cb5ae1dbdd" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="232 96 104 16" edTextCol="ff000000"
         edBkgCol="0" labelText="framerate:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="2be3f51fcfe1bf89" memberName="lblStatus"
         virtualName="" explicitFocusOrder="0" pos="272 112 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="will appear here" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="e405d3d92a19e898" memberName="cmbModes"
            virtualName="" explicitFocusOrder="0" pos="336 8 102 24" editable="0"
            layout="33" items="" textWhenNonSelected="Switch" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
