# **********************************************************************************************************************
# SFELDOMECONTROLLER TESTS CMAKELIST
# **********************************************************************************************************************

# ----------------------------------------------------------------------------------------------------------------------
# CONFIGURATION

# Set the installation test path.
set(GLOBAL_LIBSFELDOMECONTROLLER_TESTS_INSTALL_PATH ${MODULES_GLOBAL_INSTALL_BIN_PATH}/testing)

# ----------------------------------------------------------------------------------------------------------------------
# ADD THE MODULES (SIMPLE AND COMPLEX TESTS)

# Add all the Modules.
add_subdirectory(AppDomeControllerTest)
add_subdirectory(AppDomeControllerServerTest)
add_subdirectory(AppDomeControllerClientTest)

# **********************************************************************************************************************
