/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Math: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Math.cpp
 A class for math
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

#include "JMD_Math_Common.hpp"

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





//================================================================================//
//================================================================================//
//====================== JMD Utils Math Class Implementation =====================//
//================================================================================//
//================================================================================//

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
JMD::JMD_Utils_Math::JMD_Utils_Math() {}
/*------------- end Constructors -------------*/


/*--------------- Destructors ---------------*/
JMD::JMD_Utils_Math::~JMD_Utils_Math() { }
/*------------- end Destructors -------------*/


/*--------------- General ---------------*/
		
/*----- Degrees To Radians -----*/
double JMD::JMD_Utils_Math::DegreesToRadians(double degree_param) { return degree_param * JMD::DEF_MATH_PI / 180.0; }
/*--- End Degrees To Radians ---*/

/*----- Radians To Degrees -----*/
double JMD::JMD_Utils_Math::RadiansToDegrees(double radian_param) { return radian_param * 180.0 / JMD::DEF_MATH_PI; }
/*--- End Radians To Degrees ---*/

/*----- Factorial -----*/
unsigned int JMD::JMD_Utils_Math::Factorial(unsigned int num_param, unsigned int stop_param)
{
	//variables
	unsigned int result = stop_param;
	
	//calculate
	for(int i = stop_param+1; i <= num_param; i++) { result *= i; }
	
	//return
	return result;
}
/*--- End Factorial ---*/

/*----- NChooseK -----*/
unsigned int JMD::JMD_Utils_Math::NChooseK(unsigned int n_param, unsigned int k_param)
{
	return Factorial(n_param,k_param+1) / Factorial(n_param - k_param);
}
/*--- End NChooseK ---*/

/*----- Modulo -----*/
int JMD::JMD_Utils_Math::Modulo(int n_param, int M_param)
{
	return ((n_param % M_param) + M_param) % M_param;
}
double JMD::JMD_Utils_Math::Modulo(double n_param, double M_param)
{
	return fmod(fmod(n_param,M_param) + M_param,M_param);
}
/*--- End Modulo ---*/

/*------------- End General -------------*/

		
/*--------------- Vector Methods ---------------*/

/*----- Vector Length -----*/
float  JMD::JMD_Utils_Math::Vector_Length(float *vector_param, unsigned int size_param) { return Vector_Norm(vector_param,size_param,2); }
double JMD::JMD_Utils_Math::Vector_Length(double *vector_param, unsigned int size_param) { return Vector_Norm(vector_param,size_param,2); }
float  JMD::JMD_Utils_Math::Vector_Length(vector<float> *vector_param) { return Vector_Norm(vector_param,2); }
double JMD::JMD_Utils_Math::Vector_Length(vector<double> *vector_param) { return Vector_Norm(vector_param,2); }
/*--- End Vector Length ---*/

/*----- Vector Norm -----*/
float JMD::JMD_Utils_Math::Vector_Norm(float *vector_param, unsigned int size_param, unsigned int norm_param)
{
	//calculate sum|x_i^norm_param|
	float sum = 0.0f;
	for( int i=0; i < size_param; i++ ) { sum += pow( abs(vector_param[i]), norm_param ); }
	
	//return norm_param-root(sum)
	return pow( sum, 1.0f/norm_param );
}
double JMD::JMD_Utils_Math::Vector_Norm(double *vector_param, unsigned int size_param, unsigned int norm_param)
{
    //calculate sum|x_i^norm_param|
	double sum = 0.0;
	for( int i=0; i < size_param; i++ ) { sum += pow( abs(vector_param[i]), norm_param ); }
	
	//return norm_param-root(sum)
	return pow( sum, 1.0/norm_param );	
}
float JMD::JMD_Utils_Math::Vector_Norm(vector<float> *vector_param, unsigned int norm_param)
{
	//calculate sum|x_i^norm_param|
	float sum = 0.0f;
	for( float i : *vector_param ) { sum += pow( abs(i), norm_param ); }
	
	//return norm_param-root(sum)
	return pow( sum, 1.0f/norm_param );
}
double JMD::JMD_Utils_Math::Vector_Norm(vector<double> *vector_param, unsigned int norm_param)
{
	//calculate sum|x_i^norm_param|
	double sum = 0.0;
	for( double i : *vector_param ) { sum += pow( abs(i), norm_param ); }
	
	//return norm_param-root(sum)
	return pow( sum, 1.0/norm_param );
}
/*--- End Vector Norm ---*/

/*----- Vector Normalize -----*/
void JMD::JMD_Utils_Math::Vector_Normalize(float *vector_param, unsigned int size_param) 
{
	//get length
	float length = Vector_Length(vector_param, size_param);
	
	//normalize elements
	for( int i=0; i < size_param; i++ ) { vector_param[i] /= length; }
}
void JMD::JMD_Utils_Math::Vector_Normalize(double *vector_param, unsigned int size_param) 
{
	//get length
	double length = Vector_Length(vector_param, size_param);
	
	//normalize elements
	for( int i=0; i < size_param; i++ ) { vector_param[i] /= length; }
}
void JMD::JMD_Utils_Math::Vector_Normalize(vector<float> *vector_param) 
{
	//get length
	float length = Vector_Length(vector_param);
	
	//normalize elements
	for(vector<float>::iterator iter=vector_param->begin(); iter != vector_param->end(); ++iter)
	{
		*iter /= length;
	}
}
void JMD::JMD_Utils_Math::Vector_Normalize(vector<double> *vector_param) 
{
	//get length
	float length = Vector_Length(vector_param);
	
	//normalize elements
	for(vector<double>::iterator iter=vector_param->begin(); iter != vector_param->end(); ++iter)
	{
		*iter /= length;
	}
}
/*--- End Vector Normalize ---*/

/*----- Vector Dot Product -----*/
float JMD::JMD_Utils_Math::Vector_Dot_Product(float *vector1_param, float *vector2_param, int vector1_size_param, int vector2_size_param)
{
	//dot product
	float dot_product = 0.0f;
	
	//check that vectors are the same size
	if( vector1_size_param != vector2_size_param )
	{
		myWriter.Writer_Error("JMD_Utils_Math","Vector_Dot_Product","Provided Vectors are not the same size. Returning 0.",0);
	}
	else
	{
		for(int i=0; i < vector1_size_param; i++)
		{
			dot_product += vector1_param[i] * vector2_param[i];
		}
	}
	
	//return
	return dot_product;
}
float JMD::JMD_Utils_Math::Vector_Dot_Product(vector<float> *vector1_param, vector<float> *vector2_param)
{
	//dot product
	float dot_product = 0.0f;
	
	//check that vectors are same size
	if( vector1_param->size() != vector2_param->size() )
	{
		myWriter.Writer_Error("JMD_Utils_Math","Vector_Dot_Product","Provided Vectors are not the same size. Returning 0.",0);
	}
	else
	{
		//iterators
		typedef vector<float>::iterator iter;
		
		//sum dot product
		for(pair<iter,iter> iters(vector1_param->begin(),vector2_param->begin()); iters.first != vector1_param->end(); ++iters.first, ++iters.second)
		{
			dot_product += (*iters.first) * (*iters.second);
		}
	}
	
	//return
	return dot_product;
}
double JMD::JMD_Utils_Math::Vector_Dot_Product(double *vector1_param, double *vector2_param, int vector1_size_param, int vector2_size_param)
{
	//dot product
	double dot_product = 0.0;
	
	//check that vectors are the same size
	if( vector1_size_param != vector2_size_param )
	{
		myWriter.Writer_Error("JMD_Utils_Math","Vector_Dot_Product","Provided Vectors are not the same size. Returning 0.",0);
	}
	else
	{
		for(int i=0; i < vector1_size_param; i++)
		{
			dot_product += vector1_param[i] * vector2_param[i];
		}
	}
	
	//return
	return dot_product;
}
double JMD::JMD_Utils_Math::Vector_Dot_Product(vector<double> *vector1_param, vector<double> *vector2_param)
{
	//dot product
	double dot_product = 0.0;
	
	//check that vectors are same size
	if( vector1_param->size() != vector2_param->size() )
	{
		myWriter.Writer_Error("JMD_Utils_Math","Vector_Dot_Product","Provided Vectors are not the same size. Returning 0.",0);
	}
	else
	{
		//iterators
		typedef vector<double>::iterator iter;
		
		//sum dot product
		for(pair<iter,iter> iters(vector1_param->begin(),vector2_param->begin()); iters.first != vector1_param->end(); ++iters.first, ++iters.second)
		{
			dot_product += (*iters.first) * (*iters.second);
		}
	}
	
	//return
	return dot_product;
}
/*--- End Vector Dot Product ---*/
		
/*----- Vector Cross Product -----*/
// computes the cross product of two vectors and stores the 
// result in a third vector
//
// input:
//    vector1_param - pointer to vector A (double,float)
//    vector2_param - poitner to vector B (double,float)
//    out_vector_param - A X B
void JMD::JMD_Utils_Math::Vector_Cross_Product_3Dim(float *vector1_param, float *vector2_param, float *out_vector_param)
{
	out_vector_param[0] =  ( vector1_param[1]*vector2_param[2] - vector1_param[2]*vector2_param[1] );
	out_vector_param[1] = -( vector1_param[0]*vector2_param[2] - vector1_param[2]*vector2_param[0] );
	out_vector_param[2] =  ( vector1_param[0]*vector2_param[1] - vector1_param[1]*vector2_param[0] );
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_3Dim(double *vector1_param, double *vector2_param, double *out_vector_param)
{
	out_vector_param[0] =  ( vector1_param[1]*vector2_param[2] - vector1_param[2]*vector2_param[1] );
	out_vector_param[1] = -( vector1_param[0]*vector2_param[2] - vector1_param[2]*vector2_param[0] );
	out_vector_param[2] =  ( vector1_param[0]*vector2_param[1] - vector1_param[1]*vector2_param[0] );
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_3Dim(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param)
{
	out_vector_param->at(0) =  ( vector1_param->at(1)*vector2_param->at(2) - vector1_param->at(2)*vector2_param->at(1) );
	out_vector_param->at(1) = -( vector1_param->at(0)*vector2_param->at(2) - vector1_param->at(2)*vector2_param->at(0) );
	out_vector_param->at(2) =  ( vector1_param->at(0)*vector2_param->at(1) - vector1_param->at(1)*vector2_param->at(0) );
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_3Dim(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param)
{
	out_vector_param->at(0) =  ( vector1_param->at(1)*vector2_param->at(2) - vector1_param->at(2)*vector2_param->at(1) );
	out_vector_param->at(1) = -( vector1_param->at(0)*vector2_param->at(2) - vector1_param->at(2)*vector2_param->at(0) );
	out_vector_param->at(2) =  ( vector1_param->at(0)*vector2_param->at(1) - vector1_param->at(1)*vector2_param->at(0) );
}
//void Vector_Cross_Product(float *vector1_param, float *vector2_param, float *out_vector_param);
//void Vector_Cross_Product(double *vector1_param, double *vector2_param, double *out_vector_param);
//void Vector_Cross_Product(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
//void Vector_Cross_Product(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
/*--- End Vector Cross Product ---*/
		
/*----- Vector Cross Product Normalize -----*/
void JMD::JMD_Utils_Math::Vector_Cross_Product_Normalize_3Dim(float *vector1_param, float *vector2_param, float *out_vector_param)
{
	//cross product
	Vector_Cross_Product_3Dim(vector1_param,vector2_param,out_vector_param);
	
	//normalize
	Vector_Normalize(out_vector_param,3);
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_Normalize_3Dim(double *vector1_param, double *vector2_param, double *out_vector_param)
{
	//cross product
	Vector_Cross_Product_3Dim(vector1_param,vector2_param,out_vector_param);
	
	//normalize
	Vector_Normalize(out_vector_param,3);
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_Normalize_3Dim(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param)
{
	//cross product
	Vector_Cross_Product_3Dim(vector1_param,vector2_param,out_vector_param);
	
	//normalize
	Vector_Normalize(out_vector_param);
}
void JMD::JMD_Utils_Math::Vector_Cross_Product_Normalize_3Dim(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param)
{
	//cross product
	Vector_Cross_Product_3Dim(vector1_param,vector2_param,out_vector_param);
	
	//normalize
	Vector_Normalize(out_vector_param);
}
//void Vector_Cross_Product_Normalize(float *vector1_param, float *vector2_param, float *out_vector_param);
//void Vector_Cross_Product_Normalize(double *vector1_param, double *vector2_param, double *out_vector_param);
//void Vector_Cross_Product_Normalize(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
//void Vector_Cross_Product_Normalize(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
/*--- End Vector Cross Product Normalize ---*/
		
/*------------- End Vector Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
