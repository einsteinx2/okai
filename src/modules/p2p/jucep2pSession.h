/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  29 Jul 2008 10:15:29 am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.11

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_JUCEPPSESSION_JUCEPPSESSION_BD255178__
#define __JUCER_HEADER_JUCEPPSESSION_JUCEPPSESSION_BD255178__

//[Headers]     -- You can add your own extra header files here --
#include "juce02.h"
#include "p2p_uiSession.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class jucep2pSession  : public Component,
                        public ButtonListener,
                        public ComboBoxListener
{
public:
    //==============================================================================
    jucep2pSession ();
    ~jucep2pSession();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateAutorunItems();
	void sendMessage(int param1 = 0, void* param2 = 0, int param3 = 0) {
		messenger.postMessage(new Message(param1, reinterpret_cast<int>(param2), param3, this));
	}
	void setReady(bool rdy) {
		chkReady->setToggleState(rdy, false);
	}
	void appendText(String*);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int textLength;
	class P2PSessionMessageListener :
		public MessageListener
	{
		void  handleMessage (const Message &message);
	} messenger;
    //[/UserVariables]

    //==============================================================================
    TextEditor* txtChat;
    TextEditor* txtChatInput;
    ToggleButton* chkReady;
    ComboBox* cmbRun;
    Label* lblRun;
    ComboBox* cmbDelay;
    Label* lblDelay;
    ToggleButton* chkRecord;
    ComboBox* cmbSmoothing;
    Label* lblSmoothing;
    TextButton* btnDisconnect;
    TextButton* btnGetIP;
    TextButton* btnDrop;
    TextButton* btnChangeGame;
    TextButton* btnCpr;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    jucep2pSession (const jucep2pSession&);
    const jucep2pSession& operator= (const jucep2pSession&);
};


#endif   // __JUCER_HEADER_JUCEPPSESSION_JUCEPPSESSION_BD255178__
