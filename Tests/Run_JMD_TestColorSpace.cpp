/*
 //--------------------------------------------------------------------//
 
 --------------------------
 Joseph DeGol
 UIUC Fall 2016
 --------------------------
 Run_JMD_TestColorSpace: Version 1.0
 --------------------------
 
 ----------------------------------------------------------------
 Run_JMD_TestColorSpace.cpp
 This program allows a user to see how images look when converted
 to other colorspaces
 ----------------------------------------------------------------
 
 //--------------------------------------------------------------------//
*/





//====================================================================//
//====================================================================//
//============================ Preamble ==============================//
//====================================================================//
//====================================================================//

//--------------------------------------------------------------------//
//---------------------------- Includes ------------------------------//
//--------------------------------------------------------------------//

//system
#include <stdio.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <libgen.h>

//Utilities
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Options.hpp"

//opencv
#ifdef OPENCV_FOUND
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

//lcm
#ifdef LCM_FOUND
#include <lcm/lcm-cpp.hpp>
#include "JMD/LCM_Image_Type.hpp"
#endif

//--------------------------------------------------------------------//
//------------------------- End Includes -----------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//--------------------------- Namespaces -----------------------------//
//--------------------------------------------------------------------//

using namespace JMD;

//--------------------------------------------------------------------//
//------------------------- End Namespaces ---------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//---------------------------- Globals -------------------------------//
//--------------------------------------------------------------------//

//Program Name
string ProgramName = "Run_JMD_TestColorSpace";

//Description
string Description =
"This program displays images in differen color spaces. For more information,\n\
see the list of program options in the help menu by typing --help or -h.";

//Objects
JMD_Utils_Writer  myWriter;
JMD_Utils_Options myOptions(ProgramName,"1.0","Joseph DeGol","University of Illinois",Description,&myWriter);

//LCM
#ifdef LCM_FOUND 
lcm::LCM myLCM;
#endif

/*----- Options -----*/

//Setters
std::string LCM_In_Channel  = "";

/*--- End Options ---*/
	
//--------------------------------------------------------------------//
//---------------------------- Globals -------------------------------//
//--------------------------------------------------------------------//

//====================================================================//
//====================================================================//
//============================ Preamble ==============================//
//====================================================================//
//====================================================================//





//====================================================================//
//====================================================================//
//============================  Functions ============================//
//====================================================================//
//====================================================================//

//--------------------------------------------------------------------//
//----------------------------- Helpers ------------------------------//
//--------------------------------------------------------------------//

/*----- Run_LCM -----*/
#ifdef LCM_FOUND
class Handler 
{
    public:
        ~Handler() {}
        void handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const JMD::LCM_Image_Type *myLCMImage)
        {	
			//copy buffer to OpenCV Mat
			cv::Mat image( myLCMImage->Height, myLCMImage->Width, CV_MAKETYPE(0,myLCMImage->Channels) );
			std::copy( myLCMImage->Data.begin(), myLCMImage->Data.end(), image.data );
			
			//variables
			cv::Mat ImLAB;
			cv::Mat ImL;
			cv::Mat ImA;
			cv::Mat ImB;
			
			//convert
			cv::cvtColor(image,ImLAB,CV_BGR2Lab);
			
			//channels
			cv::extractChannel(ImLAB,ImL,0);
			cv::extractChannel(ImLAB,ImA,1);
			cv::extractChannel(ImLAB,ImB,2);
			
			//show
			cv::namedWindow( "DEBUG_ShowLAB - L", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - L", ImL );
			cv::namedWindow( "DEBUG_ShowLAB - A", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - A", ImA );
			cv::namedWindow( "DEBUG_ShowLAB - B", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - B", ImB );
			cv::waitKey(1);
        }
};

void Run_LCM()
{
	/*--- Setup ---*/
	
	//init LCM
	if(!myLCM.good()) { myWriter.Writer_Error(ProgramName,"LCM_Setup","Error initializing lcm.",true); }
	
	/*- End Setup -*/
	
	/*--- Run ---*/
	
	//create handler
	Handler myHandler;
	
	//subscribe to channel
    myLCM.subscribe(LCM_In_Channel, &Handler::handleMessage, &myHandler);
    
    //run
	myWriter.Writer_Note(ProgramName,"Run_LCM","Subscribed to " + LCM_In_Channel + ". Press ctrl-c to quit.",true);
    while(0 == myLCM.handle());
	
	/*- End Run -*/
}
#else
void Run_LCM() { myWriter.Writer_Warning(ProgramName,"Run_LCM","LCM Libraries not found.",true); }
void Publish_LCM() { myWriter.Writer_Warning(ProgramName,"Publish_LCM","LCM Libraries not found.",true); }
#endif
/*--- End Run_LCM ---*/

/*----- Process Images -----*/
#ifdef OPENCV_FOUND
void Process_Images(std::vector<std::string> &ImageList)
{
	//iterate over image list
	unsigned int framect = 0;
	for(std::vector<std::string>::const_iterator it = ImageList.begin(); it != ImageList.end(); ++it)
	{
		//read image
		cv::Mat image;
		image = cv::imread(*it);
	
		//read okay?
		if(!image.data ) { myWriter.Writer_Warning(ProgramName,"Process_Images","Could not open: "+(*it),false); }
		else
		{
			//variables
			cv::Mat ImLAB;
			cv::Mat ImL;
			cv::Mat ImA;
			cv::Mat ImB;
			
			//convert
			cv::cvtColor(image,ImLAB,CV_BGR2Lab);
			
			//channels
			cv::extractChannel(ImLAB,ImL,0);
			cv::extractChannel(ImLAB,ImA,1);
			cv::extractChannel(ImLAB,ImB,2);
			
			//show
			cv::namedWindow( "DEBUG_ShowLAB - L", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - L", ImL );
			cv::namedWindow( "DEBUG_ShowLAB - A", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - A", ImA );
			cv::namedWindow( "DEBUG_ShowLAB - B", cv::WINDOW_AUTOSIZE );
			cv::imshow( "DEBUG_ShowLAB - B", ImB );
			cv::waitKey(0);
		}
	}
}
#else
void Process_Images(std::vector<std::string> &ImageList) { myWriter.Writer_Warning(ProgramName,"Process_Images","OpenCV Libraries not found.",true); }
#endif
/*--- End Process Images ---*/

/*----- Signal Handler -----*/
static void Signal_Handler(int signum) 
{	
	//exit
	myWriter.Writer_OutroPrompt(ProgramName); 
	exit(0);
}
void Setup_SignalHandler()
{
	//signal handler
	struct sigaction new_action, old_action;
    new_action.sa_handler = Signal_Handler;
    sigemptyset (&new_action.sa_mask);
    sigaddset(&new_action.sa_mask, SIGTERM);
    new_action.sa_flags = 0;
    sigaction (SIGINT, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) { sigaction (SIGINT, &new_action, NULL); }
    sigaction (SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN) { sigaction (SIGHUP, &new_action, NULL); }
    sigaction (SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN) { sigaction (SIGTERM, &new_action, NULL); }
}
/*--- End Signal Handler ---*/

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//---------------------------- Callbacks -----------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//

//====================================================================//
//====================================================================//
//============================  Functions ============================//
//====================================================================//
//====================================================================//





//====================================================================//
//====================================================================//
//============================== Main ================================//
//====================================================================//
//====================================================================//

int main( int argc, char **argv )
{   

	//----------------------------------------------------------------//
	//--------------------------- Options ----------------------------//
	//----------------------------------------------------------------//
	
	//description vector
	vector<string> description_vector;
	
	/*----- Set Colors -----*/
	myWriter.SetProgramHeaderColor(JMD::JMD_Utils_Writer::WHITE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetProgramTextColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetAccentColor(JMD::JMD_Utils_Writer::WHITE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetInOutPromptColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetFPromptColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetNoteColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetWarningColor(JMD::JMD_Utils_Writer::YELLOW,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetErrorColor(JMD::JMD_Utils_Writer::RED,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetListColor(JMD::JMD_Utils_Writer::WHITE);
	myWriter.SetQueryColor(JMD::JMD_Utils_Writer::GREEN,JMD::JMD_Utils_Writer::NONE,true);
	/*--- End Set Colors ---*/
	
	
	/*----- Add Options - Flags and Callbacks - Alphabetical -----*/
	
	/*--- End Add Options - Flags and Callbacks - Alphabetical ---*/
	
	
	/*----- Add Options - Setters and Setter_Callbacks -----*/
	
	//lcm
	myOptions.Add_Option_Setter("--subscribe_lcm","-b","Subscribes to image data on LCM channel.",&LCM_In_Channel,true);
	
	/*--- End Add Options - Setters and Settter_Callbacks ---*/
	
	//parse help - return if false
	if( !myOptions.Parse_Help(argc,argv) ) { return 0; }
	
	//----------------------------------------------------------------//
	//------------------------- End Options --------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//---------------------------- Setup -----------------------------//
	//----------------------------------------------------------------//
	
	//extra argv vector
	std::vector<std::string> Extra_Argv;
		
	//intro prompt
	myWriter.Writer_IntroPrompt(ProgramName,"Joseph DeGol","University of Illinois","1.0","Help: ./Run_JMD_TestColorSpace --help","Program to view images in various colorspaces.");
	
	//parse options
	myOptions.Parse(argc,argv,true,&Extra_Argv);
	
	//signal handler
	Setup_SignalHandler();
		
	//----------------------------------------------------------------//
	//-------------------------- End Setup ---------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//----------------------------- Run ------------------------------//
	//----------------------------------------------------------------//
	
	//LCM
	if( !(LCM_In_Channel.empty()) ) { Run_LCM(); }
	
	//from file
	else if( !Extra_Argv.empty() ) { Process_Images(Extra_Argv); }
	
	//no valid input
	else 
	{ 
		myWriter.Writer_Warning(ProgramName,"Main","No valid input method provided:",false);
		myWriter.Writer_Continued("        0. No images provided on command line.",true);
		myWriter.Writer_Continued("        1. Subscribe with LCM (-b) not given or libraries missing.",true);
	}
	
	//----------------------------------------------------------------//
	//--------------------------- End Run ----------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//-------------------------- Shutdown ----------------------------//
	//----------------------------------------------------------------//
	
	//exit
	myWriter.Writer_OutroPrompt(ProgramName);
	
	//return
	return 0;
	
	//----------------------------------------------------------------//
	//------------------------- End Shutdown -------------------------//
	//----------------------------------------------------------------//
	
}

//====================================================================//
//====================================================================//
//============================== Main ================================//
//====================================================================//
//====================================================================//
