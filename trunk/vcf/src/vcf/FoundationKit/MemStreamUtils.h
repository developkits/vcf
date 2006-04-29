#ifndef _VCF_MEMSTREAMUTILS_H__
#define _VCF_MEMSTREAMUTILS_H__
//MemStreamUtils.h

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#if _MSC_VER > 1000
#   pragma once
#endif


namespace VCF {
/**
\class GenericMemoryBuffer MemStreamUtils.h "vcf/FoundationKit/MemStreamUtils.h"
GenericMemoryBuffer
@author  Jim Crafton
@date 3/7/2000
@version 1.0
*/

template <class DATA_TYPE>
class GenericMemoryBuffer {
public:
	GenericMemoryBuffer( const ulong32 allocationSize = 65536) :
			allocationSize_(allocationSize),
			currentBuffer_(NULL),
			currentAllocationSize_(0),
			seekPos_(0),
			currentSize_(0)
			{}



	virtual ~GenericMemoryBuffer(){
		delete [] currentBuffer_;
		currentBuffer_ = NULL;
	}

	inline DATA_TYPE* getBuffer()	{
		return currentBuffer_;
	}

	inline ulong32 getSize()  {
		return currentSize_;
	}

	inline unsigned long write( const unsigned char* buffer, const ulong32& sizeOfBuffer ) {
		if ( currentAllocationSize_ < (seekPos_ + sizeOfBuffer) ) {
			allocate( sizeOfBuffer );
		}

		char* tmp = (char*)currentBuffer_;
		tmp += seekPos_;

		memcpy( tmp, buffer, sizeOfBuffer );

		seekPos_ += sizeOfBuffer;

		if ( seekPos_ > currentSize_ ) {
			currentSize_ += sizeOfBuffer;
		}

		return sizeOfBuffer;
	}

	inline unsigned long read( unsigned char* buffer, const ulong32& sizeOfBuffer ) {
		if ( currentAllocationSize_ < (seekPos_ + sizeOfBuffer) ) {
			throw NoFreeMemException("No longer have enough memory in this buffer to read from.");
		}

		char* tmp = (char*)currentBuffer_;
		tmp += seekPos_;

		memcpy( buffer, tmp, sizeOfBuffer );

		seekPos_ += sizeOfBuffer;

		return sizeOfBuffer;
	}

	inline void setSeekPos( ulong32 pos ) {
		seekPos_ = pos;
	}

	inline ulong32 getSeekPos() {
		return seekPos_;
	}


	inline void clear() {

		currentAllocationSize_ = 0;
		delete [] currentBuffer_;
		currentBuffer_ = NULL;
		allocate(allocationSize_);
	}
protected:
	inline void allocate( ulong32 sizeToAllocate ) {
		ulong32 oldSize = currentAllocationSize_;

		sizeToAllocate = (sizeToAllocate - oldSize);

		currentAllocationSize_ += ((sizeToAllocate/allocationSize_) * allocationSize_) +
			( (sizeToAllocate % allocationSize_) ? allocationSize_ : 0 );

		DATA_TYPE* tmp = new DATA_TYPE[currentAllocationSize_];

		if ( NULL == tmp ) {
			throw NoFreeMemException();
		}

		if ( NULL != currentBuffer_ ) {
			//copy over the old bits
			memcpy( tmp, currentBuffer_, oldSize );
			delete [] currentBuffer_;
		}

		currentBuffer_ = tmp;
	}
private:
	ulong32 allocationSize_;
	DATA_TYPE* currentBuffer_;
	ulong32 currentAllocationSize_;
	ulong32 seekPos_;
	ulong32 currentSize_;
};




/**
\class CharMemStream MemStreamUtils.h "vcf/FoundationKit/MemStreamUtils.h"
*/
class FOUNDATIONKIT_API CharMemStream : public GenericMemoryBuffer<char> {
public:
	CharMemStream( const ulong32& allocationSize = 65536 ) :
		GenericMemoryBuffer<char> ( allocationSize ) {
	}

	virtual ~CharMemStream(){};

};

}; //end of namespace VCF


#endif // _VCF_MEMSTREAMUTILS_H__

/**
$Id$
*/
