//OSXDialog.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ApplicationKitPrivate.h"
#include "vcf/ApplicationKit/OSXDialog.h"
#include "vcf/ApplicationKit/OSXControl.h"

using namespace VCF;

OSXDialog::OSXDialog()	:
	owner_(NULL),
	dialogComponent_(NULL),
	dialogRef_(NULL)
{

}

OSXDialog::OSXDialog( Control* owner, Dialog* component ):
	owner_(owner),
	dialogComponent_(component),
	dialogRef_(NULL)
{

}

OSXDialog::~OSXDialog()
{

}

WindowAttributes OSXDialog::getCreationWinAttrs()
{
	return kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute |
              kWindowCompositingAttribute |
              kWindowStandardHandlerAttribute | kWindowLiveResizeAttribute;
}

void OSXDialog::create( Control* owningControl )
{
	if ( NULL == owner_ ) {
		OSXWindow::create( owningControl );
	}
	else {
		WindowAttributes attrs = kWindowCompositingAttribute | kWindowStandardHandlerAttribute;
	
		::Rect bounds = {0,0,0,0};

		OSStatus err = CreateNewWindow( kSheetAlertWindowClass, attrs, &bounds, &windowRef_ );
		if ( noErr != err ) {
			throw RuntimeException( MAKE_ERROR_MSG_2("CreateNewWindow() failed!") );
		}
		else {
			err = SetWindowProperty( windowRef_, 
									 VCF_PROPERTY_CREATOR, 
									 VCF_PROPERTY_WINDOW_VAL, 
									 sizeof(OSXWindow*), 
									 this );
			
			if ( noErr != err ) {
				throw RuntimeException( MAKE_ERROR_MSG_2("SetWindowProperty() failed!") );
			}
			
			static EventTypeSpec eventsToHandle[] ={
				// { kEventClassWindow, kEventWindowGetIdealSize },
				//{ kEventClassCommand, kEventCommandProcess },
				//{ kEventClassCommand, kEventCommandUpdateStatus },
			{ kEventClassWindow, kEventWindowClose },
			{ kEventClassWindow, kEventWindowActivated },
			{ kEventClassWindow, kEventWindowDeactivated },
			{ kEventClassWindow, kEventWindowFocusAcquired },
			{ kEventClassWindow, kEventWindowFocusRelinquish },
				
			{ kEventClassWindow, kEventWindowDrawContent },
			{ kEventClassMouse, kEventMouseDown },
			{ kEventClassMouse, kEventMouseUp },
			{ kEventClassMouse, kEventMouseMoved },
			{ kEventClassMouse, kEventMouseDragged },
			{ kEventClassMouse, kEventMouseEntered },
			{ kEventClassMouse, kEventMouseExited },
			{ kEventClassMouse, kEventMouseWheelMoved },
			{ kEventClassKeyboard, kEventRawKeyDown },
			{ kEventClassKeyboard, kEventRawKeyUp },
			{ kEventClassWindow, kEventWindowBoundsChanged } };
			
			
			InstallWindowEventHandler( windowRef_,
									   OSXWindow::getEventHandlerUPP(),
									   sizeof(eventsToHandle)/sizeof(eventsToHandle[0]),
									   eventsToHandle,
									   this,
									   &handlerRef_ );
			
			
			
			Frame* ownerFrame = owner_->getParentFrame();
			sheetParent_ = (WindowRef) ownerFrame->getPeer()->getHandleID();
			
			UIToolkit::postEvent( new GenericEventHandler<Control>( owningControl, &Control::handleEvent ),
								  new VCF::ComponentEvent( owningControl, Component::COMPONENT_CREATED ),	true );		
			
		}
	}
}

void OSXDialog::setVisible( const bool& visible )
{
	 if ( NULL != owner_ ) {
		if ( visible ) {
			ShowSheetWindow( windowRef_, sheetParent_ );
		}
		else {
			HideSheetWindow( windowRef_ );
		}
	 }
	 else {
		OSXWindow::setVisible( visible ); 
	 }
}

void OSXDialog::showMessage( const String& message, const String& caption )
{
	DialogItemIndex itemIndex;
	CFTextString msg;
	msg = message;
	
	
	CreateStandardAlert( kAlertPlainAlert, 
						 msg, 
						 NULL, 
						 NULL,
						 &dialogRef_ );
						 
	windowRef_ = GetDialogWindow(dialogRef_);
	
	setText( caption );
	
	RunStandardAlert (dialogRef_, NULL, &itemIndex);
	
	dialogRef_ = NULL;
}

UIToolkit::ModalReturnType OSXDialog::showMessage( const String& message, const String& caption,
												const long& messageButtons,	const Dialog::MessageStyle& messageStyle )
{
	UIToolkit::ModalReturnType result = UIToolkit::mrNone;
	DialogItemIndex itemIndex;
	CFTextString msg;
	msg = message;
	
	AlertStdCFStringAlertParamRec alertParams;
	
    AlertType alertType = kAlertPlainAlert;
	
	GetStandardAlertDefaultParams(&alertParams,kStdCFStringAlertVersionOne);
	
	switch ( messageStyle ){
		case Dialog::msDefault: {

		}
		break;

		case Dialog::msError: {
			alertType = kAlertStopAlert;
		}
		break;

		case Dialog::msInfo: {
			alertType = kAlertNoteAlert;
		}
		break;

		case Dialog::msWarning: {
			alertType = kAlertCautionAlert;
		}
		break;
	}
	
	if ( messageButtons & Dialog::mbOK ) {
		alertParams.defaultText=(CFStringRef)kAlertDefaultOKText;  
		alertParams.defaultButton=kAlertStdAlertOKButton;  
	}
	else if ( messageButtons & Dialog::mbOKCancel ) {
		alertParams.cancelText = (CFStringRef)kAlertDefaultCancelText;
		alertParams.cancelButton = kAlertStdAlertCancelButton;
	}
	else if ( messageButtons & Dialog::mbYesNo ) {
		alertParams.defaultText = CFSTR("Yes");
		
		alertParams.cancelText = CFSTR("No");
		alertParams.cancelButton = kAlertStdAlertOtherButton;
	}
	else if ( messageButtons & Dialog::mbYesNoCancel ) {
		alertParams.defaultText = CFSTR("Yes");
		
		alertParams.cancelText = CFSTR("No");
		alertParams.cancelButton = kAlertStdAlertOtherButton;
		
		alertParams.otherText = CFSTR("Cancel");
	}
	else if ( messageButtons & Dialog::mbRetryCancel ) {
		alertParams.defaultText = CFSTR("Retry");
		
		alertParams.cancelText = CFSTR("Cancel");
		alertParams.cancelButton = kAlertStdAlertOtherButton;
	}
	else if ( messageButtons & Dialog::mbAbortRetryIgnore ) {
		alertParams.defaultText = CFSTR("Abort");
		
		alertParams.cancelText = CFSTR("Retry");
		alertParams.cancelButton = kAlertStdAlertOtherButton;
		
		alertParams.otherText = CFSTR("Ignore");
	}

	if ( messageButtons & Dialog::mbHelp ) {
		
	}
						
	CreateStandardAlert( alertType, 
						 msg, 
						 NULL, 
						 &alertParams,
						 &dialogRef_ );
						 
	windowRef_ = GetDialogWindow(dialogRef_);
	
	setText( caption );
	
	RunStandardAlert (dialogRef_, NULL, &itemIndex);
	
	if ( messageButtons & Dialog::mbOK ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrOK;
		}
		else {
			result = UIToolkit::mrCancel;
		}
	}
	else if ( messageButtons & Dialog::mbOKCancel ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrOK;
		}
		else if ( itemIndex == 2 ) {
			result = UIToolkit::mrCancel;
		}
	}
	else if ( messageButtons & Dialog::mbYesNo ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrYes;
		}
		else if ( itemIndex == 2 ) {
			result = UIToolkit::mrNo;
		}
	}
	else if ( messageButtons & Dialog::mbYesNoCancel ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrYes;
		}
		else if ( itemIndex == 2 ) {
			result = UIToolkit::mrNo;
		}
		else if ( itemIndex == 3 ) {
			result = UIToolkit::mrCancel;
		}
	}
	else if ( messageButtons & Dialog::mbRetryCancel ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrRetry;
		}
		else if ( itemIndex == 2 ) {
			result = UIToolkit::mrCancel;
		}
	}
	else if ( messageButtons & Dialog::mbAbortRetryIgnore ) {
		if ( itemIndex == 1 ) {
			result = UIToolkit::mrAbort;
		}
		else if ( itemIndex == 2 ) {
			result = UIToolkit::mrRetry;
		}
		else if ( itemIndex == 3 ) {
			result = UIToolkit::mrIgnore;
		}
	} 
	
	dialogRef_ = NULL;
	
	return result;
}

void OSXDialog::init()
{

}

OSStatus OSXDialog::handleOSXEvent( EventHandlerCallRef nextHandler, EventRef theEvent )
{
	return OSXWindow::handleOSXEvent( nextHandler, theEvent );
}


/**
*CVS Log info
*$Log$
*Revision 1.2.2.1  2004/10/18 03:10:30  ddiego
*osx updates - add initial command button support, fixed rpoblem in mouse handling, and added dialog support.
*
*Revision 1.2  2004/08/07 02:49:08  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.4  2004/06/06 07:05:30  marcelloptr
*changed macros, text reformatting, copyright sections
*
*/


