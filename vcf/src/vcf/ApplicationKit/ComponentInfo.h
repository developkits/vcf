#ifndef _VCF_COMPONENTINFO_H__
#define _VCF_COMPONENTINFO_H__
//ComponentInfo.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {

class Image;

/**
*ComponentInfo represents special data regarding a component's class.
*This information is generally used by development tools to aid in things
*like autogenerating code, for example.
*/
class APPKIT_API ComponentInfo : public Object {
public :
	ComponentInfo( const String& componentUUID, const String& author="", const String& company="",
					const String& copyright="", const String& additionalInfo="",
					const String& packageName="", const String& packageUUID="",
					Image* componentImage=NULL );

	virtual ~ComponentInfo();

	/**
	*The name of the author of the component class
	*/
	String getAuthor() {
		return author_;
	}

	/**
	*the copyright, if any, for the components source code
	*/
	String getCopyright() {
		return copyright_;
	}

	/**
	*The name of the company responsible for the component
	*/
	String getCompany() {
		return company_;
	}

	/**
	*any additional information that can be rendered as text.
	*/
	String getAdditionalInfo() {
		return additionalInfo_;
	}

	/**
	*the UUID that represents the corresponding component class.
	*This UUID is guaranteed to be able to create a valid instance
	*of the component at runtime.
	*/
	String getComponentUUID() {
		return componentUUID_;
	}

	/**
	returns the name of the package (VPL) that this component belongs to
	*/
	String getPackageName() {
		return packageName_;
	}

	/**
	returns the package UUID, that is, the unique string id for the package
	that this component belongs to. The package anme and package UUID
	<b>must</b> refer to the same package!
	*/
	String getPackageUUID() {
		return packageUUID_;
	}

	/**
	*an image that represents the component. This image must have
	*dimensions of 25 pixels wide by 25 pixel high.
	*@return Image a pointer to the image representing the component. May
	*be NULL if no image exists. In this case a default image will be used.
	*/
	Image* getComponentImage() {
		return componentImage_;
	}

	/**
	*returns an enumeration of all the headers
	*that are required to successfully compile an implementation file
	*that uses this component.
	*/
	Enumerator<String>* getRequiredHeaders();

	/**
	*returns an enumeration of all the C++ namespaces
	*that are required to successfully compile an implementation file
	*that uses this component.
	*/
	Enumerator<String>* getRequiredNamespaces();

	void addHeader( const String& header );

	void removeHeader( const String& header );

	void removeAllHeaders();

	void addNamespace( const String& aNamespace );

	void removeNamespace( const String& aNamespace );

	void removeAllNamespaces();


protected:
	String author_;
	String copyright_;
	String company_;
	String additionalInfo_;
	String componentUUID_;
	String packageName_;
	String packageUUID_;
	Image* componentImage_;


	std::vector<String> requiredHeaders_;
	EnumeratorContainer<std::vector<String>,String> reqHeadersContainer_;

	std::vector<String> requiredNamespaces_;
	EnumeratorContainer<std::vector<String>,String> reqNamespacesContainer_;
};


};


/**
*CVS Log info
*$Log$
*Revision 1.1.2.2  2004/04/29 03:43:13  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*Revision 1.1.2.1  2004/04/28 00:28:15  ddiego
*migration towards new directory structure
*
*Revision 1.9.4.1  2004/04/26 21:58:18  marcelloptr
*changes for dir reorganization: _VCF_MACRO_H__
*
*Revision 1.9  2003/12/18 05:15:56  ddiego
*merge from devmain-0-6-2 branch into the stable branch
*
*Revision 1.8.2.2  2003/09/21 04:15:34  ddiego
*moved the cvs info data to teh bottom of the header instead of the
*top so it is easier to get to the actual header class declarations
*instead of having to wade through all the cvs info stuff.
*
*Revision 1.8.2.1  2003/09/12 00:09:32  ddiego
*added better tabbing behaviour - fixed an earlier bug.
*Code is easier to understand and it simplified the implementation
*a bit as well
*Moved around the RTTI definitions for various Appkit classes to a
*single inline and stripped them from the various class headers
*
*Revision 1.8  2003/08/09 02:56:42  ddiego
*merge over from the devmain-0-6-1 branch
*Changes
*Features:
*-Added additional implementation to better support the MVC architecture in
*the VCF
*
*-Added a Document/View architecure that is similar to MFC's or NextSteps's
*Doc/View architectures
*
*-Integrated the Anti Grain Graphics library into the GraphicsKit. There is
*now basic support for it in terms of drawing vector shapes
*(fills and strokes). Image support will come in the next release
*
*-Added several documented graphics tutorials
*
*Bugfixes:
*
*[ 775744 ] wrong buttons on a dialog
*[ 585239 ] Painting weirdness in a modal dialog ?
*[ 585238 ] Modal dialog which makes a modal Dialog
*[ 509004 ] Opening a modal Dialog causes flicker
*[ 524878 ] onDropped not called for MLTcontrol
*
*Plus an issue with some focus and getting the right popup window to activate
*has also been fixed
*
*Revision 1.7.2.1  2003/05/25 19:07:09  ddiego
*fixed bug [ 524878 ] onDropped not called for MLTcontrol. This
*was due to errors in the drag-drop implementation for WIn32 and is
*now fixed.
*Also cleaned up the drag-drop implementation and moved/deleted a number of
*irrelevant files for COM support. The vcf/include/com and vcf/src/COM
*directories are now gotten rid of.
*
*Revision 1.7  2003/05/17 20:37:01  ddiego
*this is the checkin for the 0.6.1 release - represents the merge over from
*the devmain-0-6-0 branch plus a few minor bug fixes
*
*Revision 1.6.2.1  2003/03/12 03:09:24  ddiego
*switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
*Also changed all vcf builder files to accomadate this.
*Changes were made to the Stream classes to NOT multiple inheritance and to
*be a little more correct. Changes include breaking the FileStream into two
*distinct classes, one for input and one for output.
*
*Revision 1.6  2003/02/26 04:30:37  ddiego
*merge of code in the devmain-0-5-9 branch into the current tree.
*most additions are in the area of the current linux port, but the major
*addition to this release is the addition of a Condition class (currently
*still under development) and the change over to using the Delegate class
*exclusively from the older event handler macros.
*
*Revision 1.5.20.1  2002/12/25 22:06:17  ddiego
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


#endif // _VCF_COMPONENTINFO_H__


