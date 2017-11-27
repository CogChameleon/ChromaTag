/*
 //--------------------------------------------------------------------//
 
 --------------------------
 Joseph DeGol
 UIUC Fall 2017
 --------------------------
 Run_JMD_TestColorConversion: Version 1.0
 --------------------------
 
 ----------------------------------------------------------------
 Run_JMD_TestColorConversion.cpp
 This program tests different ways of converting pixels to LAB
 and examines the amount of time it takes
 ----------------------------------------------------------------
 
 //--------------------------------------------------------------------//
*/





//====================================================================//
//====================================================================//
//============================ Preamble ==============================//
//====================================================================//
//====================================================================//

//--------------------------------------------------------------------//
//---------------------------- Includes ------------------------------//
//--------------------------------------------------------------------//

//system
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <signal.h>
#include <libgen.h>
#include <unordered_map>

//Utilities
#include "JMD_Utils_Writer.hpp"
#include "JMD_Utils_Options.hpp"
#include "JMD_Vision_Process.hpp"

//opencv
#ifdef OPENCV_FOUND
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

#define HASH_RGB(R, G, B) B + (G << 8) + (R << 16) 

//--------------------------------------------------------------------//
//------------------------- End Includes -----------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//--------------------------- Namespaces -----------------------------//
//--------------------------------------------------------------------//

using namespace JMD;

//--------------------------------------------------------------------//
//------------------------- End Namespaces ---------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//---------------------------- Globals -------------------------------//
//--------------------------------------------------------------------//

//Program Name
string ProgramName = "Run_JMD_TestColorConversion";

//Description
string Description =
"This program tests different ways of converting pixels to LAB and\n\
 examines the amount of time it takes. For help, type --help or -h.";

//Objects
JMD_Utils_Writer  myWriter;
JMD_Utils_Options myOptions(ProgramName,"1.0","Joseph DeGol","University of Illinois",Description,&myWriter);

/*----- Options -----*/

int N = 1000;
int E = -1;
std::string Input_Image;

/*--- End Options ---*/
	
//--------------------------------------------------------------------//
//---------------------------- Globals -------------------------------//
//--------------------------------------------------------------------//

//====================================================================//
//====================================================================//
//============================ Preamble ==============================//
//====================================================================//
//====================================================================//





//====================================================================//
//====================================================================//
//============================  Functions ============================//
//====================================================================//
//====================================================================//

//--------------------------------------------------------------------//
//----------------------------- Helpers ------------------------------//
//--------------------------------------------------------------------//

unsigned int hash3chars(unsigned int r, unsigned int g, unsigned int b)
{
    return  r + 256*g + 65536*b;
}
unsigned int hash3chars2(unsigned int r, unsigned int g, unsigned int b)
{
    return b + (g << 8) + (r << 16);
}

class CCTable
{
    private:
    unsigned int private_hash(unsigned int r, unsigned int g, unsigned int b)
    {
        return b + (g << 8) + (r << 16);
    }

    std::vector<float> myLAB_ATable;

    public:
    CCTable()
    {
        myLAB_ATable.resize(256*256*256);
        JMD_Vision_Process color_convert;
        double A;
        for(unsigned int r = 0; r < 256; r++)
        {
            for(unsigned int g = 0; g < 256; g++)
            {
                for(unsigned int b = 0; b < 256; b++)
                {
                    //convert
                    A = color_convert.RGBToLAB_A_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );

                    //store
                    myLAB_ATable[ hash3chars2(r,g,b) ] = A;
                }
            }
        }
    }

    double getA(unsigned int r, unsigned int g, unsigned int b)
    {
        return myLAB_ATable[ b + (g << 8) + (r << 16) ];
    }
};

class CCTable2
{
    public:
    static vector<float> myLAB_AVec;

    static void InitA()
    {
        myLAB_AVec.resize(256*256*256);
        JMD_Vision_Process color_convert;
        double A;
        for(unsigned int r = 0; r < 256; r++)
        {
            for(unsigned int g = 0; g < 256; g++)
            {
                for(unsigned int b = 0; b < 256; b++)
                {
                    //convert
                    A = color_convert.RGBToLAB_A_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );

                    //store
                    myLAB_AVec[ hash_rgb2(r,g,b) ] = A;
                }
            }
        }
    }

    static double hash_rgb2(unsigned int r, unsigned int g, unsigned int b)
    {
        return b + (g << 8) + (r << 16);
    }
};
vector<float> CCTable2::myLAB_AVec;

//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//---------------------------- Callbacks -----------------------------//
//--------------------------------------------------------------------//


//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//

//====================================================================//
//====================================================================//
//============================  Functions ============================//
//====================================================================//
//====================================================================//





//====================================================================//
//====================================================================//
//============================== Main ================================//
//====================================================================//
//====================================================================//

int main( int argc, char **argv )
{   

	//----------------------------------------------------------------//
	//--------------------------- Options ----------------------------//
	//----------------------------------------------------------------//
	
	//description vector
	vector<string> description_vector;
	
	/*----- Set Colors -----*/
	myWriter.SetProgramHeaderColor(JMD::JMD_Utils_Writer::WHITE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetProgramTextColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetAccentColor(JMD::JMD_Utils_Writer::WHITE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetInOutPromptColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetFPromptColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetNoteColor(JMD::JMD_Utils_Writer::BLUE,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetWarningColor(JMD::JMD_Utils_Writer::YELLOW,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetErrorColor(JMD::JMD_Utils_Writer::RED,JMD::JMD_Utils_Writer::NONE,true);
	myWriter.SetListColor(JMD::JMD_Utils_Writer::WHITE);
	myWriter.SetQueryColor(JMD::JMD_Utils_Writer::GREEN,JMD::JMD_Utils_Writer::NONE,true);
	/*--- End Set Colors ---*/
	
	
	/*----- Add Options - Flags and Callbacks - Alphabetical -----*/
	/*--- End Add Options - Flags and Callbacks - Alphabetical ---*/
	
	
	/*----- Add Options - Setters and Setter_Callbacks -----*/

    // input image
    myOptions.Add_Option_Setter("--input_image","-i","Input image.",&Input_Image,true);

    // number of random pixels
    myOptions.Add_Option_Setter("--num_pixels","-n","Number of random pixels to convert.",&N,true);

    //experiment to test
    // 1 - Vision Process Convert
    // 2 - OpenCV Convert
    // 3 - Hash Table
    // 4 - Hash table (bit shifts)
    // 5 - Hash Vector double (bit shifts)
    // 6 - Hash Vector float (bit shifts)
    // 7 - Hash Vector float (bit shifts macro)
    // 8 - Hash Class
    // 9 - Statis Hash Class
    myOptions.Add_Option_Setter("--experiment","-e","Experiment to test.",&E,true);

	/*--- End Add Options - Setters and Settter_Callbacks ---*/
	
	//parse help - return if false
	if( !myOptions.Parse_Help(argc,argv) ) { return 0; }
	
	//----------------------------------------------------------------//
	//------------------------- End Options --------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//---------------------------- Setup -----------------------------//
	//----------------------------------------------------------------//
	
	//extra argv vector
	std::vector<std::string> Extra_Argv;
		
	//intro prompt
	myWriter.Writer_IntroPrompt(ProgramName,"Joseph DeGol","University of Illinois","1.0","Help: ./Run_JMD_TestColorConversion --help","Program to test speed of different color conversion methods.");
	
	//parse options
	myOptions.Parse(argc,argv,true,&Extra_Argv);

    //color conversion
    JMD_Vision_Process myVP;

    //setup random number generator
    srand(time(NULL));

	//----------------------------------------------------------------//
	//-------------------------- End Setup ---------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//----------------------------- Run ------------------------------//
	//----------------------------------------------------------------//
	
    //an image was given on command line
    if( !Input_Image.empty() )
    {
        //read image
        cv::Mat image;
        image = cv::imread( Input_Image );

        //image dimensions
        int nrow = image.rows;
        int ncol = image.cols;

        //read okay?
        if( !image.data )
        { 
            myWriter.Writer_Warning(ProgramName, "Main", "Could not open: "+Input_Image, false); 
        }
        else
        {
            //variables
            unsigned char *p;
            unsigned char R, G, B;
            double t0, t1, t2;
            double A;
            cv::Mat RGB, LAB;
            
            //generate N random pixel locations
            std::vector<int> randu;
            std::vector<int> randv;
            for(int i = 0; i < N; i++)
            {
                randu.push_back( rand() % ncol );
                randv.push_back( rand() % nrow );
            }
            

            /*----- Experiment 1 -----*/
            // convert pixels to LAB using Vision_Process
            if(E == -1 || E == 1)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 1 (Vision_Process Convert) Beginning ...",true);
                t0 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = 3*randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = myVP.RGBToLAB_A_D65_Pixel(R,G,B);
                }
                t0 = ((double)cv::getTickCount() - t0) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t0 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 1 (Vision_Process Convert) Done.",true);
                std::cout << std::endl;
            }
            /*--- End Experiment 1 ---*/

            /*----- Experiment 2 -----*/
            // convert pixels to LAB using OpenCV
            if(E == -1 || E == 2)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 2 (OpenCV Convert) Beginning ...",true);
                t1 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //convert
                    RGB = image(cv::Rect(u,v,1,1));
                    cv::cvtColor(RGB,LAB,CV_BGR2Lab);
                    
                    //value
                    cv::Vec3b lab = LAB.at<cv::Vec3b>(0,0);
                    A = lab.val[1];
                }
                t1 = ((double)cv::getTickCount() - t1) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t1 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 2 (OpenCV Convert) Done.",true);
                std::cout << std::endl;
            }
            /*--- End Experiment 2 ---*/

            /*----- Make Hashes -----*/
            std::unordered_map<unsigned int,double> LABMap;
            std::vector<double> LABVecD;
            std::vector<float>  LABVecF;
            if(E == -1 || E > 2)
            {
                // make hash table
                myWriter.Writer_Note(ProgramName,"Main","Creating Hash Table ...",true);
                LABVecD.resize(256*256*256);
                LABVecF.resize(256*256*256);
                for(unsigned int r = 0; r < 256; r++)
                {
                    for(unsigned int g = 0; g < 256; g++)
                    {
                        for(unsigned int b = 0; b < 256; b++)
                        {
                            //convert
                            A = myVP.RGBToLAB_A_D65_Pixel( static_cast<unsigned char>(r), static_cast<unsigned int>(g), static_cast<unsigned int>(b) );

                            //store
                            LABMap[ hash3chars2(r,g,b) ] = A;
                            LABVecD[ hash3chars2(r,g,b) ] = A;
                            LABVecF[ hash3chars2(r,g,b) ] = A;
                        }
                    }
                }
            }
            /*----- End Make Hashes -----*/

            /*----- Experiment 3 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 3)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 3 (Hash Table Convert) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = LABMap[ hash3chars(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 3 (Hash Table Convert) Done.",true);
                std::cout << std::endl;
            }
            /*--- End Experiment 3 ---*/

            /*----- Experiment 4 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 4)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 4 (Hash Table Convert (bit shifts)) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = LABMap[ hash3chars2(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 4 (Hash Table Convert (bit shifts)) Done.",true);
                std::cout << std::endl;
            }
            /*--- End Experiment 4 ---*/

            /*----- Experiment 5 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 5)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 5 (Hash Vector Convert (bit shifts)) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = LABVecD[ hash3chars2(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 5 (Hash Vector Convert (bit shifts)) Done.",true);
            }
            /*--- End Experiment 5 ---*/

            /*----- Experiment 6 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 6)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 6 (Hash Vector Convert (bit shifts)) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = LABVecF[ hash3chars2(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 6 (Hash Vector Convert (bit shifts)) Done.",true);
            }
            /*--- End Experiment 6 ---*/

            /*----- Experiment 7 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 7)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 7 (Hash Vector Convert (bit shifts macro)) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = LABVecF[ HASH_RGB(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 7 (Hash Vector Convert (bit shifts macro)) Done.",true);
            }
            /*--- End Experiment 7 ---*/

            /*----- Experiment 8 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 8)
            {
                CCTable myCC;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 8 (Hash Class Convert (bit shifts)) Beginning ...",true);
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = myCC.getA(R,G,B);
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 8 (Hash Class Convert (bit shifts)) Done.",true);
                std::cout << std::endl;
            }
            /*--- End Experiment 8 ---*/

            /*----- Experiment 8 -----*/
            // read converted pixels from hash table
            if(E == -1 || E == 9)
            {
                myWriter.Writer_Note(ProgramName,"Main","Experiment 9 (Hash Static Class Convert (bit shifts)) Beginning ...",true);
                CCTable2::InitA();
                t2 = (double)cv::getTickCount();
                for(int i = 0; i < randu.size(); i++)
                {
                    //position
                    int u = randu[i];
                    int v = randv[i];

                    //get pixels
                    p = image.ptr<unsigned char>(v);
                    B = p[u];
                    G = p[u + 1];
                    R = p[u + 2];

                    //convert
                    A = CCTable2::myLAB_AVec[ HASH_RGB(R,G,B) ];
                }
                t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
                std::cout << "        Time Elapsed = " << t2 << std::endl;
                myWriter.Writer_Note(ProgramName,"Main","Experiment 9 (Hash Static Class Convert (bit shifts)) Done.",true);
            }
            /*--- End Experiment 8 ---*/
        }
    }
    else
    {
        myWriter.Writer_Warning(ProgramName,"Main","No image provided on command line",false);
    }
	
	//----------------------------------------------------------------//
	//--------------------------- End Run ----------------------------//
	//----------------------------------------------------------------//
	
	
	
	//----------------------------------------------------------------//
	//-------------------------- Shutdown ----------------------------//
	//----------------------------------------------------------------//
	
	//exit
	myWriter.Writer_OutroPrompt(ProgramName);
	
	//return
	return 0;
	
	//----------------------------------------------------------------//
	//------------------------- End Shutdown -------------------------//
	//----------------------------------------------------------------//
	
}

//====================================================================//
//====================================================================//
//============================== Main ================================//
//====================================================================//
//====================================================================//
