#ifndef _VCF_BADMODELSTATEEXCEPTION_H__
#define _VCF_BADMODELSTATEEXCEPTION_H__
//BadModelStateException.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF
{

/**
*Throw when a model state has changed and it is determined that the change makes the
*model corrupt or invalid. This error is probably recoverable.
*
*@version 1.0
*author Jim Crafton
*/
class APPLICATIONKIT_API BadModelStateException : public VCF::BasicException {
public:
	BadModelStateException( const VCF::String & message ):
		BasicException( message ){};

	BadModelStateException():
		BasicException( BAD_MODEL_STATE_EXCEPTION ){};

	BadModelStateException( const VCF::String & message, const int lineNumber ):
		VCF::BasicException(message, lineNumber){};

	virtual ~BadModelStateException() throw() {};

};

};


/**
*CVS Log info
*$Log$
*Revision 1.2  2004/08/07 02:49:05  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.3  2004/06/06 07:05:29  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 03:43:12  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*/


#endif // _VCF_BADMODELSTATEEXCEPTION_H__


