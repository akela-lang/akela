#if defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__) || defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#define IS_UNIX 1
#else
#define IS_UNIX 0
#endif

#if defined(_WIN32) || defined(WIN32)
#define IS_WIN 1
#else
#define IS_WIN 0
#endif
