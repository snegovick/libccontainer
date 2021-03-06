#define CLIST_TRUE 1
#define CLIST_FALSE 0
#define CLIST_ENOTFOUND -1
#define CLIST_ENOTEMPTY -2

#define CERR_OK 1
#define CERR_GENERAL_ERROR 0
#define CERR_ENOTFOUND -1
#define CERR_ENOTEMPTY -2
#define CERR_ENOMEM -3
#define CERR_NOENTRIES -4

#ifndef clist_malloc
#define clist_malloc malloc
#endif

#ifndef clist_free
#define clist_free free
#endif
