// XMLNameValue.hpp: interface for the CXMLNameValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNAMEVALUE_HPP__37D9F67E_2B1F_462B_B8F2_CD5DD88B2B8F__INCLUDED_)
#define AFX_XMLNAMEVALUE_HPP__37D9F67E_2B1F_462B_B8F2_CD5DD88B2B8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef JESCOMMON_EXPORTS
#define JESCOMMON_API __declspec(dllexport)
#else
#define JESCOMMON_API __declspec(dllimport)
#endif

class CXMLNameValue  
{
public:
	JESCOMMON_API CXMLNameValue();
	virtual ~CXMLNameValue();
	JESCOMMON_API const _TCHAR* GetName();
	JESCOMMON_API const _TCHAR* GetValue();
	void SetName(const _TCHAR *name);
	void SetValue(const _TCHAR *name);

private:
	_TCHAR m_name[128];
	_TCHAR m_value[512];
};

#endif // !defined(AFX_XMLNAMEVALUE_HPP__37D9F67E_2B1F_462B_B8F2_CD5DD88B2B8F__INCLUDED_)
