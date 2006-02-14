#ifndef _VCF_HTMLBROWSERCONTROL_H__
#define _VCF_HTMLBROWSERCONTROL_H__
//HTMLBrowserControl.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


#define HTMLBROWSERCONTROL_CLASSID		"3bfb2646-a7e9-40b2-948b-9eef92db7965"


namespace VCF  {
	class HTMLAuthenticationEvent : public Event {
	public:
		inline HTMLAuthenticationEvent( Object* source, const String& url );


		String userName;
		String password;
		String url;
		bool authenticationCancelled;

		virtual Object* clone( bool deep=false ) {
			return new HTMLAuthenticationEvent(*this);
		}		
	};

	class APPLICATIONKIT_API HTMLEvent : public Event {
	public:
		HTMLEvent( Object* source, uint32 type ):Event(source,type), value(0),maxValue(0) {};

		virtual Object* clone( bool deep=false ) {
			return new HTMLEvent(*this);
		}
	
		uint32 value;
		uint32 maxValue;
		String status;
		String url;

	};


	class APPLICATIONKIT_API HTMLElementEvent : public Event {
	public:
		HTMLElementEvent( Object* source, uint32 type ):Event(source,type) {};

		virtual Object* clone( bool deep=false ) {
			return new HTMLElementEvent(*this);
		}

		String elementID;
	};

/**
*Class HTMLBrowserControl documentation
*/
class APPLICATIONKIT_API HTMLBrowserControl : public Control {
public:
	HTMLBrowserControl();
	virtual ~HTMLBrowserControl();
	
	enum HTMLPolicy {
		hpNone = 0,
		hpAllowPopups =			0x0001,
		hpAllowsScrollbars =	0x0002,
		hpAllowsTextSelection = 0x0004,
		hpUseDefaultAuthenticationUI  = 0x0008,
		hpAllowDefaultContextMenu  = 0x0010,
	};

	enum HTMLEvents{
		heBase = 'HTev',
		heURLLoadingBegun,
		heURLLoading,
		heURLLoaded,
		heURLLoadError,
		heStatusChanged,
		heNewWindowDisplayed,
		heTitleChanged,
		heAuthenticationRequested,
		heElementClicked
	};

	/**
	@delegate URLLoadingBegun
	@event HTMLEvent
	@eventtype heURLLoadingBegun
	*/
	DELEGATE( URLLoadingBegun );

	/**
	@delegate URLLoading
	@event HTMLEvent
	@eventtype heURLLoading
	*/
	DELEGATE( URLLoading );

	/**
	@delegate URLLoaded
	@event HTMLEvent
	@eventtype heURLLoaded
	*/
	DELEGATE( URLLoaded );

	/**
	@delegate URLLoadError
	@event HTMLEvent
	@eventtype heURLLoadError
	*/
	DELEGATE( URLLoadError );


	/**
	@delegate StatusChanged
	@event HTMLEvent
	@eventtype heStatusChanged
	*/
	DELEGATE( StatusChanged );

	/**
	@delegate NewWindowDisplayed
	@event HTMLEvent
	@eventtype heNewWindowDisplayed
	*/
	DELEGATE( NewWindowDisplayed );

	/**
	@delegate TitleChanged
	@event HTMLEvent
	@eventtype heTitleChanged
	*/
	DELEGATE( TitleChanged );
	
	/**
	@delegate AuthenticationRequested
	@event HTMLAuthenticationEvent
	@eventtype heAuthenticationRequested
	*/
	DELEGATE( AuthenticationRequested );


	virtual void paint( GraphicsContext* ctx );

	virtual void afterCreate( ComponentEvent* e );

	String getCurrentURL();

	void setCurrentURL( const String& url );

	void goBack();

	void goForward();

	void goHome();

	void refresh();

	bool isLoadingURL();

	void stopLoadingURL();

	void setFromHTML( const String& html );

	String getTitle();

	void edit( const bool& val );

	void copy();

	void selectAll();

	bool getAllowsPopupWindows() {
		return (policyState_ & hpAllowPopups) ? true : false;
	}

	void setAllowsPopupWindows( bool val );

	bool getAllowsScrollbars() {
		return (policyState_ & hpAllowsScrollbars) ? true : false;
	}

	void setAllowsScrollbars( bool val );

	bool getAllowsTextSelection() {
		return (policyState_ & hpAllowsTextSelection) ? true : false;
	}	

	void setAllowsTextSelection( bool val );

	bool getUseDefaultAuthenticationUI() {
		return (policyState_ & hpUseDefaultAuthenticationUI) ? true : false;
	}

	void setUseDefaultAuthenticationUI( bool val ) {
		if ( val ) {
			policyState_ |= hpUseDefaultAuthenticationUI;
		}
		else {
			policyState_ &= ~hpUseDefaultAuthenticationUI;
		}		
	}

	bool getAllowDefaultContextMenu() {
		return (policyState_ & hpAllowDefaultContextMenu) ? true : false;
	}

	void setAllowDefaultContextMenu( bool val ) {
		if ( val ) {
			policyState_ |= hpAllowDefaultContextMenu;
		}
		else {
			policyState_ &= ~hpAllowDefaultContextMenu;
		}		
	}

	String getActiveElementID();

	String getElementIDFromPoint( Point* pt );

	String getElementHTMLText( const String& elementName );

	void setElementHTMLText( const String& elementName, const String& html );

	String getElementText( const String& elementName );

	void setElementText( const String& elementName, const String& text );	
	
	/**
	Sets one of the more common event handlers for an element. Some elements
	may not respond to this.
	This will fire a HTMLElementEvent instance.
	*/
	bool setElementClickedEventHandler( const String& elementName, EventHandler* handler );


protected:
	HTMLBrowserPeer* browserPeer_;
	uint32 policyState_;

private:
};



HTMLAuthenticationEvent::HTMLAuthenticationEvent( Object* source, const String& aUrl ):
	Event( source, HTMLBrowserControl::heAuthenticationRequested),
	url(aUrl),
	authenticationCancelled(false)
{

}


}; //end of namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.2.6.3  2006/02/14 05:13:09  ddiego
*more browser updates.
*
*Revision 1.2.6.2  2006/02/13 22:11:59  ddiego
*added further html support and better browser example code.
*
*Revision 1.2.6.1  2006/02/13 05:10:32  ddiego
*added better html browser support.
*
*Revision 1.2  2004/08/07 02:49:08  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.3  2004/06/06 07:05:30  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 03:43:13  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:17  ddiego
*migration towards new directory structure
*
*Revision 1.4.4.1  2004/04/26 21:58:20  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.4  2003/12/18 05:15:57  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.3.4.3  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.3.4.2  2003/09/06 19:45:45  ddiego
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
*Revision 1.3.4.1  2003/09/05 21:03:38  ddiego
*moved the various control destructor's to protected scope
*migrated code into destroy() overridden methods
*----------------------------------------------------------------------
*
*Revision 1.3  2003/05/17 20:37:03  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.2.22.1  2003/03/12 03:09:34  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.2  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_HTMLBROWSERCONTROL_H__


