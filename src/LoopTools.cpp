/*
 Name:		LoopTools.cpp
 Created:	1/27/2022 1:49:05 AM
 Author:	Robert
 Editor:	http://www.visualmicro.com
*/

#include "LoopTools.h"
#include <ADM.h>

namespace __loopcount
{
	namespace objs
	{
		SIMPLEADVANCE SimpleAdvance;
		CLOCKADVANCE ClockAdvance;
		FIRSTLOOP FirstLoop;
		PERIODRESET PeriodReset;
	}
	COUNTERTYPE* SimpleAdvance = &objs::SimpleAdvance;
	COUNTERTYPE* FirstLoop = &objs::FirstLoop;
	COUNTERTYPE* PeriodReset = &objs::PeriodReset;
	COUNTERTYPE* ClockAdvance = &objs::ClockAdvance;
}

LOOPCOUNTER::LOOPCOUNTER()
{
	if (!mConfigured)
	{
		mFirstLoop = true;
		mPreLoop = true;
		mCount = 0;
		mPeriod = 0;
		mCountMethod = COUNTMETHOD::SimpleAdvance;
		mCounterType = __loopcount::FirstLoop;
	}
}

LOOPCOUNTER::LOOPCOUNTER(uint32_t startValue)
{
	Configure(startValue);
}
LOOPCOUNTER::LOOPCOUNTER(COUNTMETHOD countMethod)
{
	Configure(countMethod);
}
LOOPCOUNTER::LOOPCOUNTER(uint32_t startValue, COUNTMETHOD countMethod, uint32_t resetPeriod)
{
	Configure(startValue, countMethod, resetPeriod);
}

LOOPCOUNTER::operator uint32_t()
{
	return mCount;
}
void LOOPCOUNTER::Configure(uint32_t startValue)
{
	Configure(startValue, COUNTMETHOD::SimpleAdvance, 0);
}
void LOOPCOUNTER::Configure(COUNTMETHOD countMethod)
{
	Configure(0, countMethod, 0);
}
void LOOPCOUNTER::Configure(COUNTMETHOD countMethod, int32_t resetPeriod)
{
	Configure(0, countMethod, resetPeriod);
}
void LOOPCOUNTER::Configure(uint32_t startValue, COUNTMETHOD countMethod, uint32_t resetPeriod)
{
	if (!mConfigured)
	{
		mFirstLoop = true;
		mPreLoop = true;
		mCount = 0;
		mPeriod = resetPeriod;
		mCountMethod = countMethod;
		mCounterType = __loopcount::SimpleAdvance;
		mConfigured = true;
	}
}

void LOOPCOUNTER::SetResetPeriod(uint32_t resetPeriod)
{
	mPeriod = resetPeriod;
}

bool LOOPCOUNTER::Advance()
{
	//xbugWatch(__FUNCTION__);
	return mCounterType->Advance(this);
}

bool LOOPCOUNTER::FirstLoop()
{
	return mFirstLoop;
}

void LOOPCOUNTER::Reset()
{
	mCount = 0;
}

void LOOPCOUNTER::ClearFirstLoop()
{
	if (mFirstLoop)
	{
		Serial.print("\n\nFirst Loop Cleared...\n\n");
		if (mCountMethod == COUNTMETHOD::SimpleAdvance)
		{
			mCounterType = __loopcount::SimpleAdvance;
		}
		if (mCountMethod == COUNTMETHOD::CatchClock)
		{
			mCounterType = __loopcount::ClockAdvance;
		}
		if (mCountMethod == COUNTMETHOD::PeriodReset)
		{
			mCounterType = __loopcount::PeriodReset;
		}
	}
}


namespace __loopcount
{
	bool SIMPLEADVANCE::Advance(LOOPCOUNTER* loopCounter)
	{
		loopCounter->mCount++;
		return true;
	}
	/*		Runs only ONCE, the first time. ClearFirstLoop() sets resets
	*	the pointer to the user configured COUNTMETHOD or to the default,
		SimpleAdvance. This just gives an option to do something with
		only the first loop with code here, or by testing the FirstLoop()
		call, before calling Advance() at the top of user loop...  */
	bool FIRSTLOOP::Advance(LOOPCOUNTER* loopCounter)
	{
		loopCounter->mCount++;
		loopCounter->mFirstLoop = false;
		loopCounter->ClearFirstLoop();
	}
	bool CLOCKADVANCE::Advance(LOOPCOUNTER* loopCounter)
	{

	}
	bool PERIODRESET::Advance(LOOPCOUNTER* loopCounter)
	{
		if (loopCounter->mCount > loopCounter->mPeriod)
		{
			loopCounter->mCount = 0;
			return true;
		}
		loopCounter->mCount++;
		return false;
	}
}

