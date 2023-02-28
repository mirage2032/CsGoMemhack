#pragma once
#include "../MemMan.h"
#include "Modules/BasicModule.h"
#include "vector"

class CsGoCheat
{
public:
	std::vector<BasicModule*> modules;
	MemMan MemClass;
	DWORD gameModule;
	DWORD localPlayer;
	bool init_success_ = false;

	CsGoCheat();
	void GetLocalPlayer();
private:
	DWORD GetEntity(int index);
	void Execute();
	~CsGoCheat();
};
