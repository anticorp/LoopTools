/*
 Name:		LoopTools.h
 Created:	1/27/2022 1:49:05 AM
 Author:	Robert Hoople
 Editor:	http://www.visualmicro.com
*/
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/* Loop Counter is a tool for loop counting in your Arduino sketch,
whether that's in the main loop or another branching loop of a 
library or state machine.It offers simple counting as well as more
advanced tools such as Period Reset or Clock Caching that can reduce
code - load by elimating the need to keep calling millis().
The code is optimised using pointers as opposed to run - time
conditional testing, so if the Loop Counter object is set to be
a simple advance, the function call is a single line of code that only
advances the internal counter.Clock caching also uses no conditionals,
but Period Reset uses one conditional every time the counter is advanced
to test if the period time has been reached.
Loop counter implicity converts to whatever the internal counter is,
and can simply be assigned to an unsigned integer anywhere.
For Period Reset, MyClock.Advance() returns a boolean true once,
every time the internal counter is reset to 0, and can be used as a timing
trigger like so : if (MyClock.Advance()) { // Do when counter resets }.
	The internal counter will be testedand incremented each time the if
		statement is evaluated.
*/

#ifndef _LoopCounter_h
#define _LoopCounter_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "ClockCache.h"

	enum class COUNTMETHOD
	{
		SimpleAdvance,
		CatchClock,
		PeriodReset,
	};

	class LOOPCOUNTER;

	namespace __loopcount
	{
		class COUNTERTYPE
		{
		public:
			COUNTERTYPE() {}
			virtual bool Advance(LOOPCOUNTER* loopCounter) {}
		};

		class SIMPLEADVANCE : public COUNTERTYPE
		{
		public:
			SIMPLEADVANCE() {}
			bool Advance(LOOPCOUNTER* loopCounter) override;

		private:
		};

		class FIRSTLOOP : public COUNTERTYPE
		{
		public:
			FIRSTLOOP() {}
			bool Advance(LOOPCOUNTER* loopCounter) override;

		private:
		};

		class CLOCKADVANCE : public COUNTERTYPE
		{
		public:
			CLOCKADVANCE() {}
			bool Advance(LOOPCOUNTER* loopCounter) override;

		private:
		};
		class PERIODRESET : public COUNTERTYPE
		{
		public:
			PERIODRESET() {}
			bool Advance(LOOPCOUNTER* loopCounter) override;

		private:
		};

		namespace objs
		{
			extern SIMPLEADVANCE SimpleAdvance;
			extern CLOCKADVANCE ClockAdvance;
			extern FIRSTLOOP FirstLoop;
			extern PERIODRESET PeriodReset;
		}

		extern COUNTERTYPE* SimpleAdvance;
		extern COUNTERTYPE* ClockAdvance;
		extern COUNTERTYPE* FirstLoop;
		extern COUNTERTYPE* PeriodReset;

	}

	class LOOPCOUNTER
	{
	public:
		LOOPCOUNTER();
		LOOPCOUNTER(uint32_t startValue);
		LOOPCOUNTER(COUNTMETHOD countMethod);
		LOOPCOUNTER(uint32_t startValue, COUNTMETHOD countMethod, uint32_t resetPeriod);
		operator uint32_t();
		void Configure(uint32_t startValue);
		void Configure(COUNTMETHOD countMethod);
		void Configure(COUNTMETHOD countMethod, int32_t resetPeriod);

		void Configure(uint32_t startValue, COUNTMETHOD countMethod, uint32_t resetPeriod);
		void SetResetPeriod(uint32_t resetPeriod);
		bool Advance();
		bool FirstLoop();
		void Reset();

	protected:
		friend class __loopcount::SIMPLEADVANCE;
		friend class __loopcount::CLOCKADVANCE;
		friend class __loopcount::FIRSTLOOP;
		friend class __loopcount::PERIODRESET;
		bool mPreLoop;
		uint32_t mCount;
		uint32_t mPeriod;
		bool mTrigger;
		void ClearFirstLoop();

	private:
		bool mFirstLoop;
		bool mConfigured;
		ClockCache mLoopClock;
		COUNTMETHOD mCountMethod;
		__loopcount::COUNTERTYPE* mCounterType;
	};

#endif

