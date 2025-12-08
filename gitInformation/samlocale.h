// samlocale.h

DWORD GetLangIdFromLocale(LCID locale);
LCID  GetLocaleFromLangId(DWORD LangId);
DWORD GetCodePageFromLangId(DWORD LangId);
DWORD GetUserLangId();
int   OkLangIdInACP(DWORD langid);
DWORD SamLangId(DWORD langid); // converts a codepage specific language to a SAM language
