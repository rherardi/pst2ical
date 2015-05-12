// EntryTransformer.hpp: interface for the CEntryTransformer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTRYTRANSFORMER_HPP__FF5439F9_FA39_4E7B_B41F_F4CBD3A5E3D6__INCLUDED_)
#define AFX_ENTRYTRANSFORMER_HPP__FF5439F9_FA39_4E7B_B41F_F4CBD3A5E3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

class CEntryTransformer  
{
public:
	CEntryTransformer();
	virtual ~CEntryTransformer();

	// Get/Set m_name
	NCALXML_API _TCHAR const * GetName();
	NCALXML_API void SetName(const _TCHAR *name);

	// Get/Set m_placeholder
	NCALXML_API _TCHAR const * GetPlaceholder();
	NCALXML_API void SetPlaceholder(const _TCHAR *placeholder);

	// Get/Set m_resolveDocField
	NCALXML_API _TCHAR const * GetResolveDocField();
	NCALXML_API void SetResolveDocField(const _TCHAR *resolveDocField);

	// Get/Set m_resolveDocForm
	NCALXML_API _TCHAR const * GetResolveDocForm();
	NCALXML_API void SetResolveDocForm(const _TCHAR *resolveDocForm);

	// Get/Set m_resolveDocLookupField
	NCALXML_API _TCHAR const * GetResolveDocLookupField();
	NCALXML_API void SetResolveDocLookupField(const _TCHAR *resolveDocLookupField);

	// Get/Set m_pstItemProperty
	NCALXML_API bool const GetPstItemProperty();
	NCALXML_API void SetPstItemProperty(const bool pstItemProperty);

	// Get/Set m_multivalue
	NCALXML_API bool const IsMultivalue();
	NCALXML_API void SetMultivalue(const bool multivalue);

	// Get/Set m_onlyFirstValue
	NCALXML_API bool const GetOnlyFirstValue();
	NCALXML_API void SetOnlyFirstValue(const bool onlyFirstValue);

	// Get/Set m_multivalueSeparator
	NCALXML_API _TCHAR const * GetMultivalueSeparator();
	NCALXML_API void SetMultivalueSeparator(const _TCHAR *multivalueSeparator);

	// Get/Set m_orgHierarchy
	NCALXML_API bool const GetOrgHierarchy();
	NCALXML_API void SetOrgHierarchy(const bool orgHierarchy);

	// Get/Set m_extractCN
	NCALXML_API bool const GetExtractCN();
	NCALXML_API void SetExtractCN(const bool extractCN);

	// Get/Set m_smtpLookupRequired
	NCALXML_API bool const GetSmtpLookupRequired();
	NCALXML_API void SetSmtpLookupRequired(const bool smtpLookupRequired);

	// Get/Set m_dateMask
	NCALXML_API _TCHAR const * GetDateMask();
	NCALXML_API void SetDateMask(const _TCHAR *dateMask);

	// Get/Set m_valueModifier
	NCALXML_API bool IsConversionRequired();
	NCALXML_API void SetConversionRequired(bool conversion);

	// Get/Set m_valueModifier
	NCALXML_API _TCHAR const * GetValueModifier();
	NCALXML_API void SetValueModifier(const _TCHAR *valueModifier);

	// Get/Set m_blankIfAbsent
	NCALXML_API bool IsBlankIfAbsent();
	NCALXML_API void SetBlankIfAbsent(bool wrap);

	// Get/Set m_binary
	NCALXML_API bool IsBinary();
	NCALXML_API void SetBinary(bool binary);

	// Get/Set m_namedProperty
	NCALXML_API bool IsNamedProperty();
	NCALXML_API void SetNamedProperty(bool namedProperty);

protected:
	bool m_pstItemProperty;
	_TCHAR m_name[128];
	_TCHAR m_placeholder[256];
	_TCHAR m_resolveDocField[128];
	_TCHAR m_resolveDocForm[128];
	_TCHAR m_resolveDocLookupField[128];
	bool m_multivalue;
	bool m_onlyFirstValue;
	_TCHAR m_multivalueSeparator[64];
	bool m_orgHierarchy;
	bool m_extractCN;
	bool m_smtpLookupRequired;
	_TCHAR m_dateMask[64];
	bool m_conversion;
	_TCHAR m_valueModifier[64];
	bool m_blankIfAbsent;
	bool m_binary;
	bool m_namedProperty;
};

#endif // !defined(AFX_ENTRYTRANSFORMER_HPP__FF5439F9_FA39_4E7B_B41F_F4CBD3A5E3D6__INCLUDED_)
