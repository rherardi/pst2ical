// CalendarItem.cpp: implementation of the CCalendarItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalendarItem.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalendarItem::CCalendarItem()
{
	m_name[0] = _T('\0');
	m_value[0] = _T('\0');
	m_condition[0] = _T('\0');
	m_conditionLotusDocField[0] = _T('\0');
	m_conditionLotusDocFieldValue[0] = _T('\0');
}

CCalendarItem::~CCalendarItem()
{

}

// Get/Set m_name
_TCHAR const * CCalendarItem::GetName()
{
	return &m_name[0];
}
void CCalendarItem::SetName(const _TCHAR *name)
{
	if (name != NULL) {
		::_tcscpy(m_name, name);
	}
}

// Get/Set m_name
_TCHAR const * CCalendarItem::GetValue()
{
	return &m_value[0];
}
void CCalendarItem::SetValue(const _TCHAR *value)
{
	if (value != NULL) {
		::_tcscpy(m_value, value);
	}
}

// Get/Set m_condition
_TCHAR const * CCalendarItem::GetCondition()
{
	return &m_condition[0];
}
void CCalendarItem::SetCondition(const _TCHAR *condition)
{
	if (condition != NULL) {
		::_tcscpy(m_condition, condition);
	}
}

// Get/Set m_conditionLotusDocField
_TCHAR const * CCalendarItem::GetConditionLotusDocField()
{
	return &m_conditionLotusDocField[0];
}
void CCalendarItem::SetConditionLotusDocField(const _TCHAR *condition)
{
	if (condition != NULL) {
		::_tcscpy(m_conditionLotusDocField, condition);
	}
}

// Get/Set m_conditionLotusDocFieldValue
_TCHAR const * CCalendarItem::GetConditionLotusDocFieldValue()
{
	return &m_conditionLotusDocFieldValue[0];
}
void CCalendarItem::SetConditionLotusDocFieldValue(const _TCHAR *condition)
{
	if (condition != NULL) {
		::_tcscpy(m_conditionLotusDocFieldValue, condition);
	}
}

