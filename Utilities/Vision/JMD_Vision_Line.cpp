/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD Vision Line: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Line.cpp
 A Class for a Line
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

#include "JMD_Vision_Line.hpp"

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
//============= JMD Vision LineSegment2D Implementation ==============//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_Vision_LineSegment2D::JMD_Vision_LineSegment2D() 
{ 
	
}
JMD::JMD_Vision_LineSegment2D::JMD_Vision_LineSegment2D(JMD_Vision_Point2D *point1_param, JMD_Vision_Point2D *point2_param)
{
	myPoint1.U( point1_param->U() );
	myPoint1.V( point1_param->V() );
	myPoint1.W( point1_param->W() );
	myPoint2.U( point2_param->U() );
	myPoint2.V( point2_param->V() );
	myPoint2.W( point2_param->W() );
}
JMD::JMD_Vision_LineSegment2D::JMD_Vision_LineSegment2D(double point1_u_param, double point1_v_param, double point2_u_param, double point2_v_param)
{
	myPoint1.U( point1_u_param );
	myPoint1.V( point1_v_param );
	myPoint2.U( point2_u_param );
	myPoint2.V( point2_v_param );
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_Vision_LineSegment2D::~JMD_Vision_LineSegment2D() { }
/*------------- End Destructors -------------*/

    

/*--------------- Utility ---------------*/

/*----- Midpoint -----*/
void JMD::JMD_Vision_LineSegment2D::Midpoint( JMD_Vision_Point2D *point_param )
{
	point_param->U( (myPoint1.U() + myPoint2.U()) * 0.5 );
	point_param->V( (myPoint1.V() + myPoint2.V()) * 0.5 );
}
/*--- End Midpoint ---*/

/*----- Length -----*/
double JMD::JMD_Vision_LineSegment2D::Length()
{
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	return sqrt(x*x + y*y);
}
double JMD::JMD_Vision_LineSegment2D::Length_Est()
{
	return fabs( myPoint2.U() - myPoint1.U() ) + fabs( myPoint2.V() - myPoint1.V() );
}
/*--- End Length ---*/

/*----- Slope -----*/
void JMD::JMD_Vision_LineSegment2D::Slope( JMD_Vision_Point2D *point_param )
{
	//Length
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	double l = sqrt( x*x + y*y );
	
	//return
	point_param->U( x / l );
	point_param->V( y / l );
}
/*--- End Slope ---*/

/*----- Distance To -----*/
double JMD::JMD_Vision_LineSegment2D::DistanceTo(JMD_Vision_Point2D *point_param)
{
	// | (x2-x1)(y2-y0) - (x1-x0)(y2-y0) |
	double a = ( myPoint2.U() - myPoint1.U() );		//x2 - x1
	double b = ( myPoint1.V() - point_param->V() ); //y1 - y0
	double c = ( myPoint1.U() - point_param->U() ); //x1 - x0
	double d = ( myPoint2.V() - myPoint1.V() );     //y2 - y1
	double top = fabs( a*b - c*d );
	
	// sqrt( (x2-x1)^2 + (y2-y1)^2 )
	double bot = sqrt( a*a + d*d );
	
	// return
	return top / bot;
}
double JMD::JMD_Vision_LineSegment2D::DistanceTo_Est(JMD_Vision_Point2D *point_param)
{
	return 0;
}
/*--- End Distance To ---*/

/*----- Component -----*/
double JMD::JMD_Vision_LineSegment2D::Component(JMD_Vision_Point2D *point_param)
{
	//variables
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	double l = sqrt( x*x + y*y );
	
	// (a . b) / |a|
	return (x * point_param->U() + y * point_param->V()) / l;
}
/*--- End Component ---*/

/*----- Projection -----*/
void JMD::JMD_Vision_LineSegment2D::Projection(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *projection_param)
{
	//variables
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	double l = sqrt( x*x + y*y );
	
	// (a . b) / |a|
	JMD_Vision_Point2D b( point_param->U() - myPoint1.U(), point_param->V() - myPoint1.V() );
	double component = (x * b.U() + y * b.V()) / l;
	
	// projection = component * a / |a|
	double projection_scale = component / l;
	projection_param->U( projection_scale * x + myPoint1.U() );
	projection_param->V( projection_scale * y + myPoint1.V() );
}
/*--- End Projection ---*/

/*----- IsColinear -----*/
bool JMD::JMD_Vision_LineSegment2D::IsColinear(JMD_Vision_LineSegment2D *line_param)
{
	//not colinear
	return false;
}
/*--- End IsColinear ---*/

/*----- Intersection -----*/
void JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_LineSegment2D *line_param, JMD_Vision_Point2D *point_param)
{

}
void JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_Point2D *line1_slope_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param)
{
	double cx = ( line1_slope_param->V() - line2_slope_param->V() ) / ( line2_slope_param->U() - line1_slope_param->U() );
	double cy = line1_slope_param->U() * cx + line1_slope_param->V();
	intersection_param->U( cx );
	intersection_param->V( cy );
}
bool JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_LineSegment2D *line1_param, JMD_Vision_Point2D *line1_slope_param, JMD_Vision_LineSegment2D *line2_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param, double *t2_param, double *t1_param)
{
	//variables
	double x1 = line1_param->myPoint1.U();
	double x2 = line2_param->myPoint1.U();
	double y1 = line1_param->myPoint1.V();
	double y2 = line2_param->myPoint1.V();
	double v1x = line1_slope_param->U();
	double v1y = line1_slope_param->V();
	double v2x = line2_slope_param->U();
	double v2y = line2_slope_param->V();
	//std::cout << "pt1 = ( " << x1 << ", " << y1 << " ).    pt2 = ( " << x2 << ", " << y2 << " ) " << std::endl;
	//std::cout << "pt3 = " << line1_param->myPoint2 << ".   pt4 = " << line2_param->myPoint2 << std::endl; 
	//std::cout << "v1  = ( " << v1x << ", " << v1y << " ).    v2 = ( " << v2x << ", " << v2y << " ) " << std::endl;
	
	//check for parallel lines
	double den = v1x*v2y - v1y*v2x;
	//std::cout << "denom = " << den << std::endl;
	if ( fabs(den) < JMD::DEF_EQUALITY_THRESHOLD ) 
	{
		//(p2-p1) x v1 == 0 ==> colinear lines
		if( fabs( (x2-x1)*v1y - (y2-y1)*v1x ) < JMD::DEF_EQUALITY_THRESHOLD ) 
		{ 
			// populate intersection as midpoint of middle line
			double U = 0.5 * (line1_param->myPoint2.U() + line2_param->myPoint1.U() );
			double V = 0.5 * (line1_param->myPoint2.V() + line2_param->myPoint1.V() );
			intersection_param->U( U );
			intersection_param->V( V );
			
			// populate t's
			double t1 = (v1x != 0) ? (U - x1)/v1x : (V - y1)/v1y;
			double t2 = (v2x != 0) ? (U - x2)/v2x : (V - y2)/v2y;
			if(t1_param) { *t1_param = t1; }
			if(t2_param) { *t2_param = t2; }
			
			//return success
			//std::cout << "COLINEAR: picking midpoint of midline ==> " << U << " " << V << " " << t1 << " " << t2 << std::endl;
			return true;
		}
		//(p2-p1) x v1 != 0 ==> parallel lines
		else { return false; } 
	}
	
	//times
	double t2 = ( v1y * (x2 - x1) - v1x * (y2 - y1) ) / den;
	double t1 = (abs(v1x) < JMD::DEF_EQUALITY_THRESHOLD) ? (y2 - y1 + v2y*t2) / v1y : ( x2 - x1 + v2x*t2 ) / v1x;
	
	//intersection
	//double c2x = x2 + v2x * t2;
	//double c2y = y2 + v2y * t2;
	//double c1x = x1 + v1x * t1;
	//double c1y = y1 + v1y * t1;
	intersection_param->U( x2 + v2x * t2 );
	intersection_param->V( y2 + v2y * t2 );
	
	//std::cout << "t2 = " << t2 << ".    t1 = " << t1 << std::endl;
	//std::cout << "dot product = " << v1x * v2x + v1y * v2y << std::endl;
	//std::cout << "c2 = ( " << c2x << ", " << c2y << " )    c1 = ( " << c1x << ", " << c1y << " )" << std::endl; 
	
	//return t
	if(t1_param) { *t1_param = t1; }
	if(t2_param) { *t2_param = t2; }
	
	//success
	return true;
}
/*--- End Intersection ---*/

/*------------- End Utility -------------*/



/*--------------- Setters/Getters ---------------*/
    
/*----- Point 1 -----*/
void JMD::JMD_Vision_LineSegment2D::Point1(JMD_Vision_Point2D *point_param) { Point1( point_param->U() , point_param->V(), point_param->W() ); }
void JMD::JMD_Vision_LineSegment2D::Point1(double U_param, double V_param, double W_param)
{
	myPoint1.U( U_param );
	myPoint1.V( V_param );
	myPoint1.W( W_param );
}
JMD::JMD_Vision_Point2D* JMD::JMD_Vision_LineSegment2D::Point1() { return &myPoint1; }
/*--- End Point 1 ---*/

/*----- Point 2 -----*/
void JMD::JMD_Vision_LineSegment2D::Point2(JMD_Vision_Point2D *point_param) { Point2( point_param->U() , point_param->V(), point_param->W() ); }
void JMD::JMD_Vision_LineSegment2D::Point2(double U_param, double V_param, double W_param)
{
	myPoint2.U( U_param );
	myPoint2.V( V_param );
	myPoint2.W( W_param );
}
JMD::JMD_Vision_Point2D* JMD::JMD_Vision_LineSegment2D::Point2() { return &myPoint2; }
/*--- End Point 2 ---*/

/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/

/*----- >, >=, <, <= operators -----*/
bool JMD::JMD_Vision_LineSegment2D::operator >  (const JMD_Vision_LineSegment2D &line_param) { return true; }
bool JMD::JMD_Vision_LineSegment2D::operator >= (const JMD_Vision_LineSegment2D &line_param) { return true; }
bool JMD::JMD_Vision_LineSegment2D::operator <  (const JMD_Vision_LineSegment2D &line_param) { return true; }
bool JMD::JMD_Vision_LineSegment2D::operator <= (const JMD_Vision_LineSegment2D &line_param) { return true; }
/*--- End >, >=, <, <= operators ---*/

/*----- Operator << -----*/
std::ostream& JMD::operator << (std::ostream &os, const JMD::JMD_Vision_LineSegment2D &line_param)
{
	
	/*----- Output Stream -----*/
	
	os << line_param.myPoint1 << " ==> " << line_param.myPoint2;
			
	/*--- End Output Stream ---*/
	
	//return
	return os;
}
/*--- End Operator << ---*/

/*--------------- Operator Overloads ---------------*/


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//





//====================================================================//
//====================================================================//
//================= JMD Vision Line2D Implementation =================//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//

/*--------------- General ---------------*/

/*----- Private Fit Line -----*/
bool JMD::JMD_Vision_Line2D::Private_FitLine()
{
	return false;
}
/*--- End Private Fit Line ---*/

/*------------- End General -------------*/

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_Vision_Line2D::JMD_Vision_Line2D() 
{ 
	
}
JMD::JMD_Vision_Line2D::JMD_Vision_Line2D(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *direction_param)
{
	//point and direction (parametric)
	myPoint.U( point_param->U() );
	myPoint.V( point_param->V() );
	myPoint.W( point_param->W() );
	myDirection.U( direction_param->U() );
	myDirection.V( direction_param->V() );
	myDirection.W( direction_param->W() );
	
	//M and B (slope intercept)
	// check for non-infinite slope
	if( fabs(myDirection.U()) < JMD::DEF_EQUALITY_THRESHOLD )
	{
		//set to infinite
		myM = JMD::DEF_PINF;
		myB = JMD::DEF_PINF;
	}
	else
	{
		//myM = Vy / Vx
		myM = myDirection.V() / myDirection.U();
		
		//myB = Py + t*Vy
		double t = -myPoint.U() / myDirection.U();
		myB = myPoint.V() + t * myDirection.V();
	}
}
JMD::JMD_Vision_Line2D::JMD_Vision_Line2D(std::vector<JMD_Vision_Point2D> *points_param)
{
	//variables
	unsigned int ct = 0;
	double x,y;
	double sumx  = 0, sumy  = 0;
	double sumx2 = 0, sumy2 = 0;
	double sumxy = 0;
	double denom, meanx, meany;
	JMD_Vision_Point2D *currPt;
	
	//calculate sums for all points
	for(std::vector<JMD_Vision_Point2D>::iterator it = points_param->begin(); it != points_param->end(); ++it)
	{
		//sums
		sumx += it->U();
		sumy += it->V();
		sumx2 += it->U() * it->U();
		sumy2 += it->V() * it->V();
		sumxy += it->U() * it->V();
		
		//count
		ct++;
	}
	
	//enough points?
	if(ct < 2) 
	{ 
		myM = JMD::DEF_PINF; 
		myB = JMD::DEF_PINF;
		myPoint.U(JMD::DEF_PINF);
		myPoint.V(JMD::DEF_PINF);
		myDirection.U(JMD::DEF_PINF);
		myDirection.V(JMD::DEF_PINF);
		return;
	}
	
	//check for infinite slope
	meanx = sumx / ct;
	meany = sumy / ct;
	denom = sumx2 - sumx * meanx;
	if( fabs(denom) < JMD::DEF_EQUALITY_THRESHOLD )
	{
		//set M and B
		myM = JMD::DEF_PINF;
		myB = JMD::DEF_PINF;
		
		//set point to (x,0 )and direction (1,0)
		myPoint.U(meanx);
		myPoint.V(0);
		myDirection.U(0);
		myDirection.V(1);
	}
	else
	{
		//set M and B
		myM = (sumxy - sumx * meany) / denom;
		myB = meany - myM * meanx;
		
		//set point to (0,B) and direction
		myPoint.U(0);
		myPoint.V(myB);
		myDirection.U(1);
		myDirection.V(myM);
	}
}
JMD::JMD_Vision_Line2D::JMD_Vision_Line2D(std::vector<JMD_Vision_Point2D*> *points_param)
{
	//variables
	unsigned int ct = 0;
	double x,y;
	double sumx  = 0, sumy  = 0;
	double sumx2 = 0, sumy2 = 0;
	double sumxy = 0;
	double denom, meanx, meany;
	JMD_Vision_Point2D *currPt;
	
	//calculate sums for all points
	for(std::vector<JMD_Vision_Point2D*>::iterator it = points_param->begin(); it != points_param->end(); ++it)
	{
		//current point
		currPt = *it;
		
		//sums
		sumx += currPt->U();
		sumy += currPt->V();
		sumx2 += currPt->U() * currPt->U();
		sumy2 += currPt->V() * currPt->V();
		sumxy += currPt->U() * currPt->V();
		
		//count
		ct++;
	}
	
	//enough points?
	if(ct < 2) 
	{ 
		myM = JMD::DEF_PINF; 
		myB = JMD::DEF_PINF;
		myPoint.U(JMD::DEF_PINF);
		myPoint.V(JMD::DEF_PINF);
		myDirection.U(JMD::DEF_PINF);
		myDirection.V(JMD::DEF_PINF);
		return;
	}
	
	//check for infinite slope
	meanx = sumx / ct;
	meany = sumy / ct;
	denom = sumx2 - sumx * meanx;
	if( fabs(denom) < JMD::DEF_EQUALITY_THRESHOLD )
	{
		//set M and B
		myM = JMD::DEF_PINF;
		myB = JMD::DEF_PINF;
		
		//set point to (x,0 )and direction (1,0)
		myPoint.U(meanx);
		myPoint.V(0);
		myDirection.U(0);
		myDirection.V(1);
	}
	else
	{
		//set M and B
		myM = (sumxy - sumx * meany) / denom;
		myB = meany - myM * meanx;
		
		//set point to (0,B) and direction
		myPoint.U(0);
		myPoint.V(myB);
		myDirection.U(1);
		myDirection.V(myM);
	}
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_Vision_Line2D::~JMD_Vision_Line2D() { }
/*------------- End Destructors -------------*/

    

/*--------------- Utility ---------------*/

/*----- Distance To -----*/
/*double JMD::JMD_Vision_LineSegment2D::DistanceTo(JMD_Vision_Point2D *point_param)
{
	// | (x2-x1)(y2-y0) - (x1-x0)(y2-y0) |
	double a = ( myPoint2.U() - myPoint1.U() );		//x2 - x1
	double b = ( myPoint1.V() - point_param->V() ); //y1 - y0
	double c = ( myPoint1.U() - point_param->U() ); //x1 - x0
	double d = ( myPoint2.V() - myPoint1.V() );     //y2 - y1
	double top = fabs( a*b - c*d );
	
	// sqrt( (x2-x1)^2 + (y2-y1)^2 )
	double bot = sqrt( a*a + d*d );
	
	// return
	return top / bot;
}
double JMD::JMD_Vision_LineSegment2D::DistanceTo_Est(JMD_Vision_Point2D *point_param)
{
	return 0;
}*/
/*--- End Distance To ---*/

/*----- Component -----*/
/*double JMD::JMD_Vision_LineSegment2D::Component(JMD_Vision_Point2D *point_param)
{
	//variables
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	double l = sqrt( x*x + y*y );
	
	// (a . b) / |a|
	return (x * point_param->U() + y * point_param->V()) / l;
}*/
/*--- End Component ---*/

/*----- Projection -----*/
/*void JMD::JMD_Vision_LineSegment2D::Projection(JMD_Vision_Point2D *point_param, JMD_Vision_Point2D *projection_param)
{
	//variables
	double x = myPoint2.U() - myPoint1.U();
	double y = myPoint2.V() - myPoint1.V();
	double l = sqrt( x*x + y*y );
	
	// (a . b) / |a|
	JMD_Vision_Point2D b( point_param->U() - myPoint1.U(), point_param->V() - myPoint1.V() );
	double component = (x * b.U() + y * b.V()) / l;
	
	// projection = component * a / |a|
	double projection_scale = component / l;
	projection_param->U( projection_scale * x + myPoint1.U() );
	projection_param->V( projection_scale * y + myPoint1.V() );
}*/
/*--- End Projection ---*/

/*----- Intersection -----*/
/*void JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_LineSegment2D *line_param, JMD_Vision_Point2D *point_param)
{

}
void JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_Point2D *line1_slope_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param)
{
	double cx = ( line1_slope_param->V() - line2_slope_param->V() ) / ( line2_slope_param->U() - line1_slope_param->U() );
	double cy = line1_slope_param->U() * cx + line1_slope_param->V();
	intersection_param->U( cx );
	intersection_param->V( cy );
}
bool JMD::JMD_Vision_LineSegment2D::Intersection(JMD_Vision_LineSegment2D *line1_param, JMD_Vision_Point2D *line1_slope_param, JMD_Vision_LineSegment2D *line2_param, JMD_Vision_Point2D *line2_slope_param, JMD_Vision_Point2D *intersection_param, double *t2_param, double *t1_param)
{
	//variables
	double x1 = line1_param->myPoint1.U();
	double x2 = line2_param->myPoint1.U();
	double y1 = line1_param->myPoint1.V();
	double y2 = line2_param->myPoint1.V();
	double v1x = line1_slope_param->U();
	double v1y = line1_slope_param->V();
	double v2x = line2_slope_param->U();
	double v2y = line2_slope_param->V();
	
	//check for parallel lines
	double den = v1x*v2y - v1y*v2x;
	if ( den < JMD::DEF_EQUALITY_THRESHOLD ) { return false; }
	
	//times
	double t2 = ( v1y * (x2 - x1) - v1x * (y2 - y1) ) / den;
	double t1 = (abs(v1x) < JMD::DEF_EQUALITY_THRESHOLD) ? (y2 - y1 + v2y*t2) / v1y : ( x2 - x1 + v2x*t2 ) / v1x;
	
	//intersection
	//double c2x = x2 + v2x * t2;
	//double c2y = y2 + v2y * t2;
	//double c1x = x1 + v1x * t1;
	//double c1y = y1 + v1y * t1;
	intersection_param->U( x2 + v2x * t2 );
	intersection_param->V( y2 + v2y * t2 );
	
	//std::cout << "pt1 = ( " << x1 << ", " << y1 << " ).    pt2 = ( " << x2 << ", " << y2 << " ) " << std::endl;
	//std::cout << "v1  = ( " << v1x << ", " << v1y << " ).    v2 = ( " << v2x << ", " << v2y << " ) " << std::endl;
	//std::cout << "t2 = " << t2 << ".    t1 = " << t1 << std::endl;
	//std::cout << "dot product = " << v1x * v2x + v1y * v2y << std::endl;
	//std::cout << "c2 = ( " << c2x << ", " << c2y << " )    c1 = ( " << c1x << ", " << c1y << " )" << std::endl; 
	
	//return t
	if(t1_param) { *t1_param = t1; }
	if(t2_param) { *t2_param = t2; }
	
	//success
	return true;
}*/
/*--- End Intersection ---*/

/*------------- End Utility -------------*/



/*--------------- Setters/Getters ---------------*/
    
/*----- Point -----*/
void JMD::JMD_Vision_Line2D::Point(JMD_Vision_Point2D *point_param) { Point( point_param->U() , point_param->V(), point_param->W() ); }
void JMD::JMD_Vision_Line2D::Point(double U_param, double V_param, double W_param)
{
	myPoint.U( U_param );
	myPoint.V( V_param );
	myPoint.W( W_param );
}
JMD::JMD_Vision_Point2D* JMD::JMD_Vision_Line2D::Point() { return &myPoint; }
/*--- End Point ---*/

/*----- Direction -----*/
void JMD::JMD_Vision_Line2D::Direction(JMD_Vision_Point2D *point_param) { Direction( point_param->U() , point_param->V(), point_param->W() ); }
void JMD::JMD_Vision_Line2D::Direction(double U_param, double V_param, double W_param)
{
	myDirection.U( U_param );
	myDirection.V( V_param );
	myDirection.W( W_param );
}
JMD::JMD_Vision_Point2D* JMD::JMD_Vision_Line2D::Direction() { return &myDirection; }
/*--- End Direction ---*/

/*----- M -----*/
void JMD::JMD_Vision_Line2D::M(double M_param) { myM = M_param; }
double JMD::JMD_Vision_Line2D::M() { return myM; }
/*--- End M ---*/

/*----- B -----*/
void JMD::JMD_Vision_Line2D::B(double B_param) { myB = B_param; }
double JMD::JMD_Vision_Line2D::B() { return myB; }
/*--- End B ---*/

/*----- XY -----*/
// y = mx + b
double JMD::JMD_Vision_Line2D::X(double Y_param) 
{
	//if slope is infinite
	if( myM == JMD::DEF_PINF) { return myPoint.U(); }
	
	//else x = (y-b) / m
	else                                  { return (Y_param - myB) / myM; }  
}
double JMD::JMD_Vision_Line2D::Y(double X_param) 
{
	//if slope is infinite
	if( myM == JMD::DEF_PINF) { return JMD::DEF_PINF; }
	
	//else y = mx + b
	else                                  { return myM * X_param + myB; }
}
/*--- End XY ---*/

/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/

/*----- Operator << -----*/
std::ostream& JMD::operator << (std::ostream &os, const JMD::JMD_Vision_Line2D &line_param)
{
	
	/*----- Output Stream -----*/
	
	os << line_param.myPoint << " + t * " << line_param.myDirection << "  ( y = " << line_param.myM << "x + " << line_param.myB << " )";
			
	/*--- End Output Stream ---*/
	
	//return
	return os;
}
/*--- End Operator << ---*/

/*--------------- Operator Overloads ---------------*/


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
