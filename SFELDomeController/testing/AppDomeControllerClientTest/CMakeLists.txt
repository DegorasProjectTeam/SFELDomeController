# **********************************************************************************************************************
# LIBZMQUTILS EXAMPLE CMAKELIST
# **********************************************************************************************************************

# ----------------------------------------------------------------------------------------------------------------------
# CONFIGURATION

# Config.
set(APP_NAME AppDomeControllerClientTest)
# --
set(EXAMPLE_INSTALL_PATH ${GLOBAL_LIBZMQUTILS_EXAMPLES_INSTALL_PATH}/${APP_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_NAME})

# ----------------------------------------------------------------------------------------------------------------------
# LAUNCHER

# Get the source files for client.
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Auxiliar libraries containers.
set(LIBS_DEBUG)
set(LIBS_RELEASE)

# Prepare the client library for link.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBS_DEBUG ${LIB_CLIENT_NAME})
else()
    set(LIBS_RELEASE ${LIB_CLIENT_NAME})
endif()

# Setup the client launcher.
macro_setup_launcher("${APP_NAME}" "${LIBS_RELEASE}" "${LIBS_DEBUG}" ${SOURCES})

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${APP_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# ----------------------------------------------------------------------------------------------------------------------

# **********************************************************************************************************************
