/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  20 Aug 2008 4:33:37 pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_1CA874D0__
#define __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_1CA874D0__

//[Headers]     -- You can add your own extra header files here --
#include "clientgui.h"
#include "kaillera_uiServerWindowLists.h"

namespace n02 {
	namespace kaillera {
		class KailleraUsers: public TableListBoxModel {
			int  getNumRows ();
			void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
			void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
			void  cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e);
		};
		class KailleraGames: public TableListBoxModel {
			int  getNumRows ();
			void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
			void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
			void  cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e);
		};
		class KailleraChatInput: public TextEditorListener {
			void  textEditorTextChanged (TextEditor &editor){}
			void  textEditorReturnKeyPressed (TextEditor &editor);
			void  textEditorEscapeKeyPressed (TextEditor &editor){}
			void  textEditorFocusLost (TextEditor &editor){}
		};
		void uiNewGameCallback();
		void uiSetTitleCallback(String & text);
		void uiGameWindowCreateCallback();
		void uiGameWindowCreateCloseCallback();
		void uiGameWindowShowCallback();
		void uiGameWindowHideCallback();
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
class juceKailleraServerConnection  : public Component,
                                      public ButtonListener
{
public:
    //==============================================================================
    juceKailleraServerConnection ();
    ~juceKailleraServerConnection();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void appendText(String & text);
	void clearText();
	void sendCommand(n02::kaillera::KailleraListsCommand * cmd);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	n02::kaillera::KailleraUsers kailleraUsers;
	n02::kaillera::KailleraGames kailleraGames;
	n02::kaillera::KailleraChatInput chatInputListener;
	void handleCommandMessage(int  commandId);
	int textLength;
    //[/UserVariables]

    //==============================================================================
    TableListBox* lstUsers;
    TextEditor* txtChat;
    TextEditor* txtChatInput;
    TextButton* btnNewGame;
    TableListBox* lstGames;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    juceKailleraServerConnection (const juceKailleraServerConnection&);
    const juceKailleraServerConnection& operator= (const juceKailleraServerConnection&);
};


#endif   // __JUCER_HEADER_JUCEKAILLERASERVERCONNECTION_JUCEKAILLERASERVERCONNECTION_1CA874D0__
