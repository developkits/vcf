//BasicInputStream.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/FoundationKit/FoundationKit.h"
using namespace VCF;

BasicInputStream::BasicInputStream()
{
	init();
}

BasicInputStream::BasicInputStream( const String& textBuffer )
{
	init();
	/**
	JC
	WARNING !!!!
	We are treating this like ascii strings!!!
	*/
	inStream_.write( (const unsigned char*)textBuffer.ansi_c_str(), textBuffer.size() );
	inStream_.setSeekPos( 0 );
	totalStreamSize_ = textBuffer.size();
}

BasicInputStream::BasicInputStream( const char* dataBuffer, const unsigned long& dataBufferSize )
{
	init();
	inStream_.write( (const unsigned char*)dataBuffer, dataBufferSize );
	inStream_.setSeekPos( 0 );
	totalStreamSize_ = dataBufferSize;
}

BasicInputStream::BasicInputStream( InputStream* inStream )
{
	init();
	inputStream_ = inStream;
}


void BasicInputStream::init()
{
	inputStream_ = NULL;
	totalStreamSize_ = 0;
}

BasicInputStream::~BasicInputStream()
{

}

void BasicInputStream::seek(const unsigned long& offset, const SeekType& offsetFrom)
{
	if ( NULL != inputStream_ ) {
		inputStream_->seek( offset, offsetFrom );
	}

	switch ( offsetFrom ) {
		case stSeekFromStart: {
			inStream_.setSeekPos( offset );
		}
		break;

		case stSeekFromEnd: {
			inStream_.setSeekPos( inStream_.getSize() - offset );
		}
		break;

		case stSeekFromRelative: {
			inStream_.setSeekPos( inStream_.getSeekPos() + offset );
		}
		break;
	}
}

unsigned long BasicInputStream::getSize()
{
	return inStream_.getSize();
}

char* BasicInputStream::getBuffer()
{
	return inStream_.getBuffer();
}

unsigned long BasicInputStream::read( unsigned char* bytesToRead, unsigned long sizeOfBytes )
{
	unsigned long result = 0;
	if ( NULL != inputStream_ ){
		result = inputStream_->read( bytesToRead, sizeOfBytes );
		inStream_.write( bytesToRead, sizeOfBytes );
	}
	else {
		result = inStream_.read( bytesToRead, sizeOfBytes );
	}

	return result;
}

ulong32 BasicInputStream::getCurrentSeekPos()
{
	if ( NULL != inputStream_ ) {
		return inputStream_->getCurrentSeekPos();
	}
	return inStream_.getSeekPos();
}

bool BasicInputStream::isEOS()
{
	if ( NULL != inputStream_ ) {
		return inputStream_->getCurrentSeekPos() == (inputStream_->getSize()-1);
	}
	return inStream_.getSeekPos() == (totalStreamSize_-1);
}


/**
$Id$
*/
