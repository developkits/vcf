#ifndef _VCF_MENUITEMSHORTCUT_H__
#define _VCF_MENUITEMSHORTCUT_H__
//MenuItemShortcut.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {

class MenuItem;

class MenuItemShortcut : public Object {
public:
	MenuItemShortcut( MenuItem* item );

	virtual ~MenuItemShortcut();

	/**
	*
	*/
	void setShortcutKey( uint32 keyMask );

	/**
	*
	*/
	void setShortcutCharacter( const VCFChar& shortcutChar );
private:
	MenuItem* menuItem_;
};

};


/**
*CVS Log info
*$Log$
*Revision 1.1.2.2  2004/04/29 03:43:14  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:18  ddiego
*migration towards new directory structure
*
*Revision 1.6.8.1  2004/04/26 21:58:20  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.6  2003/05/17 20:37:03  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.5.2.1  2003/03/12 03:09:36  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.5  2003/02/26 04:30:38  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.4.20.1  2003/01/08 00:19:43  marcelloptr
*mispellings and newlines at the end of all source files
*
*Revision 1.4  2002/01/24 01:46:47  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_MENUITEMSHORTCUT_H__


