// CalendarItem.hpp: interface for the CCalendarItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALENDARITEM_HPP__EC66FE86_817C_4257_84C8_5D4905E2CCBE__INCLUDED_)
#define AFX_CALENDARITEM_HPP__EC66FE86_817C_4257_84C8_5D4905E2CCBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

class CCalendarItem  
{
public:
	CCalendarItem();
	virtual ~CCalendarItem();

	// Get/Set m_attribute
	NCALXML_API _TCHAR const * GetName();
	NCALXML_API void SetName(const _TCHAR *name);

	// Get/Set m_value
	NCALXML_API _TCHAR const * GetValue();
	NCALXML_API void SetValue(const _TCHAR *value);

	// Get/Set m_condition
	NCALXML_API _TCHAR const * GetCondition();
	NCALXML_API void SetCondition(const _TCHAR *value);

	// Get/Set m_conditionLotusDocField
	NCALXML_API _TCHAR const * GetConditionLotusDocField();
	NCALXML_API void SetConditionLotusDocField(const _TCHAR *value);

	// Get/Set m_conditionLotusDocFieldValue
	NCALXML_API _TCHAR const * GetConditionLotusDocFieldValue();
	NCALXML_API void SetConditionLotusDocFieldValue(const _TCHAR *value);

protected:
	_TCHAR m_name[128];
	_TCHAR m_value[512];
	_TCHAR m_condition[128];
	_TCHAR m_conditionLotusDocField[128];
	_TCHAR m_conditionLotusDocFieldValue[128];

};

#endif // !defined(AFX_CALENDARITEM_HPP__EC66FE86_817C_4257_84C8_5D4905E2CCBE__INCLUDED_)
