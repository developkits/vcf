#ifndef _VCF_LOCK_H__
#define _VCF_LOCK_H__
//Lock.h

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
*Class Lock
*@author Tiziano Franzoi, Nov 15, 2001
*/
class FOUNDATIONKIT_API Lock :public VCF::Object
{
public:
	Lock( VCF::SynchObject& synchObject ):synchObject_(synchObject)
	{
		validSynchObject_ = synchObject_.lock();
	}


	virtual ~Lock()
	{
		if( true == validSynchObject_ )
		{
			synchObject_.unlock();
		}
	}
protected:
	bool validSynchObject_;
	VCF::SynchObject& synchObject_;

private:
	/**
	* we forbid to copy a lock, as it would probably
	* cause some trouble.
	* Plus it would be impossible to define an explicit 
	* assignment operator with a reference. With this
	* the CodeWarrior doesn't complain anymore.
	*/
	Lock& operator = ( const Lock& lock ) { return *this; };

};
} // end of namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.2.4.1  2004/12/20 23:48:07  marcelloptr
*forbidden assignment operator for the Lock class
*
*Revision 1.2  2004/08/07 02:49:13  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.3  2004/06/06 07:05:32  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 04:07:08  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 03:29:40  ddiego
*migration towards new directory structure
*
*Revision 1.7.4.1  2004/04/26 21:58:48  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.7  2003/12/18 05:15:59  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.6.4.1  2003/10/23 04:24:51  ddiego
*more musical chairs with headers again, in yet another attempt to make
*them more efficent to speed up compiles.
*Removed all teh template RTTI classes and put them all in one header
*called VCFRTTIImpl.h. This should help compile speeds a bit.
*The next step is to look at some of the event classes and remove ones
*that aren't really neccessary - I'd estimate that 50% of the current
*event classes are unneccessary and can be removed.
*
*Revision 1.6  2003/05/17 20:37:17  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.5.16.2  2003/03/23 03:23:53  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.5.16.1  2003/03/12 03:11:50  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.5  2002/05/09 03:10:43  ddiego
*merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
*Revision 1.4.4.1  2002/03/20 21:56:56  zzack
*Changed Include Style of FoundationKit
*
*Revision 1.4  2002/01/24 01:46:49  ddiego
*added a cvs "log" comment to the top of all files in vcf/src and vcf/include
*to facilitate change tracking
*
*/


#endif // _VCF_LOCK_H__


