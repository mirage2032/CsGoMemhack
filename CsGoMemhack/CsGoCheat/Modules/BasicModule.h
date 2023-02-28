#pragma once
#include "../CsGoCheat.h"
class BasicModule
{
protected:
	CsGoCheat *cheat;
	
public:
	BasicModule(CsGoCheat* cheat);
	virtual void Tick();
	~BasicModule();
};

