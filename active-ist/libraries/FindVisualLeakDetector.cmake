# Macros/Scripts for locating JAUS++ Files

# Clear return values in case the module is loaded more than once.
#SET(VLD_FOUND FALSE)			# Found?
#SET(VLD_SET_DEPENDENCY TRUE)	# Build as dependency
#SET(VLD_DEPENDENCY "")			# Value to use (name of project) for dependency.
#SET(VLD_CXX_FLAGS -DVLD_ENABLED)# Compiler Flags
SET(VLD_BINARIES "")			# Binary files


IF(WIN32)
	FIND_PATH(VLD_ROOT_DIR
				NAMES 
					include/vld.h
				PATHS
					"C:/Program Files (x86)/Visual Leak Detector/"
			  )
	
	IF(VLD_ROOT_DIR)
		SET(VLD_FOUND TRUE)
		SET(VLD_CXX_FLAGS -DVLD_ENABLED)# Compiler Flags
		INCLUDE_DIRECTORIES(${VLD_ROOT_DIR}/include)
		IF(CMAKE_CL_64) # Get version for architecture type
			LINK_DIRECTORIES(${VLD_ROOT_DIR}/lib/Win64)
		ELSE()
			LINK_DIRECTORIES(${VLD_ROOT_DIR}/lib/Win32)
		ENDIF(CMAKE_CL_64)
		ADD_DEFINITIONS(${VLD_CXX_FLAGS})
	ENDIF(VLD_ROOT_DIR)
ENDIF()


