#include "stdafx.h"

#include <stdlib.h>
#include "list.h"

LIST *LST_Add(LIST **lst, int id, int dataSize)
{
	LIST *node = NULL;
	LIST *cur = *lst;
	LIST *prev = NULL;

	if ((node = (LIST *)malloc(sizeof(LIST))) != NULL) {
//	if ((node = (LIST *)GlobalAlloc(GMEM_FIXED, sizeof(LIST))) != NULL) {
		node->id = id;
		if ((node->data = malloc(dataSize * sizeof(TCHAR))) == NULL) {
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
				(cur->id < node->id)) {
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

void LST_Release(LIST **lst)
{
	LIST *cur = *lst;
	LIST *prev = 0;

	while (cur != 0) {
		prev = cur;
		cur = cur->next;
		if (prev->data != NULL)
			free(prev->data);
		if (prev != NULL)
			free(prev);
//		GlobalFree(prev->data);
		prev->data = NULL;
//		GlobalFree(prev);
		prev = NULL;
	}
	*lst = 0;
}/* LST_Release */

LIST *LST_Lookup(LIST **list, int id)
{
	LIST *ptr;
	LIST *found = NULL;

	ptr = *list;

	while (ptr != NULL) {
		if (ptr->id == id) {
			found = ptr;
			break;
		}
		ptr = ptr->next;
	}

	return found;
}/* LST_Lookup */

void LST_Remove(LIST **lst, int id)
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

LIST *LST_LookupAdd(LIST **lst, int id, int dataSize)
{
	LIST *item;
	if ((item = LST_Lookup(lst, id)) == NULL)
		item = LST_Add(lst, id, dataSize);
	else if (item->size != dataSize) {
		free(item->data);
//		GlobalFree(item->data);
		item->data = NULL;
		if ((item->data = malloc(dataSize * sizeof(TCHAR))) == NULL) {
//		if ((item->data = (char *)GlobalAlloc(GMEM_FIXED, dataSize)) == NULL) {
			free(item);
//			GlobalFree(item);
			item = NULL;
		}
		item->size = dataSize;
	}
	return item;
}