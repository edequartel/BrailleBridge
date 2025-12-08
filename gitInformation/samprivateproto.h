
#ifdef _WIN32_WCE
# define DLLIMPDEF	// if on WinCE use DEF file instead
# define ptr *
#else 
# ifdef WIN32
#  ifdef SAMDLL		// if we are compiling sam32.dll
#   define DLLIMPDEF _declspec(dllexport)
#  else				// else this file is included by a client
#   define DLLIMPDEF _declspec(dllimport)
#  endif // SAMDLL
#  define ptr *
# else // WIN32
// we must be compiling sam16.dll
# define ptr _far *
# define DLLIMPDEF
# endif // WIN32
#endif // _WIN32_WCE

#ifdef __cplusplus
extern "C" {
#endif

DWORD DLLIMPDEF WINAPI SamInitEx(
	DWORD type,
	HWND hwnd,
	WORD message,
	PIIK2 *piik2
	);

#ifdef __cplusplus
}
#endif

#undef ptr
