#ifndef _VCF_COMMANDBUTTON_H__
#define _VCF_COMMANDBUTTON_H__
//CommandButton.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


/* Generated by Together */



namespace VCF{


class ButtonPeer;





#define COMMANDBUTTON_CLASSID		"8B2CDC30-3CAD-11d4-B553-00C04F0196DA"

/**
The CommandButton is a native push button that fires an event
when pressed, or "clicked".
@delegates
	@del CommandButton::ButtonClicked
*/
class APPLICATIONKIT_API CommandButton :  public VCF::Control, public VCF::Button {
public:

	CommandButton();

	virtual ~CommandButton();

	/**
	@delegate ButtonClicked this is fired when the button is clicked, either
	in response to events sent by the native windowing system, or in response
	to calling the click() method.
	@event ButtonEvent
	@see click()
	*/
	DELEGATE(ButtonClicked);

	/**
	*This macro creates a method for adding a listener to the CommandButton's events
	*/
	virtual void addButtonClickHandler( EventHandler* handler ) {
		ButtonClicked += handler;
	}

	/**
	*This macro creates a method for removing a listener to the CommandButton's events
	*/
	virtual void removeButtonClickHandler( EventHandler* handler ) {
		ButtonClicked -= handler;
	}


    virtual void click();

	virtual void setDefault( const bool& defaultButton );

	virtual bool isDefault();

	/**
	*sets the caption for the button. The is the
	*text that is displayed on the button's face
	*/
    void setCaption( const String& caption );

    String getCaption();

	/**
	*The command type the button represents.
	*Certain command types will automatically send
	*the event to the dialog they belong to when
	*used on modal dialogs.
	*/
	ButtonCommandType getCommandType();

	/**
	*sets the
	*/
	void setCommandType( const ButtonCommandType& commandType );

	/**
	*
	*/
	virtual void paint(GraphicsContext * context);

	/**
	*
	*/
	void setIsPressed( const bool& pressed ) {
		isPressed_ = pressed;
	}

	/**
	*
	*/
	bool isPressed() {
		return isPressed_;
	}

	/**
	*
	*/
	virtual void mnemonicActivate();

	virtual double getPreferredHeight();
protected:
    bool isPressed_;
    ButtonPeer * buttonPeer_;
    String caption_;
	ButtonCommandType commandType_;

	void onFocusGained( FocusEvent* event );
	void onFocusLost( FocusEvent* event );


};

}


/**
*CVS Log info
*$Log$
*Revision 1.1.2.3  2004/06/06 07:05:29  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 03:43:12  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:14  ddiego
*migration towards new directory structure
*
*Revision 1.19.4.1  2004/04/26 21:58:18  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.19  2003/12/18 05:15:56  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.18.4.4  2003/12/08 05:05:27  ddiego
*added a bunch more documentation to classes, and added support for new doc
*keywords (@delegates, @del, @delegate, @event, and @eventtype). This
*makes it easier to see at a glance what events a class fires off.
*Added finishing touches for the Action implementation and have a partially
*complete example for this checked in.
*
*Revision 1.18.4.3  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.18.4.2  2003/09/06 19:45:44  ddiego
*changes to ensure that the destroy() is properly called.
*Destructors were moved back to pbulic scope in case that was causing a bug.
*Moving them all(all teh component derived classes) to private scope causes
*a complete failure in hte VC6 compiler. Alas...
*So destructors are present, and modal dialogs can still be created on the
*stack, but almost any other component should be created on the heap, and
*gotten rid of via free() NOT using operator delete directly.
*also moved the vcs info to the end of the file for some of the headers
*This will be an ongoing process.
*
*Revision 1.18.4.1  2003/09/05 21:03:38  ddiego
*moved the various control destructor's to protected scope
*migrated code into destroy() overridden methods
*----------------------------------------------------------------------
*
*Revision 1.18  2003/05/17 20:37:01  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.17.2.2  2003/03/23 03:23:44  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.17.2.1  2003/03/12 03:09:21  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.17  2003/02/26 04:30:37  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.16.2.4  2003/02/24 05:42:17  ddiego
*moved the code for the VariantData calss into it's own header
*migrated to the new event style using Delegates instead of relying on
*the ugly macros that we were using before - same functionality though
*made sure everything still works and compiles, including neccessary
*changes in the VCF Builder so that it creates code in the new style
*This changes to teh new style of using Delegates completes task 58837
*
*Revision 1.16.2.3  2003/02/03 05:21:55  ddiego
*added support for command types to the PuchButton class
*added support for clicks in the PuchButton class to auto close a dialog
*if it is parented to one, similar to the CommandButton. This is part of the
*gradual phase out of the CommandButton class
*Add supprt for drawing a focus rect in the PushButton class
*plugged in tabbing bahaviour to the X11UIToolkit. It seems as though
*"Shift" + "Tab" is not being picked up properly
*Finished implementing the X11Dialog::showMessage(String,String) method
*and also add some finishing touches to teh X11Dialog so it displays correctly
*Add some finishing touches to the X11UIToolkit::runModalEventLoop() as well
*
*Revision 1.16.2.2  2002/12/27 23:04:30  marcelloptr
*Improved macros for automatic import/export of libraries. - Warning fixes. - Other Minor Changes.
*
*Revision 1.16.2.1  2002/12/25 22:37:59  ddiego
*more stupid fixes to get rid of the damn gcc no newline warning...grrr...
*
*Revision 1.16  2002/11/18 00:46:07  ddiego
*this is the merge over of all the bug fixes and features (mostly
*VCF Builder stuff) from the devmain-0-5-8 branch
*
*Revision 1.15.10.1  2002/09/27 23:38:35  ddiego
*added support for geting various layout information
*added support for drawing various graphics UI primitives like scroll buttons
*tabs, tab pages, etc, partially to have this support in place for use
*in MacOSX.
*Also added scrolling buttons to TabbedPages, this fixes task 56652 in the
*ApplicationKit list
*
*Revision 1.15  2002/05/09 03:10:42  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.14.4.1  2002/04/27 15:41:54  ddiego
*removed ApplicationKit.h and optimized header includes as
*well as adding better include guard support
*
*Revision 1.14  2002/02/17 01:07:14  ddiego
*removed OpenGL stuff to minimize memory impact. It will be in it's own
*module. Also fixed bugs:
*[ #500694 ] Combo text over writing down arrow
*[ #498481 ] pasting into edit control
*
*Revision 1.13  2002/01/26 15:11:08  ddiego
*Default button now working
*
*Revision 1.12  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_COMMANDBUTTON_H__


