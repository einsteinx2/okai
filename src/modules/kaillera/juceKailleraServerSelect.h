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

#ifndef __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_17CB935__
#define __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_17CB935__

//[Headers]     -- You can add your own extra header files here --
#include "juce.h"
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
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.


	class ServersListListboxModel
		: public TableListBoxModel {
		int  getNumRows ();
		void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
		void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
		void  cellClicked (int rowNumber, int columnId, const MouseEvent &e);
		void  cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e);
		void  deleteKeyPressed (int lastRowSelected);
		void  returnKeyPressed (int lastRowSelected);
	} serversListModel;



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
    Path internalPath1;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraServerSelect (const juceKailleraServerSelect&);
    const juceKailleraServerSelect& operator= (const juceKailleraServerSelect&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERASERVERSELECT_JUCEKAILLERASERVERSELECT_17CB935__
