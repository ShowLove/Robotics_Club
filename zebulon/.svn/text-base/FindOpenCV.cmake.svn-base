# Macros/Scripts for locating OSG SDK Files

#MESSAGE(STATUS "Finding OSG SDK...")

# Clear return values in case the module is loaded more than once.
#SET(OpenCV_FOUND FALSE)		# No OSG Files located yet
#SET(OpenCV_LIBRARIES "")		# No OSG Files located yet
SET(OpenCV_CXX_FLAGS "")		# No OSG Files located yet
#SET(OpenCV_BINARIES "")		# Binary files and DLLS to copy.
IF(WIN32)
	SET(OpenCV_LIBRARY_PREFIX "lib")
	SET(OpenCV_ARCHIVE_EXTENSION ".lib")
	SET(OpenCV_BINARY_EXTENSION ".dll")
ELSE()
	SET(OpenCV_LIBRARY_PREFIX "lib")
	SET(OpenCV_ARCHIVE_EXTENSION ".so")
	SET(OpenCV_BINARY_EXTENSION "")
ENDIF()

SET(OpenCV_LIBNAMES 
		cv cvaux cxcore highgui ml
		opencv_core opencv_calib3d opencv_contrib 
		opencv_features2d opencv_flann opencv_gpu 
		opencv_highgui opencv_imgproc opencv_legacy
		opencv_ml opencv_objdetect opencv_video)
		
IF( MSVC10)
	SET(SEARCH_PATHS 
			"/usr/local"
				"/usr"
				"E:\\software\\active\\3rdParty\\OpenCV\\09.10.21\\msvc10"
				"C:\\software\\active\\3rdParty\\OpenCV\\09.10.21\\msvc10"
				"C:\\active\\3rdParty\\OpenCV\\09.10.21\\msvc10"
				"C:\\Active\\3rdParty\\OpenCV\\09.10.21\\msvc10"
				"C:\\OpenCV2.0\\include\\")
ELSE()
	SET(SEARCH_PATHS 
				"/usr/local"
					"/usr"
					"E:\\software\\active\\3rdParty\\OpenCV\\09.10.21\\msvc9"
					"C:\\software\\active\\3rdParty\\OpenCV\\09.10.21\\msvc9"
					"C:\\active\\3rdParty\\OpenCV\\09.10.21\\msvc9"
					"C:\\Active\\3rdParty\\OpenCV\\09.10.21\\msvc9"
					"C:\\OpenCV2.0\\include\\")
ENDIF()

FIND_PATH(OpenCV_DIR
			NAMES
				include/opencv/cv.h
			PATHS
				${SEARCH_PATHS})

IF(OpenCV_DIR)
	SET(OpenCV_INCLUDE_DIRS ${OpenCV_DIR}/include)
	SET(OpenCV_LIBRARY_DIRS ${OpenCV_DIR}/lib)
	IF(EXISTS ${OpenCV_LIBRARY_DIRS}/opencv)
	     SET(OpenCV_LIBRARY_DIRS ${OpenCV_DIR}/lib/opencv)
	ENDIF()
	IF(WIN32)
		SET(OpenCV_BINARY_DIRS ${OpenCV_DIR}/bin)
	ELSE()
		SET(OpenCV_BINARY_DIRS ${OpenCV_DIR}/lib)
	ENDIF(WIN32)
ENDIF(OpenCV_DIR)

IF(NOT OpenCV_FOUND) # Don't run again if already found.
	IF(OpenCV_LIBRARY_DIRS AND OpenCV_INCLUDE_DIRS)
			
		IF(WIN32)
			#MESSAGE(STATUS ${OpenCV_LIBRARY_DIRS}/*${OpenCV_ARCHIVE_EXTENSION})
			FILE(GLOB LIBRARY_FILES ${OpenCV_LIBRARY_DIRS}/*${OpenCV_ARCHIVE_EXTENSION})
			FOREACH(LIB ${LIBRARY_FILES})
			#MESSAGE(STATUS ${LIB})
			STRING(LENGTH ${LIB} LIB_STR_LEN)
			MATH(EXPR LIB_SUB_START "${LIB_STR_LEN} - 5")
			STRING(SUBSTRING ${LIB} ${LIB_SUB_START} 5 LIB_STR_END)
			IF(${LIB_STR_END} MATCHES "d${OpenCV_ARCHIVE_EXTENSION}" OR ${LIB_STR_END} MATCHES "D${OpenCV_ARCHIVE_EXTENSION}")
				SET(OpenCV_LIBRARIES 
						${OpenCV_LIBRARIES}
						debug ${LIB})
				#MESSAGE(STATUS "DEBUG ${LIB}")
			ELSE()
				SET(OpenCV_LIBRARIES 
						${OpenCV_LIBRARIES}
						optimized ${LIB})
				#MESSAGE(STATUS "RELEASE ${LIB}")
			ENDIF()
		ENDFOREACH(LIB)
		ELSE()
			#MESSAGE(STATUS ${OpenCV_LIBRARY_DIRS}/*${OpenCV_ARCHIVE_EXTENSION})
			FOREACH(LIB ${OpenCV_LIBNAMES})
				#MESSAGE(STATUS ${LIB})
				SET(FULL_LIB "${OpenCV_LIBRARY_DIRS}/${OpenCV_LIBRARY_PREFIX}${LIB}${OpenCV_ARCHIVE_EXTENSION}")
				#MESSAGE(STATUS ${FULL_LIB})
				IF (EXISTS ${FULL_LIB})
					SET(OpenCV_LIBRARIES 
							${OpenCV_LIBRARIES}
							${FULL_LIB})
					#MESSAGE(STATUS "DEBUG ${FULL_LIB}")
				ENDIF()
			ENDFOREACH(LIB)
		ENDIF(WIN32)
        IF(WIN32)
		FILE(GLOB_RECURSE OpenCV_BINARIES ${OpenCV_BINARY_DIRS}/*.dll)
		#MESSAGE(STATUS ${OpenCV_BINARIES})
		INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
		LINK_DIRECTORIES(${OpenCV_LIBRARY_DIRS})
		OPTION(COPY_OPENCV_BINARIES "Copy OpenCV Binaries to Output Directory" ${COPY_OPENCV_BINARIES})
		IF(COPY_OPENCV_BINARIES)
			FOREACH(BINARY ${OpenCV_BINARIES})
				EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy 
							${BINARY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
							RESULT_VARIABLE COPY_RESULT
							ERROR_VARIABLE ERROR)
				IF(NOT COPY_RESULT EQUAL 0)
					MESSAGE(STATUS "FindOpenCV - ${ERROR}")
				ENDIF()
			ENDFOREACH(BINARY)
		ENDIF(COPY_OPENCV_BINARIES)
        ENDIF(WIN32)
		#MESSAGE(STATUS "FindOpenCV - Found OSG SDK")
        IF(OpenCV_LIBRARIES)
            SET(OpenCV_FOUND TRUE)	
        ENDIF()
	ELSE()
		MESSAGE(STATUS "FindOpenCV - Could Not Locate OpenCV SDK") 
		MESSAGE(STATUS "FindOpenCV - Only Log Available Reader Code Will Build") 
	ENDIF(OpenCV_LIBRARY_DIRS AND OpenCV_INCLUDE_DIRS)
ENDIF()

IF(NOT OpenCV_FOUND)
    find_package(OpenCV)
    set(OpenCV_LIBRARIES ${OpenCV_LIBS})
ENDIF()





