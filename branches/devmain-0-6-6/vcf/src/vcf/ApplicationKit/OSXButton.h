#ifndef _VCF_OSXBUTTON_H__
#define _VCF_OSXBUTTON_H__

#ifndef _VCF_DRAWUISTATE_H__
#	include "vcf/GraphicsKit/DrawUIState.h"
#endif // _VCF_DRAWUISTATE_H__

#ifndef _VCF_BUTTONPEER_H__
#	include "vcf/ApplicationKit/ButtonPeer.h"
#endif // _VCF_BUTTONPEER_H__


/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/

namespace VCF {

class CommandButton;

class OSXButton : public OSXControl, public ButtonPeer {
public:

	OSXButton( CommandButton* control );

	virtual ~OSXButton();
	
	virtual Image* getImage();
		
	virtual void setImage( Image* image );
		
	virtual ButtonState getState();
	
	virtual void create( Control* owningControl );
	
	virtual String getText();

    virtual void setText( const String& text );
	
protected:

	virtual OSStatus handleOSXEvent( EventHandlerCallRef nextHandler, EventRef theEvent );

	ButtonState state_;
	CommandButton* commandButton_;
};


};



/**
*CVS Log info
*$Log$
*Revision 1.1.2.1  2004/10/18 03:10:30  ddiego
*osx updates - add initial command button support, fixed rpoblem in mouse handling, and added dialog support.
*
*/

#endif //_VCF_OSXBUTTON_H__

