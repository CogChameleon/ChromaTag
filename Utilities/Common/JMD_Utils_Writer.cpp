/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Writer: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Writer.cpp
 A class for writing project specific output to the terminal
 ================================================================
 
 //====================================================================//
 */





//====================================================================//
//====================================================================//
//============================ Preamble ==============================//
//====================================================================//
//====================================================================//


//---------------------------------------------------------------//
//------------------------- Includes ----------------------------//
//---------------------------------------------------------------//

#include "JMD_Utils_Writer.hpp"

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//----------------------- Namespaces ----------------------------//
//---------------------------------------------------------------//

//namespaces

//---------------------------------------------------------------//
//---------------------- end Namespaces -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//------------------------- Globals -----------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//------------------------ end Globals --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//------------------- Function Prototypes -----------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//------------------ end Function Prototypes --------------------//
//---------------------------------------------------------------//

//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//





//================================================================================//
//================================================================================//
//==================== JMD Utils Writer Class Implementation =====================//
//================================================================================//
//================================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//

std::string JMD::JMD_Utils_Writer::Set_Color(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param)
{
	//set up
    std::stringstream s;
    s << "\033[0m\033[";
    if (!text_color_param && !highlight_color_param && !underline_param && !bold_param){
        s << "0"; // reset colors if no params
    }
    else
    {
		//bold
		if (bold_param)
		{
			s << "1";
			if (underline_param || highlight_color_param || text_color_param) { s << ";"; }
		}
		
	
		//underline
		if (underline_param)
		{
			s << "4";
			if (highlight_color_param || text_color_param) { s << ";"; }
		}
	
		//text color
		if (text_color_param) {
			s << 29 + text_color_param;
			if (highlight_color_param) { s << ";"; }
		}
	
		//highlight color
		if (highlight_color_param) {
			s << 39 + highlight_color_param;
		}
	}
	    
    //return
    s << "m";
    return s.str();
}

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_Utils_Writer::JMD_Utils_Writer() : 
	AccentStyles(4,0),
    NoteStyles(4,0), 
    WarningStyles(4,0), 
    ErrorStyles(4,0),
    ListStyles(4,0),
    InOutPromptStyles(4,0), 
    FunctionPromptStyles(4,0), 
    ProgressStyles(4,0), 
    ProgramHeaderStyles(4,0), 
    ProgramTextStyles(4,0),
    QueryStyles(4,0),
    InputStyles(4,0),
    FileNumber(0)
{ }
/*------------- end Constructors -------------*/


/*--------------- Destructors ---------------*/
JMD::JMD_Utils_Writer::~JMD_Utils_Writer() { }
/*------------- end Destructors -------------*/


/*--------------- Writing Methods ---------------*/

/*----- Writer Open -----*/
int  JMD::JMD_Utils_Writer::Writer_Open(std::string file_name_param, std::ios_base::openmode mode_param)
{
	//create new fstream and push onto vector
	FilesVector.push_back(new std::fstream);
	
	//open file
	(FilesVector.at(FileNumber))->open(file_name_param,mode_param);
	
	//success
	if( (FilesVector.at(FileNumber))->is_open() )
	{
		//map
		FilesMap.push_back(FilesVector.size()-1);
		
		//increment FileNumber
		FileNumber++;
	
		//return FileNumber
		return FileNumber-1;
	}
	
	//failure
	else
	{
		//pop fstream
		FilesVector.pop_back();
		
		//return failure
		return -1;
	}
}
/*--- End Writer Open ---*/

/*----- Writer Open -----*/
int  JMD::JMD_Utils_Writer::Writer_Close(int file_num_param)
{
	//variables
	int idx_ct = 0;
	int file_idx = -1;
	
	//search map to make sure file number is valid and get index in map
	for(std::vector<int>::iterator it = FilesMap.begin(); it != FilesMap.end(); ++it)
	{
		//found file number? map to file index, break
		if( *it == file_num_param)
		{
			file_idx = idx_ct;
			break;
		}
		
		//next file
		idx_ct++;
	}
	
	//valid file #
	if(file_idx >= 0)
	{
		//close file and pop fstream
		(FilesVector.at(file_idx))->close();
		FilesVector.erase( FilesVector.begin() + file_idx );
		
		//update map
		FilesMap.erase( FilesMap.begin() + file_idx );
	
		//return
		return 1;
	}
	else { return -1; }
}
/*--- End Writer Open ---*/

/*----- Writer Line -----*/
void JMD::JMD_Utils_Writer::Writer_Line(int file_num_param, std::string text_param)
{
	//valid file #
	if(1) {	(*FilesVector.at(file_num_param)) << text_param << std::endl; }
}
void JMD::JMD_Utils_Writer::Writer_Line(int file_num_param, std::vector<std::string> text_param, std::string delimeter_param)
{
	//valid file #
	if(1)
	{
		//write each element
		for( std::vector<std::string>::iterator it = text_param.begin(); it != text_param.end(); ++it)
		{
			(*FilesVector.at(file_num_param)) << *it << delimeter_param;
		}
		
		//endl
		(*FilesVector.at(file_num_param)) << std::endl;
	}
}
/*--- End Writer Line ---*/
        
/*----- Writer Continued -----*/
void JMD::JMD_Utils_Writer::Writer_Continued(std::string continued_param, bool newline_param, unsigned int backspace_param)
{
	std::string overwrite = "";
	for(unsigned int i=0; i < backspace_param; i++) { overwrite += '\b'; }
	
	std::cout << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << overwrite << continued_param << Set_Color();
	if(newline_param) { std::cout << std::endl; }
}
/*--- End Writer Continued ---*/

/*----- Writer Note -----*/
void JMD::JMD_Utils_Writer::Writer_Note(std::string class_param, std::string method_param, std::string message_param, bool newline_param, bool overwrite_param)
{
	std::string overwrite = "";
	if(overwrite_param) { overwrite = "\r"; }
	
	std::cout << overwrite << "    " 
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << class_param
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " ::: "
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << method_param
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " :: "
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << "Note"
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " : " 
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << message_param << Set_Color();
	if(newline_param) { std::cout<<std::endl; }
}
/*--- End Writer Note ---*/

/*-----Writer Warning -----*/
void JMD::JMD_Utils_Writer::Writer_Warning(std::string class_param, std::string method_param, std::string message_param, bool exit_param)
{
	std::cout << "    " 
		 << Set_Color( (Color)WarningStyles[0], (Color)WarningStyles[1], (bool)WarningStyles[2], (bool)WarningStyles[3]) << class_param
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " ::: " 
		 << Set_Color( (Color)WarningStyles[0], (Color)WarningStyles[1], (bool)WarningStyles[2], (bool)WarningStyles[3]) << method_param 
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " :: " 
		 << Set_Color( (Color)WarningStyles[0], (Color)WarningStyles[1], (bool)WarningStyles[2], (bool)WarningStyles[3]) << "Warning"
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " : " 
		 << Set_Color( (Color)WarningStyles[0], (Color)WarningStyles[1], (bool)WarningStyles[2], (bool)WarningStyles[3]) << message_param << Set_Color() << std::endl;
	if(exit_param) { exit(-1); }
}
/*--- End Writer Warning ---*/

/*----- Writer Error -----*/
void JMD::JMD_Utils_Writer::Writer_Error(std::string class_param, std::string method_param, std::string message_param, bool exit_param)
{
	std::cout << "    " 
		 << Set_Color( (Color)ErrorStyles[0], (Color)ErrorStyles[1], (bool)ErrorStyles[2], (bool)ErrorStyles[3]) << class_param
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " ::: " 
		 << Set_Color( (Color)ErrorStyles[0], (Color)ErrorStyles[1], (bool)ErrorStyles[2], (bool)ErrorStyles[3]) << method_param 
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " :: " 
		 << Set_Color( (Color)ErrorStyles[0], (Color)ErrorStyles[1], (bool)ErrorStyles[2], (bool)ErrorStyles[3]) << "Error"
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " : " 
		 << Set_Color( (Color)ErrorStyles[0], (Color)ErrorStyles[1], (bool)ErrorStyles[2], (bool)ErrorStyles[3]) << message_param << Set_Color() << std::endl;
	if(exit_param) { exit(-1); }
}
/*--- End Writer Error ---*/

/*----- Writer List -----*/
void JMD::JMD_Utils_Writer::Writer_List(std::string class_param, std::string method_param, std::string list_param, std::vector<std::string> list_items_param)
{
	std::cout << "    "
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << class_param 
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " ::: " 
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << method_param 
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " :: " 
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << "List" 
		 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " : " 
		 << Set_Color( (Color)NoteStyles[0], (Color)NoteStyles[1], (bool)NoteStyles[2], (bool)NoteStyles[3]) << list_param << std::endl;
	for( std::vector<std::string>::iterator iter = list_items_param.begin(); iter != list_items_param.end(); ++iter)
	{
		std::cout << Set_Color() << "        " 
			      << Set_Color( (Color)ListStyles[0], (Color)ListStyles[1], (bool)ListStyles[2], (bool)ListStyles[3]) << *iter << std::endl;
	}
	std::cout << std::endl;
}
/*--- End Writer List ---*/
 	
/*----- Writer Program Help -----*/
void JMD::JMD_Utils_Writer::Writer_ProgramHelp(std::string program_param, std::string author_param, std::string organization_param, std::string version_param, std::string description_param, std::vector<std::string> options_flag_param, std::vector<std::string> options_setter_param)
{	
	/*----- program info -----*/
	
	//name
	std::cout << std::endl << std::endl;
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << std::left << std::setw(14) << "Program Name: ";
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	std::cout << program_param << std::endl;
	
	//version
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << std::left << std::setw(14) << "Version: "; 
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	std::cout << version_param << std::endl;
	
	//author
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << std::left << std::setw(14) << "Author: ";
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	std::cout << author_param << std::endl;
	
	//organization
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << std::left << std::setw(14) << "Organization: ";
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	std::cout << organization_param << std::endl;
	std::cout << std::endl;
	/*--- end program info ---*/
	
	//description
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << "Description: " << std::endl;
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	std::cout << description_param << std::endl;
	std::cout << std::endl;
	
	//options_flag
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << "Flag Options:" << std::endl;
	std::cout << "(Example Usage: ./exec --flag1 --flag2)" << std::endl;
	std::cout << "(Example Usage: ./exec -f1 -f2)" << std::endl;
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	for( std::vector<std::string>::iterator iter = options_flag_param.begin(); iter != options_flag_param.end(); ++iter )
	{
		std::cout << *iter << std::endl;
	}
	std::cout << std::endl;
	
	//options_setters
	std::cout << Set_Color( (Color)ProgramHeaderStyles[0], (Color)ProgramHeaderStyles[1], (bool)ProgramHeaderStyles[2], (bool)ProgramHeaderStyles[3]);
	std::cout << "Setter Options: " << std::endl;
	std::cout << "(Example Usage: ./exec --setter1 setter1_value --setter2 setter2_value)" << std::endl;
	std::cout << "(Example Usage: ./exec -s1 setter1_value -s2 setter2_value)" << std::endl;
	std::cout << Set_Color( (Color)ProgramTextStyles[0], (Color)ProgramTextStyles[1], (bool)ProgramTextStyles[2], (bool)ProgramTextStyles[3]);
	for( std::vector<std::string>::iterator iter = options_setter_param.begin(); iter != options_setter_param.end(); ++iter )
	{
		std::cout << *iter << std::endl;
	}
	std::cout << std::endl;
	std::cout << Set_Color() << std::endl << std::endl;
}
/*--- End Writer Program Help ---*/
 			
/*----- Writer Intro Prompt -----*/
void JMD::JMD_Utils_Writer::Writer_IntroPrompt(std::string program_param, std::string author_param, std::string organization_param, std::string version_param, std::string options_param, std::string note_param)
{
	std::string tmp;
	if(program_param.size() > 26) { program_param = program_param.substr(1,26); }
	if(author_param.size()  > 20) { author_param  = author_param.substr(1,20);  }
	if(organization_param.size() > 36) { organization_param = organization_param.substr(1,36); }
	if(version_param.size() > 10) { version_param = version_param.substr(1,10); }
	if(options_param.size() > 46) { options_param = options_param.substr(1,46); }
	if(note_param.size()    > 46) { note_param    = note_param.substr(1,46); }
	
	std::cout << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "--                                              --" << std::endl;
	tmp = program_param + ", " + version_param;
	std::cout << "- " 
		      << Set_Color( (Color)InOutPromptStyles[0], (Color)InOutPromptStyles[1], (bool)InOutPromptStyles[2], (bool)InOutPromptStyles[3]) << std::left << std::setw(46) << tmp 
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " -" << std::endl;
	tmp = author_param + ", " + organization_param;
	std::cout << "- " 
		      << Set_Color( (Color)InOutPromptStyles[0], (Color)InOutPromptStyles[1], (bool)InOutPromptStyles[2], (bool)InOutPromptStyles[3]) << std::left << std::setw(46) << tmp
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " -" << std::endl;
	std::cout << "--                                              --" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "- " 
		      << Set_Color( (Color)InOutPromptStyles[0], (Color)InOutPromptStyles[1], (bool)InOutPromptStyles[2], (bool)InOutPromptStyles[3]) << std::left << std::setw(47) << note_param 
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << "-" << std::endl;
	std::cout << "- " 
		      << Set_Color( (Color)InOutPromptStyles[0], (Color)InOutPromptStyles[1], (bool)InOutPromptStyles[2], (bool)InOutPromptStyles[3]) << std::left << std::setw(47) << options_param 
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << "-" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << Set_Color() << std::endl;
}
/*--- End Writer Intro Prompt ---*/

/*----- Writer Outro Prompt -----*/
void JMD::JMD_Utils_Writer::Writer_OutroPrompt(std::string program_param)
{
	if(program_param.size() > 36) { program_param = program_param.substr(1,36); }
	
	std::cout << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "-- " 
		      << Set_Color( (Color)InOutPromptStyles[0], (Color)InOutPromptStyles[1], (bool)InOutPromptStyles[2], (bool)InOutPromptStyles[3]) << "Exiting " << std::left << std::setw(37) << program_param 
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << "--" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << Set_Color() << std::endl;
}
/*--- End Writer Outro Prompt ---*/

/*----- Writer Function Prompt -----*/
void JMD::JMD_Utils_Writer::Writer_FunctionPrompt(std::string function_param)
{
	if(function_param.size() > 46) { function_param = function_param.substr(1,46); }
	
	std::cout << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "- " 
		      << Set_Color( (Color)FunctionPromptStyles[0], (Color)FunctionPromptStyles[1], (bool)FunctionPromptStyles[2], (bool)FunctionPromptStyles[3]) << std::left << std::setw(47) << function_param 
		      << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << "-" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << Set_Color() << std::endl;
}
/*--- End Writer Function Prompt ---*/

/*----- Writer Progress -----*/
void JMD::JMD_Utils_Writer::Writer_Progress(int current_iteration_param, int total_iteration_param)
{
	//set color
	std::cout << Set_Color( (Color)ProgressStyles[0], (Color)ProgressStyles[1], (bool)ProgressStyles[2], (bool)ProgressStyles[3]);
	
	//starting
	if (current_iteration_param == 0) { std::cout << "    0% |"; }
	
	//ending
	else if (current_iteration_param == total_iteration_param) { std::cout << " -- | 100%" << Set_Color() << std::endl; }
	
	//in between
	else
	{
		int Mark10 = total_iteration_param / 10;
		int tmp = 0;
		for(int ct=1; ct < 10; ct++)
		{
			tmp = Mark10 * ct;
			if(current_iteration_param == tmp) 
			{
				std::cout << " -- " << 10*ct;
				return;
			}
		}
	}
	
}
/*--- End Writer Progress ---*/
 		
/*------------- End Writing Methods -------------*/


/*--------------- Property Methods ---------------*/
        
/*----- Set Accent Color -----*/
void JMD::JMD_Utils_Writer::SetAccentColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	AccentStyles[0] = (int)text_color_param;
	AccentStyles[1] = (int)highlight_color_param;
	AccentStyles[2] = (int)bold_param;
	AccentStyles[3] = (int)underline_param;
}
/*--- End Set Accent Color ---*/

/*----- Set Intro Outro Prompt Color -----*/
void JMD::JMD_Utils_Writer::SetInOutPromptColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	InOutPromptStyles[0] = (int)text_color_param;
	InOutPromptStyles[1] = (int)highlight_color_param;
	InOutPromptStyles[2] = (int)bold_param;
	InOutPromptStyles[3] = (int)underline_param;
}
/*----- Set Intro Outro Prompt Color -----*/

/*----- Set Function Prompt Color -----*/
void JMD::JMD_Utils_Writer::SetFPromptColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	FunctionPromptStyles[0] = (int)text_color_param;
	FunctionPromptStyles[1] = (int)highlight_color_param;
	FunctionPromptStyles[2] = (int)bold_param;
	FunctionPromptStyles[3] = (int)underline_param;
}
/*----- Set Function Prompt Color -----*/

/*----- Set Note Color -----*/
void JMD::JMD_Utils_Writer::SetNoteColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	NoteStyles[0] = (int)text_color_param;
	NoteStyles[1] = (int)highlight_color_param;
	NoteStyles[2] = (int)bold_param;
	NoteStyles[3] = (int)underline_param;
}
/*--- End Set Note Color ---*/

/*----- Set Warning Color -----*/
void JMD::JMD_Utils_Writer::SetWarningColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	WarningStyles[0] = (int)text_color_param;
	WarningStyles[1] = (int)highlight_color_param;
	WarningStyles[2] = (int)bold_param;
	WarningStyles[3] = (int)underline_param;
}
/*--- End Set Warning Color ---*/

/*----- Set Error Color -----*/
void JMD::JMD_Utils_Writer::SetErrorColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	ErrorStyles[0] = (int)text_color_param;
	ErrorStyles[1] = (int)highlight_color_param;
	ErrorStyles[2] = (int)bold_param;
	ErrorStyles[3] = (int)underline_param;
}
/*--- End Set Error Color ---*/

/*----- Set List Color -----*/
void JMD::JMD_Utils_Writer::SetListColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	ListStyles[0] = (int)text_color_param;
	ListStyles[1] = (int)highlight_color_param;
	ListStyles[2] = (int)bold_param;
	ListStyles[3] = (int)underline_param;
}
/*--- End Set List Color ---*/

/*----- Set Progress Color -----*/
void JMD::JMD_Utils_Writer::SetProgressColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	ProgressStyles[0] = (int)text_color_param;
	ProgressStyles[1] = (int)highlight_color_param;
	ProgressStyles[2] = (int)bold_param;
	ProgressStyles[3] = (int)underline_param;
}
/*--- End Set Progress Color ---*/

/*----- Set Program Header Color -----*/
void JMD::JMD_Utils_Writer::SetProgramHeaderColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	ProgramHeaderStyles[0] = (int)text_color_param;
	ProgramHeaderStyles[1] = (int)highlight_color_param;
	ProgramHeaderStyles[2] = (int)bold_param;
	ProgramHeaderStyles[3] = (int)underline_param;
}
/*--- End Set Program Header Color ---*/

/*----- Set Program Text Color -----*/
void JMD::JMD_Utils_Writer::SetProgramTextColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	ProgramTextStyles[0] = (int)text_color_param;
	ProgramTextStyles[1] = (int)highlight_color_param;
	ProgramTextStyles[2] = (int)bold_param;
	ProgramTextStyles[3] = (int)underline_param;
}
/*--- End Set Program Text Color ---*/

/*----- Set Query Color -----*/
void JMD::JMD_Utils_Writer::SetQueryColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	QueryStyles[0] = (int)text_color_param;
	QueryStyles[1] = (int)highlight_color_param;
	QueryStyles[2] = (int)bold_param;
	QueryStyles[3] = (int)underline_param;
}
/*--- End Set Query Color ---*/

/*----- Set Input Color -----*/
void JMD::JMD_Utils_Writer::SetInputColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param, bool bold_param, bool underline_param)
{
	InputStyles[0] = (int)text_color_param;
	InputStyles[1] = (int)highlight_color_param;
	InputStyles[2] = (int)bold_param;
	InputStyles[3] = (int)underline_param;
}
/*--- End Set Input Color ---*/

/*----- Reset Color -----*/
void JMD::JMD_Utils_Writer::ResetColor()
{
	Set_Color();
	std::fill(AccentStyles.begin(),			AccentStyles.end(),			0);
	std::fill(NoteStyles.begin(),           NoteStyles.end(),           0);
	std::fill(WarningStyles.begin(),        WarningStyles.end(),        0);
	std::fill(ErrorStyles.begin(),          ErrorStyles.end(),          0);
	std::fill(ListStyles.begin(),			ListStyles.end(),			0);
	std::fill(InOutPromptStyles.begin(),    InOutPromptStyles.end(),    0);
	std::fill(FunctionPromptStyles.begin(), FunctionPromptStyles.end(), 0);
	std::fill(ProgressStyles.begin(),       ProgressStyles.end(),       0);
	std::fill(ProgramHeaderStyles.begin(),  ProgramHeaderStyles.end(),  0);
	std::fill(ProgramTextStyles.begin(),    ProgramTextStyles.end(),    0);
	std::fill(QueryStyles.begin(),			QueryStyles.end(),			0);
	std::fill(InputStyles.begin(),			InputStyles.end(),			0);
}
/*--- End Reset Color ---*/

/*------------- End Property Methods -------------*/


/*--------------- Conversion Methods ---------------*/

/*----- Bool To String -----*/
std::string JMD::JMD_Utils_Writer::BoolToString(bool bool_param)
{
	return bool_param ? "true" : "false";
}
/*--- End Bool To String ---*/

/*----- Int To String -----*/
std::string JMD::JMD_Utils_Writer::IntToString(int int_param)
{
	std::stringstream ss;
	ss << int_param;
	return ss.str();
}
std::string JMD::JMD_Utils_Writer::Int64ToString(int64_t int64_param)
{
	std::stringstream ss;
	ss << int64_param;
	return ss.str();
}
/*--- end Int To String ---*/

/*----- Float To String -----*/
std::string JMD::JMD_Utils_Writer::FloatToString(float float_param, int precision_param)
{
	std::stringstream ss;
	ss << std::setprecision(precision_param) << float_param;
	return ss.str();
}
/*--- End Float To String ---*/

/*----- Double To String -----*/
std::string JMD::JMD_Utils_Writer::DoubleToString(double double_param, int precision_param)
{
	std::stringstream ss;
	ss << std::setprecision(precision_param) << double_param;
	return ss.str();
}
/*--- End Double To String ---*/

/*----- String To Bool -----*/
bool JMD::JMD_Utils_Writer::StringToBool( const std::string &string_param )
{
	return StringToUpperString(string_param).compare("TRUE") == 0 ? true : false;
}
/*--- End String To Bool ---*/

/*----- String To Int -----*/
int JMD::JMD_Utils_Writer::StringToInt( const std::string &string_param )
{
	std::stringstream ss(string_param);
	int result;
	return ss >> result ? result : 0;
}
/*--- End String To Int ---*/

/*----- String To Float -----*/
float JMD::JMD_Utils_Writer::StringToFloat( const std::string &string_param )
{
	std::stringstream ss(string_param);
	float result;
	return ss >> result ? result : 0.0f;
}
/*--- End String To Float ---*/

/*----- String To Double -----*/
double JMD::JMD_Utils_Writer::StringToDouble( const std::string &string_param )
{
	std::stringstream ss(string_param);
	double result;
	return ss >> result ? result : 0.0;
}
/*--- End String To Double ---*/

/*----- String To Upper String -----*/
std::string JMD::JMD_Utils_Writer::StringToUpperString( std::string string_param )
{
	transform(string_param.begin(),string_param.end(),string_param.begin(), ::toupper);
	return string_param;
}
/*--- end String To Upper String ---*/

/*----- String To Lower String -----*/
std::string JMD::JMD_Utils_Writer::StringToLowerString( std::string string_param )
{
	transform(string_param.begin(),string_param.end(),string_param.begin(), ::tolower);
	return string_param;
}
/*--- End String To Lower String ---*/
	
/*------------- End Conversion Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
