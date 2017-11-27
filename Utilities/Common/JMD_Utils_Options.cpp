/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Options: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Options.cpp
 A class for creating, setting, and running program options
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

#include "JMD_Utils_Options.hpp"

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

/*--------------- Ops Class Implementation ---------------*/

/*----- Constructors -----*/
JMD::JMD_Utils_Options::Ops::Ops(string command_long_param, string command_short_param, string command_description_param, JMD_Utils_Object value_param, Ops_Type type_param, bool spacer_param):
long_option(command_long_param), short_option(command_short_param), description(command_description_param), value(value_param), type(type_param), spacer(spacer_param)
{ }
/*--- End Constructors ---*/

/*----- Ops Class Methods -----*/
void JMD::JMD_Utils_Options::Ops::Execute(char *argv)
{
	//setters
	if(argv)
	{
		switch(type)
		{
			case OPS_BOOL:   Set_Bool(argv);   break;
			case OPS_INT:    Set_Int(argv);    break;
			case OPS_CHAR:   Set_Char(argv);   break;
			case OPS_DOUBLE: Set_Double(argv); break;
			case OPS_FLOAT:  Set_Float(argv);  break;
			case OPS_STRING: Set_String(argv); break;
		}
	}
	
	//flags
	else
	{
		//set point to value to true
		bool *tmp_ptr = Get_Value<bool *>(value);
		*tmp_ptr = true;
	}
}

void JMD::JMD_Utils_Options::Ops::Set_Bool(char* argv)
{
	//variables
	JMD_Utils_Writer tmpWriter;
	string s(argv);
	bool *tmp_ptr = Get_Value<bool *>(value);
	
	//true
	if(tmpWriter.StringToLowerString(s) == "true" || s == "1")
	{
		*tmp_ptr = true;
	}
	
	//false
	else
	{
		*tmp_ptr = false;
	}
}
void JMD::JMD_Utils_Options::Ops::Set_Int(char* argv)
{	
	//variables
	JMD_Utils_Writer tmpWriter;
	string s(argv);
	
	//get pointer to value
	int *tmp_ptr = Get_Value<int *>(value);
	
	//convert and set value
	*tmp_ptr = tmpWriter.StringToInt(s);
}

void JMD::JMD_Utils_Options::Ops::Set_Char(char* argv)
{
	//get char value ptr
	char *tmp_ptr = Get_Value<char *>(value);
	
	//set value
	*tmp_ptr = *argv;
}

void JMD::JMD_Utils_Options::Ops::Set_Double(char* argv)
{
	//variables
	JMD_Utils_Writer tmpWriter;
	string s(argv);
	
	//get pointer to value
	double *tmp_ptr = Get_Value<double *>(value);
	
	//convert and set value
	*tmp_ptr = tmpWriter.StringToDouble(s);
}

void JMD::JMD_Utils_Options::Ops::Set_Float(char* argv)
{
	//variables
	JMD_Utils_Writer tmpWriter;
	string s(argv);
	
	//get pointer to value
	float *tmp_ptr = Get_Value<float *>(value);
	
	//convert and set value
	*tmp_ptr = tmpWriter.StringToFloat(s);
}

void JMD::JMD_Utils_Options::Ops::Set_String(char* argv)
{
	//variables
	string s(argv);
	
	//get point to value
	string *tmp_ptr = Get_Value<string *>(value);
	
	//set value
	*tmp_ptr = s;
}
/*--- End Ops Class Methods ---*/

/*------------ End Ops Class Implementation -------------*/

/*--------------- Add Option ---------------*/
void JMD::JMD_Utils_Options::Add_Option(string command_long_param, string command_short_param, string command_description_param, JMD_Utils_Object obj_param, Ops::Ops_Type type_param, bool spacer_param)
{
	//create option
	Ops o(command_long_param, command_short_param, command_description_param, obj_param, type_param, spacer_param);
	
	//add option
	Options.push_back(o);
}
/*------------- End Add Option -------------*/

/*--------------- Add Option Callback Ghost ---------------*/
void JMD::JMD_Utils_Options::Add_Option_Callback_Ghost(string command_long_param, string command_short_param,void (*callback_param)())
{
	//create any-object and add option
	JMD_Utils_Object tmp = callback_param;
	Add_Option(command_long_param,command_short_param,"",tmp,Ops::OPS_SETTER_CALLBACK,false);
}
/*------------- End Add Option Callback Ghost -------------*/

/*--------------- Vector To String ---------------*/
std::string JMD::JMD_Utils_Options::VectorToString(std::vector<std::string> &vector_param)
{
	//variables
	std::string tmp = vector_param[0];
	stringstream ss;
	
	//iterate through vector
	for(vector<string>::iterator it = vector_param.begin()+1; it != vector_param.end(); ++it)
	{
		ss << std::endl << std::setw(LONG_OP_WIDTH+SHORT_OP_WIDTH+MULTI_LINE_WIDTH) << std::left << " " << *it;
	}
	
	//return
	tmp += ss.str();
	return tmp;
}
/*------------- End Vector To String -------------*/

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


/*--------------- Constructors ---------------*/
JMD::JMD_Utils_Options::JMD_Utils_Options(string program_param, string version_param, string author_param, string organization_param, string description_param, JMD_Utils_Writer *writer_param) :
	Program(program_param),
	Version(version_param),
	Author(author_param),
	Organization(organization_param),
	Description(description_param),
	myWriter(writer_param)
{ }
/*------------- end Constructors -------------*/


/*--------------- Destructors ---------------*/
JMD::JMD_Utils_Options::~JMD_Utils_Options() { }
/*------------- end Destructors -------------*/


/*--------------- Option Methods ---------------*/


/*----- Add Option Flag -----*/
void JMD::JMD_Utils_Options::Add_Option_Flag(string command_long_param, string command_short_param, string command_description_param, bool *flag_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = flag_param;
	Add_Option(command_long_param, command_short_param, command_description_param, tmp, Ops::OPS_FLAG, spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Flag(string command_long_param, string command_short_param, vector<string> &command_description_param, bool *flag_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = flag_param;
	
	//Squeeze Descripton
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param, command_short_param, squeezed_description, tmp, Ops::OPS_FLAG, spacer_param);
}
/*--- End Add Option Flag ---*/
		
		
/*----- Add Option Setter -----*/

/*--- bool ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, bool * bool_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = bool_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_BOOL,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, bool * bool_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = bool_param;
	
	//Squeeze Descripton
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_BOOL,spacer_param);
}
/*- end bool -*/

/*--- int ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, int * int_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = int_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_INT,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, int * int_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = int_param;
	
	//Squeeze Descripton
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_INT,spacer_param);
}
/*- end int -*/

/*--- char ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, char * char_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = char_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_CHAR,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, char * char_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = char_param;
	
	//Squeeze Description
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_CHAR,spacer_param);
}
/*- end char -*/

/*--- double ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, double * double_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = double_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_DOUBLE,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, double * double_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = double_param;
	
	//Squeeze Description
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_DOUBLE,spacer_param);
}
/*- end double -*/

/*--- float ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, float * float_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = float_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_FLOAT,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, float * float_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = float_param;
	
	//Squeeze Description
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_FLOAT,spacer_param);
}
/*- end float -*/

/*--- string ---*/
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, string command_description_param, string * string_param, bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = string_param;
	Add_Option(command_long_param,command_short_param,command_description_param,tmp,Ops::OPS_STRING,spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter(string command_long_param, string command_short_param, vector<string> &command_description_param, string * string_param, bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = string_param;
	
	//Squeeze Description
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param,command_short_param,squeezed_description,tmp,Ops::OPS_STRING,spacer_param);
}
/*- end string -*/

/*--- End Add Option Setter ---*/


/*----- Add Option Callback -----*/
void JMD::JMD_Utils_Options::Add_Option_Callback(string command_long_param, string command_short_param, string command_description_param, void (*callback_param)(), bool spacer_param)
{
	//create any-object and add option
	JMD_Utils_Object tmp = callback_param;
	Add_Option(command_long_param, command_short_param, command_description_param,tmp,Ops::OPS_CALLBACK, spacer_param);
}
void JMD::JMD_Utils_Options::Add_Option_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, void (*callback_param)(), bool spacer_param)
{
	//create any-object
	JMD_Utils_Object tmp = callback_param;
	
	//Squeeze Description
	std::string squeezed_description = VectorToString(command_description_param);
	
	//Add Option
	Add_Option(command_long_param, command_short_param, squeezed_description,tmp,Ops::OPS_CALLBACK, spacer_param);
}
/*--- End Add Option Callback ---*/


/*----- Add Option Setter Callback -----*/
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, bool *bool_param,     void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, bool_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, int *int_param,       void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, int_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, char *char_param,     void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, char_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, double *double_param, void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, double_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, float *float_param,   void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, float_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, string command_description_param, string *string_param, void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, string_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);	
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, bool *bool_param,     void (*callback_param)(), bool spacer_param)
{
	Add_Option_Setter(command_long_param, command_short_param, command_description_param, bool_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);	
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, int *int_param,       void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, int_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);		
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, char *char_param,     void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, char_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);		
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, double *double_param, void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, double_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);		
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, float *float_param,   void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, float_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);		
}
void JMD::JMD_Utils_Options::Add_Option_Setter_Callback(string command_long_param, string command_short_param, vector<string> &command_description_param, string *string_param, void (*callback_param)(), bool spacer_param)
{
    Add_Option_Setter(command_long_param, command_short_param, command_description_param, string_param, spacer_param);
	Add_Option_Callback_Ghost(command_long_param, command_short_param, callback_param);		
}
/*--- End Add Option Setter Callback ---*/

/*------------- End Option Methods -------------*/


/*--------------- Parse Methods ---------------*/

/*----- Parse Help -----*/
bool JMD::JMD_Utils_Options::Parse_Help(int argc, char **argv)
{
	if(argc == 1) { return true; }
	
	for(int i=1; i < argc; i++)
	{
		string s = argv[i];
		if(s == "--help" || s == "-h")
		{
			Display_Help();
			
			//return exit signal
			return false;//exit(EXIT_SUCCESS);
		}
	}
	
	//return okay
	return true;
}
/*--- End Parse Help ---*/

/*----- Parse -----*/
bool JMD::JMD_Utils_Options::Parse(int argc, char **argv, bool warn_param, std::vector<std::string> *extra_argv_param)
{
	//if no params given
	if(argc == 1) { return true; }
	
	//if request for extra params, turn off warn
	if (extra_argv_param) { warn_param = false; }
	
	//for each option given
	bool found;
	for(int i = 1; i < argc; i++)
	{
		
		//get option
		string s = argv[i];
		found    = false;

		//if help flag, display help and quit
		if(s == "--help" || s == "-h"){
				Display_Help();
				
				//return exit signal
				return false;//exit(EXIT_SUCCESS);
		}
		
		//search through added options
		for(unsigned int j = 0; j < Options.size(); j++)
		{
			//if provided option matches an option string
			if(s == Options[j].long_option || s == Options[j].short_option)
			{
				//flag found
				found = true;
				
				//save callbacks for after flags and setters
				if( Options[j].type == Ops::OPS_CALLBACK )
				{
					Callbacks.push_back(Options[j]);
				}
				
				//flags
				else if( Options[j].type == Ops::OPS_FLAG )
				{
					Options[j].Execute();
				}
				
				//setters
				else
				{
					Options[j].Execute(argv[++i]);
					
					//look for setter_callbacks
					for(unsigned int k = 0; k < Options.size(); k++)
					{
						if( (s == Options[k].long_option || s == Options[k].short_option) && Options[k].type == Ops::OPS_SETTER_CALLBACK )
						{
							Callbacks.push_back(Options[k]);
						}
					}
					
				}
				
				//next option
				break;
			}
		}
		
		//if not found
		if(!found) 
		{ 
			//warn about bad param
			if(warn_param)
			{
			    string tmp = "Invalid Option: " + s + ".";
			    if(myWriter) { myWriter->Writer_Warning("JMD_Utils_Options","Parse",tmp,false); }
			    else
			    { 
				    JMD_Utils_Writer tmpWriter;
				    tmpWriter.Writer_Warning("JMD_Utils_Options","Parse",tmp,false);
			    }
			}
			
			//add to extra param
			if(extra_argv_param)
			{
				extra_argv_param->push_back(std::string( s ));
			}
		}
	}
	
	//call the callbacks
	for(unsigned int i=0; i < Callbacks.size(); i++)
	{
		void (*tmp_ptr)() = Get_Value<void (*)()>(Callbacks[i].value);
		(*tmp_ptr)();
	}
	
	//return okay
	return true;
}
/*--- End Parse ---*/

/*------------- End Parse Methods -------------*/


/*--------------- Utility ---------------*/

/*----- Display Help -----*/	
void JMD::JMD_Utils_Options::Display_Help()
{
	//variables
	vector<string> flag_text;
	vector<string> setter_text;
	string result = "--help           [-h]       (Displays this dialog and exits)";
	string spacer = "";
	
	//populate options vector
	flag_text.push_back(result);
	flag_text.push_back(spacer);
	for(unsigned int i = 0; i < Options.size(); i++)
	{
		//stringstream for formatting
		stringstream ss;
		ss << std::setw(LONG_OP_WIDTH) << std::left << Options[i].long_option << " " << std::setw(SHORT_OP_WIDTH) << std::left << Options[i].short_option;
		
		//add description and store in result
		result = ss.str() + "  " + Options[i].description;
		
		//push back to correct list
		if(Options[i].type == Ops::OPS_FLAG || Options[i].type == Ops::OPS_CALLBACK) //flags and callbacks
		{
			flag_text.push_back(result);
			if(Options[i].spacer) { flag_text.push_back(spacer); }
		}
		else if(Options[i].type == Ops::OPS_SETTER_CALLBACK) //catches the callback part of setter callback
		{
			//does nothing
		}
		else //setter
		{
			setter_text.push_back(result);
			if(Options[i].spacer) { setter_text.push_back(spacer); }
		}
		
	}
	
	//Writer Display Help
	if(myWriter) { myWriter->Writer_ProgramHelp(Program,Author,Organization,Version,Description,flag_text,setter_text); }
	else
	{
		JMD_Utils_Writer tmpWriter;
		tmpWriter.Writer_ProgramHelp(Program,Author,Organization,Version,Description,flag_text,setter_text);
	}
}
/*--- End Display Help ---*/

/*------------- End Utility -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
