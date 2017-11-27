#-----------------------------------------------------#
# Detector Level CMakeLists.txt
# branching into Detector folder
#
# Written by Joseph DeGol
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- Functions ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

#prompt function
function (prompt_det param1)
    message("    |-- ChromaTag :::: ${param1}")
endfunction()

#update function
function (update_det param1)
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
prompt_det("Entering Detector Level CMakeLists.txt.")


#dependencies
if(OpenCV_FOUND)
    set(Dependencies ${Dependencies} ${OpenCV_LIBS})
endif(OpenCV_FOUND)


#----- ChromaTag Detector Classes -----#

#common
update_lib("Adding Library: ChromaTag_Common")
add_library(ChromaTag_Common JMD_ChromaTag_Common.cpp)

#detect
update_lib("Adding Library: ChromaTag_Detect")
add_library(ChromaTag_Detect JMD_ChromaTag_Detect.cpp)
target_link_libraries(ChromaTag_Detect Math Vision_Process Vision_Point Vision_Line ChromaTag_Common ${Dependencies})

#decode
update_lib("Adding Library: ChromaTag_Decode")
add_library(ChromaTag_Decode JMD_ChromaTag_Decode.cpp)
target_link_libraries(ChromaTag_Decode ChromaTag_Common ${Dependencies})

#pose
update_lib("Adding Library: ChromaTag_Pose")
add_library(ChromaTag_Pose JMD_ChromaTag_Pose.cpp)
target_link_libraries(ChromaTag_Pose ChromaTag_Common ${Dependencies})

#--- End ChromaTag Detector Classes ---#


#----- ChromaTag Library -----#

#add ChromaTag
update_lib("Adding Library: ChromaTag")
add_library(ChromaTag JMD_ChromaTag.cpp)
target_link_libraries(ChromaTag Writer Timer ChromaTag_Common ChromaTag_Detect ChromaTag_Decode ChromaTag_Pose ${Dependencies})

#--- End ChromaTag Library ---#


#Leaving Prompt
prompt_det("Leaving Detector Level CMakeLists.txt.")

#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#
