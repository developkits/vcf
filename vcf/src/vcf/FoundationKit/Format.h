#ifndef _VCF_FORMAT_H__
#define _VCF_FORMAT_H__

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
	\p
	The Format class is used to format a string, similar to the 
	sprintf/printf family of functions, only made typesafe for
	C++, and less likely to encounter buffer overflow errors 
	that are so easy to do with sprintf and friends.
	\p
	The inspiration for this class comes unashamedly from the
	Boost Format library (http://www.boost.org/libs/format/doc/format.html).
	Any mistakes in the "translation" are mine, not Boost's.
	
	\p
	The basic idea is to pass in a string to the Format instance,
	and then use the "%" operator to separate your arguments. 
	Currently the usage is \e exactly the same as you would use 
	printf/sprintf, in terms of formatting. For example:
	\code
	//printf	
	printf( "Hello World, %d times!", 10 );

	//Encoding the same using a Format object
	String s = Format("Hello World, %d times!") % 10;
	\endcode
	The "s" variable will now contain "Hello World, 10 times!". 
	
	\p
	The Format object will throw assert exceptions (in debug mode) if too
	few arguments are passed in, or too many. For example:
	\code
	//BAD - too few arguments!!
	String s = Format("Hello World, %d times!");
	\endcode
	The above example is expecting 1 argument, and didn't receive any, and will
	thus assert. 

	\code
	//BAD too many arguments!!
	String s = Format("Hello World, %d times!") % 100 % 0.56564;
	\endcode
	The above example expected 1 argument, but received 2, and will assert.

	@see StringUtils::format(const Format&)
	@see StringUtils::traceWithArgs(const Format&)
	@see System::print(const Format&)
	@see System::println(const Format&)
	*/
	class Format {
	public:
		
		Format( const String& fmtStr ): fmtStr_(fmtStr),currentPos_(0),
			expectedFormatArgCount_(0), currentFormatArgCount_(0){
			
			expectedFormatArgCount_ = countFormatTokens();
			
			currentPos_ = fmtStr_.find( "%" );
			if ( String::npos == currentPos_ ) {
				output_ += fmtStr_;
				fmtStr_.erase();
			}
		}
		
		//should we add virtual here for customizing the class???
		~Format() {
			
		}


		// operator used to concatenate arguments
		template<typename ValType> 
			Format& operator% ( const ValType& val ) {
			currentFormatArgCount_  ++;
			
			VCF_ASSERT ( currentFormatArgCount_ <= expectedFormatArgCount_ );
			if ( currentFormatArgCount_ > expectedFormatArgCount_ ) {
				return *this;
			}
			
			if ( !fmtStr_.empty() ) {  
				if ( String::npos != currentPos_ ) {
					//look ahead and see if we have two consecutive %% chars
					//if so treat as one %
					if ( (fmtStr_.size() >= (currentPos_+1)) && (fmtStr_[currentPos_+1] == '%') ) {
						output_ += fmtStr_.substr(0, currentPos_ ); //just copy off the first "%" char
						fmtStr_.erase( 0, currentPos_+1 );//erase up the 2nd "%" pos
					}
					else { //we have to format this string
						int endPos = getNextFormatTokenEndPos( currentPos_ );
						
						output_ += fmtStr_.substr( 0, currentPos_ );
						fmtStr_.erase( 0, currentPos_ );
						String fmt;
						
						if ( String::npos != endPos ) {												
							fmt = fmtStr_.substr( 0, endPos-currentPos_ );			
							
							fmtStr_.erase( 0, endPos-currentPos_ );
						}
						else{
							fmt = fmtStr_;						
							fmtStr_.erase();
						}
						
						int cb = fmt.size()+256;
						char* tmp = new char[cb];
						int n = sprintf( tmp, fmt.ansi_c_str(), val );
						if ( (cb-1) < n ) {
							// we probably had a memory corruption
							throw RuntimeException( MAKE_ERROR_MSG_2("Format: unable to printout the full string as required.") );
						}
						output_ += tmp;
						delete [] tmp;
					}
				}
				else {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
				
				currentPos_ = fmtStr_.find( "%" );
				if ( String::npos == currentPos_ ) {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
			}		
			return *this;
		}
		
#ifndef VCF_CW
		// specialization for a String value.
		template <String>
			Format& operator% (const String& val) {

			currentFormatArgCount_  ++;
			
			VCF_ASSERT ( currentFormatArgCount_ <= expectedFormatArgCount_ );
			if ( currentFormatArgCount_ > expectedFormatArgCount_ ) {
				return *this;
			}
			
			if ( !fmtStr_.empty() ) {  
				if ( String::npos != currentPos_ ) {
					//look ahead and see if we have two consecutive %% chars
					//if so treat as one %
					if ( (fmtStr_.size() >= (currentPos_+1)) && (fmtStr_[currentPos_+1] == '%') ) {
						output_ += fmtStr_.substr(0, currentPos_ ); //just copy off the first "%" char
						fmtStr_.erase( 0, currentPos_+1 );//erase up the 2nd "%" pos
					}
					else { //we have to format this string
						int endPos = getNextFormatTokenEndPos( currentPos_ );
						
						output_ += fmtStr_.substr( 0, currentPos_ );
						fmtStr_.erase( 0, currentPos_ );
						String fmt;
						
						if ( String::npos != endPos ) {												
							fmt = fmtStr_.substr( 0, endPos-currentPos_ );			
							
							fmtStr_.erase( 0, endPos-currentPos_ );
						}
						else{
							fmt = fmtStr_;						
							fmtStr_.erase();
						}
						
						char* tmp = new char[fmt.size()+val.size()+10];
						sprintf( tmp, fmt.ansi_c_str(), val.ansi_c_str() );
						output_ += tmp;
						delete [] tmp;
					}
				}
				else {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
				
				currentPos_ = fmtStr_.find( "%" );
				if ( String::npos == currentPos_ ) {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
			}		
			return *this;
		}
#endif //VCF_CW		
		
		
		operator String() const {
			//assert if the % operator wasn't called the correct number of times
			VCF_ASSERT ( currentFormatArgCount_ == expectedFormatArgCount_ );
			return output_;
		}


	protected:
		int countFormatTokens() const {
			int result = 0;
			String searchToken = "%";
			int pos = fmtStr_.find( searchToken );
			
			while ( pos != String::npos ) {
				if ( !( (fmtStr_.size() >= (pos+1)) && (fmtStr_[pos+1] == '%') ) ) {
					result ++;
				}
				pos = fmtStr_.find( searchToken, pos+1);
			}
			
			return result;
		}
		
		int getNextFormatTokenEndPos( int startPos ) const {  
			
			int result = String::npos;
			
			if ( fmtStr_[startPos] != '%' ) {
				return result;
			}
			
			
			//flags: "-+0 #"
			
			//width [1..9,0]*
			
			//precision ".[1..9,0]"
			
			//precision for int? "hlI64L"
			
			//type "cCdiouxXeEfgnpsS"
			
			
			String search = "-+0 #123456789*.hlI64LcCdiouxXeEfgnpsS";
			
			result = fmtStr_.find_first_not_of( search, startPos+1 );
			
			return result;
		}
	private:
		String fmtStr_;
		int currentPos_;
		String output_;
		int expectedFormatArgCount_;
		int currentFormatArgCount_;
	};

// In-class member template specializations (as used above)
// are non-standard (Section 14.7.3.2) hence this definition below. - ACH
#ifdef VCF_CW
		// specialization for a String value.
		template <> inline
			Format& Format::operator%<String> (const String& val) {

			currentFormatArgCount_  ++;
			
			VCF_ASSERT ( currentFormatArgCount_ <= expectedFormatArgCount_ );
			if ( currentFormatArgCount_ > expectedFormatArgCount_ ) {
				return *this;
			}
			
			if ( !fmtStr_.empty() ) {  
				if ( String::npos != currentPos_ ) {
					//look ahead and see if we have two consecutive %% chars
					//if so treat as one %
					if ( (fmtStr_.size() >= (currentPos_+1)) && (fmtStr_[currentPos_+1] == '%') ) {
						output_ += fmtStr_.substr(0, currentPos_ ); //just copy off the first "%" char
						fmtStr_.erase( 0, currentPos_+1 );//erase up the 2nd "%" pos
					}
					else { //we have to format this string
						int endPos = getNextFormatTokenEndPos( currentPos_ );
						
						output_ += fmtStr_.substr( 0, currentPos_ );
						fmtStr_.erase( 0, currentPos_ );
						String fmt;
						
						if ( String::npos != endPos ) {												
							fmt = fmtStr_.substr( 0, endPos-currentPos_ );			
							
							fmtStr_.erase( 0, endPos-currentPos_ );
						}
						else{
							fmt = fmtStr_;						
							fmtStr_.erase();
						}
						
						char* tmp = new char[fmt.size()+val.size()+10];
						sprintf( tmp, fmt.ansi_c_str(), val.ansi_c_str() );
						output_ += tmp;
						delete [] tmp;
					}
				}
				else {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
				
				currentPos_ = fmtStr_.find( "%" );
				if ( String::npos == currentPos_ ) {
					output_ += fmtStr_;
					fmtStr_.erase();
				}
			}		
			return *this;
		}
#endif VCF_CW

};

/**
*CVS Log info
*$Log$
*Revision 1.1.2.8  2005/04/14 16:01:21  marcelloptr
*fixed template specialization sintax
*
*Revision 1.1.2.7  2005/04/11 17:07:10  iamfraggle
*Changes allowing compilation of Win32 port under CodeWarrior
*
*Revision 1.1.2.6  2005/04/05 16:42:56  marcelloptr
*introduced safer use of erase to clear a string
*
*Revision 1.1.2.5  2005/03/25 17:59:18  marcelloptr
*added template specialization for String so to avoid problems with c_str() ansi_c_str()
*
*Revision 1.1.2.2  2005/03/15 01:51:51  ddiego
*added support for Format class to take the place of the
*previously used var arg funtions in string utils and system. Also replaced
*existing code in the framework that made use of the old style var arg
*functions.
*
*Revision 1.1.2.1  2005/03/14 05:44:51  ddiego
*added the Formatter class as part of the process of getting rid of the var arg methods in System and StringUtils.
*
*/

#endif //_VCF_FORMAT_H__

