// XMLProc.h: interface for the CXMLProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPROC_H__98F73F2A_7E4C_4D14_AF61_06664CED6C51__INCLUDED_)
#define AFX_XMLPROC_H__98F73F2A_7E4C_4D14_AF61_06664CED6C51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef JESCOMMON_EXPORTS
#define JESCOMMON_API __declspec(dllexport)
#else
#define JESCOMMON_API __declspec(dllimport)
#endif

#if defined(JESCOMMON_EXPORTS) || defined(JESCOMMON_IMPORTS)
XERCES_CPP_NAMESPACE_USE
XALAN_CPP_NAMESPACE_USE
#endif

#define TRANSFORMER_SELECTOR 	_T("child::*")

#define CONFIG_ENTRY_SELECTOR		_T("//configEntry[@setting=\"%s\"]")
#define XMLATTRIBUTE_MULTIVALUE		_T("multivalue")
#define XMLATTRIBUTE_MULTIVALUE_SEPARATOR		_T("multivalueSeparator")
#define XMLATTRIBUTE_VALUE			_T("value")
#define XMLATTRIBUTE_NAME			_T("name")
#define XMLATTRIBUTE_PLACEHOLDER	_T("placeholder")
#define XMLATTRIBUTE_ONLYFIRSTVALUE	_T("onlyFirstValue")
#define XMLATTRIBUTE_DATEMASK		_T("dateMask")
#define XMLATTRIBUTE_CONDITION		_T("condition")
#define XMLATTRIBUTE_NUMERIC		_T("numeric")
#define XMLATTRIBUTE_VALUEMODIFIER	_T("valueModifier")
#define XMLATTRIBUTE_INPUT			_T("input")
#define XMLATTRIBUTE_OUTPUT			_T("output")
#define XMLATTRIBUTE_IGNORE			_T("ignore")
#define XMLATTRIBUTE_VIEWS			_T("views")
#define XMLATTRIBUTE_MESSAGEFORMS	_T("messageForms")
#define XMLATTRIBUTE_SKIPMESSAGEFORMS	_T("skipMessageForms")
#define XMLATTRIBUTE_FIELD_PRESENT	_T("fieldPresent")
#define XMLATTRIBUTE_DATEFILTERFIELD		_T("dateFilterField")
#define XMLATTRIBUTE_DATEFILTERAFTER		_T("dateFilterAfter")
#define XMLATTRIBUTE_DATEFILTERBEFORE		_T("dateFilterBefore")
#define XMLATTRIBUTE_EXTRACTCN		_T("extractCN")
#define XMLATTRIBUTE_SMTPLOOKUP_REQUIRED _T("smtpLookupRequired")

#define XMLATTRIBUTE_CONDITION_LD_FIELD _T("conditionLotusDocField")
#define XMLATTRIBUTE_CONDITION_LD_FIELD_VALUE _T("conditionLotusDocFieldValue")
#define XMLATTRIBUTE_DOT_PRESENT_IN_VALUE _T("dotPresentInValue")

#define XMLATTRIBUTE_BINARY			_T("binary")
#define XMLATTRIBUTE_NAMED_PROPERTY	_T("namedProperty")

#define XMLATTRIBUTE_ADDRESS_CONCAT_SEP	_T("addressConcatenationSeparator")
#define XMLATTRIBUTE_ADDRESS_CONVERSION	_T("addressConversion")
#define XMLATTRIBUTE_FILE				_T("file")
#define XMLATTRIBUTE_LANGUAGE			_T("language")

#define XMLELEMENT_PSTITEMPROPERTY	_T("pstItemProperty")

class CXMLNameValue;

class CXMLProc  
{
public:
	JESCOMMON_API CXMLProc();
	JESCOMMON_API virtual ~CXMLProc();
	JESCOMMON_API int CXMLProc::Parse();
	JESCOMMON_API const _TCHAR* getConfigSettingValue(const _TCHAR* settingName, const _TCHAR* selector = CONFIG_ENTRY_SELECTOR);
	JESCOMMON_API CXMLNameValue* getNextNameValue(_TCHAR* selector, const _TCHAR *name_attr = _T("name"), const _TCHAR *value_attr = _T("value"));
	JESCOMMON_API bool matchesRegExp(char *regExp, char* matchString);

	// Get/Set m_xmlConfig
	JESCOMMON_API _TCHAR const * GetXmlConfig();
	JESCOMMON_API void SetXmlConfig(const _TCHAR *xmlConfig);

	// Get/Set m_xmlConfig
	JESCOMMON_API _TCHAR const * GetSchemaName();
	JESCOMMON_API void SetSchemaName(const _TCHAR *schemaPrefix, const _TCHAR *schemaName);

	// Get/Set m_verbose
	JESCOMMON_API bool const GetVerbose();
	JESCOMMON_API void SetVerbose(const bool verbose);


#ifdef NDIR2JES
	CLdifEntry* getNextLdifPersonEntry();
	CLdifEntry* getNextLdifGroupEntry();
	CLdifEntryTransformer* getNextLdifEntryTransformer();
	void insertLdifEntry(const _TCHAR *attribute, const _TCHAR *value);
	CSmtpAddr* getSmtpAddrInfo();
#endif

#ifdef NMAIL2JES
	CMailbox* getMailboxInfo();
	CMessageItem * GetNextMessageItem(_TCHAR *partCondition = NULL);
	CEntryTransformer* getNextEntryTransformer();
	CConversionRuleEntry* getNextConversionRuleEntry();
	void resetConversionRuleIndex();
#endif

private:

protected:
	JESCOMMON_API void getEntryList(_TCHAR* selector);
	bool m_verbose;
	_TCHAR m_xmlConfig[256];
	_TCHAR m_schemaName[256];

//#ifdef XML_INTERNAL

#if defined(JESCOMMON_EXPORTS) || defined(JESCOMMON_IMPORTS)
	XercesDOMParser *parser;
	SAXErrorHandler *errHandler;
	DOMDocument* theXercesDocument;
	XalanDocument* theDocument;
//	XalanSourceTreeDOMSupport*		theDOMSupport;
	XercesDOMSupport*		theDOMSupport;
	XalanDocumentPrefixResolver*		thePrefixResolver;
//	XalanSourceTreeParserLiaison*	theLiaison;
	XercesParserLiaison* theLiaison;

	MutableNodeRefList* m_entryList;
	int m_currentEntryIndex;
	bool m_entryListInitialized;

	MutableNodeRefList* m_transformerList;
	int m_currentTransformerIndex;
	bool m_transformerListInitialized;

	MutableNodeRefList* m_conversionList;
	int m_currentConversionIndex;
	bool m_conversionListInitialized;
#endif
//#endif
};

#endif // !defined(AFX_XMLPROC_H__98F73F2A_7E4C_4D14_AF61_06664CED6C51__INCLUDED_)
