#include "stdafx.h"

#include <xercesc/sax/SAXParseException.hpp>
#include "SAXErrorHandler.hpp"
//#if defined(XERCES_NEW_IOSTREAMS)
//#include <iostream>
//#else
//#include <iostream.h>
//#endif
//#include <stdlib.h>
//#include <string.h>


void SAXErrorHandler::warning(const SAXParseException&)
{
    //
    // Ignore all warnings.
    //
}

void SAXErrorHandler::error(const SAXParseException& toCatch)
{
    fSawErrors = true;
	_tprintf(_T("Error at file \"%s\", line %d, column %d\n   Message: %s\n"), toCatch.getSystemId(), 
		toCatch.getLineNumber(), toCatch.getColumnNumber(), toCatch.getMessage());
//    XERCES_STD_QUALIFIER cerr << "Error at file \"" << StrX(toCatch.getSystemId())
//		 << "\", line " << toCatch.getLineNumber()
//		 << ", column " << toCatch.getColumnNumber()
//         << "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAXErrorHandler::fatalError(const SAXParseException& toCatch)
{
    fSawErrors = true;
	_tprintf(_T("Fatal Error at file \"%s\", line %d, column %d\n   Message: %s\n"), toCatch.getSystemId(), 
		toCatch.getLineNumber(), toCatch.getColumnNumber(), toCatch.getMessage());
//    XERCES_STD_QUALIFIER cerr << "Fatal Error at file \"" << StrX(toCatch.getSystemId())
//		 << "\", line " << toCatch.getLineNumber()
//		 << ", column " << toCatch.getColumnNumber()
//         << "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAXErrorHandler::resetErrors()
{
    fSawErrors = false;
}
