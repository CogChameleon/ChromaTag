#-----------------------------------------------------#
# Tests Level CMakeLists.txt
# branching into Tests folder
#
# Written by Joseph DeGol
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- Functions ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

#prompt function
function (prompt_cts param1)
    message("    |-- ChromaTag :::: ${param1}")
endfunction()

#update function
function (update_cts param1)
    message("        o ChromaTag :: ${param1}")
endfunction()

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

#Entering Prompt
prompt_cts("Entering Tests Level CMakeLists.txt.")

#ChromaTag Dependencies
if(OpenCV_FOUND)
    set(Dependencies ${Dependencies} ${OpenCV_LIBS})
endif(OpenCV_FOUND)
if(LCM_FOUND)
    set(Dependencies ${Dependencies} ${LCM_LIBRARIES})
endif(LCM_FOUND)

#Test Color Space
update_cts("Adding Executable: Run_TestColorSpace")
add_executable(Run_TestColorSpace Run_JMD_TestColorSpace.cpp)
target_link_libraries(Run_TestColorSpace Writer Options ${Dependencies})

#Test Color Conversion Speed
update_cts("Adding Executable: Run_TestColorConversion")
add_executable(Run_TestColorConversion Run_JMD_TestColorConversion.cpp)
target_link_libraries(Run_TestColorConversion Writer Options Vision_Process ${Dependencies})

#Leaving Prompt
prompt_cts("Leaving Tests Level CMakeLists.txt.")

#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#
