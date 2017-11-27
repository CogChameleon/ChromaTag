/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Timer: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Timer.cpp
 A class for stopwatch, timestamp, and sleep functionality
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

#include "JMD_Utils_Timer.hpp"

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





//================================================================================//
//================================================================================//
//===================== JMD Utils Timer Class Implementation =====================//
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
JMD::JMD_Utils_Timer::JMD_Utils_Timer() {}
/*------------- end Constructors -------------*/


/*--------------- Destructors ---------------*/
JMD::JMD_Utils_Timer::~JMD_Utils_Timer() { }
/*------------- end Destructors -------------*/


/*--------------- Stopwatch Methods ---------------*/

/*----- Reset -----*/
void JMD::JMD_Utils_Timer::Reset()
{
	//set Start to current time
	gettimeofday(&Start,NULL);
}
/*--- End Reset ---*/
		
/*----- Elapsed Time Milli Seconds -----*/
long long int JMD::JMD_Utils_Timer::ElapsedTimeMilliSeconds()
{
	//get current time
	gettimeofday(&Current,NULL);
	
	//get time delta (Current - Start)
	timeval delta;
	timersub(&Current,&Start,&delta);
	
	//return milli seconds
	long long int msecs = delta.tv_sec*1000 + delta.tv_usec/1000;
	return msecs;
}
/*--- End Elapsed Time Milli Seconds ---*/
		
/*----- Elapsed Time Seconds -----*/
double JMD::JMD_Utils_Timer::ElapsedTimeSeconds()
{
	//get current time
	gettimeofday(&Current,NULL);
	
	//get time delta (Current - Start)
	timeval delta;
	timersub(&Current,&Start,&delta);
	
	//return seconds
	double secs=double(delta.tv_sec);
	secs += double(delta.tv_usec)/1000000.0;
	return secs;
}
/*--- End Elapsed Time Seconds ---*/

/*------------- End Stopwatch Methods -------------*/


/*--------------- Sleep Methods ---------------*/

/*----- Sleep -----*/
struct timespec JMD::JMD_Utils_Timer::Sleep(time_t sec, long nano)
{
	//set up struct
	struct timespec sleepT, remT;
	sleepT.tv_sec = sec;
	sleepT.tv_nsec = nano;
		
	//sleep
	nanosleep(&sleepT, &remT);
	
	//return	
	return remT;
}
/*--- End Sleep ---*/

/*----- Sleep Nano -----*/
void JMD::JMD_Utils_Timer::Sleep_Nano(long nano)
{
	Sleep(0,nano);
}
/*--- End Sleep Nano ---*/

/*----- Sleep Micro -----*/
void JMD::JMD_Utils_Timer::Sleep_Micro(long micro)
{
	Sleep(0, micro * 1000L);
}
/*--- End Sleep Micro ---*/

/*----- Sleep Milli -----*/
void JMD::JMD_Utils_Timer::Sleep_Milli(long milli)
{
	Sleep(0, milli * 1000000L);
}
/*--- End Sleep Milli ---*/

/*----- Sleep Seconds -----*/
void JMD::JMD_Utils_Timer::Sleep_Seconds(long seconds)
{
	Sleep(seconds,0L);
}
/*--- End Sleep Seconds ---*/
	
/*------------- End Sleep Methods -------------*/


/*--------------- Timestamp Methods ---------------*/
        
/*----- Timestamp -----*/
int64_t JMD::JMD_Utils_Timer::Timestamp()
{
	//get time
	struct timeval tv;
	gettimeofday (&tv, NULL);
	
	//return timestamp
	return (int64_t) tv.tv_sec * 1000000 + tv.tv_usec;
}
/*--- End Timestamp ---*/

/*----- Timestamp Seconds -----*/
int64_t JMD::JMD_Utils_Timer::Timestamp_Seconds(int64_t v)
{
	//return timestamp in seconds
	return v/1000000;
}
/*--- End Timestamp Seconds ---*/

/*----- Timestamp USeconds -----*/
int64_t JMD::JMD_Utils_Timer::Timestamp_USeconds(int64_t v)
{
	//return timestamp in micro seconds
	return v%1000000;
}
/*--- End Timestamp USeconds ---*/

/*----- Timestamp To Timeval -----*/
void JMD::JMD_Utils_Timer::Timestamp_To_Timeval(int64_t v, struct timeval *tv)
{
	//set timeval struct values
	tv->tv_sec  = Timestamp_Seconds(v); 
	tv->tv_usec = Timestamp_USeconds(v);
}
/*--- End Timestamp To Timeval ---*/

/*----- Timestamp To Timespec -----*/
void JMD::JMD_Utils_Timer::Timestamp_To_Timespec(int64_t v, struct timespec *ts)
{
	//set timespec struct values
	ts->tv_sec  = Timestamp_Seconds(v); 
	ts->tv_nsec = Timestamp_USeconds(v)*1000;
}
/*--- End Timestamp To Timespec ---*/

/*------------- End Timestamp Methods -------------*/

//---------------------------------------------------------------//
//-------------------------- Public -----------------------------//
//---------------------------------------------------------------//


//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//
//====================================================================//