//ValidationEvent.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


/* Generated by Together */
#include "vcf/ApplicationKit/ApplicationKit.h"


using namespace VCF;


ValidationEvent::ValidationEvent( Object* source ):
	Event( source )
{

}


ValidationEvent::~ValidationEvent()
{

}

bool ValidationEvent::isValidationSuccessful()
{
	return this->validationSuccessful_;
}

void ValidationEvent::setValidationSuccessful( const bool& validationSuccessful )
{
	this->validationSuccessful_ = validationSuccessful;
}


/**
*CVS Log info
*$Log$
*Revision 1.1.2.2  2004/04/29 03:43:15  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:20  ddiego
*migration towards new directory structure
*
*Revision 1.8  2003/05/17 20:37:19  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.7.2.3  2003/03/23 06:46:01  marcelloptr
*minor fixes
*
*Revision 1.7.2.2  2003/03/23 03:23:55  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.7.2.1  2003/03/12 03:11:57  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.7  2003/02/26 04:30:45  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.6.2.1  2002/12/27 23:04:45  marcelloptr
*Improved macros for automatic import/export of libraries. - Warning fixes. - Other Minor Changes.
*
*Revision 1.6  2002/11/18 00:46:08  ddiego
*this is the merge over of all the bug fixes and features (mostly
*VCF Builder stuff) from the devmain-0-5-8 branch
*
*Revision 1.5.16.1  2002/11/04 04:50:17  ddiego
*bug fixes are:
*void Win32MenuItem::clearChildren(), was passing in an incorrect index into DeleteMenu()
*fixed an issue when deleting a project from a workspace. This was crashing due to open
*documents with bad pointers. Closing a project now closes all open documents that are
*associated with the project
*Verified that the new file and new class and new form, and new project and checking the validity of
*entered data.
*Fixed a bug in Win32Dialog where the Frame::allowClose() virtual method was not being
*called.
*Fixed a bug in the Delegate class which would allow multiple additions of the same event handler.
*
*Revision 1.5  2002/01/24 01:46:49  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


