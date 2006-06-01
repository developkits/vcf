#ifndef _VCF_LINUXREGISTRY_H__
#define _VCF_LINUXREGISTRY_H__
//LinuxRegistry.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


namespace VCF {

class LinuxRegistry : public Object, public RegistryPeer{

public:
	LinuxRegistry();

	virtual ~LinuxRegistry();

	virtual void setRoot( const RegistryKeyType& rootKey );

	virtual RegistryKeyType getRoot();

	virtual bool openKey( const String& keyname, const bool& createIfNonExistant );

	virtual bool setValue( const String& value, const String& valuename );

	virtual bool setValue( const uint32& value, const String& valuename );

	virtual bool setValue( const bool& value, const String& valuename );

	virtual bool setValue( void* dataBuffer, const uint32& dataBufferSize, const String& valuename );

	virtual String getStringValue( const String& valuename );

	virtual uint32 getIntValue( const String& valuename );

	virtual bool getBoolValue( const String& valuename );

	virtual void getDataBufValue( const String& valuename, uint32& dataBufferSize, void** dataBuffer );

	virtual Enumerator<String>* getKeyNames();

	virtual Enumerator<RegistryValueInfo*>* getValues();

	virtual String getCurrentKey();
private:
	std::vector<RegistryValueInfo*> values_;
	std::vector<String> keys_;
	EnumeratorContainer<std::vector<RegistryValueInfo*>,RegistryValueInfo*> valuesContainer_;
	EnumeratorContainer<std::vector<String>,String> keysContainer_;
	RegistryKeyType rootKeyType_;
	String currentRegKey_;
};

}; //end of namespace VCF


/**
*CVS Log info
*$Log$
*Revision 1.1.2.4  2004/06/06 07:05:32  marcelloptr
*changed macros, text reformatting, copyright sections
*
*Revision 1.1.2.2  2004/04/29 04:07:08  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 03:29:40  ddiego
*migration towards new directory structure
*
*Revision 1.2.8.1  2004/04/26 21:58:42  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.2  2003/05/17 20:37:12  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.1.14.1  2003/03/12 03:11:02  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.1  2002/05/27 15:58:22  ddiego
*added linux peer classes for the FoundationKit port to linux
*this now means the FoundationKit will start up correctly on
*linu thought it is still not 100% functional yet
*
*/


#endif // _VCF_LINUXREGISTRY_H__

