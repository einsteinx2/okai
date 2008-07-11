/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  6 Jul 2008 3:06:03 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEKAILLERASERVERGAME_JUCEKAILLERASERVERGAME_27E06946__
#define __JUCER_HEADER_JUCEKAILLERASERVERGAME_JUCEKAILLERASERVERGAME_27E06946__

//[Headers]     -- You can add your own extra header files here --
#include "juce02.h"
namespace n02 {
	namespace kaillera {

		class KailleraPlayersList: public TableListBoxModel {
			int  getNumRows ();
			void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
			void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
		};

		void uiStartGameCallback();
		void uiLeaveGameCallback();
		void uiKickGameCallback();

		class KailleraGameChatInput: public TextEditorListener {
			void  textEditorTextChanged (TextEditor &editor){}
			void  textEditorReturnKeyPressed (TextEditor &editor);
			void  textEditorEscapeKeyPressed (TextEditor &editor){}
			void  textEditorFocusLost (TextEditor &editor){}
		};

		extern int activeGameCaps;
		extern int selectedAutorunIndex;
		extern int selectedDelayParam;
		extern int recordingEnabled;
		extern bool hosting;

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
class juceKailleraServerGame  : public Component,
                                public ButtonListener,
                                public ComboBoxListener
{
public:
    //==============================================================================
    juceKailleraServerGame ();
    ~juceKailleraServerGame();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void appendText(String & text);
	void clearText();
	void updateAutorunItems();
	void updatePlayers();
	void redrawPlayersRow(int index);
	void handleCommandMessage(int  commandId);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	n02::kaillera::KailleraPlayersList kailleraPlayers;
	int textLength;
	n02::kaillera::KailleraGameChatInput chatInput;
    //[/UserVariables]

    //==============================================================================
    TextEditor* txtChat;
    TextEditor* txtChatInput;
    TableListBox* lstPlayers;
    TextButton* btnStartGame;
    TextButton* btnLeaveGame;
    TextButton* btnKick;
    ComboBox* cmbRun;
    Label* lblRun;
    ComboBox* cmbDelay;
    Label* lblDelay;
    ToggleButton* chkRecord;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraServerGame (const juceKailleraServerGame&);
    const juceKailleraServerGame& operator= (const juceKailleraServerGame&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERASERVERGAME_JUCEKAILLERASERVERGAME_27E06946__
