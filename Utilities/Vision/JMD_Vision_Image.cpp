/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2014
 ==========================
 JMD Vision Image: Version 1.0
 ==========================
 
 ================================================================
 JMD_Vision_Image.cpp
 An image class
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

#include "JMD_Vision_Image.hpp"

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
//================= JMD Vision Image Implementation ==================//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//

/*----- Private Init -----*/
void JMD::JMD_Vision_Image::Private_Init(JMD::JMD_Utils_Writer *writer_param, Memory_Type memory_param, Color_Space color_param)
{
	//image properties
	mySize  = myWidth*myHeight*myChannels;
	myMemType = memory_param;
	myColorSpace = color_param;
	
	//flags
	isHeapMemory  = false;
	
	//writer
	if(writer_param) { myWriter = writer_param;           isHeapWriter = false; }
	else             { myWriter = new JMD_Utils_Writer(); isHeapWriter = true; }
}
/*----- Private Init -----*/

/*----- Private Create -----*/
void JMD::JMD_Vision_Image::Private_Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, Memory_Type memory_param, Color_Space color_param)
{
	//force nonzero
	if(height_param < 1 || width_param < 1 || channels_param < 1)
	{
		myWriter->Writer_Warning("JMD_Vision_Image","Private_Create","Zero value for height or width or channels given. Skipping...",1);
		return;
	}
	else if(height_param * width_param * channels_param > DEF_MAX_MATRIX_SIZE)
	{
		myWriter->Writer_Warning("JMD_Vision_Image","Private_Create","Requested image size beyond limit. See JMD_Utils_Defines.hpp. Skipping...",1);
		return;
	}
	
	//set params
	mySize   = height_param * width_param * channels_param;
	myWidth  = width_param;
	myHeight = height_param;
	myChannels = channels_param;
	myMemType = memory_param;
	myColorSpace = color_param;
	
	//free old memory
	if (isHeapMemory)
	{
		if(myData) { delete [] myData; myData = NULL; }
	}
	
	//create memory
	myData = new unsigned char[mySize]();
	
	//turn on heap memory flag
	isHeapMemory = true;
}
/*--- End Private Create ---*/
        
//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_Vision_Image::JMD_Vision_Image(JMD_Utils_Writer* writer_param) : myWidth(0), myHeight(0), myChannels(0), myData(NULL)
{
	Private_Init(writer_param,JMD::JMD_Vision_Image::Memory_Type::MEMORY_PACKED,JMD::JMD_Vision_Image::Color_Space::COLOR_RGB);
}
JMD::JMD_Vision_Image::JMD_Vision_Image(const unsigned int height_param, const unsigned int width_param, const unsigned int channels_param, unsigned char *data_param, Memory_Type memory_param, Color_Space color_param, JMD::JMD_Utils_Writer* writer_param) :
myWidth(width_param), myHeight(height_param), myChannels(channels_param), myData(data_param)
{
	//init
	Private_Init(writer_param,memory_param,color_param);
	
	//force nonnegative sizes and smaller than max size
	if(height_param < 0 || width_param < 0 || channels_param < 0)
	{
		myWriter->Writer_Warning("JMD_Vision_Image","Constructor","negative value for height or width or channels given.",1);
	}
	else if(height_param * width_param * channels_param > DEF_MAX_MATRIX_SIZE)
	{
		myWriter->Writer_Warning("JMD_Vision_Image","Constructor","Requested image size beyond limit. See JMD_Utils_Defines.hpp.",1);
	}
}
/*JMD::JMD_Vision_Image::JMD_Vision_Image(const JMD::JMD_Vision_Image &image_param) 
{
	//init
	Private_Init(image_param.myWriter);
	
	//copy
	Create(image_param.myHeight,image_param.myWidth,image_param.myChannels,image_param.myData);
}*/
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_Vision_Image::~JMD_Vision_Image() 
{
	//free memory
	if(isHeapMemory) {	if(myData) { delete [] myData; myData = NULL; isHeapMemory = false; } }
	
	//free writer
	if(isHeapWriter) { if(myWriter) { delete myWriter; myWriter = NULL; isHeapWriter = false; } }
}
/*------------- End Destructors -------------*/

    
 
/*--------------- Utility ---------------*/

/*----- Create -----*/
void JMD::JMD_Vision_Image::Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, Memory_Type memory_param, Color_Space color_param)
{
	//Private Create
	Private_Create(height_param,width_param,channels_param,memory_param,color_param);
	for(unsigned int i=0; i < myHeight; i++) { for(unsigned int j=0; j < myWidth; j++) {
		Lookup_SubsToIdx[i][j] = i*myWidth+j;
	}}
}
void JMD::JMD_Vision_Image::Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, unsigned char *data_param, Memory_Type memory_param, Color_Space color_param)
{
	//Private Create
	Private_Create(height_param,width_param,channels_param,memory_param,color_param);
	
	//memcpy
	if (data_param && mySize > 0) { memcpy(myData,data_param,mySize*sizeof(unsigned char)); }
}
void JMD::JMD_Vision_Image::Create(unsigned int height_param, unsigned int width_param, unsigned int channels_param, unsigned char  data_param, Memory_Type memory_param, Color_Space color_param)
{
	//Private Create
	Private_Create(height_param,width_param,channels_param,memory_param,color_param);
	
	//Fill
	Fill(data_param);
}
/*--- End Create ---*/

/*----- Fill -----*/
void JMD::JMD_Vision_Image::Fill(const unsigned char scalar_param) { std::fill(myData,myData+mySize,scalar_param); }
/*--- End Fill ---*/

/*----- Clear -----*/
void JMD::JMD_Vision_Image::Clear() 
{
	//free memory
	if(isHeapMemory) {	if(myData) { delete [] myData; myData = NULL; isHeapMemory = false; } }
	
	//nullify pointers
	myData           = NULL;
	
	//set parameters
	myWidth    = 0;
	myHeight   = 0;
	myChannels = 0;
	mySize     = 0;
}
/*--- End Clear ---*/
        
/*----- SubscriptsToIndex -----*/
unsigned int JMD::JMD_Vision_Image::SubscriptsToIndex(unsigned int row_index_param, unsigned int col_index_param, unsigned int tun_index_param) const
{
	//bounds checking
	//if( row_index_param < 0 || row_index_param >= myHeight )  { myWriter->Writer_Error("JMD_Vision_Image","SubscriptsToIndex","Given row index value out of bounds.",1); }
	//if( col_index_param < 0 || col_index_param >= myWidth  )  { myWriter->Writer_Error("JMD_Vision_Image","SubscriptsToIndex","Given col index value out of bounds.",1); }
	//if( tun_index_param < 0 || tun_index_param >= myChannels) { myWriter->Writer_Error("JMD_Vision_Image","SubscriptsToIndex","Given tun index value out of bounds.",1); }
	
	//return
	switch(myMemType)
	{
		case MEMORY_PLANAR:	return   myHeight*myWidth*tun_index_param + row_index_param*myWidth    + col_index_param; break;
		case MEMORY_PACKED: return myChannels*myWidth*row_index_param + col_index_param*myChannels + tun_index_param; break;
	}
}
/*--- End SubscriptsToIndex ---*/

/*----- IndexToSubscripts -----*/
void JMD::JMD_Vision_Image::IndexToSubscripts(unsigned int index_param, unsigned int *row_index_param, unsigned int *col_index_param, unsigned int *tun_index_param)
{
	//bounds checking
	if( index_param < 0 || index_param >= mySize) { myWriter->Writer_Error("JMD_Vision_Image","IndexToSubscripts","Given index value out of bounds.",1); }
	
	//set
	unsigned int i,j,k;
	switch(myMemType)
	{
		case MEMORY_PLANAR:
			k = floor(index_param / (myWidth*myHeight));
			i = floor( (index_param - k*myWidth*myHeight) / myWidth);
			j = index_param - k*myWidth*myHeight - i*myWidth;
			break;
		case MEMORY_PACKED:
			i = floor(index_param / (myWidth*myChannels));
			j = floor( (index_param - i*myWidth*myChannels) / myWidth );
			k = index_param - i*myWidth*myChannels - j*myWidth;
			break;
	}
	
	//return if ptrs exist
	if(row_index_param) { *row_index_param = i; }
	if(col_index_param) { *col_index_param = j; }
	if(tun_index_param) { *tun_index_param = k; }
}
/*--- End IndexToSubscripts ---*/

/*--------------- Utility ---------------*/



/*--------------- Image Processing ---------------*/

/*----- RGBTo -----*/
void JMD::JMD_Vision_Image::RGBToYUV(JMD::JMD_Vision_Image &image_param)
{
	for(unsigned int i=0; i < mySize; i+=3) 
	{
		//RGB to Y
		image_param(i) = RGB2Y(myData[i],myData[i+1],myData[i+2]);
		
		//RGB to U
		image_param(i+1) = RGB2U(myData[i],myData[i+1],myData[i+2]);
		
		//RGB to V
		image_param(i+2) = RGB2V(myData[i],myData[i+1],myData[i+2]);
	}
}
void JMD::JMD_Vision_Image::RGBToLAB(JMD::JMD_Vision_Image &image_param)
{}
void JMD::JMD_Vision_Image::RGBToGray(JMD::JMD_Vision_Image &image_param) 
{
	switch(myMemType)
	{
		/*--- PACKED ---*/
		case MEMORY_PACKED:
		{
			unsigned int px = 0;
			for(unsigned int i=0; i < mySize; i+=3) { image_param.myData[px++] = (myData[i]+myData[i+1]+myData[i+2]) /3; }
			break;
		}
		/*- END PACKED -*/
		
		/*--- PLANAR ---*/
		case MEMORY_PLANAR:
		{
			unsigned int sh = mySize/3;
			for(unsigned int i=0; i < sh; i++) { image_param.myData[i] = (myData[i] + myData[i+sh] + myData[i+sh+sh]) /3; }
			break;
		}
		/*- END PLANAR -*/
	} 
}
/*--- End RGBTo ---*/

/*----- YUVTo -----*/
void JMD::JMD_Vision_Image::YUVToRGB(JMD::JMD_Vision_Image &image_param)
{}
void JMD::JMD_Vision_Image::YUVToLAB(JMD::JMD_Vision_Image &image_param)
{}
void JMD::JMD_Vision_Image::YUVToGray(JMD::JMD_Vision_Image &image_param)
{}
/*--- End YUVTo ---*/

/*----- LABTo -----*/
void JMD::JMD_Vision_Image::LABToRGB(JMD::JMD_Vision_Image &image_param)
{}
void JMD::JMD_Vision_Image::LABToYUV(JMD::JMD_Vision_Image &image_param)
{}
void JMD::JMD_Vision_Image::LABToGray(JMD::JMD_Vision_Image &image_param)
{}
/*--- End LABTo ---*/

/*----- Color -----*/
void JMD::JMD_Vision_Image::Color(JMD::JMD_Vision_Image &image_param, Color_Space color_param)
{
	if(color_param == myColorSpace) 
	{ 
		myWriter->Writer_Warning("JMD_Vision_Image","Color","Current and Desired Colorspace are the same.",0);
		image_param = (*this);
	}
	else
	{
		switch(myColorSpace)
		{
			/*--- RGB ---*/
			case COLOR_RGB: 
				switch(color_param)
				{
					case COLOR_YUV:  RGBToYUV(image_param); break;
					case COLOR_LAB:  RGBToLAB(image_param); break;
					case COLOR_GRAY: RGBToGray(image_param); break;
				}
				break;
			/*- End RGB -*/
			
			/*--- YUV ---*/
			case COLOR_YUV:
				switch(color_param)
				{
					case COLOR_RGB:  YUVToRGB(image_param); break;
					case COLOR_LAB:  YUVToLAB(image_param); break;
					case COLOR_GRAY: YUVToGray(image_param); break;
				}
				break;
			/*- End YUV -*/
			
			/*--- LAB ---*/
			case COLOR_LAB:
				switch(color_param)
				{
					case COLOR_RGB: LABToRGB(image_param); break;
					case COLOR_YUV: LABToYUV(image_param); break;
					case COLOR_GRAY: LABToGray(image_param); break;
				}
				break;
			/*- End LAB -*/
			
			//can't convert gray to anything
			case COLOR_GRAY: myWriter->Writer_Error("JMD_Vision_Image","Color","Current color is gray. Can't convert to other color spaces.",1); break;
		}
	}
}
JMD::JMD_Vision_Image JMD::JMD_Vision_Image::Color(Color_Space color_param)
{
	
}
/*--- End Color ---*/
		
/*----- Gradient -----*/
void JMD::JMD_Vision_Image::Gradient(JMD::JMD_Vision_Image &magnitude_param, JMD::JMD_Vision_Image &direction_param, int force_channel_param, unsigned int border_param, unsigned int subsample_param)
{
	//check if image sizes match
	if(this->myWidth != magnitude_param.myWidth || this->myWidth != direction_param.myWidth)     { myWriter->Writer_Error("JMD_Vision_Image","Gradient","Output images must be the same width as input image.",1); } 
	if(this->myHeight != magnitude_param.myHeight || this->myHeight != direction_param.myHeight) { myWriter->Writer_Error("JMD_Vision_Image","Gradient","Output images must be the same height as input image.",1); }
	if(this->myWidth-2-2*border_param < 3 && this->myHeight-2-2*border_param < 3) { myWriter->Writer_Error("JMD_Vision_Image","Gradient","Image too small to calculate gradient given border and subsample parameters.",1);	}

	//variables
	short Ix,Iy;
	unsigned int ChannelSt = 0, ChannelEnd = this->myChannels;
	
	//focus on one channel
	if(force_channel_param != JMD::DEF_DUMMY_INT && direction_param.myChannels == 1 && magnitude_param.myChannels == 1)
	{
		if(force_channel_param < 0 || force_channel_param >= this->myChannels) { myWriter->Writer_Error("JMD_Vision_Image","Gradient","Force Channel out of bounds.",1); }
		ChannelSt = force_channel_param; 
		ChannelEnd = force_channel_param+1;
	}
	//calculate for all channels
	else if(this->myChannels != magnitude_param.myChannels || this->myChannels != direction_param.myChannels) { myWriter->Writer_Error("JMD_Vision_Image","Gradient","Output images must be have same channels as input image.",1); }
	
	//gradient calculation
	unsigned int Stride = myChannels*myWidth;
	unsigned int Stridei = 0;
	unsigned int Stridej = 0;
	for(unsigned int k=ChannelSt; k < ChannelEnd; k++)
	{
		for(unsigned int i=1+border_param; i < this->myHeight-1-border_param; i++)
		{
			Stridei = Stride*(i+1);
			for(unsigned int j=1+border_param; j < this->myWidth-1-border_param; j++)
			{	
				//magnitude
				//Ix = myData[magnitude_param.Lookup_SubsToIdx[i][j]];
				//Ix = myData[Stridei+j*myChannels+k];
				Ix = this->Data(i+1,j,k)-this->Data(i-1,j,k);
				Iy = this->Data(i,j+1,k)-this->Data(i,j-1,k);
				magnitude_param(i,j,k) = sqrt(Ix*Ix+Iy*Iy);
				//magnitude_param(i,j,k) = DEF_UCharSqrt(Ix*Ix+Iy*Iy);
				
				//direction
				//direction_param(i,j,k) = atan2(Iy,Ix);
				
			}
		}
	}
}
/*--- End Gradient ---*/

/*------------- End Image Processing -------------*/



/*--------------- Setters/Getters ---------------*/
    
/*----- Width -----*/
unsigned int JMD::JMD_Vision_Image::Cols()  { return myWidth; }
unsigned int JMD::JMD_Vision_Image::Width() { return myWidth; }
/*--- End Width ---*/
        
/*----- Height -----*/
unsigned int JMD::JMD_Vision_Image::Rows()   { return myHeight; }
unsigned int JMD::JMD_Vision_Image::Height() { return myHeight; }
/*--- End Height ---*/
        
/*----- Channels -----*/
unsigned int JMD::JMD_Vision_Image::Channels() { return myChannels; }
/*--- End Channels ---*/
        
/*----- Channel -----*/
JMD::JMD_Vision_Image JMD::JMD_Vision_Image::Channel( unsigned int channel_param ) const
{
	//check channel
	if(channel_param < 0 || channel_param > this->myChannels-1) { this->myWriter->Writer_Error("JMD_Vision_Image","Channel","Channel parameter outside bounds.",1); }
	
	//Create result
	JMD_Vision_Image result;
	result.Create(this->myHeight,this->myWidth,1);
	Channel(channel_param,result);
	
	//return
	return result;
}        
void JMD::JMD_Vision_Image::Channel(unsigned int channel_param, JMD_Vision_Image &dest_param) const
{
	//check parameters
	if(dest_param.myWidth != this->myWidth || dest_param.myHeight != this->myHeight) { this->myWriter->Writer_Error("JMD_Vision_Image","Channel","Destination image must be same width and height as source image.",1); }
	if(channel_param < 0 || channel_param > this->myChannels-1) { this->myWriter->Writer_Error("JMD_Vision_Image","Channel","Channel parameter outside bounds.",1); }
	if(dest_param.myChannels > 1) { this->myWriter->Writer_Warning("JMD_Vision_Image","Channel","Destination image has more than one channel available, using 0 index.",0); }
	
	//copy data
	for(unsigned int i=0; i < this->myHeight; i++)
	{
		for(unsigned int j=0; j < this->myWidth; j++)
		{
			dest_param(i,j,0) = (*this)(i,j,channel_param);
		}
	}
}
/*--- End Channel ---*/

/*----- Size -----*/
unsigned int JMD::JMD_Vision_Image::Size() { return mySize; }
/*--- End Size ---*/
        
/*----- Data -----*/
unsigned char* JMD::JMD_Vision_Image::Data() { return myData; }
unsigned char  JMD::JMD_Vision_Image::Data(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param) const
{ 
	//check that index is okay
	//if( row_index_param < 0 || row_index_param >= myHeight )   { myWriter->Writer_Error("JMD_Vision_Image","Data()","Row index out of bounds.",1); }
	//if( col_index_param < 0 || col_index_param >= myWidth  )   { myWriter->Writer_Error("JMD_Vision_Image","Data()","Col index out of bounds.",1); }
	//if( tun_index_param < 0 || tun_index_param >= myChannels ) { myWriter->Writer_Error("JMD_Vision_Image","Data()","Tun index out of bounds.",1); }
	
	//index
	unsigned int index = SubscriptsToIndex(row_index_param,col_index_param,tun_index_param);
	
	//return
	return myData[ index ];
}
unsigned char& JMD::JMD_Vision_Image::Data(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param)
{
	//check that index is okay
	//if( row_index_param < 0 || row_index_param >= myHeight )   { myWriter->Writer_Error("JMD_Vision_Image","Data()","Row index out of bounds.",1); }
	//if( col_index_param < 0 || col_index_param >= myWidth  )   { myWriter->Writer_Error("JMD_Vision_Image","Data()","Col index out of bounds.",1); }
	//if( tun_index_param < 0 || tun_index_param >= myChannels ) { myWriter->Writer_Error("JMD_Vision_Image","Data()","Tun index out of bounds.",1); }
	
	//index
	unsigned int index = SubscriptsToIndex(row_index_param,col_index_param,tun_index_param);
	
	//return
	return myData[ index ];
}
/*--- End Data ---*/
        
/*------------ End Setters/Getters ------------*/



/*--------------- Operator Overloads ---------------*/

/*----- = operator -----*/
JMD::JMD_Vision_Image& JMD::JMD_Vision_Image::operator=(const JMD::JMD_Vision_Image &rhs_param)
{
	//check for self assignment
	if( this != &rhs_param ) 
	{	
		//Copy
		Create(rhs_param.myHeight,rhs_param.myWidth,rhs_param.myChannels,rhs_param.myData);
		this->myMemType        = rhs_param.myMemType;
	}
	
	//return
	return (*this);
}
/*--- End = operator ---*/
        
/*----- () operator -----*/
unsigned char  JMD::JMD_Vision_Image::operator()(const unsigned int index_param) const 
{ 
	//check that index is okay
	if( index_param < 0 || index_param >= mySize ) { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Index out of bounds.",1); }
	
	//return
	return myData[ index_param ];
}
unsigned char& JMD::JMD_Vision_Image::operator()(const unsigned int index_param) 
{
	//check that index is okay
	if( index_param < 0 || index_param >= mySize ) { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Index out of bounds.",1); }
	
	//return
	return myData[ index_param ];
}
unsigned char  JMD::JMD_Vision_Image::operator()(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param) const
{
	//check that index is okay
	if( row_index_param < 0 || row_index_param >= myHeight )   { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Row index out of bounds.",1); }
	if( col_index_param < 0 || col_index_param >= myWidth  )   { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Col index out of bounds.",1); }
	if( tun_index_param < 0 || tun_index_param >= myChannels ) { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Tun index out of bounds.",1); }
	
	//index
	unsigned int index = SubscriptsToIndex(row_index_param,col_index_param,tun_index_param);
	
	//return
	return myData[ index ];
}
unsigned char& JMD::JMD_Vision_Image::operator()(const unsigned int row_index_param, const unsigned int col_index_param, const unsigned int tun_index_param)
{
	//check that index is okay
	if( row_index_param < 0 || row_index_param >= myHeight )   { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Row index out of bounds.",1); }
	if( col_index_param < 0 || col_index_param >= myWidth  )   { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Col index out of bounds.",1); }
	if( tun_index_param < 0 || tun_index_param >= myChannels ) { myWriter->Writer_Error("JMD_Vision_Image","operator ()","Tun index out of bounds.",1); }
	
	//index
	unsigned int index = SubscriptsToIndex(row_index_param,col_index_param,tun_index_param);
	
	//return
	return myData[ index ];
}
/*--- End () operator ---*/
        
/*----- Operator << -----*/
std::ostream& JMD::operator << (std::ostream &os, const JMD::JMD_Vision_Image &image_param)
{	
	//check for empty
	if(image_param.mySize == 0 || !image_param.myData) { os << "[ ]"; return os; }
	
	/*----- Output Stream -----*/

	//for each channel
	for(unsigned int k=0; k < image_param.myChannels; k++)
	{
		os << "(:,:," << k << ")" << std::endl;
		
		//for each row
		for(unsigned int i=0; i < image_param.myHeight; i++)
		{
			//for each column
			for(unsigned int j=0; j < image_param.myWidth; j++)
			{
				//place value
				os << std::setw(8) << std::fixed << std::setprecision(4);
				if(image_param(i,j,k) == JMD::DEF_NAN) { os << "NaN"; }
				else { os << +image_param(i,j,k); }
				
				//comma
				if(j < image_param.myWidth-1) { os << ", "; }
			}
			
			//end row
			os << " |";
			if(i < image_param.myHeight-1) { os << std::endl; }
		}
		
		//end slice
		if(k < image_param.myChannels-1) { os << std::endl; }
	}

	/*--- End Output Stream ---*/
	
	//return
	return os;
}
/*--- End Operator << ---*/

/*----- Operator >> -----*/
std::istream& JMD::operator >> (std::istream &is, JMD::JMD_Vision_Image &image_param)
{
	//return
	return is;
}
/*--- End Operator >> ---*/

/*--------------- Operator Overloads ---------------*/


//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
