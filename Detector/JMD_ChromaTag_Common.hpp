#pragma once
#ifndef JMD_CHROMATAG_COMMON
#define JMD_CHROMATAG_COMMON

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Common: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Common.hpp
 A bunch of small classes that are used throughout the chroma tags
 pipeline
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

//utilities
#include "JMD_Utils_Defines.hpp"
#include "JMD_Vision_Image.hpp"
#include "JMD_Vision_Process.hpp"
#include "JMD_Vision_Point.hpp"
#include "JMD_Vision_Line.hpp"
#include "JMD_Math_Common.hpp"

//opencv
#ifdef OPENCV_FOUND
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d.hpp>
#endif

//macros
#define HASH_RGB(R, G, B) B + (G << 8) + (R << 16) 

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







namespace JMD 
{
	
    /*--------------- Enumerations ---------------*/

    /*----- ChromaTag Family -----*/
    // An enumeration used to define the characteristics of the tag
    // being used
    //
    // RGBW_16H5 - 4 x 4 grid with colors from center to border Red, Green, Black, White
    // RGBW_25H9 - 5 x 5 grid with colors from center to border Red, Green, Black, White
    // RGBW_36H11 - 6 x 6 grid with colors from center to border Red, Green, Black, White
    // RGRBW_36H11 - 6 x 6 grid with colors from center to border Red, Green, Red, Black, White
    typedef enum ChromaTag_Family { RGBW_16H5 = 0, RGBW_25H9, RGBW_36H11, RGRBW_36H11 } ChromaTag_Family;
    /*--- End ChromaTag Family ---*/
    
    /*----- ChromaTag Ring Color -----*/
    // An enumeration used to define the color of a ring
    //
    // RING_COLOR_REDGREEN - red ring inside a green ring
    // RING_COLOR_GREENRED - green ring inside a red ring
    // RING_COLOR_REDBLACK - red ring inside a black ring
    // RING_COLOR_GREENBLACK - green ring inside a black ring
    // RING_COLOR_BLACKWHITE - black ring inside a white ring
    typedef enum ChromaTag_RingColor { RING_COLOR_REDGREEN = 1, RING_COLOR_GREENRED, RING_COLOR_GREENBLACK, RING_COLOR_REDBLACK, RING_COLOR_BLACKWHITE } ChromaTag_RingColor;
    /*--- End ChromaTag Ring Color ---*/
    
    /*----- ChromaTag Input Color Space -----*/
    // An enumeration used to define the color space of the input
    // images
    //
    // RGB - Red, Green, Blue; RGB will be converted before detection which takes time
    // BGR - Blue, Green, Red; BGR will be converted before detection which takes time
    // YUV - Can be used as is for detection, but will take time to convert to LAB
    typedef enum ChromaTag_InputColorSpace { InputCS_RGB = 0, InputCS_BGR, InputCS_YUV } ChromaTag_InputColorSpace;
    /*--- End ChromaTag Input Color Space ---*/

    /*----- ChromaTag Detection Color Space -----*/
    // An enumeration used to define the color space of that is used
    // for detection and decoding
    //
    // YUV - quicker conversion than LAB, but gives worse results in general
    // LAB - slower conversion than YUV, but gives better results in general
    typedef enum ChromaTag_DetectionColorSpace { DetectionCS_YUV = 0, DetectionCS_LAB } ChromaTag_DetectionColorSpace;
    /*--- End ChromaTag Detection Color Space ---*/
    
    /*----- ChromaTag Use Rings -----*/
    // An enumeration used to define which rings are used when solving for
    // certain quantities such as the pose or homography
    //
    // USE_RINGS_OUTER - specifies to only use the outer-mose ring
    // USE_RING_ALL - specifices to use all rings
    typedef enum ChromaTag_UseRings { USE_RINGS_OUTER = 0, USE_RINGS_ALL } ChromaTag_UseRings;
    /*--- End ChromaTag Use Rings ---*/

    /*------------- End Enumerations -------------*/

	
	
	
	
    //====================================================================//
    //====================================================================//
    //================ JMD ChromaTag Settings Definition =================//
    //====================================================================//
    //====================================================================//
    // Struct to hold settings for the ChromaTag class. This struct can be
    // passed to the ChromaTag class to update the settings.
    struct JMD_ChromaTag_Settings
    {
        /*---------- General ----------*/
        
        //myVerboseLevel
        // defines how much output the program sends to the terminal
        // <0 - quiet, nothing will be output to the terminal
        //  0 - only minimal output is provided
        //  1 - output about each step is provided
        //  2 - output about each substep is provided
        // >2 - all output is provided (DEBUGGING)
        int                                  myVerboseLevel;
        
        //myDebugLevel
        // defines how much info is output to text file
        // calculated as a bit switch for
        // (1) FrameRate - rate each frame is processed
        // (2) NumDetections - number of detections 
        // (3) Detections - the four corners for each ring of each detection
        // (4) Homography - nine numbers for homography
        // (5) TagCode - number of tag
        // (6) Pose - R11 R12 R13 R21 R22 R23 R31 R32 R33 Tx Ty Tz
        //
        // e.g. if you want to write to file the following:
        // FrameRate and TagCode => 100010 = 34
        // Homography and Pose => 000101 = 5
        // etc...
        int                                  myDebugLevel;
        
        //myFamily
        // defines the Tag Family for detection. Options are:
        // RGBW_16H5 - 4x4 grid with colors from center to border Red, Green, Black, White
        // RGBW_25H9 - 5x5 grid with colors from center to border Red, Green, Black, White
        // RGBW_36H11 - 6x6 grid with colors from center to border Red, Green, Black, White
        // RGRBW_36H11 - 6x6 grid with colors from center to border Red, Green, Red, Black, White
        JMD::ChromaTag_Family                myTagFamily;
        
        //myInputColorSpace
        // defines the color space of the input images. Options are:
        // RGB - Red Green Blue
        // BGR - Blue Green Red
        // YUV (YCrCb) - Y -> intensity, UV - color
        // default = RGB
        JMD::ChromaTag_InputColorSpace       myInputColorSpace;
        
        /*-------- End General --------*/
        
		
        /*---------- Detection ----------*/
        
        //myDetectionColorSpace
        // defines the color space used for detection. Options are:
        // L*A*B - slower to convert but tends to give better results
        // YUV (YCrCb) - May not require conversion if image is given in YUV format, tends to give worse results
        // default = LAB
        JMD::ChromaTag_DetectionColorSpace   myDetectionColorSpace;
        
        //mySubsample
        // defines the amount of subsampling that is done when looking for initial
        // red pixels in the image. The larger this number, the less pixels that
        // will be scanned and the faster things will run; however, small tags may
        // be missed if they are smaller than the subsample size. 
        // e.g. mySubsample = 2 means that every other pixel is sampled
        // e.g. mySubsample = 10 means that every 10th pixel is sampled
        // default = 10
        unsigned int                         mySubsample;
        
        //myBorder
        // defines the border around the images that is skipped during the original
        // scan for red pixels. E.g. 10 means that a 10 pixels border around the
        // edge of the image will not be scanned for red pixels
        // default = 20
        unsigned int                         myBorder;
        
        //myHitThreshold
        // defines how many pixels within a color region must be hit before
        // success
        // default = 5
        unsigned int                         myHitThreshold;
        
        //myInitialScanNum
        // defines how many initial scan directions should be used, the directions
        // will then be an even split of 360 degrees; i.e. 360 / 4 = 90 degrees
        // which will result in scans up, right, down, and left
        unsigned int                         myInitialScanNum;
        
        //myMaxInitialBorderIterations
        // defines how many iterations will be done to push the initial border
        // to the red-green edge
        // default = 14
        unsigned int                         myMaxInitialBorderIterations;
        
        //myMaxDetections
        // defines the maximum number of detections, this is simply here
        // because we preallocate all the detections and popuate the detections
        // as they are found
        // default = 50
        unsigned int                         myMaxDetections;
        
        //myAreaRatioThreshold
        // defines the quit condition for how the initial border is pushed to
        // the black-white edge. Specifically, the algorithm keeps traack of the
        // current polgyon area and the potential max area and if the ratio is
        // above the threshold, we quit
        // default = 0.95
        double                               myAreaRatioThreshold;
        
        //myLABRedDiffThresh
        // defines the difference threshold between two neighboring pixels to
        // consider a pixel as red, which starts the detection process.
        // default = 25
        double                               myLABRedDiffThresh;
        
        //myRedGreenBorderThresh
        // threshold to decide if a scan crossed the red green border
        // default = 20
        double                               myRedGreenBorderThresh;
        
        //myGreenBlackBorderThresh
        // threshold to decide if a scan crossed the green black border
        // default = 7
        double                               myGreenBlackBorderThresh;
        
        //myBlackWhiteBorderThresh
        // threshold to decide if a scan crossed the black white border
        // default = 7
        double                               myBlackWhiteBorderThresh;

        //myMaxExpandDistanceMultiplier
        // The max expand distance is the furthest we have scanned to find a point so
        // far. New points can only be so far from the max scan. This is a way of
        // rejecting bad polygons early on, though this case is rare
        double                              myMaxExpandDistMultiplier;
        
        //myMaxBorderRefineIterations
        // max iterations for kmeans when refining the border
        unsigned int                        myMaxBorderRefineIterations;
        
        //myRefineCornersLevel
        // defines how much refinement to do for corners after
        // border has been refined down to four edges
        //  0 - no refinement
        // >1 - refinement occurs
        unsigned int                        myRefineCornersLevel;
        
        /*-------- End Detection --------*/
        
		
        /*---------- Decode ----------*/
        
        //myUseRingsForHomography
        // defines which rings to use when solving for homography
        // USE_RINGS_OUTER - only uses outer-most ring
        // USE_RINGS_ALL - uses all rings
        JMD::ChromaTag_UseRings            myUseRingsForHomography;
        
        //myMaxSearchHammingDistance
        // defines how far away from the original code to go when
        // precomputing possible tags that are near the true tag.
        // More specifically, for a given tag, all tags that are
        // within max hamming distance of that tag will be saved
        // and searched for as a match to the true tag
        unsigned int                       myMaxSearchHammingDistance;
        
        /*-------- End Decode --------*/
        
		
        /*---------- Pose ----------*/
        
        //myUseRingsForHomography
        // defines which rings to use when solving for pose with PnP
        // USE_RINGS_OUTER - only uses outer-most ring
        // USE_RINGS_ALL - uses all rings
        JMD::ChromaTag_UseRings            myUseRingsForPose;
        
        //myTagSize
        // size of tag in some metric system; e.g. 10 cm. If not
        // specified, a size of 1.0 is used.
        double                             myTagSize;
        
        //myFX, myFY
        // focal length in x and y direction for intrinsic matrix 
        // of the camera [fx 0 cx, 0 fy cy, 0 0 1]
        double                             myFX;
        double                             myFY;
        
        //myCX, myCY
        // principal point in x and y direction for intrinsic matrix 
        // of the camera [fx 0 cx, 0 fy cy, 0 0 1]
        double                             myCX;
        double                             myCY;
        
        //myK1, myK2, myK3, myP1, myP2
        // distortion coefficients
        double                             myK1;
        double                             myK2;
        double                             myK3;
        double                             myP1;
        double                             myP2;
        
        /*-------- End Pose --------*/
		
        
        /*---------- Viewer ----------*/
        
        //myWindowWidth, myWindowHeight
        // defines the size of the GLUT window for the viewer
        int                                myWindowWidth;
        int                                myWindowHeight;
        
        //myWindowX, myWindowY
        // defines the x and y location of the GLUT window for
        // the viewer
        int                                myWindowX;
        int                                myWindowY;
        
        //myZoomSpeed
        // defines the size of increments of zoom when the mouse
        // scroll wheel is spun
        double                             myZoomSpeed;
        
        //myMaxPoseHistory
        // maximum number of poses to render as time increments
        unsigned int                       myMaxPoseHistory;
        
        /*-------- End Viewer --------*/
        
        
	    //set defaults
	    JMD_ChromaTag_Settings() : 
	        myVerboseLevel(              JMD::DEF_DUMMY_INT),
            myDebugLevel(                JMD::DEF_DUMMY_INT),
            myTagFamily(                (JMD::ChromaTag_Family)JMD::DEF_DUMMY_INT),
            myInputColorSpace(          (JMD::ChromaTag_InputColorSpace)JMD::DEF_DUMMY_INT),
            myDetectionColorSpace(      (JMD::ChromaTag_DetectionColorSpace)JMD::DEF_DUMMY_INT),
	        mySubsample(                 JMD::DEF_DUMMY_UINT),
            myBorder(                    JMD::DEF_DUMMY_UINT),
            myHitThreshold(              JMD::DEF_DUMMY_UINT),
            myInitialScanNum(            JMD::DEF_DUMMY_UINT),
            myMaxInitialBorderIterations(JMD::DEF_DUMMY_UINT),
            myMaxDetections(             JMD::DEF_DUMMY_UINT),
            myAreaRatioThreshold(        JMD::DEF_DUMMY_DBL),
            myLABRedDiffThresh(          JMD::DEF_DUMMY_DBL),
            myRedGreenBorderThresh(      JMD::DEF_DUMMY_DBL),
            myGreenBlackBorderThresh(    JMD::DEF_DUMMY_DBL),
            myBlackWhiteBorderThresh(    JMD::DEF_DUMMY_DBL),
            myMaxExpandDistMultiplier(   JMD::DEF_DUMMY_DBL),
            myMaxBorderRefineIterations( JMD::DEF_DUMMY_UINT),
            myRefineCornersLevel(        JMD::DEF_DUMMY_UINT),
            myUseRingsForHomography(    (JMD::ChromaTag_UseRings)JMD::DEF_DUMMY_INT),
            myMaxSearchHammingDistance(  JMD::DEF_DUMMY_UINT),
            myUseRingsForPose(          (JMD::ChromaTag_UseRings)JMD::DEF_DUMMY_INT),
            myTagSize(                   JMD::DEF_DUMMY_DBL),
            myFX(                        JMD::DEF_DUMMY_DBL),
            myFY(                        JMD::DEF_DUMMY_DBL),
            myCX(                        JMD::DEF_DUMMY_DBL),
            myCY(                        JMD::DEF_DUMMY_DBL),
            myK1(                        JMD::DEF_DUMMY_DBL),
            myK2(                        JMD::DEF_DUMMY_DBL),
            myK3(                        JMD::DEF_DUMMY_DBL),
            myP1(                        JMD::DEF_DUMMY_DBL),
            myP2(                        JMD::DEF_DUMMY_DBL),
            myWindowWidth(               JMD::DEF_DUMMY_INT),
            myWindowHeight(              JMD::DEF_DUMMY_INT),
            myWindowX(                   JMD::DEF_DUMMY_INT),
            myWindowY(                   JMD::DEF_DUMMY_INT),
            myZoomSpeed(                 JMD::DEF_DUMMY_DBL),
            myMaxPoseHistory(            JMD::DEF_DUMMY_UINT)
	        { }
            
    };
    //====================================================================//
    //====================================================================//
    //================ JMD ChromaTag Settings Definition =================//
    //====================================================================//
    //====================================================================//
	
	
	
    
    
    //====================================================================//
    //====================================================================//
    //================ JMD ChromaTag Detection Definition ================//
    //====================================================================//
    //====================================================================//
    
    
    //==========================================================//
    //==================== TagBorderSegment ====================//
    //==========================================================//
    // Class to hold Border Segments that make up the Tag. These segments
    // are defined by two points, and also have length and midpoint values
    //
    struct TagBorderSegment
    {
        // Color of ring of which this segment is a part
        ChromaTag_RingColor       myRingColor;
        
        // Line that is made of two points. A starting and ending point
        // Each point on the line has a W() value that is used to store
        // A color reference for the next ring; e.g. if we have a red
        // ring inside a green ring, the W() value stores a reference to
        // green for when we start scanning in green. myLP1_EdgeColorRef
        // and myLP2_EdgeColorRef store the values of the edge color
	    JMD_Vision_LineSegment2D  myLine;
        double                    myLP1_EdgeColorRef;
        double                    myLP2_EdgeColorRef;
	
	    // midpoint of the line
	    JMD_Vision_Point2D        myMidpoint;
        
        // triangle area apex point
        JMD_Vision_Point2D        myTriangleApex;
		
        // slope of the line (x,y)
        JMD_Vision_Point2D        mySlope;
        
        // initial scan direction
        JMD_Vision_Point2D        myScanDir;
        
	    // length of the line
	    double                    myLength;
        
        // area of triangle formed using tag segment as the base and the
        // intersection of neighboring segments as the apex
        double                    myArea;
        bool                      isZeroed;
    };
    //==========================================================//
    //==================== TagBorderSegment ====================//
    //==========================================================//
    
    
    //=======================================================//
    //==================== TagBorderRing ====================//
    //=======================================================//
    // typedef for a border ring. Each section of the tag is
    // one ring that is made up of border segments
    typedef std::vector<TagBorderSegment *> TagBorderRing;
    //=======================================================//
    //==================== TagBorderRing ====================//
    //=======================================================//
    
    
    //=======================================================//
    //==================== TagBorderSet =====================//
    //=======================================================//
    // typedef for a vector of TagBorderRings. Each tag is made
    // of a TagBorderSet
    typedef std::vector<TagBorderRing *> TagBorderSet;
    //=======================================================//
    //==================== TagBorderSet =====================//
    //=======================================================//
    
    
    //==================================================//
    //==================== TagCode =====================//
    //==================================================//
    // class to hold a perturbed code and the original
    // code, along with any other data related to the
    // code that we might need
    struct TagCode
    {
        //the perturbed code
        unsigned long long myCode;
        
        //hamming distance (should never be very large)
        unsigned int       myHammingDistance;
        
        //the Hamming Dist = 0 code that myCode is mapped
        //back to in the event that we find a match
        unsigned long long myH0Code;
    };
    //==================================================//
    //==================== TagCode =====================//
    //==================================================//
    
    
    //=================================================================//
    //==================== JMD_ChromaTag_Detection ====================//
    //=================================================================//
    // Struct to hold the tag detections
    struct JMD_ChromaTag_Detection
    {
        /*---------- Detection ----------*/
        TagBorderSet TagBorders;
        /*-------- End Detection --------*/
        
        /*---------- Decode ----------*/
        unsigned long long TagCode;
        unsigned int       TagBits;
        bool               IsCode;
        /*-------- End Decode --------*/
        
        /*---------- Pose ----------*/
        double TagRotation[4];
        double TagTranslation[3];
        bool   IsPose;
        /*-------- End Pose --------*/
        
        //true if active
        bool IsActive;
	};
    //=================================================================//
    //==================== JMD_ChromaTag_Detection ====================//
    //=================================================================//
    
    
    //==================================================================//
    //==================== JMD_ChromaTag_Collection ====================//
    //==================================================================//
    // typedef for a collection of ChromaTag Detections
    typedef std::vector<JMD_ChromaTag_Detection *> JMD_ChromaTag_Collection;
    //==================================================================//
    //==================== JMD_ChromaTag_Collection ====================//
    //==================================================================//
    
    
    //====================================================================//
    //====================================================================//
    //================ JMD ChromaTag Detection Definition ================//
    //====================================================================//
    //====================================================================//
	
}//end namespace JMD

#endif
