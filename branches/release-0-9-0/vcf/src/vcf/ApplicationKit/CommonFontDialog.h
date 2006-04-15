#ifndef _VCF_COMMONFONTDIALOG_H__
#define _VCF_COMMONFONTDIALOG_H__
//CommonFont.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {

class CommonFontDialogPeer;

class Control;

class Font;

/**
\class CommonFontDialog CommonFontDialog.h "vcf/ApplicationKit/CommonFontDialog.h"
This class is used to display a common font dialog. 
*/
class APPLICATIONKIT_API CommonFontDialog : public CommonDialog{
public:
	/**
	Creates a font dialog. If the owner is NULL then the dialog is 
	non modal. If the owner is \em not NULL then the dialog is modal
	relative to the control that started it. For most cases you 
	do \em not want to pass in a NULL owner.
	*/
	CommonFontDialog( Control* owner );

	virtual ~CommonFontDialog();

	/**
	Displays the dialog. If the user selects a font, then it 
	returns true, otherwise false.
	*/
	virtual bool execute();

	/**
	Returns the font the user selected, or NULL.
	*/
	Font* getSelectedFont();

	/**
	Sets the initial font characteristics to show when the 
	dialog is displayed. This is generally called \em before
	the execute() function is called.
	*/
	void setSelectedFont( Font* selectedFont );
private:
	CommonFontDialogPeer* peer_;
};

}; //end of namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.2  2006/04/07 02:35:22  ddiego
*initial checkin of merge from 0.6.9 dev branch.
*
*Revision 1.1.2.2  2006/03/14 02:25:46  ddiego
*large amounts of source docs updated.
*
*Revision 1.1.2.1  2005/09/02 01:01:20  ddiego
*changed some of the common dialogs around, was using a less clear class name.
*
*Revision 1.3  2004/12/01 04:31:20  ddiego
*merged over devmain-0-6-6 code. Marcello did a kick ass job
*of fixing a nasty bug (1074768VCF application slows down modal dialogs.)
*that he found. Many, many thanks for this Marcello.
*
*Revision 1.2.2.1  2004/11/21 00:19:10  ddiego
*fixed a few more res loading bugs, and added yet another resource example.
*
*Revision 1.2  2004/08/07 02:49:06  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.2  2004/04/29 03:43:12  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:15  ddiego
*migration towards new directory structure
*
*Revision 1.7.4.1  2004/04/26 21:58:18  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.7  2003/12/18 05:15:56  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.6.4.2  2003/09/21 04:15:34  ddiego
*moved the cvs info data to teh bottom of the header instead of the
*top so it is easier to get to the actual header class declarations
*instead of having to wade through all the cvs info stuff.
*
*Revision 1.6.4.1  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.6  2003/05/17 20:37:01  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.5.2.1  2003/03/12 03:09:22  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.5  2003/02/26 04:30:37  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.4.20.1  2002/12/25 22:06:16  ddiego
*whole bunch of little changes to the header files used by the ApplicationKit
*to get rid of no newline warnings by gcc.
*fixes to various event handlers in the ApplicationKit to compile with gcc
*since gcc does not like a member function pointer without the "&"
*addressof operator.
*Added initial file for the X11 UIToolkit implementation
*
*Revision 1.4  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_COMMONFONTDIALOG_H__


