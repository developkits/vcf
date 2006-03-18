#ifndef _VCF_DROPTARGET_H__
#define _VCF_DROPTARGET_H__
//DropTarget.h

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

class Control;

class DropTargetPeer;

/**
The DropTarget class represents a way to control and be notified when a
something is "dropped" ( or just "dragged over" ) as a result of 
a drag-drop operation.
In this way, for example, the mouse cursor can be modified according to
the control is over.

A drop target works by being registered with a control. This registration is
accomplished by calling the addTargetControl() on the drop target and passing
in a control instance. For example:
\code
TextControl* textControl = ...//get the control from somewhere
Panel* somePanel = ...//get the panel from somewhere

DropTarget dropTarget;
dropTarget.addTargetControl( textControl );
dropTarget.addTargetControl( somePanel );

\endcode

@delegates
	@del DropTarget::DropTargetDraggingOver
	@del DropTarget::DropTargetDropped
	@del DropTarget::DropTargetLeft
	@del DropTarget::DropTargetEntered
*/
class APPLICATIONKIT_API DropTarget : public Component {
public:

	enum {
		DRAGGING_OVER =  CUSTOM_EVENT_TYPES + 303,
		DRAG_DROPPED,
		DRAG_LEFT,
		DRAG_ENTERED
	};

    DropTarget( Control * control );

	DropTarget();

	DropTarget( Component* owner );

	DropTarget( const String& name, Component* owner );

	DropTarget( const String& name );


	virtual ~DropTarget();

	/**
	@delegate DropTargetDraggingOver is fired by the underlying windowing system
	when a dataobject is being dragged over a registered drop target.
	@event DropTargetEvent
	@eventtype DropTarget::DRAGGING_OVER
	*/
	DELEGATE(DropTargetDraggingOver)

	/**
	@delegate DropTargetDropped this is fired by the windowing system when something
	is "dropped" on a control registered with the drop target
	@event DropTargetEvent
	@eventtype
	*/
	DELEGATE(DropTargetDropped)

	/**
	@delegate DropTargetLeft is fired by the underlying windowing system
	when the "something" being dragged has left the control registered with this
	drop target.
	@event DropTargetEvent
	@eventtype DropTarget::DRAG_LEFT
	*/
	DELEGATE(DropTargetLeft)

	/**
	@delegate DropTargetEntered this is fired by the underlying windowing system
	when something that is being dragged first enters the control registered with
	this drop target.
	@event DropTargetEvent
	@eventtype DropTarget::DRAG_ENTERED
	*/
	DELEGATE(DropTargetEntered)

	virtual void handleEvent( Event* event );

	DropTargetPeer* getDropTargetPeer();

	/**
	gets the first control in the target collection.
	Typically a DropTarget will just have a single control
	that it's associated with (though it may certainly have
	more than one).
	*/
	Control* getTarget();

	/**
	Returns all the control targets for this drop
	target.
	*/
	Enumerator<Control*>* getTargets();

	/**
	Adds a new target control to the drop target's collection
	and registers it.
	*/
	void addTargetControl( Control* control );

	void removeTargetControl( Control* control );
protected:
	DropTargetPeer* dropTargetPeer_;
	std::vector<Control*> targets_;
	EnumeratorContainer<std::vector<Control*>,Control*> targetContainer_;
};

};


/**
*CVS Log info
*$Log$
*Revision 1.4.2.1  2006/03/18 22:17:42  ddiego
*removed par tag for doxygen comments as its not needed and
*screws up the doc formatting.
*
*Revision 1.4  2005/07/18 03:54:19  ddiego
*documentation updates.
*
*Revision 1.3  2004/12/01 04:31:21  ddiego
*merged over devmain-0-6-6 code. Marcello did a kick ass job
*of fixing a nasty bug (1074768VCF application slows down modal dialogs.)
*that he found. Many, many thanks for this Marcello.
*
*Revision 1.2.2.1  2004/11/07 19:32:19  marcelloptr
*more documentation
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
*Revision 1.1.2.1  2004/04/28 00:28:16  ddiego
*migration towards new directory structure
*
*Revision 1.10.4.1  2004/04/26 21:58:36  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.10  2003/12/18 05:15:57  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.9.4.6  2003/12/13 15:56:08  ddiego
**** empty log message ***
*
*Revision 1.9.4.5  2003/12/08 05:05:28  ddiego
*added a bunch more documentation to classes, and added support for new doc
*keywords (@delegates, @del, @delegate, @event, and @eventtype). This
*makes it easier to see at a glance what events a class fires off.
*Added finishing touches for the Action implementation and have a partially
*complete example for this checked in.
*
*Revision 1.9.4.4  2003/09/22 01:48:03  ddiego
*some minor additions ot teh DropTarget to allow it to have multiple
*control targets
*also a few other misc fixes
*
*Revision 1.9.4.3  2003/09/21 04:15:34  ddiego
*moved the cvs info data to teh bottom of the header instead of the
*top so it is easier to get to the actual header class declarations
*instead of having to wade through all the cvs info stuff.
*
*Revision 1.9.4.2  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.9.4.1  2003/09/02 02:11:14  ddiego
*fixed some glitches in drag drop. also cleaned up the drag drop event
*classes. Began some fixes to the html browser implementation on Win32
*
*Revision 1.9  2003/05/17 20:37:06  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.8.2.2  2003/03/23 03:23:46  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.8.2.1  2003/03/12 03:09:50  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.8  2003/02/26 04:30:38  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.7.20.2  2003/02/24 05:42:18  ddiego
*moved the code for the VariantData calss into it's own header
*migrated to the new event style using Delegates instead of relying on
*the ugly macros that we were using before - same functionality though
*made sure everything still works and compiles, including neccessary
*changes in the VCF Builder so that it creates code in the new style
*This changes to teh new style of using Delegates completes task 58837
*
*Revision 1.7.20.1  2002/12/25 22:06:21  ddiego
*whole bunch of little changes to the header files used by the ApplicationKit
*to get rid of no newline warnings by gcc.
*fixes to various event handlers in the ApplicationKit to compile with gcc
*since gcc does not like a member function pointer without the "&"
*addressof operator.
*Added initial file for the X11 UIToolkit implementation
*
*Revision 1.7  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_DROPTARGET_H__


