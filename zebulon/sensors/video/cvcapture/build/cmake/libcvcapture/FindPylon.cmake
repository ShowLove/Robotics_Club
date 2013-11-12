
# Macros/Scripts for locating Basler Pylon Library Files

# Clear return values in case the module is loaded more than once.
#set(Pylon_FOUND FALSE)			# Found?
#set(Pylon_CXX_FLAGS -DPylon_ENABLED)	# Compiler Flags
set(Pylon_LIBRARIES "")			# Library files
set(Pylon_BINARIES "")			# Binary files


if(WIN32)

elseif(UNIX) # Linux System

	find_path(Pylon_ROOT_DIR
			NAMES
				include/pylon/PylonIncludes.h
			PATHS
				/opt/pylon/)
	if(Pylon_ROOT_DIR)
		set(Pylon_FOUND TRUE)
		set(Pylon_CXX_FLAGS -DPylon_ENABLED -DUSE_GIGE)
		set(Pylon_INCLUDE_DIRS
			${Pylon_ROOT_DIR}/include
                        ${Pylon_ROOT_DIR}/library/CPP/include
                        ${Pylon_ROOT_DIR}/genicam/library/CPP/include/)
		#message(STATUS ${CMAKE_SYSTEM_PROCESSOR})
		if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")		
			set(Pylon_LIBRARY_DIRS
				${Pylon_ROOT_DIR}/lib64 
		                ${Pylon_ROOT_DIR}/genicam/bin/Linux64_x64
		         	${Pylon_ROOT_DIR}/genicam/bin/Linux64_x64/GenApi/Generic)
			link_directories(/opt/pylon/genicam/bin/Linux64_x64)
			link_directories(/opt/pylon/genicam/bin/Linux64_x64/GenApi/Generic)
		else()
			set(Pylon_LIBRARY_DIRS
				${Pylon_ROOT_DIR}/lib 
				${Pylon_ROOT_DIR}/genicam/bin/Linux32_i86
				${Pylon_ROOT_DIR}/genicam/bin/Linux32_i86/GenApi/Generic)
		endif()
		#message(STATUS ${Pylon_LIBRARY_DIRS})
		set(Pylon_LINKER_FLAGS "-Wl,-E")
                #message(STATUS ${Pylon_ROOT_DIR})
                set(Pylon_LIBRARIES
			pylonbase
			GCBase_gcc40_v2_1)

	endif()
endif(WIN32)

