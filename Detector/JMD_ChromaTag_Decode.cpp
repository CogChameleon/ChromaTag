/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Decode: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Decode.cpp
 A class for ChromaTag Decoding
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

#include "JMD_ChromaTag_Decode.hpp"

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
//================ JMD ChromaTag Decode Implementation ===============//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//
        
/*--------------- Setup Methods ---------------*/

/*----- Private Init -----*/
void JMD::JMD_ChromaTag_Decode::Private_Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param)
{
    // color tables
	myLAB_LTable = l_table_param;
    myLAB_ATable = a_table_param;
    myLAB_BTable = b_table_param;

	//settings
	mySettings = settings_param;
	
	//number of horizontal bits based on Tag Family
	switch(mySettings->myTagFamily)
	{
		case RGBW_16H5:   myNumHorizontalBits = 4; break;
		case RGBW_25H9:   myNumHorizontalBits = 5; break;
		case RGBW_36H11:
		case RGRBW_36H11: myNumHorizontalBits = 6; break;
		default:          myNumHorizontalBits = 4; break;
	}
	
	//build image grid
	Private_BuildGrid(myImageGrid);
	
	//number of rings to use
	switch(mySettings->myUseRingsForHomography)
	{
		case USE_RINGS_OUTER: myNumRingsToUse = 1; break;
		case USE_RINGS_ALL:   myNumRingsToUse = 3; break;
		default: myNumRingsToUse = 1; break;
	}
	
	//populate the image plane locations based on the number
	//of rings that will be used
	double s;
	for(int i = 0; i < myNumRingsToUse; i++)
	{
		//update s
		s = (myNumRingsToUse - i) / myNumRingsToUse;
		
		//add four points
		myImagePoints.push_back( cv::Point2f( s,  s) );	//top right
		myImagePoints.push_back( cv::Point2f( s, -s) );	//bottom right
		myImagePoints.push_back( cv::Point2f(-s, -s) );	//bottom left
		myImagePoints.push_back( cv::Point2f(-s,  s) );	//top left
	}
    
    //generate tag codes
    Private_BuildTagFamily();
}
/*--- End Private Init ---*/

/*----- Private Build Grid -----*/
void JMD::JMD_ChromaTag_Decode::Private_BuildGrid(std::vector<cv::Point2f> &grid_param)
{
	//variables
	double gridstep = 2.0 / (2 + myNumHorizontalBits); //grid step = (1 - - 1) / (2 + #of horizontal grid squares)
	double gridx = -1.0 + (gridstep + gridstep*0.5);
	double gridy = -1.0 + (gridstep + gridstep*0.5);
	
	//for each vertical bit
	for(int i = 0; i < myNumHorizontalBits; i++)
	{
		//grid y = furthest up + step
		gridy = -1.0 + (gridstep + gridstep*0.5);
		
		//for each horizontal bit
		for(int j = 0; j < myNumHorizontalBits; j++)
		{
			//save grid space
			grid_param.push_back( cv::Point2f(gridx,gridy) );

			//increment gridy by step
			gridy += gridstep;
		}
		
		//increment gridx by step
		gridx += gridstep;
	}
}
/*--- End Private Build Grid ---*/

/*------------- End General Helpers -------------*/



/*--------------- Tag Family Methods ---------------*/

/*----- Private Family -----*/
void JMD::JMD_ChromaTag_Decode::Private_BuildTagFamily()
{
    //variables
    std::vector<unsigned long long> H0Codes;
    unsigned int max_hamming = mySettings->myMaxSearchHammingDistance;
    unsigned int numbits = myNumHorizontalBits * myNumHorizontalBits;
    
    //get true codes
    switch(mySettings->myTagFamily)
	{
		case RGBW_16H5:   Private_Family16H5(H0Codes); break;
		case RGBW_25H9:   Private_Family25H9(H0Codes); break;
		case RGBW_36H11:
		case RGRBW_36H11: Private_Family36H11(H0Codes); break;
		default:          Private_Family16H5(H0Codes); break;
	}
    
    //check that Max Search Hamming Distance makes sense
    if(max_hamming > 3)
    {
        max_hamming = 3;
    }
    
    //perturb codes by hamming distance to get final set of tags
    for(std::vector<unsigned long long>::iterator cit = H0Codes.begin(); cit != H0Codes.end(); ++cit)
    {
        //current code
        unsigned long long code = *cit;
        
        //add current code
        Private_AddTagCode(code, code, 0);
        
        //perturb by 1
        if(max_hamming >= 1)
        {
            //for each bit, perturb 1 bit and add code
            for(int i = 0; i < numbits; i++)
            {
                Private_AddTagCode(code, code ^ (1L << i) , 1);
            }
        }
        
        //perturb by 2
        if(max_hamming >= 2)
        {
            //for each bit, perturb 1 bit and add code
            for(int i = 0; i < numbits; i++)
            {
                for(int j = 0; j < i; j++)
                {
                    Private_AddTagCode(code, code ^ (1L << i) ^ (1L << j) , 2);
                }
            }
        }
        
        //perturb by 3
        if(max_hamming >= 3)
        {
            //for each bit, perturb 1 bit and add code
            for(int i = 0; i < numbits; i++)
            {
                for(int j = 0; j < i; j++)
                {
                    for(int k = 0; k < j; k++)
                    {
                        Private_AddTagCode(code, code ^ (1L << i) ^ (1L << j) ^ (1L << k), 3);
                    }
                }
            }
        }
    }
}
/*--- End Private Family ---*/

/*----- Private Family 16H5 -----*/
void JMD::JMD_ChromaTag_Decode::Private_Family16H5(std::vector<unsigned long long> &codes_param)
{
	//clear codes just in case
	codes_param.clear();
	
	//make space for 30 tags
	codes_param.resize(30);
	
	//push back tag payloads
    codes_param[0] = 0x000000000000231bUL;    codes_param[1] = 0x0000000000002ea5UL;    codes_param[2] = 0x000000000000346aUL;
    codes_param[3] = 0x00000000000045b9UL;    codes_param[4] = 0x00000000000079a6UL;    codes_param[5] = 0x0000000000007f6bUL;
    codes_param[6] = 0x000000000000b358UL;    codes_param[7] = 0x000000000000e745UL;    codes_param[8] = 0x000000000000fe59UL;    
    codes_param[9] = 0x000000000000156dUL;    codes_param[10] = 0x000000000000380bUL;    codes_param[11] = 0x000000000000f0abUL;
    codes_param[12] = 0x0000000000000d84UL;    codes_param[13] = 0x0000000000004736UL;    codes_param[14] = 0x0000000000008c72UL;
    codes_param[15] = 0x000000000000af10UL;    codes_param[16] = 0x000000000000093cUL;    codes_param[17] = 0x00000000000093b4UL;
    codes_param[18] = 0x000000000000a503UL;    codes_param[19] = 0x000000000000468fUL;    codes_param[20] = 0x000000000000e137UL;
    codes_param[21] = 0x0000000000005795UL;    codes_param[22] = 0x000000000000df42UL;    codes_param[23] = 0x0000000000001c1dUL;
    codes_param[24] = 0x000000000000e9dcUL;    codes_param[25] = 0x00000000000073adUL;    codes_param[26] = 0x000000000000ad5fUL;
    codes_param[27] = 0x000000000000d530UL;    codes_param[28] = 0x00000000000007caUL;    codes_param[29] = 0x000000000000af2eUL;
}
/*--- End Private Family 16H5 ---*/

/*----- Private Family 25H9 -----*/
void JMD::JMD_ChromaTag_Decode::Private_Family25H9(std::vector<unsigned long long> &codes_param)
{
	//clear codes just in case
	codes_param.clear();
	
	//make space for 30 tags
	codes_param.resize(35);
	
	//push back tag payloads
	codes_param[0] = 0x000000000155cbf1UL;    codes_param[1] = 0x0000000001e4d1b6UL;
    codes_param[2] = 0x00000000017b0b68UL;    codes_param[3] = 0x0000000001eac9cdUL;    codes_param[4] = 0x00000000012e14ceUL;
    codes_param[5] = 0x00000000003548bbUL;    codes_param[6] = 0x00000000007757e6UL;    codes_param[7] = 0x0000000001065dabUL;
    codes_param[8] = 0x0000000001baa2e7UL;    codes_param[9] = 0x0000000000dea688UL;    codes_param[10] = 0x000000000081d927UL;
    codes_param[11] = 0x000000000051b241UL;    codes_param[12] = 0x0000000000dbc8aeUL;    codes_param[13] = 0x0000000001e50e19UL;
    codes_param[14] = 0x00000000015819d2UL;    codes_param[15] = 0x00000000016d8282UL;    codes_param[16] = 0x000000000163e035UL;
    codes_param[17] = 0x00000000009d9b81UL;    codes_param[18] = 0x000000000173eec4UL;    codes_param[19] = 0x0000000000ae3a09UL;
    codes_param[20] = 0x00000000005f7c51UL;    codes_param[21] = 0x0000000001a137fcUL;    codes_param[22] = 0x0000000000dc9562UL;
    codes_param[23] = 0x0000000001802e45UL;    codes_param[24] = 0x0000000001c3542cUL;    codes_param[25] = 0x0000000000870fa4UL;
    codes_param[26] = 0x0000000000914709UL;    codes_param[27] = 0x00000000016684f0UL;    codes_param[28] = 0x0000000000c8f2a5UL;
    codes_param[29] = 0x0000000000833ebbUL;    codes_param[30] = 0x000000000059717fUL;    codes_param[31] = 0x00000000013cd050UL;
    codes_param[32] = 0x0000000000fa0ad1UL;    codes_param[33] = 0x0000000001b763b0UL;    codes_param[34] = 0x0000000000b991ceUL;
}
/*--- End Private Family 25H9 ---*/

/*----- Private Family 36H11 -----*/
void JMD::JMD_ChromaTag_Decode::Private_Family36H11(std::vector<unsigned long long> &codes_param)
{
	//clear codes just in case
	codes_param.clear();
	
	//make space for 587 tags
	codes_param.resize(587);
	
	//push back tag payloads
	codes_param[0] = 0x0000000d5d628584UL;    codes_param[1] = 0x0000000d97f18b49UL;    codes_param[2] = 0x0000000dd280910eUL;
    codes_param[3] = 0x0000000e479e9c98UL;    codes_param[4] = 0x0000000ebcbca822UL;    codes_param[5] = 0x0000000f31dab3acUL;    codes_param[6] = 0x0000000056a5d085UL;
    codes_param[7] = 0x000000010652e1d4UL;    codes_param[8] = 0x000000022b1dfeadUL;    codes_param[9] = 0x0000000265ad0472UL;    codes_param[10] = 0x000000034fe91b86UL;
    codes_param[11] = 0x00000003ff962cd5UL;    codes_param[12] = 0x000000043a25329aUL;    codes_param[13] = 0x0000000474b4385fUL;    codes_param[14] = 0x00000004e9d243e9UL;
    codes_param[15] = 0x00000005246149aeUL;    codes_param[16] = 0x00000005997f5538UL;    codes_param[17] = 0x0000000683bb6c4cUL;    codes_param[18] = 0x00000006be4a7211UL;
    codes_param[19] = 0x00000007e3158eeaUL;    codes_param[20] = 0x000000081da494afUL;    codes_param[21] = 0x0000000858339a74UL;    codes_param[22] = 0x00000008cd51a5feUL;
    codes_param[23] = 0x00000009f21cc2d7UL;    codes_param[24] = 0x0000000a2cabc89cUL;    codes_param[25] = 0x0000000adc58d9ebUL;    codes_param[26] = 0x0000000b16e7dfb0UL;
    codes_param[27] = 0x0000000b8c05eb3aUL;    codes_param[28] = 0x0000000d25ef139dUL;    codes_param[29] = 0x0000000d607e1962UL;    codes_param[30] = 0x0000000e4aba3076UL;
    codes_param[31] = 0x00000002dde6a3daUL;    codes_param[32] = 0x000000043d40c678UL;    codes_param[33] = 0x00000005620be351UL;    codes_param[34] = 0x000000064c47fa65UL;
    codes_param[35] = 0x0000000686d7002aUL;    codes_param[36] = 0x00000006c16605efUL;    codes_param[37] = 0x00000006fbf50bb4UL;    codes_param[38] = 0x00000008d06d39dcUL;
    codes_param[39] = 0x00000009f53856b5UL;    codes_param[40] = 0x0000000adf746dc9UL;    codes_param[41] = 0x0000000bc9b084ddUL;    codes_param[42] = 0x0000000d290aa77bUL;
    codes_param[43] = 0x0000000d9e28b305UL;    codes_param[44] = 0x0000000e4dd5c454UL;    codes_param[45] = 0x0000000fad2fe6f2UL;    codes_param[46] = 0x0000000181a8151aUL;
    codes_param[47] = 0x000000026be42c2eUL;    codes_param[48] = 0x00000002e10237b8UL;    codes_param[49] = 0x0000000405cd5491UL;    codes_param[50] = 0x00000007742eab1cUL;
    codes_param[51] = 0x000000085e6ac230UL;    codes_param[52] = 0x00000008d388cdbaUL;    codes_param[53] = 0x00000009f853ea93UL;    codes_param[54] = 0x0000000c41ea2445UL;
    codes_param[55] = 0x0000000cf1973594UL;    codes_param[56] = 0x000000014a34a333UL;    codes_param[57] = 0x000000031eacd15bUL;    codes_param[58] = 0x00000006c79d2dabUL;
    codes_param[59] = 0x000000073cbb3935UL;    codes_param[60] = 0x000000089c155bd3UL;    codes_param[61] = 0x00000008d6a46198UL;    codes_param[62] = 0x000000091133675dUL;
    codes_param[63] = 0x0000000a708d89fbUL;    codes_param[64] = 0x0000000ae5ab9585UL;    codes_param[65] = 0x0000000b9558a6d4UL;    codes_param[66] = 0x0000000b98743ab2UL;
    codes_param[67] = 0x0000000d6cec68daUL;    codes_param[68] = 0x00000001506bcaefUL;    codes_param[69] = 0x00000004becd217aUL;    codes_param[70] = 0x00000004f95c273fUL;
    codes_param[71] = 0x0000000658b649ddUL;    codes_param[72] = 0x0000000a76c4b1b7UL;    codes_param[73] = 0x0000000ecf621f56UL;    codes_param[74] = 0x00000001c8a56a57UL;
    codes_param[75] = 0x00000003628e92baUL;    codes_param[76] = 0x000000053706c0e2UL;    codes_param[77] = 0x00000005e6b3d231UL;    codes_param[78] = 0x00000007809cfa94UL;
    codes_param[79] = 0x0000000e97eead6fUL;    codes_param[80] = 0x00000005af40604aUL;    codes_param[81] = 0x00000007492988adUL;    codes_param[82] = 0x0000000ed5994712UL;
    codes_param[83] = 0x00000005eceaf9edUL;    codes_param[84] = 0x00000007c1632815UL;    codes_param[85] = 0x0000000c1a0095b4UL;    codes_param[86] = 0x0000000e9e25d52bUL;
    codes_param[87] = 0x00000003a6705419UL;    codes_param[88] = 0x0000000a8333012fUL;    codes_param[89] = 0x00000004ce5704d0UL;    codes_param[90] = 0x0000000508e60a95UL;
    codes_param[91] = 0x0000000877476120UL;    codes_param[92] = 0x0000000a864e950dUL;    codes_param[93] = 0x0000000ea45cfce7UL;    codes_param[94] = 0x000000019da047e8UL;
    codes_param[95] = 0x000000024d4d5937UL;    codes_param[96] = 0x00000006e079cc9bUL;    codes_param[97] = 0x000000099f2e11d7UL;    codes_param[98] = 0x000000033aa50429UL;
    codes_param[99] = 0x0000000499ff26c7UL;    codes_param[100] = 0x000000050f1d3251UL;    codes_param[101] = 0x000000066e7754efUL;    codes_param[102] = 0x000000096ad633ceUL;
    codes_param[103] = 0x00000009a5653993UL;    codes_param[104] = 0x0000000aca30566cUL;    codes_param[105] = 0x0000000c298a790aUL;    codes_param[106] = 0x00000008be44b65dUL;
    codes_param[107] = 0x0000000dc68f354bUL;    codes_param[108] = 0x000000016f7f919bUL;    codes_param[109] = 0x00000004dde0e826UL;    codes_param[110] = 0x0000000d548cbd9fUL;
    codes_param[111] = 0x0000000e0439ceeeUL;    codes_param[112] = 0x0000000fd8b1fd16UL;    codes_param[113] = 0x000000076521bb7bUL;    codes_param[114] = 0x0000000d92375742UL;
    codes_param[115] = 0x0000000cab16d40cUL;    codes_param[116] = 0x0000000730c9dd72UL;    codes_param[117] = 0x0000000ad9ba39c2UL;    codes_param[118] = 0x0000000b14493f87UL;
    codes_param[119] = 0x000000052b15651fUL;    codes_param[120] = 0x0000000185409cadUL;    codes_param[121] = 0x000000077ae2c68dUL;    codes_param[122] = 0x000000094f5af4b5UL;
    codes_param[123] = 0x00000000a13bad55UL;    codes_param[124] = 0x000000061ea437cdUL;    codes_param[125] = 0x0000000a022399e2UL;    codes_param[126] = 0x0000000203b163d1UL;
    codes_param[127] = 0x00000007bba8f40eUL;    codes_param[128] = 0x000000095bc9442dUL;    codes_param[129] = 0x000000041c0b5358UL;    codes_param[130] = 0x00000008e9c6cc81UL;
    codes_param[131] = 0x00000000eb549670UL;    codes_param[132] = 0x00000009da3a0b51UL;    codes_param[133] = 0x0000000d832a67a1UL;    codes_param[134] = 0x0000000dcd4350bcUL;
    codes_param[135] = 0x00000004aa05fdd2UL;    codes_param[136] = 0x000000060c7bb44eUL;    codes_param[137] = 0x00000004b358b96cUL;    codes_param[138] = 0x0000000067299b45UL;
    codes_param[139] = 0x0000000b9c89b5faUL;    codes_param[140] = 0x00000006975acaeaUL;    codes_param[141] = 0x000000062b8f7afaUL;    codes_param[142] = 0x000000033567c3d7UL;
    codes_param[143] = 0x0000000bac139950UL;    codes_param[144] = 0x0000000a5927c62aUL;    codes_param[145] = 0x00000005c916e6a4UL;    codes_param[146] = 0x0000000260ecb7d5UL;
    codes_param[147] = 0x000000029b7bbd9aUL;    codes_param[148] = 0x0000000903205f26UL;    codes_param[149] = 0x0000000ae72270a4UL;    codes_param[150] = 0x00000003d2ec51a7UL;
    codes_param[151] = 0x000000082ea55324UL;    codes_param[152] = 0x000000011a6f3427UL;    codes_param[153] = 0x00000001ca1c4576UL;    codes_param[154] = 0x0000000a40c81aefUL;
    codes_param[155] = 0x0000000bddccd730UL;    codes_param[156] = 0x00000000e617561eUL;    codes_param[157] = 0x0000000969317b0fUL;    codes_param[158] = 0x000000067f781364UL;
    codes_param[159] = 0x0000000610912f96UL;    codes_param[160] = 0x0000000b2549fdfcUL;    codes_param[161] = 0x000000006e5aaa6bUL;    codes_param[162] = 0x0000000b6c475339UL;
    codes_param[163] = 0x0000000c56836a4dUL;    codes_param[164] = 0x0000000844e351ebUL;    codes_param[165] = 0x00000004647f83b4UL;    codes_param[166] = 0x00000000908a04f5UL;
    codes_param[167] = 0x00000007f51034c9UL;    codes_param[168] = 0x0000000aee537fcaUL;    codes_param[169] = 0x00000005e92494baUL;    codes_param[170] = 0x0000000d445808f4UL;
    codes_param[171] = 0x000000028d68b563UL;    codes_param[172] = 0x000000004d25374bUL;    codes_param[173] = 0x00000002bc065f65UL;    codes_param[174] = 0x000000096dc3ea0cUL;
    codes_param[175] = 0x00000004b2ade817UL;    codes_param[176] = 0x000000007c3fd502UL;    codes_param[177] = 0x0000000e768b5cafUL;    codes_param[178] = 0x000000017605cf6cUL;
    codes_param[179] = 0x0000000182741ee4UL;    codes_param[180] = 0x000000062846097cUL;    codes_param[181] = 0x000000072b5ebf80UL;    codes_param[182] = 0x0000000263da6e13UL;
    codes_param[183] = 0x0000000fa841bcb5UL;    codes_param[184] = 0x00000007e45e8c69UL;    codes_param[185] = 0x0000000653c81fa0UL;    codes_param[186] = 0x00000007443b5e70UL;
    codes_param[187] = 0x00000000a5234afdUL;    codes_param[188] = 0x000000074756f24eUL;    codes_param[189] = 0x0000000157ebf02aUL;    codes_param[190] = 0x000000082ef46939UL;
    codes_param[191] = 0x000000080d420264UL;    codes_param[192] = 0x00000002aeed3e98UL;    codes_param[193] = 0x0000000b0a1dd4f8UL;    codes_param[194] = 0x0000000b5436be13UL;
    codes_param[195] = 0x00000007b7b4b13bUL;    codes_param[196] = 0x00000001ce80d6d3UL;    codes_param[197] = 0x000000016c08427dUL;    codes_param[198] = 0x0000000ee54462ddUL;
    codes_param[199] = 0x00000001f7644cceUL;    codes_param[200] = 0x00000009c7b5cc92UL;    codes_param[201] = 0x0000000e369138f8UL;    codes_param[202] = 0x00000005d5a66e91UL;
    codes_param[203] = 0x0000000485d62f49UL;    codes_param[204] = 0x0000000e6e819e94UL;    codes_param[205] = 0x0000000b1f340eb5UL;    codes_param[206] = 0x000000009d198ce2UL;
    codes_param[207] = 0x0000000d60717437UL;    codes_param[208] = 0x00000000196b856cUL;    codes_param[209] = 0x0000000f0a6173a5UL;    codes_param[210] = 0x000000012c0e1ec6UL;
    codes_param[211] = 0x000000062b82d5cfUL;    codes_param[212] = 0x0000000ad154c067UL;    codes_param[213] = 0x0000000ce3778832UL;    codes_param[214] = 0x00000006b0a7b864UL;
    codes_param[215] = 0x00000004c7686694UL;    codes_param[216] = 0x00000005058ff3ecUL;    codes_param[217] = 0x0000000d5e21ea23UL;    codes_param[218] = 0x00000009ff4a76eeUL;
    codes_param[219] = 0x00000009dd981019UL;    codes_param[220] = 0x00000001bad4d30aUL;    codes_param[221] = 0x0000000c601896d1UL;    codes_param[222] = 0x0000000973439b48UL;
    codes_param[223] = 0x00000001ce7431a8UL;    codes_param[224] = 0x000000057a8021d6UL;    codes_param[225] = 0x0000000f9dba96e6UL;    codes_param[226] = 0x000000083a2e4e7cUL;
    codes_param[227] = 0x00000008ea585380UL;    codes_param[228] = 0x0000000af6c0e744UL;    codes_param[229] = 0x0000000875b73babUL;    codes_param[230] = 0x0000000da34ca901UL;
    codes_param[231] = 0x00000002ab9727efUL;    codes_param[232] = 0x0000000d39f21b9aUL;    codes_param[233] = 0x00000008a10b742fUL;    codes_param[234] = 0x00000005f8952dbaUL;
    codes_param[235] = 0x0000000f8da71ab0UL;    codes_param[236] = 0x0000000c25f9df96UL;    codes_param[237] = 0x000000006f8a5d94UL;    codes_param[238] = 0x0000000e42e63e1aUL;
    codes_param[239] = 0x0000000b78409d1bUL;    codes_param[240] = 0x0000000792229addUL;    codes_param[241] = 0x00000005acf8c455UL;    codes_param[242] = 0x00000002fc29a9b0UL;
    codes_param[243] = 0x0000000ea486237bUL;    codes_param[244] = 0x0000000b0c9685a0UL;    codes_param[245] = 0x00000001ad748a47UL;    codes_param[246] = 0x000000003b4712d5UL;
    codes_param[247] = 0x0000000f29216d30UL;    codes_param[248] = 0x00000008dad65e49UL;    codes_param[249] = 0x00000000a2cf09ddUL;    codes_param[250] = 0x00000000b5f174c6UL;
    codes_param[251] = 0x0000000e54f57743UL;    codes_param[252] = 0x0000000b9cf54d78UL;    codes_param[253] = 0x00000004a312a88aUL;    codes_param[254] = 0x000000027babc962UL;
    codes_param[255] = 0x0000000b86897111UL;    codes_param[256] = 0x0000000f2ff6c116UL;    codes_param[257] = 0x000000082274bd8aUL;    codes_param[258] = 0x000000097023505eUL;
    codes_param[259] = 0x000000052d46edd1UL;    codes_param[260] = 0x0000000585c1f538UL;    codes_param[261] = 0x0000000bddd00e43UL;    codes_param[262] = 0x00000005590b74dfUL;
    codes_param[263] = 0x0000000729404a1fUL;    codes_param[264] = 0x000000065320855eUL;    codes_param[265] = 0x0000000d3d4b6956UL;    codes_param[266] = 0x00000007ae374f14UL;
    codes_param[267] = 0x00000002d7a60e06UL;    codes_param[268] = 0x0000000315cd9b5eUL;    codes_param[269] = 0x0000000fd36b4eacUL;    codes_param[270] = 0x0000000f1df7642bUL;
    codes_param[271] = 0x000000055db27726UL;    codes_param[272] = 0x00000008f15ebc19UL;    codes_param[273] = 0x0000000992f8c531UL;    codes_param[274] = 0x000000062dea2a40UL;
    codes_param[275] = 0x0000000928275cabUL;    codes_param[276] = 0x000000069c263cb9UL;    codes_param[277] = 0x0000000a774cca9eUL;    codes_param[278] = 0x0000000266b2110eUL;
    codes_param[279] = 0x00000001b14acbb8UL;    codes_param[280] = 0x0000000624b8a71bUL;    codes_param[281] = 0x00000001c539406bUL;    codes_param[282] = 0x00000003086d529bUL;
    codes_param[283] = 0x00000000111dd66eUL;    codes_param[284] = 0x000000098cd630bfUL;    codes_param[285] = 0x00000008b9d1ffdcUL;    codes_param[286] = 0x000000072b2f61e7UL;
    codes_param[287] = 0x00000009ed9d672bUL;    codes_param[288] = 0x000000096cdd15f3UL;    codes_param[289] = 0x00000006366c2504UL;    codes_param[290] = 0x00000006ca9df73aUL;
    codes_param[291] = 0x0000000a066d60f0UL;    codes_param[292] = 0x0000000e7a4b8addUL;    codes_param[293] = 0x00000008264647efUL;    codes_param[294] = 0x0000000aa195bf81UL;
    codes_param[295] = 0x00000009a3db8244UL;    codes_param[296] = 0x0000000014d2df6aUL;    codes_param[297] = 0x00000000b63265b7UL;    codes_param[298] = 0x00000002f010de73UL;
    codes_param[299] = 0x000000097e774986UL;    codes_param[300] = 0x0000000248affc29UL;    codes_param[301] = 0x0000000fb57dcd11UL;    codes_param[302] = 0x00000000b1a7e4d9UL;
    codes_param[303] = 0x00000004bfa2d07dUL;    codes_param[304] = 0x000000054e5cdf96UL;    codes_param[305] = 0x00000004c15c1c86UL;    codes_param[306] = 0x0000000cd9c61166UL;
    codes_param[307] = 0x0000000499380b2aUL;    codes_param[308] = 0x0000000540308d09UL;    codes_param[309] = 0x00000008b63fe66fUL;    codes_param[310] = 0x0000000c81aeb35eUL;
    codes_param[311] = 0x000000086fe0bd5cUL;    codes_param[312] = 0x0000000ce2480c2aUL;    codes_param[313] = 0x00000001ab29ee60UL;    codes_param[314] = 0x00000008048daa15UL;
    codes_param[315] = 0x0000000dbfeb2d39UL;    codes_param[316] = 0x0000000567c9858cUL;    codes_param[317] = 0x00000002b6edc5bcUL;    codes_param[318] = 0x00000002078fca82UL;
    codes_param[319] = 0x0000000adacc22aaUL;    codes_param[320] = 0x0000000b92486f49UL;    codes_param[321] = 0x000000051fac5964UL;    codes_param[322] = 0x0000000691ee6420UL;
    codes_param[323] = 0x0000000f63b3e129UL;    codes_param[324] = 0x000000039be7e572UL;    codes_param[325] = 0x0000000da2ce6c74UL;    codes_param[326] = 0x000000020cf17a5cUL;
    codes_param[327] = 0x0000000ee55f9b6eUL;    codes_param[328] = 0x0000000fb8572726UL;    codes_param[329] = 0x0000000b2c2de548UL;    codes_param[330] = 0x0000000caa9bce92UL;
    codes_param[331] = 0x0000000ae9182db3UL;    codes_param[332] = 0x000000074b6e5bd1UL;    codes_param[333] = 0x0000000137b252afUL;    codes_param[334] = 0x000000051f686881UL;
    codes_param[335] = 0x0000000d672f6c02UL;    codes_param[336] = 0x0000000654146ce4UL;    codes_param[337] = 0x0000000f944bc825UL;    codes_param[338] = 0x0000000e8327f809UL;
    codes_param[339] = 0x000000076a73fd59UL;    codes_param[340] = 0x0000000f79da4cb4UL;    codes_param[341] = 0x0000000956f8099bUL;    codes_param[342] = 0x00000007b5f2655cUL;
    codes_param[343] = 0x0000000d06b114a6UL;    codes_param[344] = 0x0000000d0697ca50UL;    codes_param[345] = 0x000000027c390797UL;    codes_param[346] = 0x0000000bc61ed9b2UL;
    codes_param[347] = 0x0000000cc12dd19bUL;    codes_param[348] = 0x0000000eb7818d2cUL;    codes_param[349] = 0x0000000092fcecdaUL;    codes_param[350] = 0x000000089ded4ea1UL;
    codes_param[351] = 0x0000000256a0ba34UL;    codes_param[352] = 0x0000000b6948e627UL;    codes_param[353] = 0x00000001ef6b1054UL;    codes_param[354] = 0x00000008639294a2UL;
    codes_param[355] = 0x0000000eda3780a4UL;    codes_param[356] = 0x000000039ee2af1dUL;    codes_param[357] = 0x0000000cd257edc5UL;    codes_param[358] = 0x00000002d9d6bc22UL;
    codes_param[359] = 0x0000000121d3b47dUL;    codes_param[360] = 0x000000037e23f8adUL;    codes_param[361] = 0x0000000119f31cf6UL;    codes_param[362] = 0x00000002c97f4f09UL;
    codes_param[363] = 0x0000000d502abfe0UL;    codes_param[364] = 0x000000010bc3ca77UL;    codes_param[365] = 0x000000053d7190efUL;    codes_param[366] = 0x000000090c3e62a6UL;
    codes_param[367] = 0x00000007e9ebf675UL;    codes_param[368] = 0x0000000979ce23d1UL;    codes_param[369] = 0x000000027f0c98e9UL;    codes_param[370] = 0x0000000eafb4ae59UL;
    codes_param[371] = 0x00000007ca7fe2bdUL;    codes_param[372] = 0x00000001490ca8f6UL;    codes_param[373] = 0x00000009123387baUL;    codes_param[374] = 0x0000000b3bc73888UL;
    codes_param[375] = 0x00000003ea87e325UL;    codes_param[376] = 0x00000004888964aaUL;    codes_param[377] = 0x0000000a0188a6b9UL;    codes_param[378] = 0x0000000cd383c666UL;
    codes_param[379] = 0x000000040029a3fdUL;    codes_param[380] = 0x0000000e1c00ac5cUL;    codes_param[381] = 0x000000039e6f2b6eUL;    codes_param[382] = 0x0000000de664f622UL;
    codes_param[383] = 0x0000000e979a75e8UL;    codes_param[384] = 0x00000007c6b4c86cUL;    codes_param[385] = 0x0000000fd492e071UL;    codes_param[386] = 0x00000008fbb35118UL;
    codes_param[387] = 0x000000040b4a09b7UL;    codes_param[388] = 0x0000000af80bd6daUL;    codes_param[389] = 0x000000070e0b2521UL;    codes_param[390] = 0x00000002f5c54d93UL;
    codes_param[391] = 0x00000003f4a118d5UL;    codes_param[392] = 0x000000009c1897b9UL;    codes_param[393] = 0x0000000079776eacUL;    codes_param[394] = 0x0000000084b00b17UL;
    codes_param[395] = 0x00000003a95ad90eUL;    codes_param[396] = 0x000000028c544095UL;    codes_param[397] = 0x000000039d457c05UL;    codes_param[398] = 0x00000007a3791a78UL;
    codes_param[399] = 0x0000000bb770e22eUL;    codes_param[400] = 0x00000009a822bd6cUL;    codes_param[401] = 0x000000068a4b1fedUL;    codes_param[402] = 0x0000000a5fd27b3bUL;
    codes_param[403] = 0x00000000c3995b79UL;    codes_param[404] = 0x0000000d1519dff1UL;    codes_param[405] = 0x00000008e7eee359UL;    codes_param[406] = 0x0000000cd3ca50b1UL;
    codes_param[407] = 0x0000000b73b8b793UL;    codes_param[408] = 0x000000057aca1c43UL;    codes_param[409] = 0x0000000ec2655277UL;    codes_param[410] = 0x0000000785a2c1b3UL;
    codes_param[411] = 0x000000075a07985aUL;    codes_param[412] = 0x0000000a4b01eb69UL;    codes_param[413] = 0x0000000a18a11347UL;    codes_param[414] = 0x0000000db1f28ca3UL;
    codes_param[415] = 0x0000000877ec3e25UL;    codes_param[416] = 0x000000031f6341b8UL;    codes_param[417] = 0x00000001363a3a4cUL;    codes_param[418] = 0x0000000075d8b9baUL;
    codes_param[419] = 0x00000007ae0792a9UL;    codes_param[420] = 0x0000000a83a21651UL;    codes_param[421] = 0x00000007f08f9fb5UL;    codes_param[422] = 0x00000000d0cf73a9UL;
    codes_param[423] = 0x0000000b04dcc98eUL;    codes_param[424] = 0x0000000f65c7b0f8UL;    codes_param[425] = 0x000000065ddaf69aUL;    codes_param[426] = 0x00000002cf9b86b3UL;
    codes_param[427] = 0x000000014cb51e25UL;    codes_param[428] = 0x0000000f48027b5bUL;    codes_param[429] = 0x00000000ec26ea8bUL;    codes_param[430] = 0x000000044bafd45cUL;
    codes_param[431] = 0x0000000b12c7c0c4UL;    codes_param[432] = 0x0000000959fd9d82UL;    codes_param[433] = 0x0000000c77c9725aUL;    codes_param[434] = 0x000000048a22d462UL;
    codes_param[435] = 0x00000008398e8072UL;    codes_param[436] = 0x0000000ec89b05ceUL;    codes_param[437] = 0x0000000bb682d4c9UL;    codes_param[438] = 0x0000000e5a86d2ffUL;
    codes_param[439] = 0x0000000358f01134UL;    codes_param[440] = 0x00000008556ddcf6UL;    codes_param[441] = 0x000000067584b6e2UL;    codes_param[442] = 0x000000011609439fUL;
    codes_param[443] = 0x000000008488816eUL;    codes_param[444] = 0x0000000aaf1a2c46UL;    codes_param[445] = 0x0000000f879898cfUL;    codes_param[446] = 0x00000008bbe5e2f7UL;
    codes_param[447] = 0x0000000101eee363UL;    codes_param[448] = 0x0000000690f69377UL;    codes_param[449] = 0x0000000f5bd93cd9UL;    codes_param[450] = 0x0000000cea4c2bf6UL;
    codes_param[451] = 0x00000009550be706UL;    codes_param[452] = 0x00000002c5b38a60UL;    codes_param[453] = 0x0000000e72033547UL;    codes_param[454] = 0x00000004458b0629UL;
    codes_param[455] = 0x0000000ee8d9ed41UL;    codes_param[456] = 0x0000000d2f918d72UL;    codes_param[457] = 0x000000078dc39fd3UL;    codes_param[458] = 0x00000008212636f6UL;
    codes_param[459] = 0x00000007450a72a7UL;    codes_param[460] = 0x0000000c4f0cf4c6UL;    codes_param[461] = 0x0000000367bcddcdUL;    codes_param[462] = 0x0000000c1caf8cc6UL;
    codes_param[463] = 0x0000000a7f5b853dUL;    codes_param[464] = 0x00000009d536818bUL;    codes_param[465] = 0x0000000535e021b0UL;    codes_param[466] = 0x0000000a7eb8729eUL;
    codes_param[467] = 0x0000000422a67b49UL;    codes_param[468] = 0x0000000929e928a6UL;    codes_param[469] = 0x000000048e8aefccUL;    codes_param[470] = 0x0000000a9897393cUL;
    codes_param[471] = 0x00000005eb81d37eUL;    codes_param[472] = 0x00000001e80287b7UL;    codes_param[473] = 0x000000034770d903UL;    codes_param[474] = 0x00000002eef86728UL;
    codes_param[475] = 0x000000059266ccb6UL;    codes_param[476] = 0x00000000110bba61UL;    codes_param[477] = 0x00000001dfd284efUL;    codes_param[478] = 0x0000000447439d1bUL;
    codes_param[479] = 0x0000000fece0e599UL;    codes_param[480] = 0x00000009309f3703UL;    codes_param[481] = 0x000000080764d1ddUL;    codes_param[482] = 0x0000000353f1e6a0UL;
    codes_param[483] = 0x00000002c1c12dccUL;    codes_param[484] = 0x0000000c1d21b9d7UL;    codes_param[485] = 0x0000000457ee453eUL;    codes_param[486] = 0x0000000d66faf540UL;
    codes_param[487] = 0x000000044831e652UL;    codes_param[488] = 0x0000000cfd49a848UL;    codes_param[489] = 0x00000009312d4133UL;    codes_param[490] = 0x00000003f097d3eeUL;
    codes_param[491] = 0x00000008c9ebef7aUL;    codes_param[492] = 0x0000000a99e29e88UL;    codes_param[493] = 0x00000000e9fab22cUL;    codes_param[494] = 0x00000004e748f4fbUL;
    codes_param[495] = 0x0000000ecdee4288UL;    codes_param[496] = 0x0000000abce5f1d0UL;    codes_param[497] = 0x0000000c42f6876cUL;    codes_param[498] = 0x00000007ed402ea0UL;
    codes_param[499] = 0x0000000e5c4242c3UL;    codes_param[500] = 0x0000000d5b2c31aeUL;    codes_param[501] = 0x0000000286863be6UL;    codes_param[502] = 0x0000000160444d94UL;
    codes_param[503] = 0x00000005f0f5808eUL;    codes_param[504] = 0x0000000ae3d44b2aUL;    codes_param[505] = 0x00000009f5c5d109UL;    codes_param[506] = 0x00000008ad9316d7UL;
    codes_param[507] = 0x00000003422ba064UL;    codes_param[508] = 0x00000002fed11d56UL;    codes_param[509] = 0x0000000bea6e3e04UL;    codes_param[510] = 0x000000004b029eecUL;
    codes_param[511] = 0x00000006deed7435UL;    codes_param[512] = 0x00000003718ce17cUL;    codes_param[513] = 0x000000055857f5e2UL;    codes_param[514] = 0x00000002edac7b62UL;
    codes_param[515] = 0x0000000085d6c512UL;    codes_param[516] = 0x0000000d6ca88e0fUL;    codes_param[517] = 0x00000002b7e1fc69UL;    codes_param[518] = 0x0000000a699d5c1bUL;
    codes_param[519] = 0x0000000f05ad74deUL;    codes_param[520] = 0x00000004cf5fb56dUL;    codes_param[521] = 0x00000005725e07e1UL;    codes_param[522] = 0x000000072f18a2deUL;
    codes_param[523] = 0x00000001cec52609UL;    codes_param[524] = 0x000000048534243cUL;    codes_param[525] = 0x00000002523a4d69UL;    codes_param[526] = 0x000000035c1b80d1UL;
    codes_param[527] = 0x0000000a4d7338a7UL;    codes_param[528] = 0x00000000db1af012UL;    codes_param[529] = 0x0000000e61a9475dUL;    codes_param[530] = 0x000000005df03f91UL;
    codes_param[531] = 0x000000097ae260bbUL;    codes_param[532] = 0x000000032d627fefUL;    codes_param[533] = 0x0000000b640f73c2UL;    codes_param[534] = 0x000000045a1ac9c6UL;
    codes_param[535] = 0x00000006a2202de1UL;    codes_param[536] = 0x000000057d3e25f2UL;    codes_param[537] = 0x00000005aa9f986eUL;    codes_param[538] = 0x00000000cc859d8aUL;
    codes_param[539] = 0x0000000e3ec6cca8UL;    codes_param[540] = 0x000000054e95e1aeUL;    codes_param[541] = 0x0000000446887b06UL;    codes_param[542] = 0x00000007516732beUL;
    codes_param[543] = 0x00000003817ac8f5UL;    codes_param[544] = 0x00000003e26d938cUL;    codes_param[545] = 0x0000000aa81bc235UL;    codes_param[546] = 0x0000000df387ca1bUL;
    codes_param[547] = 0x00000000f3a3b3f2UL;    codes_param[548] = 0x0000000b4bf69677UL;    codes_param[549] = 0x0000000ae21868edUL;    codes_param[550] = 0x000000081e1d2d9dUL;
    codes_param[551] = 0x0000000a0a9ea14cUL;    codes_param[552] = 0x00000008eee297a9UL;    codes_param[553] = 0x00000004740c0559UL;    codes_param[554] = 0x0000000e8b141837UL;
    codes_param[555] = 0x0000000ac69e0a3dUL;    codes_param[556] = 0x00000009ed83a1e1UL;    codes_param[557] = 0x00000005edb55ecbUL;    codes_param[558] = 0x000000007340fe81UL;
    codes_param[559] = 0x000000050dfbc6bfUL;    codes_param[560] = 0x00000004f583508aUL;    codes_param[561] = 0x0000000cb1fb78bcUL;    codes_param[562] = 0x00000004025ced2fUL;
    codes_param[563] = 0x000000039791ebecUL;    codes_param[564] = 0x000000053ee388f1UL;    codes_param[565] = 0x00000007d6c0bd23UL;    codes_param[566] = 0x000000093a995fbeUL;
    codes_param[567] = 0x00000008a41728deUL;    codes_param[568] = 0x00000002fe70e053UL;    codes_param[569] = 0x0000000ab3db443aUL;    codes_param[570] = 0x00000001364edb05UL;
    codes_param[571] = 0x000000047b6eeed6UL;    codes_param[572] = 0x000000012e71af01UL;    codes_param[573] = 0x000000052ff83587UL;    codes_param[574] = 0x00000003a1575dd8UL;
    codes_param[575] = 0x00000003feaa3564UL;    codes_param[576] = 0x0000000eacf78ba7UL;    codes_param[577] = 0x00000000872b94f8UL;    codes_param[578] = 0x0000000da8ddf9a2UL;
    codes_param[579] = 0x00000009aa920d2bUL;    codes_param[580] = 0x00000001f350ed36UL;    codes_param[581] = 0x000000018a5e861fUL;    codes_param[582] = 0x00000002c35b89c3UL;
    codes_param[583] = 0x00000003347ac48aUL;    codes_param[584] = 0x00000007f23e022eUL;    codes_param[585] = 0x00000002459068fbUL;    codes_param[586] = 0x0000000e83be4b73UL;
}
/*--- End Private Family 36H11 ---*/

/*------------- End Tag Family Methods -------------*/



/*--------------- Decode Methods ---------------*/

/*----- Private Decode -----*/
void JMD::JMD_ChromaTag_Decode::Private_Decode(JMD_Vision_Image &image_param)
{
	std::cout << "JMD_ChromaTag_Decode::Private_Decode not implemented yet. OpenCV missing from cmake path." << std::endl;
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Decode::Private_Decode(cv::Mat &image_param)
{
    //variables
	std::vector<cv::Point2f> tag_points, grid_samples;
	std::vector<double> GreenToB_Values(12), RedToB_Values(4);
	JMD_ChromaTag_Detection *curr_detection;
	TagBorderSet *curr_borders;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
    TagCode *found_code;
    
	//for each detection
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//ptr to current detection
		curr_detection = *it;
		
		//Was Active? If not, break, we have reached end of active detections
		if(!curr_detection->IsActive) { break; }
		
		//Pointer to detection's border
		curr_borders = &(curr_detection->TagBorders);
		
		//for each border
		for(int rit = 1; rit <= myNumRingsToUse; rit++)
		{
			//current ring
			curr_ring = curr_borders->at( curr_borders->size() - rit );
			
			//for each segment
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				//current segment
				curr_seg = *sit;
				
				//save corner point
				tag_points.push_back( cv::Point2f( curr_seg->myLine.Point1()->U(), curr_seg->myLine.Point1()->V() ) );
			}
		}
		
		//try to find homography
		cv::Mat H;
		if( !Private_Homography(tag_points,H) )
		{
			continue;
		}
		
		//Warp image grid to make sample grid
		cv::perspectiveTransform( myImageGrid, grid_samples, H );
		
		//read off pixel values
		unsigned char *p;
		unsigned char R,G,B;
        unsigned int hash_idx;
		int U, V, j, redct = 0, greenct = 0;
		double LAB_L;
		double MinRedB   = JMD::DEF_PINF, MaxRedB   = JMD::DEF_NINF;
		double MinGreenB = JMD::DEF_PINF, MaxGreenB = JMD::DEF_NINF;
		for(int i = 0; i < grid_samples.size(); i++)
		{	
            //U,V
			U = static_cast<int>( grid_samples[i].x );
			V = static_cast<int>( grid_samples[i].y );
			
            //check if U,V is in image, skip over pixel spot if not
            if( U < 0 || V < 0 || U >= image_param.cols || V >= image_param.rows)
            {
                continue;
            }
            
			//point to pixel row
			p = image_param.ptr<unsigned char>(V);
			j = 3 * U;
			
			//R, G, B values
			B = p[j];
			G = p[j + 1];
			R = p[j + 2];
			
			//Convert to LAB
            hash_idx = HASH_RGB(R,G,B);
            LAB_L = myLAB_LTable->at( hash_idx );
			
			//store in red or green array
			if(i == 5 || i == 6 || i == 9 || i == 10) 
			{
				//save red value
				RedToB_Values[redct] = LAB_L; 
				redct++;
				
				//save max and min
				if(LAB_L > MaxRedB) { MaxRedB = LAB_L; }
				if(LAB_L < MinRedB) { MinRedB = LAB_L; } 
			}
			else 
			{
                //save green value
				GreenToB_Values[greenct] = LAB_L; 
				greenct++; 
				
				//save max and min
				if(LAB_L > MaxGreenB) { MaxGreenB = LAB_L; }
				if(LAB_L < MinGreenB) { MinGreenB = LAB_L; }
			}
		}
        
        //cluster into code
        redct = 0;
        greenct = 0;
        unsigned long long code = 0;
        double red_thresh = (MaxRedB + MinRedB) / 2;
        double green_thresh = (MaxGreenB + MinGreenB) / 2;
        for(int i = 0; i < grid_samples.size(); i++)
        {
            //shift left 1, does nothing on first iteration
            code = (code << 1);
            
            //red
            if(i == 5 || i == 6 || i == 9 || i ==10)
            {
                //if value greater than red thresh, mark as 1
                if( RedToB_Values[redct] > red_thresh )
                {
                    code |= 1;
                }
                else
                {
                    //do nothing
                }
                redct++;
            }
            //green
            else
            {
                //if value greater than green thres, mark as 1
                if( GreenToB_Values[greenct] < green_thresh )
                {
                    code |= 1;
                }
                else
                {
                    //do nothing
                }
                greenct++;
            }
        }
		
        //Find Tag
        if( !Private_FindTagCode(code, &found_code) )
        {
            //failed to find tag, remove detection
            curr_detection->IsCode = false;
        }
        else
        {
            //set decode values for current detection
            curr_detection->IsCode  = true;
            curr_detection->TagBits = myNumHorizontalBits * myNumHorizontalBits;
            curr_detection->TagCode = found_code->myH0Code;
        }
        
		//clear tag points
		tag_points.clear();
	}
}
#endif
/*--- End Private Decode ---*/

/*----- Private Find Tag Code -----*/
bool JMD::JMD_ChromaTag_Decode::Private_FindTagCode(unsigned long long code_param, TagCode **tag_code_param)
{
    //for each rotation
    for(int rct = 0; rct < 4; rct ++)
    {
        //find code
        std::unordered_map<unsigned long long,TagCode>::iterator got = myTagCodes.find(code_param);
        if( got != myTagCodes.end() )
        {
            //return TagCode
            *tag_code_param = &(got->second);
            
            //found code
            return true;
        }
        
        //rotate code
        code_param = Private_RotateCodeCW(code_param);
    }
    
    //failed to find code
    return false;
}
/*--- End Private Find Tag Code ---*/

/*----- Private Homography -----*/
void JMD::JMD_ChromaTag_Decode::Private_Homography()
{
	
}
#ifdef OPENCV_FOUND
bool JMD::JMD_ChromaTag_Decode::Private_Homography(std::vector<cv::Point2f> &tag_points_param, cv::Mat &H_param)
{	
	//try to find homography
	try 
	{
		H_param = cv::findHomography(myImagePoints,tag_points_param);
	}
	catch (...)
	{
		return false;
	}
	
	//return success
	return true;
}
#endif
/*--- End Private Homography ---*/

/*------------- End Decode Methods -------------*/





/*----- Private Add Tag Code -----*/
void JMD::JMD_ChromaTag_Decode::Private_AddTagCode(unsigned long long h0_code_param, unsigned long long code_param, unsigned int hamming_param)
{
    //code struct
    TagCode tmp;
    
    //populate values
    tmp.myH0Code = h0_code_param;
    tmp.myCode   = code_param;
    tmp.myHammingDistance = hamming_param;
    
    //add code
    myTagCodes[code_param] = tmp;
}
/*--- End Private Add Tag Code ---*/



/*----- Private Rotate Code CW -----*/
unsigned long long JMD::JMD_ChromaTag_Decode::Private_RotateCodeCW(unsigned long long code_param)
{
    //variables
    unsigned long long rot_code = 0;
    unsigned long long shift;
    int idx;
    
    //for each row, starting at end
    for(int r = myNumHorizontalBits - 1; r >= 0; r--)
    {
        //for each col, starting at beginning
        for(int c = 0; c < myNumHorizontalBits; c++)
        {
            //shift 1 left
            rot_code = rot_code << 1;
            
            idx = r + myNumHorizontalBits * c;
            shift = static_cast<unsigned long long>(1) << idx;
            if( (code_param & shift) != 0 )
            {
                rot_code |= 1;
            }
        }
    }
    
    //return code
    return rot_code;
}
/*--- End Private Rotate Code CW ---*/

/*------------- End Decode Helpers -------------*/
		
//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//





//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_ChromaTag_Decode::JMD_ChromaTag_Decode(JMD_ChromaTag_Collection *collection_param)
{
	//collection
	myDetections = collection_param;
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_ChromaTag_Decode::~JMD_ChromaTag_Decode() 
{

}
/*------------- End Destructors -------------*/



/*--------------- Setup ---------------*/

/*----- Init -----*/
void JMD::JMD_ChromaTag_Decode::Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param) 
{ 
	Private_Init(l_table_param,a_table_param,b_table_param,settings_param);
}
/*--- End Init ---*/

/*------------- End Setup -------------*/

		
		
/*--------------- Tag Decode Methods ---------------*/    

/*---------- Detect ----------*/
void JMD::JMD_ChromaTag_Decode::Decode(JMD::JMD_Vision_Image &image_param)
{
	
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Decode::Decode( cv::Mat &image_param )
{
	Private_Decode(image_param);
}
#endif
/*-------- End Detect --------*/
            
/*------------- End Tag Decode Methods -------------*/
        
        

/*--------------- Utility ---------------*/
/*--------------- Utility ---------------*/



/*--------------- Setters/Getters ---------------*/    
/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/
/*--------------- Operator Overloads ---------------*/



/*--------------- DEBUG Methods ---------------*/

/*----- DEBUG ShowLAB -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Decode::DEBUG_ShowLAB(cv::Mat &image_param)
{
    //variables
    cv::Mat ImLAB;
    cv::Mat ImL;
    cv::Mat ImA;
    cv::Mat ImB;
    
    //convert
    cv::cvtColor(image_param,ImLAB,CV_BGR2Lab);
    
    //channels
    cv::extractChannel(ImLAB,ImL,0);
    cv::extractChannel(ImLAB,ImA,1);
    cv::extractChannel(ImLAB,ImB,2);
    
    //show
    cv::namedWindow( "DEBUG_ShowLAB - L", cv::WINDOW_AUTOSIZE );
    cv::imshow( "DEBUG_ShowLAB - L", ImL );
    cv::namedWindow( "DEBUG_ShowLAB - A", cv::WINDOW_AUTOSIZE );
    cv::imshow( "DEBUG_ShowLAB - A", ImA );
    cv::namedWindow( "DEBUG_ShowLAB - B", cv::WINDOW_AUTOSIZE );
    cv::imshow( "DEBUG_ShowLAB - B", ImB );
    cv::waitKey(0);
}
#endif
/*--- End DEBUG ShowLAB ---*/

/*----- DEBUG ShowGrid -----*/
#ifdef OPENCV_FOUND
	void DEBUG_ShowGrid() {}
#endif
/*--- End DEBUG ShowGrid ---*/

/*----- DEBUG ShowGridSamples -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Decode::DEBUG_ShowGridSamples(cv::Mat &image_param, std::vector<cv::Point2f> &samples_param)
{
	for(std::vector<cv::Point2f>::iterator it = samples_param.begin(); it != samples_param.end(); ++it)
	{
		cv::circle(image_param,*it,2,cv::Scalar(0,255,0),-1);
		//std::cout << it->x << " " << it->y << std::endl;
	}
}
#endif
/*--- End DEBUG ShowGridSamples ---*/

/*----- DEBUG ShowCodes -----*/
void JMD::JMD_ChromaTag_Decode::DEBUG_ShowCodes(cv::Mat &image_param)
{
    //variables
	TagBorderSet curr_border;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	//for each detection
	for(JMD_ChromaTag_Collection::iterator cit = myDetections->begin(); cit != myDetections->end(); ++cit)
	{
		//quit if unactive
		if( !(*cit)->IsActive ) { return; }
        
        //skip if no code
        if( !(*cit)->IsCode) { continue; }
		
		//current border, outer ring
		curr_border = (*cit)->TagBorders;
        curr_ring = curr_border.back();
		
        //get left, top, right, and bottom most points
        JMD_Vision_Point2D *left_most  = curr_ring->back()->myLine.Point1(); 
        JMD_Vision_Point2D *top_most   = curr_ring->back()->myLine.Point1(); 
        JMD_Vision_Point2D *right_most = curr_ring->back()->myLine.Point1();
        JMD_Vision_Point2D *bot_most   = curr_ring->back()->myLine.Point1();
        
        //for each segment
        for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
        {
            curr_seg = *sit;
            JMD_Vision_Point2D *Pt1 = curr_seg->myLine.Point1();
            
            //left most
            if(Pt1->U() < left_most->U() )  { left_most  = Pt1; }
            //right most
            if(Pt1->U() > right_most->U() ) { right_most = Pt1; }
            //up most
            if(Pt1->V() < top_most->U() )   { top_most   = Pt1; }
            //down most
            if(Pt1->U() < bot_most->U() )   { bot_most   = Pt1; }
        }
        
        //decide where to draw text
        char str[200];
        sprintf(str,"%llu",(*cit)->TagCode);
        if(left_most->U() < 9*image_param.cols/10)
        {
            cv::putText(image_param, str, cv::Point2f(right_most->U()+5,right_most->V()), cv::FONT_HERSHEY_PLAIN, 1.2,  cv::Scalar(255,0,0,255),2);
        }
        else
        {
            cv::putText(image_param, str, cv::Point2f(left_most->U()-50,left_most->V()), cv::FONT_HERSHEY_PLAIN, 1.2,  cv::Scalar(255,0,0,255),2);
        }
		
	}
}
/*--- End DEBUG ShowCodes ---*/

/*----- DEBUG OutputCodes -----*/
void JMD::JMD_ChromaTag_Decode::DEBUG_OutputCodes()
{
    for (auto& x: myTagCodes)
    {
        //code
        TagCode tmp = x.second;
        
        //convert to binary
        std::bitset<16> bcode0(tmp.myH0Code);
        std::bitset<16> bcode1(tmp.myCode);

        //print
        std::cout << tmp.myH0Code << " (" << bcode0 << ")   <===   " << tmp.myCode << " (" << bcode1 << ")" << std::endl;
    }
}
/*--- End DEBUG OutputCodes ---*/

/*------------- End DEBUG Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
