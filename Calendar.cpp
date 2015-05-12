// Calendar.cpp: implementation of the CCalendar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calendar.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalendar::CCalendar()
{

}

CCalendar::~CCalendar()
{

}

// Get/Set m_dateFilterField
_TCHAR const * CCalendar::GetDateFilterField()
{
	return &m_dateFilterField[0];
}

void CCalendar::SetDateFilterField(const _TCHAR *dateFilterField)
{
	if (dateFilterField != NULL) {
		::_tcscpy(m_dateFilterField, dateFilterField);
	}
}

// Get/Set m_dateFilterAfter
_TCHAR const * CCalendar::GetDateFilterAfter()
{
	return &m_dateFilterAfter[0];
}

void CCalendar::SetDateFilterAfter(const _TCHAR *dateFilterAfter)
{
	if (dateFilterAfter != NULL) {
		::_tcscpy(m_dateFilterAfter, dateFilterAfter);
	}
}

// Get/Set m_dateFilterBefore
_TCHAR const * CCalendar::GetDateFilterBefore()
{
	return &m_dateFilterBefore[0];
}

void CCalendar::SetDateFilterBefore(const _TCHAR *dateFilterBefore)
{
	if (dateFilterBefore != NULL) {
		::_tcscpy(m_dateFilterBefore, dateFilterBefore);
	}
}

// Get/Set m_dateMask
_TCHAR const * CCalendar::GetDateMask()
{
	return &m_dateMask[0];
}

void CCalendar::SetDateMask(const _TCHAR *dateMask)
{
	if (dateMask != NULL) {
		::_tcscpy(m_dateMask, dateMask);
	}
}