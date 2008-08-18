/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  18 Aug 2008 2:10:08 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_8071AC4F__
#define __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_8071AC4F__

//[Headers]     -- You can add your own extra header files here --
#include "juce02.h"
namespace n02 {
	namespace kaillera {
		class ServersListListboxModel
			: public TableListBoxModel {
public:
				ServersListListboxModel(){}
				int  getNumRows ();
				void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
				void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
				void  cellClicked (int rowNumber, int columnId, const MouseEvent &e);
				void  cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e);
				void  deleteKeyPressed (int lastRowSelected);
				void  returnKeyPressed (int lastRowSelected);
		};
	};
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class juceKailleraServerSelect  : public Component,
                                  public ComboBoxListener,
                                  public ButtonListener
{
public:
    //==============================================================================
    juceKailleraServerSelect ();
    ~juceKailleraServerSelect();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void saveConfig();
	void updateIP(String & ip);
	void updateLV();
	void updateServers();
	void redrawServersRow(int index);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.


	n02::kaillera::ServersListListboxModel serversListModel;


    //[/UserVariables]

    //==============================================================================
    ComboBox* cmbModes;
    TableListBox* lstServers;
    TextButton* btnAdd;
    TextButton* btnEdit;
    TextButton* btnDelete;
    TextEditor* txtNick;
    Label* label;
    Label* label2;
    ComboBox* cmbConnection;
    TextEditor* txtIP;
    Label* label3;
    Label* label4;
    TextButton* btnConnect;
    TextButton* btnPing;
    TextButton* btnMastersList;
    HyperlinkButton* hlnkWebsite;
    Path internalPath1;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraServerSelect (const juceKailleraServerSelect&);
    const juceKailleraServerSelect& operator= (const juceKailleraServerSelect&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_8071AC4F__
