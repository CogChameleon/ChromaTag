#pragma once
#ifndef JMD_UTILS_OPTIONS_H_
#define JMD_UTILS_OPTIONS_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Options: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Options.hpp
 A class for creating, setting, and running program options
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 --- End Enumerations ---
 
 
 ----- Constructors -----
 
 JMD_Utils_Options(string program_param = "", string version_param = "", string author_param = "", string organization_param = "", string description_param = "", JMD_Utils_Writer *writer_param = NULL);
 
 --- end Constructors ---
 
 
 ----- Option Methods -----
 
 void Add_Option_Flag(string command_long_param, string command_short_param, string command_description_param, bool * flag_param)
		
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, bool * bool_param)
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, int * int_param)
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, char * char_param)
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, double * double_param)
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, float * float_param)
 void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, string * string_param)

 void Add_Option_Callback(string command_long_param, string command_short_param, string command_description_param, void (*callback_function_param)())
		
 --- end Option Methods ---
 
 
 ----- Parse Methods -----
 
 void Parse_Help(int argc, char **argv)
 void Parse(int argc, char **argv)
 
 --- End Parse Methods ---
  
 
 ----- Utility Methods -----
 
 void Display_Help()
 
 --- End Utility Methods ---
        
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

//utilities
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Object.hpp"

//system
#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <cstdlib>

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//----------------------- Namespaces ----------------------------//
//---------------------------------------------------------------//

using namespace std;

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
    class JMD_Utils_Options
    {
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Ops Class ---------------*/
        class Ops
        {
			
			/*----- private -----*/
			private:
				
			//private setters
			void Set_Bool  (char* argv);
			void Set_Int   (char* argv);
			void Set_Char  (char* argv);
			void Set_Double(char* argv);
			void Set_Float (char* argv);
			void Set_String(char* argv); 
			/*--- end private ---*/
			
			/*----- public -----*/
        	public:
        	
        	//enum
			typedef enum Ops_Type { OPS_FLAG = 0, OPS_BOOL, OPS_INT, OPS_CHAR, OPS_DOUBLE, OPS_FLOAT, OPS_STRING, OPS_CALLBACK, OPS_SETTER_CALLBACK } Ops_Type;
			
			//Ops profile
			string long_option;						// long option
			string short_option;					// short option
			string description;						// description
			JMD_Utils_Object value;					// holds value
			Ops_Type type;							// type of value
			bool spacer;                            // space after option
			
			//constructors
			Ops(string command_long_param, string command_short_param, string command_description_param, JMD_Utils_Object value_param, Ops_Type type_param, bool spacer_param = false);
			
			//Execute
			void Execute(char *argv = NULL);  	
			
			/*--- end public ---*/
			
        };
        /*------------- End Ops Class -------------*/
        
        /*--------------- Variables ---------------*/
        
        //vectors of options
		vector<Ops> Options;
		vector<Ops> Callbacks;
		
		//writer
		JMD_Utils_Writer *myWriter;
		
		//information
		string Program;
		string Version;
		string Author;
		string Organization;
		string Description;

        /*------------- End Variables -------------*/
        
        /*--------------- Helpers ---------------*/
        
        /*----- Add Option -----*/
        // Adds a flag that sets a boolean variable to true if option is provided
		//
		// input:
		//    command_long_param - long option text, e.g. "--help"
		//    command_short_param - short option text, e.g. "-h"
		//    command_description_param - description of the option
		//    (set_ or cb_)obj_param - object containing value for option
		//    type_param - type indicator for the object type
		//    spacer_param - add space after option in help dialog
        void Add_Option(string command_long_param, string command_short_param, string command_description_param, JMD_Utils_Object obj_param, Ops::Ops_Type type_param, bool spacer_param = false);
        /*--- End Add Option ---*/
        
        /*----- Add Option Callback Ghost -----*/
        // Adds a callback that will fire with another option but doesn't
        // act as its own seperate option
        //
        // input:
        //    command_long_param - long option text, e.g. "--help"
        //    command_short_param - short option text, e.g. "-h"
        //    callback_param - callback function that is called
        void Add_Option_Callback_Ghost(string command_long_param, string command_short_param,void (*callback_param)());
        /*--- End Add Option Callback Ghost ---*/
        
        /*----- VectorToString -----*/
        // Converts a vector to string where each string is on a new line
        // that has been shifted by (LONG_OP_WIDTH+SHORT_OP_WIDTH+MULTI_LINE_WIDTH
        //
        // input:
        //    vector_param - vector of strings
        // output:
        //    string - formatted string from vector<string>
        std::string VectorToString(std::vector<std::string> &vector_param);
        /*--- End VectorToString ---*/
        
        /*------------- End Helpers -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
        /*--------------- Variables ---------------*/
        const static int LONG_OP_WIDTH = 16;
        const static int SHORT_OP_WIDTH = 6;
        const static int MULTI_LINE_WIDTH = 6;
        /*------------- End Variables -------------*/
        
		/*--------------- Constructors ---------------*/
		// construct options object, provides auto generated help menu
		// in the style set by the JMD_Utils_Writer provided as pointer
		//
		// input:
		//    program_param - parent program name
		//    author_param - author of parent program
		//    organization_param - organization for which author works
		//    description_param - description of parent program
		//    writer_param - pointer to JMD_Utils_Writer of parent
		JMD_Utils_Options(string program_param = "", string version_param = "", string author_param = "", string organization_param = "", string description_param = "", JMD_Utils_Writer *writer_param = NULL);
		/*------------- end Constructors -------------*/
		
		
		/*--------------- Destructors ---------------*/
		~JMD_Utils_Options();
		/*------------- end Destructors ------------*/
		
		
		/*--------------- Option Methods ---------------*/
		
		/*----- Add Option Flag -----*/
		// Adds a flag that sets a boolean variable to true if option is provided
		//
		// input:
		//    command_long_param - long option text, e.g. "--help"
		//    command_short_param - short option text, e.g. "-h"
		//    command_description_param - string or vector<string> description of option, one string per line
		//    flag_param - sets this variable to true if flag exists
		//    spacer_param - add spacer after option in help dialog
		void Add_Option_Flag(string command_long_param, string command_short_param, string command_description_param, bool *flag_param, bool spacer_param = false);
		void Add_Option_Flag(string command_long_param, string command_short_param, vector<string> &command_description_param, bool *flag_param, bool spacer_param);
		/*--- End Add Option Flag ---*/
		
		/*----- Add Option Setter -----*/
		// Adds a setter option to the Options Object. A setter option will set a variable
		// value when the option flag is provided and parsed
		//
		// input:
		//    command_long_param - long option text, e.g. "--help"
		//    command_short_param - short option text, e.g. "-h"
		//    command_description_param - string or vector<string> description of option, one string per line
		//    type_param - the variable(s) to set (int,bool,char, etc...)
		//    spacer_param - add spacer after option in help dialog
		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, bool *bool_param,     bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, int *int_param,       bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, char *char_param,     bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, double *double_param, bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, float *float_param,   bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, string *string_param, bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, bool *bool_param,     bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, int *int_param,       bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, char *char_param,     bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, double *double_param, bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, float *float_param,   bool spacer_param = false);
 		void Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, string *string_param, bool spacer_param = false);
		/*--- End Add Option Setter ---*/
		
		/*----- Add Option Callback -----*/
		// Adds a callback function to the Options Object. A callback function will call
		// the provided function
		//
		// input:
		//    command_long_param - long option text, e.g. "--help"
		//    command_short_param - short option text, e.g. "-h"
		//    command_description_param - string or vector<string> description of the option, one string per line
		//    callback_function_param - function(s) to call
		//    spacer_param - add spacer after option in help dialog
		void Add_Option_Callback(string command_long_param, string command_short_param, string command_description_param, void (*callback_param)(), bool spacer_param = false);
		void Add_Option_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, void (*callback_param)(), bool spacer_param = false);
		/*--- End Add Option Callback ---*/
		
		/*----- Add Option Setter Callback -----*/
		// Adds a setter callback option to the Options Object. A setter
		// callback option will set a variable value and call a function
		// when the option flag is provided and parsed
		//
		// input:
		//    command_long_param - long option text, e.g. "--help"
		//    command_short_param - short option text, e.g. "-h"
		//    command_description_param - string or vector<string> description of option, one string per line
		//    type_param - the variable(s) to set (int,bool,char, etc...)
		//    callback_function_param - function(s) to call
		//    spacer_param - add spacer after option in help dialog
		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, bool *bool_param,     void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, int *int_param,       void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, char *char_param,     void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, double *double_param, void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, float *float_param,   void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, string *string_param, void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, bool *bool_param,     void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, int *int_param,       void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, char *char_param,     void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, double *double_param, void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, float *float_param,   void (*callback_param)(), bool spacer_param = false);
 		void Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, string *string_param, void (*callback_param)(), bool spacer_param = false);
		/*--- End Add Option Setter Callback ---*/
		
		/*------------- End Option Methods -------------*/
        
        
        /*--------------- Parse Methods ---------------*/
        
        /*----- Parse Help -----*/
        // Parses the provided char **, looking just for help (--help or -h)
        //
        // input:
        //    argc - number of incoming arguments, argc parameter from main
        //    argv - character array of options, argv parameter from main
		// output:
		//    bool - true if good to continue, false should result in program exit
        bool Parse_Help(int argc, char **argv);
        /*--- End Parse ---*/
        
        /*----- Parse -----*/
        // Parses the provided char **, treated as the command line arguments
        //
        // input:
        //    argc - number of incoming arguments, argc parameter from main
        //    argv - character array of options, argv parameter from main
		//    warn_param - if true, warns about unknown arguments
		//    extra_argv_param - vector with extra arguments, warn ignored
		// output:
		//    bool - true if good to continue, false should result in program exit
        bool Parse(int argc, char **argv, bool warn_param = true, std::vector<std::string> *extra_argv_param = nullptr);
        /*--- End Parse ---*/
        
        /*------------- End Parse Methods -------------*/
        
        
        /*--------------- Utility ---------------*/
        
        /*----- Display Help -----*/
        // displays the help dialog for the added options; a.k.a, the menu
        //
        void Display_Help();
        /*--- End Display Help ---*/
        
        /*------------- End Utility -------------*/
        
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

