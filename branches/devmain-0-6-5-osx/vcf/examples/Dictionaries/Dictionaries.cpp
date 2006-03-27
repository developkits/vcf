//Dictionaries.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "vcf/FoundationKit/FoundationKit.h"
#include "vcf/FoundationKit/Dictionary.h"


using namespace VCF;


typedef std::map<String,VariantData> map_t;


int main( int argc, char** argv ){

	FoundationKit::init( argc, argv );

	map_t m;
	m["sdfsd"] = 23455;


	//create a dictionary

	Dictionary dict;

	//add 2 elements to the dictionary.

	dict.insert( "size", 200 );
	dict["width"] = 150;

	dict["Name"] = "Bob";


	Dictionary::Enumerator* items = dict.getEnumerator();
	while ( items->hasMoreElements() ) {
		Dictionary::pair& item = items->nextElement();

		System::println( "dict[\"%s\"] = %s", item.first.c_str(), item.second.toString().c_str() );
	}



	System::println( "Saving dictionary..." );

	{
		//store the dictionary
		FileOutputStream fs("test.dict.txt");

		//note: The use of the TextOutputStream is not neccessary, it's
		//just for demonstration purposes.
		TextOutputStream tos(&fs);

		//dump the dictionary to the stream
		tos << &dict;

	}

	System::println( "Loading dictionary..." );

	{
		//read in the dictionary from a file
		FileInputStream fs("test.dict.txt");

		//note: The use of the TextInputStream is not neccessary, it's
		//just for demonstration purposes.
		TextInputStream tis(&fs);

		//clear the dictionary - this will remove all elements in the dictionary
		dict.clear();

		//read in the dictionary from the stream
		tis >> &dict;
	}


	items = dict.getEnumerator();
	while ( items->hasMoreElements() ) {
		Dictionary::pair& item = items->nextElement();

		System::println( "dict[\"%s\"] = %s", item.first.c_str(), item.second.toString().c_str() );
	}



	FoundationKit::terminate();
	return 0;
}


/**
*CVS Log info
*$Log$
*Revision 1.5.2.4  2004/04/29 03:40:52  marcelloptr
*reformatting of source files: macros and csvlog and copyright sections
*
*/

