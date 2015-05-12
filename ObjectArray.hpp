// ObjectArray.hpp: interface for the CObjectArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTARRAY_HPP__0212304D_7A33_48F8_BEA8_715E0FD4E094__INCLUDED_)
#define AFX_OBJECTARRAY_HPP__0212304D_7A33_48F8_BEA8_715E0FD4E094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef NMAILXML_EXPORTS
#define NMAILXML_API __declspec(dllexport)
#else
#define NMAILXML_API __declspec(dllimport)
#endif

class COutlineInfo;

class CDynList
{
public:
	CDynList();

	CDynList *next;
	CDynList *prev;
	COutlineInfo info;

	void DeleteData();
};

class CObjectArray
{
public:
	NMAILXML_API CObjectArray();
	NMAILXML_API CObjectArray(unsigned short size);
	NMAILXML_API CObjectArray& operator = (CObjectArray& arr);
	NMAILXML_API virtual ~CObjectArray();

//	void setElemAt(unsigned short index, void *object);
//	void* getElemAt(unsigned short index);
	NMAILXML_API void setElemAt(unsigned short index, COutlineInfo& info);
	NMAILXML_API COutlineInfo& getElemAt(unsigned short index);
	NMAILXML_API void removeElemAt(unsigned short index);
	NMAILXML_API void Resize(unsigned short newSize, bool preserve);
	NMAILXML_API void Print();

private:
	unsigned short m_size;
	CDynList m_list;

public:
	inline bool isValid() 
	{
		return m_size > 0;
	}

	unsigned short getLength()
	{
		return m_size;
	}
};


#endif // !defined(AFX_OBJECTARRAY_HPP__0212304D_7A33_48F8_BEA8_715E0FD4E094__INCLUDED_)
