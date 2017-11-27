#-----------------------------------------------------#
# Vision Level CMakeLists.txt
# branching into Vision folder
#
# Written by Joseph DeGol
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- Functions ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

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
prompt_lib("Entering Vision Level CMakeLists.txt.")


#----- Add Libraries -----#

#update_lib("Adding Library: Vision_Common")
#add_library(Vision_Common JMD_Vision_Common.cpp)

update_lib("Adding Library: Vision_Process")
add_library(Vision_Process JMD_Vision_Process.cpp)

update_lib("Adding Library: Vision_Image")
add_library(Vision_Image JMD_Vision_Image.cpp)
target_link_libraries(Vision_Image Writer)

update_lib("Adding Library: Vision_Point")
add_library(Vision_Point JMD_Vision_Point.cpp)

update_lib("Adding Library: Vision_Line")
add_library(Vision_Line JMD_Vision_Line.cpp)
target_link_libraries(Vision_Line Vision_Point)

#--- End Add Libraries ---#


#Leaving Prompt
prompt_lib("Leaving Vision Level CMakeLists.txt.")

#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#
