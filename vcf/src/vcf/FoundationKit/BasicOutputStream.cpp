//BasicOutputStream.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/FoundationKit/FoundationKit.h"
using namespace VCF;


BasicOutputStream::BasicOutputStream()
{
	init();
}

BasicOutputStream::~BasicOutputStream()
{

}

BasicOutputStream::BasicOutputStream( OutputStream* outStream )
{
	init();
	outputStream_ = outStream;
}

void BasicOutputStream::init()
{
	outputStream_ = NULL;
}

void BasicOutputStream::seek(const unsigned long& offset, const SeekType& offsetFrom)
{
	if ( NULL != outputStream_ ) {
		outputStream_->seek( offset, offsetFrom );
	}

	switch ( offsetFrom ) {
		case stSeekFromStart: {
			outStream_.setSeekPos( offset );
		}
		break;

		case stSeekFromEnd: {
			outStream_.setSeekPos( outStream_.getSize() - offset );
		}
		break;

		case stSeekFromRelative: {
			outStream_.setSeekPos( outStream_.getSeekPos() + offset );
		}
		break;
	}
}

unsigned long BasicOutputStream::getSize()
{
	return outStream_.getSize();
}

char* BasicOutputStream::getBuffer()
{
	if ( NULL != outputStream_ ){
		return outputStream_->getBuffer();
	}

	return outStream_.getBuffer();
}

unsigned long BasicOutputStream::write( const unsigned char* bytesToRead, unsigned long sizeOfBytes )
{
	unsigned long result = 0;
	result = outStream_.write( bytesToRead, sizeOfBytes );
	if ( NULL != outputStream_ ){
		result = outputStream_->write( bytesToRead, sizeOfBytes );
	}

	return result;
}


ulong32 BasicOutputStream::getCurrentSeekPos()
{
	if ( NULL != outputStream_ ) {
		return outputStream_->getCurrentSeekPos();
	}
	return outStream_.getSeekPos();
}


/**
$Id$
*/