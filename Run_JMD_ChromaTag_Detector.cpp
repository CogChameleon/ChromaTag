/*
 //--------------------------------------------------------------------//
 
 --------------------------
 Joseph DeGol
 UIUC Fall 2017
 --------------------------
 Run_JMD_ChromaTag_Detector: Version 1.0
 --------------------------
 
 ----------------------------------------------------------------
 Run_JMD_ChromaTag_Detector.cpp
 This file represents the program for using the ChromaTag library
 for detecting ChromaTags in images
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
#include <dirent.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Utilities
#include "JMD_Utils_Timer.hpp"
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Options.hpp"

//ChromaTag
#include "JMD_ChromaTag.hpp"

//opencv
#ifdef OPENCV_FOUND
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

//lcm
#ifdef LCM_FOUND
#include <lcm/lcm-cpp.hpp>
#include "JMD/LCM_Image_Type.hpp"
#include "JMD/LCM_ChromaTag_Detection_Type.hpp"
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
string ProgramName = "Run_JMD_ChromaTag_Detector";

//Description
string Description =
"This program uses the ChromaTag libraries for detecting a Chroma Tag\n\
fidicual marker and estimating the camera's pose based on the detection.\n\
For more information, see the list of program options in the help menu\n\
by typing --help or -h.";

//Objects
JMD_Utils_Timer myTimer;
JMD_Utils_Writer myWriter;
JMD_Utils_Options myOptions(ProgramName,"1.0","Joseph DeGol","University of Illinois",Description,&myWriter);
JMD_ChromaTag myChromaTag(&myWriter);
JMD_ChromaTag_Settings mySettings;

//LCM
#ifdef LCM_FOUND 
lcm::LCM myLCM;
#endif

/*----- Options -----*/

//Setters
int Verbose_Level     = 1;
int Log_Time_FileNo   = -1;
int Log_Detect_FileNo = -1;
std::string Config_File     = "";
std::string LCM_Out_Channel = "";
std::string LCM_In_Channel  = "";
std::string Input_Path      = "";
std::string Output_Path     = "";
std::string Log_Time_File   = "";
std::string Log_Detect_File = "";

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
//---------------------------- Prototypes ----------------------------//
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
//-------------------------- End Prototypes --------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//----------------------------- Helpers ------------------------------//
//--------------------------------------------------------------------//

/*----- Log Detections -----*/
std::string Log_Detections(JMD_ChromaTag_Collection *detections,std::string seed_string_param = "")
{
	//Tag Variables
	int DetectionSize = 0;
	unsigned long long curr_code = 0;
	JMD_ChromaTag_Detection *curr_detection;
	TagBorderSet *curr_set;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	std::string log_string = seed_string_param;
	
	//for each detection
	for(JMD_ChromaTag_Collection::iterator cit = detections->begin(); cit != detections->end(); ++cit)
	{
		//current detection
		curr_detection = *cit;
		
		//check if active, if not, we are at the end of found detections
		if( !(curr_detection->IsActive) ) { break; }
		
		//add current detection ID
		log_string += myWriter.ToString(DetectionSize);
		log_string += " ";
		
		/*--- populate corners for detections ---*/
		
		//current border set
		curr_set = &(curr_detection->TagBorders);
		
		//for each ring
		for(TagBorderSet::iterator sit = curr_set->begin(); sit != curr_set->end(); ++sit)
		{
			//current ring
			curr_ring = *sit;
			
			//for each segment
			for(TagBorderRing::iterator rit = curr_ring->begin(); rit != curr_ring->end(); ++rit)
			{
				//current segment
				curr_seg = *rit;
				
				//save first point of line to ChromaTag message
				log_string += myWriter.ToString( curr_seg->myLine.Point1()->U() ); log_string += " ";
				log_string += myWriter.ToString( curr_seg->myLine.Point1()->V() ); log_string += " ";
			}
		}
		/*- end populate corners for detections -*/
		
		
		/*--- populate codes for detections ---*/
		
		//has code?
		log_string += myWriter.ToString( curr_detection->IsCode );
		log_string += " ";
		
		//current code
		log_string += myWriter.ToString( curr_detection->TagCode );
		log_string += " ";
		
		/*- end populate codes for detections -*/
		
		
		/*--- populate pose for detections ---*/
		
		//has pose?
		log_string += myWriter.ToString( curr_detection->IsPose );
		log_string += " ";
		
		//translation
		log_string += myWriter.ToString( curr_detection->TagTranslation[0] ); log_string += " ";
		log_string += myWriter.ToString( curr_detection->TagTranslation[1] ); log_string += " ";
		log_string += myWriter.ToString( curr_detection->TagTranslation[2] ); log_string += " ";
		
		//rotation
		log_string += myWriter.ToString( curr_detection->TagRotation[0] ); log_string += " ";
		log_string += myWriter.ToString( curr_detection->TagRotation[1] ); log_string += " ";
		log_string += myWriter.ToString( curr_detection->TagRotation[2] ); log_string += " ";
		log_string += myWriter.ToString( curr_detection->TagRotation[3] ); log_string += " ";
		
		/*- end populate pose for detections -*/
		
		//increment detection size
		DetectionSize++;
	}
	
	//return
	return log_string;
}
/*--- End Log Detections ---*/

/*----- Run_LCM -----*/
#ifdef LCM_FOUND

void Publish_LCM(JMD_ChromaTag_Collection *detections)
{
	//Tag Variables
	int DetectionSize = 0;
	unsigned long long curr_code = 0;
	JMD_ChromaTag_Detection *curr_detection;
	TagBorderSet *curr_set;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	//create LCM ChromaTag Detection Object
	JMD::LCM_ChromaTag_Detection_Type myLCMChromaTag;
	
	//set timestamp to 0 for now
	myLCMChromaTag.Timestamp = 0;
	
	//for each detection
	for(JMD_ChromaTag_Collection::iterator cit = detections->begin(); cit != detections->end(); ++cit)
	{
		//current detection
		curr_detection = *cit;
		
		//check if active, if not, we are at the end of found detections
		if( !(curr_detection->IsActive) ) { break; }
		
		
		/*--- populate corners for detections ---*/
		
		//current border set
		curr_set = &(curr_detection->TagBorders);
		
		//push back new vectors for DetectionCorners
		std::vector<double> tmpu, tmpv;
		myLCMChromaTag.DetectionCornersU.push_back( tmpu );
		myLCMChromaTag.DetectionCornersV.push_back( tmpv );
		
		//for each ring
		for(TagBorderSet::iterator sit = curr_set->begin(); sit != curr_set->end(); ++sit)
		{
			//current ring
			curr_ring = *sit;
			
			//for each segment
			for(TagBorderRing::iterator rit = curr_ring->begin(); rit != curr_ring->end(); ++rit)
			{
				//current segment
				curr_seg = *rit;
				
				//save first point of line to ChromaTag message
				myLCMChromaTag.DetectionCornersU.at(DetectionSize).push_back( curr_seg->myLine.Point1()->U() );
				myLCMChromaTag.DetectionCornersV.at(DetectionSize).push_back( curr_seg->myLine.Point1()->V() );
			}
		}
		
		//ensure that vectors are correct size (if less than 3 rings, extra 0's are padded)
		myLCMChromaTag.DetectionCornersU.at(DetectionSize).resize(12,0);
		myLCMChromaTag.DetectionCornersV.at(DetectionSize).resize(12,0);
		myLCMChromaTag.DetectionRings.push_back( curr_set->size() );
		
		/*- end populate corners for detections -*/
		
		
		/*--- populate codes for detections ---*/
		
		//push back new vector for Code
		std::vector<signed char> tmpc(36,0);
		myLCMChromaTag.Code.push_back( tmpc );
		
		//has code?
		myLCMChromaTag.HasCode.push_back( curr_detection->IsCode );
		
		//current code
		curr_code = curr_detection->TagCode;
		
		//number of code bits
		myLCMChromaTag.CodeSize.push_back( curr_detection->TagBits );
		
		//convert to binary code
		std::bitset<36> curr_code_binary( curr_detection->TagCode );
		for(int i = 0; i < curr_code_binary.size(); i++)
		{
			myLCMChromaTag.Code.at(DetectionSize).at(i) = curr_code_binary[i];
		}
		
		/*- end populate codes for detections -*/
		
		
		/*--- populate pose for detections ---*/
		
		//push back new vectors for Pose
		std::vector<double> tmpt(3,0), tmpr(4,0);
		myLCMChromaTag.PoseTranslation.push_back( tmpt );
		myLCMChromaTag.PoseRotation.push_back( tmpr );
		
		//has pose?
		myLCMChromaTag.HasPose.push_back( curr_detection->IsPose );
		
		//translation
		myLCMChromaTag.PoseTranslation.at(DetectionSize).at(0) = curr_detection->TagTranslation[0];
		myLCMChromaTag.PoseTranslation.at(DetectionSize).at(1) = curr_detection->TagTranslation[1];
		myLCMChromaTag.PoseTranslation.at(DetectionSize).at(2) = curr_detection->TagTranslation[2];
		
		//rotation
		myLCMChromaTag.PoseRotation.at(DetectionSize).at(0) = curr_detection->TagRotation[0];
		myLCMChromaTag.PoseRotation.at(DetectionSize).at(1) = curr_detection->TagRotation[1];
		myLCMChromaTag.PoseRotation.at(DetectionSize).at(2) = curr_detection->TagRotation[2];
		myLCMChromaTag.PoseRotation.at(DetectionSize).at(3) = curr_detection->TagRotation[3];
		
		/*- end populate pose for detections -*/
		
		
		//increment detection size
		DetectionSize++;
	}
	
	//detection size
	myLCMChromaTag.DetectionSize = DetectionSize;
	
	//publish
	myLCM.publish(LCM_Out_Channel, &myLCMChromaTag);
}

class Handler 
{
    public:
        ~Handler() {}
        void handleMessage(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const JMD::LCM_Image_Type *myLCMImage)
        {
			if(Verbose_Level > 1) 
			{
				myWriter.Writer_Note("JMD_ChromaTag","handleMessage",myWriter.Int64ToString(myLCMImage->Timestamp)+": Message Received on "+chan,true);
			}
			
			//myImage.DataPtr((unsigned char*)&myLCMImage->Data[0]);

			//copy buffer to OpenCV Mat
			//myTimer.Reset();
			cv::Mat image( myLCMImage->Height, myLCMImage->Width, CV_MAKETYPE(0,myLCMImage->Channels) );
			std::copy( myLCMImage->Data.begin(), myLCMImage->Data.end(), image.data );
			//cout << "Copy Buffer to OpenCV Mat: " << myWriter.Int64ToString(myTimer.ElapsedTimeMilliSeconds()) << endl;
			
			//Detect
			//myTimer.Reset();
			myChromaTag.Detect_BGRToLAB(image);
			//cout << "Red Scan: " << myWriter.Int64ToString(myTimer.ElapsedTimeMilliSeconds()) << endl;
			
			//Decode
			myChromaTag.Decode(image);
			
			//Pose
			myChromaTag.Pose();
			
			//Get detections
			JMD_ChromaTag_Collection *myDetections = myChromaTag.Detections();
			
			//display
			namedWindow( ProgramName, cv::WINDOW_AUTOSIZE );
			imshow( ProgramName, image );
			cv::waitKey(1);
			
			//publish
			if( !(LCM_Out_Channel.empty()) )
			{
				Publish_LCM(myDetections);
				// Compute pose.
				//cv::Mat rvec;
				//cv::solvePnP(vTagCornersInTagFrame,vTagCornersInImageFrame,K,distCoef,rvec,tag.T);
				//tag.R = cv::Mat(3,3,CV_32F);
				//cv::Rodrigues(rvec,tag.R);
				// Convert mats to CV_32F (default for ORBSLAM).
				//tag.R.convertTo(tag.R, CV_32F);
				//tag.T.convertTo(tag.T, CV_32F);
			}
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


/*---------- Show/Save ----------*/

/*-------- End Show/Save --------*/


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
			//Detect
			myWriter.Writer_Note(ProgramName,"Process_Images","Frame " + myWriter.ToString(framect) + " : " + *it,true);
			
			double t0 = (double)cv::getTickCount();
			myChromaTag.Detect_BGRToLAB(image);
			t0 = ((double)cv::getTickCount() - t0)/cv::getTickFrequency();
			
			//Decode
			double t1 = (double)cv::getTickCount();
			myChromaTag.Decode(image);
			t1 = ((double)cv::getTickCount() - t1)/cv::getTickFrequency();
			
			//Pose
			double t2 = (double)cv::getTickCount();
			myChromaTag.Pose();
			t2 = ((double)cv::getTickCount() - t2)/cv::getTickFrequency();
			
			//Get detections
			JMD_ChromaTag_Collection *myDetections = myChromaTag.Detections();
			
			//display
			myWriter.Writer_Continued("    Detection Time = " + myWriter.ToString(t0),true);
			myWriter.Writer_Continued("    Decoding Time  = " + myWriter.ToString(t1),true);
			myWriter.Writer_Continued("    Pose Time      = " + myWriter.ToString(t2),true);
			myWriter.Writer_Continued("    FPS for Detect+Decode = " + myWriter.ToString(1/(t0+t1)),true);
			myWriter.Writer_Continued("",true);
			if(mySettings.myDebugLevel > 0)
			{
				namedWindow( ProgramName, cv::WINDOW_AUTOSIZE );
				//cv::resizeWindow( ProgramName, 1008, 756 );
				imshow( ProgramName, image );
				if(mySettings.myDebugLevel > 1)
				{
					cv::waitKey(0);
				}
				else
				{
					cv::waitKey(1);
				}
			}
			
			//write images
			if(!Output_Path.empty())
			{
				char *tmpc = (char *)malloc(strlen(it->c_str()) + 1);
				strcpy(tmpc, it->c_str());
				std::string outfile = Output_Path + std::string("/ChromaTag_") + basename(tmpc);
				cv::imwrite(outfile,image);
				free(tmpc);
			}
			
			//log
			if(Log_Time_FileNo >= 0)
			{
				//log timing
				myWriter.Writer_Line(Log_Time_FileNo,myWriter.ToString(framect) + " " + *it + " " + myWriter.ToString(1.0/(t0+t1)) + " " + myWriter.ToString(t0) + " " + myWriter.ToString(t1) + " " + myWriter.ToString(t2) );
			}
			if(Log_Detect_FileNo >= 0)
			{
				//log detections
				std::string seed_str = myWriter.ToString(framect) + " " + *it + " ";
				std::string log_dstr = Log_Detections(myDetections,seed_str);
				myWriter.Writer_Line(Log_Detect_FileNo,log_dstr);
			}
			
			//publish
			if( !(LCM_Out_Channel.empty()) )
			{
				Publish_LCM(myDetections);
			}

			//frame count
			framect++;
		}
	}
}
#else
void Process_Images(std::vector<std::string> &ImageList) { myWriter.Writer_Warning(ProgramName,"Process_Images","OpenCV Libraries not found.",true); }
#endif
/*--- End Process Images ---*/


/*----- Process Folder -----*/
void Process_Folder(std::string folder_param)
{
	//open directory
    DIR *dir;
    struct dirent *ent;
	dir = opendir(folder_param.c_str());

	//vector to store image paths
	std::vector<std::string> imageList;

	//directory found
    if (dir != NULL) 
	{
		//iterate over all files in directory
        while ((ent = readdir (dir)) != NULL) 
		{
			//get file name
            std::string imgName = ent->d_name;
			std::string imgPath = folder_param + '/' + imgName;

			//push back
			imageList.push_back( imgPath );
        }

		//close directory
        closedir(dir);
    } 
	else { myWriter.Writer_Warning(ProgramName,"Process_Folder","Could not open: "+folder_param,false); }

	//sort image list by timestamp
	std::sort(imageList.begin(), imageList.end());

	//Process Image List
	Process_Images( imageList );
}
/*--- End Process Folder ---*/


/*----- Signal Handler -----*/
static void Signal_Handler(int signum) 
{	
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
	
	//Input Colorspace
	description_vector.clear();
	description_vector.push_back("Sets the colorspace of input images. Default is 0.");
	description_vector.push_back("[0] RGB : Red Green Blue.");
	description_vector.push_back("[1] BGR : Blue Green Red.");
	description_vector.push_back("[2] YUV : Y - intensity, UV - color");
	myOptions.Add_Option_Setter("--input_cs","-i",description_vector, (int *)&(mySettings.myInputColorSpace),true);
	
	//Detection Colorspace
	description_vector.clear();
	description_vector.push_back("Sets the colorspace for detection. Default is 1.");
	description_vector.push_back("[0] YUV : Y - intensity, UV - color.");
	description_vector.push_back("[1] LAB : L - intensity, AB - color.");
	myOptions.Add_Option_Setter("--detection_cs","-d",description_vector, (int *)&(mySettings.myDetectionColorSpace),true);
	
	//config
	myOptions.Add_Option_Setter("--config","-c","Opens provided config file for setting options and parameters.",&Config_File,true);
	
	//file input
	myOptions.Add_Option_Setter("--input_folder","-F","Path to folder of images to process. Takes precedence over LCM input option.",&Input_Path,true);
	
	//lcm
	myOptions.Add_Option_Setter("--subscribe_lcm","-b","Subscribes to image data on LCM channel.",&LCM_In_Channel,false);
	myOptions.Add_Option_Setter("--publish_lcm","-p","Publishes pose data on LCM channel.",&LCM_Out_Channel,true);
	
	//file output
	myOptions.Add_Option_Setter("--output_folder","-O","Path to folder for output images with detections drawn on.",&Output_Path,false);
	myOptions.Add_Option_Setter("--log_time","-t","Log Time: provide log file path and name to log",&Log_Time_File,false);
	myOptions.Add_Option_Setter("--log_detect","-e","Log Detections: provide log file path and name to log",&Log_Detect_File,true);

	//verbose
	myOptions.Add_Option_Setter("--verbose","-v","Verbose Level: Silent(0) to Max(3). Default is 1.",&Verbose_Level,true);
	
	//debug
	myOptions.Add_Option_Setter("--debug","-x","Debug Level: See JMD_ChromaTag_Common.hpp for details. Default is 0.",(int *)&(mySettings.myDebugLevel),true);
	
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
	myWriter.Writer_IntroPrompt(ProgramName,"Joseph DeGol","University of Illinois","1.0","Help: ./Run_JMD_ChromaTag_Detector --help","Program to use Chroma Tags.");
	
	//parse options
	myOptions.Parse(argc,argv,true,&Extra_Argv);
	
	//signal handler
	Setup_SignalHandler();
	
	//logging
	if( !Log_Time_File.empty()   ) { Log_Time_FileNo   = myWriter.Writer_Open( Log_Time_File , std::fstream::out | std::fstream::trunc );   }
	if( !Log_Detect_File.empty() ) { Log_Detect_FileNo = myWriter.Writer_Open( Log_Detect_File , std::fstream::out | std::fstream::trunc ); }
	
	//----------------------------------------------------------------//
	//-------------------------- End Setup ---------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//----------------------------- Run ------------------------------//
	//----------------------------------------------------------------//
	
	/*----- Init -----*/
	
	//set settings
	myWriter.Writer_Note(ProgramName,"Main","Initializing ChromaTag",true);
	if(myChromaTag.Init(&mySettings, Config_File))
	{
		myWriter.Writer_Error(ProgramName,"Main","Failed to Initialize ChromaTag",false);
		myWriter.Writer_OutroPrompt(ProgramName);
		return 0;
	}
	
	//report
	if(Verbose_Level > 0) { myChromaTag.Display_Configuration(); }
	
	/*--- End Init ---*/
	
	/*----- Select Input -----*/
	
	//input folder
	if( !(Input_Path.empty()) ) { Process_Folder(Input_Path); }
	
	//LCM
	else if( !(LCM_In_Channel.empty()) ) { Run_LCM(); }
	
	//command line images
	else if( !Extra_Argv.empty() ) { Process_Images(Extra_Argv); }
	
	//no valid input
	else 
	{ 
		myWriter.Writer_Warning(ProgramName,"Main","No valid input method provided:",false);
		myWriter.Writer_Continued("        0. No images provided on command line.",true);
		myWriter.Writer_Continued("        1. Folder of images to process (-F) not given.",true);
		myWriter.Writer_Continued("        2. Subscribe with LCM (-b) not given or libraries missing.",true);
	}
	
	/*--- End Select Input ---*/
	
	//----------------------------------------------------------------//
	//--------------------------- End Run ----------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//-------------------------- Shutdown ----------------------------//
	//----------------------------------------------------------------//
	
	//close files
	if( Log_Time_FileNo >= 0 )   { myWriter.Writer_Close( Log_Time_FileNo );   }
	if( Log_Detect_FileNo >= 0 ) { myWriter.Writer_Close( Log_Detect_FileNo ); }

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
