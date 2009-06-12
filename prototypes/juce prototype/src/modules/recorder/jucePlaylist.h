/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  9 Aug 2008 5:15:33 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEPLAYLIST_JUCEPLAYLIST_86C933B3__
#define __JUCER_HEADER_JUCEPLAYLIST_JUCEPLAYLIST_86C933B3__

//[Headers]     -- You can add your own extra header files here --
#include "clientgui.h"
#include "common.h"

namespace n02 {
	namespace playback {
		typedef struct {
			juce::tchar value[1024];
		} FileName;

		#include "krec.h"
		class PlayListBoxModel
			: public TableListBoxModel {
		public:
			DynamicOrderedArray<krecFileHeader01, 2> list;
			DynamicOrderedArray<FileName, 2> names;
			void playSelected(int rowNo);
			void saveSelected(int rowNo);
			void playBrowse();

			void RePopulateList();
			PlayListBoxModel(){}
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
class jucePlaylist  : public Component,
                      public ButtonListener,
                      public ComboBoxListener
{
public:
    //==============================================================================
    jucePlaylist ();
    ~jucePlaylist();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void handleCommandMessage (int commandId);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	n02::playback::PlayListBoxModel playListBox;
    //[/UserVariables]

    //==============================================================================
    TableListBox* lstPlayback;
    TextButton* btnBrowse;
    ComboBox* cmbModes;
    TextButton* btnPlay;
    TextButton* btnSave;
    TextButton* btnRefresh;
    TextButton* btnDelete;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    jucePlaylist (const jucePlaylist&);
    const jucePlaylist& operator= (const jucePlaylist&);
};


#endif   // __JUCER_HEADER_JUCEPLAYLIST_JUCEPLAYLIST_86C933B3__
