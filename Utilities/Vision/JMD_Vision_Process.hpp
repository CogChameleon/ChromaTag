#pragma once
#ifndef JMD_VISION_PROCESS_H_
#define JMD_VISION_PROCESS_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD Vision Process: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Process.hpp
 A class for processing images
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
#include <cmath>
#include <algorithm>
#include <vector>

//utilities
#include "JMD_Utils_Defines.hpp"

//---------------------------------------------------------------//
//------------------------ end Includes -------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//------------------------- Macros ------------------------------//
//---------------------------------------------------------------//

//Clip
#define CLIP_CHAR(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define CLIP_DOUB(X) ( (X) > 1.0 ? 1.0 : (X) < 0.0 ? 0.0 : X)

// RGB ==> Gray
#define DEF_RGB2GRAY_LUM_REDSCALE 0.2989
#define DEF_RGB2GRAY_LUM_GREENSCALE 0.5870
#define DEF_RGB2GRAY_LUM_BLUESCALE 0.1140

// RGB ==> Linear RGB
#define DEF_LINRGB_GAMMA        2.4
#define DEF_LINRGB_NONLIN_SCALE 0.9479
#define DEF_LINRGB_NONLIN_SHIFT 0.0521
#define DEF_LINRGB_LIN_THRESH   0.04045
#define DEF_LINRGB_LIN_SCALE    0.0774

// RGB ==> YUV
#define YUV_Y_R_CHAR 66
#define YUV_Y_G_CHAR 129
#define YUV_Y_B_CHAR 25
#define YUV_U_R_CHAR -38
#define YUV_U_G_CHAR -74
#define YUV_U_B_CHAR 112
#define YUV_V_R_CHAR 112
#define YUV_V_G_CHAR -94
#define YUV_V_B_CHAR -18
#define RGB2YUV_Y_CHAR(R, G, B) CLIP_CHAR(( ( YUV_Y_R_CHAR * (R)  +  YUV_Y_G_CHAR * (G)  +  YUV_Y_B_CHAR * (B)  +  128) >> 8)  +   16)
#define RGB2YUV_U_CHAR(R, G, B) CLIP_CHAR(( ( YUV_U_R_CHAR * (R)  +  YUV_U_G_CHAR * (G)  +  YUV_U_B_CHAR * (B)  +  128) >> 8)  +  128)
#define RGB2YUV_V_CHAR(R, G, B) CLIP_CHAR(( ( YUV_V_R_CHAR * (R)  +  YUV_V_G_CHAR * (G)  +  YUV_V_B_CHAR * (B)  +  128) >> 8)  +  128)
#define YUV_Y_R_DOUB 0.299
#define YUV_Y_G_DOUB 0.587
#define YUV_Y_B_DOUB 0.114
#define YUV_U_R_DOUB -0.147
#define YUV_U_G_DOUB -0.289
#define YUV_U_B_DOUB 0.436
#define YUV_V_R_DOUB 0.615
#define YUV_V_G_DOUB -0.515
#define YUV_V_B_DOUB -0.100
#define RGB2YUV_Y_DOUB(R, G, B) YUV_Y_R_DOUB * (R)  +  YUV_Y_G_DOUB * (G)  +  YUV_Y_B_DOUB * (B)
#define RGB2YUV_U_DOUB(R, G, B) YUV_U_R_DOUB * (R)  +  YUV_U_G_DOUB * (G)  +  YUV_U_B_DOUB * (B)
#define RGB2YUV_V_DOUB(R, G, B) YUV_V_R_DOUB * (R)  +  YUV_V_G_DOUB * (G)  +  YUV_V_B_DOUB * (B)

// YUV ==> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R_CHAR(Y, U, V) CLIP_CHAR(( 298 * C(Y)                 +  409 * E(V)  +  128)  >>  8)
#define YUV2G_CHAR(Y, U, V) CLIP_CHAR(( 298 * C(Y)  -  100 * D(U)  -  208 * E(V)  +  128)  >>  8)
#define YUV2B_CHAR(Y, U, V) CLIP_CHAR(( 298 * C(Y)  +  516 * D(U)                 +  128)  >>  8)

// RGB ==> XYZ
#define XYZ_X_R_D65_DOUB 0.4124564
#define XYZ_X_G_D65_DOUB 0.3575761
#define XYZ_X_B_D65_DOUB 0.1804375
#define XYZ_Y_R_D65_DOUB 0.2126729
#define XYZ_Y_G_D65_DOUB 0.7151522
#define XYZ_Y_B_D65_DOUB 0.0721750
#define XYZ_Z_R_D65_DOUB 0.0193339
#define XYZ_Z_G_D65_DOUB 0.1191920
#define XYZ_Z_B_D65_DOUB 0.9503041
#define LINRGB2XYZ_X_D65_DOUB(R, G, B) XYZ_X_R_D65_DOUB * (R)  +  XYZ_X_G_D65_DOUB * (G)  +  XYZ_X_B_D65_DOUB * (B)
#define LINRGB2XYZ_Y_D65_DOUB(R, G, B) XYZ_Y_R_D65_DOUB * (R)  +  XYZ_Y_G_D65_DOUB * (G)  +  XYZ_Y_B_D65_DOUB * (B)
#define LINRGB2XYZ_Z_D65_DOUB(R, G, B) XYZ_Z_R_D65_DOUB * (R)  +  XYZ_Z_G_D65_DOUB * (G)  +  XYZ_Z_B_D65_DOUB * (B)
#define XYZ_X_R_D50_DOUB 0.4360747
#define XYZ_X_G_D50_DOUB 0.3850649
#define XYZ_X_B_D50_DOUB 0.1430804
#define XYZ_Y_R_D50_DOUB 0.2225045
#define XYZ_Y_G_D50_DOUB 0.7168786
#define XYZ_Y_B_D50_DOUB 0.0606169
#define XYZ_Z_R_D50_DOUB 0.0139322
#define XYZ_Z_G_D50_DOUB 0.0971045
#define XYZ_Z_B_D50_DOUB 0.7141733
#define LINRGB2XYZ_X_D50_DOUB(R, G, B) XYZ_X_R_D50_DOUB * (R)  +  XYZ_X_G_D50_DOUB * (G)  +  XYZ_X_B_D50_DOUB * (B)
#define LINRGB2XYZ_Y_D50_DOUB(R, G, B) XYZ_Y_R_D50_DOUB * (R)  +  XYZ_Y_G_D50_DOUB * (G)  +  XYZ_Y_B_D50_DOUB * (B)
#define LINRGB2XYZ_Z_D50_DOUB(R, G, B) XYZ_Z_R_D50_DOUB * (R)  +  XYZ_Z_G_D50_DOUB * (G)  +  XYZ_Z_B_D50_DOUB * (B)

//RGB ==> LAB
#define WP_D65_X 1.0 / 0.9504
#define WP_D65_Y 1.0 / 1.0000
#define WP_D65_Z 1.0 / 1.0889
#define XYZ2WP_X_D65_DOUB(X) WP_D65_X * (X)
#define XYZ2WP_Y_D65_DOUB(Y) WP_D65_Y * (Y)
#define XYZ2WP_Z_D65_DOUB(Z) WP_D65_Z * (Z)
#define WP_D50_X 1.0 / 0.9642
#define WP_D50_Y 1.0 / 1.0000
#define WP_D50_Z 1.0 / 0.8251 
#define XYZ2WP_X_D50_DOUB(X) WP_D50_X * (X)
#define XYZ2WP_Y_D50_DOUB(Y) WP_D50_Y * (Y)
#define XYZ2WP_Z_D50_DOUB(Z) WP_D50_Z * (Z) 
#define DEF_LAB_GAMMA       1.0 / 3.0
#define DEF_LAB_LIN_THRESH  (6.0 * 6.0 * 6.0) / (29.0 * 29.0 * 29.0)
#define DEF_LAB_LIN_SCALE   24389.0 / 3132.0
#define DEF_LAB_LIN_SHIFT   16.0 / 116.0
#define GAMMAXYZWP2LAB_L_DOUB(Y)    (116.0 * Y) - 16.0
#define GAMMAXYZWP2LAB_A_DOUB(Y, X) 500.0 * ( X - Y )
#define GAMMAXYZWP2LAB_B_DOUB(Y, Z) 200.0 * ( Y - Z )

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
	struct JMD_Settings_VisionProcess; //see below
	/*------------- End Forward Declerations -------------*/
    
    //====================================================================//
    //====================================================================//
    //================== JMD Vision Process Definition ===================//
    //====================================================================//
    //====================================================================//
    class JMD_Vision_Process
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        
        /*----- RGB Color -----*/
        // There are many standard types for the RGB color space
        //
        // SRGB - srgb RGB color space
        // ADOBE_RGB - adobe 1998 RGB color space
        typedef enum RGB_Color { SRGB = 0, ADOBE_RGB } RGB_Color;
        /*--- End RGB Color ---*/
        
        /*----- White Point -----*/
        // There are several standards for the white point
        // that is used in XYZ and LAB color spaces
        //
        //
        typedef enum White_Point { D65 = -5, D50 } White_Point;
        /*--- End White Point ---*/
        
        /*------------- End Enumerations -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
        /*----- Settings -----*/

        // General
        JMD::JMD_Vision_Process::RGB_Color      myRGB_InColor;
        
        // RGB <==> Linear RGB
        double                                  myGamma_RGB2LRGB;
        double                                  myNonLinScale_RGB2LRGB;
        double                                  myNonLinShift_RGB2LRGB;
        double                                  myLinThresh_RGB2LRGB;
        double                                  myLinScale_RGB2LRGB;
    
        // RGB <==> XYZ
        bool                                    myLinearizeRGB_RGB2XYZ;
        JMD::JMD_Vision_Process::White_Point    myWhitePoint_RGB2XYZ;
            
        // RGB <==> LAB
        double                                  myGamma_XYZ2LAB;
        double                                  myLinThresh_XYZ2LAB;
        double                                  myLinScale_XYZ2LAB;
        double                                  myLinShift_XYZ2LAB;
        
        // 
        
        /*--- End Settings ---*/
        
        /*------------- End Variables -------------*/
        
        
        /*--------------- General ---------------*/
        
        void Parse_Settings(JMD_Settings_VisionProcess *settings_param = nullptr);
        
        /*------------- End General -------------*/
        
        
        /*--------------- Gradient ---------------*/
        // bins the gradient directions for speed
        //
        // input:
        //    iy_param - Iy = (v1 - v2)
        //    ix_param - Ix = (u1 - u2)
        // output:
        //    double - 0-360 degrees, bins every 5 degrees
        double Private_Est_ATAN2(double iy_param, double ix_param);
        /*------------- End Gradient -------------*/
        
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
        //    settings_param - settings for vision process
        JMD_Vision_Process(JMD_Settings_VisionProcess *settings_param = nullptr);
		/*------------- end Constructors -------------*/
		
		
		/*--------------- Destructors ---------------*/
		~JMD_Vision_Process();
		/*------------- end Destructors ------------*/
		
        
        
        /*--------------- Convert RGB ==> Gray ---------------*/
        
        /*----- RGBToGray_Lightness -----*/
        // converts RGB to gray using lightness method:
        // (max(R,G,B) + min(R,G,B)) / 2
        //
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        // output:
        //    unsigned char - gray pixel value
        unsigned char RGBToGray_Lightness_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param);
        /*--- End RGBToGray_Lightness ---*/
        
        /*----- RGBToGray_Average -----*/
        // converts RGB to gray using average method:
        // (R+G+B) / 3
        //
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        // output:
        //    unsigned char - gray pixel value
        unsigned char RGBToGray_Average_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param);
        /*--- End RGBToGray_Average ---*/
        
        /*----- RGBToGray_Luminosity -----*/
        // converts RGB to gray using luminosity method:
        // 0.21*R  +  0.72*G  +  0.07*B
        //
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        //    red_scale_param - value to scale red
        //    green_scale_param - value to scale green
        //    blue_scale_param - value to scale blue
        // output:
        //    unsigned char - gray pixel value
        unsigned char RGBToGray_Luminosity_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param);
        /*--- End RGBToGray_Luminosity ---*/
        
        /*------------- End Convert RGB ==> Gray -------------*/
        
        
        
        /*--------------- Convert RGB <==> YUV/YCbCr ---------------*/
        
        /*----- RGBToLinearRGB -----*/
        // converts RGB to linear RGB using the approach:
        // if    value > TH, value = (A*value + B)^G
        // else              value = (C*value + D)^F
        // 
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        //    settings_param - settings used for linearizing a pixel
        // output:
        //    lin_red_param - red pixel value
        //    lin_green_param - green pixel value
        //    lin_blue_param - blue pixel value
        void RGBToLinearRGB_Pixel(double red_param, double green_param, double blue_param, double *lin_red_param = nullptr, double *lin_green_param = nullptr, double *lin_blue_param = nullptr);
        double RGBToLinearRGB_R_Pixel(double red_param);
        double RGBToLinearRGB_G_Pixel(double gre_param);
        double RGBToLinearRGB_B_Pixel(double blu_param);
        /*--- End RGBToLinearRGB ---*/
        
        /*----- RGBToYUV_Pixel -----*/
        // converts an RGB pixel to a YUV pixel. Double pixels assumed to range
        // between 0 and 1.
        //
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        // output:
        //    Y_param - pointer to output Y value
        //    U_param - pointer to output U value
        //    V_param - pointer to output V value
        void RGBToYUV_Pixel(double red_param, double green_param, double blue_param, double *Y_param = nullptr, double *U_param = nullptr, double *V_param = nullptr);
        double RGBToYUV_Y_Pixel(double red_param, double green_param, double blue_param);
        double RGBToYUV_U_Pixel(double red_param, double green_param, double blue_param);
        double RGBToYUV_V_Pixel(double red_param, double green_param, double blue_param);
        //
        void RGBToYUV_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param, unsigned char *Y_param = nullptr, unsigned char *U_param = nullptr, unsigned char *V_param = nullptr);
        unsigned char RGBToYUV_Y_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        unsigned char RGBToYUV_U_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        unsigned char RGBToYUV_V_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        /*--- End RGBToYUV_Pixel ---*/
        
        /*----- YUVToRGB_Pixel -----*/
        // converts a YUV pixel to an RGB pixel.Double pixels assumed to range
        // between 0 and 1.
        //
        // input:
        //    Y_param - Y pixel value
        //    U_param - U pixel value
        //    V_param - V pixel value
        // output:
        //    red_param - pointer to red pixel value
        //    green_param - pointer to green pixel value
        //    blue_param - pointer to blue pixel value
        void YUVToRGB_Pixel(double Y_param, double U_param, double V_param, double *red_param = nullptr, double *green_param = nullptr, double *blue_param = nullptr);
        void YUVToRGB_Pixel(unsigned char Y_param, unsigned char U_param, unsigned char V_param, unsigned char *red_param = nullptr, unsigned char *green_param = nullptr, unsigned char *blue_param = nullptr);
        /*--- End YUVToRGB_Pixel ---*/
        
        /*----- RGB2YCbCr_Pixel -----*/
        /*--- End RGB2YCbCr_Pixel ---*/
        
        /*------------- End Convert RGB to YUV/YCbCr -------------*/
        
        
        
        /*--------------- Convert RGB <==> XYZ ---------------*/
        
        /*----- RGBToXYZ_Pixel -----*/
        // converts an RGB pixel to an XYZ pixel. Double Images should range between 0 and 1
        //
        // input:
        //    red_param - red pixel value
        //    green_param - green pixel value
        //    blue_param - blue pixel value
        // output:
        //    X_param - pointer to output Y value
        //    Y_param - pointer to output U value
        //    Z_param - pointer to output V value
        void RGBToXYZ_Pixel(double red_param, double green_param, double blue_param, double *X_param = nullptr, double *Y_param = nullptr, double *Z_param = nullptr);
        void RGBToXYZ_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param, double *X_param = nullptr, double *Y_param = nullptr, double *Z_param = nullptr);
        void RGBToXYZ_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param, unsigned int *X_param = nullptr, unsigned int *Y_param = nullptr, unsigned int *Z_param = nullptr);
        double LinearRGBToXYZ_X_D65_Pixel(double red_param, double green_param, double blue_param);
        double LinearRGBToXYZ_Y_D65_Pixel(double red_param, double green_param, double blue_param);
        double LinearRGBToXYZ_Z_D65_Pixel(double red_param, double green_param, double blue_param);
        double LinearRGBToXYZ_X_D50_Pixel(double red_param, double green_param, double blue_param);
        double LinearRGBToXYZ_Y_D50_Pixel(double red_param, double green_param, double blue_param);
        double LinearRGBToXYZ_Z_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_X_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_Y_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_Z_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_X_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_Y_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToXYZ_Z_D50_Pixel(double red_param, double green_param, double blue_param);
        /*--- End RGBToXYZ_Pixel ---*/
        
        /*--------------- Convert RGB <==> XYZ ---------------*/
        
        
        
        /*--------------- Convert RGB <==> LAB ---------------*/
        
        /*----- RGB2LAB_Pixel -----*/
        // converts an RGB pixel to a LAB pixel. Double images should range between 0 and 1
        //
        // input:
        // output:
        void RGBToLAB_Pixel(double red_param, double green_param, double blue_param, double *L_param = nullptr, double *A_param = nullptr, double *B_param = nullptr);
        //void RGBToLAB_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param, double *L_param = nullptr, double *A_param = nullptr, double *B_param = nullptr);
        double XYZToWP_X_D65_Pixel(double X_param);
        double XYZToWP_Y_D65_Pixel(double Y_param);
        double XYZToWP_Z_D65_Pixel(double Z_param);
        double XYZToWP_X_D50_Pixel(double X_param);
        double XYZToWP_Y_D50_Pixel(double Y_param);
        double XYZToWP_Z_D50_Pixel(double Z_param);
        double XYZWPToGammaXYZWP_X_Pixel(double X_param);
        double XYZWPToGammaXYZWP_Y_Pixel(double Y_param);
        double XYZWPToGammaXYZWP_Z_Pixel(double Z_param);
        double GammaXYZWPToLAB_L_Pixel(double Y_param);
        double GammaXYZWPToLAB_A_Pixel(double Y_param, double X_param);
        double GammaXYZWPToLAB_B_Pixel(double Y_param, double Z_param);
        double RGBToLAB_L_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_A_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_B_D65_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_L_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        double RGBToLAB_A_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        double RGBToLAB_B_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        double RGBToLAB_L_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_A_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_B_D50_Pixel(double red_param, double green_param, double blue_param);
        double RGBToLAB_L_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        double RGBToLAB_A_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        double RGBToLAB_B_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param);
        /*--- End RGB2LAB_Pixel ---*/
        
        /*------------- End Convert RGB to LAB -------------*/
        
        

        /*--------------- Gradient ---------------*/
        
        /*------------- End Gradient -------------*/
        
        
        
        /*--------------- Utility Methods ---------------*/
        
        /*----- DoubleToChar_Pixel -----*/
        // converts a double pixel to a char pixel
        // double pixels are assumed to be between 0-1
        // char pixels are assumed to be between 0-255
        //
        // input:
        //    double_red(green,blue)_param - double pixel value
        // output::
        //    char_red(green,blue)_param - char pixel value
        void DoubleToChar_Pixel(double double_red_param, double double_green_param, double double_blue_param, unsigned char *char_red_param = nullptr, unsigned char *char_green_param = nullptr, unsigned char *char_blue_param = nullptr);
        void DoubleToChar_Pixel(double double_gray_param, unsigned char *char_gray_param);
        /*--- End DoubleToChar_Pixel ---*/

        /*----- CharToDouble_Pixel -----*/
        // converts a char pixel to a double pixel
        // double pixels are assumed to be between 0-1
        // char pixels are assumed to be between 0-255
        //
        // input:
        //    char_red(green,blue)_param - char pixel value
        // output::
        //    double_red(green,blue)_param - double pixel value
        void CharToDouble_Pixel(unsigned char char_red_param, unsigned char char_green_param, unsigned char char_blue_param, double *double_red_param = nullptr, double *double_green_param = nullptr, double *double_blue_param = nullptr);
        void CharToDouble_Pixel(unsigned char char_gray_param, double *double_gray_param);
        /*--- End CharToDouble_Pixel ---*/
        
        /*----- Linearize_Pixel -----*/
        // linearizes pixel using the following approach:
        // if    value > TH, value = (A*value + B)^G
        // else              value = (C*value + D)^H
        //
        // input:
        //    value - value to linearize
        //    A_parma - (A*value + B)^G
        //    B_parma - (A*value + B)^G
        //    G_param - (A*value + B)^G
        //    C_param - (C*value + D)^H
        //    D_param - (D*value + D)^H
        //    H_param - (D*value + D)^H
        //    TH_param - threshold for two parts
        // output:
        //    double - linearized value
        double Linearize_Pixel(double value_param, double A_param, double B_param, double G_param, double C_param, double D_param, double H_param, double TH_param);
        /*--- End Linearize_Pixel ---*/
        
        /*----- Gradient_Pixel -----*/
        // computes the gradient for a pixel at some (u,v)
        // with neighboring pixels u1,u2,v1,v2
        // magnitude = sqrt( (u1-u2)^2 + (v1-v2)^2 )
        // dir = atan2(v1-v2,u1-u2)
        //
        // input:
        //    u1_param - (u-1,v) pixel value
        //    u2_param - (u+1,v) pixel value
        //    v1_param - (u,v-1) pixel value
        //    v2_param - (u,v+1) pixel value
        // output:
        //    mag_param - pointer to magnitude value
        //    dir_param - pointer to direction value
        void Gradient_Pixel(double u1_param, double u2_param, double v1_param, double v2_param, double *Ix_param = nullptr, double *Iy_param = nullptr, double *mag_param = nullptr, double *dir_param = nullptr);
        void Gradient_Pixel(unsigned char u1_param, unsigned char u2_param, unsigned char v1_param, unsigned char v2_param, unsigned char *Ix_param, unsigned char *Iy_param, unsigned char *mag_param = nullptr, unsigned char *dir_param = nullptr);
        //
        // Estimated Gradient doesn't compute sqrts or squares
        // and uses a look up table to approximate atan2
        void Gradient_Pixel_Est(double u1_param, double u2_param, double v1_param, double v2_param, double *Ix_param = nullptr, double *Iy_param = nullptr, double *mag_param = nullptr, double *dir_param = nullptr);
        void Gradient_Pixel_Est(unsigned char u1_param, unsigned char u2_param, unsigned char v1_param, unsigned char v2_param, unsigned char *Ix_param, unsigned char *Iy_param, unsigned char *mag_param = nullptr, unsigned char *dir_param = nullptr);
        double Gradient_Pixel_Ix_Est(double u1_param, double u2_param);
        double Gradient_Pixel_Iy_Est(double v1_param, double v2_param);
        unsigned char Gradient_Pixel_Ix_Est(unsigned char u1_param, unsigned char u2_param);
        unsigned char Gradient_Pixel_Iy_Est(unsigned char v1_param, unsigned char v2_param);
        /*--- End Gradient_Pixel ---*/
        
        /*------------- End Utility Methods -------------*/
        
        
        
        /*--------------- Setters/Getters ---------------*/
        // setters for parameters, see the settings for
        // information on the options
        
        /*----- General -----*/
        
        // RGBColor_RGB2XYZ
        void RGB_InColor(JMD::JMD_Vision_Process::RGB_Color rgbincolor_param);
        void RGB_InColor(JMD::JMD_Vision_Process::RGB_Color *rgbincolor_param);
        
        /*--- End General ---*/
        
        
        /*----- RGB <==> LRGB -----*/
        // Gamma_RGB2LRGB
        void Gamma_RGB2LRGB(double gamma_param);
        void Gamma_RGB2LRGB(double *gamma_param);
        
        // NonLinScale_RGB2LRGB
        void NonLinScale_RGB2LRGB(double nonlinscale_param);
        void NonLinScale_RGB2LRGB(double *nonlinscale_param);
        
        // NonLinShift_RGB2LRGB
        void NonLinShift_RGB2LRGB(double nonlinshift_param);
        void NonLinShift_RGB2LRGB(double *nonlinshift_param);
        
        // LinThresh_RGB2LRGB
        void LinThresh_RGB2LRGB(double linthresh_param);
        void LinThresh_RGB2LRGB(double *linthresh_param);
        
        // LinScale_RGB2LRGB
        void LinScale_RGB2LRGB(double linscale_param);
        void LinScale_RGB2LRGB(double *linscale_param);
        /*--- End RGB <==> LRGB ---*/
        
        
        /*----- RGB <==> XYZ -----*/
        
        // LinearizeRGB_RGB2XYZ
        void LinearizeRGB_RGB2XYZ(bool linearizergb_param);
        void LinearizeRGB_RGB2XYZ(bool *linearizergb_param);
        
        // WhitePoint_RGB2XYZ
        void WhitePoint_RGB2XYZ(JMD::JMD_Vision_Process::White_Point whitepoint_param);
        void WhitePoint_RGB2XYZ(JMD::JMD_Vision_Process::White_Point *whitepoint_param);
        
        /*--- End RGB <==> XYZ ---*/
        
        
        /*----- RGB <==> LAB -----*/
        /*--- End RGB <==> LAB ---*/
        
        /*------------- End Setters/Getters -------------*/
        
        
        
        /*--------------- Operator Overloads ---------------*/
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
    //============== JMD VisionProcess Settings Definition ===============//
    //====================================================================//
    //====================================================================//
    // Struct to hold settings for the Vision_Process class. This struct can be
    // passed to the ChromaTag class to update the settings.
    struct JMD_Settings_VisionProcess
    {
		/*---------- General ----------*/
        
        // RGB_InColor
        // type of RGBColor, e.g. sRGB or Adobe 1998
        JMD::JMD_Vision_Process::RGB_Color      myRGB_InColor;
        
        /*-------- End General --------*/
        
        
        /*---------- Convert RGB <==> Linear RGB ----------*/
        
        // these parameters define the behavior of the RGB <==> linear
        // RGB conversion. The linearization is done using the following
        // formula:
        // if value > Th,  value = (A*value + B)^G
        // else            value = C*value
        //
        // myGamma = G
        double                                  myGamma_RGB2LRGB;
        //
        // myNonLinScale = A
        double                                  myNonLinScale_RGB2LRGB;
        //
        // myNonLinShift = B
        double                                  myNonLinShift_RGB2LRGB;
        //
        // myLinThresh = Th
        double                                  myLinThresh_RGB2LRGB;
        //
        // myLinScale = C
        double                                  myLinScale_RGB2LRGB;
        
        /*-------- End Convert RGB <==> Linear RGB --------*/
        
        
        /*---------- Convert RGB <==> XYZ ----------*/
        
        // these parameters define the behavior of the RGB <==>
        // XYZ conversion.
        //
        // myLinearRGB - if true, will convert RGB ==> Linear RGB first
        //               and use the parameters defined above for RGB
        //               ==> Linear RGB for conversion
        bool                                    myLinearizeRGB_RGB2XYZ;
        //
        // myWhitePoint - standard used for conversion
        JMD::JMD_Vision_Process::White_Point    myWhitePoint_RGB2XYZ;
        
        /*-------- End Convert RGB <==> XYZ --------*/
        
        
        /*---------- Convert XYZ <==> LAB ----------*/
        
        // these parameters define the behavior of the XYZ <==>
        // LAB conversion. The gamma correction is done using the
        // following formula:
        // if value > Th,  value = (value)^G
        // else            value = C*value + D
        //
        // myGamma = G
        double                                  myGamma_XYZ2LAB;
        //
        // myLinThresh = Th
        double                                  myLinThresh_XYZ2LAB;
        //
        // myLinScale = C
        double                                  myLinScale_XYZ2LAB;
        //
        // myLinShift = D
        double                                  myLinShift_XYZ2LAB;
        
        /*-------- End Convert XYZ <==> LAB --------*/
		
		//set defaults
		JMD_Settings_VisionProcess() : 
            myRGB_InColor(          JMD::JMD_Vision_Process::RGB_Color::SRGB),
            myGamma_RGB2LRGB(       DEF_LINRGB_GAMMA),
            myNonLinScale_RGB2LRGB( DEF_LINRGB_NONLIN_SCALE),
            myNonLinShift_RGB2LRGB( DEF_LINRGB_NONLIN_SHIFT),
            myLinThresh_RGB2LRGB(   DEF_LINRGB_LIN_THRESH),
            myLinScale_RGB2LRGB(    DEF_LINRGB_LIN_SCALE),
            myLinearizeRGB_RGB2XYZ( true),
            myWhitePoint_RGB2XYZ(   JMD::JMD_Vision_Process::White_Point::D65),
            myGamma_XYZ2LAB(        DEF_LAB_GAMMA),
            myLinThresh_XYZ2LAB(    DEF_LAB_LIN_THRESH),
            myLinScale_XYZ2LAB(     DEF_LAB_LIN_SCALE),
            myLinShift_XYZ2LAB(     DEF_LAB_LIN_SHIFT)
		{ }
	};
    //====================================================================//
    //====================================================================//
    //============== JMD VisionProcess Settings Definition ===============//
    //====================================================================//
    //====================================================================//
    
}//end namespace JMD


#endif

