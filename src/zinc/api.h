#ifndef ZINC_API_H
#define ZINC_API_H

#if defined(_WIN32) || defined(WIN32)
	#ifdef ZINC_EXPORT
		#define ZINC_API __declspec(dllexport)
	#else
		#define ZINC_API __declspec(dllimport)
	#endif
#else
#define ZINC_API
#endif

#endif
