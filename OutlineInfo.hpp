// OutlineInfo.hpp: interface for the COutlineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLINEINFO_HPP__B0D5B2B9_67CC_4877_9015_D7081F4D191F__INCLUDED_)
#define AFX_OUTLINEINFO_HPP__B0D5B2B9_67CC_4877_9015_D7081F4D191F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutlineInfo  
{
public:
	COutlineInfo();
	virtual ~COutlineInfo();
	COutlineInfo& operator = (COutlineInfo& info);

protected:
      int m_Default;	// with bool it doesn't work when passing object as parameter
      wchar_t m_Name[256];
      int m_Visible;
      unsigned short m_ContentType1;
      unsigned short m_ContentType2;
      wchar_t m_ContentValue[256];
      wchar_t m_ImageName[256];

 	  unsigned short m_itemOrdinalNum;	// we could have several $SiteMapList items;
	  unsigned long m_offsetInItem;		// offset inside #SiteMapList item
	  unsigned short m_spanSize;		// if our toolbar occupies more than 1 $SiteMapList items
	  unsigned long m_fullLength;		// full length with all inside entries
	  unsigned short m_entryOrdinalNum;	// either toolbar or toolbar entry ordinal number

public:
      inline void SetDefault(const bool Default)
      {
         m_Default = Default;
      }

      inline void SetName(const _TCHAR * Name)
      {
		  if (Name != NULL) {
         _tcscpy(m_Name, Name);
		  }
      }

      inline void SetVisible(bool Visible) 
      {
         m_Visible = Visible;
      }

      inline int GetDefault() const  
      {
         return m_Default;
      }


      inline _TCHAR const * GetName()  
      {
         return &m_Name[0];
      }


      inline int GetVisible() const  
      {
         return m_Visible;
      }

	  inline void SetContentType1(unsigned short ContentType1)
	  {
			m_ContentType1 = ContentType1;
	  }
	  inline unsigned short GetContentType1()
	  {
			return m_ContentType1;
	  }

	  inline void SetContentType2(unsigned short ContentType2)
	  {
			m_ContentType2 = ContentType2;
	  }
	  inline unsigned short GetContentType2()
	  {
			return m_ContentType2;
	  }

	  inline void SetContentValue(_TCHAR *ContentValue)
	  {
		  if (ContentValue != NULL) {
			_tcscpy(m_ContentValue, ContentValue);
		  }
	  }
	  inline _TCHAR const * GetContentValue()
	  {
			return &m_ContentValue[0];
	  }

	  inline void SetImageName(_TCHAR *ImageName)
	  {
		  if (ImageName != NULL) {
			_tcscpy(m_ImageName, ImageName);
		  }
	  }
	  inline _TCHAR const * GetImageName()
	  {
			return &m_ImageName[0];
	  }

		inline void SetItemOrdinalNum(unsigned short ItemOrdinalNum)
		{
			m_itemOrdinalNum = ItemOrdinalNum;
		}

		inline unsigned short GetItemOrdinalNum()
		{
			return m_itemOrdinalNum;
		}

		inline void SetOffsetInItem(unsigned long OffsetInItem)
		{
			m_offsetInItem = OffsetInItem;
		}

		inline unsigned long GetOffsetInItem()
		{
			return m_offsetInItem;
		}

		inline void SetSpanSize(unsigned short SpanSize)
		{
			m_spanSize = SpanSize;
		}

		inline unsigned short GetSpanSize()
		{
			return m_spanSize;
		}

		inline void SetFullLength(unsigned long FullLength)
		{
			m_fullLength = FullLength;
		}

		inline unsigned long GetFullLength()
		{
			return m_fullLength;
		}

		inline void SetEntryOrdinalNum(unsigned short EntryOrdinalNum)
		{
			m_entryOrdinalNum = EntryOrdinalNum;
		}

		inline unsigned short GetEntryOrdinalNum()
		{
			return m_entryOrdinalNum;
		}
};

#endif // !defined(AFX_OUTLINEINFO_HPP__B0D5B2B9_67CC_4877_9015_D7081F4D191F__INCLUDED_)
