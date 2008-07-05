/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Jul 2008 5:29:20 pm

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
#include "common.h"
using namespace n02;
//[/Headers]

#include "juceKailleraServerSelect.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace n02 {
	namespace kaillera {
		void uiModChangeCallback(int index);
		void uibtnConnectCallback();
		void uibtnPingCallback();
		void uibtnMastersListCallback();
		void uiAddServer();
		void uiEditServer(int);
		void uiDeleteServer(int);
		extern TCHAR uiUsername[32];
		extern TCHAR uiQuitMessage[128];
		extern TCHAR uiLastIP[128];
		extern int uiConnectionSetting;
	};
};
using namespace n02::kaillera;
//[/MiscUserDefs]

//==============================================================================
juceKailleraServerSelect::juceKailleraServerSelect ()
    : cmbModes (0),
      lstServers (0),
      btnAdd (0),
      btnEdit (0),
      btnDelete (0),
      txtNick (0),
      label (0),
      label2 (0),
      cmbConnection (0),
      txtIP (0),
      label3 (0),
      label4 (0),
      btnConnect (0),
      btnPing (0),
      btnMastersList (0)
{
    addAndMakeVisible (cmbModes = new ComboBox (T("new combo box")));
    cmbModes->setEditableText (false);
    cmbModes->setJustificationType (Justification::centredLeft);
    cmbModes->setTextWhenNothingSelected (T("Switch"));
    cmbModes->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbModes->addListener (this);

    addAndMakeVisible (lstServers = new TableListBox (T("serversList"), &serversListModel));
    lstServers->setName (T("new component"));

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

    addAndMakeVisible (txtNick = new TextEditor (T("new text editor")));
    txtNick->setMultiLine (false);
    txtNick->setReturnKeyStartsNewLine (false);
    txtNick->setReadOnly (false);
    txtNick->setScrollbarsShown (true);
    txtNick->setCaretVisible (true);
    txtNick->setPopupMenuEnabled (true);
    txtNick->setColour (TextEditor::backgroundColourId, Colours::white);
    txtNick->setText (String::empty);

    addAndMakeVisible (label = new Label (T("new label"),
                                          T("Nick:")));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (T("new label"),
                                           T("C.S.:")));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (cmbConnection = new ComboBox (T("new combo box")));
    cmbConnection->setEditableText (false);
    cmbConnection->setJustificationType (Justification::centredLeft);
    cmbConnection->setTextWhenNothingSelected (String::empty);
    cmbConnection->setTextWhenNoChoicesAvailable (T("(no choices)"));
    cmbConnection->addItem (T("LAN (60 PPS)"), 1);
    cmbConnection->addItem (T("Excelent (30 PPS)"), 2);
    cmbConnection->addItem (T("Good (20 PPS)"), 3);
    cmbConnection->addItem (T("Average (15 PPS)"), 4);
    cmbConnection->addItem (T("Bad (12 PPS)"), 5);
    cmbConnection->addItem (T("Low (10 PPS)"), 6);
    cmbConnection->addListener (this);

    addAndMakeVisible (txtIP = new TextEditor (T("new text editor")));
    txtIP->setMultiLine (false);
    txtIP->setReturnKeyStartsNewLine (false);
    txtIP->setReadOnly (false);
    txtIP->setScrollbarsShown (true);
    txtIP->setCaretVisible (true);
    txtIP->setPopupMenuEnabled (true);
    txtIP->setColour (TextEditor::backgroundColourId, Colours::white);
    txtIP->setText (String::empty);

    addAndMakeVisible (label3 = new Label (T("new label"),
                                           T("IP:")));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label4 = new Label (T("new label"),
                                           T("Fovorites:")));
    label4->setFont (Font (15.0000f, Font::plain));
    label4->setJustificationType (Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (btnConnect = new TextButton (T("new button")));
    btnConnect->setButtonText (T("connect"));
    btnConnect->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnConnect->addButtonListener (this);
    btnConnect->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (btnPing = new TextButton (T("new button")));
    btnPing->setButtonText (T("ping"));
    btnPing->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btnPing->addButtonListener (this);
    btnPing->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (btnMastersList = new TextButton (T("new button")));
    btnMastersList->setButtonText (T("master list"));
    btnMastersList->setConnectedEdges (Button::ConnectedOnLeft);
    btnMastersList->addButtonListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (580, 330);

    //[Constructor] You can add your own custom stuff here..

	{ // modes
		ModuleAbstraction02 * mod = n02::modHelper.modHandler->find(MTYPE02_TRANSPORT);
		if (mod != 0) {
			do {
				cmbModes->addItem(mod->name, mod->index);
			} while ((mod = n02::modHelper.modHandler->findNext(mod))!=0);
		}
	}

	// connection setting
	uiConnectionSetting = max(min(uiConnectionSetting, 6), 1);
	cmbConnection->setSelectedId(uiConnectionSetting);

	txtIP->setText(uiLastIP);
	txtNick->setText(uiUsername);

	// List
	lstServers->getHeader()->addColumn("Server", 1, 482/2, 30, -1, TableHeaderComponent::notSortable);
	lstServers->getHeader()->addColumn("IP Address", 2, 482/2 - 20, 30, -1, TableHeaderComponent::notSortable);

	lstServers->setColour (TableListBox::outlineColourId, Colours::grey);
	lstServers->setOutlineThickness (1);

    //[/Constructor]
}

juceKailleraServerSelect::~juceKailleraServerSelect()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    //[/Destructor_pre]

    deleteAndZero (cmbModes);
    deleteAndZero (lstServers);
    deleteAndZero (btnAdd);
    deleteAndZero (btnEdit);
    deleteAndZero (btnDelete);
    deleteAndZero (txtNick);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (cmbConnection);
    deleteAndZero (txtIP);
    deleteAndZero (label3);
    deleteAndZero (label4);
    deleteAndZero (btnConnect);
    deleteAndZero (btnPing);
    deleteAndZero (btnMastersList);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void juceKailleraServerSelect::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colour (0x33000000));
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void juceKailleraServerSelect::resized()
{
    cmbModes->setBounds (448, 8, 120, 24);
    lstServers->setBounds (88, 112, 482, 208);
    btnAdd->setBounds (8, 248, 80, 24);
    btnEdit->setBounds (8, 272, 80, 24);
    btnDelete->setBounds (8, 296, 80, 24);
    txtNick->setBounds (88, 16, 150, 24);
    label->setBounds (8, 16, 72, 24);
    label2->setBounds (8, 48, 72, 24);
    cmbConnection->setBounds (88, 48, 150, 24);
    txtIP->setBounds (88, 80, 242, 24);
    label3->setBounds (8, 80, 72, 24);
    label4->setBounds (8, 112, 72, 24);
    btnConnect->setBounds (330, 80, 72, 24);
    btnPing->setBounds (400, 80, 72, 24);
    btnMastersList->setBounds (470, 80, 95, 24);
    internalPath1.clear();
    internalPath1.startNewSubPath (15.0f, 267.0f);
    internalPath1.quadraticTo (17.0f, 268.0f, 22.0f, 272.0f);
    internalPath1.quadraticTo (28.0f, 275.0f, 29.0f, 277.0f);
    internalPath1.quadraticTo (29.0f, 277.0f, 27.0f, 279.0f);
    internalPath1.quadraticTo (25.0f, 281.0f, 25.0f, 281.0f);
    internalPath1.quadraticTo ((float) (-2), 293.0f, (float) (-26), 293.0f);
    internalPath1.quadraticTo ((float) (-32), 293.0f, (float) (-40), 290.0f);
    internalPath1.quadraticTo ((float) (-51), 286.0f, (float) (-51), 280.0f);
    internalPath1.quadraticTo ((float) (-51), 272.0f, (float) (-37), 263.0f);
    internalPath1.quadraticTo ((float) (-25), 257.0f, (float) (-19), 256.0f);
    internalPath1.quadraticTo ((float) (-2), 252.0f, 5.0f, 252.0f);
    internalPath1.quadraticTo (6.0f, 252.0f, 16.0f, 253.0f);
    internalPath1.quadraticTo (26.0f, 254.0f, 27.0f, 254.0f);
    internalPath1.quadraticTo (32.0f, 254.0f, 36.0f, 246.0f);
    internalPath1.quadraticTo (39.0f, 241.0f, 43.0f, 229.0f);
    internalPath1.quadraticTo (46.0f, 212.0f, 46.0f, 204.0f);
    internalPath1.lineTo (46.0f, 165.0f);
    internalPath1.quadraticTo (43.0f, 163.0f, 37.0f, 163.0f);
    internalPath1.quadraticTo (30.0f, 164.0f, 29.0f, 163.0f);
    internalPath1.quadraticTo (20.0f, 162.0f, 12.0f, 155.0f);
    internalPath1.quadraticTo (4.0f, 149.0f, 4.0f, 141.0f);
    internalPath1.quadraticTo (4.0f, 127.0f, 20.0f, 116.0f);
    internalPath1.quadraticTo (25.0f, 113.0f, 45.0f, 104.0f);
    internalPath1.quadraticTo (45.0f, 91.0f, 44.0f, 83.0f);
    internalPath1.quadraticTo (44.0f, 77.0f, 44.0f, 74.0f);
    internalPath1.quadraticTo (44.0f, 72.0f, 44.0f, 72.0f);
    internalPath1.quadraticTo (28.0f, 76.0f, 17.0f, 76.0f);
    internalPath1.quadraticTo ((float) (-7), 76.0f, (float) (-7), 61.0f);
    internalPath1.quadraticTo ((float) (-7), 56.0f, (float) (-2), 52.0f);
    internalPath1.quadraticTo (3.0f, 48.0f, 8.0f, 48.0f);
    internalPath1.quadraticTo (17.0f, 48.0f, 18.0f, 54.0f);
    internalPath1.quadraticTo (19.0f, 60.0f, 20.0f, 60.0f);
    internalPath1.quadraticTo (28.0f, 60.0f, 41.0f, 49.0f);
    internalPath1.quadraticTo (52.0f, 39.0f, 56.0f, 31.0f);
    internalPath1.quadraticTo (60.0f, 24.0f, 62.0f, 15.0f);
    internalPath1.quadraticTo (65.0f, 16.0f, 67.0f, 22.0f);
    internalPath1.quadraticTo (71.0f, 35.0f, 71.0f, 56.0f);
    internalPath1.lineTo (71.0f, 100.0f);
    internalPath1.quadraticTo (71.0f, 101.0f, 73.0f, 104.0f);
    internalPath1.quadraticTo (79.0f, 105.0f, 93.0f, 109.0f);
    internalPath1.quadraticTo (94.0f, 108.0f, 95.0f, 106.0f);
    internalPath1.quadraticTo (99.0f, 93.0f, 99.0f, 80.0f);
    internalPath1.quadraticTo (99.0f, 72.0f, 97.0f, 64.0f);
    internalPath1.quadraticTo (96.0f, 61.0f, 93.0f, 57.0f);
    internalPath1.quadraticTo (90.0f, 53.0f, 90.0f, 52.0f);
    internalPath1.quadraticTo (90.0f, 49.0f, 99.0f, 43.0f);
    internalPath1.quadraticTo (107.0f, 37.0f, 109.0f, 37.0f);
    internalPath1.quadraticTo (110.0f, 37.0f, 113.0f, 40.0f);
    internalPath1.quadraticTo (117.0f, 44.0f, 118.0f, 44.0f);
    internalPath1.quadraticTo (122.0f, 44.0f, 126.0f, 34.0f);
    internalPath1.quadraticTo (125.0f, 33.0f, 121.0f, 31.0f);
    internalPath1.quadraticTo (121.0f, 30.0f, 121.0f, 29.0f);
    internalPath1.quadraticTo (126.0f, 24.0f, 145.0f, 17.0f);
    internalPath1.quadraticTo (167.0f, 8.0f, 177.0f, 8.0f);
    internalPath1.quadraticTo (189.0f, 8.0f, 189.0f, 14.0f);
    internalPath1.quadraticTo (189.0f, 20.0f, 185.0f, 20.0f);
    internalPath1.quadraticTo (184.0f, 20.0f, 184.0f, 19.0f);
    internalPath1.lineTo (184.0f, 19.0f);
    internalPath1.quadraticTo (184.0f, 19.0f, 183.0f, 19.0f);
    internalPath1.quadraticTo (178.0f, 19.0f, 170.0f, 23.0f);
    internalPath1.quadraticTo (164.0f, 27.0f, 161.0f, 29.0f);
    internalPath1.quadraticTo (156.0f, 33.0f, 148.0f, 43.0f);
    internalPath1.quadraticTo (141.0f, 51.0f, 126.0f, 81.0f);
    internalPath1.quadraticTo (124.0f, 85.0f, 106.0f, 117.0f);
    internalPath1.quadraticTo (116.0f, 126.0f, 127.0f, 136.0f);
    internalPath1.quadraticTo (140.0f, 146.0f, 151.0f, 146.0f);
    internalPath1.quadraticTo (146.0f, 149.0f, 144.0f, 160.0f);
    internalPath1.quadraticTo (142.0f, 169.0f, 142.0f, 176.0f);
    internalPath1.quadraticTo (142.0f, 209.0f, 161.0f, 249.0f);
    internalPath1.quadraticTo (181.0f, 274.0f, 197.0f, 287.0f);
    internalPath1.quadraticTo (197.0f, 287.0f, 198.0f, 288.0f);
    internalPath1.quadraticTo (220.0f, 302.0f, 241.0f, 304.0f);
    internalPath1.lineTo (241.0f, 309.0f);
    internalPath1.quadraticTo (238.0f, 310.0f, 231.0f, 310.0f);
    internalPath1.quadraticTo (203.0f, 310.0f, 178.0f, 292.0f);
    internalPath1.quadraticTo (174.0f, 290.0f, 142.0f, 260.0f);
    internalPath1.quadraticTo (138.0f, 252.0f, 133.0f, 245.0f);
    internalPath1.quadraticTo (124.0f, 231.0f, 123.0f, 231.0f);
    internalPath1.quadraticTo (122.0f, 231.0f, 118.0f, 232.0f);
    internalPath1.quadraticTo (115.0f, 233.0f, 113.0f, 233.0f);
    internalPath1.quadraticTo (105.0f, 233.0f, 99.0f, 228.0f);
    internalPath1.quadraticTo (92.0f, 222.0f, 93.0f, 210.0f);
    internalPath1.quadraticTo (93.0f, 209.0f, 95.0f, 208.0f);
    internalPath1.quadraticTo (108.0f, 207.0f, 108.0f, 181.0f);
    internalPath1.quadraticTo (108.0f, 170.0f, 104.0f, 156.0f);
    internalPath1.quadraticTo (100.0f, 139.0f, 95.0f, 136.0f);
    internalPath1.quadraticTo (82.0f, 147.0f, 73.0f, 153.0f);
    internalPath1.lineTo (73.0f, 181.0f);
    internalPath1.quadraticTo (73.0f, 198.0f, 76.0f, 217.0f);
    internalPath1.quadraticTo (77.0f, 223.0f, 81.0f, 227.0f);
    internalPath1.quadraticTo (86.0f, 230.0f, 91.0f, 230.0f);
    internalPath1.quadraticTo (90.0f, 235.0f, 82.0f, 242.0f);
    internalPath1.quadraticTo (74.0f, 248.0f, 67.0f, 254.0f);
    internalPath1.quadraticTo (67.0f, 254.0f, 66.0f, 249.0f);
    internalPath1.quadraticTo (65.0f, 246.0f, 61.0f, 246.0f);
    internalPath1.quadraticTo (53.0f, 246.0f, 53.0f, 253.0f);
    internalPath1.quadraticTo (53.0f, 255.0f, 58.0f, 260.0f);
    internalPath1.quadraticTo (57.0f, 262.0f, 51.0f, 266.0f);
    internalPath1.quadraticTo (55.0f, 270.0f, 61.0f, 278.0f);
    internalPath1.quadraticTo (65.0f, 287.0f, 65.0f, 295.0f);
    internalPath1.quadraticTo (65.0f, 322.0f, 44.0f, 322.0f);
    internalPath1.quadraticTo (36.0f, 322.0f, 31.0f, 318.0f);
    internalPath1.quadraticTo (24.0f, 314.0f, 25.0f, 310.0f);
    internalPath1.lineTo (25.0f, 304.0f);
    internalPath1.quadraticTo (25.0f, 303.0f, 27.0f, 303.0f);
    internalPath1.quadraticTo (28.0f, 303.0f, 30.0f, 305.0f);
    internalPath1.quadraticTo (32.0f, 307.0f, 33.0f, 307.0f);
    internalPath1.quadraticTo (41.0f, 307.0f, 41.0f, 295.0f);
    internalPath1.quadraticTo (41.0f, 289.0f, 32.0f, 279.0f);
    internalPath1.quadraticTo (22.0f, 269.0f, 13.0f, 266.0f);
    internalPath1.quadraticTo (3.0f, 263.0f, (float) (-4), 263.0f);
    internalPath1.quadraticTo ((float) (-19), 263.0f, (float) (-30), 270.0f);
    internalPath1.quadraticTo ((float) (-39), 275.0f, (float) (-39), 280.0f);
    internalPath1.quadraticTo ((float) (-39), 282.0f, (float) (-37), 282.0f);
    internalPath1.quadraticTo ((float) (-30), 285.0f, (float) (-27), 284.0f);
    internalPath1.quadraticTo ((float) (-6), 282.0f, 14.0f, 268.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (83.0f, 126.0f);
    internalPath1.quadraticTo (81.0f, 125.0f, 72.0f, 122.0f);
    internalPath1.lineTo (72.0f, 134.0f);
    internalPath1.lineTo (75.0f, 134.0f);
    internalPath1.quadraticTo (81.0f, 130.0f, 83.0f, 126.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (44.0f, 142.0f);
    internalPath1.quadraticTo (44.0f, 124.0f, 44.0f, 124.0f);
    internalPath1.quadraticTo (33.0f, 126.0f, 33.0f, 135.0f);
    internalPath1.quadraticTo (33.0f, 140.0f, 44.0f, 142.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void juceKailleraServerSelect::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cmbModes)
    {
        //[UserComboBoxCode_cmbModes] -- add your combo box handling code here..
		n02::kaillera::uiModChangeCallback(cmbModes->getSelectedId());
        //[/UserComboBoxCode_cmbModes]
    }
    else if (comboBoxThatHasChanged == cmbConnection)
    {
        //[UserComboBoxCode_cmbConnection] -- add your combo box handling code here..
		uiConnectionSetting = cmbConnection->getSelectedId();
        //[/UserComboBoxCode_cmbConnection]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void juceKailleraServerSelect::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnAdd)
    {
        //[UserButtonCode_btnAdd] -- add your button handler code here..
		uiAddServer();
		lstServers->updateContent();
        //[/UserButtonCode_btnAdd]
    }
    else if (buttonThatWasClicked == btnEdit)
    {
        //[UserButtonCode_btnEdit] -- add your button handler code here..
		uiEditServer(lstServers->getSelectedRow());
		lstServers->updateContent();
        //[/UserButtonCode_btnEdit]
    }
    else if (buttonThatWasClicked == btnDelete)
    {
        //[UserButtonCode_btnDelete] -- add your button handler code here..
		uiDeleteServer(lstServers->getSelectedRow());
		lstServers->updateContent();
        //[/UserButtonCode_btnDelete]
    }
    else if (buttonThatWasClicked == btnConnect)
    {
        //[UserButtonCode_btnConnect] -- add your button handler code here..
		saveConfig();
		uibtnConnectCallback();
        //[/UserButtonCode_btnConnect]
    }
    else if (buttonThatWasClicked == btnPing)
    {
        //[UserButtonCode_btnPing] -- add your button handler code here..
		saveConfig();
		uibtnPingCallback();
        //[/UserButtonCode_btnPing]
    }
    else if (buttonThatWasClicked == btnMastersList)
    {
        //[UserButtonCode_btnMastersList] -- add your button handler code here..
		uibtnMastersListCallback();
        //[/UserButtonCode_btnMastersList]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void juceKailleraServerSelect::saveConfig() {
#ifdef UNICODE
	txtIP->getText().copyToBuffer(uiLastIP, 128);
	txtNick->getText().copyToBuffer(uiUsername, 32);
#else
	txtIP->getText().copyToUTF8(uiLastIP);
	txtNick->getText().copyToBuffer(uiUsername, 32);
#endif
}
void juceKailleraServerSelect::updateIP(String & ip) {
	txtIP->setText(ip, true);
}
void juceKailleraServerSelect::updateLV() {
	lstServers->updateContent();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="juceKailleraServerSelect"
                 componentName="" parentClasses="public Component" constructorParams=""
                 variableInitialisers="" snapPixels="5" snapActive="1" snapShown="1"
                 overlayOpacity="0.330000013" fixedSize="1" initialWidth="580"
                 initialHeight="330">
  <BACKGROUND backgroundColour="ffffffff">
    <PATH pos="0 0 100 100" fill="solid: 33000000" hasStroke="0" nonZeroWinding="1">s 14.67 266.86 q 16.71 267.89 22.33 271.66 q 27.94 275.41 29.31 277.46 q 28.97 277.46 27.09 279.01 q 25.22 280.54 25.22 280.54 q -2.01 293.2 -26.18 293.2 q -31.98 293.2 -39.8 290.46 q -50.7 286.36 -50.7 279.86 q -50.7 271.99 -36.74 263.44 q -25.5 256.6 -19.37 255.58 q -1.67 252.16 5.47 252.16 q 6.5 252.16 16.37 253.18 q 26.24 254.21 27.27 254.21 q 32.03 254.21 35.78 246.34 q 38.84 240.54 42.93 229.24 q 46.33 212.49 46.33 204.29 l 46.33 165.29 q 42.93 163.25 37.48 163.25 q 29.99 163.58 28.63 163.25 q 20.12 161.87 12.29 155.38 q 4.45 148.88 4.45 141.36 q 4.45 127 20.12 116.4 q 24.88 113.32 44.63 104.43 q 44.63 90.75 44.29 82.53 q 43.95 76.73 43.78 74.16 q 43.6 71.6 43.6 71.94 q 28.28 76.04 16.71 76.04 q -6.77 76.04 -6.77 61 q -6.77 55.52 -1.67 51.76 q 3.44 47.99 7.86 47.99 q 17.4 47.99 18.07 53.98 q 18.76 59.96 20.46 59.96 q 28.28 59.96 40.88 49.03 q 52.11 39.11 56.04 31.42 q 59.95 23.71 61.65 14.83 q 65.4 16.2 67.1 22 q 70.51 34.66 70.51 56.21 l 70.51 99.98 q 70.51 101.34 73.22 104.09 q 79.35 105.11 92.96 109.21 q 94.34 107.85 95.01 106.47 q 99.44 92.79 99.44 80.49 q 99.44 71.6 97.05 63.73 q 96.38 61 92.96 57.23 q 89.56 53.47 89.56 52.45 q 89.56 49.03 98.76 42.86 q 106.92 36.71 108.97 36.71 q 110 36.71 113.4 40.3 q 116.81 43.9 117.82 43.9 q 121.91 43.9 126.33 34.32 q 124.97 32.95 120.54 30.56 q 120.54 29.88 120.88 28.51 q 125.65 24.4 145.05 16.54 q 166.51 7.65 177.05 7.65 q 188.64 7.65 188.64 13.8 q 188.64 19.96 184.89 19.96 q 183.87 19.96 183.87 18.93 l 183.87 18.93 q 183.87 18.93 182.5 18.93 q 178.08 18.93 169.91 23.38 q 163.78 27.15 161.4 29.19 q 156.3 33.29 147.79 42.86 q 141.31 51.08 125.99 80.82 q 124.29 84.59 105.56 116.74 q 116.46 126.31 127.02 135.88 q 140.29 146.14 150.5 145.81 q 146.41 148.54 143.7 159.83 q 141.65 169.06 141.65 176.24 q 141.65 209.07 161.4 249.09 q 180.8 274.39 197.49 287.05 q 197.49 287.39 197.82 288.41 q 220.29 301.74 241.4 304.49 l 241.4 309.27 q 238.34 310.29 231.19 310.29 q 202.6 310.29 178.42 292.17 q 174.33 290.46 142.34 259.69 q 137.57 252.16 132.8 244.63 q 124.29 230.95 122.93 230.95 q 121.91 230.95 118.33 231.99 q 114.76 233.01 113.06 233.01 q 104.89 233.01 99.1 227.88 q 91.95 221.73 92.63 210.09 q 92.96 209.42 94.66 208.38 q 107.61 207.02 107.61 181.03 q 107.61 170.42 104.2 155.72 q 100.11 138.63 95.01 135.55 q 82.42 146.83 72.54 153.32 l 72.54 181.37 q 72.54 198.47 75.61 217.28 q 76.63 222.75 81.39 226.51 q 86.15 230.28 91.26 230.28 q 89.56 235.41 81.9 241.9 q 74.24 248.4 66.76 254.21 q 66.76 253.87 65.73 248.74 q 65.4 246.01 61.31 246.01 q 53.49 246.01 53.49 253.18 q 53.49 254.56 58.25 259.69 q 57.22 261.73 51.44 265.84 q 55.19 269.95 60.97 278.5 q 65.4 286.7 65.4 295.25 q 65.4 321.59 43.95 321.59 q 35.78 321.59 30.67 318.17 q 24.21 314.06 24.54 309.62 l 24.54 304.49 q 24.54 303.45 26.92 303.45 q 27.94 303.45 29.82 305 q 31.69 306.53 32.72 306.53 q 41.23 306.53 41.23 295.25 q 41.23 289.43 31.69 279.17 q 22.16 268.91 12.97 265.84 q 2.75 263.11 -4.06 263.11 q -18.69 263.11 -29.59 269.6 q -38.78 275.08 -38.78 279.86 q -38.78 281.92 -37.42 282.25 q -29.59 284.65 -26.53 284.3 q -6.44 281.57 13.65 268.24 x s 83.44 125.62 q 81.05 124.6 71.52 121.53 l 71.52 134.17 l 74.93 134.17 q 80.72 130.42 83.44 125.62 x s 44.29 142.39 q 44.29 123.58 44.29 123.58 q 33.39 125.97 33.39 134.86 q 33.39 139.99 44.29 142.39 x</PATH>
  </BACKGROUND>
  <COMBOBOX name="new combo box" id="49c69771b5e49450" memberName="cmbModes"
            virtualName="" explicitFocusOrder="0" pos="448 8 120 24" editable="0"
            layout="33" items="" textWhenNonSelected="Switch" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="new component" id="24fcc1cb902187fd" memberName="lstServers"
                    virtualName="" explicitFocusOrder="0" pos="88 112 482 208" class="TableListBox"
                    params="T(&quot;serversList&quot;), &amp;serversListModel"/>
  <TEXTBUTTON name="new button" id="7820dfae9523cee6" memberName="btnAdd" virtualName=""
              explicitFocusOrder="0" pos="8 248 80 24" bgColOff="ffbbbbff"
              buttonText="add" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="38bc363dbffd9a1d" memberName="btnEdit"
              virtualName="" explicitFocusOrder="0" pos="8 272 80 24" bgColOff="ffbbbbff"
              buttonText="edit" connectedEdges="14" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="17e3c0b7c04af57d" memberName="btnDelete"
              virtualName="" explicitFocusOrder="0" pos="8 296 80 24" bgColOff="ffbbbbff"
              buttonText="delete" connectedEdges="6" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="c5ca009e63e8a920" memberName="txtNick"
              virtualName="" explicitFocusOrder="0" pos="88 16 150 24" bkgcol="ffffffff"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="4193092e268f1071" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 16 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Nick:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="fdb27d911d611a8f" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 48 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="C.S.:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <COMBOBOX name="new combo box" id="b0ff954ac89cfaff" memberName="cmbConnection"
            virtualName="" explicitFocusOrder="0" pos="88 48 150 24" editable="0"
            layout="33" items="LAN (60 PPS)&#10;Excelent (30 PPS)&#10;Good (20 PPS)&#10;Average (15 PPS)&#10;Bad (12 PPS)&#10;Low (10 PPS)"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTEDITOR name="new text editor" id="f728b2b0d08a866d" memberName="txtIP"
              virtualName="" explicitFocusOrder="0" pos="88 80 242 24" bkgcol="ffffffff"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="8bb7e2909f454dc2" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 80 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="IP:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="9ed530021e867943" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 112 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Fovorites:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="new button" id="338911b955ce9e7b" memberName="btnConnect"
              virtualName="" explicitFocusOrder="0" pos="330 80 72 24" bgColOff="ffbbbbff"
              buttonText="connect" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="68732a1e64ca5d16" memberName="btnPing"
              virtualName="" explicitFocusOrder="0" pos="400 80 72 24" bgColOff="ffbbbbff"
              buttonText="ping" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b87b3e9343f1e548" memberName="btnMastersList"
              virtualName="" explicitFocusOrder="0" pos="470 80 95 24" buttonText="master list"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
