// XMLMailProc.h: interface for the CXMLMailProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLMAILPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_)
#define AFX_XMLMAILPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NMAILXML_EXPORTS
#define NMAILXML_API __declspec(dllexport)
#else
#define NMAILXML_API __declspec(dllimport)
#endif

#ifdef NMAILXML_EXPORTS
XERCES_CPP_NAMESPACE_USE
XALAN_CPP_NAMESPACE_USE
#endif

#define MAILBOX_FOLDERS_SELECTOR	_T("//mailbox/folders")
#define MAILBOX_MESSAGE_ITEM_SELECTOR	_T("//mailbox/message/messageItem")
#define MAILBOX_MESSAGE_ITEM_CSELECTOR	_T("//mailbox/message/messageItem[@condition=\"%s\"]")

#define CONVERSION_RULE_ENTRY_SELECTOR			_T("child::conversionRule/child::*")

#define SMTPLOOKUP_SELECTOR 	_T("//mailbox/message/messageItem/lotusDocField[@valueModifier=\"smtpLookup\"]")

class CMailbox;
class CMessageItem;
class CEntryTransformer;
class CConversionRuleEntry;

class CXMLMailProc : public CXMLProc  
{
public:
	NMAILXML_API CXMLMailProc();
	virtual ~CXMLMailProc();

	NMAILXML_API CMailbox* getMailboxInfo();
	NMAILXML_API CMessageItem * GetNextMessageItem(_TCHAR *partCondition = NULL);
	NMAILXML_API CEntryTransformer* getNextEntryTransformer(_TCHAR *nodeSelector = NULL);
	NMAILXML_API CConversionRuleEntry* getNextConversionRuleEntry(_TCHAR *nodeSelector = NULL);
	NMAILXML_API void resetConversionRuleIndex();
	NMAILXML_API void resetEntryListIndex();
	NMAILXML_API void resetTransformerListIndex();
	void InitializeMessageItemList();
	NMAILXML_API bool IsSMTPLookupRequested();
};

#endif // !defined(AFX_XMLMAILPROC_H__A27B6A75_17DA_4814_AD79_FE431D325F4C__INCLUDED_)
