#ifndef _PUSH_BUTTON_
#define _PUSH_BUTTON_

#include <stdbool.h>

typedef struct _PushButton{
	bool state;
	void (*stateHandler)(struct _PushButton*, bool);
	bool prevRawState;
	bool stableState;
	int ticksCounter;
	int ticksImmunity;
} PushButton;

void PushButtonCtor(PushButton* this, int mode, int ticksImmunity);
void PushButtonUpdate(PushButton* this, bool newState);
bool PushButtonGetState(PushButton* this);

enum PBMode{
	FrontUpSwitch,
	FrontUpDowToggle,
};

#endif
