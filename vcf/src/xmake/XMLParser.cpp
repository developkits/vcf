//XMLParser.cpp

/*
Copyright 2000-2004 The VCF Project.
Please see License.txt in the top level directory
where you installed the VCF.
*/


#include "xmake.h"
#include "XMLParser.h"



using namespace xmake;

String XMLAttr::toString()
{
	String result = " " + getName() + "=\"" + getValue() + "\" "; 
	return result;
}


XMLNode::XMLNode( const String& name, XMLNode* parentNode )
{
	name_ = name;
	parentNode_ = parentNode;
	attrsContainer_.initContainer( attrs_ );
	childNodesContainer_.initContainer( childNodes_ );

	if ( NULL != parentNode ) {
		parentNode->addChildNode( this );
	}
}


XMLNode::XMLNode( const XMLNode& node ) 
{
	name_ = node.name_;
	parentNode_ = node.parentNode_;
	CDATA_ = node.CDATA_;
	attrs_ = node.attrs_;
	//childNodes_ = node.childNodes_;
	std::vector<XMLNode*>::const_iterator it = node.childNodes_.begin();
	while ( it != childNodes_.end() ) {
		XMLNode* node = *it;	
		XMLNode* newNode = new XMLNode( *node );
		newNode->parentNode_ = this;
		addChildNode( newNode );
		
		it ++;
	}

	attrsContainer_.initContainer( attrs_ );
	childNodesContainer_.initContainer( childNodes_ );
}

XMLNode::~XMLNode()
{
	clearChildNodes();
}


XMLAttr* XMLNode::getAttrByName( const String& name )
{
	XMLAttr* result = NULL;
	std::vector<XMLAttr>::iterator it = attrs_.begin();
	while ( it != attrs_.end() ){
		if ( name == (*it).getName() ) {
			result = &(*it);
			break;
		}
		it ++;
	}
	return result;
}

XMLAttr* XMLNode::getAttrByIndex( const long& index )
{
	XMLAttr* result = NULL;
	if ( index < (long)attrs_.size() ) {
		result = &attrs_[index];
	}
	return result;
}

void XMLNode::removeNode( XMLNode* node )
{
	std::vector<XMLNode*>::iterator found = std::find( childNodes_.begin(), childNodes_.end(), node );
	if ( found != childNodes_.end() ){
		childNodes_.erase( found );
	}
}

XMLNode* XMLNode::getNodeByName( const String& name )
{
	XMLNode* result = NULL;
	
	std::vector<XMLNode*>::iterator it = childNodes_.begin();
	while ( it != childNodes_.end() ) {
		XMLNode* node = *it;
		if ( node->getName() == name ) {
			result = node;
			break;
		}
		it ++;
	}

	return result;
}

XMLNode* XMLNode::getNodeByIndex( const long& index )
{
	XMLNode* result = NULL;
	if ( (index >= 0) && (index < (long)childNodes_.size()) ) {
		result = childNodes_[index];
	}

	return result;
}

void XMLNode::clearChildNodes()
{
	std::vector<XMLNode*>::iterator it = childNodes_.begin();
	while ( it != childNodes_.end() ) {
		XMLNode* node = *it;
		delete node;
		node = NULL;
		it ++;
	}
}

long XMLNode::getDepth()
{
	long result = 0;

	XMLNode* parent = getParentNode();
	while ( NULL != parent ) {
		result ++;
		parent = parent->getParentNode();
	}
		
	return result;
}

String XMLNode::toString()
{
	String result;

	String tab;
	int tabsize = getDepth();
	for ( int i=0;i<tabsize;i++) {
		tab += "\t";
	}
	String nodeString = tab + "<" + getName() + " ";
	result += nodeString;

	std::vector<XMLAttr>::iterator attrIt = attrs_.begin();
	while ( attrIt != attrs_.end() ){
		XMLAttr* attr = &(*attrIt);
		result += attr->toString();
		attrIt ++;
	}
	
	if ( true == childNodes_.empty() ) {
		nodeString = "/>\n";
		result += nodeString;
	}
	else {
		nodeString = ">\n";
		result += nodeString;		

		std::vector<XMLNode*>::iterator nodeIt = childNodes_.begin();
		while ( nodeIt != childNodes_.end() ) {
			XMLNode* node = *nodeIt;
			result += node->toString();	
			nodeIt ++;
		}
		nodeString = tab + "</" + getName() + ">\n";
		result += nodeString;
	}

	return result;
}





XMLParser::XMLParser()
{
	parsedNodesContainer_.initContainer( parsedNodes_ );
	currentNode_ = NULL;	
	dtdStarted_ = false;

	entityMap_["amp"] = "&";
	entityMap_["quot"] = "\"";
	entityMap_["apos"] = "'";
	entityMap_["lt"] = "<";
	entityMap_["gt"] = ">";
}

XMLParser::~XMLParser()
{
	clearNodes();
}

void XMLParser::addEntity( const String& entityName, const String& value )
{
	entityMap_[entityName] = value;
}

Enumerator<XMLNode*>* XMLParser::getParsedNodes()
{
	return parsedNodesContainer_.getEnumerator();
}

void XMLParser::clearNodes()
{
	std::vector<XMLNode*>::iterator it = parsedNodes_.begin();
	if ( it != parsedNodes_.end() ) {
		// the first node is the root, delete it, and all the children go as well
		XMLNode* node = *it;
		delete node;
		node = NULL;		
	}
	parsedNodes_.clear();
}

void XMLParser::parse( const String& xmlString ) 
{
	currentNode_ = NULL;
	clearNodes();
	if ( ! xmlString.empty() ) {
		sourcePtr_ = xmlString.c_str();
		xmlBufferStart_ = sourcePtr_;
		sourceSize_ = xmlString.size();
		
		while ( nextNode() ) {
			
		}
	}
}

bool XMLParser::whiteSpace( const VCFChar& ch )
{
	return (ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\r');
}

String XMLParser::decodeText( const String& text )
{
	String result ;
	
	const VCFChar* P = text.c_str();
	const VCFChar* start = P;
	const VCFChar* currentStart = P;
	int size = text.size();


	String entity;
	const VCFChar* entityStart = NULL;
	while ( (P-start) < size ) {
		switch ( *P ) {
			case '&' : {
				if ( *(P+1) != ';' ) {
					entityStart = P;
				}
				else {
					entityStart = NULL;
				}
			}
			break;

			case ';' : {
				if ( (*(P-1) != '&') && (NULL != entityStart) ) {

					result.append( currentStart, entityStart - currentStart );

					entityStart++;
					entityStart = skipWhitespace( entityStart, P-entityStart );
				
					const VCFChar* end = skipTillWhitespace( entityStart, P-entityStart );

					entity = "";
					entity.append( entityStart, end-entityStart );
					
					if ( entity[0] == '#' ) {

						int number = 0;
						String numRef = entity.substr(1,entity.size()-1);

						if( (numRef[0] == 'x') || (numRef[0] == 'X') ) {
							numRef.erase(0,1);
							sscanf( numRef.c_str(), "%X", &number );
						}
						else {
							sscanf( numRef.c_str(), "%d", &number );
						}

						if( number >= 0 && number <= 255 ) {
							entity = String( sizeof(VCFChar),(VCFChar)number );
						}
						else {
							VCFChar tmp[256];
							sprintf( tmp, "%d", number );
							entity = tmp;
						}
					}
					else {
						std::map<String,String>::iterator found = entityMap_.find( entity );
						if ( found != entityMap_.end() ) {
							entity = found->second;
						}
						else {
							entity = "????";
						}
					}


					result.append( entity );

					currentStart = P+1;
				}
			}
			break;
		}
		P++;
	}

	if ( result.empty() ) {
		result = text;
	}
	else {
		if ( start != currentStart ) {
			result.append( currentStart, P - currentStart );
		}
	}

	return result;
}

bool XMLParser::nextNode() 
{		
	if ( sourceSize_ <= (sourcePtr_ - xmlBufferStart_) ) {
		return false;
	}
	
	const VCFChar* P = sourcePtr_;
	const VCFChar* tokenStart;
	while ( (*P != 0) && ((' ' == *P) || (13 == *P) || (10 == *P) || (9 == *P)) ) {
		P++;
	}
	
	tokenPtr_ = P;
	
	switch( *P ) {

		case ']' : {
			if ( dtdStarted_ ) {
				while ( (*P != XMLParser::TagClose) && (*P != 0) ) {
					P++;
				}

				P++;

				sourcePtr_ = P;
				dtdStarted_ = false;
				return true;
			}
		}
		break;

		case XMLParser::TagOpen : {
			P++;
			tokenStart = P;

			if ( *P == '?' ) {
				P++;

				//processing instructions - skip for now
				while ( (*P != '?') && (*P != 0) ) {
					P++;
				}
				if ( *P != '?' ) {
					throw RuntimeException( "Malformed XML processing node - end '?' expected, but none found." );	
				}

				P++;
				P = skipWhitespace( P, sourceSize_-(P-sourcePtr_) );
				if ( *P != XMLParser::TagClose ) {
					throw RuntimeException( "Malformed XML processing node - end '>' expected, but none found." );	
				}
				P++;

				sourcePtr_ = P;
				return true;
				
			}			
			else if ( *P == '!' ) {
				//check for '!'

				P++;
				if ( (*P == '-') && (*(P+1) == '-') ) {
					P += 2;
					//comment					
					const VCFChar* endComments = parseComments( P );
					P = endComments;
					P++;
					sourcePtr_ = P;
					return true;

					
				}

				String test;
				test.append( P, 25 );
				if ( test.find( "DOCTYPE" ) != String::npos ) {
					while ( (*P != '[') && (*P != 0) ) {
						P++;
					}
					//skip over the doctype decl
					P++;
					sourcePtr_ = P;
					dtdStarted_ = true;
					return true;

				}
				else if ( test.find( "ENTITY" ) != String::npos ) {
					P += 6;
					tokenStart = P;
					
					while ( (*P != XMLParser::TagClose) && (*P != 0) ) {
						P++;
					}

					parseEntity( tokenStart, P );	

					P++;
					sourcePtr_ = P;
					return true;
				}
				else if ( test.find( "[CDATA[" ) != String::npos ) {
					P += 7;
					tokenPtr_ = P;

					while ( ((*P != ']') || (*(P+1) != ']') || 
							(*(P+2) != XMLParser::TagClose)) && (*P != 0) ) {
						P++;
					}
					
					tokenString_ = "";
					
					tokenString_.append( tokenPtr_, P - tokenPtr_ );
					if ( NULL != currentNode_ ) {
						currentNode_->setCDATA( currentNode_->getCDATA() + tokenString_ );
						//	XMLParserEvent event( this, currentNode_ );
						//	fireOnNodeCDATAFound( &event );
					}

					P+= 3;
				}
			}
			else {				
				
				if ( *tokenStart != XMLParser::TagEnd ) {
					//parse the node for attributes
					
					XMLNode* node = new XMLNode("", currentNode_ ); 

					parsedNodes_.push_back( node );
					currentNode_ = node;
					P = parseNode( tokenStart, P+(sourceSize_-(P-sourcePtr_)) );
				}
				else {
					while ( (*P != XMLParser::TagClose) && (*P != 0) ) {
						P++;
					}
				}
				const VCFChar* endTag = P;
				endTag--;
				bool endTagFound = false;
				if ( *endTag == XMLParser::TagEnd ) {
					endTagFound = true;					
				}

				//skip past the closing node
				P++; 
				if ( true == endTagFound ) {
					P++; 
					sourcePtr_ = P;
					if ( NULL != currentNode_ ) {
						currentNode_ = currentNode_->getParentNode();
					}
					return true;
				}
				else {
					if ( *tokenStart == XMLParser::TagEnd ) {
						sourcePtr_ = P;
						if ( NULL != currentNode_ ) {
							currentNode_ = currentNode_->getParentNode();
						}
						return true;
					}
				}
			}			
		}
		break;

		default : {
			tokenString_ = "";
			//check for character data
			while ( (*P != '<') && ( (P - xmlBufferStart_ ) < sourceSize_) ) {
				P++;
			}

			tokenString_.append( tokenPtr_, P - tokenPtr_ );
			if ( NULL != currentNode_ ) {
				currentNode_->setCDATA( currentNode_->getCDATA() + decodeText( tokenString_ ) );
			//	XMLParserEvent event( this, currentNode_ );
			//	fireOnNodeCDATAFound( &event );
			}
		}
		break;

	}
	
	sourcePtr_ = P;
	return sourceSize_ > ( sourcePtr_ - xmlBufferStart_ );
}

const VCFChar* XMLParser::parseComments( const VCFChar* commentPtrStart ) 
{
	const VCFChar* result = NULL;
	bool commentsFinished = false;
	while ( (false == commentsFinished) && (sourceSize_ > ( commentPtrStart - xmlBufferStart_ )) ) {
		switch ( *commentPtrStart ) {
			case '-' : {
				const VCFChar* tmp = commentPtrStart;
				tmp++;
				if ( *tmp == '-' ) {
					tmp ++;
					if ( *tmp == XMLParser::TagClose ) {
						//poof - Elvis has left the building ! comments are finished
						commentPtrStart = tmp;
						commentsFinished = true;
					}
				}
			}
			break;
		}
		commentPtrStart++;
	}

	result = commentPtrStart;

	return result;
}

const VCFChar* XMLParser::skipWhitespace( const VCFChar* start, const int& length )
{
	const VCFChar* result = start;
	while ( ((result-start) < length) && (*result == ' ') || (*result == '\n') || (*result == '\t') || (*result == '\r') ) {
		result ++;
	}

	return result;
}

const VCFChar* XMLParser::skipTillWhitespace( const VCFChar* start, const int& length )
{
	const VCFChar* result = start;
	while ( ((result-start) < length) && (*result != ' ') && (*result != '\n') && (*result != '\t') && (*result != '\r') ) {
		result ++;
	}

	return result;
}

void XMLParser::parseEntity( const VCFChar* entityPtrStart, const VCFChar* entityPtrEnd )
{

}

const VCFChar* XMLParser::parseNode( const VCFChar* nodePtrStart, const VCFChar* nodePtrEnd ) 
{	
	const VCFChar* result = nodePtrStart;
	const VCFChar* start = nodePtrStart;
	
	start = skipWhitespace( start, nodePtrEnd-nodePtrStart );
	
	const VCFChar* P = start;
	const VCFChar* end = P;

	while ( (*end != XMLParser::TagClose) && (*end != XMLParser::TagEnd) && (*end != 0) ) {
		end++;
	}
	P = skipTillWhitespace( start, end-start );
	
	String s;	
	s.append( start, P - start );
	currentNode_->setName( s );	

	P = skipWhitespace( P, end-P );

	
	P = parseAttrs( P, nodePtrEnd );

	while ( (*P != XMLParser::TagClose) && (*P != 0) ) {
		P++;
	}
	
	result = P;
//	XMLParserEvent event( this, currentNode_ );
//	fireOnNodeFound( &event );

	return result;
}

const VCFChar* XMLParser::parseAttrs( const VCFChar* attrPtrStart, const VCFChar* attrPtrEnd ) 
{	
	const VCFChar* result;

	currentNode_->clearAttrs();

	const VCFChar* P = skipWhitespace(attrPtrStart,attrPtrEnd-attrPtrStart);

	bool nextAttr = (P < attrPtrEnd) && (*P != XMLParser::TagClose) && 
			(*P != XMLParser::TagEnd) && (*P != XMLParser::TagOpen);
	while ( true == nextAttr ) {
		const VCFChar* nameStart = P;	
		const VCFChar* assignmentStart = P;

		while ( ((' ' != *assignmentStart) && ('\n' != *assignmentStart) && 
					('\r' != *assignmentStart) && ('\t' != *assignmentStart)) && 
					(*assignmentStart != XMLParser::TagEquals) && 
					(assignmentStart < attrPtrEnd)  ) {
		
			assignmentStart++;
		}

		if ( *assignmentStart != XMLParser::TagEquals ) {
			assignmentStart = skipWhitespace( assignmentStart, attrPtrEnd-assignmentStart );
		}

		P = skipTillWhitespace( P, assignmentStart-P );

		String name;
		String value;
		name.append( nameStart, P - nameStart );

		P = assignmentStart;

		bool assignmentOperatorFound = false;
		switch ( *P ) {
			case XMLParser::TagEquals : {
				P++;
				P = skipWhitespace(P,attrPtrEnd-P);
				assignmentOperatorFound = true;
			}
			break;

			case 9 : case ' ' : case 10 : case 13: {
				while ( ((' ' == *P) || (13 == *P) || (10 == *P) || (9 == *P)) && (P < attrPtrEnd)  ) {
					P++;
				}
				if ( *P == XMLParser::TagEquals ) {
					P = skipWhitespace(P,attrPtrEnd-P);
					assignmentOperatorFound = true;
				}				
			}
			break;
		}

		if ( true == assignmentOperatorFound ) {				
			//goto the first quote
			while ( (*P != XMLParser::TagQuote) && (P < attrPtrEnd)  ) {
				P++;
			}
			if ( *P != XMLParser::TagQuote ) {
				throw RuntimeException( "Malformed XML Attribute - string literal expected, but no beginning quote (\") found." );	
			}
			P++;
			const VCFChar* valueStart = P;

			//goto end quote
			while ( (*P != XMLParser::TagQuote) && (P < attrPtrEnd)  ) {
				P++;
			}

			if ( *P != XMLParser::TagQuote ) {
				throw RuntimeException( "Malformed XML Attribute - string literal expected, but no beginning quote (\") found." );	
			}

			value.append( valueStart, P - valueStart );

			value = decodeText( value );

			currentNode_->addAttr( XMLAttr(name,value,currentNode_) );

			P++;

			//get rid of the white space
			P = skipWhitespace(P,attrPtrEnd-P);			
		}
		else {
			throw RuntimeException( "Malformed XML Attribute" );
		}			
		
		nextAttr = (P < attrPtrEnd) && (*P != XMLParser::TagClose) && 
			(*P != XMLParser::TagEnd) && (*P != XMLParser::TagOpen);
	}

	result = P;
	return result;
}

void XMLParser::addChildren( XMLNode* parent )
{
	parsedNodes_.push_back( parent );
	Enumerator<XMLNode*>* children = parent->getChildNodes();
	while ( children->hasMoreElements() ) {
		addChildren( children->nextElement() );
	}
}

void XMLParser::resortParsedNodes()
{
	XMLNode* first = parsedNodes_.front();

	parsedNodes_.clear();

	addChildren( first );
}



/**
$Id$
*/
