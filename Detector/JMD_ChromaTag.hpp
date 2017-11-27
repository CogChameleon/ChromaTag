#pragma once
#ifndef JMD_CHROMATAG_H_
#define JMD_CHROMATAG_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag.hpp
 A class for ChromaTag - a colored fiducial marker detection system
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 
 --- End Enumerations ---
 
 ----- Constructors -----
 
 --- end Constructors ---
 
 ----- Utility Methods -----
 
 --- End Utility Methods ---
 
 Notes:
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

//system
#include <cstring>

//ChromaTag
#include "JMD_ChromaTag_Common.hpp"
#include "JMD_ChromaTag_Detect.hpp"
#include "JMD_ChromaTag_Decode.hpp"
#include "JMD_ChromaTag_Pose.hpp"

//utilities
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Timer.hpp"

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
	
    /*--------------- Forward Declerations ---------------*/
	//struct JMD_Settings_ChromaTag; //see below
	/*------------- End Forward Declerations -------------*/
    
    //====================================================================//
    //====================================================================//
    //==================== JMD ChromaTag Definition ======================//
    //====================================================================//
    //====================================================================//
    class JMD_ChromaTag
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        
        /*------------- End Enumerations -------------*/
        
        /*--------------- Variables ---------------*/
        
        //settings defaults
        static const int                           DEFAULT_VERBOSE_LEVEL               =  0;
        static const int                           DEFAULT_DEBUG_LEVEL                 =  0;
        static const ChromaTag_Family              DEFAULT_TAG_FAMILY                  =  ChromaTag_Family::RGBW_16H5;
        static const ChromaTag_DetectionColorSpace DEFAULT_DETECTION_COLORSPACE        =  ChromaTag_DetectionColorSpace::DetectionCS_LAB;
        static const ChromaTag_InputColorSpace     DEFAULT_INPUT_COLORSPACE            =  ChromaTag_InputColorSpace::InputCS_RGB;
        static const ChromaTag_UseRings            DEFAULT_USERINGSFORPOSE             =  ChromaTag_UseRings::USE_RINGS_ALL;
        static const ChromaTag_UseRings            DEFAULT_USERINGSFORHOMOGRAPHY       =  ChromaTag_UseRings::USE_RINGS_ALL;
        static const unsigned int                  DEFAULT_SUBSAMPLE                   =  10;
        static const unsigned int                  DEFAULT_BORDER                      =  20;
        static const unsigned int                  DEFAULT_HITTHRESHOLD                =   3;
        static const unsigned int                  DEFAULT_INITIALSCANNUM              =   4;
        static const unsigned int                  DEFAULT_MAXINITIALBORDERITERATIONS  =  20;
        static const unsigned int                  DEFAULT_MAX_DETECTIONS              =  90;
        static const unsigned int                  DEFAULT_MAXBORDERREFINEITERATIONS   =  14;
        static const unsigned int                  DEFAULT_REFINECORNERS_LEVEL         =   0;
        static const unsigned int                  DEFAULT_MAXSEARCHHAMMINGDISTANCE    =   2;
        static constexpr double                    DEFAULT_AREARATIOTHRESHOLD          =   0.98;
        static constexpr double                    DEFAULT_LABREDDIFFTHRESH            =  25.0;
        static constexpr double                    DEFAULT_REDGREENBORDERTHRESH        =  20.0;
        static constexpr double                    DEFAULT_GREENBLACKBORDERTHRESH      =   7.0;
        static constexpr double                    DEFAULT_BLACKWHITEBORDERTHRESH      =   7.0;
        static constexpr double                    DEFAULT_MAXEXPANDDISTMULTIPLIER     =   1.1;
        static constexpr double                    DEFAULT_TAGSIZE                     =   1.0;
        static constexpr double                    DEFAULT_FX                          =   1.0;
        static constexpr double                    DEFAULT_FY                          =   1.0;
        static constexpr double                    DEFAULT_CX                          =   0.0;
        static constexpr double                    DEFAULT_CY                          =   0.0;
        static constexpr double                    DEFAULT_K1                          =   0.0;
        static constexpr double                    DEFAULT_K2                          =   0.0;
        static constexpr double                    DEFAULT_K3                          =   0.0;
        static constexpr double                    DEFAULT_P1                          =   0.0;
        static constexpr double                    DEFAULT_P2                          =   0.0;
        
        /*------------- End Variables -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
		
		//flags
		bool isHeapWriter;
        bool isHeapSettings;
        
        //ChromaTag
        JMD_ChromaTag_Detect     *myDetect;
        JMD_ChromaTag_Decode     *myDecode;
        JMD_ChromaTag_Pose       *myPose;
        JMD_ChromaTag_Collection *myDetections;

        //Color Hash Tables
        std::vector<double>      myLAB_LTable;
        std::vector<double>      myLAB_ATable;
        std::vector<double>      myLAB_BTable;
		
		//Utilities
		JMD_Utils_Writer         *myWriter;
        JMD_Utils_Timer          *myTimer;
        
        //Settings
        JMD_ChromaTag_Settings   *mySettings;
        std::string               myTagFamilyStr;
        std::string               myDetectionColorSpaceStr;
        std::string               myInputColorSpaceStr;
        std::string               myUseRingsForHomographyStr;
        std::string               myUseRingsForPoseStr;
        int                       myDebugFile;
		
        /*------------- End Variables -------------*/
        
        
        /*--------------- General Helpers ---------------*/
        
        /*----- Private Read Config File -----*/
        // reads settings values from the config file
        //
        // input:
        //    fs_param - OpenCV Filestorage object reference
        //    key_param - key to read from config file
        // output:
        //    bool - true if success, false otherwise
        bool Private_ReadConfigFile(cv::FileStorage &fs_param, std::string key_param = "");
        /*--- End Private Read Config File ---*/
        
        /*----- Private Init -----*/
        // does all the initialization of private instance variables
        //
        // input:
        //    settings_param - JMD_ChromaTag_Settings object ptr
        //    config_param - path to config*.yaml file for ChromaTags settings
        // output:
        //    int - 0 if success, failure otherwise
        int Private_Init(JMD_ChromaTag_Settings *settings_param = nullptr, std::string config_param = "");
        /*--- End Private Init ---*/
        
        /*------------- End General Heleprs -------------*/
        
        
        /*--------------- Set Helpers ---------------*/
        
        /*----- Private Tag Family -----*/
        // constrains the possible tag design to detect based on
        // the established enumeration values
        //
        // input:
        //    family_param - tag family
        void Private_Tag_Family(ChromaTag_Family family_param);
        /*--- End Private Tag Family ---*/
        
        /*----- Private Input Colorspace -----*/
        // constrains the possible input colorspace options based on 
        // the established enumeration values
        //
        // input:
        //    colorspace_param - input colorspace
        void Private_Input_Colorspace(ChromaTag_InputColorSpace colorspace_param);
        /*--- End Private Input Colorspace ---*/
        
        /*----- Private Detection Colorspace -----*/
        // constrains the possible detection colorspace options based 
        // on the established enumeration values
        //
        // input:
        //    colorspace_param - input colorspace
        void Private_Detection_Colorspace(ChromaTag_DetectionColorSpace colorspace_param);
        /*--- End Private Detection Colorspace ---*/
        
        /*----- Private Use Rings For Homography -----*/
        // specifies which set of rings to use when calculating
        // the homography
        //
        // input:
        //     use_param - enumeration value for which rings to use
        void Private_UseRingsForHomography(ChromaTag_UseRings use_param);
        /*--- End Private Use Rings For Homography ---*/
        
        /*----- Private Use Rings For Pose -----*/
        // specifies which set of rings to use when calculating
        // the pose
        //
        // input:
        //     use_param - enumeration value for which rings to use
        void Private_UseRingsForPose(ChromaTag_UseRings use_param);
        /*--- End Private Use Rings For Pose ---*/
        
        /*------------- End Set Helpers -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
		/*--------------- Constructors ---------------*/
		// Constructor
        //
        // input:
        //    writer_param - JMD_Utils_Writer pointer
        JMD_ChromaTag(JMD_Utils_Writer* writer_param = nullptr);
		/*------------- end Constructors -------------*/
		
        
		
		/*--------------- Destructors ---------------*/
		~JMD_ChromaTag();
		/*-------------- End Destructors ------------*/
        
        
        
        /*--------------- Setup ---------------*/
        
        /*----- Init -----*/
        // allows the user to specify parameters for ChromaTag
        //
        // input:
        //    settings_param - JMD_Settings_ChromaTag struct with settings
        //    config_param - path to config*.yaml file for ChromaTags settings
        // output:
        //    int - true if 1, failure otherwise
        int Init(JMD_ChromaTag_Settings *settings_param = NULL, std::string config_param = "");
        /*--- End Init ---*/
        
        /*------------- End Setup -------------*/
        
        
        
        /*--------------- Tag Detection Methods ---------------*/
        
        /*----- Detect -----*/
        // detects the tag in the image
        //
        // input:
        //    image_param - image to process
        void Detect( JMD_Vision_Image &image_param );
        void Detect_RGBToLAB( JMD_Vision_Image &image_param );
        void Detect_BGRToLAB( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND 
            void Detect( cv::Mat &image_param );
            void Detect_RGBToLAB( cv::Mat &image_param );
            void Detect_BGRToLAB( cv::Mat &image_param );
        #endif
        /*--- End Detect ---*/
        
        /*----- Decode -----*/
        // decodes the tag for found detections, should be called
        // after detect
        //
        // input:
        //    image_param - same image that was passed to detect
        void Decode( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND
            void Decode( cv::Mat &image_param );
        #endif
        /*--- End Decode ---*/
        
        /*----- Pose -----*/
        // calculates the pose for each detection, should be called
        // after detect and decode
        //
        void Pose();
        /*--- End Pose ---*/
        
        /*----- Detections -----*/
        // returns a pointer to the collection of tag detections
        //
        // output:
        //    JMD_ChromaTag_Collection - ptr to underlying set of detections
        JMD_ChromaTag_Collection* Detections();
        /*--- End Detections ---*/
        
        /*------------- End Tag Detection Methods -------------*/
        
                
        
        /*--------------- Utility Methods ---------------*/
        
        /*----- Display Configuration -----*/
        void Display_Configuration();
        /*--- End Display Configuration ---*/
        
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        
        /*----- General -----*/
        
        //Verbose Level
        void Verbose_Level(int verbose_param);
        void Verbose_Level(int *verbose_param);
        
        //Debug Level
        void Debug_Level(int debug_param);
        void Debug_Level(int *debug_param);
        
        /*--- End General ---*/


        /*----- Detection -----*/
        
        //Subsample
        void Subsample(unsigned int subsample_param);
        void Subsample(unsigned int *subsample_param);
        
        //Border
        void Border(unsigned int border_param);
        void Border(unsigned int *border_param);
        
        //Hit Threshold
        void HitThreshold(unsigned int threshold_param);
        void HitThreshold(unsigned int *threshold_param);

        //Initial Scan Num
        void InitialScanNum(unsigned int num_param);
        void InitialScanNum(unsigned int *num_param);
        
        //Max Initial Border Iterations
        void MaxInitialBorderIterations(unsigned int iterations_param);
        void MaxInitialBorderIterations(unsigned int *iterations_param);
        
        //Max Detections
        void MaxDetections(unsigned int detections_param);
        void MaxDetections(unsigned int *detections_param);
        
        //Min Area Ratio Threshold
        void AreaRatioThreshold(double threshold_param);
        void AreaRatioThreshold(double *threshold_param);
        
        //Tag Family
        void Tag_Family(JMD::ChromaTag_Family family_param);
        void Tag_Family(JMD::ChromaTag_Family *family_param, std::string *family_str_param = NULL);
        
        //Input Color Space
        void Input_Colorspace(JMD::ChromaTag_InputColorSpace colorspace_param);
        void Input_Colorspace(JMD::ChromaTag_InputColorSpace *colorspace_param, std::string *colorspace_str_param = NULL);
        
        //Detection Color Space
        void Detection_Colorspace(JMD::ChromaTag_DetectionColorSpace colorspace_param);
        void Detection_Colorspace(JMD::ChromaTag_DetectionColorSpace *colorspace_param, std::string *colorspace_str_param = NULL);
        
        //LAB Red Difference Threshold
        void LABRedDiffThresh(double thresh_param);
        void LABRedDiffThresh(double *thresh_param);
        
        //Red Green Border Threshold
        void RedGreenBorderThresh(double thresh_param);
        void RedGreenBorderThresh(double *thresh_param);
        
        //Green Black Border Threshold
        void GreenBlackBorderThresh(double thresh_param);
        void GreenBlackBorderThresh(double *thresh_param);
        
        //Black White Border Threshold
        void BlackWhiteBorderThresh(double thresh_param);
        void BlackWhiteBorderThresh(double *thresh_param);

        //Max Expand Distance Multiplier
        void MaxExpandDistMultiplier(double multiplier_param);
        void MaxExpandDistMultiplier(double *multiplier_param);
        
        //Max Border Refine Iterations
        void MaxBorderRefineIterations(unsigned int iters_param);
        void MaxBorderRefineIterations(unsigned int *iters_param);
        
        //Refine Corners Level
        void RefineCorners_Level(unsigned int level_param);
        void RefineCorners_Level(unsigned int *level_param);
        
        /*--- End Detection ---*/
        
        /*----- Decode -----*/
        
        //Use Rings For Homography
        void UseRingsForHomography(JMD::ChromaTag_UseRings use_param);
        void UseRingsForHomography(JMD::ChromaTag_UseRings *use_param, std::string *use_str_param);
        
        //Max Search Hamming Distance
        void MaxSearchHammingDistance(unsigned int distance_param);
        void MaxSearchHammingDistance(unsigned int *distance_param);
        
        /*--- End Decode ---*/
        
        /*----- Pose -----*/
        
        //Use Rings For Pose
        void UseRingsForPose(JMD::ChromaTag_UseRings use_param);
        void UseRingsForPose(JMD::ChromaTag_UseRings *use_param, std::string *use_str_param);
        
        //Tag Size
        void TagSize(double size_param);
        void TagSize(double *size_param);
        
        //FX
        void FX(double fx_param);
        void FX(double *fx_param);
        
        //FY
        void FY(double fy_param);
        void FY(double *fy_param);
        
        //CX
        void CX(double cx_param);
        void CX(double *cx_param);
        
        //CY
        void CY(double cy_param);
        void CY(double *cy_param);
        
        //Intrinsics
        void Intrinsics(double fx_param, double fy_param, double cx_param, double cy_param);
        void Intrinsics(double *fx_param, double *fy_param, double *cx_param, double *cy_param);
        
        //K1
        void K1(double k1_param);
        void K1(double *k1_param);
        
        //K2
        void K2(double k2_param);
        void K2(double *k2_param);
        
        //K3
        void K3(double k3_param);
        void K3(double *k3_param);
        
        //P1
        void P1(double p1_param);
        void P1(double *p1_param);
        
        //P2
        void P2(double p2_param);
        void P2(double *p2_param);
        
        //Distortion Coefficients
        void DistortionCoefficients(double k1_param, double k2_param, double k3_param, double p1_param, double p2_param);
        void DistortionCoefficients(double *k1_param, double *k2_param, double *k3_param, double *p1_param, double *p2_param);
        
        /*--- End Pose ---*/
        
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
        /*------------- End Operator Overloads -------------*/
        
        
        
        /*--------------- DEBUG Methods ---------------*/
        /*------------- End DEBUG Methods -------------*/
        
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

