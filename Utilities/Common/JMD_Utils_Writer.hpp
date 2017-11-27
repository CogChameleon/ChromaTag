#pragma once
#ifndef JMD_UTILS_WRITER_H_
#define JMD_UTILS_WRITER_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Writer: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Writer.hpp
 A class for writing project specific output to the terminal
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 Color : None=0, Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
 --- End Enumerations ---
 
 
 ----- Constructors -----
 
 Writer();
 
 --- end Constructors ---
 
 
 ----- Writing Methods -----
 
 int  Writer_Open(string file_name,ios_base::openmode mode);
 void Writer_Close(int file_num_param);
 void Writer_Line(int file_num_param, string text_param);
 void Writer_Line(int file_num_param, vector<string> text_param, string delimeter); 
 void Writer_Continued(string continued_param, bool newline_param);
 void Writer_Note(string class_param, string method_param, string message_param, bool newline_param);
 void Writer_Warning(string class_param, string method_param, string message_param, bool exit_param);
 void Writer_Error(string class_param, string method_param, string message_param, bool exit_param);
 void Writer_List(string class_param, string method_param, string list_param, vector<string> list_items_param);
 void Writer_ProgramHelp(string program_param, string author_param, string organization_param, string version_param, string description_param, vector<string> options_flag_param, vector<string> options_setter_param);
 void Writer_IntroPrompt(string program_param, string author_param, string organization_param, string version_param, string options_param, string note_param);
 void Writer_OutroPrompt(string program_param);
 void Writer_FunctionPrompt(string function_param);
 void Writer_Progress(int current_iteration_param, int total_iteration_param);
 
 --- end Writing Methods ---
 
 
 ----- Property Methods -----
 
 void SetInOutPromptColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetFPromptColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetNoteColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetWarningColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetErrorColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetProgressColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetProgramHeaderColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void SetProgramTextColor(Color text_color_param, Color highlight_color_param, bool bold_param, bool underline_param);
 void ResetColor();
 
 --- End Property Methods ---
  
  
 ----- Conversion Methods -----
 
 //ToString
 template <typename T> string ToString(T T_param);
 template <typename T> string ToString(T T_param, int precision_param);
 string BoolToString(bool bool_param);
 string IntToString(int int_param);
 string Int64ToString(int64_t int64_param);
 string FloatToString(float float_param, int precision_param);
 string DoubleToString(double double_param, int precision_param);
 
 //FromString
 template <typename T> T FromString( const string &string_param, T default_value_param = T() );
 bool   StringToBool( const string &string_param );
 int    StringToInt( const string &string_param );
 float  StringToFloat( const string &string_param );
 double StringToDouble( const string &string_param );
  
 //EditString
 string StringToUpperString( string string_param );
 string StringToLowerString( string string_param );
 
 --- End Conversion Methods ---
 
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

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <cstdlib>
#include <algorithm>

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//----------------------- Namespaces ----------------------------//
//---------------------------------------------------------------//

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







namespace JMD {
    
    //====================================================================//
    //====================================================================//
    //=================== JMD Utils Writer Definition ====================//
    //====================================================================//
    //====================================================================//
    class JMD_Utils_Writer
    {
        
        /*----- Enumerations -----*/
        
        //color
        public: typedef enum Color { NONE = 0, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE } Color;
        
        /*--- End Enumerations ---*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
        //file descriptor
        int FileNumber;
        std::vector <std::fstream*> FilesVector;
        std::vector <int>           FilesMap;
        
        //color properties
        std::vector <int> AccentStyles;			//Used with special characters in output structure, e.g. '-' and ':'
        std::vector <int> NoteStyles;			//Used with Writer Note
        std::vector <int> WarningStyles;		//Used with Writer Warning
        std::vector <int> ErrorStyles;			//Used with Write Error
        std::vector <int> ListStyles;			//Used with Write List for list items only
        std::vector <int> InOutPromptStyles;	//Used with Writer Intro and Outro Prompts
        std::vector <int> FunctionPromptStyles;	//Used with Writer Function Prompts
        std::vector <int> ProgressStyles;		//Used with Writer Progress Bar
        std::vector <int> ProgramHeaderStyles;	//Used with Writer Program Prompts for titles and headers
        std::vector <int> ProgramTextStyles;	//Used with Writer Program Prompts for body text
        std::vector <int> QueryStyles;			//Used with Writer Program for query text
        std::vector <int> InputStyles;			//Used with Writer Program for user input
        
        /*------------- End Variables -------------*/
        
        /*--------------- Helpers ---------------*/
        
        /*----- Set Color -----*/
        std::string Set_Color(Color text_color_param = NONE, Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Color ---*/
        
        /*------------- End Helpers -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
		/*--------------- Constructors ---------------*/
		JMD_Utils_Writer();
		/*------------- end Constructors -------------*/
		
		/*--------------- Destructors ---------------*/
		~JMD_Utils_Writer();
		/*------------- end Destructors ------------*/
		
		/*--------------- Writing Methods ---------------*/
		
		/*----- Writer Open -----*/
		// Opens a file with the given name for writing with the given
		// mode
		//
		// input:
		//    file_name_param - name of file to open/create
		//    mode_param - ios file mode, same values that fstream uses
		// output:
		//    int - file # for writing, -1 if failure
		int  Writer_Open(std::string file_name_param, std::ios_base::openmode mode_param = std::fstream::out | std::fstream::app);
		/*--- End Writer Open ---*/
		
		/*----- Writer Close -----*/
		// Closes the file that was open at a given number
		//
		// input:
		//    file_num_param - file # given when opening file
		// output:
		//    int - file # to close, -1 if bad #
        int  Writer_Close(int file_num_param);
        /*--- End Writer Close ---*/
        
        /*----- Writer Line -----*/
        // write a line of text to file
        //
        // input:
        //    file_num_param - file # given when opening file
        //    text_param - string or vector of string written to line
        //    delimeter_param - if vector<string> separates each element
        void Writer_Line(int file_num_param, std::string text_param);
        void Writer_Line(int file_num_param, std::vector<std::string> text_param, std::string delimeter_param = " "); 
        /*--- End Writer Line ---*/
 
		/*----- Writer Continued -----*/
		// Writes text to the console in the style of Writer_Note. Is meant to
		// be used with another writer method when a newline wasn't used.
		//
		// input:
		//    continued_param - string that will be displayed
		//    newline_param - True = newline, False = no newline
		//    backspace_param - number of chars to backspace on line
		void Writer_Continued(std::string continued_param, bool newline_param = false, unsigned int backspace_param = 0);
		/*--- End Writer Continued ---*/
		
		/*----- Writer Note -----*/
		// Writes a note to terminal in format of Class ::: Method :: Note: Message
		//
		// input:
		//    class_param - highest level where note comes from, i.e. a class
		//    method_param - similarly, the next level, i.e. a method
		//    message_param - message for user
		//    newline_param - True = newline, False = no newline
		//    overwrite_param - True = returns cursor to start of line
		void Writer_Note(std::string class_param, std::string method_param, std::string message_param, bool newline_param = false, bool overwrite_param = false);
		/*--- End Writer Note ---*/
		
		/*----- Writer Warning -----*/
		// Writes a warning to terminal in format of Class ::: Method :: Warning: Message
		//
		// input:
		//    class_param - highest level where warning comes from, i.e. a class
		//    method_param - similarly, the next level, i.e. a method
		//    message_param - message for user
		//    exit_param - True = exit, False = no exit
 		void Writer_Warning(std::string class_param, std::string method_param, std::string message_param, bool exit_param = false);
 		/*--- End Writer Warning ---*/
 		
 		/*----- Writer Error -----*/
 		// Writes an error to terminal in format of Class ::: Method :: Error: Message
		//
		// input:
		//    class_param - highest level where error comes from, i.e. a class
		//    method_param - similarly, the next level, i.e. a method
		//    message_param - message for user
		//    exit_param - True = exit, False = no exit
 		void Writer_Error(std::string class_param, std::string method_param, std::string message_param, bool exit_param = false);
 		/*--- End Writer Error ---*/
 		
 		/*----- Writer List -----*/
 		// Writes a list line by line in the format Class ::: Method :: List: item1\nitem2...
 		//
 		// input:
 		//    class_param - highest level where message comes from
 		//    method_param - similarly, the next level, i.e. a method
 		//    list_param - list text
 		//    list_items_param - vector of strings to make up the list
 		void Writer_List(std::string class_param, std::string method_param, std::string list_param, std::vector<std::string> list_items_param);
 		/*--- End Writer List ---*/
 		
 		/*----- Writer Program Help -----*/
 		// Writes the help output when a -h option is provided for a program. This includes
 		// a description of the program, a summary of options, example usage, and author info
 		//
 		// input:
 		//    program_param - name of program
 		//    author_param - name of author
 		//    organization_param - name of organization
 		//    version_param - version numbers as string
 		//    description_param - description of the program, with '\n's for paragraph formatting
 		//    options_flag_param - vector of strings, each string represents a flag option
 		//    options_setter_param - vector of string, each string represets a setter option
 		void Writer_ProgramHelp(std::string program_param, std::string author_param, std::string organization_param, std::string version_param, std::string description_param, std::vector<std::string> options_flag_param, std::vector<std::string> options_setter_param);
 		/*--- End Writer Program Help ---*/
 		
 		/*----- Writer Intro Prompt -----*/
 		// Writes an introduction for a program with fields for program name, author,
 		// organization, version, options, and notes. If size constrains are exceeded, the
 		// strings are truncated to the max size. See size constraints below.
 		//
 		// input:
 		//    program_param - name of program,                     size(program) < 27
 		//    author_name - name of author,                        size(author) < 21
 		//    organization_param - name of organization,           size(organization) < 36
 		//    version_param - version numbers as a string,         size(version) < 10
 		//    option_param - list of program command line options, size(option) < 46
 		//    note_param - any additional notes,                   size(note) < 46
 		void Writer_IntroPrompt(std::string program_param, std::string author_param, std::string organization_param, std::string version_param, std::string options_param, std::string note_param);
 		/*--- End Writer Intro Prompt ---*/
 		
 		/*----- Writer Outro Prompt -----*/
 		// Writes an outroduction for a program with field for program name
 		//
 		// input:
 		//    program_param - name of program, max length for string is 36
 		void Writer_OutroPrompt(std::string program_param);
 		/*--- End Writer Outro Prompt ---*/
 		
 		/*----- Writer Function Prompt -----*/
 		// Writes a stylized message for indicating progress, function calls, etc...
 		//
 		// input:
 		//    function_param - function/method/etc name
 		void Writer_FunctionPrompt(std::string function_param);
 		/*--- End Writer Function Prompt ---*/
 		
 		/*----- Writer Progress -----*/
 		// Writes a progress bar and updates with continued calls
 		//
 		// input:
 		//    current_iteration_param - integer for current iteration, start at 0
 		//    total_iteration_param - integer for total number of iterations
 		void Writer_Progress(int current_iteration_param, int total_iteration_param);
 		/*--- End Writer Progress ---*/
 		
 		/*----- Writer Query -----*/
 		// Writes a query that expects a user input response
 		//
 		// input:
 		//    query_param - string that is output as query
 		// output:
 		//    T - type populated by cin value
		template <typename T> T Writer_Query(std::string query_param, T *T_param)
		{
			//set query style
			std::cout << Set_Color( (Color)QueryStyles[0], (Color)QueryStyles[1], (bool)QueryStyles[2], (bool)QueryStyles[3]);
			
			//query
			std::cout << "    " 
				 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << "<<<"
				 << Set_Color( (Color)QueryStyles[0], (Color)QueryStyles[1], (bool)QueryStyles[2], (bool)QueryStyles[3]) << " Query "
				 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << ">>> " 
				 << Set_Color( (Color)QueryStyles[0], (Color)QueryStyles[1], (bool)QueryStyles[2], (bool)QueryStyles[3]) << query_param
				 << Set_Color( (Color)AccentStyles[0], (Color)AccentStyles[1], (bool)AccentStyles[2], (bool)AccentStyles[3]) << " : ";
			
			//set user input style
			std::cout << Set_Color( (Color)InputStyles[0], (Color)InputStyles[1], (bool)InputStyles[2], (bool)InputStyles[3]);
			
			//user input
			std::cin >> (*T_param);
			std::cin.get();
			
			//reset color
			std::cout << Set_Color() << std::endl;
			
			//return
			return (*T_param);
		}
 		/*--- End Writer Query ---*/
 		
		/*------------- End Writing Methods -------------*/
        
        
        /*--------------- Property Methods ---------------*/
        
        /*----- Set Accent Color -----*/
        // sets the color of accents in Writer output, e.g. '-' and ':'
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetAccentColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Accent Color ---*/
        
        /*----- Set Intro Outro Prompt Color -----*/
        // sets the intro and outro prompt color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetInOutPromptColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*----- Set Intro Outro Prompt Color -----*/
        
        /*----- Set Function Prompt Color -----*/
        // sets the function prompt color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
 		void SetFPromptColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
 		/*----- Set Function Prompt Color -----*/
 
        /*----- Set Note Color -----*/
        // sets the note color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetNoteColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Note Color ---*/
        
        /*----- Set Warning Color -----*/
        // sets the warning color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
 		void SetWarningColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
 		/*--- End Set Warning Color ---*/
 		
 		/*----- Set Error Color -----*/
 		// sets the error color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
 		void SetErrorColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
 		/*--- End Set Error Color ---*/
 		
 		/*----- Set List Color -----*/
 		// sets the list items color for text and highlight
 		//
 		// input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
 		void SetListColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
 		/*--- End Set List Color ---*/
 		
 		/*----- Set Progress Color -----*/
 		// sets the progress color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
 		void SetProgressColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
 		/*--- End Set Progress Color ---*/
        
        /*----- Set Program Header Color -----*/
        // sets the program header and titles color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetProgramHeaderColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Program Header Color ---*/
        
        /*----- Set Program Text Color -----*/
        // sets the program text color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetProgramTextColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Program Text Color ---*/
        
        /*----- Set Query Color -----*/
        // sets the query text color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetQueryColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Query Color ---*/
        
        /*----- Set Input Color -----*/
        // sets the user input text color for text and highlight
        //
        // input:
        //    text_color_param - enumeration value for color of text
        //    highlight_color_param - enumeration value for color of text highlight
        //    bold_param - boolean for bold text
        //    underline_param - boolean for underline text
        void SetInputColor(JMD::JMD_Utils_Writer::Color text_color_param, JMD::JMD_Utils_Writer::Color highlight_color_param = NONE, bool bold_param = false, bool underline_param = false);
        /*--- End Set Input Color ---*/
        
        /*----- Reset Color -----*/
        // resets the colors to the terminals defaults
        //
        void ResetColor();
        /*--- End Reset Color ---*/
        
        /*------------- End Property Methods -------------*/
        
        
        /*--------------- Conversion Methods ---------------*/
        
        /*----- To String -----*/
        // converts any templated type to a string
        //
        // input:
        //    T_param - type parameter for conversion
        //    precision_param - sets precision for floats and doubles
        //
        // output:
        //    string - string equivalent of T input
	    template <typename T> std::string ToString(T T_param)
	    {
			std::stringstream ss;
			ss << T_param;
			return ss.str();
		}
		template <typename T> std::string ToString(T T_param, int precision_param)
		{
			std::stringstream ss;
			ss << std::setprecision(precision_param) << T_param;
			return ss.str();
		}
		/*--- End To String -----*/
	
		/*----- Bool To String -----*/
		// converts a boolean value to the string "true" or "false"
		//
		// input:
		//    bool_param - boolean value for conversion
		//
		// output:
		//    string - "true" or "false"
		std::string BoolToString(bool bool_param);
		/*--- End Bool To String ---*/
	
		/*----- Int To String -----*/
		// converts an integer to a string, e.g 105 -> "105"
		//
		// input:
		//    int_param - integer for conversion
		//
		// output:
		//    string - string equivalent of integer input
		std::string IntToString(int int_param);
		std::string Int64ToString(int64_t int64_param);
		/*--- End Int To String ---*/
		
		/*----- Float To String -----*/
		// converts a float to a string, e.g. 3.14f -> "3.14"
		//
		// input:
		//    float_param - floating point number for conversion
		//    precision_param - precision for number, default = 10
		//
		// output:
		//    string - string equivalent of float input
		std::string FloatToString(float float_param, int precision_param = 10);
		/*--- End Float To String ---*/
		
		/*----- Double To String -----*/
		// converts a double to a string, e.g. 4.56 -> "4.56"
		//
		// input:
		//    double_param - double number for conversion
		//    precision_param - precision for number, default = 10
		//
		// output:
		//    string - string equivalent of double input
		std::string DoubleToString(double double_param, int precision_param = 10);
		/*--- End Double To String ---*/
	
		/*----- From String -----*/
		template <typename T> T FromString( const std::string &string_param, T default_value_param = T() )
		{
			std::stringstream ss(string_param);
			T result;
			return ss >> result ? result : default_value_param;
		}
		/*--- End From String -----*/
		
		/*----- String To Bool -----*/
		// converts a string = "true" to a bool true, false otherwise
		//
		// input:
		//    string_param - string parameter to convert to a bool
		//
		// output:
		//    bool - boolean output true or false
		bool StringToBool( const std::string &string_param );
		/*--- End String to Bool ---*/
		
		/*----- String To Int -----*/
		// converts a string to an integer if possible, 0 otherwise
		//
		// input:
		//    string_param - string parameter to convert to an integer
		//
		// output:
		//    int - integer output from string, 0 if bad string
		int StringToInt( const std::string &string_param );
		/*--- End String To Int ---*/
		
		/*----- String To Float -----*/
		// converts a string to a float if possible, 0.0f otherwise
		//
		// input:
		//    string_param - string parameter to convert to a float
		//
		// output:
		//    float - float output from string, 0.0f if bad string
		float StringToFloat( const std::string &string_param );
		/*--- End String To Float ---*/
		
		/*----- String To Double -----*/
		// converts a string to a double if possible, 0.0 otherwise
		//
		// input:
		//    string_param - string parameter to convert to a double
		//
		// output:
		//    double - double output from string, 0.0 if bad string
		double StringToDouble( const std::string &string_param );
		/*--- End String To Double ---*/
		
		/*----- String To Upper String -----*/
		// converts a string to all uppercase
		//
		// input:
		//    string_param - string for converting to all uppercase
		//
		// output:
		//    string - string in all uppercase letters
		std::string StringToUpperString( std::string string_param );
		/*--- End String To Upper String ---*/
		
		/*----- String To Lower String -----*/
		// converts a string to all lowercase
		//
		// input:
		//    string_param - string for converting to all lowercase
		//
		// output:
		//    string - string in all lowercase letters
		std::string StringToLowerString( std::string string_param );
		/*--- End String To Lower String ---*/
	
        /*------------- End Conversion Methods -------------*/
        
        //---------------------------------------------------------------//
        //------------------------ end Public ---------------------------//
        //---------------------------------------------------------------//
        
        
    };
    //====================================================================//
    //====================================================================//
    //====================================================================//
    //====================================================================//
    //====================================================================//
    
    
}//end namespace JMD


#endif

