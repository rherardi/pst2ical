// List.cpp: implementation of the CList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "List.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CList::CList()
{

}

CList::~CList()
{

}

LIST *CList::LST_Add(LIST **lst, _TCHAR *id, int dataSize)
{
	LIST *node = NULL;
	LIST *cur = *lst;
	LIST *prev = NULL;

	if ((node = (LIST *)malloc(sizeof(LIST))) != NULL) {
//	if ((node = (LIST *)GlobalAlloc(GMEM_FIXED, sizeof(LIST))) != NULL) {
//		node->id = id;
		if ((node->id = (_TCHAR *)malloc(dataSize * sizeof(TCHAR))) == NULL) {
			free(node);
			node = NULL;
			return node;
		}
		_tcscpy(node->id, id);
		if ((node->data = (_TCHAR *)malloc(dataSize * sizeof(TCHAR))) == NULL) {
//		if ((node->data = (char *)GlobalAlloc(GMEM_FIXED, dataSize)) == NULL) {
			free(node);
//			GlobalFree(node);
			node = NULL;
		}
		else {
			node->data[0] = '\0';
			node->size = dataSize;
			node->next = NULL;
			node->prev = NULL;

			while(cur != NULL &&
//				(cur->id < node->id)) {
				(_tcscmp(cur->id, node->id) < 0)) {
					prev = cur;
					cur = cur->next;
			}
			if (prev == NULL) {
				if (*lst != NULL) {
					node->next = *lst;
					node->next->prev = node;
				}
				*lst = node;
			}
			else {
				prev->next = node;
				node->prev = prev;
				if (cur != NULL) {
					node->next = cur;
					cur->prev = node;
				}
			}
		}
	}

	return node;
}/* LST_AllocAdd */

void CList::LST_Release(LIST **lst)
{
	LIST *cur = *lst;
	LIST *prev = 0;

	while (cur != 0) {
		prev = cur;
		cur = cur->next;
		if (prev->id != NULL)
			free(prev->id);
		if (prev->data != NULL)
			free(prev->data);
		if (prev != NULL)
			free(prev);
//		GlobalFree(prev->data);
		prev->id = NULL;
		prev->data = NULL;
//		GlobalFree(prev);
		prev = NULL;
	}
	*lst = 0;
}/* LST_Release */

LIST *CList::LST_Lookup(LIST **list, _TCHAR *id, bool exact)
{
	LIST *ptr;
	LIST *found = NULL;

	ptr = *list;

	while (ptr != NULL) {
		if (exact) {
			if (_tcscmp(id, ptr->id) == 0) {
				found = ptr;
				break;
			}
		}
		else {
//			_TCHAR *idPtr = _tcsstr(id, ptr->id);
//			if (idPtr != NULL && (idPtr - id) == 0) {
			if (_tcsnicmp(ptr->id, id, _tcslen(id)) == 0) {
				found = ptr;
				break;
			}
		}
		ptr = ptr->next;
	}

	return found;
}/* LST_Lookup */

LIST *CList::LST_Lookup(LIST **lst, int idInt, bool exact)
{
	_TCHAR id[32];
	wsprintf(id, _T("%d"), idInt);
	return LST_Lookup(lst, id, exact);
}

void CList::LST_Remove(LIST **lst, _TCHAR *id)
{
	LIST *item;
	item = LST_Lookup(lst, id);
	if (item != NULL) {
		if (item->prev != NULL)
			item->prev->next = item->next;
		if (item->next != NULL)
			item->next->prev = item->prev;
		free(item->data);
//		GlobalFree(item->data);
		item->data = NULL;
		if (item->prev == NULL)
			*lst = item->next;
		free(item);
//		GlobalFree(item);
		item = NULL;
	}
}

LIST *CList::LST_LookupAdd(LIST **lst, _TCHAR *id, int dataSize)
{
	LIST *item;
	if ((item = LST_Lookup(lst, id)) == NULL)
		item = LST_Add(lst, id, dataSize);
	else if (item->size != dataSize) {
		free(item->data);
//		GlobalFree(item->data);
		item->data = NULL;
		if ((item->data = (_TCHAR *)malloc(dataSize * sizeof(TCHAR))) == NULL) {
//		if ((item->data = (char *)GlobalAlloc(GMEM_FIXED, dataSize)) == NULL) {
			free(item);
//			GlobalFree(item);
			item = NULL;
		}
		item->size = dataSize;
	}
	return item;
}

LIST *CList::LST_LookupAdd(LIST **lst, int idInt, int dataSize)
{
	_TCHAR id[32];
	wsprintf(id, _T("%d"), idInt);
	return LST_LookupAdd(lst, id, dataSize);
}