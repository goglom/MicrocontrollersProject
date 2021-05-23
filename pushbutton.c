#include "pushbutton.h"

static void FrontUpDownToggle(PushButton* this, bool frontState)
{
	if (frontState)
		this->state = true;
	else
		this->state = false;
}

static  void FrontUpSwitchHandler(PushButton* this, bool frontState)
{
	if (frontState)
		this->state = !this->state;
}

void PushButtonCtor(PushButton* this, int mode, int ticksImmunity)
{
	this->prevRawState = false;
	this->state = false;
	this->stableState = false;
	this->ticksCounter = 0;
	this->ticksImmunity = ticksImmunity;
	
	switch (mode)
	{
		case FrontUpDowToggle:
			this->stateHandler = &FrontUpDownToggle;
			break;
		case FrontUpSwitch:
		default:
			this->stateHandler = &FrontUpSwitchHandler;
	}
}

void PushButtonUpdate(PushButton* this, bool newState)
{	
	if (newState != this->prevRawState)
		this->ticksCounter = 0;
	else
		++this->ticksCounter;
	
	if (this->ticksCounter >= this->ticksImmunity)
	{
		if (this->stableState != newState)
		{
			this->stableState = newState;
			bool frontState = newState;
			this->stateHandler(this, frontState);
		}
		this->ticksCounter = 0;
	}
	this->prevRawState = newState;
} 
