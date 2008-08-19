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

#ifndef __JUCER_HEADER_JUCEGAMESELECT_JUCEGAMESELECT_4EDE2455__
#define __JUCER_HEADER_JUCEGAMESELECT_JUCEGAMESELECT_4EDE2455__

//[Headers]     -- You can add your own extra header files here --
#include "clientgui.h"

namespace n02 {

	class GameSelectLB: public ListBoxModel {
		int  getNumRows ();
		void  paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);
		void  listBoxItemClicked (int row, const MouseEvent &e);
		void  listBoxItemDoubleClicked (int row, const MouseEvent &e);
		void  returnKeyPressed (int lastRowSelected);
		void selectedRowsChanged (int lastRowSelected);

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
class juceGameSelect  : public Component,
                        public ButtonListener
{
public:
    //==============================================================================
    juceGameSelect ();
    ~juceGameSelect();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void closeUp();
	void updateGameInfo(String & info);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	n02::GameSelectLB gameListModel;
    //[/UserVariables]

    //==============================================================================
    ListBox* lstGames;
    TextButton* btnOk;
    TextButton* btnCancel;
    Label* lblGameInfo;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceGameSelect (const juceGameSelect&);
    const juceGameSelect& operator= (const juceGameSelect&);
};


#endif   // __JUCER_HEADER_JUCEGAMESELECT_JUCEGAMESELECT_4EDE2455__
