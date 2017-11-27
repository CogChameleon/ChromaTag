/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2017
 ==========================
 JMD ChromaTag: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag.cpp
 A class for ChromaTag - a colored fiducial marker detection system
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

#include "JMD_ChromaTag.hpp"

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





//====================================================================//
//====================================================================//
//=================== JMD ChromaTag Implementation ===================//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//
        
/*--------------- General Helpers ---------------*/

/*----- Private Read Config File -----*/
bool JMD::JMD_ChromaTag::Private_ReadConfigFile(cv::FileStorage &fs_param, std::string key_param)
{
	//if the file was opened successfully
	if(fs_param.isOpened())
	{
		//get the node for the key
		cv::FileNode fn = fs_param[key_param];
		
		//if the key is empty, return false
		if(fn.empty()) { return false; }
		//else, key exists, return true
		else { 			 return true; }	
	}
	//else, return failure
	else
	{
		return false;
	}
}
/*--- End Private Read Config File ---*/

/*----- Private Init -----*/
int JMD::JMD_ChromaTag::Private_Init(JMD::JMD_ChromaTag_Settings *settings_param, std::string config_param)
{
	//Variables
	cv::FileStorage fs;
	cv::FileNode fn;
	if(!config_param.empty())
	{
		try 
		{
			fs.open(config_param, cv::FileStorage::READ); 
		}
		catch (...) {}
	}
	
	/*--- Init Parameters ---*/
	// if provided on command-line, they will be in the settings_param struct, otherwise, try to read from
	// config file, and if they aren't there, init with default value
	
	//verbose level
	if(settings_param && settings_param->myVerboseLevel != JMD::DEF_DUMMY_INT) { Verbose_Level(settings_param->myVerboseLevel); }
	else if( Private_ReadConfigFile(fs,std::string("VerboseLevel")) ) { Verbose_Level((int)fs["VerboseLevel"]); }
	else { Verbose_Level(DEFAULT_VERBOSE_LEVEL); }
	
	//debug level
	if(settings_param && settings_param->myDebugLevel != JMD::DEF_DUMMY_INT) { Debug_Level(settings_param->myDebugLevel); }
	else if( Private_ReadConfigFile(fs,std::string("DebugLevel")) ) { Debug_Level((int)fs["DebugLevel"]); }
	else { Debug_Level(DEFAULT_DEBUG_LEVEL); }
	
	//subsample
	if(settings_param && settings_param->mySubsample != JMD::DEF_DUMMY_UINT) { Subsample(settings_param->mySubsample); }
	else if( Private_ReadConfigFile(fs,std::string("Subsample")) ) { Subsample((int)fs["Subsample"]); }
	else { Subsample(DEFAULT_SUBSAMPLE); }
	
	//border
	if(settings_param && settings_param->myBorder != JMD::DEF_DUMMY_UINT) { Border(settings_param->myBorder); }
	else if( Private_ReadConfigFile(fs,std::string("Border")) ) { Border((int)fs["Border"]); }
	else { Border(DEFAULT_BORDER); }
	
	//hit threshold
	if(settings_param && settings_param->myHitThreshold != JMD::DEF_DUMMY_UINT) { HitThreshold(settings_param->myHitThreshold); }
	else if( Private_ReadConfigFile(fs,std::string("HitThreshold")) ) { HitThreshold((int)fs["HitThreshold"]); }
	else { HitThreshold(DEFAULT_HITTHRESHOLD); }
	
	//initial scan num
	if(settings_param && settings_param->myInitialScanNum != JMD::DEF_DUMMY_UINT) { InitialScanNum(settings_param->myInitialScanNum); }
	else if( Private_ReadConfigFile(fs,std::string("InitialScanNum")) ) { InitialScanNum((int)fs["InitialScanNum"]); }
	else { InitialScanNum(DEFAULT_INITIALSCANNUM); }
	
	//max initial border iterations
	if(settings_param && settings_param->myMaxInitialBorderIterations != JMD::DEF_DUMMY_UINT) { MaxInitialBorderIterations(settings_param->myMaxInitialBorderIterations); }
	else if( Private_ReadConfigFile(fs,std::string("MaxInitialBorderIterations")) ) { MaxInitialBorderIterations((int)fs["MaxInitialBorderIterations"]); }
	else { MaxInitialBorderIterations(DEFAULT_MAXINITIALBORDERITERATIONS); }
	
	//max detections
	if(settings_param && settings_param->myMaxDetections != JMD::DEF_DUMMY_UINT) { MaxDetections(settings_param->myMaxDetections); }
	else if( Private_ReadConfigFile(fs,std::string("MaxDetections")) ) { MaxDetections((int)fs["MaxDetections"]); }
	else { MaxDetections(DEFAULT_MAX_DETECTIONS); }
	
	//area ratio threshold
	if(settings_param && settings_param->myAreaRatioThreshold != JMD::DEF_DUMMY_DBL) { AreaRatioThreshold(settings_param->myAreaRatioThreshold); }
	else if( Private_ReadConfigFile(fs,std::string("AreaRatioThreshold")) ) { AreaRatioThreshold((double)fs["AreaRatioThreshold"]); }
	else { AreaRatioThreshold(DEFAULT_AREARATIOTHRESHOLD); }
	
	//tag family
	if(settings_param && settings_param->myTagFamily != JMD::DEF_DUMMY_INT) { Tag_Family(settings_param->myTagFamily); }
	else if( Private_ReadConfigFile(fs,std::string("TagFamily")) ) { Tag_Family((JMD::ChromaTag_Family)(int)fs["TagFamily"]); }
	else { Tag_Family(DEFAULT_TAG_FAMILY); }
	
	//input color space
	if(settings_param && settings_param->myInputColorSpace != JMD::DEF_DUMMY_INT) { Input_Colorspace(settings_param->myInputColorSpace); }
	else if( Private_ReadConfigFile(fs,std::string("InputColorSpace")) ) { Input_Colorspace((JMD::ChromaTag_InputColorSpace)(int)fs["InputColorSpace"]); }
	else { Input_Colorspace(DEFAULT_INPUT_COLORSPACE); }
	
	//detection color space
	if(settings_param && settings_param->myDetectionColorSpace != JMD::DEF_DUMMY_INT) { Detection_Colorspace(settings_param->myDetectionColorSpace); }
	else if( Private_ReadConfigFile(fs,std::string("DetectionColorSpace")) ) { Detection_Colorspace((JMD::ChromaTag_DetectionColorSpace)(int)fs["DetectionColorSpace"]); }
	else { Detection_Colorspace(DEFAULT_DETECTION_COLORSPACE); }
	
	//LAB red thresh
	if(settings_param && settings_param->myLABRedDiffThresh != JMD::DEF_DUMMY_DBL) { LABRedDiffThresh(settings_param->myLABRedDiffThresh); }
	else if( Private_ReadConfigFile(fs,std::string("LABRedDiffThresh")) ) { LABRedDiffThresh((double)fs["LABRedDiffThresh"]); }
	else { LABRedDiffThresh(DEFAULT_LABREDDIFFTHRESH); }
	
	//Red Green Border Thresh
	if(settings_param && settings_param->myRedGreenBorderThresh != JMD::DEF_DUMMY_DBL) { RedGreenBorderThresh(settings_param->myRedGreenBorderThresh); }
	else if( Private_ReadConfigFile(fs,std::string("RedGreenBorderThresh")) ) { RedGreenBorderThresh((double)fs["RedGreenBorderThresh"]); }
	else { RedGreenBorderThresh(DEFAULT_REDGREENBORDERTHRESH); }
	
	//Green Black Border Thresh
	if(settings_param && settings_param->myGreenBlackBorderThresh != JMD::DEF_DUMMY_DBL) { GreenBlackBorderThresh(settings_param->myGreenBlackBorderThresh); }
	else if( Private_ReadConfigFile(fs,std::string("GreenBlackBorderThresh")) ) { GreenBlackBorderThresh((double)fs["GreenBlackBorderThresh"]); }
	else { GreenBlackBorderThresh(DEFAULT_GREENBLACKBORDERTHRESH); }
	
	//Black White
	if(settings_param && settings_param->myBlackWhiteBorderThresh != JMD::DEF_DUMMY_DBL) { BlackWhiteBorderThresh(settings_param->myBlackWhiteBorderThresh); }
	else if( Private_ReadConfigFile(fs,std::string("BlackWhiteBorderThresh")) ) { BlackWhiteBorderThresh((double)fs["BlackWhiteBorderThresh"]); }
	else { BlackWhiteBorderThresh(DEFAULT_BLACKWHITEBORDERTHRESH); }
	
	//Max Expand Distance Multiplier
	if(settings_param && settings_param->myMaxExpandDistMultiplier != JMD::DEF_DUMMY_DBL) { MaxExpandDistMultiplier(settings_param->myMaxExpandDistMultiplier); }
	else if( Private_ReadConfigFile(fs,std::string("MaxExpandDistMultiplier")) ) { MaxExpandDistMultiplier((double)fs["MaxExpandDistMultiplier"]); }
	else { MaxExpandDistMultiplier(DEFAULT_MAXEXPANDDISTMULTIPLIER); }

	//Max Border Refine Iterations
	if(settings_param && settings_param->myMaxBorderRefineIterations != JMD::DEF_DUMMY_UINT) { MaxBorderRefineIterations(settings_param->myMaxBorderRefineIterations); }
	else if( Private_ReadConfigFile(fs,std::string("MaxBorderRefineIterations")) ) { MaxBorderRefineIterations((int)fs["MaxBorderRefineIterations"]); }
	else { MaxBorderRefineIterations(DEFAULT_MAXBORDERREFINEITERATIONS); }
	
	//Refine Corners Level
	if(settings_param && settings_param->myRefineCornersLevel != JMD::DEF_DUMMY_UINT) { RefineCorners_Level(settings_param->myRefineCornersLevel); }
	else if( Private_ReadConfigFile(fs,std::string("RefineCornersLevel")) ) { RefineCorners_Level((int)fs["RefineCornersLevel"]); }
	else { RefineCorners_Level(DEFAULT_REFINECORNERS_LEVEL); }
	
	//Use Rings For Homography
	if(settings_param && settings_param->myUseRingsForHomography != JMD::DEF_DUMMY_INT) { UseRingsForHomography(settings_param->myUseRingsForHomography); }
	else if( Private_ReadConfigFile(fs,std::string("UseRingsForHomography")) ) { UseRingsForHomography((JMD::ChromaTag_UseRings)(int)fs["UseRingsForHomography"]); }
	else { UseRingsForHomography(DEFAULT_USERINGSFORHOMOGRAPHY); }
	
	//Max Search Hamming Distance
	if(settings_param && settings_param->myMaxSearchHammingDistance != JMD::DEF_DUMMY_UINT) { MaxSearchHammingDistance(settings_param->myMaxSearchHammingDistance); }
	else if( Private_ReadConfigFile(fs,std::string("MaxSearchHammingDistance")) ) { MaxSearchHammingDistance((int)fs["MaxSearchHammingDistance"]); }
	else { MaxSearchHammingDistance(DEFAULT_MAXSEARCHHAMMINGDISTANCE); }
	
	//Use Rings For Pose
	if(settings_param && settings_param->myUseRingsForPose != JMD::DEF_DUMMY_INT) { UseRingsForPose(settings_param->myUseRingsForPose); }
	else if( Private_ReadConfigFile(fs,std::string("UseRingsForPose")) ) { UseRingsForPose((JMD::ChromaTag_UseRings)(int)fs["UseRingsForPose"]); }
	else { UseRingsForPose(DEFAULT_USERINGSFORPOSE); }
	
	//Tag Size
	if(settings_param && settings_param->myTagSize != JMD::DEF_DUMMY_DBL) { TagSize(settings_param->myTagSize); }
	else if( Private_ReadConfigFile(fs,std::string("TagSize")) ) { TagSize((double)fs["TagSize"]); }
	else { TagSize(DEFAULT_TAGSIZE); }
	
	//FX, FY
	if(settings_param && settings_param->myFX != JMD::DEF_DUMMY_DBL) { FX(settings_param->myFX); }
	else if( Private_ReadConfigFile(fs,std::string("FX")) ) { FX((double)fs["FX"]); }
	else { FX(DEFAULT_FX); }
	if(settings_param && settings_param->myFY != JMD::DEF_DUMMY_DBL) { FY(settings_param->myFY); }
	else if( Private_ReadConfigFile(fs,std::string("FY")) ) { FY((double)fs["FY"]); }
	else { FY(DEFAULT_FY); }
	
	//CX, CY
	if(settings_param && settings_param->myCX != JMD::DEF_DUMMY_DBL) { CX(settings_param->myCX); }
	else if( Private_ReadConfigFile(fs,std::string("CX")) ) { CX((double)fs["CX"]); }
	else { CX(DEFAULT_CX); }
	if(settings_param && settings_param->myCY != JMD::DEF_DUMMY_DBL) { CY(settings_param->myCY); }
	else if( Private_ReadConfigFile(fs,std::string("CY")) ) { CY((double)fs["CY"]); }
	else { CY(DEFAULT_CY); }
	
	//K1, K2, K3, P1, P2
	if(settings_param && settings_param->myK1 != JMD::DEF_DUMMY_DBL) { K1(settings_param->myK1); }
	else if( Private_ReadConfigFile(fs,std::string("K1")) ) { K1((double)fs["K1"]); }
	else { K1(DEFAULT_K1); }
	if(settings_param && settings_param->myK2 != JMD::DEF_DUMMY_DBL) { K2(settings_param->myK2); }
	else if( Private_ReadConfigFile(fs,std::string("K2")) ) { K2((double)fs["K2"]); }
	else { K2(DEFAULT_K2); }
	if(settings_param && settings_param->myK3 != JMD::DEF_DUMMY_DBL) { K3(settings_param->myK3); }
	else if( Private_ReadConfigFile(fs,std::string("K3")) ) { K3((double)fs["K3"]); }
	else { K3(DEFAULT_K3); }
	if(settings_param && settings_param->myP1 != JMD::DEF_DUMMY_DBL) { P1(settings_param->myP1); }
	else if( Private_ReadConfigFile(fs,std::string("P1")) ) { P1((double)fs["P1"]); }
	else { P1(DEFAULT_P1); }
	if(settings_param && settings_param->myP2 != JMD::DEF_DUMMY_DBL) { P2(settings_param->myP2); }
	else if( Private_ReadConfigFile(fs,std::string("P2")) ) { P2((double)fs["P2"]); }
	else { P2(DEFAULT_P2); }
	
	/*- End Init Parameters -*/
	
	
	//Allocate Memory for Tag Detections
	for(int i = 0; i < mySettings->myMaxDetections; i++)
	{
		//create new detection
		JMD_ChromaTag_Detection *tmp = new JMD_ChromaTag_Detection();
		myDetections->push_back(tmp);
	}

	//Create Hash Tables for Color Conversion
	myWriter->Writer_Note("JMD_ChromaTag","Init","Initializing ChromaTag, this takes about 30 seconds...",true);
	double L, A, B;
	JMD_Vision_Process color_converter;
	myLAB_LTable.resize(256*256*256);
	myLAB_ATable.resize(256*256*256);
	myLAB_BTable.resize(256*256*256);
	for(unsigned int r = 0; r < 256; r++)
	{
		for(unsigned int g = 0; g < 256; g++)
		{
			for(unsigned int b = 0; b < 256; b++)
			{
				//convert
				//color_converter.RGBToLAB_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b),  &L, &A, &B );
				L = color_converter.RGBToLAB_L_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );
				A = color_converter.RGBToLAB_A_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );
				B = color_converter.RGBToLAB_B_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );

				//store
				myLAB_LTable[ HASH_RGB(r,g,b) ] = L;
				myLAB_ATable[ HASH_RGB(r,g,b) ] = A;
				myLAB_BTable[ HASH_RGB(r,g,b) ] = B;
			}
		}
	}
	
	//close
	if(fs.isOpened()) { fs.release(); }
	
	//return
	return 0;
}
/*----- Private Init -----*/

/*------------- End General Helpers -------------*/


/*--------------- Set Helpers ---------------*/

/*----- Private Tag Family -----*/
void JMD::JMD_ChromaTag::Private_Tag_Family(ChromaTag_Family family_param)
{
	//variables
	std::string tmp;
	
	//case
	switch(family_param)
	{
		case RGBW_16H5: tmp = "RGBW_16H5"; break;
		case RGBW_25H9: tmp = "RGBW_25H9"; break;
		case RGBW_36H11: tmp = "RGBW_36H11"; break;
		case RGRBW_36H11: tmp = "RGRBW_36H11"; break;
				
		default:
			family_param = RGBW_16H5;
			tmp = "RGBW_16H5";
			if(mySettings->myVerboseLevel > 1) { myWriter->Writer_Warning("JMD_ChromaTag","Tag_Family","Bad Option: Setting to default = RGBW_16H5",false); }
	}
	
	//return
	mySettings->myTagFamily = family_param;
	myTagFamilyStr = tmp;
}
/*--- End Private Tag Family ---*/

/*----- Private Input Colorspace -----*/
void JMD::JMD_ChromaTag::Private_Input_Colorspace(ChromaTag_InputColorSpace colorspace_param)
{
	//variables
	std::string tmp;
	
	//case
	switch(colorspace_param)
	{
		case InputCS_RGB: tmp = "RGB"; break;
		case InputCS_BGR: tmp = "BGR"; break;
		case InputCS_YUV: tmp = "YUV"; break;
		
		default:
			colorspace_param = InputCS_RGB;
			tmp = "RGB";
			if(mySettings->myVerboseLevel > 1) { myWriter->Writer_Warning("JMD_ChromaTag","Input_ColorSpace","Bad Option: Setting to default = RGB",false); }
	}
	
	//return
	mySettings->myInputColorSpace = colorspace_param;
	myInputColorSpaceStr = tmp;
}
/*--- End Private Input Colorspace ---*/

/*----- Private Detection Colorspace -----*/
void JMD::JMD_ChromaTag::Private_Detection_Colorspace(ChromaTag_DetectionColorSpace colorspace_param)
{
	//variables
	std::string tmp;
	
	//case
	switch(colorspace_param)
	{
		case DetectionCS_YUV: tmp = "YUV"; break;
		case DetectionCS_LAB: tmp = "LAB"; break;
		
		default:
			colorspace_param = DetectionCS_LAB;
			tmp = "LAB";
			if(mySettings->myVerboseLevel > 1) { myWriter->Writer_Warning("JMD_ChromaTag","Detection_ColorSpace","Bad Option: Setting to default = LAB",false); }
	}
	
	//return
	mySettings->myDetectionColorSpace = colorspace_param;
	myDetectionColorSpaceStr = tmp;
}
/*--- End Private Detection Colorspace ---*/

/*----- Private Use Rings For Homography -----*/
void JMD::JMD_ChromaTag::Private_UseRingsForHomography(ChromaTag_UseRings use_param)
{
	//variables
	std::string tmp;
	
	//case
	switch(use_param)
	{
		case USE_RINGS_OUTER: tmp = "USE_RINGS_OUTER"; break;
		case USE_RINGS_ALL:   tmp = "USE_RINGS_ALL";   break;
		
		default:
			use_param = USE_RINGS_ALL;
			tmp = "USE_RINGS_ALL";
			if(mySettings->myVerboseLevel > 1) { myWriter->Writer_Warning("JMD_ChromaTag","UseRingsForHomography","Bad Option: Setting to default = USE_RINGS_ALL",false); }
	}
	
	//return
	mySettings->myUseRingsForHomography = use_param;
	myUseRingsForHomographyStr = tmp;
}
/*--- End Private Use Rings For Homography ---*/

/*----- Private Use Rings For Pose -----*/
void JMD::JMD_ChromaTag::Private_UseRingsForPose(ChromaTag_UseRings use_param)
{
	//variables
	std::string tmp;
	
	//case
	switch(use_param)
	{
		case USE_RINGS_OUTER: tmp = "USE_RINGS_OUTER"; break;
		case USE_RINGS_ALL:   tmp = "USE_RINGS_ALL";   break;
		
		default:
			use_param = USE_RINGS_ALL;
			tmp = "USE_RINGS_ALL";
			if(mySettings->myVerboseLevel > 1) { myWriter->Writer_Warning("JMD_ChromaTag","UseRingsForPose","Bad Option: Setting to default = USE_RINGS_ALL",false); }
	}
	
	//return
	mySettings->myUseRingsForPose = use_param;
	myUseRingsForPoseStr = tmp;
}
/*--- End Private Use Rings For Pose ---*/

/*------------- End Set Helpers -------------*/

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//





//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_ChromaTag::JMD_ChromaTag(JMD_Utils_Writer* writer_param)
{
	//set writer if given
	if(writer_param) { myWriter = writer_param;           isHeapWriter = false; }
	else             { myWriter = new JMD_Utils_Writer(); isHeapWriter = true; }
	
	//debug file
	myDebugFile = -1;
	
	//utilities
	myTimer    = new JMD_Utils_Timer();
	mySettings = new JMD_ChromaTag_Settings();
	
	//ChromaTag
	myDetections = new JMD_ChromaTag_Collection();		//Detections Collection
	myDetect = new JMD_ChromaTag_Detect(myDetections);	//Detect Object
	myDecode = new JMD_ChromaTag_Decode(myDetections);	//Decode Object
	myPose   = new JMD_ChromaTag_Pose(myDetections);	//Pose Object
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_ChromaTag::~JMD_ChromaTag() 
{
	//close debug file
	if(myDebugFile > 0) { myWriter->Writer_Close(myDebugFile); }

	//free writer
	if(isHeapWriter) { if(myWriter) { delete myWriter; myWriter = nullptr; } }
	
	//free settings
	if(mySettings) { delete mySettings;  mySettings = nullptr; }
	
	//free utilities
	if(myTimer)         { delete myTimer;     myTimer    = nullptr; }
	
	//free ChromaTag
	if(myDetect)        { delete myDetect;    myDetect   = nullptr; }
	if(myDecode)        { delete myDecode;    myDecode   = nullptr; }
	if(myPose)          { delete myPose;      myPose     = nullptr; }
	if(myDetections) 
	{
		//delete each detection
		for(int i=0; i < myDetections->size(); i++)
		{
			delete myDetections->at(i);
		}
		myDetections->clear();
		
		//delete collection 
		delete myDetections; myDetections = nullptr; 
	}
}
/*------------- End Destructors -------------*/



/*--------------- Setup ---------------*/

/*----- Init -----*/
int JMD::JMD_ChromaTag::Init(JMD_ChromaTag_Settings *settings_param, std::string config_param)
{
    //private init
    Private_Init(settings_param, config_param);
	
	//pass settings on to chroma tag modules
	myDetect->Init(&myLAB_LTable,&myLAB_ATable,&myLAB_BTable,mySettings);
	myDecode->Init(&myLAB_LTable,&myLAB_ATable,&myLAB_BTable,mySettings);
	myPose->Init(mySettings);
	
	//success
	return 0;
}
/*--- End Init ---*/

/*------------- End Setup -------------*/



/*--------------- Tag Detection Methods ---------------*/    

/*---------- Detect ----------*/
void JMD::JMD_ChromaTag::Detect(JMD::JMD_Vision_Image &image_param)
{
	
}
void JMD::JMD_ChromaTag::Detect_RGBToLAB(JMD::JMD_Vision_Image &image_param)
{
	
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag::Detect( cv::Mat &image_param )
{
	/*----- Color Wrappers -----*/
	/*--- End Color Wrappers ---*/
}
void JMD::JMD_ChromaTag::Detect_RGBToLAB( cv::Mat &image_param ) 
{
	//detect
	myDetect->Detect_RGBToLAB(image_param);
}
void JMD::JMD_ChromaTag::Detect_BGRToLAB( cv::Mat &image_param ) 
{
	//detect
	myDetect->Detect_BGRToLAB(image_param);
}
#endif
/*-------- End Detect --------*/

/*---------- Decode ----------*/
void JMD::JMD_ChromaTag::Decode( JMD::JMD_Vision_Image &image_param )
{

}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag::Decode( cv::Mat &image_param )
{
	myDecode->Decode(image_param);
}
#endif
/*-------- End Decode --------*/

/*---------- Pose ----------*/
void JMD::JMD_ChromaTag::Pose()
{
	myPose->Pose();
}
/*-------- End Pose --------*/
            
/*----- Detections -----*/
JMD::JMD_ChromaTag_Collection* JMD::JMD_ChromaTag::Detections() { return myDetections; }
/*--- End Detections ---*/

/*------------- End Tag Detection Methods -------------*/



/*--------------- Utility ---------------*/

/*----- Display Configuration -----*/
void JMD::JMD_ChromaTag::Display_Configuration ()
{
	/*--- make list ---*/
	std::vector<std::string> config_list;
	std::string tmp;
	
	//Verbose Level
	tmp = "Verbose Level:                    " + myWriter->ToString(mySettings->myVerboseLevel) + ".";
	config_list.push_back(tmp);
	
	//Debug Level
	tmp = "Debug Level:                      " + myWriter->ToString(mySettings->myDebugLevel) + ".";
	config_list.push_back(tmp);
	
	//Subsample
	tmp = "Subsample:                        " + myWriter->ToString(mySettings->mySubsample) + ".";
	config_list.push_back(tmp);
	
	//Border
	tmp = "Border:                           " + myWriter->ToString(mySettings->myBorder) + ".";
	config_list.push_back(tmp);
	
	//Hit Threshold
	tmp = "Hit Threshold:                    " + myWriter->ToString(mySettings->myHitThreshold) + ".";
	config_list.push_back(tmp);
	
	//Initial Scan Num
	tmp = "Initial Scan Number:              " + myWriter->ToString(mySettings->myInitialScanNum) + ".";
	config_list.push_back(tmp);
	
	//Max Initial Border Iterations
	tmp = "Max Initial Border Iterations:    " + myWriter->ToString(mySettings->myMaxInitialBorderIterations) + ".";
	config_list.push_back(tmp);
	
	//Max Detections
	tmp = "Max Detections:                   " + myWriter->ToString(mySettings->myMaxDetections) + ".";
	config_list.push_back(tmp);
	
	//Area Ratio Threshold
	tmp = "Area Ratio Threshold:             " + myWriter->ToString(mySettings->myAreaRatioThreshold) + ".";
	config_list.push_back(tmp);
	
	//Tag Family
	tmp = "Tag Family:                       " + myTagFamilyStr + ".";
	config_list.push_back(tmp);
	
	//Input Color Space
	tmp = "Input Color Space:                " + myInputColorSpaceStr + ".";
	config_list.push_back(tmp);
	
	//Detection Color Space
	tmp = "Detection Color Space:            " + myDetectionColorSpaceStr + ".";
	config_list.push_back(tmp);
	
	//LAB Red Threshold
	tmp = "LAB Red Difference Threshold:     " + myWriter->ToString(mySettings->myLABRedDiffThresh) + ".";
	config_list.push_back(tmp);
	
	//Red Green Border Thresh
	tmp = "Red Green Border Threshold:       " + myWriter->ToString(mySettings->myRedGreenBorderThresh) + ".";
	config_list.push_back(tmp);
	
	//Green Black Border Thresh
	tmp = "Green Black Border Threshold:     " + myWriter->ToString(mySettings->myGreenBlackBorderThresh) + ".";
	config_list.push_back(tmp);
	
	//Black White Border Thresh
	tmp = "Black White Border Threshold:     " + myWriter->ToString(mySettings->myBlackWhiteBorderThresh) + ".";
	config_list.push_back(tmp);

	//Max Expand Distance Multiplier
	tmp = "Max Expand Distance Multiplier:   " + myWriter->ToString(mySettings->myMaxExpandDistMultiplier) + ".";
	config_list.push_back(tmp);
	
	//Max Border Refine Iterations
	tmp = "Max Border Refine Iterations:     " + myWriter->ToString(mySettings->myMaxBorderRefineIterations) + ".";
	config_list.push_back(tmp);
	
	//Corner Refine Level
	tmp = "Refine Corners Level:             " + myWriter->ToString(mySettings->myRefineCornersLevel) + ".";
	config_list.push_back(tmp);
	
	//Use Rings For Homography
	tmp = "Use Rings For Homography:         " + myUseRingsForHomographyStr + ".";
	config_list.push_back(tmp);
	
	//Max Search Hamming Distance
	tmp = "Max Search Hamming Distance       " + myWriter->ToString(mySettings->myMaxSearchHammingDistance) + ".";
	config_list.push_back(tmp);
	
	//Use Rings For Pose
	tmp = "Use Rings For Pose:               " + myUseRingsForPoseStr + ".";
	config_list.push_back(tmp);
	
	//Tag Size
	tmp = "Tag Size:                         " + myWriter->ToString(mySettings->myTagSize) + ".";
	config_list.push_back(tmp);
	
	//Intrinsic Matrix
	tmp = "Intrinsic Matrix:                 | " + myWriter->ToString(mySettings->myFX) + " 0 " + myWriter->ToString(mySettings->myCX) + " |\n" +
	      "                                          | 0 " + myWriter->ToString(mySettings->myFY) + " " + myWriter->ToString(mySettings->myCY) + " |\n" +
		  "                                          | 0 0 1 |";
    config_list.push_back(tmp);
	
	//Distortion Coefficients
	tmp = "Distortion Coefficients           | " + myWriter->ToString(mySettings->myK1) + ", " + myWriter->ToString(mySettings->myK2) + ", " + myWriter->ToString(mySettings->myP1) + ", " + myWriter->ToString(mySettings->myP2) + ", " + myWriter->ToString(mySettings->myK3) + " |";
	config_list.push_back(tmp);
	
	/*- end make list -*/
	
	//display list
	myWriter->Writer_List("JMD_ChromaTag","Display_Configuration","ChromaTag Configuration",config_list);
	
}
/*--- End Display Configuration ---*/

/*--------------- Utility ---------------*/



/*--------------- Setters/Getters ---------------*/

/*----- General -----*/

//Verbose Level
void JMD::JMD_ChromaTag::Verbose_Level(int verbose_param)  
{ 
	mySettings->myVerboseLevel = verbose_param;
	if (mySettings->myVerboseLevel > 2) 
	{
		std::string tmp = "Verbose Level Changed to " + myWriter->ToString(mySettings->myVerboseLevel);
		myWriter->Writer_Note("JMD_ChromaTag","Verbose_Level",tmp,true); 
	}
}
void JMD::JMD_ChromaTag::Verbose_Level(int *verbose_param) { *verbose_param = mySettings->myVerboseLevel; }

//Debug Level
void JMD::JMD_ChromaTag::Debug_Level(int debug_param)
{
	mySettings->myDebugLevel = debug_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Debug Level Changed to " + myWriter->ToString(mySettings->myDebugLevel);
		myWriter->Writer_Note("JMD_ChromaTag","Debug_Level",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Debug_Level(int *debug_param) { *debug_param = mySettings->myDebugLevel; }

/*--- End General ---*/


/*----- Detection -----*/

//Subsample
void JMD::JMD_ChromaTag::Subsample(unsigned int subsample_param)
{
	mySettings->mySubsample = subsample_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Subsample Changed to " + myWriter->ToString(mySettings->mySubsample);
		myWriter->Writer_Note("JMD_ChromaTag","Subsample",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Subsample(unsigned int *subsample_param) { *subsample_param = mySettings->mySubsample; }

//Border
void JMD::JMD_ChromaTag::Border(unsigned int border_param)
{
	mySettings->myBorder = border_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Border Changed to " + myWriter->ToString(mySettings->myBorder);
		myWriter->Writer_Note("JMD_ChromaTag","Border",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Border(unsigned int *border_param) { *border_param = mySettings->myBorder; }

//Hit Threshold
void JMD::JMD_ChromaTag::HitThreshold(unsigned int threshold_param)
{
	mySettings->myHitThreshold = threshold_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Hit Threshold Changed to " + myWriter->ToString(mySettings->myHitThreshold);
		myWriter->Writer_Note("JMD_ChromaTag","HitThreshold",tmp,true);
	}
}
void JMD::JMD_ChromaTag::HitThreshold(unsigned int *threshold_param) { *threshold_param = mySettings->myHitThreshold; }

//Initial Scan Num
void JMD::JMD_ChromaTag::InitialScanNum(unsigned int num_param)
{
	mySettings->myInitialScanNum = num_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Initial Scan Number Changed to " + myWriter->ToString(mySettings->myInitialScanNum);
		myWriter->Writer_Note("JMD_ChromaTag","InitialScanNum",tmp,true);
	}
}
void JMD::JMD_ChromaTag::InitialScanNum(unsigned int *num_param) { *num_param = mySettings->myInitialScanNum; }
		
//Max Initial Border Iterations
void JMD::JMD_ChromaTag::MaxInitialBorderIterations(unsigned int iterations_param)
{
	mySettings->myMaxInitialBorderIterations = iterations_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Max Initial Border Iterations Changed to " + myWriter->ToString(mySettings->myMaxInitialBorderIterations);
		myWriter->Writer_Note("JMD_ChromaTag","MaxInitialBorderIterations",tmp,true);
	}
}
void JMD::JMD_ChromaTag::MaxInitialBorderIterations(unsigned int *iterations_param) { *iterations_param = mySettings->myMaxInitialBorderIterations; }
		
//Max Detections
void JMD::JMD_ChromaTag::MaxDetections(unsigned int detections_param)
{
	mySettings->myMaxDetections = detections_param;
	if(mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Max Detections Changed to " + myWriter->ToString(mySettings->myMaxDetections);
		myWriter->Writer_Note("JMD_ChromaTag","MaxDetections",tmp,true);
	}
}
void JMD::JMD_ChromaTag::MaxDetections(unsigned int *detections_param) { *detections_param = mySettings->myMaxDetections; }

//Area Ratio Threshold
void JMD::JMD_ChromaTag::AreaRatioThreshold(double threshold_param)
{
	mySettings->myAreaRatioThreshold = threshold_param;
	if(mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Area Ratio Threshold Changed to " + myWriter->ToString(mySettings->myAreaRatioThreshold);
		myWriter->Writer_Note("JMD_ChromaTag","AreaRatioThreshold",tmp,true);
	}
}
void JMD::JMD_ChromaTag::AreaRatioThreshold(double *threshold_param) { *threshold_param = mySettings->myAreaRatioThreshold; }

//Tag Family
void JMD::JMD_ChromaTag::Tag_Family(JMD::ChromaTag_Family family_param)
{
	Private_Tag_Family(family_param);
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Tag Family Changed to " + myTagFamilyStr;
		myWriter->Writer_Note("JMD_ChromaTag","Tag_Family",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Tag_Family(JMD::ChromaTag_Family *family_param, std::string *family_str_param)
{
	*family_param = mySettings->myTagFamily;
	if(family_str_param) { *family_str_param = myTagFamilyStr; }
}

//Input Color Space
void JMD::JMD_ChromaTag::Input_Colorspace(JMD::ChromaTag_InputColorSpace colorspace_param) 
{
	Private_Input_Colorspace(colorspace_param);
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Input Color Space Changed to " + myInputColorSpaceStr;
		myWriter->Writer_Note("JMD_ChromaTag","Input_Colorspace",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Input_Colorspace(JMD::ChromaTag_InputColorSpace *colorspace_param, std::string *colorspace_str_param) 
{
	*colorspace_param = mySettings->myInputColorSpace;
	if(colorspace_str_param) { *colorspace_str_param = myInputColorSpaceStr; }
}

//Detection Color Space
void JMD::JMD_ChromaTag::Detection_Colorspace(JMD::ChromaTag_DetectionColorSpace colorspace_param) 
{
	Private_Detection_Colorspace(colorspace_param);
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Detection Color Space Changed to " + myDetectionColorSpaceStr;
		myWriter->Writer_Note("JMD_ChromaTag","Detection_Colorspace",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Detection_Colorspace(JMD::ChromaTag_DetectionColorSpace *colorspace_param, std::string *colorspace_str_param) 
{
	*colorspace_param = mySettings->myDetectionColorSpace;
	if(colorspace_str_param) { *colorspace_str_param = myDetectionColorSpaceStr; }
}

//LAB Red Difference Threshold
void JMD::JMD_ChromaTag::LABRedDiffThresh(double thresh_param)
{
	mySettings->myLABRedDiffThresh = thresh_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "LAB Red Difference Threshold Changed to " + myWriter->ToString(mySettings->myLABRedDiffThresh);
		myWriter->Writer_Note("JMD_ChromaTag","LABRedDiffThresh",tmp,true);
	}
}
void JMD::JMD_ChromaTag::LABRedDiffThresh(double *thresh_param) { *thresh_param = mySettings->myLABRedDiffThresh; }

//Red Green Border Thresh
void JMD::JMD_ChromaTag::RedGreenBorderThresh(double thresh_param)
{
	mySettings->myRedGreenBorderThresh = thresh_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Red Green Border Threshold Changed to " + myWriter->ToString(mySettings->myRedGreenBorderThresh);
		myWriter->Writer_Note("JMD_ChromaTag","RedGreenBorderThresh",tmp,true);
	}
}
void JMD::JMD_ChromaTag::RedGreenBorderThresh(double *thresh_param) { *thresh_param = mySettings->myRedGreenBorderThresh; }

//Green Black Border Thresh
void JMD::JMD_ChromaTag::GreenBlackBorderThresh(double thresh_param)
{
	mySettings->myGreenBlackBorderThresh = thresh_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Green Black Border Threshold Changed to " + myWriter->ToString(mySettings->myGreenBlackBorderThresh);
		myWriter->Writer_Note("JMD_ChromaTag","GreenBlackBorderThresh",tmp,true);
	}
}
void JMD::JMD_ChromaTag::GreenBlackBorderThresh(double *thresh_param) { *thresh_param = mySettings->myGreenBlackBorderThresh; }

//Black White Border Thresh
void JMD::JMD_ChromaTag::BlackWhiteBorderThresh(double thresh_param)
{
	mySettings->myBlackWhiteBorderThresh = thresh_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Black White Border Threshold Changed to " + myWriter->ToString(mySettings->myBlackWhiteBorderThresh);
		myWriter->Writer_Note("JMD_ChromaTag","BlackWhiteBorderThresh",tmp,true);
	}
}
void JMD::JMD_ChromaTag::BlackWhiteBorderThresh(double *thresh_param) { *thresh_param = mySettings->myBlackWhiteBorderThresh; }

//Max Expand Distance Multiplier
void JMD::JMD_ChromaTag::MaxExpandDistMultiplier(double multiplier_param)
{
	mySettings->myMaxExpandDistMultiplier = multiplier_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Max Expand Distance Multiplier Changed to " + myWriter->ToString(mySettings->myMaxExpandDistMultiplier);
		myWriter->Writer_Note("JMD_ChromaTag","MaxExpandDistMultiplier",tmp,true);
	}
}
void JMD::JMD_ChromaTag::MaxExpandDistMultiplier(double *multiplier_param) { *multiplier_param = mySettings->myMaxExpandDistMultiplier; }


//Max Border Refine Iterations
void JMD::JMD_ChromaTag::MaxBorderRefineIterations(unsigned int iters_param)
{
	mySettings->myMaxBorderRefineIterations = iters_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Max Border Refine Iterations Changed to " + myWriter->ToString(mySettings->myMaxBorderRefineIterations);
		myWriter->Writer_Note("JMD_ChromaTag","MaxBorderRefineIterations",tmp,true);
	}
}
void JMD::JMD_ChromaTag::MaxBorderRefineIterations(unsigned int *iters_param) { *iters_param = mySettings->myMaxBorderRefineIterations; }

//Refine Corners Level
void JMD::JMD_ChromaTag::RefineCorners_Level(unsigned int level_param)
{
	mySettings->myRefineCornersLevel = level_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Refine Corners Level Changed to " + myWriter->ToString(mySettings->myRefineCornersLevel);
		myWriter->Writer_Note("JMD_ChromaTag","RefineCorners_Level",tmp,true);
	}
}
void JMD::JMD_ChromaTag::RefineCorners_Level(unsigned int *level_param) { *level_param = mySettings->myRefineCornersLevel; }

/*--- End Detection ---*/
   
/*----- Decode -----*/

//Use Rings For Homography
void JMD::JMD_ChromaTag::UseRingsForHomography(JMD::ChromaTag_UseRings use_param)
{
	Private_UseRingsForHomography(use_param);
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Use Rings For Homography Changed to " + myUseRingsForHomographyStr;
		myWriter->Writer_Note("JMD_ChromaTag","UseRingsForHomography",tmp,true);
	}
}
void JMD::JMD_ChromaTag::UseRingsForHomography(JMD::ChromaTag_UseRings *use_param, std::string *use_str_param)
{
	*use_param = mySettings->myUseRingsForHomography;
	if(use_str_param) { *use_str_param = myUseRingsForHomographyStr; }
}

//Max Search Hamming Distance
void JMD::JMD_ChromaTag::MaxSearchHammingDistance(unsigned int distance_param)
{
	mySettings->myMaxSearchHammingDistance = distance_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Max Search Hamming Distance Changed to " + myWriter->ToString(mySettings->myMaxSearchHammingDistance);
		myWriter->Writer_Note("JMD_ChromaTag","MaxSearchHammingDistance",tmp,true);
	}
}
void JMD::JMD_ChromaTag::MaxSearchHammingDistance(unsigned int *distance_param) { *distance_param = mySettings->myMaxSearchHammingDistance; }

/*--- End Decode ---*/

/*----- Pose -----*/

//Use Rings For Pose
void JMD::JMD_ChromaTag::UseRingsForPose(JMD::ChromaTag_UseRings use_param)
{
	Private_UseRingsForPose(use_param);
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Use Rings For Pose Changed to " + myUseRingsForPoseStr;
		myWriter->Writer_Note("JMD_ChromaTag","UseRingsForPose",tmp,true);
	}
}
void JMD::JMD_ChromaTag::UseRingsForPose(JMD::ChromaTag_UseRings *use_param, std::string *use_str_param)
{
	*use_param = mySettings->myUseRingsForPose;
	if(use_str_param) { *use_str_param = myUseRingsForPoseStr; }
}

//Tag Size
void JMD::JMD_ChromaTag::TagSize(double size_param)
{
	mySettings->myTagSize = size_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Tag Size Changed to " + myWriter->ToString(mySettings->myTagSize);
		myWriter->Writer_Note("JMD_ChromaTag","TagSize",tmp,true);
	}
}
void JMD::JMD_ChromaTag::TagSize(double *size_param) { *size_param = mySettings->myTagSize; }

//FX
void JMD::JMD_ChromaTag::FX(double fx_param)
{
	mySettings->myFX = fx_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "FX Changed to " + myWriter->ToString(mySettings->myFX);
		myWriter->Writer_Note("JMD_ChromaTag","FX",tmp,true);
	}
}
void JMD::JMD_ChromaTag::FX(double *fx_param) { *fx_param = mySettings->myFX; }

//FY
void JMD::JMD_ChromaTag::FY(double fy_param)
{
	mySettings->myFY = fy_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "FY Changed to " + myWriter->ToString(mySettings->myFY);
		myWriter->Writer_Note("JMD_ChromaTag","FY",tmp,true);
	}
}
void JMD::JMD_ChromaTag::FY(double *fy_param) { *fy_param = mySettings->myFY; }

//CX
void JMD::JMD_ChromaTag::CX(double cx_param)
{
	mySettings->myCX = cx_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "CX Changed to " + myWriter->ToString(mySettings->myCX);
		myWriter->Writer_Note("JMD_ChromaTag","CX",tmp,true);
	}
}
void JMD::JMD_ChromaTag::CX(double *cx_param) { *cx_param = mySettings->myCX; }

//CY
void JMD::JMD_ChromaTag::CY(double cy_param)
{
	mySettings->myCY = cy_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "CY Changed to " + myWriter->ToString(mySettings->myCY);
		myWriter->Writer_Note("JMD_ChromaTag","CY",tmp,true);
	}
}
void JMD::JMD_ChromaTag::CY(double *cy_param) { *cy_param = mySettings->myCY; }

//Intrinsics
void JMD::JMD_ChromaTag::Intrinsics(double fx_param, double fy_param, double cx_param, double cy_param)
{
	mySettings->myFX = fx_param;
	mySettings->myFY = fy_param;
	mySettings->myCX = cx_param;
	mySettings->myCY = cy_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Intrinsics Changed to | " + myWriter->ToString(mySettings->myFX) + " 0 " + myWriter->ToString(mySettings->myCX) + " |\n";
		           tmp += "                      | 0 " + myWriter->ToString(mySettings->myFY) + " " + myWriter->ToString(mySettings->myCY) + " |\n";
				   tmp += "                      | 0 0 1 |";
		myWriter->Writer_Note("JMD_ChromaTag","Intrinsics",tmp,true);
	}
}
void JMD::JMD_ChromaTag::Intrinsics(double *fx_param, double *fy_param, double *cx_param, double *cy_param)
{
	*fx_param = mySettings->myFX;
	*fy_param = mySettings->myFY;
	*cx_param = mySettings->myCX;
	*cy_param = mySettings->myCY;
}

//K1
void JMD::JMD_ChromaTag::K1(double k1_param)
{
	mySettings->myK1 = k1_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "K1 Changed to " + myWriter->ToString(mySettings->myK1);
		myWriter->Writer_Note("JMD_ChromaTag","K1",tmp,true);
	}
}
void JMD::JMD_ChromaTag::K1(double *k1_param) { *k1_param = mySettings->myK1; }

//K2
void JMD::JMD_ChromaTag::K2(double k2_param)
{
	mySettings->myK2 = k2_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "K2 Changed to " + myWriter->ToString(mySettings->myK2);
		myWriter->Writer_Note("JMD_ChromaTag","K2",tmp,true);
	}
}
void JMD::JMD_ChromaTag::K2(double *k2_param) { *k2_param = mySettings->myK2; }

//K3
void JMD::JMD_ChromaTag::K3(double k3_param)
{
	mySettings->myK3 = k3_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "K3 Changed to " + myWriter->ToString(mySettings->myK3);
		myWriter->Writer_Note("JMD_ChromaTag","K3",tmp,true);
	}
}
void JMD::JMD_ChromaTag::K3(double *k3_param) { *k3_param = mySettings->myK3; }

//P1
void JMD::JMD_ChromaTag::P1(double p1_param)
{
	mySettings->myP1 = p1_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "P1 Changed to " + myWriter->ToString(mySettings->myP1);
		myWriter->Writer_Note("JMD_ChromaTag","P1",tmp,true);
	}
}
void JMD::JMD_ChromaTag::P1(double *p1_param) { *p1_param = mySettings->myP1; }

//P2
void JMD::JMD_ChromaTag::P2(double p2_param)
{
	mySettings->myP2 = p2_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "P2 Changed to " + myWriter->ToString(mySettings->myP2);
		myWriter->Writer_Note("JMD_ChromaTag","P2",tmp,true);
	}
}
void JMD::JMD_ChromaTag::P2(double *p2_param) { *p2_param = mySettings->myP2; }

//Distortion Coefficients
void JMD::JMD_ChromaTag::DistortionCoefficients(double k1_param, double k2_param, double k3_param, double p1_param, double p2_param)
{
	mySettings->myK1 = k1_param;
	mySettings->myK2 = k2_param;
	mySettings->myK2 = k3_param;
	mySettings->myP1 = p1_param;
	mySettings->myP2 = p2_param;
	if (mySettings->myVerboseLevel > 2)
	{
		std::string tmp = "Distortion Coefficients Changed to [ " + myWriter->ToString(mySettings->myK1) + ", " + myWriter->ToString(mySettings->myK2) + ", " + ", " + myWriter->ToString(mySettings->myP1) + ", " + myWriter->ToString(mySettings->myP2) + myWriter->ToString(mySettings->myK3) + " ]";
		myWriter->Writer_Note("JMD_ChromaTag","DistortionCoefficients",tmp,true);
	}
}
void JMD::JMD_ChromaTag::DistortionCoefficients(double *k1_param, double *k2_param, double *k3_param, double *p1_param, double *p2_param)
{
	*k1_param = mySettings->myK1;
	*k2_param = mySettings->myK2;
	*k3_param = mySettings->myK3;
	*p1_param = mySettings->myP1;
	*p2_param = mySettings->myP2;
}

/*--- End Pose ---*/

/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/
/*--------------- Operator Overloads ---------------*/



/*--------------- DEBUG Methods ---------------*/
/*------------- End DEBUG Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
