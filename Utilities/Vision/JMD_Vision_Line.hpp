#pragma once
#ifndef JMD_VISION_LINE_H_
#define JMD_VISION_LINE_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD Vision Line: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Line.hpp
 A Class for a Line
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 
 --- End Enumerations ---
 
 ----- Constructors -----
 
 --- end Constructors ---
 
 ----- Utility Methods -----
 
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

//system
#include <iostream>
#include <cmath>
#include <vector>

//utilities
#include "JMD_Vision_Point.hpp"
#include "JMD_Utils_Defines.hpp"

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//------------------------- Macros ------------------------------//
//---------------------------------------------------------------//

//---------------------------------------------------------------//
//------------------------ end Macros ---------------------------//
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

/*----- Forward Declerations -----*/
namespace JMD {
	//classes
	class JMD_Vision_LineSegment2D;
	class JMD_Vision_Line2D;
    
	//friend operators >> , <<
	std::ostream &operator << (std::ostream &os, const JMD_Vision_LineSegment2D &line_param);
    std::ostream &operator << (std::ostream &os, const JMD_Vision_Line2D &line_param);
}
/*--- End Forward Declerations ---*/

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
	/*------------- End Forward Declerations -------------*/
    
    //====================================================================//
    //====================================================================//
    //================ JMD Vision LineSegment2D Definition ===============//
    //====================================================================//
    //====================================================================//
    class JMD_Vision_LineSegment2D
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        /*------------- End Enumerations -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
        // start and end point of line
        JMD_Vision_Point2D myPoint1;
        JMD_Vision_Point2D myPoint2;
        
        /*------------- End Variables -------------*/
        
        
        /*--------------- General ---------------*/
        /*------------- End General -------------*/
        
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
        //    point1_param - point 1 of line
        //    point2_param - point 2 of line
        JMD_Vision_LineSegment2D();
        JMD_Vision_LineSegment2D(JMD_Vision_Point2D *point1_param, JMD_Vision_Point2D *point2_param);
        JMD_Vision_LineSegment2D(double point1_u_param, double point1_v_param, double point2_u_param, double point2_v_param);
		/*------------- end Constructors -------------*/
		
		
        
		/*--------------- Destructors ---------------*/
		~JMD_Vision_LineSegment2D();
		/*------------- end Destructors -------------*/
		
        
        
        /*--------------- Utility Methods ---------------*/
        
        /*----- Midpoint -----*/
        // calculates the midpoint of the line
        //
        // output:
        //    point_param - 2D pt ptr that is set with midpoint values
        void Midpoint(JMD_Vision_Point2D *point_param);
        /*--- End Midpoint ---*/
        
        /*----- Length -----*/
        // calculates the length of the line segment
        //
        double Length();
        //
        // Estimate that doesn't square or sqrt
        //
        double Length_Est();
        /*--- End Length ---*/
        
        /*----- Slope -----*/
        // returns the slope of the line as a point(x,y)
        //
        // output:
        //    point_param - 2D pt ptr that is set with the slope values
        void Slope(JMD_Vision_Point2D *point_param);
        /*--- End Slope ---*/
        
        /*----- Distance To -----*/
        // calculates the distance from *this point to
        // a given line or point
        //
        // input:
        //    line_param - line to find distance to
        //    point_param - point to find distance to
        // output:
        //    double - distance value
        double DistanceTo(JMD_Vision_Point2D *point_param);
        //
        // Estimates that do not calculate squares or sqrts
        //
        double DistanceTo_Est(JMD_Vision_Point2D *point_param);
        /*--- End Distance To ---*/
        
        /*----- Component -----*/
        // given a point in space, calculates the component
        //
        // input:
        //    point_param - ptr to a point
        // output:
        //    double - component value
        double Component(JMD_Vision_Point2D *point_param);
        /*--- End Component ---*/
        
        /*----- Projection -----*/
        // given a point in space, projets that point onto
        // the line
        //
        // input:
        //    point_param - ptr to a point
        // output:
        //    projection_param - ptr to a point
        void Projection(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *projection_param);
        /*--- End Projection ---*/
        
        /*----- IsColinear -----*/
        // given another line, returns true if the lines
        // are colinear and false otherwise
        //
        // input:
        //    line_param - ptr to line for colinearity test
        //    line1_slope_param - slope of line 1
        //    line2_slope_param - slope of line 2
        // output:
        //    bool - true if colinear, false otherwise
        bool IsColinear(JMD_Vision_LineSegment2D *line_param);
        /*--- End IsColinear ---*/
        
        /*----- Intersection -----*/
        // finds the point of intersection between (*this)
        // and a given line, or, if given slopes, can find the
        // intersection of two lines. Failure occurs if the
        // two lines given are parallel and false is returned
        //
        // input:
        //    line_param - ptr to line for intersection
        //    line1_slope_param - slope of line 1
        //    line2_slope_param - slope of line 2
        //    t2_param - where on line 2 intersection happened
        //    t1_param - where on line 1 intersection happened
        // output:
        //    intersection_param - intersection point
        //    bool - true if success, false if failure
        void Intersection(JMD_Vision_LineSegment2D *line_param, JMD_Vision_Point2D *point_param);
        static bool Intersection(JMD_Vision_LineSegment2D *line1_param, JMD_Vision_Point2D *line1_slope_param, JMD_Vision_LineSegment2D *line2_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param, double *t2_param = nullptr, double *t1_param = nullptr);
        static void Intersection(JMD_Vision_Point2D *line1_slope_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param);
        /*--- End Intersection ---*/
        
        
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        
		/*----- Point 1 -----*/
        void                Point1(JMD_Vision_Point2D *point_param);
        void                Point1(double U_param, double V_param, double W_param = 0);
        JMD_Vision_Point2D* Point1();
		/*--- End Point 1 ---*/
		
		/*----- Point 2 -----*/
        void                Point2(JMD_Vision_Point2D *point_param);
        void                Point2(double U_param, double V_param, double W_param = 0);
        JMD_Vision_Point2D* Point2();
		/*--- End Point 2 ---*/
        
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
        
        /*----- >, >=, <, <= operators -----*/
        // compares the lengths of the *this line and provided line
        //
        // input:
        //    line_param - line to compare against
        bool operator >  (const JMD_Vision_LineSegment2D &line_param);
        bool operator >= (const JMD_Vision_LineSegment2D &line_param);
        bool operator <  (const JMD_Vision_LineSegment2D &line_param);
        bool operator <= (const JMD_Vision_LineSegment2D &line_param);
        /*--- End >, >=, <, <= operators ---*/
        
        /*----- Operator << -----*/
        // passes along the line data in an out stream object, for use
		// in things like cout; e.g. cout << line_param;
		// 
        friend std::ostream& operator << (std::ostream &os, const JMD_Vision_LineSegment2D &line_param);
        /*--- End Operator << ---*/
        
        /*------------- End Operator Overloads -------------*/
        
        //---------------------------------------------------------------//
        //------------------------ end Public ---------------------------//
        //---------------------------------------------------------------//
        
        
    };
    //====================================================================//
    //====================================================================//
    //====================================================================//
    //====================================================================//
    //====================================================================//
    
    
    
    
    
    //====================================================================//
    //====================================================================//
    //==================== JMD Vision Line2D Definition ==================//
    //====================================================================//
    //====================================================================//
    class JMD_Vision_Line2D
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        /*------------- End Enumerations -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
        //parametric form
        JMD_Vision_Point2D myPoint;
        JMD_Vision_Point2D myDirection;
        
        //slope intercept form (y = mx + b)
        double myM;
        double myB;
        
        /*------------- End Variables -------------*/
        
        
        /*--------------- General ---------------*/
        
        /*----- Private Fit Line -----*/
        // fits a line to points given the sum of the points
        bool Private_FitLine();
        /*--- End Fit Line ---*/
        
        /*------------- End General -------------*/
        
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
        //    point_param - point 1 of line (parametric)
        //    direction_param - vector direction of line with point (parametric)
        JMD_Vision_Line2D();
        JMD_Vision_Line2D(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *direction_param);
        //
        //    points_param - vector of points, line is least squares best fit line
        JMD_Vision_Line2D(std::vector<JMD_Vision_Point2D> *points_param);
        JMD_Vision_Line2D(std::vector<JMD_Vision_Point2D*> *points_param);
		/*------------- end Constructors -------------*/
		
		
        
		/*--------------- Destructors ---------------*/
		~JMD_Vision_Line2D();
		/*------------- end Destructors -------------*/
		
        
        
        /*--------------- Utility Methods ---------------*/
        
        /*----- Distance To -----*/
        // calculates the distance from *this point to
        // a given line or point
        //
        // input:
        //    line_param - line to find distance to
        //    point_param - point to find distance to
        // output:
        //    double - distance value
        //double DistanceTo(JMD_Vision_Point2D *point_param);
        //
        // Estimates that do not calculate squares or sqrts
        //
        //double DistanceTo_Est(JMD_Vision_Point2D *point_param);
        /*--- End Distance To ---*/
        
        /*----- Component -----*/
        // given a point in space, calculates the component
        //
        // input:
        //    point_param - ptr to a point
        // output:
        //    double - component value
        //double Component(JMD_Vision_Point2D *point_param);
        /*--- End Component ---*/
        
        /*----- Projection -----*/
        // given a point in space, projets that point onto
        // the line
        //
        // input:
        //    point_param - ptr to a point
        // output:
        //    projection_param - ptr to a point
        //void Projection(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *projection_param);
        /*--- End Projection ---*/
        
        /*----- Intersection -----*/
        // finds the point of intersection between (*this)
        // and a given line, or, if given slopes, can find the
        // intersection of two lines. Failure occurs if the
        // two lines given are parallel
        //
        // input:
        //    line_param - ptr to line for intersection
        //    line1_slope_param - slope of line 1
        //    line2_slope_param - slope of line 2
        //    t2_param - where on line 2 intersection happened
        //    t1_param - where on line 1 intersection happened
        // output:
        //    intersection_param - intersection point
        //    bool - true if success, false if failure
        //void Intersection(JMD_Vision_LineSegment2D *line_param, JMD_Vision_Point2D *point_param);
        //static bool Intersection(JMD_Vision_LineSegment2D *line1_param, JMD_Vision_Point2D *line1_slope_param, JMD_Vision_LineSegment2D *line2_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param, double *t2_param = nullptr, double *t1_param = nullptr);
        //static void Intersection(JMD_Vision_Point2D *line1_slope_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param);
        /*--- End Intersection ---*/
        
        
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        
		/*----- Point -----*/
        void                Point(JMD_Vision_Point2D *point_param);
        void                Point(double U_param, double V_param, double W_param = 0);
        JMD_Vision_Point2D* Point();
		/*--- End Point 1 ---*/
		
		/*----- Direction -----*/
        void                Direction(JMD_Vision_Point2D *point_param);
        void                Direction(double U_param, double V_param, double W_param = 0);
        JMD_Vision_Point2D* Direction();
		/*--- End Direction ---*/
        
        /*----- M -----*/
        void M(double M_param);
        double M();
        /*--- End M ---*/
        
        /*----- B -----*/
        void B(double B_param);
        double B();
        /*--- End B ---*/
        
        /*----- XY -----*/
        double X(double Y_param);
        double Y(double X_param);
        /*--- End XY ---*/
        
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
        
        /*----- Operator << -----*/
        // passes along the line data in an out stream object, for use
		// in things like cout; e.g. cout << line_param;
		// 
        friend std::ostream& operator << (std::ostream &os, const JMD_Vision_Line2D &line_param);
        /*--- End Operator << ---*/
        
        /*------------- End Operator Overloads -------------*/
        
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

