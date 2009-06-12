/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  2 Jul 2008 3:58:22 pm

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

#include "juceLicenseComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace n02 {
	void LicenseDeclined();
	void LicenseAccepted();
};
using namespace n02;
//[/MiscUserDefs]

//==============================================================================
juceLicenseComponent::juceLicenseComponent ()
    : Component (T("license")),
      txtLicense (0),
      btnAccept (0),
      btnDecline (0)
{
    addAndMakeVisible (txtLicense = new TextEditor (T("txtLicense")));
    txtLicense->setMultiLine (true);
    txtLicense->setReturnKeyStartsNewLine (false);
    txtLicense->setReadOnly (true);
    txtLicense->setScrollbarsShown (true);
    txtLicense->setCaretVisible (true);
    txtLicense->setPopupMenuEnabled (false);
    txtLicense->setText (T("Open Kaillera client: n02 License\n\nCopyright (c) Open Kaillera Team 2003-2008\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\nThe above copyright notice and this permission notice must be included in all copies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."));

    addAndMakeVisible (btnAccept = new TextButton (T("Accept button")));
    btnAccept->setButtonText (T("Accept"));
    btnAccept->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    btnAccept->addButtonListener (this);
    btnAccept->setColour (TextButton::buttonColourId, Colour (0xffadffb4));
    btnAccept->setColour (TextButton::buttonOnColourId, Colour (0xff47e14f));

    addAndMakeVisible (btnDecline = new TextButton (T("Decline button")));
    btnDecline->setButtonText (T("Decline"));
    btnDecline->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    btnDecline->addButtonListener (this);
    btnDecline->setColour (TextButton::buttonColourId, Colour (0xffffbbbb));
    btnDecline->setColour (TextButton::buttonOnColourId, Colour (0xffff4444));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 320);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

juceLicenseComponent::~juceLicenseComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (txtLicense);
    deleteAndZero (btnAccept);
    deleteAndZero (btnDecline);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceLicenseComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceLicenseComponent::resized()
{
    txtLicense->setBounds (8, 8, 584, 280);
    btnAccept->setBounds (360, 288, 120, 24);
    btnDecline->setBounds (480, 288, 110, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceLicenseComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnAccept)
    {
        //[UserButtonCode_btnAccept] -- add your button handler code here..
		LicenseAccepted();
        //[/UserButtonCode_btnAccept]
    }
    else if (buttonThatWasClicked == btnDecline)
    {
        //[UserButtonCode_btnDecline] -- add your button handler code here..
		LicenseDeclined();
        //[/UserButtonCode_btnDecline]
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

<JUCER_COMPONENT documentType="Component" className="juceLicenseComponent" componentName="license"
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="600" initialHeight="320">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="txtLicense" id="b6234a01bf7e8f45" memberName="txtLicense"
              virtualName="" explicitFocusOrder="0" pos="8 8 584 280" initialText="Open Kaillera client: n02 License&#10;&#10;Copyright (c) Open Kaillera Team 2003-2008&#10;&#10;Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the &quot;Software&quot;), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:&#10;&#10;The above copyright notice and this permission notice must be included in all copies or substantial portions of the Software.&#10;&#10;THE SOFTWARE IS PROVIDED &quot;AS IS&quot;, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="1" popupmenu="0"/>
  <TEXTBUTTON name="Accept button" id="d04e65980ca8fbb8" memberName="btnAccept"
              virtualName="" explicitFocusOrder="0" pos="360 288 120 24" bgColOff="ffadffb4"
              bgColOn="ff47e14f" buttonText="Accept" connectedEdges="6" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Decline button" id="49efe1ad8690158c" memberName="btnDecline"
              virtualName="" explicitFocusOrder="0" pos="480 288 110 24" bgColOff="ffffbbbb"
              bgColOn="ffff4444" buttonText="Decline" connectedEdges="5" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
