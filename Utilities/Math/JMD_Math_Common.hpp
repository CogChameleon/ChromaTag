#pragma once
#ifndef JMD_UTILS_MATH_H_
#define JMD_UTILS_MATH_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Math: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Math.hpp
 A class for math
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 --- End Enumerations ---
 
 ----- Constructors -----
 
 Math();
 
 --- end Constructors ---
 
 
 ----- Linear Algebra Methods -----
 	
 
 --- end Linear Algebra Methods ---
 
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
#include <cmath>
#include <array>
#include <vector>
#include <utility>

//utilities
#include "JMD_Utils_Defines.hpp"
#include "JMD_Utils_Writer.hpp"

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//----------------------- Namespaces ----------------------------//
//---------------------------------------------------------------//

using namespace std;

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
    //==================== JMD Utils Math Definition =====================//
    //====================================================================//
    //====================================================================//
    class JMD_Utils_Math
    {
        
        /*----- Enumerations -----*/
        
        /*--- End Enumerations ---*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
		JMD_Utils_Writer myWriter;
		
        /*------------- End Variables -------------*/
        
        /*--------------- Helpers ---------------*/
        
        
        /*------------- End Helpers -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
		/*--------------- Constructors ---------------*/
		JMD_Utils_Math();
		/*------------- end Constructors -------------*/
		
		
		/*--------------- Destructors ---------------*/
		~JMD_Utils_Math();
		/*------------- end Destructors ------------*/
		
		
		/*--------------- General ---------------*/
		
		/*----- Degrees To Radians -----*/
		// converts degrees to radians
		//
		// input:
		//    degree_param - input degrees
		// output:
		//    double - converted to radians
		double DegreesToRadians(double degree_param);
		/*--- End Degrees To Radians ---*/
		
		/*----- Radians To Degrees -----*/
		// converts radians to degrees
		//
		// input:
		//    radian_param - input radians
		// output:
		//    double - converted to degrees
		double RadiansToDegrees(double radian_param);
		/*--- End Radians To Degrees ---*/
		
		/*----- Factorial -----*/
		// takes the factorial of a number.
		// e.g. 5! = 5 * 4 * 3 * 2 * 1.
		// however, can be stopped earlier than 1;
		// e.g. Factorial(5,3) = 5 * 4 * 3
		// this function doesn't check to see if the
		// parameters make sense; e.g. the stop value
		// should not be larger than the factorial number
		//
		// input:
		//    num_param - starting value of factorial
		//    stop_param - stops calculation early, default is 1
		// output:
		//    unsigned int - factorial result
		unsigned int Factorial(unsigned int num_param, unsigned int stop_param = 2);
		/*--- End Factorial ---*/
		
		/*----- NChooseK -----*/
		// calculates n choose k:
		//  n! / ( k! * (n-k)! )
		// this function doesn't check to see if
		// n and k make sense, so input to this funciton
		// should be such that n >= k
		//
		// input:
		//    n_param - n
		//    k_param - k
		// output:
		//    unsigned int - n choose k result
		unsigned int NChooseK(unsigned int n_param, unsigned int k_param);
		/*--- End NChooseK ---*/
		
		/*----- Modulo -----*/
		// calculates the equivalent of python modulu
		// the only time this differs from the normal c % operator
		// is for negative numbers.
		//
		// input:
		//    n_param - n % M
		//    M_param - n % M
		// output:
		//    int - integer modulo
		int Modulo(int n_param, int M_param);
		double Modulo(double n_param, double M_param);
		/*--- End Modulo ---*/
		
		/*------------- End General -------------*/
		
		
		/*--------------- Vector Methods ---------------*/
		
		/*----- Vector Length -----*/
		// returns the length of the vector with L2-norm
		//
		// input:
		//    vector_param - pointer to vector (double,float)
		//    size_param - number of elements in vector
		// output:
		//    length - length of vector
		float  Vector_Length(float *vector_param, unsigned int size_param);
		double Vector_Length(double *vector_param, unsigned int size_param);
		float  Vector_Length(vector<float> *vector_param);
		double Vector_Length(vector<double> *vector_param);
		/*--- End Vector Length ---*/
		
		/*----- Vector Norm -----*/
		// returns the norm of the vector
		//
		// input:
		//    vector_param - pointer to vector (double,float)
		//    size_param - number of elements in vector
		//    norm_param - L-<norm_param> to calculate
		// output:
		//    norm - L-<norm_param> norm of vector
		float  Vector_Norm(float *vector_param, unsigned int size_param, unsigned int norm_param);
		double Vector_Norm(double *vector_param, unsigned int size_param, unsigned int norm_param);
		float  Vector_Norm(vector<float> *vector_param, unsigned int norm_param);
		double Vector_Norm(vector<double> *vector_param, unsigned int norm_param);
		/*--- End Vector Norm ---*/
		
		/*----- Vector Normalize -----*/
		// normalizes the vector
		//
		// input:
		//    vector_param - pointer to vector (double,float)
		//    size_param - number of elements in vector
		void Vector_Normalize(float *vector_param, unsigned int size_param);
		void Vector_Normalize(double *vector_param, unsigned int size_param);
		void Vector_Normalize(vector<float> *vector_param);
		void Vector_Normalize(vector<double> *vector_param);
		/*--- End Vector Normalize ---*/
		
		/*----- Vector Dot Product -----*/
		// returns the dot product of two vectors
		//
		// input:
		//    vector1_param - pointer to vector A (double,float)
		//    vector2_param - pointer to vector B (double,float)
		//    vector1_size_param - size of vector 1
		//    vector2_size_param - size of vector 2
		// output:
		//    AdotB - A . B
		float  Vector_Dot_Product(float *vector1_param, float *vector2_param, int vector1_size_param, int vector2_size_param);
		float  Vector_Dot_Product(vector<float> *vector1_param, vector<float> *vector2_param);
		double Vector_Dot_Product(double *vector1_param, double *vector2_param, int vector1_size_param, int vector2_size_param);
		double Vector_Dot_Product(vector<double> *vector1_param, vector<double> *vector2_param);
		/*--- End Vector Dot Product ---*/
		
		/*----- Vector Cross Product -----*/
		// computes the cross product of two vectors and stores the 
		// result in a third vector
		//
		// input:
		//    vector1_param - pointer to vector A (double,float)
		//    vector2_param - poitner to vector B (double,float)
		//    out_vector_param - A X B
		void Vector_Cross_Product_3Dim(float *vector1_param, float *vector2_param, float *out_vector_param);
		void Vector_Cross_Product_3Dim(double *vector1_param, double *vector2_param, double *out_vector_param);
		void Vector_Cross_Product_3Dim(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
		void Vector_Cross_Product_3Dim(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
		//void Vector_Cross_Product(float *vector1_param, float *vector2_param, float *out_vector_param);
		//void Vector_Cross_Product(double *vector1_param, double *vector2_param, double *out_vector_param);
		//void Vector_Cross_Product(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
		//void Vector_Cross_Product(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
		/*--- End Vector Cross Product ---*/
		
		/*----- Vector Cross Product Normalize -----*/
		// computes the cross product of two vectors and stores the 
		// normalized result in a third vector
		//
		// input:
		//    vector1_param - pointer to vector A (double,float)
		//    vector2_param - poitner to vector B (double,float)
		//    out_vector_param - normalized A X B
		void Vector_Cross_Product_Normalize_3Dim(float *vector1_param, float *vector2_param, float *out_vector_param);
		void Vector_Cross_Product_Normalize_3Dim(double *vector1_param, double *vector2_param, double *out_vector_param);
		void Vector_Cross_Product_Normalize_3Dim(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
		void Vector_Cross_Product_Normalize_3Dim(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
		//void Vector_Cross_Product_Normalize(float *vector1_param, float *vector2_param, float *out_vector_param);
		//void Vector_Cross_Product_Normalize(double *vector1_param, double *vector2_param, double *out_vector_param);
		//void Vector_Cross_Product_Normalize(vector<float> *vector1_param, vector<float> *vector2_param, vector<float> *out_vector_param);
		//void Vector_Cross_Product_Normalize(vector<double> *vector1_param, vector<double> *vector2_param, vector<double> *out_vector_param);
		/*--- End Vector Cross Product Normalize ---*/
		
		/*------------- End Vector Methods -------------*/
        
        
        /*--------------- Matrix Methods ---------------*/
        
        /*----- Matrix Transpose-----*/
        /*--- End Matrix Tranpose ---*/
        
        /*----- Matrix Inverse -----*/
        /*--- End Matrix Inverse ---*/
        
        /*----- Matrix Determinant -----*/
		/*--- End Matrix Determinant ---*/
        
        /*------------- End Matrix Methods -------------*/
        
        
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

