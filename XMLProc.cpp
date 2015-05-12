// XMLProc.cpp: implementation of the CXMLProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <xercesc/util/XercesDefs.hpp>

#include <xalanc/Include/PlatformDefinitions.hpp>

#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/framework/LocalFileInputSource.hpp>
//#include <xercesc/framework/MemBufInputSource.hpp>

#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanDOMException.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>

#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDocumentWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>

//#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
//#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
//#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>
#include <xalanc/XPath/NodeRefList.hpp>
#include <xalanc/XPath/MutableNodeRefList.hpp>

#include <xalanc/XercesParserLiaison/XercesDocumentWrapper.hpp>

//#define XML_INTERNAL

#include "SAXErrorHandler.hpp"

#include "XMLProc.hpp"
#include "XMLNameValue.hpp"
#include "resource.h"

#ifdef NDIR2JES
#include "LdifEntry.hpp"
#include "LdifEntryTransformer.hpp"
#include "SmtpAddr.hpp"
#endif

#ifdef NMAIL2JES
#include "Mailbox.hpp"
#include "MessageItem.hpp"
#include "EntryTransformer.hpp"
#include "ConversionRuleEntry.hpp"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLProc::CXMLProc()
{
	m_verbose = false;
	m_entryList = NULL;
	m_entryListInitialized = false;
	m_transformerListInitialized = false;
	m_xmlConfig[0] = _T('\0');
	m_schemaName[0] = _T('\0');

	// Initialize the XML4C2 system
	try
	{

		XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XPathEvaluator)

		XMLPlatformUtils::Initialize();

		XPathEvaluator::initialize();

		//
		//  Create our parser, then attach an error handler to the parser.
		//  The parser will call back to methods of the ErrorHandler if it
		//  discovers errors during the course of parsing the XML document.
		//
		parser = new XercesDOMParser();

	//	parser->setValidationScheme(gValScheme);
	//	parser->setDoNamespaces(gDoNamespaces);
	//	parser->setDoSchema(gDoSchema);
	//	parser->setValidationSchemaFullChecking(gSchemaFullChecking);
	//	parser->setCreateEntityReferenceNodes(gDoCreate);

	}

	catch(const XMLException &toCatch)
	{
//		XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
//			 << "  Exception message:"
//			 << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
//		return 1;
	}


}

CXMLProc::~CXMLProc()
{
/*
	//
	//  Delete the parser itself.  Must be done prior to calling Terminate, below.
	//
	delete parser;
*/
	XPathEvaluator::terminate();

	// And call the termination method
	XMLPlatformUtils::Terminate();
}

int CXMLProc::Parse()
{
	int retCode = MIGRATION_STEP_SUCCESS;
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::Parse()");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}

	//
	//  Parse the XML file, catching any XML exceptions that might propogate
	//  out of it.
	//
	bool errorsOccured = false;
	try
	{
		parser->parse(m_xmlConfig);
	}
	catch (const OutOfMemoryException& e)
	{
//		XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
		errorsOccured = true;
	}
	catch (const XMLException& e)
	{
//		XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n   Message: "
//			 << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
		_tprintf(_T("error: %s\n"), e.getMessage());
		errorsOccured = true;
	}

	catch (const DOMException& e)
	{
//		const unsigned int maxChars = 2047;
//		XMLCh errText[maxChars + 1];

//		XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << gXmlFile << "'\n"
//			 << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;

//		if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
//			 XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;

		_tprintf(_T("error: %s\n"), e.getMessage());
		errorsOccured = true;
	}

	catch (...)
	{
//		XERCES_STD_QUALIFIER cerr << "An error occurred during parsing\n " << XERCES_STD_QUALIFIER endl;
		_tprintf(_T("Uncatched exception!\n"));
		errorsOccured = true;
	}

	// If the parse was successful, output the document data from the DOM tree
	if (!errorsOccured && !errHandler->getSawErrors()) {


//		XALAN_USING_XERCES(LocalFileInputSource)

		XALAN_USING_XALAN(XalanDocument)
		XALAN_USING_XALAN(XalanDocumentPrefixResolver)
		XALAN_USING_XALAN(XalanDOMString)
		XALAN_USING_XALAN(XalanNode)
		XALAN_USING_XALAN(XObjectPtr)

		XALAN_USING_XALAN(XercesParserLiaison)
		XALAN_USING_XALAN(XercesDOMSupport)

//		_tprintf(_T("err20: %d %d\n"), errorsOccured, errHandler->getSawErrors());
//		_tprintf(_T("err20\n"));
		theDOMSupport = new XercesDOMSupport();

		try {
//		_tprintf(_T("err200\n"));
		theXercesDocument = parser->getDocument();
		assert(theXercesDocument != NULL);
//		_tprintf(_T("err201\n"));
		XercesParserLiaison *theLiaison = new XercesParserLiaison(*theDOMSupport);
//		XercesParserLiaison *theLiaison = new XercesParserLiaison();
//		XercesDocumentWrapper *wrapper = theLiaison->doCreateDocument(theXercesDocument, false, false, false, false);
//		MemoryManagerType theManager = XALAN_DEFAULT_MEMMGR;
//		XercesDocumentWrapper *wrapper = new XercesDocumentWrapper(*XMLPlatformUtils::fgMemoryManager, theXercesDocument);
//		_tprintf(_T("err2010\n"));

//		XercesDocumentWrapper *wrapper = new XercesDocumentWrapper(theXercesDocument);
//		_tprintf(_T("err2011:\n"));
		assert(theLiaison != NULL);
		theDocument =
				theLiaison->createDocument(theXercesDocument, true, true, true);
		assert(theDocument != 0);

//		_tprintf(_T("err202:\n"));
		thePrefixResolver = new XalanDocumentPrefixResolver(theDocument);
		}
		catch (const XMLException& e)
		{
			_tprintf(_T("XMLException: %s\n"), e.getMessage());
		}
		catch (const DOMException& e)
		{
			_tprintf(_T("DOMException: %s\n"), e.getMessage());
		}
		catch (const SAXParseException& e)
		{
			_tprintf(_T("SAXParseException: %s\n"), e.getMessage());
		}
		catch (const XercesDOMException& e)
		{
			_tprintf(_T("XercesDOMException: %d\n"), e.getExceptionCode());
		}
		catch (const XalanDOMException& e)
		{
			_tprintf(_T("XalanDOMException: %d\n"), e.getExceptionCode());
		}
		
		catch(exception e) {
			_tprintf(_T("exception\n"));
			retCode = MIGRATION_ERR_FAILED;
			goto cleanup;
//			printf(e.what());
		}
		catch(...) {
			_tprintf(_T("Uncatched exception!\n"));
			retCode = MIGRATION_ERR_FAILED;
			goto cleanup;
		}

	}
	else {
//		_tprintf(_T("err2: %d %d\n"), errorsOccured, errHandler->getSawErrors());
		retCode = MIGRATION_ERR_FAILED;
		goto cleanup;
	}
cleanup:
	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	return retCode;
}

// Get/Set m_xmlConfig
_TCHAR const * CXMLProc::GetXmlConfig()
{
	return &m_xmlConfig[0];
}
void CXMLProc::SetXmlConfig(const _TCHAR *xmlConfig)
{
	::_tcscpy(m_xmlConfig, xmlConfig);
}

// Get/Set m_schemaName
_TCHAR const * CXMLProc::GetSchemaName()
{
	return &m_schemaName[0];
}
void CXMLProc::SetSchemaName(const _TCHAR *schemaPrefix, const _TCHAR *schemaName)
{

	// For later - try to use MemBufInputSource and loadGrammar
	::_tcscpy(m_schemaName, schemaName);

    parser->setDoNamespaces(true);
	parser->setDoSchema(true);
	_TCHAR externalSchemaLocation[512];
	_tcscpy(externalSchemaLocation, schemaPrefix);
	_tcscat(externalSchemaLocation, _T(" "));
	_tcscat(externalSchemaLocation, schemaName);

	parser->setExternalSchemaLocation(externalSchemaLocation);
	parser->setValidationScheme(XercesDOMParser::Val_Always);

	errHandler = new SAXErrorHandler();
	parser->setErrorHandler(errHandler);

}

// Get/Set m_verbose
bool const CXMLProc::GetVerbose()
{
	return m_verbose;
}
void CXMLProc::SetVerbose(const bool verbose)
{
	m_verbose = verbose;
}


// Should be common for all configuration files
const _TCHAR* CXMLProc::getConfigSettingValue(const _TCHAR *settingName, const _TCHAR* selector)
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::getConfigSettingValue");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}

	const _TCHAR* settingValue = NULL;
	_TCHAR selectorFilled[256];

	_stprintf(selectorFilled, selector, settingName);
	if (m_verbose){
		::LoadString(hInst, IDS_PROCESSING_CHOICE, usage, 256);
		::_tprintf(_TEXT("%s selector: %s\n"), usage, selectorFilled);
	}

	assert(theDocument != 0);
	assert(theDOMSupport != 0);
	assert(thePrefixResolver != 0);
	XPathEvaluator	*theEvaluator = new XPathEvaluator();

	XalanNode* const	theConfigNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(selectorFilled).c_str(),
							*thePrefixResolver);

	if (theConfigNode != NULL)
	{
		const XalanNamedNodeMap* attrs = theConfigNode->getAttributes();
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		settingValue = valueNode->getNodeValue().c_str();
		if (m_verbose){
			::LoadString(hInst, IDS_PROCESSING_CHOICE, usage, 256);
			::_tprintf(_TEXT("%s settingValue: %s\n"), usage, settingValue);
		}
	}

	theEvaluator->~XPathEvaluator();

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	return settingValue;
}

void CXMLProc::getEntryList(_TCHAR* selector)
{
	_TCHAR usage[256];
//	if (_tcscmp(selector, _T("//mailbox/message/messageItem[@condition=\"Sender\"]")) == 0)
//		_tprintf(_T("-->111 Sender\n"));
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::getEntryList");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s (selector: %s)\n"), usage, current_method, selector);
	}
//	_tprintf(_T("-->112 Sender\n"));
//	XPathEvaluator	*theEvaluator = new XPathEvaluator();
	XPathEvaluator	theEvaluator;
//	_tprintf(_T("-->113 Sender\n"));
	NodeRefList	theResult;
//	_tprintf(_T("-->114 Sender\n"));
	// check here: http://www.nabble.com/Xalan---C---Users-f273.html
	assert(theDOMSupport != 0);
	assert(theDocument != 0);
	assert(thePrefixResolver != 0);
	try {
		theEvaluator.selectNodeList(
								theResult,
								*theDOMSupport,
								theDocument,
								XalanDOMString(selector).c_str(),
								*thePrefixResolver);
//		_tprintf(_T("-->115 Sender\n"));
		m_entryList = new MutableNodeRefList(theResult, theResult.getMemoryManager());
	}
	catch (const XMLException& e)
	{
		_tprintf(_T("XMLException: %s\n"), e.getMessage());
	}
	catch (const DOMException& e)
	{
		_tprintf(_T("DOMException: %s\n"), e.getMessage());
	}
	catch (const SAXParseException& e)
	{
		_tprintf(_T("SAXParseException: %s\n"), e.getMessage());
	}
	catch (const XercesDOMException& e)
	{
		_tprintf(_T("XercesDOMException: %d\n"), e.getExceptionCode());
	}
	catch (const XalanDOMException& e)
	{
		_tprintf(_T("XalanDOMException: %d\n"), e.getExceptionCode());
	}
	catch(...)
	{
		_tprintf(_T("Generic exception caught!\n"));
	}

//	theEvaluator->~XPathEvaluator();
	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
}

CXMLNameValue* CXMLProc::getNextNameValue(_TCHAR* selector, const _TCHAR *name_attr, const _TCHAR *value_attr)
{
	CXMLNameValue *entry;
	if (!m_entryListInitialized) {
		getEntryList(selector);
		m_entryListInitialized = true;
	}
	if (m_currentEntryIndex < m_entryList->getLength()) {
		entry = new CXMLNameValue();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(name_attr));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(value_attr));
		entry->SetName(nameNode->getNodeValue().c_str());
		if (valueNode != NULL)
			entry->SetValue(valueNode->getNodeValue().c_str());

		m_currentEntryIndex++;
	}
	else {
		m_currentEntryIndex = 0;
		m_entryListInitialized=false;
		entry = NULL;
	}

	return entry;
}

bool CXMLProc::matchesRegExp(char *regExpPattern, char* matchString)
{
	RegularExpression *regExp = new RegularExpression(regExpPattern);
	return regExp->matches(matchString);
}

#ifdef NDIR2JES
CLdifEntry* CXMLProc::getNextLdifPersonEntry()
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::getNextLdifPersonEntry");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	CLdifEntry *entry;
	if (!m_entryListInitialized) {
		getEntryList(LDIF_PERSON_SELECTOR);
		m_entryListInitialized = true;
	}
	if (m_currentEntryIndex < m_entryList->getLength()) {
		entry = new CLdifEntry();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* attrNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ATTRIBUTE));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		entry->SetAttribute(attrNode->getNodeValue().c_str());
		entry->SetValue(valueNode->getNodeValue().c_str());

		m_currentEntryIndex++;
	}
	else {
		m_currentEntryIndex = 0;
		m_entryListInitialized=false;
		entry = NULL;
	}

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	return entry;
}

CLdifEntry* CXMLProc::getNextLdifGroupEntry()
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::getNextLdifGroupEntry");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	CLdifEntry *entry;
	if (!m_entryListInitialized) {
		getEntryList(LDIF_GROUP_SELECTOR);
		m_entryListInitialized = true;
	}
	if (m_currentEntryIndex < m_entryList->getLength()) {
		entry = new CLdifEntry();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* attrNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ATTRIBUTE));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		entry->SetAttribute(attrNode->getNodeValue().c_str());
		entry->SetValue(valueNode->getNodeValue().c_str());

		m_currentEntryIndex++;
	}
	else {
		m_currentEntryIndex = 0;
		m_entryListInitialized=false;
		entry = NULL;
	}

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	return entry;
}

CLdifEntryTransformer* CXMLProc::getNextLdifEntryTransformer()
{
	_TCHAR usage[256];
	HINSTANCE hInst = ::GetModuleHandle(NULL);
	_TCHAR current_method[] = _TEXT("CXMLProc::getNextLdifEntryTransformer");
	if (m_verbose){
		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}

	CLdifEntryTransformer *transformer;
	if (!m_transformerListInitialized) {
		XalanNode* theNode = m_entryList->item(m_currentEntryIndex-1);
		XPathEvaluator	*theEvaluator = new XPathEvaluator();
		NodeRefList	theResult;
		theEvaluator->selectNodeList(
								theResult,
								*theDOMSupport,
								theNode,
								XalanDOMString(LDIF_TRANSFORMER_SELECTOR).c_str(),
								*thePrefixResolver);
		m_transformerList = new MutableNodeRefList(theResult, theResult.getMemoryManager());
		theEvaluator->~XPathEvaluator();
		m_transformerListInitialized = true;
	}
	if (m_currentTransformerIndex < m_transformerList->getLength()) {
		transformer = new CLdifEntryTransformer();
		const XalanNode* const	theTransformerNode = m_transformerList->item(m_currentTransformerIndex);
		const XalanNamedNodeMap* attrs = theTransformerNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* placeholderNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_PLACEHOLDER));
		XalanNode* multivalueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE));
		XalanNode* onlyFirstValueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ONLYFIRSTVALUE));
		XalanNode* multivalueSeparatorNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE_SEPARATOR));
		XalanNode* orgHierarchyNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ORG_HIERARCHY));
		XalanNode* resolveDocFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_FIELD));
		XalanNode* resolveDocFormNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_FORM));
		XalanNode* resolveDocLookupFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_LOOKUP_FIELD));
		
		transformer->SetName(nameNode->getNodeValue().c_str());
		transformer->SetPlaceholder(placeholderNode->getNodeValue().c_str());
		if (multivalueNode == NULL) {
			transformer->SetMultivalue(false);
		}
		else {
			transformer->SetMultivalue(_tcscmp(multivalueNode->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		if (onlyFirstValueNode == NULL) {
			transformer->SetOnlyFirstValue(true);
		}
		else {
			transformer->SetOnlyFirstValue(_tcscmp(onlyFirstValueNode->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		if (multivalueSeparatorNode == NULL) {
			transformer->SetMultivalueSeparator(_T(";"));
		}
		else {
			transformer->SetMultivalueSeparator(multivalueSeparatorNode->getNodeValue().c_str());
		}
		if (orgHierarchyNode == NULL) {
			transformer->SetOrgHierarchy(false);
		}
		else {
			transformer->SetOrgHierarchy(_tcscmp(orgHierarchyNode->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		if (resolveDocFieldNode != NULL) {
//			_tprintf(_T("resolveDocFieldNode: %s\n"), resolveDocFieldNode->getNodeValue().c_str());
			transformer->SetResolveDocField(resolveDocFieldNode->getNodeValue().c_str());
		}
		if (resolveDocFormNode != NULL) {
//			_tprintf(_T("resolveDocFormNode: %s\n"), resolveDocFormNode->getNodeValue().c_str());
			transformer->SetResolveDocForm(resolveDocFormNode->getNodeValue().c_str());
		}
		if (resolveDocLookupFieldNode != NULL) {
//			_tprintf(_T("resolveDocLookupFieldNode: %s\n"), resolveDocLookupFieldNode->getNodeValue().c_str());
			transformer->SetResolveDocLookupField(resolveDocLookupFieldNode->getNodeValue().c_str());
		}

		if (_tcscmp(theTransformerNode->getNodeName().c_str(), XMLELEMENT_LOTUSDOCFIELD) == 0) {
			transformer->SetLotusDocField(true);
		}
		else {
			transformer->SetLotusDocField(false);
		}

		m_currentTransformerIndex++;
	}
	else {
		m_currentTransformerIndex = 0;
		m_transformerListInitialized = false;
		transformer = NULL;
	}

	if (m_verbose){
		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
	}
	return transformer;
}

void CXMLProc::insertLdifEntry(const _TCHAR *attribute, const _TCHAR *value)
{
	try
	{

	const XalanNode* const	theCurrentEntryNode = m_entryList->item(m_currentEntryIndex-1);
	const XalanNamedNodeMap* attrs = theCurrentEntryNode->getAttributes();
	XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
	assert(theDocument != 0);
	DOMElement *theNewNode= theXercesDocument->createElement(theCurrentEntryNode->getNodeName().c_str());

	int i = 0;
	int j = 0;
	for (i=0; i<attrs->getLength(); i++) {
		theNewNode->setAttribute(attrs->item(i)->getNodeName().c_str(), attrs->item(i)->getNodeValue().c_str());
	}

	const DOMNamedNodeMap* newAttrs = theNewNode->getAttributes();
	DOMNode* valueNodeDOM = newAttrs->getNamedItem(XMLATTRIBUTE_VALUE);
	valueNodeDOM->setNodeValue(value);

	const XalanNodeList *childNodes = theCurrentEntryNode->getChildNodes();
	for (j=0; j < childNodes->getLength(); j++) {
		if (childNodes->item(j)->getNodeType() != XalanElement::ELEMENT_NODE) {
			continue;
		}
//		_tprintf(_T("ax201 %s %d\n"), childNodes->item(j)->getNodeName().c_str(), childNodes->item(j)->getNodeType());
		DOMElement *theNewChildNode = theXercesDocument->createElement(childNodes->item(j)->getNodeName().c_str());
		attrs = childNodes->item(j)->getAttributes();
		for (i=0; i<attrs->getLength(); i++) {
			theNewChildNode->setAttribute(attrs->item(i)->getNodeName().c_str(), attrs->item(i)->getNodeValue().c_str());
		}
		theNewNode->appendChild(theNewChildNode);
	}

	theXercesDocument->getDocumentElement()->appendChild(theNewNode);

	XercesParserLiaison *theLiaison2 = new XercesParserLiaison(*theDOMSupport);
	XalanDocument* theXDoc = theLiaison2->createDocument(theXercesDocument);

	XPathEvaluator	*theEvaluator = new XPathEvaluator();
	NodeRefList	theResult;
	theEvaluator->selectNodeList(
							theResult,
							*theDOMSupport,
							theXDoc,
							XalanDOMString(_T("//ldifEntry")).c_str(),
							*thePrefixResolver);
	MutableNodeRefList* testList = new MutableNodeRefList(theResult, theResult.getMemoryManager());
	if (testList->getLength() > 0) {
		m_entryList->addNode(testList->item(testList->getLength()-1));
	}
	theEvaluator->~XPathEvaluator();
	}
	catch (const XMLException& e)
	{
		_tprintf(_T("XMLException: %s\n"), e.getMessage());
	}
	catch (const DOMException& e)
	{
		_tprintf(_T("DOMException: %s\n"), e.getMessage());
	}
}

CSmtpAddr* CXMLProc::getSmtpAddrInfo()
{
	_TCHAR selector[256];
	CSmtpAddr* info = NULL;
	_tcscpy(selector, SMTP_ADDR_SELECTOR);

	XPathEvaluator	*theEvaluator = new XPathEvaluator();

	XalanNode* const	theSmtpAddrNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(selector).c_str(),
							*thePrefixResolver);

	if (theSmtpAddrNode != NULL)
	{
		const XalanNamedNodeMap* attrs = theSmtpAddrNode->getAttributes();
		XalanNode* fileNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_FILE));
		XalanNode* separatorNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ADDRESS_CONCAT_SEP));
		XalanNode* conversionNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ADDRESS_CONVERSION));
		info = new CSmtpAddr();
		info->SetFile(fileNode->getNodeValue().c_str());
		info->SetSeparator(separatorNode->getNodeValue().c_str());
		info->SetConversion(conversionNode->getNodeValue().c_str());
	}

	theEvaluator->~XPathEvaluator();
	return info;
}
#endif
