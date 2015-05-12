// XMLMailProc.cpp: implementation of the CXMLMailProc class.
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
#include "XMLMailProc.hpp"

#include "Mailbox.hpp"
#include "MessageItem.hpp"
#include "EntryTransformer.hpp"
#include "ConversionRuleEntry.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLMailProc::CXMLMailProc()
{

}

CXMLMailProc::~CXMLMailProc()
{

}

CMailbox* CXMLMailProc::getMailboxInfo()
{
	_TCHAR selector[256];
	CMailbox* info = NULL;
	_tcscpy(selector, MAILBOX_FOLDERS_SELECTOR);

	XPathEvaluator	*theEvaluator = new XPathEvaluator();

	XalanNode* const	theFoldersNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(selector).c_str(),
							*thePrefixResolver);

	if (theFoldersNode != NULL)
	{
		const XalanNamedNodeMap* attrs = theFoldersNode->getAttributes();
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		info = new CMailbox();
		info->SetFolders(valueNode->getNodeValue().c_str());
		XalanNode* ignoreNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_IGNORE));
		if (ignoreNode != NULL)
			info->SetIgnoredFolders(ignoreNode->getNodeValue().c_str());
		XalanNode* viewsNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VIEWS));
		if (viewsNode != NULL)
			info->SetViews(viewsNode->getNodeValue().c_str());

		XalanNode* const theMailboxNode = theFoldersNode->getParentNode();
		attrs = theMailboxNode->getAttributes();
		XalanNode* dateFilterFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEFILTERFIELD));
		XalanNode* dateFilterAfterNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEFILTERAFTER));
		XalanNode* dateFilterBeforeNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEFILTERBEFORE));
		XalanNode* dateMaskNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEMASK));
		if (dateFilterFieldNode != NULL && 
			(dateFilterAfterNode != NULL || dateFilterBeforeNode != NULL) && 
			dateMaskNode != NULL) {
			info->SetDateFilterField(dateFilterFieldNode->getNodeValue().c_str());
			if (dateFilterAfterNode != NULL)
				info->SetDateFilterAfter(dateFilterAfterNode->getNodeValue().c_str());
			if (dateFilterBeforeNode != NULL)
				info->SetDateFilterBefore(dateFilterBeforeNode->getNodeValue().c_str());
			info->SetDateMask(dateMaskNode->getNodeValue().c_str());
		}
	}

	if (!m_entryListInitialized) {
		getEntryList(MAILBOX_MESSAGE_ITEM_SELECTOR);
		m_currentEntryIndex = 0;
		m_entryListInitialized = true;
	}

	theEvaluator->~XPathEvaluator();
	return info;
}

void CXMLMailProc::InitializeMessageItemList()
{
	getEntryList(MAILBOX_MESSAGE_ITEM_SELECTOR);
	m_currentEntryIndex = 0;
	m_entryListInitialized = true;
}

CMessageItem* CXMLMailProc::GetNextMessageItem(_TCHAR *partCondition)
{
	CMessageItem *entry = NULL;
	if (partCondition != NULL && !m_entryListInitialized) {
		_TCHAR selectorFilled[256];

//		if (_tcscmp(partCondition, _T("Sender")) == 0)
//			_tprintf(_T("-->10 Sender\n"));

		_stprintf(selectorFilled, MAILBOX_MESSAGE_ITEM_CSELECTOR, partCondition);
//		if (_tcscmp(partCondition, _T("Sender")) == 0)
//			_tprintf(_T("-->11 Sender\n"));
		getEntryList(selectorFilled);
//		if (_tcscmp(partCondition, _T("Sender")) == 0)
//			_tprintf(_T("-->12 Sender\n"));
		m_currentEntryIndex = 0;
		m_entryListInitialized = true;
	}

	if (m_entryList != NULL && (m_currentEntryIndex < m_entryList->getLength())) {
		entry = new CMessageItem();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		XalanNode* conditionNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION));
		XalanNode* numericNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NUMERIC));
		entry->SetName(nameNode->getNodeValue().c_str());
		entry->SetValue(valueNode->getNodeValue().c_str());
		if (conditionNode != NULL) {
			entry->SetCondition(conditionNode->getNodeValue().c_str());
		}
		if (numericNode != NULL) {
			entry->SetNumeric(_tcsicmp(numericNode->getNodeValue().c_str(), _T("true")) == 0);
			entry->SetValueNumeric(_ttoi(entry->GetValue()));
		}
		m_currentEntryIndex++;
	}
	else {
		m_currentEntryIndex = 0;
		m_entryListInitialized=false;
		entry = NULL;
	}
	return entry;
}

void CXMLMailProc::resetEntryListIndex()
{
	m_currentEntryIndex = 0;
	m_entryListInitialized = false;
	if (m_entryList != NULL) {
		delete(m_entryList);
		m_entryList = NULL;
	}
}

void CXMLMailProc::resetConversionRuleIndex()
{
	m_currentConversionIndex = 0;
	m_conversionListInitialized = false;
	if (m_conversionList != NULL) {
		delete(m_conversionList);
		m_conversionList = NULL;
	}
}

void CXMLMailProc::resetTransformerListIndex()
{
	m_currentTransformerIndex = 0;
	m_transformerListInitialized = false;
	if (m_transformerList != NULL) {
		delete(m_transformerList);
		m_transformerList = NULL;
	}
}

CConversionRuleEntry* CXMLMailProc::getNextConversionRuleEntry(_TCHAR *nodeSelector)
{
	CConversionRuleEntry *conversionRule = NULL;
	if (!m_conversionListInitialized) {
		XalanNode* theNode = NULL;
		XPathEvaluator	*theEvaluator = new XPathEvaluator();
		if (nodeSelector == NULL)
			theNode = m_transformerList->item(m_currentTransformerIndex-1);
		else {
			theNode = theEvaluator->selectSingleNode(
				*theDOMSupport,
				theDocument,
				XalanDOMString(nodeSelector).c_str(),
				*thePrefixResolver);
		}
		NodeRefList	theResult;
		theEvaluator->selectNodeList(
								theResult,
								*theDOMSupport,
								theNode,
								XalanDOMString(CONVERSION_RULE_ENTRY_SELECTOR).c_str(),
								*thePrefixResolver);
		m_conversionList = new MutableNodeRefList(theResult, theResult.getMemoryManager());
		theEvaluator->~XPathEvaluator();
		m_conversionListInitialized = true;
		m_currentConversionIndex = 0;
	}
	if (m_currentConversionIndex < m_conversionList->getLength()) {
		conversionRule = new CConversionRuleEntry();
		const XalanNode* const	theConversionNode = m_conversionList->item(m_currentConversionIndex);
		const XalanNamedNodeMap* attrs = theConversionNode->getAttributes();
		XalanNode* inputNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_INPUT));
		XalanNode* outputNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_OUTPUT));
		if (inputNode != NULL) {
			conversionRule->SetInput(inputNode->getNodeValue().c_str());
		}
		else {
			conversionRule->SetDefault(true);
		}
		conversionRule->SetOutput(outputNode->getNodeValue().c_str());
		m_currentConversionIndex++;
	}
	else {
		m_currentConversionIndex = 0;
		m_conversionListInitialized = false;
		conversionRule = NULL;
	}
	return conversionRule;
}

CEntryTransformer* CXMLMailProc::getNextEntryTransformer(_TCHAR *nodeSelector)
{
//	_TCHAR usage[256];
//	HINSTANCE hInst = ::GetModuleHandle(NULL);
//	_TCHAR current_method[] = _TEXT("CXMLMailProc::getNextEntryTransformer");
//	if (m_verbose){
//		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
//		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
//	}

	CEntryTransformer *transformer;
	if (!m_transformerListInitialized) {
		XalanNode* theNode = NULL;
		XPathEvaluator	*theEvaluator = new XPathEvaluator();
		if (nodeSelector == NULL)
			theNode = m_entryList->item(m_currentEntryIndex-1);
		else {
			theNode = theEvaluator->selectSingleNode(
				*theDOMSupport,
				theDocument,
				XalanDOMString(nodeSelector).c_str(),
				*thePrefixResolver);
		}
		NodeRefList	theResult;
		theEvaluator->selectNodeList(
								theResult,
								*theDOMSupport,
								theNode,
								XalanDOMString(TRANSFORMER_SELECTOR).c_str(),
								*thePrefixResolver);
		m_transformerList = new MutableNodeRefList(theResult, theResult.getMemoryManager());
		theEvaluator->~XPathEvaluator();
		m_transformerListInitialized = true;
	}
	if (m_currentTransformerIndex < m_transformerList->getLength()) {
		transformer = new CEntryTransformer();
		const XalanNode* const	theTransformerNode = m_transformerList->item(m_currentTransformerIndex);
		const XalanNamedNodeMap* attrs = theTransformerNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* placeholderNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_PLACEHOLDER));
		XalanNode* multivalueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE));
		XalanNode* onlyFirstValueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ONLYFIRSTVALUE));
		XalanNode* multivalueSeparatorNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE_SEPARATOR));
//		XalanNode* orgHierarchyNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ORG_HIERARCHY));
//		XalanNode* resolveDocFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_FIELD));
//		XalanNode* resolveDocFormNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_FORM));
//		XalanNode* resolveDocLookupFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_RESOLVE_DOC_LOOKUP_FIELD));
		XalanNode* dateMaskNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEMASK));
		XalanNode* valueModifierNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUEMODIFIER));
		
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
//		if (orgHierarchyNode == NULL) {
//			transformer->SetOrgHierarchy(false);
//		}
//		else {
//			transformer->SetOrgHierarchy(_tcscmp(orgHierarchyNode->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
//		}
//		if (resolveDocFieldNode != NULL) {
//			_tprintf(_T("resolveDocFieldNode: %s\n"), resolveDocFieldNode->getNodeValue().c_str());
//			transformer->SetResolveDocField(resolveDocFieldNode->getNodeValue().c_str());
//		}
//		if (resolveDocFormNode != NULL) {
//			_tprintf(_T("resolveDocFormNode: %s\n"), resolveDocFormNode->getNodeValue().c_str());
//			transformer->SetResolveDocForm(resolveDocFormNode->getNodeValue().c_str());
//		}
//		if (resolveDocLookupFieldNode != NULL) {
//			_tprintf(_T("resolveDocLookupFieldNode: %s\n"), resolveDocLookupFieldNode->getNodeValue().c_str());
//			transformer->SetResolveDocLookupField(resolveDocLookupFieldNode->getNodeValue().c_str());
//		}

		if (_tcscmp(theTransformerNode->getNodeName().c_str(), XMLELEMENT_PSTITEMPROPERTY) == 0) {
			transformer->SetLotusDocField(true);
		}
		else {
			transformer->SetLotusDocField(false);
		}
		if (dateMaskNode != NULL) {
			transformer->SetDateMask(dateMaskNode->getNodeValue().c_str());
		}
		if (valueModifierNode != NULL) {
			transformer->SetValueModifier(valueModifierNode->getNodeValue().c_str());
		}

		m_currentTransformerIndex++;
	}
	else {
		m_currentTransformerIndex = 0;
		m_transformerListInitialized = false;
		transformer = NULL;
	}

//	if (m_verbose){
//		::LoadString(hInst, IDS_EXIT_METHOD, usage, 256);
//		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
//	}
	return transformer;
}

bool CXMLMailProc::IsSMTPLookupRequested()
{
	bool result = false;

	XPathEvaluator	*theEvaluator = new XPathEvaluator();
	NodeRefList	theResult;

	XalanNode* const	theSmtplookupNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(SMTPLOOKUP_SELECTOR).c_str(),
							*thePrefixResolver);

	if (theSmtplookupNode != NULL)
		result = true;

	theEvaluator->~XPathEvaluator();
	return result;
}
