// ConversionRuleEntry.hpp: interface for the CConversionRuleEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSIONRULEENTRY_HPP__B3F6DAC4_8A9D_4BC3_8DE7_3BA1D7FFA951__INCLUDED_)
#define AFX_CONVERSIONRULEENTRY_HPP__B3F6DAC4_8A9D_4BC3_8DE7_3BA1D7FFA951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NCALXML_EXPORTS
#define NCALXML_API __declspec(dllexport)
#else
#define NCALXML_API __declspec(dllimport)
#endif

class CConversionRuleEntry  
{
public:
	CConversionRuleEntry();
	virtual ~CConversionRuleEntry();

	// Get/Set m_input
	NCALXML_API _TCHAR const * GetInput();
	NCALXML_API void SetInput(const _TCHAR *input);

	// Get/Set m_output
	NCALXML_API _TCHAR const * GetOutput();
	NCALXML_API void SetOutput(const _TCHAR *output);

	// Get/Set m_fieldPresent
	NCALXML_API bool IsFieldPresent();
	NCALXML_API void SetFieldPresent(bool fieldPresent);

	NCALXML_API bool IsDefault();
	NCALXML_API void SetDefault(bool def);

protected:
	_TCHAR m_input[256];
	_TCHAR m_output[256];
	bool m_fieldPresent;
	bool m_def;
};

#endif // !defined(AFX_CONVERSIONRULEENTRY_HPP__B3F6DAC4_8A9D_4BC3_8DE7_3BA1D7FFA951__INCLUDED_)
