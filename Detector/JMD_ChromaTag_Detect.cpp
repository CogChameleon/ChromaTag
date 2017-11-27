/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2015
 ==========================
 JMD ChromaTag Detect: Version 1.0
 ==========================
 
 ================================================================
 JMD_ChromaTag_Detect.cpp
 A class for ChromaTag Detection
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

#include "JMD_ChromaTag_Detect.hpp"

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
//================ JMD ChromaTag Detect Implementation ===============//
//====================================================================//
//====================================================================//

//---------------------------------------------------------------//
//-------------------------- Private ----------------------------//
//---------------------------------------------------------------//
        
/*--------------- Setup Methods ---------------*/

/*----- Private Init -----*/
void JMD::JMD_ChromaTag_Detect::Private_Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param)
{
	// color tables
	myLAB_LTable = l_table_param;
    myLAB_ATable = a_table_param;
    myLAB_BTable = b_table_param;

	//settings
	mySettings = settings_param;

	//resize borders for given family
	unsigned int ringNum = 0;
	std::vector<ChromaTag_RingColor> RingColorSequence = { RING_COLOR_REDGREEN };
	switch(mySettings->myTagFamily)
	{
		//3 cases for 3 rings
		case RGBW_16H5:
		case RGBW_25H9:
		case RGBW_36H11: 
		{
			ringNum = 3;  
			myFamilyStr = "RGBW";
			RingColorSequence.push_back(RING_COLOR_GREENBLACK);
			RingColorSequence.push_back(RING_COLOR_BLACKWHITE); 
			break;
		}
		
		// 4 rings
		case RGRBW_36H11: 
		{
			ringNum = 4; 
			myFamilyStr = "RGRBW"; 
			RingColorSequence.push_back(RING_COLOR_GREENRED);
			RingColorSequence.push_back(RING_COLOR_REDBLACK); 
			RingColorSequence.push_back(RING_COLOR_BLACKWHITE); 
			break;
		}
	}
	
	//precompute scan directions
	double rad = 0;
	double inc = 2*JMD::DEF_MATH_PI / mySettings->myInitialScanNum;
	for(int i = 0; i < mySettings->myInitialScanNum; i++)
	{
		JMD_Vision_Point2D tmp( cos(rad), sin(rad) );
		myExpandDirTable.push_back(tmp);
		rad += inc;
	}
	
	//for each preallocated detection object
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		
		//activate detection
		curr_detection->IsActive = true;
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		
		//for each ring on the border
		for(int i = 0; i < ringNum; i++)
		{
			//new ring
			TagBorderRing *newRing = new TagBorderRing();
			
			//for each initial scan point
			for(int j = 0; j < mySettings->myInitialScanNum; j++)
			{
				//create new segment
				TagBorderSegment *newSeg = new TagBorderSegment();
				newSeg->myRingColor = RingColorSequence.at(i);
				
				//push segment
				newRing->push_back(newSeg);
			}
			
			//push ring to border set
			curr_borders->push_back(newRing);
		}
	}
	
	//set border segment scan directions first time
	Private_ResetBorders();
}
/*--- End Private Init ---*/

/*----- Private Detect -----*/
void JMD::JMD_ChromaTag_Detect::Private_Detect( JMD_Vision_Image &image_param )
{
	
}
void JMD::JMD_ChromaTag_Detect::Private_Detect_RGBToLAB( JMD_Vision_Image &image_param )
{
	
}
void JMD::JMD_ChromaTag_Detect::Private_Detect_BGRToLAB( JMD_Vision_Image &image_param )
{
	
}
#ifdef OPENCV_FOUND 
void JMD::JMD_ChromaTag_Detect::Private_Detect( cv::Mat &image_param )
{
	
}
void JMD::JMD_ChromaTag_Detect::Private_Detect_RGBToLAB( cv::Mat &image_param )
{
	
}
void JMD::JMD_ChromaTag_Detect::Private_Detect_BGRToLAB( cv::Mat &image_param )
{
	//setup
	int nr = image_param.rows - mySettings->myBorder;
	int sr = mySettings->myBorder;
	int nc = image_param.cols - mySettings->myBorder;
	int sc = 3*sr;
	unsigned int nc3   = 3 * nc;
	unsigned int samp3 = 3 * mySettings->mySubsample;
	unsigned int seedc = sc - samp3;
	int i,j;
	unsigned char* p;
	unsigned char R,G,B;
	double A, oldA;
	bool badOldA = false;
	unsigned int ActiveTagCount = 0;
	
	// Scan over pixels looking for red, use difference value
	for( i = sr; i < nr; i += mySettings->mySubsample)
	{
		//starting new row, get pointer to row
		p = image_param.ptr<unsigned char>(i);

		//get first pixel for diffs
		B = p[seedc];
		G = p[seedc + 1];
		R = p[seedc + 2];
		oldA = myLAB_ATable->at( HASH_RGB(R,G,B) );

		for( j = sc; j < nc3; j += samp3)
		{
			//skip scanned regions, set oldA as bad
			if( Private_SkipScannedRegion(i,&j) ) { badOldA = true; continue; }

			//get RGB Pixels
			B = p[j];
			G = p[j + 1];
			R = p[j + 2];
			
			//convert to LAB
			A = myLAB_ATable->at( HASH_RGB(R,G,B) );
			
			//if oldA was bad, store new A as oldA and iterate
			if( badOldA ) 
			{
				oldA = A;
				badOldA = false;
				continue;
			}

			//more red than thresh?
			if(A - oldA > mySettings->myLABRedDiffThresh)
			{
				//set active detection border
				myBorders = &(myDetections->at(ActiveTagCount)->TagBorders);
				myDetections->at(ActiveTagCount)->IsActive = true;
				ActiveTagCount++;
				
				//Expand
				if( Private_Expand(image_param,j/3,i,A) )
				{
					//Find Border
					if( Private_Find_Border( image_param ) )
					{
						//refine border
						if( Private_Refine_Border( image_param ) )
						{
							//refine corners
							Private_Refine_Corners( image_param );
						}
						else
						{
							//remove detected borders
							Private_ResetBorder();
							
							//update detections count
							ActiveTagCount--;
							myDetections->at(ActiveTagCount)->IsActive = false;
						}
					}
					else
					{
						//remove detection borders
						Private_ResetBorder();

						//update detections count
						ActiveTagCount--;
						myDetections->at(ActiveTagCount)->IsActive = false;
					}
				}
				else
				{	
					//remove detection borders
					Private_ResetBorder();

					//update detections count
					ActiveTagCount--;
					myDetections->at(ActiveTagCount)->IsActive = false;
				}
			}
		}
	}
	
	//display
	DEBUG_ShowDetections( image_param , 2 );
	//DEBUG_ShowLines( image_param );
	//DEBUG_ShowPoints( image_param );
	//DEBUG_ShowScanOrigins(image_param);
	//cv::namedWindow( "Display window" );
	//cv::imshow( "Display window", image_param );
	//cv::waitKey(0);
}
#endif
/*--- End Private Detect ---*/

/*------------- End Setup Methods -------------*/



/*--------------- Scanning Methods ---------------*/

/*----- Private Expand -----*/
bool JMD::JMD_ChromaTag_Detect::Private_Expand( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param )
{
	
}
#ifdef OPENCV_FOUND
bool JMD::JMD_ChromaTag_Detect::Private_Expand( cv::Mat &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param )
{
	//variables
	TagBorderRing *myCurrRing = nullptr;
	std::vector <TagBorderRing*> myPrevRings;
	unsigned int familyCt = 0;
	bool found = false;
	
	//TagBorderSet Iterator
	TagBorderSet::iterator it = myBorders->begin();
	
	
	//----- Expand Red to Green -----//
	
	//set current ring to first inner ring
	myCurrRing = *it;
	
	//expand w/ provided center point, exit if not found
	found = Private_Expand_RedGreen(image_param, u_param, v_param, red_ref_param, myCurrRing, &myCenterPoint);
	if(!found) { return false; }
	
	//expand again from center point, exit if not found
	found = Private_Expand_RedGreen(image_param, myCenterPoint.U(), myCenterPoint.V(), red_ref_param, myCurrRing, &myCenterPoint);
	if(!found) { return false; }
	
	//expand again from center point, exit if not found
	found = Private_Expand_RedGreen(image_param, myCenterPoint.U(), myCenterPoint.V(), red_ref_param, myCurrRing);
	if(!found) { return false; }
	
	//next ring
	myPrevRings.push_back(myCurrRing);
	++it;
	familyCt++;
	
	//--- End Expand Red to Green ---//
	
	
	//----- Expand Remaining Rings -----//
	
	//expand for each ring
	for(; it != myBorders->end(); ++it)
	{
		//reference to current ring
		myCurrRing = *it;
		
		//Current and Next Color of Rings
		switch(myFamilyStr[familyCt])
		{
			//Red
			case 'R':
				//to Green
				if(myFamilyStr[familyCt+1] == 'G')
				{
					found = Private_Expand_RedGreen(image_param, u_param, v_param, red_ref_param, myCurrRing);
				}
				//to Black
				else
				{
					//found = Private_Expand_RedBlack();
				}
				break;
			
			//Green
			case 'G':
				//to Black
				if(myFamilyStr[familyCt+1] == 'B')
				{
					found = Private_Expand_GreenBlack(image_param, &myPrevRings, myCurrRing);
				}
				//to Red
				else
				{
					//found = Private_Expand_GreenRed();
				}
				break;
			
			//Black
			case 'B': 
			found = Private_Expand_BlackWhite(image_param, &myPrevRings, myCurrRing ); break;
			
			//DEBUGGING: catching problems
			default: std::cout << "PROBLEM WITH TAG FAMILY!" << std::endl;
			
		}
		
		//if not found, exit
		if(!found) 
		{
			//failure occured, but we want to keep the scan information
			//so we store in the outer ring, the last successful scan boundaries
			if(myPrevRings.back() != nullptr)
			{
				Private_Copy_BorderRing( myPrevRings.back(), myBorders->back() );
			} 
			return false; 
		}
		
		//previous ring
		myPrevRings.push_back(myCurrRing);
			
		//update familyCt
		familyCt++;
	}
	//--- End Expand Remaining Rings ---//
	
	//return
	return found;
}
bool JMD::JMD_ChromaTag_Detect::Private_Expand_RedGreen( cv::Mat &image_param, unsigned int u_param, unsigned int v_param, double red_ref_param, TagBorderRing *ring_param, JMD_Vision_Point2D *center_point_param )
{
	//variables
	unsigned int scan_ct = 0;
	double minu = JMD::DEF_PINF, maxu = JMD::DEF_NINF;
	double minv = JMD::DEF_PINF, maxv = JMD::DEF_NINF;
	double dist, dx, dy;
	JMD_Vision_Point2D *currPt;
	
	//for each point on ring
	for(TagBorderRing::iterator it = ring_param->begin(); it != ring_param->end(); ++it)
	{
		scan_ct += Private_Scan_Direction_RedGreen(image_param,red_ref_param,u_param,v_param, (*it)->myScanDir.U(), (*it)->myScanDir.V(), (*it)->myLine.Point1(), &((*it)->myLP1_EdgeColorRef) );
		currPt = (*it)->myLine.Point1();
		
		//finding center point
		if(center_point_param)
		{
			if( currPt->U() > maxu ) { maxu = currPt->U(); }
			if( currPt->U() < minu ) { minu = currPt->U(); }
			if( currPt->V() > maxv ) { maxv = currPt->V(); }
			if( currPt->V() < minv ) { minv = currPt->V(); }
		}
	}
	
	//Found RedGreen?
	if(scan_ct == mySettings->myInitialScanNum) 
	{
		//center point?
		if(center_point_param) 
		{ 
			center_point_param->U( 0.5 * (maxu + minu) ); 
			center_point_param->V( 0.5 * (maxv + minv) );
		} 
		return true; 
	}
	else { return false; }
}
bool JMD::JMD_ChromaTag_Detect::Private_Expand_GreenBlack( cv::Mat &image_param, std::vector<TagBorderRing*> *inner_rings_param, TagBorderRing *outer_ring_param )
{
	//variables
	TagBorderRing *curr_ring;
	JMD_Vision_Point2D *ref_pt;
	unsigned int scan_ct = 0, spoke_ct = 0;
	TagBorderRing *inner_ring_param = inner_rings_param->back();
	std::vector<JMD_Vision_Point2D*> ref_pts;
	
	//for each spoke
	for(TagBorderRing::iterator oit = outer_ring_param->begin(); oit != outer_ring_param->end(); ++oit)
	{
		//clear reference points
		ref_pts.clear();
		
		//populate reference points
		for(std::vector<TagBorderRing*>::iterator iit = inner_rings_param->begin(); iit != inner_rings_param->end(); ++iit)
		{
			//current ring
			curr_ring = *iit;
			
			//push back point along spoke
			ref_pts.push_back( curr_ring->at(spoke_ct)->myLine.Point1() );
		}
		
		//scan
		scan_ct += Private_Scan_Direction_GreenBlack(image_param, &ref_pts, &((*oit)->myScanDir), (*oit)->myLine.Point1(), &((*oit)->myLP1_EdgeColorRef) );
		
		//next spoke
		spoke_ct++;
	}
	
	//Found GreenBlack?
	if(scan_ct == mySettings->myInitialScanNum) { return true; }
	else { return false; }
}
bool JMD::JMD_ChromaTag_Detect::Private_Expand_BlackWhite( cv::Mat &image_param, std::vector<TagBorderRing*> *inner_rings_param, TagBorderRing *outer_ring_param )
{
	//variables
	TagBorderRing *curr_ring;
	JMD_Vision_Point2D *ref_pt;
	unsigned int scan_ct = 0, spoke_ct = 0;
	TagBorderRing *inner_ring_param = inner_rings_param->back();
	std::vector<JMD_Vision_Point2D*> ref_pts;
	
	//for each spoke
	for(TagBorderRing::iterator oit = outer_ring_param->begin(); oit != outer_ring_param->end(); ++oit)
	{
		//clear reference points
		ref_pts.clear();
		
		//populate reference points
		for(std::vector<TagBorderRing*>::iterator iit = inner_rings_param->begin(); iit != inner_rings_param->end(); ++iit)
		{
			//current ring
			curr_ring = *iit;
			
			//push back point along spoke
			ref_pts.push_back( curr_ring->at(spoke_ct)->myLine.Point1() );
		}
		
		//scan
		scan_ct += Private_Scan_Direction_BlackWhite(image_param, &ref_pts, &((*oit)->myScanDir), (*oit)->myLine.Point1(), &((*oit)->myLP1_EdgeColorRef) );
		
		//next spoke
		spoke_ct++;
	}
	
	//Found BlackWhite?
	if(scan_ct == mySettings->myInitialScanNum) { return true; }
	else { return false; }
}
#endif
/*--- End Private Expand ---*/

/*----- Private Skip Scanned Region -----*/
bool JMD::JMD_ChromaTag_Detect::Private_SkipScannedRegion(int i, int *j)
{	
	//variables
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	//for each preallocated detection object
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		
		//Was Active? If not, return false, we have reached the end of active detections
		if(!curr_detection->IsActive) { return false; }
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		
		//outer border ring
		curr_ring = curr_borders->back();
		
		//In Polygon?
		if( Private_InPolygonEstimate(i,j,curr_ring) ) { return true; }
	}
	
	//return false
	return false;
}
/*--- End Private Skip Scanned Region ---*/

/*----- Private Scan Direction -----*/
unsigned int JMD::JMD_ChromaTag_Detect::Private_Scan_Direction_RedGreen( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param )
{
	
}
unsigned int JMD::JMD_ChromaTag_Detect::Private_Scan_Direction_BlackWhite( JMD_Vision_Image &image_param, unsigned int u_param, unsigned int v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param )
{
	
}
#ifdef OPENCV_FOUND
unsigned int JMD::JMD_ChromaTag_Detect::Private_Scan_Direction_RedGreen( cv::Mat &image_param, double red_ref_param, double u_param, double v_param, double u_dir_param, double v_dir_param, JMD_Vision_Point2D *edge_point_param, double *edgecolor_param )
{
	//range
	double urange, vrange;
	Private_Range(image_param.rows, image_param.cols, u_param, v_param, u_dir_param, v_dir_param, &urange, &vrange);
	
	/*--- Variables ---*/
	
	//Found is set to 1 if edge is found
	unsigned int Found = 0;
	
	//hit counts hits on green, misses count misses on red and green
	unsigned int j, uct = 0, vct = 0, hit = 0, miss = 0;
	
	//Int and Double locations of pixels
	double Ud = u_param - u_dir_param;
	double Vd = v_param - v_dir_param;
	unsigned int U = round(Ud), Uold = 0;
	unsigned int V = round(Vd), Vold = 0;
	
	//point to pixel
	unsigned char* p = image_param.ptr<unsigned char>(V);
	
	//RGB to LAB conversions
	unsigned char R,G,B;
	double A;
	double APrev = red_ref_param;
	double ACurr = red_ref_param;
	double ANext = red_ref_param;
	double GreenRef = red_ref_param;
	
	//Finding the Edge
	unsigned int U_Edge, V_Edge;
	double W_Edge;
	double Max_Grad_Edge = JMD::DEF_NINF, Cur_Grad_Edge = JMD::DEF_NINF, Col_Grad_Edge = JMD::DEF_NINF;
	
	/*- End Variables -*/
	
	//scan
	while(vct < vrange && uct < urange)
	{
		//update position, add dirs again if didn't move to a new pixel, increase row and col counts
		Ud = Ud + u_dir_param; U = static_cast<unsigned int>(round(Ud)); 
		Vd = Vd + v_dir_param; V = static_cast<unsigned int>(round(Vd));
		if( V == Vold)
		{
			if(U == Uold) { continue; }
			uct++;
		}
		else 
		{ 
			p = image_param.ptr<unsigned char>(V); 
			vct++;
			if(U != Uold) { uct++; }
		}
		
		//value at new position in LAB
		j = 3 * U;
		B = p[j];
		G = p[j + 1];
		R = p[j + 2];
		A = myLAB_ATable->at( HASH_RGB(R,G,B) );

		//remember
		APrev = ACurr;
		ACurr = ANext;
		ANext = A;

		/*--- Evaluate Pixel ---*/
		
		//hit green?
		Col_Grad_Edge = myVisionProcess->Gradient_Pixel_Ix_Est(red_ref_param,A);
		if(Col_Grad_Edge > mySettings->myRedGreenBorderThresh)
		{
			//largest gradient location
			Cur_Grad_Edge = myVisionProcess->Gradient_Pixel_Ix_Est(APrev,ANext);
			if(Cur_Grad_Edge > Max_Grad_Edge)
			{
				Max_Grad_Edge = Cur_Grad_Edge;
				U_Edge = U;
				V_Edge = V;
				W_Edge = A;
			}

			
			//green reference - lower number = more green in LAB
			if(A < GreenRef) { GreenRef = A; }

			//seen enough green to be satisfied?
			hit++;
			if(hit > mySettings->myHitThreshold)
			{
				Found = 1;
				break;
			}
		}
		/*- End Evaluate Pixel -*/
		
		//update
		Uold = U;
		Vold = V;
	}
	
	//return
	if(edge_point_param) 
	{ 
		edge_point_param->U(U_Edge);
		edge_point_param->V(V_Edge);
		edge_point_param->W(GreenRef);
	}
	if(edgecolor_param) { *edgecolor_param = W_Edge; }
	return Found;
}
unsigned int JMD::JMD_ChromaTag_Detect::Private_Scan_Direction_GreenBlack( cv::Mat &image_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, JMD_Vision_Point2D *scan_dir_param, JMD_Vision_Point2D *edge_point_param, double *edgecolor_param )
{
	//parse input
	JMD_Vision_Point2D *ref_pt = ref_pts_param->back();
	double green_ref_param = ref_pt->W();
	double u_param = ref_pt->U();
	double v_param = ref_pt->V();
	double u_dir_param = scan_dir_param->U();
	double v_dir_param = scan_dir_param->V();
	unsigned char* p;
	unsigned char R,G,B;
	unsigned int j;
	
	//range
	double urange, vrange;
	Private_Range(image_param.rows, image_param.cols, u_param, v_param, u_dir_param, v_dir_param, &urange, &vrange);

	//limit estimates, false if estimate is beyond image region
	double u_edge_est, v_edge_est;
	if( !Private_Edge_Estimate(image_param.rows, image_param.cols, ref_pts_param, u_param, v_param, &urange, &vrange, &u_edge_est, &v_edge_est) )
	{
		//set edge to estimate
		edge_point_param->U(u_edge_est);
		edge_point_param->V(v_edge_est);
		edge_point_param->W(0);
		
		//return
		return 1;
	}
	
	/*--- Variables ---*/
	
	//Found is set to 1 if edge is found
	unsigned int Found = 0;
	
	//hit counts hits on green, misses count misses on red and green
	unsigned int vct = 0, uct = 0, hit = 0, miss = 0;
	
	//Int and Double locations of pixels
	double Ud = u_param - u_dir_param;
	double Vd = v_param - v_dir_param;
	unsigned int U = round(u_param), Uold = 0;
	unsigned int V = round(v_param), Vold = 0;
	
	//point to pixel
	p = image_param.ptr<unsigned char>(V);
	
	//RGB to LAB conversions
	unsigned char L = 0, LNext = 0, LLast = 0;
	double A = green_ref_param, ALast = green_ref_param, ANext = green_ref_param;
	std::vector<double> A_Values;
	std::vector<double> L_Values;
	std::vector<double> U_Positions;
	std::vector<double> V_Positions;
	
	//Finding the Edge
	bool Beyond_RedGreen = false;
	unsigned int U_Edge, V_Edge;
	double W_Edge;
	double Col_Grad_Edge = JMD::DEF_NINF;
	double BlackRef = JMD::DEF_PINF;
	
	/*- End Variables -*/
	
	
	//scan
	while(vct < vrange && uct < urange)
	{
		//update position, add dirs again if didn't move to a new pixel, increase row and col counts
		Ud = Ud + u_dir_param; U = static_cast<unsigned int>(round(Ud)); 
		Vd = Vd + v_dir_param; V = static_cast<unsigned int>(round(Vd));
		if( V == Vold)
		{
			if(U == Uold) { continue; }
			uct++;
		}
		else 
		{ 
			p = image_param.ptr<unsigned char>(V);
			vct++;
			if(U != Uold) { uct++; }
		}
		
		//value at new position in LAB
		j = 3 * U;
		B = p[j];
		G = p[j + 1];
		R = p[j + 2];
		
		//remember A
		ALast = A;
		A = ANext;
		ANext = myLAB_ATable->at( HASH_RGB(R,G,B) );
		
		//Remember L
		LLast = L;
		L = LNext;
		LNext = myVisionProcess->RGBToGray_Luminosity_Pixel(R,G,B);
				
		/*--- Evaluate Pixel ---*/
		//escape red-green border?
		//this checks to make sure we have at least scanned far enough
		//to find one pixel that was similar in color to the green reference
		if(!Beyond_RedGreen)
		{
			//pixel gradient
			Col_Grad_Edge = myVisionProcess->Gradient_Pixel_Ix_Est(ANext,green_ref_param);
		
			//pixel gradient similar to reference green?
			if(Col_Grad_Edge <= mySettings->myGreenBlackBorderThresh)
			{
				Beyond_RedGreen = true;
				LLast = LNext;
				L = LNext;
				ALast = ANext;
				A = ANext;
			}
		}
		else
		{
			//black reference - remember darkest pixel
			if(LNext < BlackRef) { BlackRef = LNext; }
			
			//remember
			A_Values.push_back(ANext - ALast);
			L_Values.push_back(LNext - LLast);
			U_Positions.push_back(Ud);
			V_Positions.push_back(Vd);
		}
		/*- End Evaluate Pixel -*/
		
		//update
		Uold = U;
		Vold = V;
	}
	
	//if never scanned, return
	if(A_Values.empty()) { return 0; }
	
	//scan for max gradients to be edge
	std::vector<double>::iterator ait = A_Values.begin();
	std::vector<double>::iterator lit = L_Values.begin();
	double combined;
	double combined_max = *ait - *lit;
	unsigned int ct = 0;
	unsigned int combined_max_ct   = 0;
	for(; ait != A_Values.end(); ++ait, ++lit )
	{
		combined = *ait - *lit;
		if(combined > combined_max) { combined_max = combined; combined_max_ct = ct; }
		ct++;
	}
	
	//set edge
	unsigned int edge_idx = combined_max_ct;
	U_Edge = static_cast<unsigned int>( U_Positions.at(edge_idx) );
	V_Edge = static_cast<unsigned int>( V_Positions.at(edge_idx) );
	
	//return
	if(edge_point_param) 
	{ 
		edge_point_param->U(U_Edge);
		edge_point_param->V(V_Edge);
		edge_point_param->W(BlackRef);
	}
	if(edgecolor_param) { *edgecolor_param = W_Edge; }
	return 1;
}
unsigned int JMD::JMD_ChromaTag_Detect::Private_Scan_Direction_BlackWhite( cv::Mat &image_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, JMD_Vision_Point2D *scan_dir_param, JMD_Vision_Point2D *edge_point_param, double *edgecolor_param)
{
	//parse input
	JMD_Vision_Point2D *ref_pt = ref_pts_param->back();
	double black_ref_param = ref_pt->W();
	double u_param = ref_pt->U();
	double v_param = ref_pt->V();
	double u_dir_param = scan_dir_param->U();
	double v_dir_param = scan_dir_param->V();
	
	//range
	double urange, vrange;
	Private_Range(image_param.rows, image_param.cols, u_param, v_param, u_dir_param, v_dir_param, &urange, &vrange);
	
	//limit estimates, false if estimate is beyond image region
	double u_edge_est, v_edge_est;
	if( !Private_Edge_Estimate(image_param.rows, image_param.cols, ref_pts_param, u_param, v_param, &urange, &vrange, &u_edge_est, &v_edge_est) )
	{
		//set edge to estimate
		edge_point_param->U(u_edge_est);
		edge_point_param->V(v_edge_est);
		edge_point_param->W(0);
		return 1;
	}
	
	/*--- Variables ---*/
	
	//Found is set to 1 if edge is found
	unsigned int Found = 0;
	
	//hit counts hits on green, misses count misses on red and green
	unsigned int j, vct = 0, uct = 0, hit = 0, miss = 0;
	
	//Int and Double locations of pixels
	double Ud = u_param - u_dir_param;
	double Vd = v_param - v_dir_param;
	unsigned int U = round(u_param), Uold = 0;
	unsigned int V = round(v_param), Vold = 0;
	
	//point to pixel
	unsigned char* p = image_param.ptr<unsigned char>(V);
	
	//RGB to Gray conversions
	unsigned char R,G,B;
	double GrayPrev = 128, GrayCurr = 128, GrayNext = 128;
	std::vector<double> G_Values;
	std::vector<double> U_Positions;
	std::vector<double> V_Positions;
	
	//Finding the Edge
	bool Beyond_GreenBlack = false;
	unsigned int U_Edge, V_Edge, W_Edge;
	double Col_Grad_Edge = JMD::DEF_NINF;
	double WhiteRef = black_ref_param;
	
	/*- End Variables -*/
	
	//scan
	while(vct < vrange && uct < urange)
	{
		//update position, add dirs again if didn't move to a new pixel
		Ud = Ud + u_dir_param; U = static_cast<unsigned int>(round(Ud)); 
		Vd = Vd + v_dir_param; V = static_cast<unsigned int>(round(Vd));
		if( V == Vold)
		{
			if(U == Uold) { continue; }
			uct++;
		}
		else 
		{ 
			p = image_param.ptr<unsigned char>(V);
			vct++;
			if(U != Uold) { uct++; } 
		}
		
		//value at new position in Grayscale
		j = 3 * U;
		B = p[j];
		G = p[j + 1];
		R = p[j + 2];
		
		//Remember L
		GrayPrev = GrayCurr;
		GrayCurr = GrayNext;
		GrayNext = myVisionProcess->RGBToGray_Luminosity_Pixel(R,G,B);
		
		/*--- Evaluate Pixel ---*/
		
		//escape green-black border?
		//this checks to make sure we have at least scanned far enough
		//to find one pixel that was similar in color to the black reference
		if(!Beyond_GreenBlack)
		{
			//pixel gradient
			Col_Grad_Edge = myVisionProcess->Gradient_Pixel_Ix_Est(GrayNext,black_ref_param);
		
			//pixel gradient similar to reference black?
			if(Col_Grad_Edge < mySettings->myBlackWhiteBorderThresh)
			{
				Beyond_GreenBlack = true;
				GrayPrev = GrayNext;
				GrayCurr = GrayNext;
			}
		}
		else
		{
			//white reference - remember lightest pixel
			if(GrayNext > WhiteRef) { WhiteRef = GrayNext; }
			
			//remember
			G_Values.push_back(GrayNext - GrayPrev);
			U_Positions.push_back(Ud);
			V_Positions.push_back(Vd);
		}
		/*- End Evaluate Pixel -*/
		
		//update
		Uold = U;
		Vold = V;
	}
	
	//if never scanned, return
	if(G_Values.empty()) { return Found; }
	
	//scan for max gradients to be edge
	std::vector<double>::iterator git = G_Values.begin();
	double blackwhite_max = *git;
	unsigned int ct = 0;
	unsigned int blackwhite_max_ct = 0;
	for(; git != G_Values.end(); ++git )
	{
		if(*git > blackwhite_max) { blackwhite_max = *git; blackwhite_max_ct = ct; }
		ct++;
	}
	
	//set edge
	U_Edge = static_cast<unsigned int>( U_Positions.at(blackwhite_max_ct) );
	V_Edge = static_cast<unsigned int>( V_Positions.at(blackwhite_max_ct) );
	
	//return
	if(edge_point_param) 
	{ 
		edge_point_param->U(U_Edge);
		edge_point_param->V(V_Edge);
		edge_point_param->W(WhiteRef);
	}
	if(edgecolor_param) { *edgecolor_param = W_Edge; }
	return 1;
}
#endif
/*--- End Private Scan Direction ---*/

/*----- Private Range -----*/
void JMD::JMD_ChromaTag_Detect::Private_Range(int rows_param, int cols_param, double u_param, double v_param, double u_dir_param, double v_dir_param, double *u_range_param, double *v_range_param)
{
	//variables
	double umag, vmag;
	u_param = round(u_param);
	v_param = round(v_param);
	
	//Range in Rows
	if( fabs(u_dir_param) < JMD::DEF_EQUALITY_THRESHOLD ) { umag = JMD::DEF_PINF; }
	else if ( u_dir_param > 0 )                           { umag = cols_param - u_param - 1; }
	else                                                  { umag = u_param - 1; }
	
	//Range in Columns
	if( fabs(v_dir_param) < JMD::DEF_EQUALITY_THRESHOLD ) { vmag = JMD::DEF_PINF; }
	else if ( v_dir_param > 0 )                           { vmag = rows_param - v_param - 1; }
	else                                                  { vmag = v_param - 1; }
	
	//return
	*u_range_param = umag;
	*v_range_param = vmag;
}
/*--- End Private Range ---*/

/*------------- End Scanning Methods -------------*/



/*--------------- Find Border Methods ---------------*/

/*----- Private_Initial_Border -----*/
void JMD::JMD_ChromaTag_Detect::Private_Initial_Border( JMD_Vision_Image &image_param, JMD_Vision_Point2D *center_param )
{
	
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::Private_Initial_Border( cv::Mat &image_param, JMD_Vision_Point2D *center_param )
{
	//variables
	TagBorderRing *ring;
	bool inner_ring = true;
	double centerU = 0, centerV = 0;
	double tmpU, tmpV;
	double minU = JMD::DEF_PINF, maxU = JMD::DEF_NINF, minV = JMD::DEF_PINF, maxV = JMD::DEF_NINF;
	int ct = 0;
	
	//for each border ring
	TagBorderSet::iterator rit = myBorders->begin();
	for(; rit != myBorders->end(); ++rit)
	{
		ring = *rit;
		
		//for each segment
		TagBorderRing::iterator sit = ring->begin();
		for(; sit != ring->end()-1; ++sit)
		{
			//Point 1 Already Set, Set Point 2 to Point 1 of next segment
			(*sit)->myLine.Point2( (*(sit+1))->myLine.Point1() );
			(*sit)->myLP2_EdgeColorRef = (*(sit+1))->myLP1_EdgeColorRef;
			(*sit)->myLength = (*sit)->myLine.Length();
			(*sit)->myLine.Midpoint( &((*sit)->myMidpoint) );
			(*sit)->myLine.Slope( &((*sit)->mySlope) );
			
			//update center calculation
			if(inner_ring)
			{
				tmpU = (*sit)->myLine.Point1()->U();
				tmpV = (*sit)->myLine.Point1()->V();
				if(tmpU > maxU) { maxU = tmpU; }
				else if(tmpU < minU) { minU = tmpU; }
				if(tmpV > maxV) { maxV = tmpV; }
				else if(tmpV < minV) { minV = tmpV; }
			}
		}
		
		//last segment
		(*sit)->myLine.Point2( (*ring->begin())->myLine.Point1() );
		(*sit)->myLP2_EdgeColorRef = (*ring->begin())->myLP1_EdgeColorRef;
		(*sit)->myLength = (*sit)->myLine.Length();
		(*sit)->myLine.Midpoint( &((*sit)->myMidpoint) );
		(*sit)->myLine.Slope( &((*sit)->mySlope) );
		
		//no longer inner ring
		if(inner_ring) 
		{
			//set U and V
			tmpU = (*sit)->myLine.Point1()->U();
			tmpV = (*sit)->myLine.Point1()->V();
			if(tmpU > maxU) { maxU = tmpU; }
			else if(tmpU < minU) { minU = tmpU; }
			if(tmpV > maxV) { maxV = tmpV; }
			else if(tmpV < minV) { minV = tmpV; }
			center_param->U( (maxU + minU) * 0.5 );
			center_param->V( (maxV + minV) * 0.5 );
			
			//set red reference (W)
			unsigned int j = 3 * static_cast<unsigned int>(center_param->U());
			unsigned char* p = image_param.ptr<unsigned char>( static_cast<unsigned int>(center_param->V()) );
			center_param->W( myLAB_ATable->at( HASH_RGB( p[j+2], p[j+1], p[j] ) ) );
			
			//turn off inner_ring flag
			inner_ring = false; 
		}
	}
	
	//update max distance-to-center and area calculation for outer ring
	myMaxExpandDistance = JMD::DEF_NINF;
	for(TagBorderRing::iterator sit = ring->begin(); sit != ring->end(); ++sit)
	{
		//max distance to center
		JMD_Vision_Point2D *pt1 = (*sit)->myLine.Point1();
		JMD_Vision_Point2D *pt2 = (*sit)->myLine.Point2();
		double du1 = pt1->U() - myCenterPoint.U();
		double dv1 = pt1->V() - myCenterPoint.V();
		double du2 = pt2->U() - myCenterPoint.U();
		double dv2 = pt2->V() - myCenterPoint.V();
		double d1 = sqrt( du1*du1 + dv1*dv1 );
		double d2 = sqrt( du2*du2 + dv2*dv2 );
		double D = sqrt( d1*d1 + d2*d2 );
		if( D > myMaxExpandDistance ) { myMaxExpandDistance = D; }
		
		//set neighbor pointers
		TagBorderSegment *left =  sit == ring->begin() ? *(ring->end()-1) : *(sit-1); //wrap around or previous segment
		TagBorderSegment *right = sit == ring->end()-1 ? *(ring->begin()) : *(sit+1); //wrap around or next segment
		
		//calculate area
		(*sit)->myArea = Private_SegmentArea( *sit, left, right, &((*sit)->myTriangleApex) );
	}
}
#endif
/*--- End Private Initial Border ---*/

/*----- Private Find Border -----*/
bool JMD::JMD_ChromaTag_Detect::Private_Find_Border( JMD_Vision_Image &image_param )
{
	
}
#ifdef OPENCV_FOUND
bool JMD::JMD_ChromaTag_Detect::Private_Find_Border( cv::Mat &image_param )
{
	/*----- Build Initial Border -----*/
	
	//Initial Border
	Private_Initial_Border(image_param, &myCenterPoint);
	
	//Initial Area
	double curr_area, full_area;
	Private_FindPolygonArea( myBorders->back(), &curr_area, &full_area );
	
	/*--- End Build Initial Border ---*/
	
	
	/*----- Push Border to Edge -----*/
	int found;
	double max_area, t_param, edge_color;
	JMD_Vision_Point2D EdgePt, IntPt;
	JMD_Vision_Point2D ScanDir, TestDir;
	JMD_Vision_LineSegment2D ScanLine, TestLine;
	TagBorderRing *outer_ring = myBorders->back();
	TagBorderRing *curr_ring;
	std::vector<JMD_Vision_Point2D*> ref_pts;
	for(int ct = 0; ct < mySettings->myMaxInitialBorderIterations; ct++)
	{
		//if tag area is within threshold of total possible area, quit
		if(curr_area / (curr_area+full_area) > mySettings->myAreaRatioThreshold) { return true; }
		
		//get largest area line, break if no active lines
		int idx = Private_FindMaxArea( &max_area );
		if(idx < 0) { return true; }
		TagBorderSegment *line = outer_ring->at(idx);
		
		//Direction Of Scan
		ScanLine.Point1( myCenterPoint.U(), myCenterPoint.V() );
		if(max_area == JMD::DEF_PINF)
		{
			ScanDir.U( line->myMidpoint.U() - myCenterPoint.U() );
			ScanDir.V( line->myMidpoint.V() - myCenterPoint.V() );
			ScanLine.Point2( line->myMidpoint.U(),line->myMidpoint.V() );
		}
		else
		{
			ScanDir.U( line->myTriangleApex.U() - myCenterPoint.U() );
			ScanDir.V( line->myTriangleApex.V() - myCenterPoint.V() );
			ScanLine.Point2( line->myTriangleApex.U(),line->myTriangleApex.V() );
		}

		//normalize direction
		double norm_const = sqrt( ScanDir.U()*ScanDir.U()+ScanDir.V()*ScanDir.V() );
		ScanDir.U( ScanDir.U() / norm_const );
		ScanDir.V( ScanDir.V() / norm_const );
		
		//scan in direction for each ring
		unsigned int familyCt = 0;
		ref_pts.clear();
		for(TagBorderSet::iterator it = myBorders->begin(); it != myBorders->end(); ++it)
		{
			//reference to current ring
			curr_ring = *it;
			switch(myFamilyStr[familyCt])
			{
				//Red
				case 'R':
					//to Green
					if(myFamilyStr[familyCt+1] == 'G')
					{
						//scan to red green border
						JMD_Vision_LineSegment2D::Intersection(&ScanLine,&ScanDir,&(curr_ring->at(idx)->myLine),&(curr_ring->at(idx)->mySlope),&EdgePt);
						found = Private_Scan_Direction_RedGreen(image_param, myCenterPoint.W(), EdgePt.U(), EdgePt.V(), ScanDir.U(), ScanDir.V(), &EdgePt, &edge_color );
					}
					//to Black
					else
					{
						//found = Private_Expand_RedBlack(image_param, u_param, v_param);
					}
					break;
				
				//Green
				case 'G':
					//to Black
					if(myFamilyStr[familyCt+1] == 'B')
					{
						//Testing to see if the scan start point is beyond the ring, we do this by finding
						//the intersection point and seeing the "t" is > 1, this requires though that we
						//use the direction as the full vector size, instead of normalizing it to a length
						//of 1. If the t_param value is < 1, we know that the starting scan point IS beyond
						//the ring, and we start there for the scan, if it is inside the ring, we push the
						//starting position out to the ring
						TestLine.Point1(&myCenterPoint);
						TestLine.Point2(&EdgePt);
						TestDir.U( EdgePt.U() - myCenterPoint.U() );
						TestDir.V( EdgePt.V() - myCenterPoint.V() );
						JMD_Vision_LineSegment2D::Intersection(&(curr_ring->at(idx)->myLine),&(curr_ring->at(idx)->mySlope),&(TestLine),&(TestDir),&IntPt, &t_param);
						
						//Scan
						if(t_param > 1) 
						{ 
							EdgePt.U( IntPt.U() ); 
							EdgePt.V( IntPt.V() ); 
						}
						found = Private_Scan_Direction_GreenBlack(image_param, &ref_pts, &ScanDir, &EdgePt, &edge_color);
						
					}
					//to Red
					else
					{
						//found = Private_Expand_GreenRed(image_param, u_param, v_param);
					}
					break;
				
				//Black
				case 'B': 
				{
					//See Note above. Doing same thing here: checking to see if t_param is > 1. If t_param is
					//> 1, we know that the starting scan point IS beyond the ring, etc...
					TestLine.Point1(&myCenterPoint);
					TestLine.Point2(&EdgePt);
					TestDir.U( EdgePt.U() - myCenterPoint.U() );
					TestDir.V( EdgePt.V() - myCenterPoint.V() );
					JMD_Vision_LineSegment2D::Intersection(&(curr_ring->at(idx)->myLine),&(curr_ring->at(idx)->mySlope),&(TestLine),&(TestDir),&IntPt, &t_param);

					//Scan
					if(t_param > 1) 
					{ 
						EdgePt.U( IntPt.U() );
						EdgePt.V( IntPt.V() );
					}
					found = Private_Scan_Direction_BlackWhite(image_param, &ref_pts, &ScanDir, &EdgePt, &edge_color);
					break;
				}
				
				//should never be here
				default: std::cout << "BAD FAMILY STRING VALUE" << std::endl;
				
			}
			
			//if not found, exit
			if(!found) 
			{
				return false; 
			}
			
			//if point was in slice, try to add segment and update area
			bool pt_in_slice = Private_InSlice( curr_ring->at(idx) , &EdgePt );
			bool was_seg_inserted = false;
			std::vector<int> was_seg_convex;
			if( pt_in_slice )
			{
				//Update area for last ring
				if(it == myBorders->end()-1)
				{
					//if distance to newest point is well beyond initial scan distances
					double du = EdgePt.U() - myCenterPoint.U();
					double dv = EdgePt.V() - myCenterPoint.V();
					double D = sqrt( du*du + dv*dv );
					if( D > mySettings->myMaxExpandDistMultiplier*myMaxExpandDistance )
					{
						return false; 
					}
					
					//force convexity of new point
					bool was_pt_convex = Private_ForceConvex(curr_ring->at(idx), &EdgePt);
					
					//insert new segment after convexity force so that the edge point
					//has been shifted if necessary
					was_seg_inserted = Private_InsertRingSegment(image_param, curr_ring, idx, &EdgePt, edge_color);
					
					//force convexity of neighboring lines, only need to check this if
					//the new edge point is off the edge that was split;
					//otherwise, the point was moved to the edge, so the areas of the lines
					//connected to that point will both be zero, so force them to zero
					//during the UpdatePolygonArea call below
					if( was_seg_inserted && was_pt_convex ) { Private_ForceConvex(curr_ring, idx, was_seg_convex); }
					
					//update areas
					Private_UpdatePolygonArea(curr_ring, idx, &EdgePt, &curr_area, &full_area, was_pt_convex, was_seg_inserted, was_seg_convex);
				}
				else
				{
					//update ring
					was_seg_inserted = Private_InsertRingSegment(image_param, curr_ring, idx, &EdgePt, edge_color);
				}
			}
			
			//if segment was not inserted, we need to go back to previous rings and
			//remove inserted segments, then, we can simply give up on this scan direction
			if(!was_seg_inserted)
			{
				//delete ring segments that have been inserted so far. We set the reverse iterator
				//to the iterator, which will offset it backward by one, and then reverse_iterate
				//back to the beginning of the data structure
				Private_UndoRingSegments( it, idx);
				
				//if this is not the outer ring, then we need to zero the area of the
				//final ring's segment at this index so that we don't continue to scan
				//in the same direction
				if(it != myBorders->end()-1)
				{
					//set current ring to outer ring
					curr_ring = *(myBorders->end()-1);
					
					//Update the segment's area and the full area
					//was_pt_convex forced to true so that no areas are zeroed
					//was_seg_inserted forced to false so that the idx area is zeroed
					//was_seg_convex empty so that no areas are zeroed
					Private_UpdatePolygonArea(curr_ring, idx, &EdgePt, &curr_area, &full_area, true, false, was_seg_convex);
				}
				// however, if the point to add was never in slice, then we need to still do this for the outer ring
				// because the code above will not be reached that would otherwise zero the area
				else if(!pt_in_slice)
				{
					Private_UpdatePolygonArea(curr_ring, idx, &EdgePt, &curr_area, &full_area, true, false, was_seg_convex);
				}
				
				//break from ring iteration loop
				break;
			}
			
			//save reference points
			ref_pts.push_back( new JMD_Vision_Point2D( EdgePt ) );
				
			//update familyCt
			familyCt++;	
		}
	}
	/*--- End Push Border to Edge ---*/
	
	//DEBUG
	//cv::circle(image_param,cv::Point(myCenterPoint.U(),myCenterPoint.V()),2,cv::Scalar(255,255,0),-1);
	//DEBUG_ShowPoint( image_param, &myCenterPoint);
	//DEBUG_ShowPoint( image_param, &IntPt );
	//DEBUG_ShowPoint( image_param, &EdgePt);
	//DEBUG_ShowLines( image_param );
	//DEBUG_ShowPoints( image_param );
	//DEBUG_OutputBorder();
	//DEBUG_ShowPoints( image_param, points_param );
	//DEBUG_OutputBorder(border_param);
	//cv::waitKey(0);
	//exit(0);
}
#endif
/*--- End Private Find Border ---*/

/*----- Private Insert Ring Segment -----*/
bool JMD::JMD_ChromaTag_Detect::Private_InsertRingSegment()
{
	return false;
}
#ifdef OPENCV_FOUND
bool JMD::JMD_ChromaTag_Detect::Private_InsertRingSegment(cv::Mat &image_param, TagBorderRing *ring_param, int index_param, JMD_Vision_Point2D *point_param, double edge_color_param)
{
	//line points
	JMD_Vision_Point2D LineP1, LineP2;
	TagBorderSegment *Segment = ring_param->at(index_param);
	TagBorderSegment *newBS;
	ChromaTag_RingColor curr_ring_color = Segment->myRingColor;
	double LP1_ColorRef = Segment->myLP1_EdgeColorRef, LP2_ColorRef = Segment->myLP2_EdgeColorRef;
	LineP1.U( Segment->myLine.Point1()->U() ); LineP1.V( Segment->myLine.Point1()->V() ); LineP1.W( Segment->myLine.Point1()->W() );
	LineP2.U( Segment->myLine.Point2()->U() ); LineP2.V( Segment->myLine.Point2()->V() ); LineP2.W( Segment->myLine.Point2()->W() );
	
	//check if new point isn't already an edge of line
	if( LineP1 == *point_param || LineP2 == *point_param )
	{
		//return failure
		return false;
	}
	else
	{
		//new line 1 - replace info in current line
		Segment->myLine.Point1( LineP1.U(), LineP1.V(), LineP1.W() );
		Segment->myLine.Point2( point_param->U(), point_param->V(), point_param->W() );
		Segment->myLP2_EdgeColorRef = edge_color_param;
		Segment->myLength = Segment->myLine.Length();
		Segment->myLine.Midpoint( &(Segment->myMidpoint) );
		Segment->myLine.Slope( &(Segment->mySlope) );
		
		newBS = new TagBorderSegment();
		newBS->myLine.Point1( point_param->U(), point_param->V(), point_param->W() );
		newBS->myLine.Point2( LineP2.U(), LineP2.V(), LineP2.W() );
		newBS->myLP1_EdgeColorRef = edge_color_param;
		newBS->myLP2_EdgeColorRef = LP2_ColorRef;
		newBS->myLength = newBS->myLine.Length();
		newBS->myLine.Midpoint( &(newBS->myMidpoint) );
		newBS->myLine.Slope( &(newBS->mySlope) );
		newBS->myRingColor = curr_ring_color;
		newBS->isZeroed = false;
		
		//insert
		ring_param->insert(ring_param->begin() + index_param + 1, newBS);
		
		//return success
		return true;
	}
}
#endif
/*--- End Private Insert Ring Segment ---*/

/*----- Private Undo Ring Segment -----*/
void JMD::JMD_ChromaTag_Detect::Private_UndoRingSegment(TagBorderRing *ring_param, int index_param)
{
	//variables
	int prev_idx, next_idx;
	
	//shift line at index+1 to connect with line at index-1
	prev_idx = index_param <= 0                     ? ring_param->size()-1 : index_param - 1;
	next_idx = index_param >= ring_param->size() - 1 ? 0                   : index_param + 1;
	ring_param->at(next_idx)->myLine.Point1( ring_param->at(prev_idx)->myLine.Point2() );
	
	//update line info
	ring_param->at(next_idx)->myLength = ring_param->at(next_idx)->myLine.Length();
	ring_param->at(next_idx)->myLine.Midpoint( &(ring_param->at(next_idx)->myMidpoint) );
	ring_param->at(next_idx)->myLine.Slope( &(ring_param->at(next_idx)->mySlope) );
	
	//delete line at index
	delete *(ring_param->begin()+index_param);
	ring_param->erase(ring_param->begin() + index_param);
}
void JMD::JMD_ChromaTag_Detect::Private_UndoRingSegments(TagBorderSet::iterator it, int index_param)
{
	//undo each segment
	for(TagBorderSet::reverse_iterator rit(it); rit != myBorders->rend(); ++rit )
	{
		Private_UndoRingSegment(*rit, index_param);
	}
}
/*--- End Private Undo Ring Segment ---*/

/*----- Private Reset Borders -----*/
void JMD::JMD_ChromaTag_Detect::Private_ResetBorders()
{
	//variables
	int ct = 0;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	//for each preallocated detection object
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		
		//Was Active? If not, break, we have reached the end of active detections
		if(!curr_detection->IsActive) { break; }
		else 
		{ 
			curr_detection->IsActive = false;
			curr_detection->IsPose = false;
			curr_detection->IsCode = false;
		}
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		
		//Reset Border
		Private_ResetBorder(curr_borders);
	}
}
void JMD::JMD_ChromaTag_Detect::Private_ResetBorder(TagBorderSet *borders_param)
{
	//variables
	int ct = 0;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	TagBorderSet *curr_borders = ( borders_param == nullptr ? myBorders : borders_param );
	
	//for each border ring
	for(TagBorderSet::iterator rit = curr_borders->begin(); rit != curr_borders->end(); ++rit)
	{
		//current ring
		curr_ring = *rit;
		
		//for each segment
		ct = 0;
		for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); )
		{
			if(ct < mySettings->myInitialScanNum)
			{
				//current segment
				curr_seg = *sit;
			
				//reset slopes
				curr_seg->myScanDir.U( myExpandDirTable.at(ct).U() );
				curr_seg->myScanDir.V( myExpandDirTable.at(ct).V() );
				
				//reset flags
				curr_seg->isZeroed = false;
				
				//increment iterator
				++sit;
			}
			else
			{
				delete *sit;
				sit = curr_ring->erase( sit );
			}
			
			//increment
			ct++;
		}
	}
}
/*--- End Private Reset Borders ---*/

/*----- Private Copy BorderRing -----*/
void JMD::JMD_ChromaTag_Detect::Private_Copy_BorderRing( TagBorderRing *src_param, TagBorderRing *dest_param )
{
	//variables
	TagBorderSegment *curr_src_seg;
	TagBorderSegment *curr_dest_seg;
	int segCt = 0;
	
	//copy each segment
	for( TagBorderRing::iterator it = src_param->begin(); it != src_param->end(); ++it )
	{
		//current src segment
		curr_src_seg  = *it;
		
		//if destination has enough space for segment, we are okay, if not
		//then we need to make a new segment and push it onto the ring
		if(segCt < dest_param->size())
		{
			//current segment
			curr_dest_seg = dest_param->at(segCt);
			
			//copy points of line (should be all we need at this point
			curr_dest_seg->myLine.Point1( curr_src_seg->myLine.Point1() );
			curr_dest_seg->myLine.Point2( curr_src_seg->myLine.Point2() );
		}
		else
		{
			//current segment must be a new segment
			curr_dest_seg = new TagBorderSegment();
			
			//copy points of line (should be all we need at this point
			curr_dest_seg->myLine.Point1( curr_src_seg->myLine.Point1() );
			curr_dest_seg->myLine.Point2( curr_src_seg->myLine.Point2() );
			
			//push segment onto border ring
			dest_param->push_back(curr_dest_seg);
		}
		
		//increment
		segCt++;
	}
}
/*--- End Private Copy BorderRing ---*/

/*------------- End Find Border Methods -------------*/



/*--------------- Refine Border Methods ---------------*/

/*----- Private Refine Border -----*/
void JMD::JMD_ChromaTag_Detect::Private_Refine_Border( JMD_Vision_Image &image_param )
{

}
#ifdef OPENCV_FOUND
bool JMD::JMD_ChromaTag_Detect::Private_Refine_Border( cv::Mat &image_param )
{
	//Variables
	TagBorderRing *ring;
	TagBorderSegment *seg;
	
	//KMeans
	if( !Private_KMeans_Angle(image_param) )
	{
		//return failure
		return false;
	}
	
	//return success
	return true;
}
#endif
/*--- End Private Refine Border ---*/

/*----- Private Refine Corners -----*/
void JMD::JMD_ChromaTag_Detect::Private_Refine_Corners( JMD_Vision_Image &image_param )
{
	
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::Private_Refine_Corners( cv::Mat &image_param )
{
	//if level > 0
	if(mySettings->myRefineCornersLevel)
	{
		//variables
		TagBorderRing    *curr_ring;
		TagBorderSegment *curr_seg;
		TagBorderSegment *prev_seg;
		int cx, cy, tlx, tly, brx, bry, rw, rh, L, rw2;
		double p1x, p1y, p2x, p2y, t;
		cv::Mat patch, cpatch;
		cv::Rect patch_bounds;
		
		/// Set the parameters for subpixel corners
		cv::Size zeroZone = cv::Size( -1, -1 );
		cv::TermCriteria criteria = cv::TermCriteria( cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 40, 0.001 );
		
		//detector
		std::vector<cv::KeyPoint> keypoints;
		std::vector<cv::Point2f> corners;
		cv::Ptr<cv::FeatureDetector> detector( cv::GFTTDetector::create(1) );
		
		//for each ring
		for(TagBorderSet::iterator rit = myBorders->begin(); rit != myBorders->end(); ++rit)
		{
			//current ring
			curr_ring = *rit;
			
			//for each corner
			prev_seg = *(curr_ring->end()-1);
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				//current segment
				curr_seg = *sit;
				p1x = curr_seg->myLine.Point1()->U();
				p1y = curr_seg->myLine.Point1()->V();
				p2x = curr_seg->myLine.Point2()->U();
				p2y = curr_seg->myLine.Point2()->V();
				
				//center point
				cx = p1x;
				cy = p1y;
				
				//make rectangle
				L   = 0.3 * std::max( abs(p1x-p2x), abs(p1y-p2y) );
				rw  = std::max( 15, L );
				rw2 = rw/2;
				tlx  = std::max( cx - rw2, 0 );
				tly  = std::max( cy - rw2, 0 );
				if(tlx > image_param.cols-1 || tly > image_param.rows-1)
				{
					//update prev_seg
					prev_seg = curr_seg;
					continue;
				}
				brx  = std::min( cx + rw2, image_param.cols-1 );
				bry  = std::min( cy + rw2, image_param.rows-1 );
				if(brx < 0 || bry < 0)
				{
					//update prev_seg
					prev_seg = curr_seg;
					continue;
				}
				patch_bounds = cv::Rect( tlx, tly, brx-tlx, bry-tly );
				
				//make patch
				patch = image_param(patch_bounds);
				
				//convert to colorspace
				switch(curr_seg->myRingColor)
				{
					case RING_COLOR_REDGREEN: 
						cv::cvtColor(patch,cpatch,CV_BGR2Lab); 
						cv::extractChannel(cpatch,cpatch,1); 
						break;
					case RING_COLOR_GREENRED: break;
					case RING_COLOR_REDBLACK: break;
					case RING_COLOR_GREENBLACK: 
						cv::cvtColor(patch,cpatch,CV_BGR2Lab); 
						cv::extractChannel(cpatch,cpatch,0); 
						break;
					case RING_COLOR_BLACKWHITE:
						cv::cvtColor(patch,cpatch,CV_BGR2Lab);
						cv::extractChannel(cpatch,cpatch,0); 
						break;
					default: std::cout << "BAD RING COLOR: " << curr_seg->myRingColor << std::endl; continue; // should never be here
				}
				
				//detect keypoints
				detector->detect(cpatch, keypoints);
							
				//if keypoint found
				if( !keypoints.empty() )
				{
					//sub pixel if myRefineCornersLevel > 1
					if(mySettings->myRefineCornersLevel > 1)
					{
						//convert keypoints to corners
						cv::KeyPoint::convert(keypoints,corners);
						
						//window size
						int ws = 3;
						int cx = corners[0].x;
						int cy = corners[0].y;
						int lx = max(0,cx - ws);
						int ty = max(0,cy - ws);
						int rx = min(cpatch.cols,cx + ws);
						int by = min(cpatch.rows,cy + ws);
						int ww = min( cx-(lx+1) , (rx-1)-cx );
						int wh = min( cy-(ty+1) , (by-1)-cy );
						cv::Size winSize = cv::Size( ww, wh );
						
						//Calculate the refined corner locations
						cv::cornerSubPix( cpatch, corners, winSize, zeroZone, criteria );
						
						//update line posiiton
						curr_seg->myLine.Point1()->U( corners[0].x + patch_bounds.x );
						curr_seg->myLine.Point1()->V( corners[0].y + patch_bounds.y );
						prev_seg->myLine.Point2()->U( corners[0].x + patch_bounds.x );
						prev_seg->myLine.Point2()->V( corners[0].y + patch_bounds.y );
					}
					else
					{	
						//update line position
						curr_seg->myLine.Point1()->U( keypoints[0].pt.x + patch_bounds.x );
						curr_seg->myLine.Point1()->V( keypoints[0].pt.y + patch_bounds.y );
						prev_seg->myLine.Point2()->U( keypoints[0].pt.x + patch_bounds.x );
						prev_seg->myLine.Point2()->V( keypoints[0].pt.y + patch_bounds.y );
					}
					
				}
				
				//update prev_seg
				prev_seg = curr_seg;
			}
		}
	}
}
#endif
/*--- End Private Refine Corners ---*/

/*----- Private Circular Distance -----*/
double JMD::JMD_ChromaTag_Detect::Private_CircularDistance(double angle_param, double ref_param)
{
	//Math Library for Modulo
	JMD_Utils_Math myMath;
	
	// (((A1 - A2) + pi) % 2pi) - pi
	return myMath.Modulo((angle_param - ref_param) + JMD::DEF_MATH_PI, 2*JMD::DEF_MATH_PI) - JMD::DEF_MATH_PI;
}
/*--- End Private Circular Distance ---*/

/*----- Private KMeans Angle -----*/
bool JMD::JMD_ChromaTag_Detect::Private_KMeans_Angle(cv::Mat &image_param)
{
	//variables
	TagBorderRing *curr_ring;
	
	//for each ring
	for(TagBorderSet::iterator sit = myBorders->begin(); sit != myBorders->end(); ++sit)
	{
		//current ring
		curr_ring = *sit;
		
		//data and weights	
		std::vector<double> angles;
		std::vector<double> lengths;
		for(TagBorderRing::iterator rit = curr_ring->begin(); rit != curr_ring->end(); ++rit)
		{
			angles.push_back( atan2( (*rit)->mySlope.V() , (*rit)->mySlope.U() )  );
			lengths.push_back( (*rit)->myLength );
		}
		
		//kmeans
		double split = JMD::DEF_MATH_PI / 2.0;
		std::vector<double> means { angles[0], angles[0]+split, angles[0]+2*split, angles[0]+3*split };
		std::vector<int> clusters(angles.size());
		if( !Private_KMeans(image_param, angles, lengths, &means, &clusters ) )
		{
			return false;
		}
		
		/*--- Line Base Point ---*/
		// in this section, we find the anchor point that the line will go through
		// Using this point and the means, we have all the information we need for
		// our four lines
		
		// line points
		std::vector<JMD_Vision_Point2D> line_points(4);
		
		//for each mean
		for( int mct = 0; mct < means.size(); mct++ )
		{
			//current mean angle
			double mean = means[mct];
				
			//for each angle in cluster
			double min_dist = JMD::DEF_NINF;
			int ang_idx = -1;
			double sumu = 0;
			double sumv = 0;
			int ptct = 0;
			for( int act = 0; act < angles.size(); act++ )
			{
				//angle in mean
				if( clusters[act] == mct )
				{
					//circular distance to mean
					if(1)
					{
						double cdist = Private_CircularDistance(angles[act], mean);
						
						//distance is zero
						if( abs(cdist) < JMD::DEF_EQUALITY_THRESHOLD )
						{
							ang_idx = act;
							break;
						}
						
						//negative distance closest to zero
						if( cdist < 0 && cdist >= min_dist )
						{
							ang_idx = act;
							min_dist = cdist;
						}
					}
					// weighted average of points
					else
					{
						JMD_Vision_Point2D *pt1 = curr_ring->at(act)->myLine.Point1();
						JMD_Vision_Point2D *pt2 = curr_ring->at(act)->myLine.Point2();
						sumu += lengths.at(act) * pt1->U();
						sumv += lengths.at(act) * pt1->V();
						sumu += lengths.at(act) * pt2->U();
						sumv += lengths.at(act) * pt2->V();
						ptct += 2*lengths.at(act);
					}
				}
			}
			
			//at this point, we found the index of the segment with the closest negative
			//angle. The next line will be the closest positive angle, so we just take point 2
			//which is the shared point between these lines
			line_points.at(mct) = JMD_Vision_Point2D(curr_ring->at(ang_idx)->myLine.Point2()->U(), curr_ring->at(ang_idx)->myLine.Point2()->V());
		}
		/*- End Line Base Point -*/
		
		/*----- Shrink Border to Four Lines -----*/
		// in this section, we remove all the lines except four because
		// we will populate those four lines with the final four corners
		// of each ring
		
		//for each segment
		int segct = 0;
		for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); )
		{
			if(segct < 4)
			{	
				//increment iterator
				++sit;
			}
			else
			{
				delete *sit;
				sit = curr_ring->erase( sit );
			}
			
			//increment
			segct++;
		}
		
		/*--- End Shrink Border to Four Lines ---*/
		
		
		/*--- Fit Lines ---*/
		// in this section, we calculate the four corners by finding
		// the intersections of the four lines
		
		//for each line
		for(int lct = 0; lct < line_points.size(); lct++)
		{
			// line indices
			int l1_idx = lct;
			int l2_idx = (lct+1) % line_points.size();
			
			//calculate intersection
			JMD_Vision_LineSegment2D l1( line_points.at(l1_idx).U(), line_points.at(l1_idx).V(), line_points.at(l1_idx).U(), line_points.at(l1_idx).V() );
			JMD_Vision_LineSegment2D l2( line_points.at(l2_idx).U(), line_points.at(l2_idx).V(), line_points.at(l2_idx).U(), line_points.at(l2_idx).V() );
			JMD_Vision_Point2D slope1( cos(means[l1_idx]), sin(means[l1_idx]) );
			JMD_Vision_Point2D slope2( cos(means[l2_idx]), sin(means[l2_idx]) );
			JMD_Vision_Point2D intPt;
			JMD_Vision_LineSegment2D::Intersection(&l1, &slope1, &l2, &slope2, &intPt);
			
			//update ring
			curr_ring->at(l1_idx)->myLine.Point2( intPt.U(), intPt.V() );
			curr_ring->at(l2_idx)->myLine.Point1( intPt.U(), intPt.V() );
		}
		
		/*- End Fit Lines -*/
	}
	
	//success
	return true;
}
bool JMD::JMD_ChromaTag_Detect::Private_KMeans(cv::Mat &image_param, std::vector<double> &data, std::vector<double> &weights, std::vector<double> *means, std::vector<int> *clusters)
{
	// means
	std::vector<double> oldmeans(4);
	
	//KMeans Cluster: iterate a max of 10 times
	for(int i = 0; i < mySettings->myMaxBorderRefineIterations; i++)
	{
		/*--- Assign ---*/
		// assign each angle to a cluster
		
		//for each angle
		for(int act = 0; act < data.size(); act++)
		{
			//for each mean
			double mindist = JMD::DEF_PINF;
			for(int cct = 0; cct < means->size(); cct++)
			{
				//distance to mean
				double dist = fabs( Private_CircularDistance( data[act], means->at(cct) ) );
				
				//min distance?
				if(dist < mindist)
				{
					mindist = dist;
					clusters->at(act) = cct;
				}
			}
		}
		/*- End Assign -*/
		
		/*--- Means ---*/
		// update means based on assignment
		
		//copy means to oldmeans
		oldmeans = (*means);
		
		//for each angle, add to cluster sum
		std::vector<double> sums(4);
		std::vector<double> refs(4);
		std::vector<double> counts(4);
		std::vector<bool> flag(4);
		for(int act = 0; act < data.size(); act++)
		{
			//get cluster
			int cluster = clusters->at(act);
			
			//if first value, save reference
			if( !flag[cluster] )
			{
				flag[cluster] = true;
				refs[cluster] = data[act];
			}
			
			//add angle to correct sum
			sums[ cluster ] += weights[act] * Private_CircularDistance(data[act], refs[cluster]);
			
			//keep track of total count for each sum
			counts[ cluster ] += weights[act];
		}
		
		
		//for each cluster, divide by count to get new means
		for(int cct = 0; cct < means->size(); cct++)
		{
			//check that all clusters were represented, quit if not
			if( counts[cct] < JMD::DEF_EQUALITY_THRESHOLD ) { return false; }
			
			// calculate mean
			means->at(cct) = Private_CircularDistance( refs[cct] + sums[cct] / counts[cct], 0.0 );
		}
		
		//if means didn't change, finish
		if((*means) == oldmeans)
		{
			break;
		}
		/*- End Means -*/
	}
	
	//return success
	return true;
}
/*--- End Private KMeans Angle ---*/

/*------------- End Refine Border Methods -------------*/



/*--------------- Polygon Geometry Methods ---------------*/

/*----- Private Edge Estimate -----*/
bool JMD::JMD_ChromaTag_Detect::Private_Edge_Estimate(unsigned int rows_param, unsigned int cols_param, std::vector<JMD_Vision_Point2D*> *ref_pts_param, double u_param, double v_param, double *u_range_param, double *v_range_param, double *u_edge_est_param, double *v_edge_est_param)
{
	//variables
	double udir, vdir, ulimit, vlimit;
	double sumu = 0, sumv = 0;
	std::vector<JMD_Vision_Point2D> dist_pts_u, dist_pts_v;

	//calculate limit estimates
	if(ref_pts_param->size() > 1) //fit line for distance
	{
		//multiple points, get distances from last point
		unsigned int pt_ct = 0;
		double lastu = myCenterPoint.U();
		double lastv = myCenterPoint.V();
		for( std::vector<JMD_Vision_Point2D*>::iterator it = ref_pts_param->begin(); it != ref_pts_param->end(); ++it )
		{
			//dist between current point and last point
			double u_dist = (*it)->U() - lastu;
			double v_dist = (*it)->V() - lastv;
			
			//sum
			sumu += u_dist;
			sumv += v_dist;
			
			//update data
			lastu = (*it)->U();
			lastv = (*it)->V();
			pt_ct++;
		}
		
		//mean
		udir = sumu / pt_ct;
		vdir = sumv / pt_ct;
		
		//enofrce min limits
		if( fabs(udir) < 4 ) { udir >= 0 ? udir = 4 : udir = -4; }
		if( fabs(vdir) < 4 ) { vdir >= 0 ? vdir = 4 : vdir = -4; }
		
		//scan max
		ulimit = u_param + 1.5 * udir;
		vlimit = v_param + 1.5 * vdir;
	}
	else
	{	
		//only one point, just take 1.5 * direction
		udir = u_param - myCenterPoint.U();
		vdir = v_param - myCenterPoint.V();
		
		//enforce min limits
		if( fabs(udir) < 4 ) { udir >= 0 ? udir = 4 : udir = -4; }
		if( fabs(vdir) < 4 ) { vdir >= 0 ? vdir = 4 : vdir = -4; }
		
		//scan max
		ulimit = u_param + 1.5 * (udir);
		vlimit = v_param + 1.5 * (vdir);
	}
	
	//check that they are within image region
	if(ulimit < 0 || vlimit < 0 || ulimit > cols_param || vlimit > rows_param)
	{
		//if not in range, calculate edge estimate and return
		*u_edge_est_param = u_param + udir;
		*v_edge_est_param = v_param + vdir;
		
		// return
		return false;
	}
	
	//edit urange and vrange
	if(*u_range_param != JMD::DEF_PINF) { *u_range_param = abs(ulimit - u_param); }
	if(*v_range_param != JMD::DEF_PINF) { *v_range_param = abs(vlimit - v_param); }
	
	//return
	return true;
}
/*--- End Private Edge Estimate ---*/

/*----- Private Find Max Area -----*/
int JMD::JMD_ChromaTag_Detect::Private_FindMaxArea(double *area_param)
{
	//variables
	double max_area = -1, max_length = -1;
	int max_index = -1;
	int ct = 0;
	
	//last ring
	TagBorderRing *ring = myBorders->back();
	
	//iterate over ring for max area
	for(TagBorderRing::iterator sit = ring->begin(); sit != ring->end(); ++sit)
	{	
		//if infinite area, set and look at line lengths
		if (max_area == JMD::DEF_PINF)
		{
			// if current segment is also inf, look at line length
			if( (*sit)->myArea == JMD::DEF_PINF && (*sit)->myLength > max_length )
			{
				max_length = (*sit)->myLength;
				max_index = ct;
			}
		}
		else if ( (*sit)->myArea > max_area )
		{
			max_area = (*sit)->myArea;
			max_length = (*sit)->myLength;
			max_index = ct;
		}
		
		ct++;
	}
	
	//return
	if(area_param) { *area_param = max_area; }
	return max_index;
}
/*--- End Private Find Max Area ---*/

/*----- Private Find Polygon Area -----*/
void JMD::JMD_ChromaTag_Detect::Private_FindPolygonArea(TagBorderRing *ring_param, double *polygon_param, double *possible_param)
{
	//variables
	TagBorderSegment *seg;
	double poly_area = 0, poss_area = 0;
	int ct = 0;
	
	//iterate over vertices
	for(TagBorderRing::iterator it = ring_param->begin(); it != ring_param->end(); ++it)
	{
		//segment
		seg = (*it);
		
		//area for segment
		poly_area += ( seg->myLine.Point1()->U() * seg->myLine.Point2()->V() - seg->myLine.Point1()->V() * seg->myLine.Point2()->U() );
		
		//possible area
		if(poss_area != JMD::DEF_PINF && seg->myArea != JMD::DEF_PINF) { poss_area += seg->myArea; }
		else { poss_area = JMD::DEF_PINF; }
		
		//increment
		ct++;
	}
	
	//return
	*polygon_param = ct > 2 ? 0.5 * poly_area : 0;
	*possible_param = poss_area;
}
/*--- End Private Find Polygon Area ---*/

/*----- Private Update Polygon Area -----*/
void JMD::JMD_ChromaTag_Detect::Private_UpdatePolygonArea(TagBorderRing *ring_param, int index_param, JMD_Vision_Point2D *apex_param, double *polygon_param, double *possible_param, bool was_pt_convex_param, bool was_seg_inserted_param, std::vector<int> &was_seg_convex_param)
{
	//variables
	double new_area;
	int ct = 0;
	TagBorderSegment *segment;
	
	/*----- update polygon area -----*/
	
	// if the segment was not inserted, the area will not change
	// if the point was not convex, the area will not change
	if(was_seg_inserted_param && was_pt_convex_param)
	{
		//find base point
		int pt2_idx = index_param + 1 >= ring_param->size() ? 0 : index_param + 1;
		JMD_Vision_Point2D base_pt;
		JMD_Vision_LineSegment2D ProjLine( ring_param->at(index_param)->myLine.Point1(), ring_param->at(pt2_idx)->myLine.Point2() );
		ProjLine.Projection(apex_param, &base_pt);
		
		//line from base to apex
		JMD_Vision_LineSegment2D HeightLine(&base_pt,apex_param);
		
		//area = 0.5 * base_to_apex_height * base_length
		new_area = 0.5 * ProjLine.Length() * HeightLine.Length();
		*polygon_param = *polygon_param + new_area;
	}
	
	/*--- end update polygon area ---*/
	
	/*----- update possible area -----*/
	new_area = 0;
	int ring_val_m1 = (index_param-1 >= 0)                 ? index_param-1 : ring_param->size()-1;
	int ring_val_p1 = (index_param+1 < ring_param->size()) ? index_param+1 : 0;
	int ring_val_p2 = (ring_val_p1+1 < ring_param->size()) ? ring_val_p1+1 : 0; 
	
	//for each segment in polygon
	for(TagBorderRing::iterator it = ring_param->begin(); it != ring_param->end(); ++it)
	{
		//current segment
		segment = *it;
		
		/*--- did segment possible area change? ---*/
		
		//if segment is already zeroed, skip
		if(segment->isZeroed) 
		{ 
			//skip
		}
		//if the segment was not added, this means the scan ended on
		//a point that is already part of the border points, so we zero
		//the area of the segment that prompted the scan
		else if( !was_seg_inserted_param && (ct == index_param) )
		{
			segment->myArea = 0;
			segment->isZeroed = true;
		}
		//if added point was not convex, it was pushed to the border
		//so we can force the possible area for the two segments attached
		//to that point to zero
		else if(!was_pt_convex_param && (ct == index_param || ct == ring_val_p1) )
		{
			segment->myArea = 0;
			segment->isZeroed = true;
		}
		//if any lines were made convex, check now
		else if( std::find(was_seg_convex_param.begin(), was_seg_convex_param.end(), ct) != was_seg_convex_param.end() )
		{
			segment->myArea = 0;
			segment->isZeroed = true;
		}
		//point was convex and added lines were also convex, so we just calculate the new
		//areas of all four lines
		else if(ct == index_param || ct == ring_val_m1 || ct == ring_val_p1 || ct == ring_val_p2)
		{
			//set neighbor pointers
			TagBorderSegment *left  = it == ring_param->begin() ? *(ring_param->end()-1) : *(it-1); //wrap around or previous segment
			TagBorderSegment *right = it == ring_param->end()-1 ? *(ring_param->begin()) : *(it+1); //wrap around or next segment
			
			//calculate area
			segment->myArea = Private_SegmentArea( segment, left, right, &((*it)->myTriangleApex) );
			
			//zeroed?
			if(segment->myArea == 0.0) { segment->isZeroed = true; }
			
			//update new area
			if(new_area != JMD::DEF_PINF && segment->myArea != JMD::DEF_PINF) {	new_area += segment->myArea; }
			else { new_area = JMD::DEF_PINF; }
		}
		//if we are looking at segments that were never affected by the addition of the new point
		else
		{
			new_area += segment->myArea;
		}
		/*- end did segment possible area change? -*/
		
		//next segment
		ct++;
	}
	*possible_param = new_area;
	/*--- end update possible area ---*/
}
/*--- End Private Update Polygon Area ---*/

/*----- Private Segment Area -----*/
double JMD::JMD_ChromaTag_Detect::Private_SegmentArea(TagBorderSegment *base_param, TagBorderSegment *neighbor_left_param, TagBorderSegment *neighbor_right_param, JMD_Vision_Point2D *apex_param )
{
	//variables
	JMD_Vision_Point2D IPt1;
	JMD_Vision_Point2D Pt1V;
	JMD_Vision_Point2D ProjPt;
	JMD_Vision_LineSegment2D ProjLine;
	double t1, t2;
	
	//If Base is colinear with either neighbor, area = 0
	if( Private_IsColinear(base_param, neighbor_left_param) || Private_IsColinear(base_param, neighbor_right_param) )
	{
		//set triangle apex to base midpoint
		apex_param->U( base_param->myMidpoint.U() );
		apex_param->V( base_param->myMidpoint.V() );
		
		//return 0 area
		return 0.0;
	}
	
	//Get Intersection of Neighboring Lines
	if( !JMD_Vision_LineSegment2D::Intersection( &(neighbor_left_param->myLine), &(neighbor_left_param->mySlope), &(neighbor_right_param->myLine), &(neighbor_right_param->mySlope),&IPt1, &t2, &t1) )
	{
		//parallel lines, infinite area
		return JMD::DEF_PINF;
	}
	
	//IPt1 to Vector
	Pt1V.U( IPt1.U() - myCenterPoint.U() );
	Pt1V.V( IPt1.V() - myCenterPoint.V() );
	
	//Is Intersection In Direction of base?
	if( t2 < JMD::DEF_EQUALITY_THRESHOLD  &&  t1 > (1.0-JMD::DEF_EQUALITY_THRESHOLD) )
	{
		//triangle apex
		apex_param->U( IPt1.U() );
		apex_param->V( IPt1.V() );
		
		//area of triangle
		return 0.5 * base_param->myLength * base_param->myLine.DistanceTo(&IPt1);
	}
	//No ==> Infinite
	else
	{
		return JMD::DEF_PINF;
	}
}
/*--- End Private Segment Area ---*/

/*----- Private Is Colinear -----*/
bool JMD::JMD_ChromaTag_Detect::Private_IsColinear(TagBorderSegment *seg1_param, TagBorderSegment *seg2_param)
{
	//variables
	double v1x = seg1_param->mySlope.U();
	double v2x = seg2_param->mySlope.U();
	double v1y = seg1_param->mySlope.V();
	double v2y = seg2_param->mySlope.V();
	
	//dot product = 1?
	if( abs( v1x*v2x + v1y*v2y ) > (1-JMD::DEF_EQUALITY_THRESHOLD) )
	{
		//colinear
		return true;
	}
	
	//not colinear
	return false;
}
/*--- End Private Is Colinear ---*/

/*----- Private Is Convex -----*/
bool JMD::JMD_ChromaTag_Detect::Private_IsConvex()
{
	//variables
	bool is_convex = true;
	
	//iterate over rings
	for(TagBorderSet::iterator it = myBorders->begin(); it != myBorders->end(); ++it)
	{
		TagBorderRing *curr_ring = *it;
		is_convex = is_convex & Private_IsConvex(curr_ring);
	}
	
	//return
	return is_convex;
}
bool JMD::JMD_ChromaTag_Detect::Private_IsConvex(TagBorderRing *ring_param)
{
	//variables
	TagBorderSegment *pk1, *pk2, *pk3;
	double dx1, dx2, dy1, dy2;
	double zcross;
	bool sign;
	int N = ring_param->size();
	
	//all 3 point polygons are convex
	if(N < 4) { return true; }
	
	//for each segment
	for(int ct = 0; ct < N; ct++)
	{
		//grab correct segments
		pk1 = ring_param->at( (ct  ) % N);
		pk2 = ring_param->at( (ct+1) % N);
		pk3 = ring_param->at( (ct+2) % N);
		
		//calculate d's
		dx1 = pk2->myLine.Point1()->U() - pk1->myLine.Point1()->U();
		dy1 = pk2->myLine.Point1()->V() - pk1->myLine.Point1()->V();
		dx2 = pk3->myLine.Point1()->U() - pk2->myLine.Point1()->U();
		dy2 = pk3->myLine.Point1()->V() - pk2->myLine.Point1()->V();
		
		//cross product
		zcross = dx1*dy2 - dy1*dx2;
		
		//sign
		if(ct == 0) { sign = zcross > 0; }
		else
		{
			//sign changes, return false, not convex
			if( sign != (zcross > 0) ) { return false; }
		} 
	}
	
	//return true, yes convex
	return true;
}
bool JMD::JMD_ChromaTag_Detect::Private_IsConvex(JMD_Vision_LineSegment2D *seg1_param, JMD_Vision_LineSegment2D *seg2_param)
{
	//vector directions of lines
	double v1x = seg1_param->Point2()->U() - seg1_param->Point1()->U();
	double v2x = seg2_param->Point2()->U() - seg2_param->Point1()->U();
	double v1y = seg1_param->Point2()->V() - seg1_param->Point1()->V();
	double v2y = seg2_param->Point2()->V() - seg2_param->Point1()->V();
	
	//normalize
	double v1n = sqrt(v1x * v1x + v1y * v1y);
	double v2n = sqrt(v2x * v2x + v2y * v2y);
	v1x /= v1n; 
	v1y /= v1n;
	v2x /= v2n; 
	v2y /= v2n;
	
	//angle of two lines = acos( (v1 . v2) / (|v1||v2|) )
	double angle = atan2( v2y,v2x ) - atan2( v1y,v1x );
	//double angle = acos( (v1x*v2x + v1y*v2y) / ( sqrt(v1x*v1x+v1y*v1y) * sqrt(v2x*v2x+v2y*v2y) ) );
	
	//return true if angle between (0,Pi) or (-Pi,-2Pi)
	return (angle >= 0 && angle <= JMD::DEF_MATH_PI) || angle <= -1 * JMD::DEF_MATH_PI;
}
/*--- End Private Is Convex ---*/

/*----- Private Force Convex -----*/
void JMD::JMD_ChromaTag_Detect::Private_ForceConvex(TagBorderRing *ring_param, int index_param, std::vector<int> &was_convex_segidx_param)
{
	//indices for which segments we are working with
	int seg_idx_m1 = index_param;
	int seg_idx_m2 = (seg_idx_m1-1  >= 0)                  ? seg_idx_m1-1  : ring_param->size()-1;
	int seg_idx_p1 = (index_param+1 <  ring_param->size()) ? index_param+1 : 0;
	int seg_idx_p2 = (seg_idx_p1+1  <  ring_param->size()) ? seg_idx_p1+1  : 0;
	
	//flags for moving clockwise and counter clockwise
	bool move_clock   = true;
	bool move_counter = true;
	
	//flags to know if we did at least convexity fix
	bool one_fix_clock = false;
	bool one_fix_counter = false;
	
	//for each segment pair
	unsigned int iters = static_cast<unsigned int>(ring_param->size() / 2);
	for(int i = 0; i < iters; i++)
	{
		/*--- move counter one segment ---*/
		
		//if still moving
		if( move_counter )
		{
			//check if not convex
			if( !Private_IsConvex( &(ring_param->at(seg_idx_m2)->myLine), &(ring_param->at(seg_idx_m1)->myLine) ) )
			{
				//update was_convex indices, always add m2, but no need to add m1 after
				//the first fix because it will be redundant since we only shift by 1 segment
				if( !one_fix_counter )
				{
					was_convex_segidx_param.push_back(seg_idx_m1);
					one_fix_counter = true;
				}
				was_convex_segidx_param.push_back(seg_idx_m2);
				
				//force convex by moving shared point out to make the two lines parallel
				Private_ForceConvex( ring_param->at(seg_idx_m2), ring_param->at(seg_idx_m1) );
								
				//shift
				seg_idx_m1 = seg_idx_m2;
				seg_idx_m2--;
				if( seg_idx_m2 < 0 ) { seg_idx_m2 = ring_param->size()-1; }
			}
			//convex, stop moving counter clockwise
			else 
			{ 
				move_counter = false;
				
				//if we did move at least one segment, we need to fix the area for final neighboring segment
				if( one_fix_counter )
				{
					//if the neighboring segment isn't zeroed, update area
					if( !(ring_param->at(seg_idx_m2)->isZeroed) )
					{
						int seg_idx_m3 = (seg_idx_m2-1 >= 0) ? seg_idx_m2-1 : ring_param->size()-1;
						ring_param->at(seg_idx_m2)->myArea = Private_SegmentArea( ring_param->at(seg_idx_m2), ring_param->at(seg_idx_m3), ring_param->at(seg_idx_m1), &( ring_param->at(seg_idx_m2)->myTriangleApex ) );
					}
				}
			}
		}
		
		/*- end move counter one segment -*/
		
		
		/*--- move clockwise one segment ---*/
		
		//if still moving
		if( move_clock )
		{
			//check if not convex
			if( !Private_IsConvex( &(ring_param->at(seg_idx_p1)->myLine), &(ring_param->at(seg_idx_p2)->myLine) ) )
			{
				//update was_convex indices
				if( !one_fix_clock )
				{
					was_convex_segidx_param.push_back(seg_idx_p1);
					one_fix_clock = true;
				}
				was_convex_segidx_param.push_back(seg_idx_p2);

				//force convex by moving shared point out to make the two lines parallel
				Private_ForceConvex( ring_param->at(seg_idx_p1), ring_param->at(seg_idx_p2) );
				
				//update areas
				
				//shift
				seg_idx_p1 = seg_idx_p2;
				seg_idx_p2++;
				if( seg_idx_p2 >= ring_param->size() ) { seg_idx_p2 = 0; }
			}
			//convex, stop moving clockwise
			else 
			{ 
				move_clock = false;
				
				//if we did move at least one segment, we need to fix the area for final neighboring segment
				if( one_fix_clock )
				{
					//if the neighboring segment isn't zeroed, update area
					if( !(ring_param->at(seg_idx_p2)->isZeroed) )
					{
						int seg_idx_p3 = (seg_idx_p2+1 < ring_param->size()) ? seg_idx_p2+1 : 0;
						ring_param->at(seg_idx_p2)->myArea = Private_SegmentArea( ring_param->at(seg_idx_p2), ring_param->at(seg_idx_p1), ring_param->at(seg_idx_p3), &( ring_param->at(seg_idx_p2)->myTriangleApex ) );
					}
				}
			}
		}
		
		/*- end move clockwise one segment -*/
		
		//break if we are no longer moving in either direction
		if(move_clock == false && move_counter == false) { break; }
	}
	
}
void JMD::JMD_ChromaTag_Detect::Private_ForceConvex(TagBorderRing *ring_param, int index_param, bool *was_seg2next_convex_param, bool *was_prevSeg1_convex_param)
{
	//indices to line segments
	int seg_idx_prev = (index_param-1  >= 0)                  ? index_param-1  : ring_param->size()-1;
	int seg_idx_seg1 =  index_param;
	int seg_idx_seg2 = (index_param+1  <  ring_param->size()) ? index_param+1  : 0;
	int seg_idx_next = (seg_idx_seg2+1 <  ring_param->size()) ? seg_idx_seg2+1 : 0;
	
	//check segment 2 and next neighbor
	if( !Private_IsConvex( &(ring_param->at(seg_idx_seg2)->myLine), &(ring_param->at(seg_idx_next)->myLine) ) )
	{
		//return convex boolean
		if(was_seg2next_convex_param) { *was_seg2next_convex_param = false; }
		
		//force convexity by moving the shared point out
		//to make the two lines parallel
		Private_ForceConvex( ring_param->at(seg_idx_seg2), ring_param->at(seg_idx_next) );
		
		//update areas - the UpdatePolygonArea function will take care of the
		//main four segments, but the segment at next+1 will also change
		int seg_idx_next1 = (seg_idx_next+1  < ring_param->size()) ? seg_idx_next+1  : 0;
		int seg_idx_next2 = (seg_idx_next1+1 < ring_param->size()) ? seg_idx_next1+1 : 0;
		ring_param->at(seg_idx_next1)->myArea = Private_SegmentArea( ring_param->at(seg_idx_next1), ring_param->at(seg_idx_next), ring_param->at(seg_idx_next2), &( ring_param->at(seg_idx_next1)->myTriangleApex ) );
	}
	else 
	{
		if(was_seg2next_convex_param) { *was_seg2next_convex_param = true; } 
	}
	
	//check segment 1 and prev neighbor
	if( !Private_IsConvex( &(ring_param->at(seg_idx_prev)->myLine), &(ring_param->at(seg_idx_seg1)->myLine) ) )
	{
		//return convex boolean
		if(was_prevSeg1_convex_param) { *was_prevSeg1_convex_param = false; }
		
		//force convexity by moving the shared point out
		//to make the two lines parallel
		Private_ForceConvex( ring_param->at(seg_idx_prev), ring_param->at(seg_idx_seg1) );
		
		//update areas - the UpdatePolygonArea function will take care of the
		//main four segments, but the segment at prev-1 will also change
		int seg_idx_prev1 = (seg_idx_prev-1  >= 0) ? seg_idx_prev-1  : ring_param->size()-1;
		int seg_idx_prev2 = (seg_idx_prev1-1 >= 0) ? seg_idx_prev1-1 : ring_param->size()-1;
		ring_param->at(seg_idx_prev1)->myArea = Private_SegmentArea( ring_param->at(seg_idx_prev1), ring_param->at(seg_idx_prev2), ring_param->at(seg_idx_prev), &( ring_param->at(seg_idx_prev1)->myTriangleApex ) );
	}
	else 
	{ 
		if(was_prevSeg1_convex_param) { *was_prevSeg1_convex_param = true; }
	}
}
void JMD::JMD_ChromaTag_Detect::Private_ForceConvex(TagBorderSegment *seg1_param, TagBorderSegment *seg2_param)
{
	//projected point
	JMD_Vision_Point2D ProjPt;
	
	//Get Projection Point
	JMD_Vision_LineSegment2D ProjLine( seg1_param->myLine.Point1(), seg2_param->myLine.Point2() );
	ProjLine.Projection( seg2_param->myLine.Point1(), &ProjPt );
	
	//normalized vector of projection
	double oU = seg1_param->myLine.Point2()->U();
	double oV = seg1_param->myLine.Point2()->V();
	double vU = ProjPt.U() - oU;
	double vV = ProjPt.V() - oV;
	double L = sqrt( vU*vU + vV*vV );
	L = (L > 0) ? L : 1;
	JMD_Vision_Point2D ProjVec( vU / L, vV / L );
	
	//Set Point + epsilon
	ProjPt.U( ProjPt.U() + 0.0000 * ProjVec.U() );
	ProjPt.V( ProjPt.V() + 0.0000 * ProjVec.V() );
	seg1_param->myLine.Point2( &ProjPt );
	seg2_param->myLine.Point1( &ProjPt );
	
	//update other line information
	//seg1_param->myLP2_EdgeColorRef = LP2_ColorRef;
	seg1_param->myLength = seg1_param->myLine.Length();
	seg1_param->myLine.Midpoint( &(seg1_param->myMidpoint) ); 
	seg1_param->myLine.Slope( &(seg1_param->mySlope) );
	//seg1_param->myLP1_EdgeColorRef = edge_color_param;
	seg2_param->myLength = seg2_param->myLine.Length();
	seg2_param->myLine.Midpoint( &(seg2_param->myMidpoint) );
	seg2_param->myLine.Slope( &(seg2_param->mySlope) );
}
bool JMD::JMD_ChromaTag_Detect::Private_ForceConvex(TagBorderSegment *seg_param, JMD_Vision_Point2D *point_param)
{
	//project point onto segment
	JMD_Vision_Point2D ProjPt;
	JMD_Vision_LineSegment2D ProjLine( seg_param->myLine.Point1(), seg_param->myLine.Point2() );
	ProjLine.Projection( point_param, &ProjPt );
	
	//vector from center to point
	JMD_Vision_Point2D ScanVec( point_param->U() - myCenterPoint.U(), point_param->V() - myCenterPoint.V() );
	
	//vector from point to projected point
	JMD_Vision_Point2D ProjVec( ProjPt.U() - point_param->U(), ProjPt.V() - point_param->V() );
	
	//if inside (if vectors point in same direction)
	double dot = ProjVec.U() * ScanVec.U() + ProjVec.V() * ScanVec.V();
	if( dot > 0 )
	{
		//calculate vector length to normalize
		double len = sqrt( ProjVec.U() * ProjVec.U() + ProjVec.V() * ProjVec.V() );
		
		//push to border + epsilon to ensure numerically that they are convex
		point_param->U( ProjPt.U() + 0.0000 * ProjVec.U() / len );
		point_param->V( ProjPt.V() + 0.0000 * ProjVec.V() / len );
		
		//return false - extra point was not already convex
		return false;
	}
	else
	{
		//return true - extra point was already convex
		return true;
	}
}
/*--- End Private Force Convex ---*/

/*----- Private In Slice -----*/
bool JMD::JMD_ChromaTag_Detect::Private_InSlice(TagBorderSegment *seg_param, JMD_Vision_Point2D *point_param)
{
	// slope of first line of slice
	JMD_Vision_Point2D *point1 = seg_param->myLine.Point1();
	double s1y = point1->V() - myCenterPoint.V();
	double s1x = point1->U() - myCenterPoint.U();

	// slope of second line of slice
	JMD_Vision_Point2D *point2 = seg_param->myLine.Point2();
	double s2y = point2->V() - myCenterPoint.V();
	double s2x = point2->U() - myCenterPoint.U();

	// slope of query point
	double qy = point_param->V() - myCenterPoint.V();
	double qx = point_param->U() - myCenterPoint.U();

	// query slope between the slice slopes?
	if ( (s1y*qx - s1x*qy) * (s2y*qx - s2x*qy) < 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*--- End Private In Slice ---*/

/*----- Private In Polygon Estimate -----*/
bool JMD::JMD_ChromaTag_Detect::Private_InPolygonEstimate(int i, int *j, TagBorderRing *ring_param)
{
	//variables
	TagBorderSegment *curr_seg;
	double min_u = JMD::DEF_PINF, max_u = JMD::DEF_NINF;
	double min_v = JMD::DEF_PINF, max_v = JMD::DEF_NINF;
	double curr_u, curr_v;
	int jd3;
	
	//for each segment
	for(TagBorderRing::iterator it = ring_param->begin(); it != ring_param->end(); ++it)
	{
		//current segment
		curr_seg = *it;
		curr_u = curr_seg->myLine.Point1()->U();
		curr_v = curr_seg->myLine.Point1()->V();
			
		//min and max U
		if(curr_u < min_u) { min_u = curr_u; }
		if(curr_u > max_u) { max_u = curr_u; }
		
		//min and max V
		if(curr_v < min_v)  { min_v = curr_v; }
		if(curr_v > max_v)	{ max_v = curr_v; }
	}
	
	//check if inside
	jd3 = (*j)/3;
	if(i >= min_v && i <= max_v && jd3 >= min_u && jd3 <= max_u) 
	{
		//push j to the edge of border
		*j = 3*max_u; 
		
		//return true
		return true; 
	}
	
	//else
	return false;
}
/*--- End Private In Polygon Estimate ---*/

/*------------- End Polygon Geometry Methods -------------*/
		
//---------------------------------------------------------------//
//------------------------ end Private --------------------------//
//---------------------------------------------------------------//





//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//

/*--------------- Constructors ---------------*/
JMD::JMD_ChromaTag_Detect::JMD_ChromaTag_Detect(JMD_ChromaTag_Collection *collection_param)
{	
	//utilities
	myVisionProcess = new JMD_Vision_Process();
	
	//collection
	myDetections = collection_param;
}
/*------------- End Constructors -------------*/

	
	
/*--------------- Destructors ---------------*/
JMD::JMD_ChromaTag_Detect::~JMD_ChromaTag_Detect() 
{
	//free
	if(myVisionProcess) { delete myVisionProcess; myVisionProcess = nullptr; }
	
	//free memory of detection borders
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		
		//for each ring of the border
		for(int i = 0; i < curr_borders->size(); i++)
		{
			//for each segment
			for(int j = 0; j < curr_borders->at(i)->size(); j++)
			{
				//delete border segment
				delete curr_borders->at(i)->at(j);
			}
			curr_borders->at(i)->clear();
			
			//delete border ring
			delete curr_borders->at(i);
		}
		curr_borders->clear();
	}
	
}
/*------------- End Destructors -------------*/



/*--------------- Setup ---------------*/

/*----- Init -----*/
void JMD::JMD_ChromaTag_Detect::Init(std::vector<double> *l_table_param, std::vector<double> *a_table_param, std::vector<double> *b_table_param, JMD_ChromaTag_Settings *settings_param) 
{
	Private_Init(l_table_param,a_table_param,b_table_param, settings_param);
}
/*--- End Init ---*/

/*------------- End Setup -------------*/

		
		
/*--------------- Tag Detection Methods ---------------*/    

/*---------- Detect ----------*/
void JMD::JMD_ChromaTag_Detect::Detect(JMD::JMD_Vision_Image &image_param)
{
	
}
void JMD::JMD_ChromaTag_Detect::Detect_RGBToLAB(JMD::JMD_Vision_Image &image_param)
{
	
}
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::Detect( cv::Mat &image_param )
{
	/*----- Color Wrappers -----*/
	switch(mySettings->myInputColorSpace)
	{
		case InputCS_RGB: Detect_RGBToLAB(image_param); break;
		case InputCS_BGR: Detect_BGRToLAB(image_param); break;
		case InputCS_YUV: break;
	}
	/*--- End Color Wrappers ---*/
}
void JMD::JMD_ChromaTag_Detect::Detect_RGBToLAB( cv::Mat &image_param )
{
	//reset borders
	Private_ResetBorders();
	
	//Detect
	Private_Detect_RGBToLAB(image_param);
}
void JMD::JMD_ChromaTag_Detect::Detect_BGRToLAB( cv::Mat &image_param )
{
	//reset borders
	Private_ResetBorders();
	
	//Detect
	Private_Detect_BGRToLAB(image_param);
}
#endif
/*-------- End Detect --------*/
            
/*------------- End Tag Detection Methods -------------*/
        
        

/*--------------- Utility ---------------*/

/*--------------- Utility ---------------*/



/*--------------- Setters/Getters ---------------*/    
/*------------- End Setters/Getters -------------*/



/*--------------- Operator Overloads ---------------*/
/*--------------- Operator Overloads ---------------*/



/*--------------- DEBUG Methods ---------------*/
        
/*----- DEBUG_ShowDetections -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowDetections(cv::Mat &image_param)
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
		
		//current border
		curr_border = (*cit)->TagBorders;
		
		//for each ring
		for(TagBorderSet::iterator rit = curr_border.begin(); rit != curr_border.end(); ++rit)
		{
			//current ring
			curr_ring = *rit;
			
			//for each segment
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				curr_seg = *sit;
				JMD_Vision_Point2D *Pt1 = curr_seg->myLine.Point1();
				JMD_Vision_Point2D *Pt2 = curr_seg->myLine.Point2();
				cv::line( image_param, cv::Point(Pt1->U(),Pt1->V()), cv::Point(Pt2->U(), Pt2->V()), cv::Scalar(0,128,255), 8, cv::LINE_AA );
			}
		}
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowDetections(cv::Mat &image_param, unsigned int ring_param)
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
		
		//current border
		curr_border = (*cit)->TagBorders;
		
		//selected ring
		curr_ring = curr_border.at(ring_param);
		
		//for each segment
		for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
		{
			curr_seg = *sit;
			JMD_Vision_Point2D *Pt1 = curr_seg->myLine.Point1();
			JMD_Vision_Point2D *Pt2 = curr_seg->myLine.Point2();
			cv::line( image_param, cv::Point(Pt1->U(),Pt1->V()), cv::Point(Pt2->U(), Pt2->V()), cv::Scalar(0,128,255), 8, cv::LINE_AA );
		}
	}
}
#endif
/*--- End DEBUG_ShowDetections ---*/

/*----- DEBUG_ShowLines -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param)
{
	//for each border ring
	int rct = 0;
	for(TagBorderSet::iterator it = myBorders->begin(); it != myBorders->end(); ++it)
	{
		TagBorderRing *tmpRing = (*it);
		
		//for each segment
		for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
		{
			TagBorderSegment *tmpSeg = (*rit);
			JMD_Vision_Point2D *Pt1 = tmpSeg->myLine.Point1();
			JMD_Vision_Point2D *Pt2 = tmpSeg->myLine.Point2();
			if(rct == 2)      { cv::line( image_param, cv::Point(Pt1->U(),Pt1->V()), cv::Point(Pt2->U(), Pt2->V()), cv::Scalar(0,128,255), 2 ); }
			//else if(rct == 1) { cv::line( image_param, cv::Point(Pt1->U(),Pt1->V()), cv::Point(Pt2->U(), Pt2->V()), cv::Scalar(255,0,255) , 2); }
			//else if(rct == 0) { cv::line( image_param, cv::Point(Pt1->U(),Pt1->V()), cv::Point(Pt2->U(), Pt2->V()), cv::Scalar(255,0,0) , 2); }
		}
		rct++;
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param, std::vector<double> &means_param, std::vector<JMD_Vision_Point2D*> &points_param)
{
	//for each mean
	for(int cct = 0; cct < means_param.size(); cct++)
	{
		// equation for a line y = mx + b
		double m = tan( means_param.at(cct) );
		double b = points_param.at(cct)->V() - m * points_param.at(cct)->U();
		
		// line points
		double p1u, p1v, p2u, p2v;
		if( fabs(m) > 5000)
		{
			//std::cout << "INFINITE SLOPE" << std::endl;
			p1u = points_param.at(cct)->U(); p1v = 0;
			p2u = points_param.at(cct)->U(); p2v = 480;
		}
		// more vertical than horizontal
		else if( fabs(m) > 1)
		{
			p1u = -b / m; p1v = 0;
			p2u = (480-b)/m; p2v = 480;
		}
		// more horizontal than vertical
		else
		{
			p1u = 0;   p1v = b;
			p2u = 752; p2v = m * 752 + b;
		}
		//std::cout << *(points_param.at(cct)) << ". m = " << m << " ==> Draw (p1u,p2u) ==> (p1v,p2v) : " << p1u << " " << p2u << " " << p1v << " " << p2v << std::endl;
		
		//draw
		cv::circle( image_param, cv::Point( points_param.at(cct)->U(), points_param.at(cct)->V()), 3, cv::Scalar(0,0,255), -1);
		cv::line( image_param, cv::Point(p1u,p1v), cv::Point(p2u,p2v), cv::Scalar(0,0,0), 2);
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param, std::vector<double> &means_param, std::vector<JMD_Vision_Point2D> &points_param)
{
	//for each mean
	for(int cct = 0; cct < means_param.size(); cct++)
	{
		// equation for a line y = mx + b
		double m = tan( means_param.at(cct) );
		double b = points_param.at(cct).V() - m * points_param.at(cct).U();
		
		// line points
		double p1u, p1v, p2u, p2v;
		if( fabs(m) > 5000)
		{
			//std::cout << "INFINITE SLOPE" << std::endl;
			p1u = points_param.at(cct).U(); p1v = 0;
			p2u = points_param.at(cct).U(); p2v = 480;
		}
		// more vertical than horizontal
		else if( fabs(m) > 1)
		{
			p1u = -b / m; p1v = 0;
			p2u = (480-b)/m; p2v = 480;
		}
		// more horizontal than vertical
		else
		{
			p1u = 0;   p1v = b;
			p2u = 752; p2v = m * 752 + b;
		}
		//std::cout << points_param.at(cct) << ". m = " << m << " ==> Draw (p1u,p2u) ==> (p1v,p2v) : " << p1u << " " << p2u << " " << p1v << " " << p2v << std::endl;
		
		//draw
		cv::circle( image_param, cv::Point( points_param.at(cct).U(), points_param.at(cct).V()), 3, cv::Scalar(0,0,255), -1);
		cv::line( image_param, cv::Point(p1u,p1v), cv::Point(p2u,p2v), cv::Scalar(0,0,0), 2);
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param, std::list<JMD_Vision_LineSegment2D> *border_param)
{
	int ct = 0;
	for(std::list<JMD_Vision_LineSegment2D>::iterator it=border_param->begin(); it != border_param->end(); ++it)
	{
		double U1 = it->Point1()->U();
		double V1 = it->Point1()->V();
		double U2 = it->Point2()->U();
		double V2 = it->Point2()->V();
		//std::cout << "Line " << ct << ": (" << U1 << ", " << V1 << ") ==> (" << U2 << ", " << V2 << ") " << std::endl;
		cv::line( image_param, cv::Point(it->Point1()->U(),it->Point1()->V()), cv::Point(it->Point2()->U(), it->Point2()->V()), cv::Scalar(0,255,0) );
		//cv::circle(image_param,cv::Point(j/3,i),3,cv::Scalar(0,255,0),-1);
		ct++;
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param, std::vector<JMD_Vision_LineSegment2D> *border_param)
{
	int ct = 0;
	for(std::vector<JMD_Vision_LineSegment2D>::iterator it=border_param->begin(); it != border_param->end(); ++it)
	{
		double U1 = it->Point1()->U();
		double V1 = it->Point1()->V();
		double U2 = it->Point2()->U();
		double V2 = it->Point2()->V();
		//std::cout << "Line " << ct << ": (" << U1 << ", " << V1 << ") ==> (" << U2 << ", " << V2 << ") " << std::endl;
		cv::line( image_param, cv::Point(it->Point1()->U(),it->Point1()->V()), cv::Point(it->Point2()->U(), it->Point2()->V()), cv::Scalar(0,255,0) );
		//cv::circle(image_param,cv::Point(j/3,i),3,cv::Scalar(0,255,0),-1);
		ct++;
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowLines(cv::Mat &image_param, std::vector<TagBorderSegment*> *border_param)
{
	int ct = 0;
	for(std::vector<TagBorderSegment*>::iterator it=border_param->begin(); it != border_param->end(); ++it)
	{
		double U1 = (*it)->myLine.Point1()->U();
		double V1 = (*it)->myLine.Point1()->V();
		double U2 = (*it)->myLine.Point2()->U();
		double V2 = (*it)->myLine.Point2()->V();
		double Sx = (*it)->mySlope.U();
		double Sy = (*it)->mySlope.V();
		double L  = (*it)->myLength;
		//std::cout << "Line " << ct << ": (" << U1 << ", " << V1 << ") ==> (" << U2 << ", " << V2 << ").    Slope = ( " << Sx << ", " << Sy << " ).    L = " << L << std::endl;
		cv::line( image_param, cv::Point(U1,V1), cv::Point(U2, V2), cv::Scalar(0,255,0) );
		//cv::circle(image_param,cv::Point(j/3,i),3,cv::Scalar(0,255,0),-1);
		ct++;
	}
}
#endif
/*--- End DEBUG_ShowLines ---*/

/*----- DEBUG ShowPoints -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowPoint(cv::Mat &image_param, JMD_Vision_Point2D *point_param)
{
	cv::circle(image_param, cv::Point(point_param->U(),point_param->V()), 3, cv::Scalar(255,255,0),-1);
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowPoints(cv::Mat &image_param)
{
	//for each border ring
	for(TagBorderSet::iterator it = myBorders->begin(); it != myBorders->end(); ++it)
	{
		TagBorderRing *tmpRing = (*it);
		
		//for each segment
		for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
		{
			TagBorderSegment *tmpSeg = (*rit);
			JMD_Vision_Point2D *tmpPt = tmpSeg->myLine.Point2();
			cv::circle(image_param, cv::Point(tmpPt->U(),tmpPt->V()), 3, cv::Scalar(255,255,0),-1);
			tmpPt = tmpSeg->myLine.Point1();
			cv::circle(image_param, cv::Point(tmpPt->U(),tmpPt->V()), 3, cv::Scalar(255,255,0),-1);
		}
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowPoints(cv::Mat &image_param, std::vector<JMD_Vision_Point2D> *points_param)
{
	//for each point
	int ct = 0;
	for(std::vector<JMD_Vision_Point2D>::iterator it = points_param->begin(); it != points_param->end(); ++it)
	{
		//std::cout << "Point " << ct++ << ": (" << it->U() << ", " << it->V() << " )." << std::endl; 
		cv::circle(image_param, cv::Point(it->U(),it->V()), 3,cv::Scalar(255,0,0),-1);
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowPoints(cv::Mat &image_param, unsigned int ring_param)
{
	//for one border ring
	TagBorderRing *tmpRing = myBorders->at(ring_param);
	
	//for each segment
	for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
	{
		TagBorderSegment *tmpSeg = (*rit);
		JMD_Vision_Point2D *tmpPt = tmpSeg->myLine.Point1();
		cv::circle(image_param, cv::Point(tmpPt->U(),tmpPt->V()), 3, cv::Scalar(255,255,0),-1);
	}
}
#endif
/*--- End DEBUG ShowPoints ---*/

/*----- DEBUG ShowBorderEstimate -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowBorderEstimate(cv::Mat &image_param)
{
	//for red border ring
	TagBorderRing *tmpRing = myBorders->at(0);
	
	//find min and max u and v
	double minu = JMD::DEF_PINF, maxu = JMD::DEF_NINF, minv = JMD::DEF_PINF, maxv = JMD::DEF_NINF;
	
	//for each segment
	for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
	{
		TagBorderSegment *tmpSeg = (*rit);
		JMD_Vision_Point2D *Pt1 = tmpSeg->myLine.Point1();
		
		//save min and max u and v
		if(Pt1->U() < minu) { minu = Pt1->U(); }
		if(Pt1->U() > maxu) { maxu = Pt1->U(); }
		if(Pt1->V() < minv) { minv = Pt1->V(); }
		if(Pt1->V() > maxv) { maxv = Pt1->V(); }
	}
	
	//plot rectangles at several scales
	double w = maxu - minu;
	double h = maxv - minv;
	for(int i = 0; i < 4; i++)
	{
		//left right up down
		double L = minu - i * w/2;
		double R = maxu + i * w/2;
		double U = minv - i * h/2;
		double D = maxv + i * h/2;
		
		//draw
		cv::line(image_param, cv::Point(L,U),cv::Point(L,D), cv::Scalar(255,255,0), 2);
		cv::line(image_param, cv::Point(L,D),cv::Point(R,D), cv::Scalar(255,255,0), 2);
		cv::line(image_param, cv::Point(R,D),cv::Point(R,U), cv::Scalar(255,255,0), 2);
		cv::line(image_param, cv::Point(R,U),cv::Point(L,U), cv::Scalar(255,255,0), 2);
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowBorderEstimatePoints(cv::Mat &image_param)
{
	//for red border ring
	TagBorderRing *tmpRing = myBorders->at(0);
	
	//find min and max u and v
	double minu = JMD::DEF_PINF, maxu = JMD::DEF_NINF, minv = JMD::DEF_PINF, maxv = JMD::DEF_NINF;
	
	//for each segment
	for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
	{
		TagBorderSegment *tmpSeg = (*rit);
		JMD_Vision_Point2D *Pt1 = tmpSeg->myLine.Point1();
		
		//save min and max u and v
		if(Pt1->U() < minu) { minu = Pt1->U(); }
		if(Pt1->U() > maxu) { maxu = Pt1->U(); }
		if(Pt1->V() < minv) { minv = Pt1->V(); }
		if(Pt1->V() > maxv) { maxv = Pt1->V(); }
	}
	
	//plot rectangles at several scales
	double w = maxu - minu;
	double h = maxv - minv;
	double midu = (maxu+minu)/2;
	double midv = (maxv+minv)/2;
	for(int i = 0; i < 4; i++)
	{
		for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
		{
			//point on segment
			TagBorderSegment *tmpSeg = (*rit);
			JMD_Vision_Point2D *Pt1 = tmpSeg->myLine.Point1();
			double U = Pt1->U();
			double V = Pt1->V();
			
			//scaled in direction of point
			double tu = U - midu;
			double tv = V - midv;
			double su = midu + tu * (i+1);
			double sv = midv + tv * (i+1);
			
			//draw
			cv::circle(image_param, cv::Point(su,sv), 2, cv::Scalar(255,255,0),-1);
		}
	}
}
#endif
/*--- End DEBUG ShowBorderEstimate ---*/

/*----- DEBUG ShowScan -----*/
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowScan(cv::Mat &image_param, unsigned int u_param, unsigned int v_param, TagBorderRing *ring_param,unsigned int family_param)
{
	//variables
	double uadj;
	JMD_Vision_Point2D *currPt;
	cv::Scalar color(255,0,0);
	switch(family_param)
	{
		case 0: uadj = 0; break;
		case 1: uadj = 1; color[2] = 255; break;
		case 2: uadj = 2; color[0] = 0; color[1] = 128; color[2] = 255; break;
		default: break;
	}
	
	//for each point on ring
	for(TagBorderRing::iterator it = ring_param->begin(); it != ring_param->end(); ++it)
	{
		//point on edge
		currPt = (*it)->myLine.Point1();
		std::cout << "Scan Dir = " << (*it)->myScanDir.U() << " " << (*it)->myScanDir.V() << " " << currPt->U() << " " << currPt->V() << std::endl;
		
		//scan right
		if( (*it)->myScanDir.U() > 0 && fabs((*it)->myScanDir.V()) < JMD::DEF_EQUALITY_THRESHOLD)
		{
			std::cout << "scan right" << std::endl;
			for(int uct = u_param; uct <= currPt->U(); uct++) { cv::circle(image_param,cv::Point(uct,currPt->V()),1,color,-1); }
		}
		//scan left
		if( (*it)->myScanDir.U() < 0 && fabs((*it)->myScanDir.V()) < JMD::DEF_EQUALITY_THRESHOLD)
		{
			std::cout << "scan left" << std::endl;
			for(int uct = u_param; uct >= currPt->U(); uct--) { cv::circle(image_param,cv::Point(uct,currPt->V()),1,color,-1); }
		}
		//scan down
		if( fabs((*it)->myScanDir.U()) < JMD::DEF_EQUALITY_THRESHOLD && (*it)->myScanDir.V() > 0)
		{
			std::cout << "scan up" << std::endl;
			for(int vct = v_param; vct <= currPt->V(); vct++) { cv::circle(image_param,cv::Point(currPt->U(),vct),1,color,-1); }
		}
		//scan up
		if( fabs((*it)->myScanDir.U()) < JMD::DEF_EQUALITY_THRESHOLD && (*it)->myScanDir.V() < 0)
		{
			std::cout << "scan down" << std::endl;
			for(int vct = v_param; vct >= currPt->V(); vct--) { cv::circle(image_param,cv::Point(currPt->U()+uadj,vct),1,color,-1); }
		}
	}
	
}
/*--- End DEBUG ShowScan ---*/

/*----- DEBUG ShowScanOrigins -----*/
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowScanOrigins(cv::Mat &image_param)
{
	int ct, rct, DetectionCt = 0;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		//std::cout << "Detection" << std::endl;
		//active?
		if(!curr_detection->IsActive) { return; }
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		
		//outer ring
		curr_ring = curr_borders->back();
		
		//for each border ring
		/*rct = 0;
		for(TagBorderSet::iterator rit = curr_borders->begin(); rit != curr_borders->end(); ++rit)
		{
			//current ring
			curr_ring = *rit;
			std::cout << "  Ring" << std::endl;*/
			
			//for each segment
			//ct = 0;
			double center_u = 0, center_v = 0, ct = 0;
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				//current segment
				curr_seg = *sit;
					
				//point values
				double U1 = (*sit)->myLine.Point1()->U();
				double V1 = (*sit)->myLine.Point1()->V();
				//double U2 = (*sit)->myLine.Point2()->U();
				//double V2 = (*sit)->myLine.Point2()->V();
				//std::cout << "    ScanOrigins " << U1 << " " << V1 /*<< " ==> " << U2 << " " << V2*/ << std::endl;
				
				//draw line
				//cv::line(image_param,cv::Point(U1,V1), cv::Point(U2, V2), cv::Scalar(255,255,255) );
				center_u += U1;
				center_v += V1;
				ct++;
			}
			cv::circle(image_param,cv::Point(center_u/ct,center_v/ct),5,cv::Scalar(255,255,255),-1);
		//}
	}
}
/*--- End DEBUG ShowScanOrigins ---*/

/*----- DEBUG ShowCVCorners -----*/
#ifdef OPENCV_FOUND
void JMD::JMD_ChromaTag_Detect::DEBUG_ShowCVCorners( cv::Mat &image_param, std::vector<cv::Point2f> &corners_param )
{
	for(int i = 0; i < corners_param.size(); i++)
	{
		cv::circle( image_param, corners_param.at(i), 1, cv::Scalar(0,0,255), -1 );
	}
}
#endif
/*--- End DEBUG ShowCVCorners ---*/

/*----- DEBUG OutputBorder -----*/
void JMD::JMD_ChromaTag_Detect::DEBUG_OutputBorder()
{
	//variables
	int ct1 = 0, ct2 = 0;
	
	//for each border ring
	for(TagBorderSet::iterator it = myBorders->begin(); it != myBorders->end(); ++it)
	{
		TagBorderRing *tmpRing = (*it);
		std::cout << "Ring " << ct1++ << std::endl;
		//for each segment
		for(TagBorderRing::iterator rit = tmpRing->begin(); rit != tmpRing->end(); ++rit)
		{
			TagBorderSegment *tmpSeg = (*rit);
			
			std::cout << "    Segment " << ct2++ << ":  Area = " << tmpSeg->myArea << " (" << tmpSeg->isZeroed << ").  ( " << *(tmpSeg->myLine.Point1()) << " )"  << " ==> " << "( " << *(tmpSeg->myLine.Point2()) << " )" << std::endl;
		}
	}
}
void JMD::JMD_ChromaTag_Detect::DEBUG_OutputBorder(std::vector<TagBorderSegment*> *border_param)
{
	int ct = 0;
	for(std::vector<TagBorderSegment*>::iterator it=border_param->begin(); it != border_param->end(); ++it)
	{
		double U1 = (*it)->myLine.Point1()->U();
		double V1 = (*it)->myLine.Point1()->V();
		double U2 = (*it)->myLine.Point2()->U();
		double V2 = (*it)->myLine.Point2()->V();
		double Sx = (*it)->mySlope.U();
		double Sy = (*it)->mySlope.V();
		double L  = (*it)->myLength;
		std::cout << "Line " << ct << ": (" << U1 << ", " << V1 << ") ==> (" << U2 << ", " << V2 << ").    Slope = ( " << Sx << ", " << Sy << " ).    L = " << L << std::endl;
		ct++;
	}
}
/*--- End DEBUG OutputBorder ---*/

/*----- DEBUG OutputPoints -----*/
/*--- End DEBUG OutputPoints ---*/

/*----- DEBUG OutputDetections -----*/
void JMD::JMD_ChromaTag_Detect::DEBUG_OutputDetections(int subset_start_param, int subset_finish_param)
{
	int ct, rct, DetectionCt = 0;
	TagBorderRing *curr_ring;
	TagBorderSegment *curr_seg;
	
	for(JMD_ChromaTag_Collection::iterator it = myDetections->begin(); it != myDetections->end(); ++it)
	{
		//skip if outside range
		if(DetectionCt < subset_start_param || (DetectionCt > subset_finish_param && subset_finish_param != -1 ) ) { continue; }
		
		//Pointer to a detection
		JMD_ChromaTag_Detection *curr_detection = *it;
		
		//Pointer to the detection's border
		TagBorderSet *curr_borders = &(curr_detection->TagBorders);
		std::cout << "Detection " << DetectionCt << std::endl;
		//for each border ring
		rct = 0;
		for(TagBorderSet::iterator rit = curr_borders->begin(); rit != curr_borders->end(); ++rit)
		{
			//current ring
			curr_ring = *rit;
			std::cout << "    Ring " << rct << std::endl;
			//for each segment
			ct = 0;
			for(TagBorderRing::iterator sit = curr_ring->begin(); sit != curr_ring->end(); ++sit)
			{
				curr_seg = *sit;
				
				//reset slopes
				//curr_seg->myScanDir.U( myExpandDirTable.at(ct).U() );
				//curr_seg->myScanDir.V( myExpandDirTable.at(ct).V() );
					
				//cout
				std::cout << "        Segment " << ct << ":  ScanDirs = ( " << curr_seg->myScanDir.U() << " " << curr_seg->myScanDir.V() << std::endl;
				
				//increment
				ct++;
			}
			rct++;
		}
		DetectionCt++;
	}
}
/*--- End DEBUG OutputDetections ---*/
		
/*------------- End DEBUG Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
