# **********************************************************************************************************************
# LIBZMQUTILS EXAMPLE CMAKELIST
# **********************************************************************************************************************

# ----------------------------------------------------------------------------------------------------------------------
# CONFIGURATION

# Config.
set(APP_NAME AppDomeControllerClientTest)
# --
set(TEST_DIR ${APP_NAME})
set(EXAMPLE_FULL_NAME ${APP_NAME})
set(EXAMPLE_INSTALL_PATH ${GLOBAL_LIBZMQUTILS_EXAMPLES_INSTALL_PATH}/${EXAMPLE_FULL_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXAMPLE_FULL_NAME})


# ----------------------------------------------------------------------------------------------------------------------
# PUBLISHER

# Get the header files for the client.


# Get the source files for client.
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


# Setup the client launcher.
macro_setup_launcher("${APP_NAME}" "${MODULES_GLOBAL_LIBS_OPTIMIZED}" "${MODULES_GLOBAL_LIBS_DEBUG}" 
		     ${SOURCES})

# Include directories.
#target_include_directories(${APP_PUBLISHER} PRIVATE ${CMAKE_SOURCE_DIR}/${MODULE_NAME}/${TEST_NAME}/includes)

# In mingw better do static linking of the libgcc, libwinpthread and libstd.
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")
    target_link_libraries(${APP_NAME} PRIVATE -static-libgcc -static-libstdc++ -static -lpthread)
endif()

# Link against LibZMQUtils.
macro_link_libzmqutils_default(${APP_NAME} PUBLIC)

# ----------------------------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------------------------

# **********************************************************************************************************************
