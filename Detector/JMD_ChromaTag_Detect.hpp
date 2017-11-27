#pragma once
#ifndef JMD_CHROMATAG_DETECT_H_
#define JMD_CHROMATAG_DETECT_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Detect: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Detect.hpp
 A class for ChromaTag Detection
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
#include <list>
#include <queue>
#include <bitset>
#include <map>
#include <unordered_map>
#include <numeric>
#include <iterator>
#include <fstream>

//ChromaTag
#include "JMD_ChromaTag_Common.hpp"

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
    //================= JMD ChromaTag Detect Definition ==================//
    //====================================================================//
    //====================================================================//
    class JMD_ChromaTag_Detect
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
		
		//Utilities
        JMD_Vision_Process *myVisionProcess;
        
        //ChromaTag
        JMD_ChromaTag_Collection *myDetections;
        JMD_ChromaTag_Settings   *mySettings;
		
        //Color Conversion
        std::vector<double>      *myLAB_LTable;
        std::vector<double>      *myLAB_ATable;
        std::vector<double>      *myLAB_BTable;

        //Detection
        double                           myMaxExpandDistance;
        JMD_Vision_Point2D               myCenterPoint;
        TagBorderSet*                    myBorders;
        std::vector<JMD_Vision_Point2D>  myExpandDirTable;
        std::string                      myFamilyStr;
        
        /*------------- End Variables -------------*/
        
        

        /*--------------- Setup Methods ---------------*/
        // These methods are used to setup the detection. In 
        // particular, defining settings and starting the detection
        // algorithm on an input image

        /*----- Private Init -----*/
        // does all the initialization of private instance variables
        //
        // input:
        //    settings_param - JMD_ChromaTag_Settings object
        void Private_Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param = nullptr);
        /*--- End Private Init ---*/
        
        /*----- Private Detect -----*/
        // where all the detection is actually done, the convenience methods
        // for input and detection colors are to have one less set of if
        // statements when calling the Detect function
        //
        // input:
        //    image_param - the image to process
        void Private_Detect( JMD_Vision_Image &image_param );
        void Private_Detect_RGBToLAB( JMD_Vision_Image &image_param );
        void Private_Detect_BGRToLAB( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND 
            void Private_Detect( cv::Mat &image_param );
            void Private_Detect_RGBToLAB( cv::Mat &image_param );
            void Private_Detect_BGRToLAB( cv::Mat &image_param );
        #endif
        /*--- End Private Detect ---*/

        /*------------- End Setup Methods -------------*/



        /*--------------- Scanning Methods ---------------*/
        // These methods handle scanning through the image
        // to find red, green, black, and white borders of
        // ChromaTag

        /*----- Private Expand -----*/
        // scans out in num_param directions from a given (u,v) location
        // in a given image_param
        //
        // input:
        //    image_param - image to expand on
        //    u_param - column location in the image to begin expand
        //    v_param - row location in the image to begin expand
        //    red_ref_param - reference red from initial scan point
        //    points_param - vector of points
        //    (outer_)ring_param - BorderRing to populate
        //    inner_ring_param - BorderRing to use as a color reference
        //    center_point_param - estimated center point of tag
        bool Private_Expand( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param );
        #ifdef OPENCV_FOUND
            bool Private_Expand( cv::Mat &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param );
            bool Private_Expand_RedGreen( cv::Mat &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param, TagBorderRing *ring_param, JMD_Vision_Point2D *center_point_point = nullptr );
            bool Private_Expand_GreenBlack( cv::Mat &image_param, std::vector<TagBorderRing*> *inner_rings_param, TagBorderRing *outer_ring_param );
            bool Private_Expand_BlackWhite( cv::Mat &image_param, std::vector<TagBorderRing*> *inner_rings_param, TagBorderRing *outer_ring_param );
        #endif
        /*--- End Private Expand ---*/
        
        /*----- Private Skip Scanned Region -----*/
        // skips regions that have alrady been scanned for a tag
        //
        // input:
        //    i - current row value
        //    j - pointer to current column of image
        // output:
        //    bool - true if skipping, false if not skipping
        bool Private_SkipScannedRegion(int i, int *j);
        /*--- End Private Skip Scanned Region ---*/

        /*----- Private Scan Direction -----*/
        // scans image in a given direction looking for transition between sections
        //
        // input:
        //    image_param - image to scan over
        //    red_ref_param - reference to red value
        //    u_param - starting u location in pixel coordinates
        //    v_param - starting v location in pixel coordinates
        //    u_dir_param - vector u direction of scan
        //    v_dir_param - vector v direction of scan
        // output:
        //    edge_point_param - vector of points where scan hit green edge
        unsigned int Private_Scan_Direction_RedGreen( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param = nullptr );
        unsigned int Private_Scan_Direction_BlackWhite( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param = nullptr );
        #ifdef OPENCV_FOUND
            unsigned int Private_Scan_Direction_RedGreen( cv::Mat &image_param, double red_ref_param, double u_param, double v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param = nullptr, double *edgecolor_param = nullptr );
            unsigned int Private_Scan_Direction_GreenBlack( cv::Mat &image_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, JMD_Vision_Point2D *scan_dir_param, JMD_Vision_Point2D *edge_point_param = nullptr, double *edgecolor_param = nullptr);
            unsigned int Private_Scan_Direction_BlackWhite( cv::Mat &image_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, JMD_Vision_Point2D *scan_dir_param, JMD_Vision_Point2D *edge_point_param = nullptr, double *edgecolor_param = nullptr);
        #endif
        /*--- End Private Scan Direction ---*/

        /*----- Private Range -----*/
        // calculates how many pixels exist between a given location
        // defined by u_param and v_param and the edge of an image of
        // size rows_param x cols_param in a direction defined by 
        // u_dir_param and v_dir_param. If u and v limits given,
        // range will also consider these, but will still never
        // exceed the size of the image; i.e. the u and v limits
        // can shrink range, but never increase it
        //
        // input:
        //    rows_param - number of rows in an image
        //    cols_param - number of cols in an image
        //    u_param - starting u location
        //    v_param - starting v location
        //    u_dir_param - direction vector in u
        //    v_dir_param - direction vector in v
        //    u_limit_param - optional, specifies a limit on u that isn't image size
        //    v_limit_param - optional, specifies a limot on v that isn't image size
        // output:
        //    u_range_param - max number of pixels to scan in u direction
        //    v_range_param - max number of pixels to scan in v direction
        void Private_Range(int rows_param, int cols_param, double u_param, double v_param, double u_dir_param, double v_dir_param, double *u_range_param, double *v_range_param);
        /*--- End Private Range ---*/

        /*------------- End Scanning Methods -------------*/


        /*--------------- Find Border Methods ---------------*/
        // These methods handle building the border of a detection
        // by starting with an initial polygon of four edges and
        // adding new edges until convergence

        /*----- Private Initial Border -----*/
        // finds the initial border and tag center
        //
        // input:
        //    image_param - image
        //    points_param - initial set of scan points
        // output:
        //    border_param - initial border line segments
        //    center_param - initial center point
        void Private_Initial_Border( JMD_Vision_Image &image_param, JMD_Vision_Point2D *center_param );
        #ifdef OPENCV_FOUND
            void Private_Initial_Border( cv::Mat &image_param, JMD_Vision_Point2D *center_param );
        #endif
        /*--- End Private Initial Border ---*/

        /*----- Private Find Border -----*/
        // finds the tag border
        //
        // input:
        //    image_param - image to scan over
        // output:
        //    bool - true if successfully found border, false o/w
        bool Private_Find_Border( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND
            bool Private_Find_Border( cv::Mat &image_param );
        #endif
        /*--- End Private Find Border ---*/

        /*----- Private Insert Ring Segment -----*/
        // Inserts a segment into a border ring
        //
        // input:
        //    image_param - image
        //    ring_param - border ring pointer
        //    index_param - index to expanded edge
        //    point_param - point to add to ring
        //    edge_color_param - edge color reference
        // output:
        //    bool - true if success, false if failure
        bool Private_InsertRingSegment();
        #ifdef OPENCV_FOUND
            bool Private_InsertRingSegment(cv::Mat &image_param, TagBorderRing *ring_param, int index_param, JMD_Vision_Point2D *point_param, double edge_color);
        #endif
        /*--- End Private Insert Ring Segment ---*/
        
        /*----- Private Undo Ring Segment -----*/
        // Removes a segment from a border ring by
        // connecting the neighboring segments at their
        // current end points
        //
        // input:
        //    ring_param - border ring pointer
        //    index_param - index of segment to remove
        void Private_UndoRingSegment(TagBorderRing *ring_param, int index_param);
        //
        // Removes segments from current ring back to the
        // innermost ring
        //
        // input:
        //    iterator_param - iterator for TagBorderSet
        void Private_UndoRingSegments(TagBorderSet::iterator it, int index_param);
        /*--- End Private Undo Ring Segment ---*/

        /*----- Private Reset Borders -----*/
        // resets slopes on borders to begin processing a new image
        //
        void Private_ResetBorders();
        //
        // resets a detection back to inactive so that the memory
        // can be reused for another detection. If borders_param is
        // not given, it uses the current detection (myBorders)
        //
        // input:
        //    borders_param - detection to reset
        void Private_ResetBorder(TagBorderSet *borders_param = nullptr);
        //
        /*--- End Private Reset Borders ---*/
        
        /*----- Private Copy BorderRing -----*/
        // given a src and dest border ring, copies the points
        // from src into dest
        //
        // input:
        //    src_param - pointer to source Border Ring
        //    dest_param - point to destination Border Ring
        void Private_Copy_BorderRing(TagBorderRing *src_param, TagBorderRing *dest_param);
        /*--- End Private Copy BorderRing ---*/

        /*------------- End Find Border Methods -------------*/



        /*--------------- Refine Border Methods ---------------*/
        // These methods handle refining the border so that the
        // final detection has four edges and the four corners are
        // on the four corners of the tag in the image.

        /*----- Private Refine Border -----*/
        // this step takes a polygon of the border of a tag
        // and removes edges to get down to a quadrilateral
        //
        // input:
        //    image_param - image to scan over
        // output:
        //    bool - true if success, false otherwise
        void Private_Refine_Border( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND
            bool Private_Refine_Border( cv::Mat &image_param );
        #endif
        /*--- End Private Refine Border ---*/

        /*----- Private Refine Corners -----*/
        // this step takes a quadrilateral with four corners
        // and searches a small patch around each corner for
        // the sub-pixel level corner
        void Private_Refine_Corners( JMD_Vision_Image &image_param );
        #ifdef OPENCV_FOUND
            void Private_Refine_Corners( cv::Mat &image_param );
        #endif
        /*--- End Private Refine Corners ---*/

        /*----- Private Circular -----*/
        // calculates the circular distance between two angles. Assumes the
        // angles are in radians (angle_param - ref_param)
        //
        // input:
        //    angle_param - angle in radians
        //    ref_param - second angle in radians
        // output:
        //    double - difference of angle_param - ref_param
        double Private_CircularDistance(double angle_param, double ref_param);
        /*--- End Private Circular ---*/
        
        /*----- Private KMeans Angle -----*/
        // clusters angles using kmeans
        //
        // input:
        //    
        bool Private_KMeans_Angle(cv::Mat &image_param);
        bool Private_KMeans(cv::Mat &image_param, std::vector<double> &data, std::vector<double> &weights, std::vector<double> *means, std::vector<int> *clusters );
        /*--- End Private KMeans Angle ---*/

        /*------------- End Refine Border Methods -------------*/



        /*--------------- Polygon Geometry Methods ---------------*/
        // These methods are related to the geometry of the polygon
        // being built during the detection process.

        /*----- Private Edge Estimate -----*/
        // calculates an outer bound on the edge to estimate where
        // a scan should stop when looking for any ring outside the
        // the red center. Also, returns true given that the bound is
        // beyond the limits of the image and returns a best estimate
        // of where the edge would be in this case
        //
        // input:
        //    u_param - starting u location
        //    v_param - starting v location
        //    ref_pts_param - multiple points that are used to estimate the boundary
        //    im_rows_param - number of rows in an image
        //    im_cols_param - number of cols in an image
        //    u_range_param - ptr to u_range; edited and returned (max pixels to scan in dir)
        //    v_range_param - ptr to v_range; edited and returned (max pixels to scan in dir)
        //    u_edge_est_param - ptr to u edge estimate variable that will be returned
        //    v_edge_est_param - ptr to v edge estimate variable that will be returned
        // output:
        //    bool - false if estimate is beyond image region
        bool Private_Edge_Estimate(unsigned int rows_param, unsigned int cols_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, double u_param, double v_param, double *u_range_param, double *v_range_param, double *u_edge_est_param, double *v_edge_est_param);
        /*--- End Private Edge Estimate ---*/
        
        /*----- Private Find Max Area -----*/
        // finds the max area line of the outer ring
        // of the border
        //
        // input:
        //    area_param - double ptr populated with max area value
        // output:
        //    int - index of maximum area line
        int Private_FindMaxArea(double *area_param = nullptr);
        /*--- End Private Find Max Area ---*/

        /*----- Private FindPolygonArea -----*/
        // finds the area of a polygon defined by vertices. Also, gives
        // the possible expanded area given that the current vertices
        // all lie on the potential final border of the polygon
        //
        // input:
        //    ring_param - BorderRing
        //    polygon_param - area of polygon
        //    possible_param - possible area given current points are on boundary
        void Private_FindPolygonArea(TagBorderRing *ring_param, double *polygon_param, double *possible_param);
        /*--- End Private FindPolygonArea ---*/
        
        /*----- Private UpdatePolygonArea -----*/
        // finds the new area of a polygon given the addition of one new
        // point. This results in a new triangle of area that is added.
        // The height of the triangle is defined by the point at the
        // base and the point at the apex.
        // 
        // input:
        //    ring_param - BorderRing pointer
        //    index_param - index to active segment of ring
        //    apex_param - apex of new triangle section of border
        //    polygon_param - area of polygon
        //    possible_param - possible area given current points are on boundary
        //    was_pt_convex_param - false if new added point was not convex
        //    was_seg_inserted_param - false if new segment was not inserted
        //    was_seg_convex_param - vector of indices of segments that were forced to be convex
        void Private_UpdatePolygonArea(TagBorderRing *ring_param, int index_param, JMD_Vision_Point2D *apex_param, double *polygon_param, double *possible_param, bool was_pt_convex_param, bool was_seg_inserted_param, std::vector<int> &was_seg_convex_param);
        /*--- End Private UpdatePolygonArea ---*/
        
        /*----- Private Segment Area -----*/
        // calculates the triangular area formed with the segment as
        // the base of the triangle and the intersection of neighboring
        // segments as the apex
        //
        // input:
        //    base_param - base segment
        //    neighbor_left_param - segment left of base
        //    neighbor_right_param - segment right of base
        //    apex_param - point that is populated with triangle apex
        //    center_param - point at center of tag
        // output:
        //    double - area of triangle, JMD::DEF_PINF if infinite
        double Private_SegmentArea(TagBorderSegment *base_param, TagBorderSegment *neighbor_left_param, TagBorderSegment *neighbor_right_param, JMD_Vision_Point2D *apex_param);
        /*--- End Private Segment Area ---*/
        
        /*----- Private Is Colinear -----*/
        // given two neighbor segments, tests if they are
        // colinear by checking if their dot products = 1 or -1
        // (assumes normalized slopes)
        //
        // input:
        //    seg1_param - ptr to segment 1
        //    seg2_param - ptr to segment 2
        // output:
        //    bool - true if colinear, false otherwise
        bool Private_IsColinear(TagBorderSegment *seg1_param, TagBorderSegment *seg2_param);
        /*--- End Private Is Colinear ---*/
        
        /*----- Private Is Convex -----*/
        // tests if borders and/or a ring is convex, if no ring is
        // provided, then tests all border rings
        //
        // input:
        //    ring_param - vector of current border segments to test
        // output:
        //    bool - true if convex, false otherwise
        bool Private_IsConvex();
        bool Private_IsConvex(TagBorderRing *ring_param);
        //
        // tests is two cnosecutive segments are convex, the segments
        // are assumed to be flowing in clockwise order; e.g. the second
        // point of segment1 is the first point of segment2
        //
        // input:
        //    seg1_param - segment 1 where point2 is point1 of segment 2
        //    seg2_param - segment 2 where point1 is point2 of segment 1
        // output:
        //    bool - true is convex, false otherwise
        bool Private_IsConvex(JMD_Vision_LineSegment2D *seg1_param, JMD_Vision_LineSegment2D *seg2_param);
        /*--- End Private Is Convex ---*/
        
        /*----- Private Force Convex -----*/
        // given the ring and the index of the segment that was split
        // during scns, this will check for convexity moving clockwise
        // and counter-clockwise around the ring and will quit in each
        // direction once convexity is found for a segment pair
        // 
        // input:
        //    ring_param - ptr to ring to check convexity
        //    index_param - index of segmen that was split into two
        //    was_convex_segidx_param - vector of which segment had to be adjusted to make polygon convex
        void Private_ForceConvex(TagBorderRing *ring_param, int index_param, std::vector<int> &was_convex_segidx_param);
        //
        // given the ring and the index of the segment that was split
        // during scans, this will check for convexity and move points
        // on the edges to enforce convexity
        //
        // input:
        //    ring_param - ptr to ring to check convexity
        //    index_param - index of segment that was split into two
        //    was_seg2next_convex_param - ptr that is true if seg2 and next segment are convex; false otherwise
        //    was_prevSeg1_convex_param - ptr that is true if seg1 and prev segment are convex; false otherwise
        void Private_ForceConvex(TagBorderRing *ring_param, int index_param, bool *was_seg2next_convex_param = nullptr, bool *was_prevSeg1_convex_param = nullptr);
        //
        // given two concave segments, this function pushes the shared
        // point out to the threshold where the polygon becomes convex
        //
        // input:
        //    seg1_param - segment 1 
        //    seg2_param - segment 2
        void Private_ForceConvex(TagBorderSegment *seg1_param, TagBorderSegment *seg2_param);
        //
        // given a segment and a new point to be added to split that
        // segment, this function pushes the new point to the segment
        // to ensure convexity
        //
        // input:
        //    seg_param - segment
        //    point_param - point to move to be convex
        // output:
        //    bool - true if already convex, false if needed to make convex
        bool Private_ForceConvex(TagBorderSegment *seg_param, JMD_Vision_Point2D *point_param);
        /*--- End Private Force Convex ---*/
        
        /*----- Private In Slice -----*/
        // given a line, returns true if a query point is inside the "pizza slice"
        // created by the lines from the edges of the given line back to the center
        // point of the polygon
        //
        // input:
        //    TagBorderSegment *seg_param - segment that defines slice
        //    point_param - query point for whether it is in slice or not
        // output:
        //    bool - true means in slice, false means outside slice
        bool Private_InSlice(TagBorderSegment *seg_param, JMD_Vision_Point2D *point_param);
        /*--- End Private In Slice ---*/
        
        /*----- Private In Polygon Estimate -----*/
        // fits a rectangle around the polygon and return whether
        // a point is inside that rectangle
        //
        // input:
        //    i - row in image
        //    j - col in image
        //    ring_param - ring that defines the polygon using segments
        // output:
        //    bool - true if inside
        bool Private_InPolygonEstimate(int i, int *j, TagBorderRing *ring_param);
        /*--- End Private In Polygon Estimate ---*/

        /*------------- End Polygon Geometry Methods -------------*/
        
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
        //    collection_param - pointer to vector of detections to populate
        JMD_ChromaTag_Detect(JMD_ChromaTag_Collection *collection_param);
		/*------------- end Constructors -------------*/
		
        
		
		/*--------------- Destructors ---------------*/
		~JMD_ChromaTag_Detect();
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
        
        /*------------- End Tag Detection Methods -------------*/
        
		
		
        /*--------------- Utility Methods ---------------*/
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
        /*------------- End Operator Overloads -------------*/
        
        
        
        /*--------------- DEBUG Methods ---------------*/
        
        /*----- DEBUG_ShowDetections -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowDetections(cv::Mat &image_param);
            void DEBUG_ShowDetections(cv::Mat &image_param, unsigned int ring_param);
        #endif
        /*--- End DEBUG_ShowDetections ---*/
        
        /*----- DEBUG ShowLines -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowLines(cv::Mat &image_param);
            void DEBUG_ShowLines(cv::Mat &image_param, std::vector<double> &means_param, std::vector<JMD_Vision_Point2D*> &points_param);
            void DEBUG_ShowLines(cv::Mat &image_param, std::vector<double> &means_param, std::vector<JMD_Vision_Point2D> &points_param);
            void DEBUG_ShowLines(cv::Mat &image_param, std::list  <JMD_Vision_LineSegment2D> *border_param);
            void DEBUG_ShowLines(cv::Mat &image_param, std::vector<JMD_Vision_LineSegment2D> *border_param);
            void DEBUG_ShowLines(cv::Mat &image_param, std::vector<TagBorderSegment*> *border_param);
        #endif
        /*--- End DEBUG ShowLines ---*/
        
        /*----- DEBUG ShowPoints -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowPoint(cv::Mat &image_param, JMD_Vision_Point2D *point_param);
            void DEBUG_ShowPoints(cv::Mat &image_param);
            void DEBUG_ShowPoints(cv::Mat &image_param, std::vector<JMD_Vision_Point2D> *points_param);
            void DEBUG_ShowPoints(cv::Mat &image_param, unsigned int ring_param);
        #endif
        /*--- End DEBUG ShowPoints ---*/
        
        /*----- DEBUG ShowBorderEstimate -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowBorderEstimate(cv::Mat &image_param);
            void DEBUG_ShowBorderEstimatePoints(cv::Mat &image_param);
        #endif
        /*--- End DEBUG ShowBorderEstimate ---*/
        
        /*----- DEBUG ShowScan -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowScan(cv::Mat &image_param, unsigned int u_param, unsigned int v_param, TagBorderRing *ring_param, unsigned int family_param = 0);
        #endif
        /*--- End DEBUG ShowScan ---*/
        
        /*----- DEBUG ShowScanOrigins -----*/
        void DEBUG_ShowScanOrigins(cv::Mat &image_param);
        /*--- End DEBUG ShowScanOrigins ---*/
        
        /*----- DEBUG ShowCVCorners -----*/
        #ifdef OPENCV_FOUND
            void DEBUG_ShowCVCorners( cv::Mat &image_param, std::vector<cv::Point2f> &corners_param );
        #endif
        /*--- End DEBUG ShowCVCorners ---*/
        
        /*----- DEBUG OutputBorder -----*/
        void DEBUG_OutputBorder();
        void DEBUG_OutputBorder(std::vector<TagBorderSegment*> *border_param);
        /*--- End DEBUG OutputBorder ---*/
        
        /*----- DEBUG OutputDetections -----*/
        void DEBUG_OutputDetections(int subset_start_param = -1, int subset_finish_param = -1);
        /*--- End DEBUG OutputDetections ---*/
        
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