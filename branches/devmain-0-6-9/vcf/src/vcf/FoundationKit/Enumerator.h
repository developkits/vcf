#ifndef _VCF_ENUMERATOR_H__
#define _VCF_ENUMERATOR_H__
//Enumerator.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


/* Generated by Together */


namespace VCF{
/**
 This is a template class that provides an interface for
 iterating through a collection, and provides a standard set of methods.
 Either multiply inherited by classes that want to public expose a single set of children,
 or as  a returned object for safe enumeration of data collections.
 */

template <class COLLECTION_TYPE> class Enumerator{
protected:
	bool supportsEditing_;  /* moved to begin for MINGW */
public:
	Enumerator(){
		supportsEditing_ = false;
	};

	virtual ~Enumerator(){};

	/**
	indicates whether there are any more children to enumerate
	through
	@param bool specifies whether the enumeration should be forwards
	or backwards. The default value is true for enumerating forwards.
	@return bool true if there are any elements left to enumerate,
	otherwise returns false.
	*/
	virtual bool hasMoreElements(const bool& backward=false)=0;

	/**
	returns the next element in the enumeration. Moves the internal
	iterator forward
	@return COLLECTION_TYPE returns a COLLECTION_TYPE value of whatever type
	was specified in the template argument to the Enumerator
	*/
	virtual COLLECTION_TYPE nextElement()=0;

	/**
	returns the previous element in the enumeration. Moves the internal
	iterator backward
	@return COLLECTION_TYPE returns a COLLECTION_TYPE value of whatever type
	was specified in the template argument to the Enumerator
	*/
	virtual COLLECTION_TYPE prevElement()=0;

	/**
	Resets the Enumerator's internal iterator back to the beginning (or end,
	if backward is true).
	@param bool specifies which direction to reset to
	*/
	virtual void reset(const bool& backward=false)=0;

	/**
	Currently not used. Will allow editing of the Enumerator in the
	future, in which case this will return whether or not this instance
	of the enumerator allows adding or removing of elements
	@return bool true if the enumerator allows editing, otherwise false.
	*/
	bool supportsEditing(){
		return supportsEditing_;
	};
};



/**
\par
EnumeratorContainer is based on some COLLECTION class that represents a collection
class of some sort, i.e. std::vector<>, std::list, et al.
\par
The internal implementation counts on the collection having a common interface to
STL's collection classes.
\par
The collection must support forward and reverse iterators
\par
The COLLECTION type specifies the full collection associated with
the enumerator. 
\par
For example, an enumerator container of Object* using
a std::vector as it's collection type would look like this:
\code
*	EnumeratorContainer<std::vector<Object*>,Object*> objectVecEnumerator;
\endcode
The COLLECTION_TYPE represents the type of an individual element in the enumerator.
Passing in a reference to the collection variable in the contstructor will automatically
set up the internal iterator, nothing further need be done. Alternatively, you may
call the initContainer() method with a reference to the collection
*/
template <class COLLECTION, class COLLECTION_TYPE> class EnumeratorContainer : public Enumerator<COLLECTION_TYPE>{
public:

	#if defined(VCF_MINGW) 
	using Enumerator<COLLECTION_TYPE>::supportsEditing_; /* mingw follows the standart */
	#endif

	EnumeratorContainer( COLLECTION &container ): container_(&container) {
		reset();
		supportsEditing_ = true;
	};

	EnumeratorContainer(): /*backwards_(false),*/ container_(NULL){
		supportsEditing_ = true;
	};

	virtual ~EnumeratorContainer(){};

	/**
	Initializes the container with a reference to an
	existing collection
	*/
	virtual void initContainer( COLLECTION &container ){
		container_ = &container;
		reset();
	};

	virtual bool hasMoreElements(const bool& backward=false){
		bool result = false;
		if ( true == backward ){
			result = containerIterator_ != container_->begin();
		}
		else{
			result = containerIterator_ != container_->end();
		}
		return result;
	};

	virtual COLLECTION_TYPE nextElement(){
		if ( containerIterator_ != container_->end() ){
			return *containerIterator_++;
		}
		else {
			return *container_->end();
		}
	};

	virtual COLLECTION_TYPE prevElement(){
		if (containerIterator_ != container_->begin() ){
				return *--containerIterator_;
		}
		else {
			return *container_->begin();
		}
	};

	virtual void reset(const bool& backward=false){
		if ( backward ) {
			containerIterator_ = container_->end();
		}
		else {
			containerIterator_ = container_->begin();
		}
	};

	/**
	returns a pointer to the Enumerator interface of this
	container. Automatically resets the collection before
	returning the pointer.
	@return Enumerator<COLLECTION_TYPE> a pointer to the
	collection as an Enumerator interface.
	*/
	virtual Enumerator<COLLECTION_TYPE>* getEnumerator(){
		reset();
		return (Enumerator<COLLECTION_TYPE>*)this;
	};
private:
	typename COLLECTION::iterator containerIterator_;
	//typename COLLECTION::reverse_iterator reverseContainerIterator_;
	COLLECTION* container_;
	//bool backwards_;
};

/**
\par
EnumeratorMapContainer is based on some COLLECTION class that represents
an  associative collection of some sort, i.e. std::map<>
\par
The internal implementation counts on the collection having a common interface to
STL's collection classes.
\par
The collection must support forward and reverse iterators
\par
The COLLECTION type specifies the full collection associated with
the enumerator. 
\par
For example, an enumerator container of Object* using
a std::map as it's collection type would look like this:
\code
	EnumeratorMapContainer<std::map<String,Object*>,Object*> objectMapEnumerator;
\endcode
The COLLECTION_TYPE represents the type of an individual element in the enumerator.
In EnumeratorMapContainer the enumeration will only return the value of the maps
pair, not the key.
*/
template <class COLLECTION, class COLLECTION_TYPE> class EnumeratorMapContainer : public Enumerator<COLLECTION_TYPE>{
public:
	EnumeratorMapContainer( COLLECTION &container ): container_(&container){
		reset();
	};

	EnumeratorMapContainer(): container_(NULL){};

	virtual ~EnumeratorMapContainer(){};

	virtual void initContainer( COLLECTION &container ){
		container_ = &container;
		reset();
	};

	virtual Enumerator<COLLECTION_TYPE>* getEnumerator(){
		reset();
		return (Enumerator<COLLECTION_TYPE>*)this;
	};

	virtual bool hasMoreElements(const bool& backward=false){
		bool result = false;

		if ( true == backward ){
			result = containerIterator_ != container_->begin();
		}
		else{
			result = containerIterator_ != container_->end();
		}

		return result;
	};

	virtual COLLECTION_TYPE nextElement(){
		if (containerIterator_ != container_->end() ){
			COLLECTION_TYPE result = containerIterator_->second;
			containerIterator_ ++;
			return result;
		}
		else {
			return COLLECTION_TYPE();
		}
	};

	virtual COLLECTION_TYPE prevElement(){
		if (containerIterator_ != container_->begin() ){
			containerIterator_ --;
			return containerIterator_->second;
		}
		else {
			return container_->begin()->second;
		}
	};

	virtual void reset(const bool& backward=false){

		if ( backward ){
			containerIterator_ = container_->end();
		}
		else {
			containerIterator_ = container_->begin();
		}
	};

private:
	typename COLLECTION::iterator containerIterator_;
	COLLECTION* container_;
};


};


/**
CVS Log info
*$Log$
*Revision 1.3.2.1  2005/10/07 19:31:53  ddiego
*merged patch 1315995 and 1315991 into dev repos.
*
*Revision 1.3  2005/07/18 03:54:19  ddiego
*documentation updates.
*
Revision 1.2  2004/08/07 02:49:13  ddiego
merged in the devmain-0-6-5 branch to stable
*
Revision 1.1.2.2  2004/04/29 04:07:07  marcelloptr
reformatting of source files: macros and csvlog and copyright sections
*
Revision 1.1.2.1  2004/04/28 03:29:39  ddiego
migration towards new directory structure
*
Revision 1.10.4.1  2004/04/26 21:58:48  marcelloptr
changes for dir reorganization: _VCF_MACRO_H__
*
Revision 1.10  2003/12/18 05:15:59  ddiego
merge from devmain-0-6-2 branch into the stable branch
*
Revision 1.9.4.3  2003/10/23 04:24:51  ddiego
more musical chairs with headers again, in yet another attempt to make
them more efficent to speed up compiles.
Removed all teh template RTTI classes and put them all in one header
called VCFRTTIImpl.h. This should help compile speeds a bit.
The next step is to look at some of the event classes and remove ones
that aren't really neccessary - I'd estimate that 50% of the current
event classes are unneccessary and can be removed.
*
Revision 1.9.4.2  2003/08/19 14:18:56  ddiego
fixed a mistake in the fix of bug [ 789902 ], oops! OK now
*
Revision 1.9.4.1  2003/08/18 02:55:41  ddiego
added fix for bug [ 789902 ] EnumeratorContainer::prevElement() points to container_->end, also added an example
*
Revision 1.9  2003/05/17 20:37:16  ddiego
this is the checkin for the 0.6.1 release - represents the merge over from
the devmain-0-6-0 branch plus a few minor bug fixes
*
Revision 1.8.16.2  2003/03/23 03:23:53  marcelloptr
*3 empty lines at the end of the files
*
Revision 1.8.16.1  2003/03/12 03:11:49  ddiego
switched all member variable that used the "m_"<name> prefix to
* <name>"_" suffix nameing standard.
Also changed all vcf builder files to accomadate this.
Changes were made to the Stream classes to NOT multiple inheritance and to
be a little more correct. Changes include breaking the FileStream into two
distinct classes, one for input and one for output.
*
Revision 1.8  2002/05/09 03:10:43  ddiego
merged over code from development branch devmain-0-5-1a into the main CVS trunk
*
Revision 1.7.4.3  2002/03/26 02:35:33  ddiego
fixes include removing the __declspec(export) from the various template based
classes in the VCF. Aparrently this causes problems in VC7.
Also include a feature for memory tracking in debug mode.
*
Revision 1.7.4.2  2002/03/25 04:19:46  ddiego
fixed some file checking problems in xmake and fixed some general
code to be able to compile with GCC and Borland C++
*
Revision 1.7.4.1  2002/03/20 21:56:56  zzack
Changed Include Style of FoundationKit
*
Revision 1.7  2002/01/24 01:46:48  ddiego
added a cvs "log" comment to the top of all files in vcf/src and vcf/include
to facilitate change tracking
*
*/


#endif // _VCF_ENUMERATOR_H__


