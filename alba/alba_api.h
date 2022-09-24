#ifndef _ALBA_API_H
#define _ALBA_API_H

#if defined(WIN32)
	#ifdef ALBA_EXPORT
		#define ALBA_API __declspec(dllexport)
	#else
		#define ALBA_API __declspec(dllimport)
	#endif
#else
	#define ALBA_API
#endif

#endif
