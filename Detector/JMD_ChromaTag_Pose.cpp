/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Pose: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Pose.cpp
 A class for ChromaTag Pose Estimation
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

#include "JMD_ChromaTag_Pose.hpp"

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
//================= JMD ChromaTag Pose Implementation ================//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//
        
/*--------------- Setup Methods ---------------*/

/*----- Private Init -----*/
void JMD::JMD_ChromaTag_Pose::Private_Init(JMD_ChromaTag_Settings *settings_param)
{
	//settings
	mySettings = settings_param;
	
	//number of rings to use
	switch(mySettings->myUseRingsForPose)
	{
		case USE_RINGS_OUTER: myNumRingsToUse = 1; break;
		case USE_RINGS_ALL:   myNumRingsToUse = 3; break;
		default: myNumRingsToUse = 1; break;
	}
	
	//populate the image plane locations based on the number
	//of rings that will be used
	double half_size = mySettings->myTagSize / 2;
	double s;
	for(int i = 0; i < myNumRingsToUse; i++)
	{
		//update s
		s = (myNumRingsToUse - i) * half_size / myNumRingsToUse;
		
		//add four points
		myWorldPoints.push_back( cv::Point3f( s,  s, 0.0) );	//top right
		myWorldPoints.push_back( cv::Point3f( s, -s, 0.0) );	//bottom right
		myWorldPoints.push_back( cv::Point3f(-s, -s, 0.0) );	//bottom left
		myWorldPoints.push_back( cv::Point3f(-s,  s, 0.0) );	//top left
	}
	
	//intrinsic matrix
	myK[0] = mySettings->myFX;  myK[1] = 0.0;  myK[2] = mySettings->myCX;
	myK[3] = 0.0;  myK[4] = mySettings->myFY;  myK[5] = mySettings->myCY;
	myK[6] = 0.0;  myK[7] = 0.0;               myK[8] = 1.0;
	
	//distortion coefficients
	myDistCoefs[0] = mySettings->myK1;
	myDistCoefs[1] = mySettings->myK2;
	myDistCoefs[2] = mySettings->myP1;
	myDistCoefs[3] = mySettings->myP2;
	myDistCoefs[4] = mySettings->myK3;
}
/*--- End Private Init ---*/

/*------------- End Setup Methods -------------*/


/*--------------- Pose Methods ---------------*/

/*----- Private Pose -----*/
void JMD::JMD_ChromaTag_Pose::Private_Pose()
{
	//variables
	JMD_ChromaTag_Detection *curr_detection;
	TagBorderSet     *curr_borders;
	TagBorderRing    *curr_ring;
	TagBorderSegment *curr_seg;
	std::vector<cv::Point2f> image_points;
	cv::Mat rvec, tvec;
	
	//for each detection
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//ptr to current detection
		curr_detection = *it;
		
		//Was Active? If not, break, we have reached end of active detections
		if(!curr_detection->IsActive) { break; }
		
		//point to detection's borders
		curr_borders = &(curr_detection->TagBorders);
		
		//for each ring
		for(int rit = 1; rit <= myNumRingsToUse; rit++)
		{
			//current ring = last_ring - rit
			curr_ring = curr_borders->at( curr_borders->size() - rit );
			
			//for each segment
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				//current segment
				curr_seg = *sit;
				
				//save corner point
				image_points.push_back( cv::Point2f( curr_seg->myLine.Point1()->U(), curr_seg->myLine.Point1()->V() ) );
			}
		}
		
		//solve PnP problem
		if( Private_PnP(image_points, rvec, tvec) )
		{
			//mark that a pose was successfully calculated
			curr_detection->IsPose = true;
			
			//populate detection with rotation vector
			curr_detection->TagRotation[0] = rvec.at<double>(0);
			curr_detection->TagRotation[1] = rvec.at<double>(1);
			curr_detection->TagRotation[2] = rvec.at<double>(2);
			curr_detection->TagRotation[3] = rvec.at<double>(3);
			
			//populate detection with translation vector
			curr_detection->TagTranslation[0] = tvec.at<double>(0);
			curr_detection->TagTranslation[1] = tvec.at<double>(1);
			curr_detection->TagTranslation[2] = tvec.at<double>(2);
		}
		else
		{
			//failed to find a pose
			curr_detection->IsPose = false;
		}
		
		//clear image_points
		image_points.clear();
	}
	
}
/*--- End Private Pose ---*/

/*----- Private PNP -----*/
bool JMD::JMD_ChromaTag_Pose::Private_PnP(std::vector<cv::Point2f> &image_points_param, cv::Mat &rvec_param, cv::Mat &tvec_param)
{
	//create cv::Mat version of intrinsics and distortion coefficients
	cv::Mat K(3, 3, CV_64F, myK);
	cv::Mat Dist(1, 5, CV_64F, myDistCoefs);
	
	//try pnp
	try
	{
		cv::solvePnP(myWorldPoints, image_points_param, K, Dist, rvec_param, tvec_param);
	}
	catch (...)
	{
		return false;
	}
	
	//success
	return true;
}
/*--- End Private PNP ---*/

/*------------- End Pose Methods -------------*/
		
//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//





//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_ChromaTag_Pose::JMD_ChromaTag_Pose(JMD_ChromaTag_Collection *collection_param)
{
	//collection
	myDetections = collection_param;
	
	//intrinsics has 9 values [ fx 0 cx; 0 fy cy; 0 0 1 ]
	myK = new double[9];
	
	//distortion coefficients has 5 values [ k1 k2 p1 p2 k3 ]
	myDistCoefs = new double[5];
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_ChromaTag_Pose::~JMD_ChromaTag_Pose() 
{
	//delete arrays
	delete myK;
	delete myDistCoefs;
}
/*------------- End Destructors -------------*/



/*--------------- Setup ---------------*/

/*----- Init -----*/
void JMD::JMD_ChromaTag_Pose::Init(JMD_ChromaTag_Settings *settings_param) 
{ 
	Private_Init(settings_param);
}
/*--- End Init ---*/

/*------------- End Setup -------------*/

		
		
/*--------------- Tag Pose Estimation Methods ---------------*/    

/*---------- Pose ----------*/
void JMD::JMD_ChromaTag_Pose::Pose()
{
	Private_Pose();
}
/*-------- End Pose --------*/
            
/*------------- End Tag Pose Estimation Methods -------------*/
        
        

/*--------------- Utility ---------------*/
/*--------------- Utility ---------------*/



/*--------------- Setters/Getters ---------------*/    
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
