// EntryTransformer.cpp: implementation of the CEntryTransformer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EntryTransformer.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntryTransformer::CEntryTransformer()
{
	m_valueModifier[0] = _T('\0');
}

CEntryTransformer::~CEntryTransformer()
{

}

// Get/Set m_name
_TCHAR const * CEntryTransformer::GetName()
{
	return &m_name[0];
}
void CEntryTransformer::SetName(const _TCHAR *name)
{
	::_tcscpy(m_name, name);
}

// Get/Set m_placeholder
_TCHAR const * CEntryTransformer::GetPlaceholder()
{
	return &m_placeholder[0];
}
void CEntryTransformer::SetPlaceholder(const _TCHAR *placeholder)
{
	::_tcscpy(m_placeholder, placeholder);
}

// Get/Set m_resolveDocField
_TCHAR const * CEntryTransformer::GetResolveDocField()
{
	return &m_resolveDocField[0];
}
void CEntryTransformer::SetResolveDocField(const _TCHAR *resolveDocField)
{
	::_tcscpy(m_resolveDocField, resolveDocField);
}

// Get/Set m_resolveDocForm
_TCHAR const * CEntryTransformer::GetResolveDocForm()
{
	return &m_resolveDocForm[0];
}
void CEntryTransformer::SetResolveDocForm(const _TCHAR *resolveDocForm)
{
	::_tcscpy(m_resolveDocForm, resolveDocForm);
}

// Get/Set m_resolveDocLookupField
_TCHAR const * CEntryTransformer::GetResolveDocLookupField()
{
	return &m_resolveDocLookupField[0];
}
void CEntryTransformer::SetResolveDocLookupField(const _TCHAR *resolveDocLookupField)
{
	::_tcscpy(m_resolveDocLookupField, resolveDocLookupField);
}

// Get/Set m_pstItemProperty
bool const CEntryTransformer::GetPstItemProperty()
{
	return m_pstItemProperty;
}
void CEntryTransformer::SetPstItemProperty(const bool pstItemProperty)
{
	m_pstItemProperty = pstItemProperty;
}

// Get/Set m_multivalue
bool const CEntryTransformer::IsMultivalue()
{
	return m_multivalue;
}
void CEntryTransformer::SetMultivalue(const bool multivalue)
{
	m_multivalue = multivalue;
}

// Get/Set m_onlyFirstValue
bool const CEntryTransformer::GetOnlyFirstValue()
{
	return m_onlyFirstValue;
}

void CEntryTransformer::SetOnlyFirstValue(const bool onlyFirstValue)
{
	m_onlyFirstValue = onlyFirstValue;
}

// Get/Set m_multivalueSeparator
_TCHAR const * CEntryTransformer::GetMultivalueSeparator()
{
	return &m_multivalueSeparator[0];
}

void CEntryTransformer::SetMultivalueSeparator(const _TCHAR *multivalueSeparator)
{
	::_tcscpy(m_multivalueSeparator, multivalueSeparator);
}

// Get/Set m_orgHierarchy
bool const CEntryTransformer::GetOrgHierarchy()
{
	return m_orgHierarchy;
}

void CEntryTransformer::SetOrgHierarchy(const bool orgHierarchy)
{
	m_orgHierarchy = orgHierarchy;
}

// Get/Set m_extractCN
bool const CEntryTransformer::GetExtractCN()
{
	return m_extractCN;
}

void CEntryTransformer::SetExtractCN(const bool extractCN)
{
	m_extractCN = extractCN;
}


// Get/Set m_smtpLookupRequired
bool const CEntryTransformer::GetSmtpLookupRequired()
{
	return m_smtpLookupRequired;
}

void CEntryTransformer::SetSmtpLookupRequired(const bool smtpLookupRequired)
{
	m_smtpLookupRequired = smtpLookupRequired;
}

// Get/Set m_dateMask
_TCHAR const * CEntryTransformer::GetDateMask()
{
	return &m_dateMask[0];
}

void CEntryTransformer::SetDateMask(const _TCHAR *dateMask)
{
	::_tcscpy(m_dateMask, dateMask);
}

// Get/Set m_conversion
bool CEntryTransformer::IsConversionRequired()
{
	return m_conversion;
}

void CEntryTransformer::SetConversionRequired(bool conversion)
{
	m_conversion = conversion;
}

// Get/Set m_valueModifier
_TCHAR const * CEntryTransformer::GetValueModifier()
{
	return &m_valueModifier[0];
}

void CEntryTransformer::SetValueModifier(const _TCHAR *valueModifier)
{
	_tcscpy(m_valueModifier, valueModifier);
}

// Get/Set m_blankIfAbsent
bool CEntryTransformer::IsBlankIfAbsent()
{
	return m_blankIfAbsent;
}
void CEntryTransformer::SetBlankIfAbsent(bool blankIfAbsent)
{
	m_blankIfAbsent = blankIfAbsent;
}

// Get/Set m_binary
bool CEntryTransformer::IsBinary()
{
	return m_binary;
}
void CEntryTransformer::SetBinary(bool binary)
{
	m_binary = binary;
}

// Get/Set m_namedProperty
bool CEntryTransformer::IsNamedProperty()
{
	return m_namedProperty;
}
void CEntryTransformer::SetNamedProperty(bool namedProperty)
{
	m_namedProperty = namedProperty;
}

