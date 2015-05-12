// ObjectArray.cpp: implementation of the CObjectArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutlineInfo.hpp"
#include "ObjectArray.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynList::CDynList()
{
	next = NULL;
	prev = NULL;
}

void CDynList::DeleteData()
{
	// not implemented right now
	this->info.~COutlineInfo();
}

CObjectArray::CObjectArray()
{
	m_size = 0;
	m_list = CDynList();
}

CObjectArray::CObjectArray(unsigned short size)
{
	m_size = size;
	m_list = CDynList();
}

CObjectArray& CObjectArray::operator = (CObjectArray& arr)
{
//	printf("I'm called here!\n");
	this->m_list = arr.m_list;
	this->m_size = arr.m_size;
	return *this;
}

CObjectArray::~CObjectArray()
{
	for (unsigned short i=0; i<m_size; i++) {
	}
}

void CObjectArray::setElemAt(unsigned short index, COutlineInfo& info)
{
//	wchar_t prompt[128];
//	wsprintf(prompt, L"index: %d, size: %d", index, m_size);

//	MessageBoxW(NULL, prompt, L"setElemAt1", MB_OK);
	if (index == 0) {
//		_tprintf(_TEXT("inside setElem: %x %x %s\n"), info.GetName()[0], info.GetName()[1], info.GetName());
		m_list.info = info;
//		wprintf(L"info name: %s\n", m_list.info.GetName());
	}
	else if (index < m_size) {
//		MessageBoxW(NULL, L"setElemAt2", L"setElemAt1", MB_OK);
		CDynList *listNode = &m_list;
//		MessageBoxW(NULL, L"setElemAt3", L"setElemAt1", MB_OK);
		int i = 1;
		while (i <= index) {
			if (listNode->next == NULL) {
//				CDynList newNode = CDynList();
				CDynList *newNode = new CDynList();
				(*listNode).next = newNode;
//				if (i == 1) {
//					m_list.next = &newNode;
//				}
//				MessageBoxW(NULL, L"setElemAt4", L"setElemAt1", MB_OK);
			}
//			else {
//				_tprintf(_TEXT("setElemAt44: %s\n"), listNode->next->info.GetName());
//			}
			listNode = listNode->next;
			i++;
		}
		(*listNode).info = info;
//		_tprintf(_TEXT("setElemAt5: %s %s\n"), m_list.info.GetName(), m_list.next->info.GetName());
//		MessageBoxW(NULL, L"setElemAt5", L"setElemAt1", MB_OK);
	}
}

COutlineInfo& CObjectArray::getElemAt(unsigned short index)
{
	COutlineInfo *pOutlineInfo = NULL;
	if (index == 0) {
		return m_list.info;
	}
	else if (index < m_size) {
		CDynList *listNode = &m_list;
		int i = 1;
		while (listNode->next != NULL && i <= index) {
			i++;
			listNode = listNode->next;
		}
		return listNode->info;
	}
	return *pOutlineInfo;
}

void CObjectArray::removeElemAt(unsigned short index)
{
	CDynList *listNode = &m_list;
	CDynList *prevNode = &m_list;
	int i = 0;
	if (index < m_size) {
		while (i < index) {
			i++;
			prevNode = listNode;
			listNode = listNode->next;
		}
//		printf("removeElemAt1\n");
		CDynList *nextNode = listNode->next;
//		if (nextNode != NULL) {
//			_tprintf(L"nextNode: %s\n", nextNode->info.GetName());
//		}
//		printf("removeElemAt2\n");
		listNode->DeleteData();
//		printf("removeElemAt3\n");
		m_size--;
//		printf("removeElemAt4\n");
		prevNode->next = nextNode;
//		_tprintf(L"removeElemAt m_list head: %s\n", m_list.info.GetName());
	}
}

void CObjectArray::Resize(unsigned short newSize, bool preserve)
{
	m_size = newSize;
	if (newSize > m_size) {
		if (!preserve) {
			m_list.DeleteData();
		}
	}
	else if (newSize < m_size) {
		if (!preserve) {
			m_list.DeleteData();
		}
		else {
			// not implemented yet
		}
	}
}

void CObjectArray::Print()
{
	_tprintf(_T("Content of Array:\n"));
	if (m_size == 0) {
		_tprintf(_T("Content is empty!\n"));
	}
	else {
		CDynList *listNode = &m_list;
		int i = 0;
		while (listNode != NULL && i <= m_size) {
			_tprintf(_T("Name: %s, value: %s\n"), listNode->info.GetName(), listNode->info.GetContentValue());
			i++;
			listNode = listNode->next;
		}
	}
}