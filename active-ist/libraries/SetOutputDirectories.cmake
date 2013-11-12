IF(WIN32)
	IF(NOT MSVC AND NOT MINGW)
	MESSAGE(STATUS "YOU MUST CALL PROJECT BEFORE INCLUDING THIS FILE.")
	ENDIF()
ENDIF()


# Try and find lib and bin folders locally.

IF(NOT FOUND_OUTPUT_DIRECTORY_ROOT)
	GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/bin ABSOLUTE)
	IF(NOT IS_DIRECTORY ${OUTPUT_DIRECTORY_ROOT})
		#MESSAGE(STATUS "Not ${OUTPUT_DIRECTORY_ROOT}")
		GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/../../bin ABSOLUTE)
		IF(NOT IS_DIRECTORY ${OUTPUT_DIRECTORY_ROOT})
			#MESSAGE(STATUS "Not ${OUTPUT_DIRECTORY_ROOT}")
			GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/../../../bin ABSOLUTE)
			IF(IS_DIRECTORY ${OUTPUT_DIRECTORY_ROOT})
				GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/../../../ ABSOLUTE)
				SET(FOUND_OUTPUT_DIRECTORY_ROOT 1)
			ELSE()
				#MESSAGE(STATUS "Failed")
				SET(OUTPUT_DIRECTORY_ROOT ${CMAKE_BINARY_DIR})
			ENDIF()
		ELSE()
			GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/../../ ABSOLUTE)
			SET(FOUND_OUTPUT_DIRECTORY_ROOT 1)
		ENDIF()
	ELSE()
		GET_FILENAME_COMPONENT(OUTPUT_DIRECTORY_ROOT ${CMAKE_SOURCE_DIR}/ ABSOLUTE)
		SET(FOUND_OUTPUT_DIRECTORY_ROOT 1)
	ENDIF()
	#MESSAGE(STATUS "Binary Output Path: ${OUTPUT_ROOT_DIR}")
ENDIF()

SET(OUTPUT_DIRECTORY_ROOT ${OUTPUT_DIRECTORY_ROOT} CACHE PATH "Root output folder for binaries." )

IF(NOT OUTPUT_DIRECTORY_ROOT)
	SET(OUTPUT_DIRECTORY_ROOT ${CMAKE_BINARY_DIR})
ENDIF()

IF(WIN32)
	SET(CPACK_NSIS_INSTALL_ROOT  "C:\\\\UCF")
	#MESSAGE(STATUS "CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
ENDIF(WIN32)

# Set output folders for binaries and libraries.
#MESSAGE(STATUS "${OUTPUT_DIRECTORY_ROOT}/bin")
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIRECTORY_ROOT}/bin CACHE STRING "Executable output folder.")
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_DIRECTORY_ROOT}/lib CACHE STRING "Library (e.g. *.lib, *.so) output folder.")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
IF(WIN32)
	# Make sure DLL files are in the same directory as executeables in Windows
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_ROOT}/bin)
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY_ROOT}/bin)
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY_ROOT}/bin)
	SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
ELSE()
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})
ENDIF(WIN32)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
					${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}
					${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}
					${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
					${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}
					${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})

# In other systems besides MSVC, a lib prefix is
# added to all 

IF(WIN32)
	IF(MINGW)
		SET(LIB_PREFIX "")
	ELSE()
		SET(LIB_PREFIX "lib")
	ENDIF(MINGW)
ELSE()
	SET(LIB_PREFIX "")
ENDIF(WIN32)
#MESSAGE(STATUS "Lib Prefix: " ${LIB_PREFIX})


if(WIN32)
    set(LIB_INSTALL_DEST lib)
    set(BIN_INSTALL_DEST bin)
    set(INCLUDE_INSTALL_DEST include/)
else(UNIX)
    set(LIB_INSTALL_DEST lib/active)
    set(BIN_INSTALL_DEST lib/active)
    set(INCLUDE_INSTALL_DEST include/active/)
endif(WIN32)


MACRO (TODAY RESULT YEAR MONTH DAY)
	IF (WIN32)
		EXECUTE_PROCESS(COMMAND "cmd.exe" "/c" "date" "/T" OUTPUT_VARIABLE ${RESULT})
		string(REGEX REPLACE "....../../..(..).*" "\\1" ${YEAR} ${${RESULT}} )
		string(REGEX REPLACE "....(..)/../.*" "\\1" ${MONTH} ${${RESULT}} )
		string(REGEX REPLACE "....../(..)/.*" "\\1" ${DAY} ${${RESULT}} )
	ELSEIF(UNIX)
		#EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        EXECUTE_PROCESS(COMMAND "date" "+%d" OUTPUT_VARIABLE ${DAY})
        EXECUTE_PROCESS(COMMAND "date" "+%y" OUTPUT_VARIABLE ${YEAR})
        EXECUTE_PROCESS(COMMAND "date" "+%m" OUTPUT_VARIABLE ${MONTH})
        string(REPLACE "\n" "" ${YEAR} ${${YEAR}} )
        string(REPLACE "\n" "" ${DAY} ${${DAY}} )
        string(REPLACE "\n" "" ${MONTH} ${${MONTH}} )
	ELSE (WIN32)
		MESSAGE(SEND_ERROR "Date not implemented")
		SET(${RESULT} 000000)
		SET(${YEAR} 000000)
		SET(${MONTH} 000000)
		SET(${DAY} 000000)
	ENDIF (WIN32)
ENDMACRO (TODAY)
