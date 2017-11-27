#pragma once
#ifndef JMD_UTILS_TIMER_H_
#define JMD_UTILS_TIMER_H_

/*
 //====================================================================//
 
 ==========================
 Joseph DeGol
 UIUC Fall 2013
 ==========================
 JMD_Utils_Timer: Version 1.0
 ==========================
 
 ================================================================
 JMD_Utils_Timer.hpp
 A class for stopwatch, timestamp, and sleep functionality
 ================================================================
 
 List of Functions:
 
 ----- Enumerations -----
 --- End Enumerations ---
 
 ----- Constructors -----
 
 Timer();
 
 --- end Constructors ---
 
 
 ----- Stopwatch Methods -----
 	
 	void Reset();
	long long int ElapsedTimeMilliSeconds();
	double ElapsedTimeSeconds();
 
 --- end Stopwatch Methods ---
 
 
 ----- Sleep Methods -----
 
 	struct timespec Sleep(time_t sec, long nano);
	void Sleep_Nano(long nano);
	void Sleep_Micro(long micro);
	void Sleep_Milli(long milli);
	void Sleep_Seconds(long seconds);
	
 --- End Sleep Methods ---
 
 
 ----- Timestamp Methods -----
 
 	int64_t Timestamp();
	int64_t Timestamp_Seconds(int64_t v);
	int64_t Timestamp_USeconds(int64_t v);
	void Timestamp_To_Timeval(int64_t v, struct timeval *tv);
	void Timestamp_To_Timespec(int64_t v, struct timespec *ts);
		
 --- End Timestamp Methods ---
 
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

#include <cstddef>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

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







namespace JMD {
    
    //====================================================================//
    //====================================================================//
    //=================== JMD Utils Writer Definition ====================//
    //====================================================================//
    //====================================================================//
    class JMD_Utils_Timer
    {
        
        /*----- Enumerations -----*/
        
        /*--- End Enumerations ---*/
        
        //---------------------------------------------------------------//
        //------------------------- Private -----------------------------//
        //---------------------------------------------------------------//
        private:
        
        /*--------------- Variables ---------------*/
        
        timeval Start;
		timeval Current;
		
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
		JMD_Utils_Timer();
		/*------------- end Constructors -------------*/
		
		/*--------------- Destructors ---------------*/
		~JMD_Utils_Timer();
		/*------------- end Destructors ------------*/
		
		/*--------------- Stopwatch Methods ---------------*/
		
		/*----- Reset -----*/
		// resets the start time to the moment Reset() is called
		//
		void Reset();
		/*--- End Reset ---*/
		
		/*----- Elapsed Time Milli Seconds -----*/
		// returns the time in milli seconds since the last call to Reset()
		//
		long long int ElapsedTimeMilliSeconds();
		/*--- End Elapsed Time Milli Seconds ---*/
		
		/*----- Elapsed Time Seconds -----*/
		// returns the time in seconds since the last call to Reset()
		double ElapsedTimeSeconds();
		/*--- End Elapsed Time Seconds ---*/
		
		/*------------- End Stopwatch Methods -------------*/
        
        
        /*--------------- Sleep Methods ---------------*/
        
        /*----- Sleep -----*/
        // sleeps for the specified number of seconds + nanoseconds using nanosleep
        //
        // input:
        //    seconds_param - number of seconds to sleep
        //    nano_param - number of nano seconds to sleep
        // output:
        //    timespec - returned struct from nanosleep()
        struct timespec Sleep(time_t seconds_param, long nano_param);
        /*--- End Sleep ---*/
        
        /*----- Sleep Nano -----*/
        // sleeps for the provided number of nano seconds
        //
        // input:
        //    nano_param - number of nano seconds to sleep
		void Sleep_Nano(long nano_param);
		/*--- End Sleep Nano ---*/
		
		/*----- Sleep Micro -----*/
		// sleeps for the provided number of micro seconds
		//
		// input:
		//    micro_param - number of micro seconds to sleep
		void Sleep_Micro(long micro_param);
		/*--- End Sleep Micro ---*/
		
		/*----- Sleep Milli -----*/
		// sleeps for the provided number of milli seconds
		//
		// input:
		//    milli_param - number of milli seconds to sleep
		void Sleep_Milli(long milli_param);
		/*--- End Sleep Milli ---*/
		
		/*----- Sleep Seconds -----*/
		// sleeps for the provided number of seconds
		//
		// input:
		//    seconds_param - number of seconds to sleep
		void Sleep_Seconds(long seconds_param);
		/*--- End Sleep Seconds ---*/        
		
        /*------------- End Sleep Methods -------------*/
        
        
        /*--------------- Timestamp Methods ---------------*/
        
        /*----- Timestamp -----*/
        // returns a timestamp when called
        //
        // output:
        //    int64_t - returned int64 timestamp
        int64_t Timestamp();
        /*--- End Timestamp ---*/
        
        /*----- Timestamp Seconds -----*/
        // converts an int64 timestamp to seconds
        //
        // input:
        //    v - timestamp value from Timestamp()
        // output:
        //    int64_t - timestamp in seconds
		int64_t Timestamp_Seconds(int64_t v);
		/*--- End Timestamp Seconds ---*/
		
		/*----- Timestamp USeconds -----*/
		// converts an int64 timestamp to micro seconds
        //
        // input:
        //    v - timestamp value from Timestamp()
        // output:
        //    int64_t - timestamp in micro seconds
		int64_t Timestamp_USeconds(int64_t v);
		/*--- End Timestamp USeconds ---*/
		
		/*----- Timestamp To Timeval -----*/
		// converts an int64 timestamp to a timeval struct
		//
		// input:
		//    v - timestamp value from Timestamp()
		//    tv - timeval struct that is populated by v
		void Timestamp_To_Timeval(int64_t v, struct timeval *tv);
		/*--- End Timestamp To Timeval ---*/
		
		/*----- Timestamp To Timespec -----*/
		// converts an int64 timestamp to a timespec struct
		//
		// input:
		//    v - timestamp value from Timestamp()
		//    ts - timespec struct that is populated by v
		void Timestamp_To_Timespec(int64_t v, struct timespec *ts);
		/*--- End Timestamp To Timespec ---*/
		
        /*------------- End Timestamp Methods -------------*/
        
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

