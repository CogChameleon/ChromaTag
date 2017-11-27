#-----------------------------------------------------#
# Top Level CMakeLists.txt
# Starting point for building ChromaTag. Starts by building
# Utilities, then builds ChromaTag. OpenCV must already be 
# installed.
#
# Written by Joseph DeGol
#
# To Build:
#   mkdir <build_dir>
#   cd <build_dir>
#   cmake -D <Option1=1> -D <Option2=1> <Path_To_Source>
#   make
#
# Build Options:
#   -D CMAKE_BUILD_TYPE=Debug   : debug build; slow but works better with gdb and valgrind
#   -D CMAKE_BUILD_TYPE=Release : release build; faster run time
#   ...
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- Functions ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

#prompt function
function (prompt_top param1)
    message("")
    message("------------------------------------------------------------")
    message("  |-- ChromaTag :::: ${param1}")
endfunction()

function (prompt_bot param1)
    message("  |-- ChromaTag :::: ${param1}")
    message("------------------------------------------------------------")
    message("")
endfunction()

#update function
function (update_top param1)
    message("      o ChromaTag :: ${param1}")
endfunction()

#find library function
macro (find_lib param1)
	find_package( ${param1} QUIET)
	string(TOUPPER ${param1} param1_caps)
	if ( ${param1}_FOUND )
		update_top("Looking for Library ${param1}: Found")
		add_definitions(-D${param1_caps}_FOUND)
	elseif( ${param1_caps}_FOUND )
		update_top("Looking for Library ${param1_caps}: Found")
		add_definitions(-D${param1_caps}_FOUND)
	else()
		update_top("Looking for Library ${param1}: Miss")
	endif()
endmacro()

#find library pkg-config function
macro (find_lib_pkg param1)
	INCLUDE(FindPkgConfig)
	string(TOUPPER ${param1} param1_caps)
	#pkg_check_modules(${param1_caps} REQUIRED ${param1})
	pkg_check_modules(${param1_caps} QUIET ${param1})
	add_definitions(-D${param1_caps}_FOUND)
	if ( ${param1}_FOUND )
		update_top("Looking for Library ${param1}: Found")
		include_directories( ${${param1_caps}_INCLUDE_DIRS} )
		link_directories( ${${param1_caps}_DIRS} )
	elseif( ${param1_caps}_FOUND )
		update_top("Looking for Library ${param1_caps}: Found")
		include_directories( ${${param1_caps}_INCLUDE_DIRS} )
		link_directories( ${${param1_caps}_DIRS} )
	else()
		update_top("Looking for Library ${param1}: Miss")
	endif()
endmacro()

#recursive include
macro (include_directories_r top_include_dir)
	
    #for each file
	FILE(GLOB _ALL_FILES ${top_include_dir} ${top_include_dir}/*)
	FOREACH(_FILE ${_ALL_FILES})
	
	  #if is directory, and not the parent directory
	  GET_FILENAME_COMPONENT(_FILE_NAME ${_FILE} NAME)
	  GET_FILENAME_COMPONENT(_FILE_TOP_NAME ${top_include_dir} NAME)
	  IF(IS_DIRECTORY ${_FILE} AND NOT ${_FILE_NAME} STREQUAL ${_FILE_TOP_NAME})
	  
	    #first char of dir name
	    STRING(SUBSTRING ${_FILE_NAME} 0 1 _FILE_SUBSTR)
	    
	    #if not '.'
	    IF( NOT ${_FILE_SUBSTR} STREQUAL . )
		  INCLUDE_DIRECTORIES(${_FILE})
		  LINK_DIRECTORIES(${_FILE})
		  include_directories_r(${_FILE})
		ENDIF()
	  ENDIF()
	  
	ENDFOREACH()
endmacro()

#print include directories at a given level
macro (list_include_dirs current_dir)
	get_property(dirs DIRECTORY ${current_dir} PROPERTY INCLUDE_DIRECTORIES)
	foreach(dir ${dirs})
	  message(STATUS "dir='${dir}'")
	endforeach()
endmacro()

#subdirlist
macro(subdirlist result)
  file(GLOB children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/[^.]*")
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

#subfilelist
macro(subfilelist result nextdir)
  file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${nextdir} ${CMAKE_CURRENT_SOURCE_DIR}/${nextdir}/*.cpp)
  set(filelist "")
  foreach(child ${children})
	set(filelist ${filelist} ${child})
  endforeach()
  set(${result} ${filelist})
endmacro()

#Reset C and CXX Flags - Useful for external libraries that need different flags
macro(reset_compile_flags)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endmacro()

# remove a compiler flag
macro(remove_cxx_flag flag)
    string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	string(REPLACE "${flag}" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endmacro()

#-----------------------------------------------------#
#-----------------------------------------------------#
#------------------- End Functions -------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#----------------------- Build -----------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

#Version Required
cmake_minimum_required(VERSION 2.8)

#Entering Prompt
prompt_top("Entering Top Level CMakeLists.txt.")

if (UNIX)
    
    #Create Project
	project(ChromaTag)
	update_top("Creating Project: ChromaTag")
	
    #Unix Settings
    update_top("Unix Machine Found.")
	SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	
	#Release Setup
	if (RELEASE)
	    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
		SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
	endif()

	#Debug Setup
	if (DEBUG)
	    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
	    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")
    endif()
	
	#find required libraries
	find_lib(OpenCV)
    if(OpenCV_FOUND)
        set(Dependencies ${Dependencies} ${OpenCV_LIBS})
    endif(OpenCV_FOUND)

    #find optional libraries
	find_lib_pkg(lcm)
    if(LCM_FOUND)
        set(Dependencies ${Dependencies} ${LCM_LIBRARIES})
    endif(LCM_FOUND)
    
    #Add Utilities
    include_directories(Utilities)
    include_directories_r(Utilities)
    add_subdirectory(Utilities)
    
    #Add ChromaTag
    include_directories(Detector)
    include_directories_r(Detector)
    add_subdirectory(Detector)
    add_subdirectory(Tests)

    #ChromaTag Detector Executable
    update_top("Adding Executable: Run_ChromaTag_Detector")
    add_executable(Run_ChromaTag_Detector Run_JMD_ChromaTag_Detector.cpp)
    target_link_libraries(Run_ChromaTag_Detector ChromaTag Writer Options Timer ${Dependencies})

    #output note
    update_top("Libraries output to 'lib'")
    update_top("Executables output to 'bin'")

else()
    #Windows
    update_top("Windows Machine Found.")
    
    #Quit
    update_top("Implementation not tested on Windows. You will have to edit this CMakeLists.txt yourself to try with Windows.")
endif()

#Leaving Prompt
prompt_bot("Leaving Top Level CMakeLists.txt.")

#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#