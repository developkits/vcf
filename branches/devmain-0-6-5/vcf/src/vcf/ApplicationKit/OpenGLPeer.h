#ifndef _VCF_OPENGLPEER_H__
#define _VCF_OPENGLPEER_H__
//OpenGLPeer.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {

class OpenGLPeer {

public:
	OpenGLPeer(){};

	virtual ~OpenGLPeer(){};

	virtual void initGL() = 0;
	virtual void swapBuffers() = 0;

	virtual void makeCurrent() = 0;
};

}; //end of namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.1.2.2  2004/04/29 03:43:14  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:18  ddiego
*migration towards new directory structure
*
*Revision 1.8.8.1  2004/04/26 21:58:41  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.8  2003/05/17 20:37:11  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.7.10.1  2003/03/23 03:23:49  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.7  2002/09/12 03:26:04  ddiego
*merged over the changes from the devmain-0-5-5b branch
*
*Revision 1.6.12.1  2002/08/06 01:21:01  ddiego
*removed Contextpeer.h because the file name was misspelled, and naturally Win32
*is NOT case sensitive
*
*Revision 1.6  2002/01/24 01:46:48  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_OPENGLPEER_H__


