#ifndef _VCF_DIALOG_H__
#define _VCF_DIALOG_H__
//Dialog.h

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

class DialogPeer;
class WindowPeer;

#define DIALOG_CLASSID		"ED88C09A-26AB-11d4-B539-00C04F0196DA"


/**
\class DialogEvent DialogEvent.h "vcf/ApplicationKit/DialogEvent.h"
*/
class DialogEvent : public Event {
public:
	DialogEvent( Object* source, const ulong32& eventType ) : Event(source,eventType),
		returnValue_(UIToolkit::mrNone){
	
	}
	
	DialogEvent( const DialogEvent& rhs ):Event(rhs) {		
		*this = rhs;
	}
	
	DialogEvent& operator=( const DialogEvent& rhs ) {
		Event::operator =( rhs );

		returnValue_ = rhs.returnValue_;
		
		return *this;
	}
	
	virtual Object* clone( bool deep=false ) {
		return new DialogEvent(*this);
	}
	
	UIToolkit::ModalReturnType getModalResult() const {
		return returnValue_;
	}
	
	void setModalResult( UIToolkit::ModalReturnType val ) {
		returnValue_ = val;
	}
	
	
protected:
	
	UIToolkit::ModalReturnType returnValue_;
};

/**
\class Dialog Dialog.h "vcf/ApplicationKit/Dialog.h"
A Dialog is a popup frame that is generally used for user input. 


A Dialog has a special state variable that tells what the "mode"
of the dialog is. This mode is determined not by how the dialog is 
\em created but rather by how the dialog is displayed which is 
done by calling either Dialog:show(), Dialog::showModal(), or 
Dialog::showSheetModal(). Each of these sets the dialog's modal
state to either modeless, modal, or sheet modal. A modeless dialog 
is visible till the user closes the dialog, but it doesn't prevent
the use from changing focus back to the orginal window that launched
the dialog. A modal, or application modal, dialog, disables everything
else, and enters into a modal event loop that does not end till the
dialog is closed. Nothing else can be accomplished in the UI till the 
dialog is closed. A sheet modal dialog is used to attach a dialog to 
a specific window, and while the dialog is visible the window that launched
(also known as the \em owner window) the dialog is disabled, but all other 
UI activity can still take place. When the sheet modal dialog is closed the
owner window is enabled again for UI input. 


Think of a sheet modal dialog
as a middle ground between modeless and application modal.


A dialog's UI is normally a frame with a close button, and optionally a help and system 
button as well. The caption is the standard height.

@delegates
	@del Dialog::SheetModalFinished
	@del Dialog::ModalFinished
 */
class APPLICATIONKIT_API Dialog : public Frame {
public:

	enum MessageStyle {
		msDefault = 0,
		msError,
		msInfo,
		msWarning,
		msQuestion,
	};

	enum MessageButtons {
		mbOK = 1,
		mbHelp = 2,
		mbYesNo = 4,
		mbYesNoCancel = 8,
		mbOKCancel = 16,
		mbRetryCancel = 32,
		mbAbortRetryIgnore = 64
	};
	
	enum DialogEvents {
		deSheetModalFinished = CONTROL_EVENTS_LAST + 1241,
		deModalFinished
	};
	
	enum ModalState {
		msNonModal	= 0,
		msAppModal = 1,
		msSheetModal = 2
	};
	
	/**
	@delegate SheetModalFinished this delegate is used to fire events when the dialog displayed
	by calling showSheetModal is closed. The DialogEvent will contain the value of the 
	modal result from closing the dialog.
	@event DialogEvent
	@eventtype Dialog::deSheetModalFinished
	*/	
	DELEGATE( SheetModalFinished );
	
	/**
	@delegate ModalFinished This delegate is called to fire an event when the dialog 
	is closed after being displayed by a call to showModal. The DialogEvent will contain 
	the value of the modal result from closing the dialog.
	@event DialogEvent
	@eventtype Dialog::deModalFinished
	*/	
	DELEGATE( ModalFinished );
	

	Dialog( Control* owner=NULL );

	//Dialog( Frame* owner );

	virtual void setCaption( const String& caption );

	virtual void paint(GraphicsContext * context);

	/**
	*overridden to take into account client bounds
	**/
	//virtual void resizeChildren();

	virtual Rect getClientBounds(const bool& includeBorder=true);

	virtual void setClientBounds( Rect* bounds );

	/**
	This displays the dialog and enters into an application
	modal state. This function will \em not return till the 
	modal event loop is finished. The event loop can be finished
	by the user clicking on the close button of the dialog (if
	present), or by clicking on one of the button present on 
	the dialog's UI. Alternately the user may hit the "Esc"
	key and this will also close the dialog and quit the 
	modal event loop. The same can be achieved programmatically
	by calling the dialog's close() method.
	*/
	virtual UIToolkit::ModalReturnType showModal();

	/**
	This function returns once the dialog is displayed, but is considered
	"modal" for the window that owns it. If this is called and no owner window
	has been specified for the dialog, then an exception is thrown.
	The idea is to have a dialog that is part or related to the functionality of a 
	specific window (probabably a document window). Unlike showModal() which puts the
	entire application into a modal state and does not return till the modal event
	loop is finished, showSheetModal() returns. To get the "modal" result, you should
	add a custom event handler to the SheetModalFinished delegate. When the dialog
	is closed the SheetModalFinished delegate will fire an event that contains the modal
	result value.
	@see DialogEvent
	*/
	virtual void showSheetModal();

	virtual void show();

	virtual void close();

	UIToolkit::ModalReturnType getModalReturnValue() {
		return returnValue_;
	}

	void setModalReturnValue( const UIToolkit::ModalReturnType& returnType ) {
		returnValue_ = returnType;
	}

	static void showMessage( const String& message, const String& caption="" );

	static UIToolkit::ModalReturnType showMessage( const String& message, const String& caption,
											const long& messageButtons = mbOKCancel,
											const MessageStyle& messageStyle=msDefault );

	virtual void setFrameStyle( const FrameStyleType& frameStyle );

	virtual bool allowsActivation() {
		return true;
	}

	virtual void setIconImage( Image* icon );

	virtual Image* getIconImage();
	
	virtual bool isActiveFrame();

	Control* getOwner() {
		return owner_;
	}

	/**
	returns true if the dialog's Dialog::showModal() method was called to display
	teh dialog (indicating that a modal event loops is active), other wise
	it returns false, indicating non-modal behaviour.
	*/
	bool isModal() {
		return modal_ == msAppModal;
	}
	
	bool isSheetModal() {
		return modal_ == msSheetModal;
	}

	virtual void keyDown( KeyboardEvent* e );
protected:
    DialogPeer * dialogPeer_;
	WindowPeer * windowPeer_;
	Control* owner_;
	Control* previousFocusedControl_;
	UIToolkit::ModalReturnType returnValue_;
	ModalState modal_;

	void onDialogClose( Event* event );
	void onPostClose( Event* e );
	
	void showWithModalState( ModalState state );

	virtual ~Dialog();
};

};


/**
*CVS Log info
*$Log$
*Revision 1.4.2.2  2006/03/18 22:17:42  ddiego
*removed par tag for doxygen comments as its not needed and
*screws up the doc formatting.
*
*Revision 1.4.2.1  2006/03/14 02:25:46  ddiego
*large amounts of source docs updated.
*
*Revision 1.4  2005/07/09 23:14:52  ddiego
*merging in changes from devmain-0-6-7 branch.
*
*Revision 1.3.2.1  2005/06/07 21:16:24  marcelloptr
*added Dialog::msQuestion
*
*Revision 1.3  2004/12/01 04:31:21  ddiego
*merged over devmain-0-6-6 code. Marcello did a kick ass job
*of fixing a nasty bug (1074768VCF application slows down modal dialogs.)
*that he found. Many, many thanks for this Marcello.
*
*Revision 1.2.2.2  2004/10/28 03:34:16  ddiego
*more dialog updates for osx
*
*Revision 1.2.2.1  2004/10/25 03:23:57  ddiego
*and even more dialog updates. Introduced smore docs to the dialog class and added a new showXXX function.
*
*Revision 1.2  2004/08/07 02:49:07  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.4  2004/07/09 03:39:29  ddiego
*merged in changes from the OSX branch for new theming API. Added
*support for controlling the use of locale translated strings in components.
*
*Revision 1.1.2.3.2.1  2004/07/06 03:27:12  ddiego
*more osx updates that add proper support
*for lightweight controls, some fixes to text layout, and some window painting issues. Also a fix
*so that controls and windows paint either their default theme background or their background
*color.
*
*Revision 1.1.2.3  2004/06/06 07:05:30  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 03:43:13  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:16  ddiego
*migration towards new directory structure
*
*Revision 1.18.4.1  2004/04/26 21:58:19  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.18  2003/12/18 05:15:57  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.17.2.5  2003/09/21 04:15:34  ddiego
*moved the cvs info data to teh bottom of the header instead of the
*top so it is easier to get to the actual header class declarations
*instead of having to wade through all the cvs info stuff.
*
*Revision 1.17.2.4  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.17.2.3  2003/09/05 21:03:38  ddiego
*moved the various control destructor's to protected scope
*migrated code into destroy() overridden methods
*----------------------------------------------------------------------
*
*Revision 1.17.2.2  2003/08/28 21:47:31  ddiego
*added escape reponse...
*
*Revision 1.17.2.1  2003/08/18 19:52:32  ddiego
*changed the Container from being a class you derive from to a separate
*intance that is created and assigned dynamically to any Control.
*
*Revision 1.17  2003/08/09 02:56:42  ddiego
*merge over from the devmain-0-6-1 branch
*Changes
*Features:
*-Added additional implementation to better support the MVC architecture in
*the VCF
*
*-Added a Document/View architecure that is similar to MFC's or NextSteps's
*Doc/View architectures
*
*-Integrated the Anti Grain Graphics library into the GraphicsKit. There is
*now basic support for it in terms of drawing vector shapes
*(fills and strokes). Image support will come in the next release
*
*-Added several documented graphics tutorials
*
*Bugfixes:
*
*[ 775744 ] wrong buttons on a dialog
*[ 585239 ] Painting weirdness in a modal dialog ?
*[ 585238 ] Modal dialog which makes a modal Dialog
*[ 509004 ] Opening a modal Dialog causes flicker
*[ 524878 ] onDropped not called for MLTcontrol
*
*Plus an issue with some focus and getting the right popup window to activate
*has also been fixed
*
*Revision 1.16.2.3  2003/06/13 00:00:31  ddiego
*more xmake fixes for the new version
*bug fixes for bugs
*509004	Opening a modal Dialog causes flicker
*also have tooltips working again
*
*Revision 1.16.2.2  2003/06/09 03:57:14  ddiego
*fixed an issue with tooltip not showing up, and while fixing that found it
*accidentally caused another problem with application focus for the main
*window not being handled correctly. This is now working OK! Now just
*have to get rid of the dialog flicker...
*Also fixed the QTPlayer example. It had some syntax errors from using the older
*event macros (not using Delegates)
*
*Revision 1.16.2.1  2003/05/27 04:45:24  ddiego
*doing some code cleanup to fix some bugs in the way dialogs and popups
*work in Win32 port. Cleaned up the ControlPeer and DialogPeer interfaces
*a bit.
*
*Revision 1.16  2003/05/17 20:37:03  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.15.16.2  2003/03/23 03:23:45  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.15.16.1  2003/03/12 03:09:33  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.15  2002/05/09 03:10:42  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.14.4.1  2002/04/27 15:42:25  ddiego
*removed ApplicationKit.h and optimized header includes as
*well as adding better include guard support
*
*Revision 1.14  2002/01/30 22:12:47  ddiego
*fixed the internals of default CommandButton::click() method
*closes the dialog without relying on the WM_COMMAND message,
*and responds to  vkEnter keyboards events more correctly, i.e.
*the Doalog will automatically close now when you hit the enter key
*and the button wil set the modal result of the dialogd
*
*Revision 1.13  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_DIALOG_H__


