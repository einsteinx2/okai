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

#ifndef __JUCER_HEADER_JUCEPPSELECT_JUCEPPSELECT_4F97FC86__
#define __JUCER_HEADER_JUCEPPSELECT_JUCEPPSELECT_4F97FC86__

//[Headers]     -- You can add your own extra header files here --
#include "juce02.h"
#include "p2p_uiSelect.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class jucep2pSelect  : public Component,
                       public ComboBoxListener,
                       public ButtonListener
{
public:
    //==============================================================================
    jucep2pSelect ();
    ~jucep2pSelect();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void saveConfig();
	void handleCommandMessage(int  commandId);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	n02::p2p::StoredListListboxModel storedListBoxModel;
    //[/UserVariables]

    //==============================================================================
    Label* label;
    Label* label2;
    TextEditor* txtNick;
    TextEditor* txtPort;
    ComboBox* cmbModes;
    TextButton* btnNew;
    TextEditor* txtIP;
    TextButton* btnConnect;
    Label* label3;
    TableListBox* lstStored;
    Label* label4;
    TextButton* btnAdd;
    TextButton* btnEdit;
    TextButton* btnDelete;
    Path internalPath1;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    jucep2pSelect (const jucep2pSelect&);
    const jucep2pSelect& operator= (const jucep2pSelect&);
};


#endif   // __JUCER_HEADER_JUCEPPSELECT_JUCEPPSELECT_4F97FC86__
