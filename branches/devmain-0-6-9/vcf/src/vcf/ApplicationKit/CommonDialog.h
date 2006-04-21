#ifndef _VCF_COMMONDIALOG_H__
#define _VCF_COMMONDIALOG_H__
//CommonDialog.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {

/**
\class CommonDialog CommonDialog.h "vcf/ApplicationKit/CommonDialog.h"
Base class for all the standard, or "common" dialog classes. A common
dialog is one like a color dialog, or font selection dialog or 
open or save file dialog. These are generally provided by the 
windowing system, and these classes just provide convenient 
wrappers around them.

To display any common dialog, you simply call the execute() function.
This funtion will return true if the user chose something, like a 
font, color, or filename, and false if the user canceled the dialog.

*/
class APPLICATIONKIT_API CommonDialog : public Component {
public:

	CommonDialog(){};

	virtual ~CommonDialog(){};

	/**
	"Executes" or displays the dialog.
	@return bool returns true if the user made a valid selection,
	otherwise false.
	*/
	virtual bool execute() = 0;

	/**
	* Special initializations may then happen here.
	*@see Component::afterCreate()
	*/
	virtual void afterCreate( ComponentEvent* event ){};
};

}; // namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.3.2.2  2006/03/26 22:37:34  ddiego
*minor update to source docs.
*
*Revision 1.3.2.1  2006/03/14 02:25:46  ddiego
*large amounts of source docs updated.
*
*Revision 1.3  2005/07/09 23:14:52  ddiego
*merging in changes from devmain-0-6-7 branch.
*
*Revision 1.2.4.1  2005/04/09 17:20:35  marcelloptr
*bugfix [ 1179853 ] memory fixes around memset. Documentation. DocumentManager::saveAs and DocumentManager::reload
*
*Revision 1.2  2004/08/07 02:49:06  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.4  2004/07/22 04:18:58  ddiego
*fixed bug 995642 delete LoalePeer in Locale, and added some miscellaneous changes to the QTPlayer. Also fixing (not finished yet) a bug that
*prevents the TreePeer from being properly notified when the tree model's
*item is deleted.
*
*Revision 1.1.2.3  2004/06/06 07:05:29  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 03:43:12  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:15  ddiego
*migration towards new directory structure
*
*Revision 1.8.4.1  2004/04/26 21:58:18  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.8  2003/12/18 05:15:56  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.7.4.2  2003/09/17 03:27:56  ddiego
*added a fix for bug [ 807416 ] CommonFileDialog needs setFileExtension
*added methods for getting data from the open and save dialogs in the
*DocumentManager
*
*Revision 1.7.4.1  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.7  2003/05/17 20:37:01  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.6.2.1  2003/03/23 03:23:45  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.6  2003/02/26 04:30:37  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.5.20.1  2002/12/25 22:06:15  ddiego
*whole bunch of little changes to the header files used by the ApplicationKit
*to get rid of no newline warnings by gcc.
*fixes to various event handlers in the ApplicationKit to compile with gcc
*since gcc does not like a member function pointer without the "&"
*addressof operator.
*Added initial file for the X11 UIToolkit implementation
*
*Revision 1.5  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_COMMONDIALOG_H__

