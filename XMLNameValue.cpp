// XMLNameValue.cpp: implementation of the CXMLNameValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLNameValue.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLNameValue::CXMLNameValue()
{

}

CXMLNameValue::~CXMLNameValue()
{

}

const _TCHAR* CXMLNameValue::GetName()
{
	return m_name;
}

const _TCHAR* CXMLNameValue::GetValue()
{
	return m_value;
}

void CXMLNameValue::SetName(const _TCHAR *name)
{
	_tcscpy(m_name, name);
}

void CXMLNameValue::SetValue(const _TCHAR *value)
{
	_tcscpy(m_value, value);
}
