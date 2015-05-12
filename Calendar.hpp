// Mailbox.h: interface for the CCalendar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALENDAR_H__16563A3F_26DC_4EF9_8EDB_6431B7CED278__INCLUDED_)
#define AFX_CALENDAR_H__16563A3F_26DC_4EF9_8EDB_6431B7CED278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

class CCalendar  
{
public:
	CCalendar();
	virtual ~CCalendar();

	// Get/Set m_dateFilterField
	NCALXML_API _TCHAR const * GetDateFilterField();
	NCALXML_API void SetDateFilterField(const _TCHAR *dateFilterField);

	// Get/Set m_dateFilterAfter
	NCALXML_API _TCHAR const * GetDateFilterAfter();
	NCALXML_API void SetDateFilterAfter(const _TCHAR *dateFilterAfter);

	// Get/Set m_dateFilterBefore
	NCALXML_API _TCHAR const * GetDateFilterBefore();
	NCALXML_API void SetDateFilterBefore(const _TCHAR *dateFilterBefore);

	// Get/Set m_dateMask
	NCALXML_API _TCHAR const * GetDateMask();
	NCALXML_API void SetDateMask(const _TCHAR *dateMask);

protected:
	_TCHAR m_dateFilterField[64];
	_TCHAR m_dateFilterAfter[64];
	_TCHAR m_dateFilterBefore[64];
	_TCHAR m_dateMask[64];
};

//#define FOLDER_NAME_SPACE _T("#x20")

#endif // !defined(AFX_CALENDAR_H__16563A3F_26DC_4EF9_8EDB_6431B7CED278__INCLUDED_)
