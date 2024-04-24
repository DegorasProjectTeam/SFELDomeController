

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// EXPORT
// =====================================================================================================================
#if ((defined __WIN32__) || (defined _WIN32)) && (!defined LIBDOMECONTROLLER_STATIC)
    #ifdef LIBDOMECONTROLLER_LIBRARY
        #define LIBDOMECONTROLLER_EXPORT	__declspec(dllexport)
    #else
        #define LIBDOMECONTROLLER_EXPORT	__declspec(dllimport)
    #endif
#else
    /* Static libraries or non-Windows needs no special declaration. */
    # define LIBDOMECONTROLLER_EXPORT
#endif
// =====================================================================================================================
