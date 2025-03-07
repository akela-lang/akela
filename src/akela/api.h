#ifndef AKELA_API_H
#define AKELA_API_H

#if defined(WIN32)
	#ifdef AKELA_EXPORT
		#define AKELA_API __declspec(dllexport)
	#elif AKELA_IMPORT
		#define AKELA_API __declspec(dllimport)
	#else
		#define AKELA_API
	#endif
#else
	#define AKELA_API
#endif

#endif
