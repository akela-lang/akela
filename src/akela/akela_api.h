#ifndef _AKELA_API_H
#define _AKELA_API_H

#if defined(WIN32)
	#ifdef AKELA_EXPORT
		#define AKELA_API __declspec(dllexport)
	#else
		#define AKELA_API __declspec(dllimport)
	#endif
#else
	#define AKELA_API
#endif

#endif
