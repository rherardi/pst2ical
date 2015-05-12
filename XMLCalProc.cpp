// XMLMailProc.cpp: implementation of the CXMLCalProc class.
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
#include "XMLCalProc.hpp"

#include "Calendar.hpp"
#include "CalendarItem.hpp"
#include "EntryTransformer.hpp"
#include "ConversionRuleEntry.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLCalProc::CXMLCalProc()
{

}

CXMLCalProc::~CXMLCalProc()
{

}

CCalendar* CXMLCalProc::getCalendarInfo()
{
	_TCHAR selector[256];
	CCalendar* info = new CCalendar();

	XPathEvaluator	*theEvaluator = new XPathEvaluator();

	_tcscpy(selector, CALENDAR_SELECTOR);
	XalanNode* const	theNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(selector).c_str(),
							*thePrefixResolver);

	const XalanNamedNodeMap* attrs = theNode->getAttributes();
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

	if (!m_entryListInitialized) {
		InitializeCalendarItemList();
	}

	theEvaluator->~XPathEvaluator();
	return info;
}

int CXMLCalProc::GetCurrentEntryIndex()
{
	return m_currentEntryIndex;
}
void CXMLCalProc::SetCurrentEntryIndex(int currentEntryIndex)
{
	m_currentEntryIndex = currentEntryIndex;
}

void CXMLCalProc::InitializeCalendarItemList()
{
	getEntryList(CALENDAR_CALENDAR_ITEM_SELECTOR);
	m_currentEntryIndex = 0;
	m_entryListInitialized = true;
}

CCalendarItem* CXMLCalProc::GetNextCalendarItem(_TCHAR *partCondition, _TCHAR *partCondition2)
{
	CCalendarItem *entry = NULL;
	if (partCondition != NULL && !m_entryListInitialized) {
		_TCHAR selectorFilled[256];

		if (partCondition2 == NULL)
			_stprintf(selectorFilled, CALENDAR_CALENDAR_ITEM_CSELECTOR, partCondition);
		else
			_stprintf(selectorFilled, CALENDAR_CALENDAR_ITEM_CSELECTOR2, partCondition, partCondition2);
		getEntryList(selectorFilled);
		m_currentEntryIndex = 0;
		m_entryListInitialized = true;
	}

	if (m_currentEntryIndex < m_entryList->getLength()) {
		entry = new CCalendarItem();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		XalanNode* conditionNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION));
		XalanNode* conditionLotusDocFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD));
		XalanNode* conditionLotusDocFieldValueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD_VALUE));
		entry->SetName(nameNode->getNodeValue().c_str());
		entry->SetValue(valueNode->getNodeValue().c_str());
		if (conditionNode != NULL) {
			entry->SetCondition(conditionNode->getNodeValue().c_str());
		}
		if (conditionLotusDocFieldNode != NULL) {
			entry->SetConditionLotusDocField(conditionLotusDocFieldNode->getNodeValue().c_str());
		}
		if (conditionLotusDocFieldValueNode != NULL) {
			entry->SetConditionLotusDocFieldValue(conditionLotusDocFieldValueNode->getNodeValue().c_str());
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

CCalendarItem* CXMLCalProc::GetNextCalendarSMTPRequiredItem()
{
//	printf("-->10\n");
	CCalendarItem *entry = NULL;
	if (!m_entryListInitialized) {
//		printf("-->100\n");
		getEntryList(CALENDAR_ITEM_SMTPLOOKUP_REQUIRED);
		m_currentEntryIndex = 0;
		m_entryListInitialized = true;
	}
//	printf("-->2 %d\n", m_entryList->getLength());

	if (m_currentEntryIndex < m_entryList->getLength()) {
		entry = new CCalendarItem();
		const XalanNode* const	theNode = m_entryList->item(m_currentEntryIndex);
		const XalanNamedNodeMap* attrs = theNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* valueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUE));
		XalanNode* conditionNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION));
		XalanNode* conditionLotusDocFieldNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD));
		XalanNode* conditionLotusDocFieldValueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD_VALUE));
		entry->SetName(nameNode->getNodeValue().c_str());
		entry->SetValue(valueNode->getNodeValue().c_str());
		if (conditionNode != NULL) {
			entry->SetCondition(conditionNode->getNodeValue().c_str());
		}
		if (conditionLotusDocFieldNode != NULL) {
			entry->SetConditionLotusDocField(conditionLotusDocFieldNode->getNodeValue().c_str());
		}
		if (conditionLotusDocFieldValueNode != NULL) {
			entry->SetConditionLotusDocFieldValue(conditionLotusDocFieldValueNode->getNodeValue().c_str());
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

void CXMLCalProc::resetEntryListIndex()
{
	m_currentEntryIndex = 0;
	m_entryListInitialized = false;
}

void CXMLCalProc::resetEntryTransformerListIndex()
{
	m_currentTransformerIndex = 0;
	m_transformerListInitialized = false;
}

void CXMLCalProc::resetConversionRuleIndex()
{
	m_currentConversionIndex = 0;
	m_conversionListInitialized = false;
}

CConversionRuleEntry* CXMLCalProc::getNextConversionRuleEntry()
{
	CConversionRuleEntry *conversionRule = NULL;
	if (!m_conversionListInitialized) {
		XalanNode* theNode = m_transformerList->item(m_currentTransformerIndex-1);
		XPathEvaluator	*theEvaluator = new XPathEvaluator();
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
		XalanNode* fieldPresentNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_FIELD_PRESENT));
		if (inputNode != NULL) {
			conversionRule->SetInput(inputNode->getNodeValue().c_str());
		}
		else {
			conversionRule->SetDefault(true);
		}
		if (fieldPresentNode != NULL) {
			conversionRule->SetFieldPresent(_tcscmp(fieldPresentNode->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		else {
			conversionRule->SetFieldPresent(false);
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

CEntryTransformer* CXMLCalProc::getNextEntryTransformer()
{
//	_TCHAR usage[256];
//	HINSTANCE hInst = ::GetModuleHandle(NULL);
//	_TCHAR current_method[] = _TEXT("CXMLCalProc::getNextEntryTransformer");
//	if (m_verbose){
//		::LoadString(hInst, IDS_ENTER_METHOD, usage, 256);
//		::_tprintf(_TEXT("%s %s\n"), usage, current_method);
//	}

//	_tprintf(_T("-->cal0 %d<--\n"), m_currentEntryIndex);
	CEntryTransformer *transformer;
	if (!m_transformerListInitialized) {
		XalanNode* theNode = m_entryList->item(m_currentEntryIndex-1);
		XPathEvaluator	*theEvaluator = new XPathEvaluator();
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
//	_tprintf(_T("-->cal1 %d %d<--\n"), m_currentTransformerIndex, m_transformerList->getLength());
	if (m_currentTransformerIndex < m_transformerList->getLength()) {
		transformer = new CEntryTransformer();
		const XalanNode* const	theTransformerNode = m_transformerList->item(m_currentTransformerIndex);
		const XalanNamedNodeMap* attrs = theTransformerNode->getAttributes();
		XalanNode* nameNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAME));
		XalanNode* placeholderNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_PLACEHOLDER));
		XalanNode* multivalueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE));
		XalanNode* onlyFirstValueNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_ONLYFIRSTVALUE));
		XalanNode* multivalueSeparatorNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_MULTIVALUE_SEPARATOR));
		XalanNode* dateMaskNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DATEMASK));
		XalanNode* valueModifierNode = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_VALUEMODIFIER));
		XalanNode* extractCN = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_EXTRACTCN));
		XalanNode* smtpLookupRequired = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_SMTPLOOKUP_REQUIRED));
		
		XalanNode* conditionLotusDocField = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD));
		XalanNode* conditionLotusDocFieldValue = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_CONDITION_LD_FIELD_VALUE));
		XalanNode* dotPresentInValue = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_DOT_PRESENT_IN_VALUE));

		XalanNode* binaryValue = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_BINARY));
		XalanNode* namedPropertyValue = attrs->getNamedItem(XalanDOMString(XMLATTRIBUTE_NAMED_PROPERTY));

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
			transformer->SetPstItemProperty(true);
		}
		else {
			transformer->SetPstItemProperty(false);
		}
		if (dateMaskNode != NULL) {
			transformer->SetDateMask(dateMaskNode->getNodeValue().c_str());
		}
		if (valueModifierNode != NULL) {
			transformer->SetValueModifier(valueModifierNode->getNodeValue().c_str());
		}
		if (extractCN == NULL) {
			transformer->SetExtractCN(false);
		}
		else {
			transformer->SetExtractCN(_tcscmp(extractCN->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		if (smtpLookupRequired == NULL) {
			transformer->SetSmtpLookupRequired(false);
		}
		else {
			transformer->SetSmtpLookupRequired(_tcscmp(smtpLookupRequired->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
//		if (conditionLotusDocField != NULL) {
//			transformer->SetConditionLotusDocField(conditionLotusDocField->getNodeValue().c_str());
//		}
//		if (conditionLotusDocFieldValue != NULL) {
//			transformer->SetConditionLotusDocFieldValue(conditionLotusDocFieldValue->getNodeValue().c_str());
//		}
//		if (dotPresentInValue != NULL) {
//			transformer->SetDotPresentInValue(dotPresentInValue->getNodeValue().c_str());
//		}
		if (binaryValue == NULL) {
			transformer->SetBinary(false);
		}
		else {
			transformer->SetBinary(_tcscmp(binaryValue->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
		}
		if (namedPropertyValue == NULL) {
			transformer->SetNamedProperty(false);
		}
		else {
			transformer->SetNamedProperty(_tcscmp(namedPropertyValue->getNodeValue().c_str(), _T("true")) == 0 ? true : false);
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

bool CXMLCalProc::IsSMTPLookupRequested()
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

bool CXMLCalProc::IsSMTPLookupRequired(_TCHAR *condition)
{
	bool result = false;

	XPathEvaluator	*theEvaluator = new XPathEvaluator();
	NodeRefList	theResult;

	XalanNode* const	theSmtplookupRequiredNode =
							theEvaluator->selectSingleNode(
							*theDOMSupport,
							theDocument,
							XalanDOMString(SMTPLOOKUP_REQUIRED_SELECTOR).c_str(),
							*thePrefixResolver);

	if (theSmtplookupRequiredNode != NULL)
		result = true;

	theEvaluator->~XPathEvaluator();
	return result;
}
