#ifndef _VCF_LINUXPROCESSIOREDIRECTOR_H__
#define _VCF_LINUXPROCESSIOREDIRECTOR_H__
//LinuxProcessIORedirector.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


//LinuxProcessIORedirector




namespace VCF {



/**
Win32 (NT or better) implementation of ProcessIORedirectionPeer
original code swiped from:  (c) 2002 Andreas Saurwein - saurwein@uniwares.com
Any mistakes are mine due to the "port" over into the VCF.
*/
class LinuxProcessIORedirector  : public Object, public ProcessIORedirectionPeer {
public:
	LinuxProcessIORedirector();

	virtual ~LinuxProcessIORedirector();

	virtual void addOutputReadyHandler( EventHandler* eventHandler ) {
		outputReady_.addHandler( eventHandler );
	}

	virtual void removeOutputReadyHandler( EventHandler* eventHandler ) {
		outputReady_.removeHandler( eventHandler );
	}

	virtual int getProcessID() {
		return processID_;
	}

	/**
	*returns the process's main thread ID
	*/
	virtual int getProcessThreadID() {
		return processThreadID_;
	}

	/**
	*creates a new process specified by the fully
	*qualified file name in the processName argument.
	*@param String the fully qualified file name of
	*process to start. This must represent an actual
	*executable file
	*@return bool returns true if the process was successfully started
	*otherwise false.
	*/
	virtual bool createProcess( const String& processName, const String& arguments );

	/**
	*returns the fully qualified file name of the process the
	*object represents.
	*/
	virtual String getProcessFileName() {
		return commandLine_;
	}

	virtual OSHandleID getHandleID() {
		return (OSHandleID)childProcess_;
	}

	virtual void setProcess( ProcessWithRedirectedIO* process ) {
		process_ = process;
	}

	virtual ulong32 terminate();
protected:
	//this is the handle to the child process that gets started
	ulong32 childProcess_;
	ProcessWithRedirectedIO* process_;
	int processID_;
	int processThreadID_;
	String commandLine_;
	Delegate outputReady_;
};


};


/**
*CVS Log info
*$Log$
*Revision 1.2.4.1  2005/04/17 16:11:32  ddiego
*brought the foundation, agg, and graphics kits uptodate on linux
*
*Revision 1.2  2004/08/07 02:49:13  ddiego
*merged in the devmain-0-6-5 branch to stable
*
*Revision 1.1.2.4  2004/06/06 07:05:32  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 04:07:08  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 03:29:39  ddiego
*migration towards new directory structure
*
*Revision 1.6.2.1  2004/04/26 21:58:42  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.6  2004/04/03 15:48:41  ddiego
*Merged over code from the 0-6-3 branch.
*
*Revision 1.5.2.1  2004/02/16 05:34:04  ddiego
*updated linux makefiles as a result of new locale support - pushed in stubs for locale peer impl, but no functionality at this point
*
*Revision 1.5  2003/12/18 05:15:58  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.4.4.1  2003/10/02 04:50:52  ddiego
*changes to ensure the code compiles on linux. made a bunch of updates to
*the makefiles
*
*Revision 1.4  2003/05/17 20:37:12  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.3.2.2  2003/03/23 03:23:49  marcelloptr
*3 empty lines at the end of the files
*
*Revision 1.3.2.1  2003/03/12 03:10:58  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.3  2003/02/26 04:30:40  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.2.2.1  2002/12/27 23:04:39  marcelloptr
*Improved macros for automatic import/export of libraries. - Warning fixes. - Other Minor Changes.
*
*Revision 1.2  2002/11/18 00:46:08  ddiego
*this is the merge over of all the bug fixes and features (mostly
*VCF Builder stuff) from the devmain-0-5-8 branch
*
*Revision 1.1.2.1  2002/09/27 01:54:58  ddiego
*added support for bezier curves - remeber to change over to using teh MgcBeziers in Win32 too
*
*Revision 1.1.2.2  2002/09/20 21:28:44  ddiego
*added mods to support building from the VCF Builder and display the
*console build process. Also allows for double clicking in the output
*area and being able to read the output in a generic manner
*
*Revision 1.1.2.1  2002/09/17 18:48:35  ddiego
*added support in the VCF FoundationKit for redirecting io in a process
*
*/


#endif // _VCF_LINUXPROCESSIOREDIRECTOR_H__


