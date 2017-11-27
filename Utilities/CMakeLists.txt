#-----------------------------------------------------#
# Utilities Level CMakeLists.txt
# branching into Utilities folder
#
# Written by Joseph DeGol
#-----------------------------------------------------#





#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- Functions ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#

#---------- Prompt ----------#
#prompt function
function (prompt_uti param1)
    message("    |-- ChromaTag :::: ${param1}")
endfunction()

#update function
function (update_uti param1)
    message("        o ChromaTag :: ${param1}")
endfunction()

#prompt function
function (prompt_lib param1)
    message("      |-- ChromaTag :::: ${param1}")
endfunction()

#update function
function (update_lib param1)
    message("          o ChromaTag :: ${param1}")
endfunction()

#-------- End Prompt --------#

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
prompt_uti("Entering Utilities Level CMakeLists.txt.")


#Branch into each utility folder
subdirlist(SUBDIRS)
foreach(subdir ${SUBDIRS})
	#include_directories(${subdir})
    add_subdirectory(${subdir})
endforeach()


#Leaving Prompt
prompt_uti("Leaving Utilities Level CMakeLists.txt.")

#-----------------------------------------------------#
#-----------------------------------------------------#
#--------------------- End Build ---------------------#
#-----------------------------------------------------#
#-----------------------------------------------------#
