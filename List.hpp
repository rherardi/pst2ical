// List.hpp: interface for the CList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIST_HPP__CC32454D_8469_4441_985A_1FF9655D37F0__INCLUDED_)
#define AFX_LIST_HPP__CC32454D_8469_4441_985A_1FF9655D37F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* list of fields */
#define LIST struct list
LIST
{
	_TCHAR *id;
	_TCHAR *data;
	int size;
	LIST *prev;
	LIST *next;
};

class CList  
{
public:
	CList();
	virtual ~CList();

	LIST *LST_Add(LIST **lst, _TCHAR *id, int dataSize);
	void LST_Release(LIST **lst);
	LIST *LST_Lookup(LIST **list, _TCHAR *id, bool exact = true);
	LIST *LST_Lookup(LIST **list, int id, bool exact = true);
	void LST_Remove(LIST **lst, _TCHAR *id);
	LIST *LST_LookupAdd(LIST **lst, _TCHAR *id, int dataSize);
	LIST *LST_LookupAdd(LIST **lst, int id, int dataSize);

//	LIST *lst;
};

// option id
#define ID_OPT_TEXT1	32

#define ID_OPT_SERVER	37
#define ID_OPT_DOMAIN	38
#define ID_OPT_PSTMASK	39
#define ID_OPT_PSTFILENAME	40

#define ID_OPT_PROPBASE 100

#endif // !defined(AFX_LIST_HPP__CC32454D_8469_4441_985A_1FF9655D37F0__INCLUDED_)
