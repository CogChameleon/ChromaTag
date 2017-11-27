#pragma once
#ifndef JMD_VISION_IMAGE_H_
#define JMD_VISION_IMAGE_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2014
 ==========================
 JMD Vision Image: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Image.hpp
 An image class
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 
 --- End Enumerations ---
 
 ----- Constructors -----
 
 --- end Constructors ---
 
 ----- Utility Methods -----
 
 --- End Utility Methods ---
 
 Notes:
 * This image class works similarly to my Matrix class but uses only
   unsigned char data. When calculations need to be done, such as with
   gradient, integer calculations are performed, and/or a lookup table
   is used.
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

//utilities
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Defines.hpp"

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

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

/*----- Forward Declerations -----*/
namespace JMD {
	//class
	class JMD_Vision_Image;
	
	//friend operators >> , <<
	std::ostream &operator << (std::ostream &os, const JMD_Vision_Image &image_param);
	std::istream &operator >> (std::istream &is, JMD_Vision_Image &image_param);
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
	
    //====================================================================//
    //====================================================================//
    //=================== JMD Vision Image Definition ====================//
    //====================================================================//
    //====================================================================//
    class JMD_Vision_Image
    {
        
        /*--------------- Enumerations ---------------*/
        public:
        
        /*----- Memory Type -----*/
        // Is the image data stored as:
        //    packed: R(0) G(0) B(0) R(1) G(1) B(1) ... etc
        //    planar: R(0) R(1) R(2)... G(0) G(1) ... B(0) B(1)...
        //
        // MEMORY_PACKED - packed storage
        // MEMORY_PLANAR - planar storage
        typedef enum Memory_Type { MEMORY_PACKED = 0, MEMORY_PLANAR } Memory_Type;
        /*--- End Memory Type ---*/
        
        /*----- Color Space -----*/
        // color space of image
        //
        // COLOR_RGB - Red Green Blue
        // COLOR_YUV - Luminance ColorU ColorV
        // COLOR_LAB - Luminance ColorA ColorB
        // COLOR_GRAY - (R+G+B)/3 or Luminance
        typedef enum Color_Space { COLOR_RGB = 0, COLOR_YUV, COLOR_LAB, COLOR_GRAY} Color_Space;
        /*--- End Color Space ---*/
        
        /*------------- End Enumerations -------------*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
		
		//image properties
		unsigned int  myHeight;
		unsigned int  myWidth;
		unsigned int  myChannels;
		unsigned int  mySize;
		Memory_Type   myMemType;
		Color_Space   myColorSpace;
		
		//flags
		bool isHeapMemory;
		bool isHeapWriter;
		//bool isPrecomputed;
		
		//data
		unsigned char *myData;
		
		//precomputed lookup tables
		unsigned int Lookup_SubsToIdx[960][1280];
		//unsigned int *
		
		//writer
		JMD_Utils_Writer *myWriter;
		
        /*------------- End Variables -------------*/
        
        /*--------------- Helpers ---------------*/
        
        /*----- Private Init -----*/
        // does all the initialization of private instance variables
        //
        // input:
        //    writer_param - JMD_Utils_Writer object for output
        //    memory_param - PACKED or PLANAR
        //    color_param - RGB,YUV,LAB
        void Private_Init(JMD_Utils_Writer *writer_param = NULL, Memory_Type memory_param = MEMORY_PACKED, Color_Space color_param = COLOR_RGB);
        /*--- End Private Init ---*/
        
        /*----- Private Create -----*/
        // does all the work for allocating new memory
        //
        // input:
        //    height_param - height for image
        //    width_param - width for image
        //    channels_param - channels for image
        void Private_Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, Memory_Type memory_param, Color_Space color_param = COLOR_RGB);
        /*--- End Private Create ---*/
        
        /*------------- End Helpers -------------*/
        
        //---------------------------------------------------------------//
        //----------------------- end Private ---------------------------//
        //---------------------------------------------------------------//
        
        
        //---------------------------------------------------------------//
        //-------------------------- Public -----------------------------//
        //---------------------------------------------------------------//
        public:
        
		/*--------------- Constructors ---------------*/
		// Constructor
		// sets a pointer to given data, no memory allocation
        //
        // input:
        //    writer_param - JMD_Utils_Writer pointer
        //    height_param - image height
        //    width_param - image width
        //    channels_param - image channels
        //    memory_param - PACKED or PLANAR
        //    color_param - RGB,YUV,LAB
        JMD_Vision_Image(JMD_Utils_Writer* writer_param = NULL);
		JMD_Vision_Image(const unsigned int height_param, const unsigned int width_param, const unsigned int channels_param, unsigned char *data_param, Memory_Type memory_param = MEMORY_PACKED, Color_Space color_param = COLOR_RGB, JMD_Utils_Writer* writer_param = NULL);
		//JMD_Vision_Image(const JMD_Vision_Image &image_param);
		/*------------- end Constructors -------------*/
		
		
		/*--------------- Destructors ---------------*/
		~JMD_Vision_Image();
		/*-------------- End Destructors ------------*/
		
        
        /*--------------- Utility Methods ---------------*/
        
        /*----- Create -----*/
        // allocates memory for the data provided. If an unsigned char
        // passed in, the data is converted to a double image. Double
        // data should be between 0-1. If you want to copy data from
        // construction, simply call the Create() with no parameters.
        //
        // input:
        //    height_param - height of image
        //    width_param -  width of image
        //    channels_param - channels of image
        //    data_param - pointer to array or scalar to fill image
        //    memory_param - PACKED or PLANAR
        //    color_param - RGB,YUV,LAB
        void Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, Memory_Type memory_param = MEMORY_PACKED, Color_Space color_param = COLOR_RGB);
        void Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, unsigned char *data_param, Memory_Type memory_param = MEMORY_PACKED, Color_Space color_param = COLOR_RGB);
        void Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, unsigned char  data_param , Memory_Type memory_param = MEMORY_PACKED, Color_Space color_param = COLOR_RGB);
        /*--- End Create ---*/
        
        /*----- Fill -----*/
        // fills image with a scalar
        //
        // input:
        //    scalar_param - value to fill image with
        void Fill(const unsigned char scalar_param);
        /*--- End Fill ---*/
        
        /*----- Clear -----*/
        // destroys allocated image data and clears data pointers, does
        // destroy an alloacted writer and is not the same as deleting
        // an object ==> Not a replacement for delete.
        //
        void Clear();
        /*--- End Clear ---*/
        
        /*----- SubscriptsToIndex -----*/
        // takes row, column, and tunnel indices and returns an index 
        // value to the flattened matrix (array) of data
        //
        // input:
        //    row_index_param - row index
        //    col_index_param - column index
        //    tun_index_param - tunnel index (i.e. "row" through z)
        // output:
        //    unsigned int - index to flattened matrix
        unsigned int SubscriptsToIndex(unsigned int row_index_param, unsigned int col_index_param, unsigned int tun_index_param = 0) const;
		/*--- End SubscriptsToIndex ---*/

		/*----- IndexToSubscripts -----*/
		// takes an index value and returns the row, column, and tunnel
		// indices
		// 
		// input:
		//    index_param - index to flattened matrix value
		//    row_index_param - ptr to row index to populate
		//    col_index_param - ptr to column index to populate
		//    tun_index_param - ptr to tunnel index to populate
		void IndexToSubscripts(unsigned int index_param, unsigned int *row_index_param, unsigned int *col_index_param, unsigned int *tun_index_param);
		/*--- End IndexToSubscripts ---*/
        
        /*------------- End Utility Methods -------------*/
        
        
        /*--------------- Processing Methods ---------------*/
        
        void RGBToYUV(JMD::JMD_Vision_Image &image_param);
		void RGBToLAB(JMD::JMD_Vision_Image &image_param);
		void RGBToGray(JMD::JMD_Vision_Image &image_param);
		/*--- End RGBTo ---*/
		
		/*----- YUVTo -----*/
		void YUVToRGB(JMD::JMD_Vision_Image &image_param);
		void YUVToLAB(JMD::JMD_Vision_Image &image_param);
		void YUVToGray(JMD::JMD_Vision_Image &image_param);
		/*--- End YUVTo ---*/
		
		/*----- LABTo -----*/
		void LABToRGB(JMD::JMD_Vision_Image &image_param);
		void LABToYUV(JMD::JMD_Vision_Image &image_param);
		void LABToGray(JMD::JMD_Vision_Image &image_param);
		/*--- End LabTo ---*/

        /*----- Color -----*/
		// converts image to a different color space
		//
		// input:
		//    image_param - where conversion is stored
		//    color_param - desired conversion color
		void Color(JMD_Vision_Image &image_param, Color_Space color_param);
		JMD_Vision_Image Color(Color_Space color_param);
		/*--- End Color ---*/
		
        /*----- Gradient -----*/
        // computes the image gradient: magnitude and direction
        //
        // input:
        //    magnitude_param - magnitude result, should be preallocated
        //    direction_param - direction result, should be preallocated
        //    force_channel_param - give one channel to force
        //    border_param - ignores border region
        //    subsample_param - NOT USED CURRENTLY
        void Gradient(JMD_Vision_Image &magnitude_param, JMD_Vision_Image &direction_param, int force_channel_param = DEF_DUMMY_INT, unsigned int border_param = 0, unsigned int subsample_param = 0);
        /*--- End Gradient ---*/
        
        /*------------- End Processing Methods -------------*/
        
        
        /*--------------- Setters/Getters ---------------*/
        
        /*----- Width -----*/
        unsigned int Cols();
        unsigned int Width();
        /*--- End Width ---*/
        
        /*----- Height -----*/
        unsigned int Rows();
        unsigned int Height();
        /*--- End Height ---*/
        
        /*----- Channels -----*/
        unsigned int Channels();
        /*--- End Channels ---*/
        
        /*----- Channel -----*/
        JMD_Vision_Image Channel( unsigned int channel_param ) const;
        void             Channel( unsigned int channel_param, JMD_Vision_Image &dest_param ) const;
        /*--- End Channel ---*/
        
        /*----- Size -----*/
        unsigned int Size();
        /*--- End Size ---*/
        
        /*----- Data -----*/
        unsigned char* Data();
		unsigned char  Data(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param) const;
		unsigned char& Data(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param);
        /*--- End Data ---*/
        
        /*------------- End Setters/Getters -------------*/
        
        
        /*--------------- Operator Overloads ---------------*/
        
        /*----- = operator -----*/
        // does a deep copy of data, and sets pointers and parameters
        //
        // input:
        //    rhs_param - image to copy
        JMD_Vision_Image& operator=(const JMD_Vision_Image &rhs_param);
        /*--- End = operator ---*/
        
        /*----- () operator -----*/
        // accesses a data member in the image, same as calling Data()
        //
        // input:
        //    index_param - index of data element to access if matrix was flattened
        //    row_index_param - row index
        //    col_index_param - column index
        //    tun_index_param - channel index
        // output:
        //    unsigned char - returns uchar value from image
        unsigned char  operator()(const unsigned int index_param) const;
        unsigned char& operator()(const unsigned int index_param);
        unsigned char  operator()(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param) const;
        unsigned char& operator()(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param);
        /*--- End () operator ---*/
        
        /*----- Operator << -----*/
        // passes along the image data in an out stream object, for use
		// in things like cout; e.g. cout << image_param;
		// 
        friend std::ostream& operator << (std::ostream &os, const JMD::JMD_Vision_Image &image_param);
        /*--- End Operator << ---*/
        
        /*----- Operator >> -----*/
        friend std::istream& operator >> (std::istream &is, JMD::JMD_Vision_Image &image_param);
        /*--- End Operator >> ---*/
        
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

