# **********************************************************************************************************************
# Updated 24/04/2024
# **********************************************************************************************************************

# **********************************************************************************************************************

# Unset previous final values.
# You can use this values in your CMakeLists once setted.
# --
unset(LIBSERIAL_FOUND CACHE)          # Library found flag.
unset(LIBSERIAL_LIBRARIES CACHE)      # List of libraries (full path with names).
unset(LIBSERIAL_INCLUDES CACHE)       # List of includes (full path with names).
unset(LIBSERIAL_LIBRARY_DIRS CACHE)   # List of libraries directories (without names).
unset(LIBSERIAL_INCLUDE_DIRS CACHE)   # List of includes directories (without names).

# Unset previous auxiliar values.
# --
unset(LIBSERIAL_INCLUDE_PATH CACHE)
unset(LIBSERIAL_INCLUDE_FILE CACHE)
unset(LIBSERIAL_LIBRARY_PATH CACHE)
unset(LIBSERIAL_LIBRARY_FILE CACHE)
# **********************************************************************************************************************

macro(macro_search_file_private file_name current_path result_var)

    # Initial search for the file in the current directory
    file(GLOB_RECURSE found_files RELATIVE "${current_path}" "${current_path}/*/${file_name}")
    if(found_files)
    else()
        file(GLOB_RECURSE found_files RELATIVE "${current_path}" "${current_path}/${file_name}")
    endif()

    if(found_files)
        list(GET found_files 0 first_found_file)
        get_filename_component(first_found_dir "${first_found_file}" DIRECTORY)
        set(${result_var} "${current_path}/${first_found_dir}")
    else()
        # Recursively search in subdirectories
        file(GLOB children RELATIVE "${current_path}" "${current_path}/*")
        foreach(child IN LISTS children)
            if(IS_DIRECTORY "${current_path}/${child}")
                macro_search_file_private(${file_name} "${current_path}/${child}" ${result_var})
                if(${result_var})
                    break()  # Stop if the file has been found
                endif()
            endif()
        endforeach()
    endif()
endmacro()

# **********************************************************************************************************************

macro(macro_search_file_in_paths_private file_name paths result_var_path result_var_file)
    # Split file_name into directory and the actual file name
    string(FIND "${file_name}" "/" last_slash REVERSE)
    if(last_slash GREATER -1)
        string(SUBSTRING "${file_name}" 0 ${last_slash} sub_path)
        string(SUBSTRING "${file_name}" ${last_slash} -1 actual_file_name)
        if(actual_file_name MATCHES "^/")
            string(SUBSTRING ${actual_file_name} 1 -1 actual_file_name)
        endif()
        message(STATUS "  Search for: ${actual_file_name}")
    else()
        set(sub_path "")
        set(actual_file_name "${file_name}")
        message(STATUS "  Search for: ${file_name}")
    endif()

    set(local_result)
    foreach(dir ${paths})
        if(IS_DIRECTORY ${dir})
            macro_search_file_private("${actual_file_name}" "${dir}" local_result)
        endif()
        if(local_result)
            string(REGEX REPLACE "/$" "" local_result "${local_result}")
            set(${result_var_file} "${local_result}/${actual_file_name}")
            # Determine if the sub_path should be removed from the result
            string(REGEX REPLACE "/${sub_path}$" "" trimmed_path "${local_result}")
            set(${result_var_path} ${trimmed_path})  # Just set the directory path
            break()  # Stop if the file has been found
        endif()
    endforeach()
endmacro()

# **********************************************************************************************************************

# Find the library.

# Log.
message(STATUS "Configuring LibSerial...")

# Find for Windows plattform.
if(WIN32)

    message(FATAL_ERROR "  Not supported OS.")

else()

    # GLOB for directories that contain 'LIBSERIAL' in their names.
    file(GLOB LIBSERIAL_CANDIDATE_DIRS
        "../LIBSERIAL*"
        "../../LIBSERIAL*"
        "~/LIBSERIAL*")

    # List of paths to search.
    set(HARDCODED_PATHS
        "/opt"
        NO_DEFAULT_PATH)

    # Check if a forced search path is provided externally
    if(LIBSERIAL_FORCED_SEARCH_PATHS)
        message(STATUS "Forced LIBSERIAL search path is set to: ${LIBSERIAL_FORCED_SEARCH_PATHS}")
        set(SEARCH_PATHS ${LIBSERIAL_FORCED_SEARCH_PATHS})
    else()
       # Combine both lists into one if forced path is not set
       set(SEARCH_PATHS ${LIBSERIAL_CANDIDATE_DIRS} ${HARDCODED_PATHS})
    endif()

    # Search for includes.
    macro_search_file_in_paths_private("serial/serial.h" "${SEARCH_PATHS}" LIBSERIAL_INCLUDE_PATH LIBSERIAL_INCLUDE_FILE)
    macro_search_file_in_paths_private("serial/v8stdint.h" "${SEARCH_PATHS}" LIBSERIAL_INCLUDE_PATH_2 LIBSERIAL_INCLUDE_FILE_2)

    # Find the library.
    macro_search_file_in_paths_private("libserial.a" "${SEARCH_PATHS}" LIBSERIAL_LIBRARY_PATH LIBSERIAL_LIBRARY_FILE)

    # Checks.
    if(LIBSERIAL_INCLUDE_FILE AND LIBSERIAL_LIBRARY_FILE AND LIBSERIAL_INCLUDE_FILE_2)

        set(LIBSERIAL_FOUND TRUE)
        set(LIBSERIAL_LIBRARIES ${LIBSERIAL_LIBRARY_FILE})
        set(LIBSERIAL_LIBRARY_DIRS ${LIBSERIAL_LIBRARY_PATH})
        set(LIBSERIAL_INCLUDES ${LIBSERIAL_INCLUDE_FILE} ${LIBSERIAL_INCLUDE_FILE_2})
        set(LIBSERIAL_INCLUDE_DIRS ${LIBSERIAL_INCLUDE_PATH} ${LIBSERIAL_INCLUDE_PATH_2})

    else()
        message(FATAL_ERROR "  Could not find LibSerial library.")
    endif()

endif()

# Logs.
message(STATUS "  LibSerial information:")
message(STATUS "    LibSerial found: ${LIBSERIAL_FOUND}")
message(STATUS "    LibSerial libraries: ${LIBSERIAL_LIBRARIES}" )
message(STATUS "    LibSerial libraries dirs: ${LIBSERIAL_LIBRARY_DIRS}")
message(STATUS "    LibSerial includes: ${LIBSERIAL_INCLUDES}" )
message(STATUS "    LibSerial includes dirs: ${LIBSERIAL_INCLUDE_DIRS}")

# Mark as advanced.
mark_as_advanced(LIBSERIAL_LIBRARIES LIBSERIAL_INCLUDES LIBSERIAL_LIBRARY_DIRS LIBSERIAL_INCLUDE_DIRS)

# **********************************************************************************************************************
