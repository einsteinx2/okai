/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Aug 2008 9:16:09 pm

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

#include "jucePlayerPlayer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace n02 {
	namespace playback {
		extern double progress;
	};
};
//[/MiscUserDefs]

//==============================================================================
jucePlayerPlayer::jucePlayerPlayer ()
    : pbProgress (0),
      txtStatus (0)
{
    addAndMakeVisible (pbProgress = new ProgressBar (n02::playback::progress));
    pbProgress->setName (T("new component"));

    addAndMakeVisible (txtStatus = new TextEditor (T("new text editor")));
    txtStatus->setMultiLine (true);
    txtStatus->setReturnKeyStartsNewLine (false);
    txtStatus->setReadOnly (true);
    txtStatus->setScrollbarsShown (true);
    txtStatus->setCaretVisible (false);
    txtStatus->setPopupMenuEnabled (false);
    txtStatus->setText (String::empty);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (424, 176);

    //[Constructor] You can add your own custom stuff here..
	textLength = 0;
	pbProgress->setPercentageDisplay(true);
    //[/Constructor]
}

jucePlayerPlayer::~jucePlayerPlayer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (pbProgress);
    deleteAndZero (txtStatus);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void jucePlayerPlayer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void jucePlayerPlayer::resized()
{
    pbProgress->setBounds (8, 8, 408, 16);
    txtStatus->setBounds (8, 24, 408, 144);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void jucePlayerPlayer::handleCommandMessage (int commandId)
{
    //[UserCode_handleCommandMessage] -- Add your code here...
	if (commandId != 0) {
		String * s = reinterpret_cast<String*>(commandId);
		if (this != 0 && txtStatus != 0) {
			//pbProgress->setTextToDisplay(*s);
			s->append(T("\r"), 1);
			txtStatus->setHighlightedRegion(textLength, 0);
			txtStatus->insertTextAtCursor (*s);
			textLength += s->length();
		}
		delete s;
	}
    //[/UserCode_handleCommandMessage]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="jucePlayerPlayer" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="424" initialHeight="176">
  <METHODS>
    <METHOD name="handleCommandMessage (int commandId)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="new component" id="f67e360069c59d80" memberName="pbProgress"
                    virtualName="" explicitFocusOrder="0" pos="8 8 408 16" class="ProgressBar"
                    params="n02::playback::progress"/>
  <TEXTEDITOR name="new text editor" id="2e3261ec65fcba78" memberName="txtStatus"
              virtualName="" explicitFocusOrder="0" pos="8 24 408 144" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
