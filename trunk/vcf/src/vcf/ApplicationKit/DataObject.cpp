//DataObject.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


/* Generated by Together */
#include "vcf/ApplicationKit/ApplicationKit.h"

using namespace VCF;

/*
DataObject::DataObject( const char* dataToInitWith,
										 const unsigned long& dataSize,
										 const String& dataType )
{
	dataSize_ = dataSize;
	rawData_ = new char[ dataSize_ ];
	memcpy( rawData_, dataToInitWith, dataSize_ );

	init();

	addSupportedDataType( dataType );
}
*/
DataObject::DataObject()
{
	init();
}

DataObject::~DataObject()
{
	std::map<String,Persistable*>::iterator it = dataMap_.begin();
	while ( it != dataMap_.end() ) {
		delete it->second;
		it ++;
	}

	dataMap_.clear();
}

void DataObject::init()
{

}

void DataObject::addSupportedDataType( const String& dataType, Persistable* dataRepresentation )
{
	std::vector<String>::iterator found = std::find( types_.begin(), types_.end(), dataType );
	if ( found == types_.end() ) {
		types_.push_back( dataType );
	}

	if ( NULL != dataRepresentation ) {
		dataMap_[dataType] = dataRepresentation;
	}
}

bool DataObject::isTypeSupported( const String& dataType )
{
	bool result = false;

	std::vector<String>::iterator found = std::find( types_.begin(), types_.end(), dataType );
	if ( found != types_.end() ) {
		result = true;
	}

	return result;
}

bool DataObject::saveToStream( const String& dataType, OutputStream * stream )
{
	std::map<String,Persistable*>::iterator found = dataMap_.find( dataType );
	if ( found != dataMap_.end() ) {
		stream->write( found->second );
	}
	else {
		return false;
	}

	return true;
}

bool DataObject::loadFromStream( const String& dataType, InputStream * stream )
{
	std::map<String,Persistable*>::iterator found = dataMap_.find( dataType );
	if ( found != dataMap_.end() ) {
		stream->read( found->second );
	}
	else {
		return false;
	}

	return true;
}

Enumerator<String>* DataObject::getSupportedDataTypes()
{
	typesContainer_.initContainer( types_ );
	return typesContainer_.getEnumerator();
}




TextDataObject::TextDataObject( const String& text ):
	DataObject()
{

	AnsiString tmp = text;
	BinaryPersistable* data = new BinaryPersistable( NULL, tmp.size()+1 );
	tmp.copy( (char*)data->getData(), tmp.size() );
	data->getData()[tmp.size()] = 0;

	text_ = text;
	addSupportedDataType( STRING_DATA_TYPE, data );
}

TextDataObject::~TextDataObject()
{

}

String TextDataObject::getTextData()
{
	return text_;
}




ImageDataObject::ImageDataObject( Image* image ):
	DataObject()
{
	image_ = NULL;

	addSupportedDataType( IMAGE_DATA_TYPE, dynamic_cast<Persistable*>(image) );
}

ImageDataObject::~ImageDataObject()
{

}

Image* ImageDataObject::getImageData()
{
	if ( NULL == image_ ) {
		std::map<String,Persistable*>::iterator found = dataMap_.find( IMAGE_DATA_TYPE );
		if ( found != dataMap_.end() ) {
			image_ = dynamic_cast<Image*>( found->second );
		}
	}
	return image_;
}


/**
$Id$
*/