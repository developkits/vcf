

/*
Copyright 2000-2007 The VCF Project, Jim Crafton.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/ApplicationKit/ApplicationKit.h"
#include "vcf/ApplicationKit/ApplicationKitPrivate.h"
#include "vcf/ApplicationKit/XCBAbstractControl.h"
#include "vcf/ApplicationKit/XCBButton.h"
#include "vcf/ApplicationKit/CommandButton.h"


using namespace VCF;

XCBButton::XCBButton( CommandButton* button ):
    XCBAbstractControl(button),
    btnImage_(NULL)
{

}

XCBButton::~XCBButton()
{

}

Image* XCBButton::getImage()
{
    return btnImage_;
}

void XCBButton::setImage( Image* image )
{

}

ButtonState XCBButton::getState()
{
    return btnState_;
}

void XCBButton::paintPeer( xcb_connection_t &connection, const xcb_expose_event_t& event, GraphicsContext* sharedCtx )
{
    btnState_.setFocused( control_->isFocused() );
    btnState_.setEnabled( isEnabled() );

    XCBAbstractControl::paintPeer( connection, event, sharedCtx );
}

void XCBButton::handleMouseEvents(xcb_connection_t &connection, const xcb_generic_event_t& event)
{
    switch ( event.response_type ) {
		case XCB_BUTTON_PRESS : {
			const xcb_button_press_event_t* ev = (const xcb_button_press_event_t*)&event;
printf( "XCB_BUTTON_PRESS\n" );
			btnState_.setPressed( true );
		}
		break;

		case XCB_BUTTON_RELEASE : {
			const xcb_button_release_event_t* ev = (const xcb_button_release_event_t*)&event;
printf( "XCB_BUTTON_RELEASE\n" );
			//btnState_.setPressed( false );


		}
		break;

		case XCB_MOTION_NOTIFY : {
			const xcb_motion_notify_event_t* ev = (const xcb_motion_notify_event_t*)&event;
		}
		break;

		case XCB_ENTER_NOTIFY : {
			const xcb_enter_notify_event_t* ev = (const xcb_enter_notify_event_t*)&event;
		}
		break;

		case XCB_LEAVE_NOTIFY : {
			const xcb_leave_notify_event_t* ev = (const xcb_leave_notify_event_t*)&event;
		}
		break;
	}
}
