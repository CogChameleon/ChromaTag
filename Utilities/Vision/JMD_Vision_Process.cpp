/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD Vision Process: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Process.cpp
 A class for processing images
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

#include "JMD_Vision_Process.hpp"

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
//============ JMD Class Syntax Template Implementation ==============//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//

/*--------------- General ---------------*/
void JMD::JMD_Vision_Process::Parse_Settings(JMD_Settings_VisionProcess *settings_param)
{
	/*----- Defaults -----*/
	
	// General
	myRGB_InColor          = JMD::JMD_Vision_Process::RGB_Color::SRGB;
	
	// RGB <==> Linear RGB
	myGamma_RGB2LRGB       = DEF_LINRGB_GAMMA;
	myNonLinScale_RGB2LRGB = DEF_LINRGB_NONLIN_SCALE;
	myNonLinShift_RGB2LRGB = DEF_LINRGB_NONLIN_SHIFT;
	myLinThresh_RGB2LRGB   = DEF_LINRGB_LIN_THRESH;
	myLinScale_RGB2LRGB    = DEF_LINRGB_LIN_SCALE;
	
	// RGB <==> XYZ
	myLinearizeRGB_RGB2XYZ = true;
	myWhitePoint_RGB2XYZ   = JMD::JMD_Vision_Process::White_Point::D65;
	
	// XYZ <==> LAB
	myGamma_XYZ2LAB        = DEF_LAB_GAMMA;
    myLinThresh_XYZ2LAB    = DEF_LAB_LIN_THRESH;
    myLinScale_XYZ2LAB     = DEF_LAB_LIN_SCALE;
    myLinShift_XYZ2LAB     = DEF_LAB_LIN_SHIFT;
	
	/*--- End Defautls ---*/
	
	/*----- Change Settings -----*/
	if(settings_param)
	{
		// General
		myRGB_InColor          = settings_param -> myRGB_InColor;
		
		//RGB <==> Linear RGB
		myGamma_RGB2LRGB       = settings_param -> myGamma_RGB2LRGB;
		myNonLinScale_RGB2LRGB = settings_param -> myNonLinScale_RGB2LRGB;
		myNonLinShift_RGB2LRGB = settings_param -> myNonLinShift_RGB2LRGB;
		myLinThresh_RGB2LRGB   = settings_param -> myLinThresh_RGB2LRGB;
		myLinScale_RGB2LRGB    = settings_param -> myLinScale_RGB2LRGB;
	
		//RGB <==> XYZ
		myLinearizeRGB_RGB2XYZ = settings_param -> myLinearizeRGB_RGB2XYZ;
		myWhitePoint_RGB2XYZ   = settings_param -> myWhitePoint_RGB2XYZ;
		
		//XYZ <==> LAB
		myGamma_XYZ2LAB        = settings_param -> myGamma_XYZ2LAB;
    	myLinThresh_XYZ2LAB    = settings_param -> myLinThresh_XYZ2LAB;
    	myLinScale_XYZ2LAB     = settings_param -> myLinScale_XYZ2LAB;
    	myLinShift_XYZ2LAB     = settings_param -> myLinShift_XYZ2LAB;
	}
	/*--- End Change Settings ---*/
}
/*------------- End General -------------*/

/*--------------- Gradient ---------------*/
double JMD::JMD_Vision_Process::Private_Est_ATAN2(double iy_param, double ix_param)
{
	return 0;
}
/*------------- End Gradient -------------*/

//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_Vision_Process::JMD_Vision_Process(JMD_Settings_VisionProcess *settings_param) 
{
	//parse settings
	Parse_Settings(settings_param);
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_Vision_Process::~JMD_Vision_Process() { }
/*------------- End Destructors -------------*/

    

/*--------------- Convert RGB ==> Gray ---------------*/
        
/*----- RGBToGray_Lightness -----*/
unsigned char JMD::JMD_Vision_Process::RGBToGray_Lightness_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param) { return ( std::min({red_param, green_param, blue_param}) + std::max({red_param, green_param, blue_param}) ) * 0.5; }
/*--- End RGBToGray_Lightness ---*/

/*----- RGBToGray_Average -----*/
unsigned char JMD::JMD_Vision_Process::RGBToGray_Average_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param) { return (red_param + green_param + blue_param) * 0.333; }
/*--- End RGBToGray_Average ---*/

/*----- RGBToGray_Luminosity -----*/
unsigned char JMD::JMD_Vision_Process::RGBToGray_Luminosity_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param) { return DEF_RGB2GRAY_LUM_REDSCALE*red_param + DEF_RGB2GRAY_LUM_GREENSCALE*green_param + DEF_RGB2GRAY_LUM_BLUESCALE*blue_param; }
/*--- End RGBToGray_Luminosity ---*/

/*------------- End Convert RGB ==> Gray -------------*/
		
		
		
/*--------------- Convert RGB <==> Linear RGB ---------------%

/*----- RGBToLinearRGB_Pixel -----*/
void JMD::JMD_Vision_Process::RGBToLinearRGB_Pixel(double red_param, double green_param, double blue_param, double *lin_red_param, double *lin_green_param, double *lin_blue_param)
{
	// defaults
	double G  = myGamma_RGB2LRGB;
	double A  = myNonLinScale_RGB2LRGB;
	double B  = myNonLinShift_RGB2LRGB;
	double C  = myLinScale_RGB2LRGB;
	double Th = myLinThresh_RGB2LRGB;
	
	// set pixels
	if (lin_red_param)   { *lin_red_param   = RGBToLinearRGB_R_Pixel(red_param   ); }
	if (lin_green_param) { *lin_green_param = RGBToLinearRGB_G_Pixel(green_param ); }
	if (lin_blue_param)  { *lin_blue_param  = RGBToLinearRGB_B_Pixel(blue_param  ); }
}
double JMD::JMD_Vision_Process::RGBToLinearRGB_R_Pixel(double red_param) { return (red_param > myLinThresh_RGB2LRGB ? std::pow(myNonLinScale_RGB2LRGB * red_param   + myNonLinShift_RGB2LRGB, myGamma_RGB2LRGB) : myLinScale_RGB2LRGB * red_param); }
double JMD::JMD_Vision_Process::RGBToLinearRGB_G_Pixel(double gre_param) { return (gre_param > myLinThresh_RGB2LRGB ? std::pow(myNonLinScale_RGB2LRGB * gre_param   + myNonLinShift_RGB2LRGB, myGamma_RGB2LRGB) : myLinScale_RGB2LRGB * gre_param); }
double JMD::JMD_Vision_Process::RGBToLinearRGB_B_Pixel(double blu_param) { return (blu_param > myLinThresh_RGB2LRGB ? std::pow(myNonLinScale_RGB2LRGB * blu_param   + myNonLinShift_RGB2LRGB, myGamma_RGB2LRGB) : myLinScale_RGB2LRGB * blu_param); }
/*--- End RGBToLinearRGB_Pixel ---*/
		
/*--------------- Convert RGB <==> Linear RGB ---------------%
		
		
/*--------------- Convert RGB <==> YUV/YCbCr ---------------*/
        
/*----- RGB2YUV_Pixel -----*/
void JMD::JMD_Vision_Process::RGBToYUV_Pixel(double red_param, double green_param, double blue_param, double *Y_param, double *U_param, double *V_param)
{
	if(Y_param) { *Y_param = RGBToYUV_Y_Pixel(red_param, green_param, blue_param); }
	if(U_param) { *U_param = RGBToYUV_U_Pixel(red_param, green_param, blue_param); }
	if(V_param) { *V_param = RGBToYUV_V_Pixel(red_param, green_param, blue_param); }
}
double JMD::JMD_Vision_Process::RGBToYUV_Y_Pixel(double red_param, double green_param, double blue_param) { return RGB2YUV_Y_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::RGBToYUV_U_Pixel(double red_param, double green_param, double blue_param) { return RGB2YUV_U_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::RGBToYUV_V_Pixel(double red_param, double green_param, double blue_param) { return RGB2YUV_V_DOUB( red_param, green_param, blue_param ); }

void JMD::JMD_Vision_Process::RGBToYUV_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param, unsigned char *Y_param, unsigned char *U_param, unsigned char *V_param)
{
	if(Y_param) { *Y_param = RGBToYUV_Y_Pixel( red_param, green_param, blue_param ); }
	if(U_param) { *U_param = RGBToYUV_U_Pixel( red_param, green_param, blue_param ); }
	if(V_param) { *V_param = RGBToYUV_V_Pixel( red_param, green_param, blue_param ); }
}
unsigned char JMD::JMD_Vision_Process::RGBToYUV_Y_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param) { return RGB2YUV_Y_CHAR( red_param, green_param, blue_param ); }
unsigned char JMD::JMD_Vision_Process::RGBToYUV_U_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param) { return RGB2YUV_U_CHAR( red_param, green_param, blue_param ); }
unsigned char JMD::JMD_Vision_Process::RGBToYUV_V_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param) { return RGB2YUV_V_CHAR( red_param, green_param, blue_param ); }
/*--- End RGB2YUV_Pixel ---*/

/*----- YUVToRGB_Pixel -----*/
void JMD::JMD_Vision_Process::YUVToRGB_Pixel(double Y_param, double U_param, double V_param, double *red_param, double *green_param, double *blue_param)
{
	*red_param = 0;
	*green_param = 0;
	*blue_param = 0;
}
void JMD::JMD_Vision_Process::YUVToRGB_Pixel(unsigned char Y_param, unsigned char U_param, unsigned char V_param, unsigned char *red_param, unsigned char *green_param, unsigned char *blue_param)
{
	*red_param = 0;
	*green_param = 0;
	*blue_param = 0;
}
/*--- End YUVToRGB_Pixel ---*/

/*----- RGB2YCbCr_Pixel -----*/
/*--- End RGB2YCbCr_Pixel ---*/

/*------------- End Convert RGB <==> YUV/YCbCr -------------*/



/*--------------- Convert RGB <==> XYZ ---------------*/
        
/*----- RGBToXYZ_Pixel -----*/
// converts an RGB pixel to an XYZ pixel. Double Images should range between 0 and 1
//
// input:
//    red_param - red pixel value
//    green_param - green pixel value
//    blue_param - blue pixel value
//    white_point_param - enumeration for white point
//    rgb_color_param - enumeration for RGB color space
// output:
//    X_param - pointer to output Y value
//    Y_param - pointer to output U value
//    Z_param - pointer to output V value
void JMD::JMD_Vision_Process::RGBToXYZ_Pixel(double red_param, double green_param, double blue_param, double *X_param, double *Y_param, double *Z_param)
{
	
	//linearize
	if(myLinearizeRGB_RGB2XYZ)
	{
		double lred_param, lgreen_param, lblue_param;
		RGBToLinearRGB_Pixel(red_param,green_param,blue_param, &lred_param, &lgreen_param, &lblue_param);
		red_param   = lred_param;
		green_param = lgreen_param;
		blue_param  = lblue_param;
	}
	
	//convert
	switch(myWhitePoint_RGB2XYZ)
	{
		/*--- D65 ---*/
		case White_Point::D65:
			if (X_param) { *X_param = LinearRGBToXYZ_X_D65_Pixel( red_param, green_param, blue_param ); }
			if (Y_param) { *Y_param = LinearRGBToXYZ_Y_D65_Pixel( red_param, green_param, blue_param ); }
			if (Z_param) { *Z_param = LinearRGBToXYZ_Z_D65_Pixel( red_param, green_param, blue_param ); }
			break;
		/*- End D65 -*/
		
		/*--- D50 ---*/
		case White_Point::D50: 
			if (X_param) { *X_param = LinearRGBToXYZ_X_D50_Pixel( red_param, green_param, blue_param ); }
			if (Y_param) { *Y_param = LinearRGBToXYZ_Y_D50_Pixel( red_param, green_param, blue_param ); }
			if (Z_param) { *Z_param = LinearRGBToXYZ_Z_D50_Pixel( red_param, green_param, blue_param ); }
			break;
		/*- End D50 -*/
		
		/*--- Else ---*/
		default: 
			if (X_param) { *X_param = LinearRGBToXYZ_X_D65_Pixel( red_param, green_param, blue_param ); }
			if (Y_param) { *Y_param = LinearRGBToXYZ_Y_D65_Pixel( red_param, green_param, blue_param ); }
			if (Z_param) { *Z_param = LinearRGBToXYZ_Z_D65_Pixel( red_param, green_param, blue_param ); }
			break;
		/*- End Else -*/
	}
}
//void RGBToXYZ_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param, double *X_param = nullptr, double *Y_param = nullptr, double *Z_param = nullptr);
//void RGBToXYZ_Pixel(unsigned int red_param, unsigned int green_param, unsigned int blue_param, unsigned int *X_param = nullptr, unsigned int *Y_param = nullptr, unsigned int *Z_param = nullptr);
double JMD::JMD_Vision_Process::LinearRGBToXYZ_X_D65_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_X_D65_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::LinearRGBToXYZ_Y_D65_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_Y_D65_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::LinearRGBToXYZ_Z_D65_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_Z_D65_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::LinearRGBToXYZ_X_D50_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_X_D50_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::LinearRGBToXYZ_Y_D50_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_Y_D50_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::LinearRGBToXYZ_Z_D50_Pixel(double red_param, double green_param, double blue_param) { return LINRGB2XYZ_Z_D50_DOUB( red_param, green_param, blue_param ); }
double JMD::JMD_Vision_Process::RGBToXYZ_X_D65_Pixel(double red_param, double green_param, double blue_param) 
{
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to X
	return LINRGB2XYZ_X_D65_DOUB( red_param, green_param, blue_param ); 
}
double JMD::JMD_Vision_Process::RGBToXYZ_Y_D65_Pixel(double red_param, double green_param, double blue_param) 
{
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to Y 
	return LINRGB2XYZ_Y_D65_DOUB( red_param, green_param, blue_param ); 
}
double JMD::JMD_Vision_Process::RGBToXYZ_Z_D65_Pixel(double red_param, double green_param, double blue_param) 
{
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to Z
	return LINRGB2XYZ_Z_D65_DOUB( red_param, green_param, blue_param ); 
}
double JMD::JMD_Vision_Process::RGBToXYZ_X_D50_Pixel(double red_param, double green_param, double blue_param) 
{
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to X
	return LINRGB2XYZ_X_D50_DOUB( red_param, green_param, blue_param ); 
}
double JMD::JMD_Vision_Process::RGBToXYZ_Y_D50_Pixel(double red_param, double green_param, double blue_param) 
{
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to Y
	return LINRGB2XYZ_Y_D50_DOUB( red_param, green_param, blue_param ); 
}
double JMD::JMD_Vision_Process::RGBToXYZ_Z_D50_Pixel(double red_param, double green_param, double blue_param) 
{ 
	// RGB to Linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	// Linear RGB to Z
	return LINRGB2XYZ_Z_D50_DOUB( red_param, green_param, blue_param ); 
}
/*--- End RGBToXYZ_Pixel ---*/

/*--------------- Convert RGB <==> XYZ ---------------*/
		


/*--------------- Convert RGB <==> LAB ---------------*/

/*----- RGBToLAB_Pixel -----*/
void JMD::JMD_Vision_Process::RGBToLAB_Pixel(double red_param, double green_param, double blue_param, double *L_param, double *A_param, double *B_param)
{	
	//RGB to XYZ
	double X,Y,Z;
	RGBToXYZ_Pixel(red_param, green_param, blue_param, &X, &Y, &Z);
	
	//White Point XYZ + Gamma Correction
	switch(myWhitePoint_RGB2XYZ)
	{
		/*--- D65 ---*/
		case White_Point::D65:
			// Y
			Y = XYZToWP_Y_D65_Pixel( Y );
			Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
			
			// If A, then X
			if (A_param) 
			{ 
				X = XYZToWP_X_D65_Pixel( X );
				X = XYZWPToGammaXYZWP_X_Pixel( X );
			}
			
			// If B, then Y
			if (B_param) 
			{ 
				Z = XYZToWP_Z_D65_Pixel( Z );
				Z = XYZWPToGammaXYZWP_Z_Pixel( Z );				
			}
			break;
		/*- End D65 -*/
		
		/*--- D50 ---*/
		case White_Point::D50: 
			// Y
			Y = XYZToWP_Y_D50_Pixel( Y );
			Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
			
			// If A, then X
			if (A_param) 
			{ 
				X = XYZToWP_X_D50_Pixel( X );
				X = XYZWPToGammaXYZWP_X_Pixel( X );
			}
			
			// If B, then Y
			if (B_param) 
			{ 
				Z = XYZToWP_Z_D50_Pixel( Z );
				Z = XYZWPToGammaXYZWP_Z_Pixel( Z );
			}
			break;
		/*- End D50 -*/
		
		/*--- Else ---*/
		default:
			// Y
			Y = XYZToWP_Y_D65_Pixel( Y );
			Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
			
			// If A, then X
			if (A_param) 
			{ 
				X = XYZToWP_X_D65_Pixel( X );
				X = XYZWPToGammaXYZWP_X_Pixel( X );
			}
			
			// If B, then Y
			if (B_param) 
			{ 
				Z = XYZToWP_Z_D65_Pixel( Z ); 
				Z = XYZWPToGammaXYZWP_Z_Pixel( Z );
			}
			break;
		/*- End Else -*/
	}
	
	// L A B
	if (L_param) { *L_param = GammaXYZWPToLAB_L_Pixel(Y); }
	if (A_param) { *A_param = GammaXYZWPToLAB_A_Pixel(Y,X); }
	if (B_param) { *B_param = GammaXYZWPToLAB_B_Pixel(Y,Z); }
}
double JMD::JMD_Vision_Process::XYZToWP_X_D65_Pixel(double X_param) { return XYZ2WP_X_D65_DOUB( X_param ); }
double JMD::JMD_Vision_Process::XYZToWP_Y_D65_Pixel(double Y_param) { return XYZ2WP_Y_D65_DOUB( Y_param ); }
double JMD::JMD_Vision_Process::XYZToWP_Z_D65_Pixel(double Z_param) { return XYZ2WP_Z_D65_DOUB( Z_param ); }
double JMD::JMD_Vision_Process::XYZToWP_X_D50_Pixel(double X_param) { return XYZ2WP_X_D50_DOUB( X_param ); }
double JMD::JMD_Vision_Process::XYZToWP_Y_D50_Pixel(double Y_param) { return XYZ2WP_Y_D50_DOUB( Y_param ); }
double JMD::JMD_Vision_Process::XYZToWP_Z_D50_Pixel(double Z_param) { return XYZ2WP_Z_D50_DOUB( Z_param ); }
double JMD::JMD_Vision_Process::XYZWPToGammaXYZWP_X_Pixel(double X_param) { return (X_param > DEF_LAB_LIN_THRESH ? std::pow(X_param,DEF_LAB_GAMMA) : DEF_LAB_LIN_SCALE * X_param + DEF_LAB_LIN_SHIFT); }
double JMD::JMD_Vision_Process::XYZWPToGammaXYZWP_Y_Pixel(double Y_param) { return (Y_param > DEF_LAB_LIN_THRESH ? std::pow(Y_param,DEF_LAB_GAMMA) : DEF_LAB_LIN_SCALE * Y_param + DEF_LAB_LIN_SHIFT); }
double JMD::JMD_Vision_Process::XYZWPToGammaXYZWP_Z_Pixel(double Z_param) { return (Z_param > DEF_LAB_LIN_THRESH ? std::pow(Z_param,DEF_LAB_GAMMA) : DEF_LAB_LIN_SCALE * Z_param + DEF_LAB_LIN_SHIFT); }
double JMD::JMD_Vision_Process::GammaXYZWPToLAB_L_Pixel(double Y_param)                 { return GAMMAXYZWP2LAB_L_DOUB(Y_param); }
double JMD::JMD_Vision_Process::GammaXYZWPToLAB_A_Pixel(double Y_param, double X_param) { return GAMMAXYZWP2LAB_A_DOUB(Y_param, X_param ); }
double JMD::JMD_Vision_Process::GammaXYZWPToLAB_B_Pixel(double Y_param, double Z_param) { return GAMMAXYZWP2LAB_B_DOUB(Y_param, Z_param ); }
double JMD::JMD_Vision_Process::RGBToLAB_L_D65_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y
	double Y;
	Y = LinearRGBToXYZ_Y_D65_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D65_Pixel( Y );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	
	//return L
	return GammaXYZWPToLAB_L_Pixel(Y);
}
double JMD::JMD_Vision_Process::RGBToLAB_A_D65_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y and X
	double Y, X;
	Y = LinearRGBToXYZ_Y_D65_Pixel(red_param, green_param, blue_param);
	X = LinearRGBToXYZ_X_D65_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D65_Pixel( Y );
	X = XYZToWP_X_D65_Pixel( X );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	X = XYZWPToGammaXYZWP_X_Pixel( X );
	
	//return L
	return GammaXYZWPToLAB_A_Pixel(Y,X);
}
double JMD::JMD_Vision_Process::RGBToLAB_B_D65_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y and Z
	double Y, Z;
	Y = LinearRGBToXYZ_Y_D65_Pixel(red_param, green_param, blue_param);
	Z = LinearRGBToXYZ_Z_D65_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D65_Pixel( Y );
	Z = XYZToWP_Z_D65_Pixel( Z );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	Z = XYZWPToGammaXYZWP_Z_Pixel( Z );
	
	//return L
	return GammaXYZWPToLAB_B_Pixel(Y,Z);
}
double JMD::JMD_Vision_Process::RGBToLAB_L_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_L_D65_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
double JMD::JMD_Vision_Process::RGBToLAB_A_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_A_D65_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
double JMD::JMD_Vision_Process::RGBToLAB_B_D65_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_B_D65_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
double JMD::JMD_Vision_Process::RGBToLAB_L_D50_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y
	double Y;
	Y = LinearRGBToXYZ_Y_D50_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D50_Pixel( Y );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	
	//return L
	return GammaXYZWPToLAB_L_Pixel(Y);
}
double JMD::JMD_Vision_Process::RGBToLAB_A_D50_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y and X
	double Y, X;
	Y = LinearRGBToXYZ_Y_D50_Pixel(red_param, green_param, blue_param);
	X = LinearRGBToXYZ_X_D50_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D50_Pixel( Y );
	X = XYZToWP_X_D50_Pixel( X );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	X = XYZWPToGammaXYZWP_X_Pixel( X );
	
	//return L
	return GammaXYZWPToLAB_A_Pixel(Y,X);
}
double JMD::JMD_Vision_Process::RGBToLAB_B_D50_Pixel(double red_param, double green_param, double blue_param)
{
	//linear RGB
	red_param   = RGBToLinearRGB_R_Pixel(red_param  );
	green_param = RGBToLinearRGB_G_Pixel(green_param);
	blue_param  = RGBToLinearRGB_B_Pixel(blue_param );
	
	//just Y and Z
	double Y, Z;
	Y = LinearRGBToXYZ_Y_D50_Pixel(red_param, green_param, blue_param);
	Z = LinearRGBToXYZ_Z_D50_Pixel(red_param, green_param, blue_param);
	Y = XYZToWP_Y_D50_Pixel( Y );
	Z = XYZToWP_Z_D50_Pixel( Z );
	Y = XYZWPToGammaXYZWP_Y_Pixel( Y );
	Z = XYZWPToGammaXYZWP_Z_Pixel( Z );
	
	//return L
	return GammaXYZWPToLAB_B_Pixel(Y,Z);
}
double JMD::JMD_Vision_Process::RGBToLAB_L_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_L_D50_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
double JMD::JMD_Vision_Process::RGBToLAB_A_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_A_D50_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
double JMD::JMD_Vision_Process::RGBToLAB_B_D50_Pixel(unsigned char red_param, unsigned char green_param, unsigned char blue_param)
{
	return RGBToLAB_B_D50_Pixel(static_cast<double>(red_param) * DEF_1DIV255, static_cast<double>(green_param) * DEF_1DIV255, static_cast<double>(blue_param) * DEF_1DIV255);
}
/*--- End RGBToLAB_Pixel ---*/

/*------------- End Convert RGB <==> LAB -------------*/


		
/*--------------- Utility ---------------*/

/*----- DoubleToChar_Pixel -----*/
void JMD::JMD_Vision_Process::DoubleToChar_Pixel(double double_red_param, double double_green_param, double double_blue_param, unsigned char *char_red_param, unsigned char *char_green_param, unsigned char *char_blue_param)
{
	if(char_red_param)   { DoubleToChar_Pixel( double_red_param,   char_red_param   ); }
	if(char_green_param) { DoubleToChar_Pixel( double_green_param, char_green_param ); }
	if(char_blue_param)  { DoubleToChar_Pixel( double_blue_param,  char_blue_param  ); }
}
void JMD::JMD_Vision_Process::DoubleToChar_Pixel(double double_gray_param, unsigned char *char_gray_param)
{
	*char_gray_param = static_cast<unsigned char>( CLIP_CHAR(double_gray_param * 255) );
}
/*--- End DoubleToChar_Pixel ---*/

/*----- CharToDouble_Pixel -----*/
void JMD::JMD_Vision_Process::CharToDouble_Pixel(unsigned char char_red_param, unsigned char char_green_param, unsigned char char_blue_param, double *double_red_param, double *double_green_param, double *double_blue_param)
{
	if(double_red_param)   { CharToDouble_Pixel( char_red_param,   double_red_param   ); }
	if(double_green_param) { CharToDouble_Pixel( char_green_param, double_green_param ); }
	if(double_blue_param)  { CharToDouble_Pixel( char_blue_param,  double_blue_param  ); }
}
void JMD::JMD_Vision_Process::CharToDouble_Pixel(unsigned char char_gray_param, double *double_gray_param)
{
	*double_gray_param = static_cast<double>( CLIP_DOUB(char_gray_param * JMD::DEF_1DIV255) );
}
/*--- End CharToDouble_Pixel ---*/

/*----- Linearize_Pixel -----*/
double JMD::JMD_Vision_Process::Linearize_Pixel(double value_param, double A_param, double B_param, double G_param, double C_param, double D_param, double H_param, double TH_param)
{
	return (value_param > TH_param ? std::pow(A_param * value_param + B_param, G_param) : std::pow(C_param * value_param + D_param, H_param) );
}
/*--- End Linearize_Pixel ---*/

/*----- Gradient_Pixel -----*/
void JMD::JMD_Vision_Process::Gradient_Pixel(double u1_param, double u2_param, double v1_param, double v2_param, double *ix_param, double *iy_param, double *mag_param, double *dir_param)
{
	double Ix = u1_param - u2_param;
	double Iy = v1_param - v2_param;
	
	if(ix_param)  { *ix_param = Ix; }
	if(iy_param)  { *iy_param = Iy; }
	if(mag_param) { *mag_param = sqrt(Ix*Ix + Iy*Iy); }
	if(dir_param) { *dir_param = atan2(Iy,Ix); }
	
}
void JMD::JMD_Vision_Process::Gradient_Pixel(unsigned char u1_param, unsigned char u2_param, unsigned char v1_param, unsigned char v2_param, unsigned char *ix_param, unsigned char *iy_param, unsigned char *mag_param, unsigned char *dir_param)
{
	
}
void JMD::JMD_Vision_Process::Gradient_Pixel_Est(double u1_param, double u2_param, double v1_param, double v2_param, double *ix_param, double *iy_param, double *mag_param, double *dir_param)
{
	double Ix = Gradient_Pixel_Ix_Est(u1_param, u2_param);
	double Iy = Gradient_Pixel_Iy_Est(v1_param, v2_param);
	
	if(ix_param)  { *ix_param = Ix; }
	if(iy_param)  { *iy_param = Iy; }
	if(mag_param) { *mag_param = fabs(Ix) + fabs(Iy); }
	if(dir_param) { *dir_param = Private_Est_ATAN2(Iy,Ix); }
}
void JMD::JMD_Vision_Process::Gradient_Pixel_Est(unsigned char u1_param, unsigned char u2_param, unsigned char v1_param, unsigned char v2_param, unsigned char *ix_param, unsigned char *iy_param, unsigned char *mag_param, unsigned char *dir_param)
{
	unsigned char Ix = Gradient_Pixel_Ix_Est(u1_param,u2_param);
	unsigned char Iy = Gradient_Pixel_Iy_Est(v1_param,v2_param);
	
	if(*ix_param) { *ix_param = Ix; }
	if(*iy_param) { *iy_param = Iy; }
	if(mag_param) { *mag_param = fabs(Ix) + fabs(Iy); }
	if(dir_param) { *dir_param = Private_Est_ATAN2(Iy,Ix); }
}
double JMD::JMD_Vision_Process::Gradient_Pixel_Ix_Est(double u1_param, double u2_param) { return u1_param - u2_param; }
double JMD::JMD_Vision_Process::Gradient_Pixel_Iy_Est(double v1_param, double v2_param) { return v1_param - v2_param; }
unsigned char JMD::JMD_Vision_Process::Gradient_Pixel_Ix_Est(unsigned char u1_param, unsigned char u2_param) { return CLIP_CHAR(u1_param - u2_param); }
unsigned char JMD::JMD_Vision_Process::Gradient_Pixel_Iy_Est(unsigned char v1_param, unsigned char v2_param) { return CLIP_CHAR(v1_param - v2_param); }
/*--- End Gradient_Pixel ---*/
		
/*--------------- Utility ---------------*/



/*--------------- Setters/Getters ---------------*/
    
/*----- General -----*/
        
// RGBColor_RGB2XYZ
void JMD::JMD_Vision_Process::RGB_InColor(JMD::JMD_Vision_Process::RGB_Color rgbincolor_param)  { myRGB_InColor = rgbincolor_param; }
void JMD::JMD_Vision_Process::RGB_InColor(JMD::JMD_Vision_Process::RGB_Color *rgbincolor_param) { if(rgbincolor_param) { *rgbincolor_param = myRGB_InColor; } }

/*--- End General ---*/


/*----- RGB <==> LRGB -----*/

// Gamma_RGB2LRGB
void JMD::JMD_Vision_Process::Gamma_RGB2LRGB(double gamma_param)  { myGamma_RGB2LRGB = gamma_param; }
void JMD::JMD_Vision_Process::Gamma_RGB2LRGB(double *gamma_param) { if(gamma_param) { *gamma_param = myGamma_RGB2LRGB; } }

// NonLinScale_RGB2LRGB
void JMD::JMD_Vision_Process::NonLinScale_RGB2LRGB(double nonlinscale_param)  { myNonLinScale_RGB2LRGB = nonlinscale_param; }
void JMD::JMD_Vision_Process::NonLinScale_RGB2LRGB(double *nonlinscale_param) { if(nonlinscale_param) { *nonlinscale_param = myNonLinScale_RGB2LRGB; } }

// NonLinShift_RGB2LRGB
void JMD::JMD_Vision_Process::NonLinShift_RGB2LRGB(double nonlinshift_param)  { myNonLinScale_RGB2LRGB = nonlinshift_param; }
void JMD::JMD_Vision_Process::NonLinShift_RGB2LRGB(double *nonlinshift_param) { if(nonlinshift_param) { *nonlinshift_param = myNonLinScale_RGB2LRGB; } }

// LinThresh_RGB2LRGB
void JMD::JMD_Vision_Process::LinThresh_RGB2LRGB(double linthresh_param)  { myLinThresh_RGB2LRGB = linthresh_param; }
void JMD::JMD_Vision_Process::LinThresh_RGB2LRGB(double *linthresh_param) { if(linthresh_param) { *linthresh_param = myLinThresh_RGB2LRGB; } }

// LinScale_RGB2LRGB
void JMD::JMD_Vision_Process::LinScale_RGB2LRGB(double linscale_param)  { myLinScale_RGB2LRGB = linscale_param; }
void JMD::JMD_Vision_Process::LinScale_RGB2LRGB(double *linscale_param) { if(linscale_param) { *linscale_param = myLinScale_RGB2LRGB; } }

/*--- End RGB <==> LRGB ---*/


/*----- RGB <==> XYZ -----*/

// LinearizeRGB_RGB2XYZ
void JMD::JMD_Vision_Process::LinearizeRGB_RGB2XYZ(bool linearizergb_param)  { myLinearizeRGB_RGB2XYZ = linearizergb_param; }
void JMD::JMD_Vision_Process::LinearizeRGB_RGB2XYZ(bool *linearizergb_param) { if(linearizergb_param) { *linearizergb_param = myLinearizeRGB_RGB2XYZ; } }

// WhitePoint_RGB2XYZ
void JMD::JMD_Vision_Process::WhitePoint_RGB2XYZ(JMD::JMD_Vision_Process::White_Point whitepoint_param)  { myWhitePoint_RGB2XYZ = whitepoint_param; }
void JMD::JMD_Vision_Process::WhitePoint_RGB2XYZ(JMD::JMD_Vision_Process::White_Point *whitepoint_param) { if(whitepoint_param) { *whitepoint_param = myWhitePoint_RGB2XYZ; } }

/*--- End RGB <==> XYZ ---*/


/*----- RGB <==> LAB -----*/
/*--- End RGB <==> LAB ---*/

/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/

/*--------------- Operator Overloads ---------------*/


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
