#ifndef _VCF_DROPTARGETPEER_H__
#define _VCF_DROPTARGETPEER_H__
//DropTargetPeer.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


/**
*DropTargetPeer interface
*/



namespace VCF
{

class DropTarget;

class Control;

/**
\class DropTargetPeer DropTargetPeer.h "vcf/ApplicationKit/DropTargetPeer.h"
*/
class APPLICATIONKIT_API DropTargetPeer {
public:
	virtual ~DropTargetPeer(){};

	virtual void registerTarget( Control* control )=0;

	virtual void unregisterTarget( Control* control )=0;

	virtual DropTarget* getDropTarget()=0;

	virtual void setDropTarget( DropTarget* dropTarget )=0;
};

};


/**
*CVS Log info
*$Log$
*Revision 1.3  2006/04/07 02:35:23  ddiego
*initial checkin of merge from 0.6.9 dev branch.
*
*Revision 1.2.6.1  2006/03/14 02:25:47  ddiego
*large amounts of source docs updated.
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
*Revision 1.5.4.1  2004/04/26 21:58:41  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.5  2003/12/18 05:15:58  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.4.4.2  2003/09/21 04:15:35  ddiego
*moved the cvs info data to teh bottom of the header instead of the
*top so it is easier to get to the actual header class declarations
*instead of having to wade through all the cvs info stuff.
*
*Revision 1.4.4.1  2003/09/12 00:09:33  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.4  2003/05/17 20:37:11  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.3.2.1  2003/03/23 03:23:49  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.3  2003/02/26 04:30:40  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.2.14.1  2002/12/25 22:06:29  ddiego
*whole bunch of little changes to the header files used by the ApplicationKit
*to get rid of no newline warnings by gcc.
*fixes to various event handlers in the ApplicationKit to compile with gcc
*since gcc does not like a member function pointer without the "&"
*addressof operator.
*Added initial file for the X11 UIToolkit implementation
*
*Revision 1.2  2002/05/09 03:10:43  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.1.2.1  2002/05/01 01:19:50  ddiego
*added remaining new files from switching stuff around
*like the peer classes and some of the drag drop stuff
*
*Revision 1.4  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_DROPTARGETPEER_H__


