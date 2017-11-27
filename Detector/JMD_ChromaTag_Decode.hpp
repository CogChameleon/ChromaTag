#pragma once
#ifndef JMD_CHROMATAG_DECODE_H_
#define JMD_CHROMATAG_DECODE_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Decode: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Decode.hpp
 A class for ChromaTag Decoding
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
#include <bitset>
#include <unordered_map>
#include <fstream>

//ChromaTag
#include "JMD_ChromaTag_Common.hpp"

//OpenCV
#include <opencv2/calib3d/calib3d.hpp>

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
    //================= JMD ChromaTag Decode Definition ==================//
    //====================================================================//
    //====================================================================//
    class JMD_ChromaTag_Decode
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        /*------------- End Enumerations -------------*/
        
        /*--------------- Variables ---------------*/
        /*------------- End Variables -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
		
        //ChromaTag
        JMD_ChromaTag_Collection *myDetections;
        JMD_ChromaTag_Settings   *mySettings;

        //Color Conversion
        std::vector<double>      *myLAB_LTable;
        std::vector<double>      *myLAB_ATable;
        std::vector<double>      *myLAB_BTable;
        
        //Code Hash Table
        std::unordered_map<unsigned long long, TagCode> myTagCodes;
        
        //Decode
        std::vector<cv::Point2f>         myImagePoints;
        std::vector<cv::Point2f>         myImageGrid;
        unsigned int                     myNumRingsToUse;
        unsigned int                     myNumHorizontalBits;
		
        /*------------- End Variables -------------*/
        

        
        /*--------------- Setup Methods ---------------*/
        // These methods are used to setup the decoding. In
        // particular, handling settings, building the grid,
        // and building the tag family (also in Tag Family 
        // Methods)

        /*----- Private Init -----*/
        // does all the initialization of private instance variables
        //
        // input:
        //    settings_param - JMD_ChromaTag_Settings object
        void Private_Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param = nullptr);
        /*--- End Private Init ---*/

        /*----- Private Build Grid -----*/
        // builds a grid between [-1,1] x [-1,1] sampling based
        // on the number of bits in the tag family
        //
        // input:
        //    grid - reference to a vector to populate with grid locations
        void Private_BuildGrid(std::vector<cv::Point2f> &grid_param);
        /*--- End Private Build Grid ---*/
        
        /*------------- End Setup Methods -------------*/
        

        
        /*--------------- Tag Family Methods ---------------*/
        // These methods are used to build the codes of a given
        // family. These codes are created with hamming distance
        // perturbations and rotations versions and stored in a
        // hash table

        /*----- Private Build Tag Family -----*/
        // sets up all the tags for the current family
        //
        void Private_BuildTagFamily();
        /*--- End Private Build Tag Family ---*/
        
        /*----- Private Family 16H5 -----*/
        // sets up all the 16H5 tags. Uses the codes
        // that AprilTags generated
        //
        // input:
        //    codes_param - reference to vector to fill with tag codes
        void Private_Family16H5(std::vector<unsigned long long> &codes_param);
        /*--- End Private Family 16H5 ---*/
        
        /*----- Private Family 25H9 -----*/
        // sets up all the 16H5 tags. Uses the codes
        // that AprilTags generated
        //
        // input:
        //    codes_param - reference to vector to fill with tag codes
        void Private_Family25H9(std::vector<unsigned long long> &codes_param);
        /*--- End Private Family 25H9 ---*/
        
        /*----- Private Family 36H11 -----*/
        // sets up all the 16H5 tags. Uses the codes
        // that AprilTags generated
        //
        // input:
        //    codes_param - reference to vector to fill with tag codes
        void Private_Family36H11(std::vector<unsigned long long> &codes_param);
        /*--- End Private Family 36H11 ---*/
        
        /*----- Private Rotate Code CW -----*/
        // rotates the tag code by pi/2 clockwise
        //
        // input:
        //    code_param - code to rotate
        // output:
        //    rotated code - code after rotation
        unsigned long long Private_RotateCodeCW(unsigned long long code_param);
        /*--- End Private RotateCode ---*/

        /*----- Private Add Tag Code -----*/
        // adds a code to the list
        //
        // input:
        //    h0_code_param - hamming distance 0 code
        //    code_param - the perturbed code
        //    hamming_param - how much the code was perturbed from h0
        void Private_AddTagCode(unsigned long long h0_code_param, unsigned long long code_param, unsigned int hamming_param);
        /*--- End Private Add Tag Code ---*/

        /*------------- End Tag Family Methods -------------*/



        /*--------------- Decode Methods ---------------*/
        // These methods are for decoding the tag detections.
        // A homography is fit to the detection to fit a grid
        // to the quadrilateral and pixel values are read to
        // decode the tag. Then the code is matched to a code
        // in the family using the previously built hash table.
        
        /*----- Private Decode -----*/
        // entry point for the decoding process, almost everything
        // happens here
        //
        // input:
        //    image_param - the image to process
        void Private_Decode( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND
            void Private_Decode( cv::Mat &image_param );
        #endif
        /*--- End Private Decode ---*/

        /*----- Private Find Tag Code -----*/
        // looks for a code in the code hash table
        //
        // input:
        //    code_param - code to look for
        //    tag_code_param - populated with ptr to TagCode if found
        //    true_code_param - populated with the true code value
        // output:
        //    bool - true if found, false otherwise
        bool Private_FindTagCode(unsigned long long code_param, TagCode **tag_code_param);
        /*--- End Private Find Tag Code ---*/

        /*----- Private Homography -----*/
        // calculates the homography given the camera plane and corner
        // points on the tag
        //
        // input:
        //    detection_param - detection to use
        //    H_param - reference to cv::Mat to populate with homography
        // output:
        //    bool - true if success, false otherwise
        void Private_Homography();
        #ifdef OPENCV_FOUND
            bool Private_Homography(std::vector<cv::Point2f> &tag_points_param, cv::Mat &H_param);
        #endif
        /*--- End Private Homography ---*/

        /*------------- End Decode Methods -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
		/*--------------- Constructors ---------------*/
        // Constructors
        //
        // input:
        //    collection_param - pointer to vector of detections to decode
        JMD_ChromaTag_Decode(JMD_ChromaTag_Collection *collection_param);
		/*------------- end Constructors -------------*/
		
        
		
		/*--------------- Destructors ---------------*/
		~JMD_ChromaTag_Decode();
		/*-------------- End Destructors ------------*/
        
        
        
        /*--------------- Setup ---------------*/

        /*----- Init -----*/
        // allows the user to specify parameters for ChromaTag
        //
        // input:
        //    settings_param - JMD_Settings_ChromaTag struct with settings
        void Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param);
        /*--- End Init ---*/
        
        /*------------- End Setup -------------*/



        /*--------------- Tag Decoding Methods ---------------*/
        
        /*----- Decode -----*/
        // decodes the detected tags
        //
        // input:
        //    
        void Decode( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND 
            void Decode( cv::Mat &image_param );
        #endif
        /*--- End Decode ---*/
        
        /*------------- End Tag Decoding Methods -------------*/
        
		
		
        /*--------------- Utility Methods ---------------*/
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
        /*------------- End Operator Overloads -------------*/
        
        
        
        /*--------------- DEBUG Methods ---------------*/
        
        /*----- DEBUG ShowLAB -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowLAB(cv::Mat &image_param);
        #endif
        /*--- End DEBUG ShowLAB ---*/
        
        /*----- DEBUG ShowGrid -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowGrid();
        #endif
        /*--- End DEBUG ShowGrid ---*/
        
        /*----- DENUG ShowGridSamples -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowGridSamples(cv::Mat &image_param, std::vector<cv::Point2f> &samples_param);
        #endif
        /*--- End DEBUG ShowGridSamples ---*/
        
        /*----- DEBUG ShowCodes -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowCodes(cv::Mat &image_param);
        #endif
        /*--- End DEBUG ShowCodes ---*/
        
        /*----- DEBUG OutputCodes -----*/
        void DEBUG_OutputCodes();
        /*--- End DEBUG OutputCodes ---*/

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