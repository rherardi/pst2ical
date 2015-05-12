/* LIST.H */
#ifndef _LIST_H_
#define _LIST_H_

/* list of fields */
#define LIST struct list
LIST
{
	int id;
	_TCHAR *data;
	int size;
	LIST *prev;
	LIST *next;
};

#ifdef __cplusplus
extern "C" {
#endif

LIST *LST_Add(LIST **lst, int id, int dataSize);
void LST_Release(LIST **lst);
LIST *LST_Lookup(LIST **list, int id);
void LST_Remove(LIST **lst, int id);
LIST *LST_LookupAdd(LIST **lst, int id, int dataSize);

#define PARAMS_LEN		8

#define ID_HEADER		0

// option id
#define ID_OPT_TEXT1	32
#define ID_OPT_TEXT2	33
#define ID_OPT_TEXT3	34
#define ID_OPT_TEXT4	35
#define ID_OPT_TEXT		36

#define ID_OPT_SERVER	37
#define ID_OPT_DOMAIN	38
#define ID_OPT_PSTMASK	39
#define ID_OPT_PSTFILENAME	40

#define ID_OPT_PROPBASE 100

//----------------------------INI settings begin

// INI X-Mozilla
#define ID_INI_XMOZILLAST_S	230
//----------------------------INI settings end

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif
