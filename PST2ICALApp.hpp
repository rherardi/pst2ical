// Notes2PSTApp.hpp: interface for the CPST2ICALApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PST2ICALAPP_HPP__DF6D88D6_63BA_4734_8960_CA9DC7327262__INCLUDED_)
#define AFX_PST2ICALAPP_HPP__DF6D88D6_63BA_4734_8960_CA9DC7327262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPSTProc;
class CXMLCalProc;
class CCalendar;
class CCalendarItem;
class CEntryTransformer;
class CConversionRuleEntry;
class CXMLNameValue;

#define DEFAULT_CONFIG_FILE	_T("pst2ical.xml")
#define DEFAULT_SCHEMA_FILE	_T("pst2ical.xsd")

#define DEFAULT_SCHEMA_PREFIX		_T("http://www.gssnet.com/pst2ical")
#define DEFAULT_PST_EXT _T(".pst")

class CPST2ICALApp  
{
public:
	CPST2ICALApp();
	virtual ~CPST2ICALApp();

	void ShowUsage();
	_TCHAR *PrefixWithCurDir(_TCHAR *fileName, _TCHAR *fileNameFull);
	int ProcessCommandLine(int argc, _TCHAR *argv[]);
	int StartMigration(_TCHAR* exec);
	CCalendar* getCalendarInfo();
	CCalendarItem * GetNextCalendarItem(_TCHAR *partCondition = NULL);
	CCalendarItem* GetNextCalendarSMTPRequiredItem();
	CEntryTransformer* getNextEntryTransformer();
	CConversionRuleEntry* getNextConversionRuleEntry();
	void resetConversionRuleIndex();
	void resetEntryListIndex();
	void resetEntryTransformerListIndex();
	const _TCHAR* getConfigSettingValue(const _TCHAR* settingName);
	bool matchesRegExp(char *regExpPattern, char* matchString);
	CXMLCalProc *getXMLProc();
	bool IsSMTPLookupRequired();
	void InitializeCalendarItemList();
	CXMLNameValue* getNextNameValue(_TCHAR* selector, const _TCHAR *name_attr = _T("name"), const _TCHAR *value_attr = _T("value"));
	bool IsPrintFormsOnly();
	bool IsSMTPLookupRequested();
	int GetCurrentEntryIndex();
	void SetCurrentEntryIndex(int currentEntryIndex);

protected:
	CPSTProc *m_pstProc;
	CXMLCalProc *m_xmlProc;
	bool m_verbose;
	bool m_printFormsOnly;

public:

	int IsVerbose() {
		return m_verbose == true;
	}

private:
	int ExtractSchemaFile(_TCHAR* tempDir);
};

#endif // !defined(AFX_PST2ICALAPP_HPP__DF6D88D6_63BA_4734_8960_CA9DC7327262__INCLUDED_)
