/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  17 Aug 2008 6:51:13 pm

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

    g.setColour (Colour (0x33000000));
    g.fillPath (internalPath1);

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
    internalPath1.clear();
    internalPath1.startNewSubPath (169.0f, 302.0f);
    internalPath1.quadraticTo (169.0f, 315.0f, 154.0f, 324.0f);
    internalPath1.quadraticTo (140.0f, 332.0f, 126.0f, 332.0f);
    internalPath1.quadraticTo (97.0f, 332.0f, 40.0f, 298.0f);
    internalPath1.quadraticTo (44.0f, 297.0f, 52.0f, 293.0f);
    internalPath1.quadraticTo (47.0f, 278.0f, 39.0f, 278.0f);
    internalPath1.quadraticTo (32.0f, 278.0f, 31.0f, 283.0f);
    internalPath1.quadraticTo (31.0f, 288.0f, 31.0f, 293.0f);
    internalPath1.quadraticTo (17.0f, 286.0f, 9.0f, 286.0f);
    internalPath1.quadraticTo (2.0f, 286.0f, (float) (-9), 289.0f);
    internalPath1.quadraticTo ((float) (-23), 293.0f, (float) (-27), 299.0f);
    internalPath1.lineTo ((float) (-31), 299.0f);
    internalPath1.lineTo ((float) (-31), 296.0f);
    internalPath1.quadraticTo ((float) (-31), 292.0f, (float) (-5), 274.0f);
    internalPath1.quadraticTo (2.0f, 270.0f, 9.0f, 265.0f);
    internalPath1.quadraticTo (18.0f, 260.0f, 20.0f, 253.0f);
    internalPath1.quadraticTo (21.0f, 249.0f, 21.0f, 245.0f);
    internalPath1.lineTo (21.0f, 212.0f);
    internalPath1.quadraticTo (19.0f, 211.0f, 6.0f, 211.0f);
    internalPath1.quadraticTo ((float) (-7), 210.0f, (float) (-12), 206.0f);
    internalPath1.quadraticTo ((float) (-18), 202.0f, (float) (-18), 194.0f);
    internalPath1.quadraticTo ((float) (-18), 182.0f, (float) (-5), 170.0f);
    internalPath1.quadraticTo (0.0f, 166.0f, 23.0f, 151.0f);
    internalPath1.quadraticTo (23.0f, 149.0f, 23.0f, 139.0f);
    internalPath1.quadraticTo (23.0f, 120.0f, 20.0f, 114.0f);
    internalPath1.quadraticTo (18.0f, 108.0f, 14.0f, 106.0f);
    internalPath1.quadraticTo (10.0f, 106.0f, 5.0f, 102.0f);
    internalPath1.quadraticTo (42.0f, 85.0f, 65.0f, 59.0f);
    internalPath1.quadraticTo (66.0f, 58.0f, 63.0f, 58.0f);
    internalPath1.quadraticTo (64.0f, 58.0f, 57.0f, 60.0f);
    internalPath1.quadraticTo (41.0f, 66.0f, 24.0f, 72.0f);
    internalPath1.quadraticTo (3.0f, 79.0f, (float) (-10), 79.0f);
    internalPath1.quadraticTo ((float) (-19), 79.0f, (float) (-27), 75.0f);
    internalPath1.quadraticTo ((float) (-37), 70.0f, (float) (-37), 60.0f);
    internalPath1.quadraticTo ((float) (-37), 52.0f, (float) (-29), 47.0f);
    internalPath1.quadraticTo ((float) (-21), 41.0f, (float) (-13), 41.0f);
    internalPath1.quadraticTo (2.0f, 41.0f, 2.0f, 52.0f);
    internalPath1.quadraticTo (2.0f, 55.0f, 1.0f, 59.0f);
    internalPath1.lineTo (1.0f, 60.0f);
    internalPath1.quadraticTo (19.0f, 58.0f, 45.0f, 35.0f);
    internalPath1.quadraticTo (67.0f, 14.0f, 89.0f, (float) (-8));
    internalPath1.quadraticTo (93.0f, (float) (-8), 95.0f, (float) (-2));
    internalPath1.quadraticTo (97.0f, 4.0f, 98.0f, 10.0f);
    internalPath1.quadraticTo (102.0f, 17.0f, 111.0f, 22.0f);
    internalPath1.quadraticTo (120.0f, 28.0f, 127.0f, 28.0f);
    internalPath1.quadraticTo (135.0f, 28.0f, 141.0f, 18.0f);
    internalPath1.quadraticTo (159.0f, 24.0f, 159.0f, 65.0f);
    internalPath1.quadraticTo (159.0f, 91.0f, 150.0f, 110.0f);
    internalPath1.quadraticTo (143.0f, 127.0f, 119.0f, 156.0f);
    internalPath1.quadraticTo (141.0f, 181.0f, 141.0f, 194.0f);
    internalPath1.quadraticTo (141.0f, 207.0f, 132.0f, 219.0f);
    internalPath1.quadraticTo (122.0f, 230.0f, 109.0f, 230.0f);
    internalPath1.quadraticTo (101.0f, 230.0f, 93.0f, 225.0f);
    internalPath1.quadraticTo (84.0f, 219.0f, 84.0f, 210.0f);
    internalPath1.quadraticTo (84.0f, 206.0f, 88.0f, 202.0f);
    internalPath1.quadraticTo (92.0f, 198.0f, 95.0f, 198.0f);
    internalPath1.quadraticTo (96.0f, 198.0f, 99.0f, 199.0f);
    internalPath1.quadraticTo (101.0f, 200.0f, 102.0f, 200.0f);
    internalPath1.quadraticTo (109.0f, 200.0f, 110.0f, 187.0f);
    internalPath1.quadraticTo (110.0f, 185.0f, 108.0f, 180.0f);
    internalPath1.quadraticTo (106.0f, 175.0f, 104.0f, 175.0f);
    internalPath1.quadraticTo (103.0f, 175.0f, 94.0f, 182.0f);
    internalPath1.quadraticTo (82.0f, 191.0f, 78.0f, 193.0f);
    internalPath1.quadraticTo (72.0f, 195.0f, 62.0f, 201.0f);
    internalPath1.quadraticTo (61.0f, 203.0f, 61.0f, 213.0f);
    internalPath1.quadraticTo (61.0f, 243.0f, 75.0f, 275.0f);
    internalPath1.quadraticTo (94.0f, 318.0f, 126.0f, 318.0f);
    internalPath1.quadraticTo (127.0f, 318.0f, 137.0f, 315.0f);
    internalPath1.quadraticTo (120.0f, 308.0f, 120.0f, 298.0f);
    internalPath1.quadraticTo (120.0f, 290.0f, 129.0f, 283.0f);
    internalPath1.quadraticTo (138.0f, 278.0f, 146.0f, 278.0f);
    internalPath1.quadraticTo (155.0f, 278.0f, 163.0f, 286.0f);
    internalPath1.quadraticTo (169.0f, 294.0f, 169.0f, 302.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (111.0f, 103.0f);
    internalPath1.quadraticTo (111.0f, 93.0f, 105.0f, 78.0f);
    internalPath1.quadraticTo (96.0f, 58.0f, 83.0f, 59.0f);
    internalPath1.lineTo (74.0f, 59.0f);
    internalPath1.quadraticTo (76.0f, 62.0f, 78.0f, 69.0f);
    internalPath1.quadraticTo (69.0f, 72.0f, 64.0f, 83.0f);
    internalPath1.quadraticTo (58.0f, 94.0f, 58.0f, 103.0f);
    internalPath1.quadraticTo (58.0f, 103.0f, 58.0f, 106.0f);
    internalPath1.quadraticTo (58.0f, 108.0f, 58.0f, 114.0f);
    internalPath1.quadraticTo (58.0f, 122.0f, 58.0f, 139.0f);
    internalPath1.lineTo (79.0f, 139.0f);
    internalPath1.quadraticTo (91.0f, 139.0f, 100.0f, 142.0f);
    internalPath1.quadraticTo (111.0f, 120.0f, 111.0f, 103.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (87.0f, 161.0f);
    internalPath1.quadraticTo (74.0f, 156.0f, 70.0f, 156.0f);
    internalPath1.lineTo (60.0f, 156.0f);
    internalPath1.lineTo (60.0f, 182.0f);
    internalPath1.quadraticTo (68.0f, 180.0f, 77.0f, 173.0f);
    internalPath1.quadraticTo (85.0f, 166.0f, 87.0f, 161.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (21.0f, 187.0f);
    internalPath1.quadraticTo (21.0f, 186.0f, 21.0f, 180.0f);
    internalPath1.quadraticTo (21.0f, 175.0f, 21.0f, 170.0f);
    internalPath1.quadraticTo (13.0f, 176.0f, 13.0f, 182.0f);
    internalPath1.quadraticTo (13.0f, 185.0f, 15.0f, 187.0f);
    internalPath1.closeSubPath();

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
        n02::p2p::port = common_min(common_max(txtPort->getText().getIntValue(), 0), 655634);
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
  <BACKGROUND backgroundColour="ffffffff">
    <PATH pos="0 0 100 100" fill="solid: 33000000" hasStroke="0" nonZeroWinding="1">s 168.7 301.84 q 168.7 314.61 153.67 324.28 q 140.23 332.39 126.41 332.39 q 97.12 332.39 39.77 298.36 q 44.24 296.81 51.58 292.95 q 46.69 277.85 38.55 277.85 q 32.04 277.85 30.83 283.28 q 30.83 287.91 30.83 292.56 q 16.99 285.59 9.26 285.59 q 1.54 285.59 -9.45 289.08 q -23.27 293.32 -26.52 299.12 l -30.59 299.12 l -30.59 295.64 q -31 291.78 -4.56 274.37 q 2.36 269.74 9.26 265.48 q 18.22 259.68 20.25 253.1 q 21.47 249.23 21.47 245.36 l 21.47 212.49 q 18.63 210.94 5.8 210.55 q -7.01 210.16 -12.29 206.11 q -17.58 202.04 -17.58 193.54 q -17.58 181.54 -4.56 169.94 q 0.33 165.68 22.7 150.6 q 23.09 149.04 23.09 139 q 23.09 120.42 20.25 113.84 q 17.81 108.43 13.74 106.49 q 10.49 105.73 5.2 101.86 q 41.8 85.22 64.99 58.92 q 65.81 58.14 62.96 58.14 q 63.78 58.14 57.27 60.08 q 41 65.88 24.32 71.68 q 2.75 78.65 -9.84 78.65 q -18.8 78.65 -26.52 75.16 q -36.69 70.14 -36.69 60.47 q -36.69 52.34 -28.77 46.73 q -20.83 41.13 -13.11 41.13 q 2.36 41.13 2.36 51.95 q 2.36 55.06 1.13 58.92 l 1.13 59.69 q 19.02 57.75 45.06 34.94 q 67.02 13.66 88.98 -7.61 q 92.66 -8 94.69 -2.01 q 96.72 3.99 98.35 9.8 q 102.01 16.75 111.16 22.18 q 120.31 27.59 126.82 27.59 q 135.36 27.59 140.64 17.53 q 159.36 23.72 159.36 65.49 q 159.36 91.02 150.4 109.97 q 142.68 126.61 118.68 155.62 q 141.46 180.76 141.46 193.54 q 141.46 207.45 131.9 218.68 q 122.34 229.89 108.52 229.89 q 100.79 229.89 93.05 224.87 q 84.11 219.07 84.11 210.16 q 84.11 206.29 88.18 202.24 q 92.25 198.17 94.69 198.17 q 95.9 198.17 98.55 199.34 q 101.19 200.49 102.42 200.49 q 108.93 200.49 110.14 187.34 q 110.14 185.02 108.11 180.19 q 106.07 175.35 104.45 175.35 q 103.22 175.35 93.87 182.31 q 82.08 191.21 78.41 193.15 q 72.31 195.47 62.14 201.27 q 60.92 202.82 60.92 212.88 q 60.92 242.65 74.75 274.76 q 93.87 317.7 126.41 317.7 q 127.23 317.7 136.58 314.61 q 119.9 307.64 119.9 298.36 q 119.9 289.84 129.26 283.28 q 137.79 277.85 146.33 277.85 q 155.29 277.85 162.6 285.98 q 168.7 293.71 168.7 301.84 x s 111.35 102.62 q 111.35 92.96 104.86 77.87 q 95.9 58.14 83.29 58.53 l 74.34 58.53 q 75.98 62.01 78.01 68.98 q 69.47 71.68 63.56 82.71 q 57.68 93.73 57.68 103.4 q 57.68 103.4 58.07 105.73 q 58.48 108.04 58.48 113.84 q 58.48 122.35 58.07 139 l 78.82 139 q 91.02 139 100.38 142.47 q 111.35 119.66 111.35 102.62 x s 87.36 161.03 q 74.34 156.01 69.88 156.01 l 60.1 156.01 l 60.1 182.31 q 67.84 180.37 76.59 173.23 q 85.33 166.07 87.36 161.03 x s 21.47 186.56 q 21.47 185.8 21.47 180.37 q 21.47 174.96 21.47 169.94 q 13.33 176.13 13.33 181.54 q 13.33 185.41 14.95 186.56 x</PATH>
  </BACKGROUND>
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
