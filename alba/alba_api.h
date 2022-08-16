#ifndef _ALBA_API_H
#define _ALBA_API_H

	#ifdef ALBA_EXPORT
		#define ALBA_API __declspec(dllexport)
	#else
		#define ALBA_API __declspec(dllimport)
	#endif

#endif
