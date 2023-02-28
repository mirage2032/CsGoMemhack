#include <iostream>
#include "../csgo.hpp"
#include "CsGoCheat.h"
#include "Modules/EspModule.h"

CsGoCheat::CsGoCheat()
{
	std::cout << "Identifying game memory area" << std::endl;
	uintptr_t procID = MemClass.getProcess(L"csgo.exe");
	if (procID==NULL)
	{
		std::cout << "Process \"csgo.exe\" not found\nMake sure the game is opend." << std::endl;
		return;
	}
	gameModule = MemClass.getModule(procID, L"client.dll");
	std::cout << "Memory area identified" << std::endl;
	modules[0] = new EspModule(this);
	init_success_ = true;
}

void CsGoCheat::GetLocalPlayer()
{
	localPlayer = MemClass.readMem<DWORD>(gameModule + hazedumper::signatures::dwLocalPlayer);
}

DWORD CsGoCheat::GetEntity(int index)
{
	return MemClass.readMem<DWORD>(gameModule + hazedumper::signatures::dwEntityList + index * 0x10);
}

void CsGoCheat::Execute()
{
	while (true)
	{
		Sleep(100);
		GetLocalPlayer();
		if(localPlayer == NULL)
			continue;
		for (int i=0;i<modules.size();i++)
		{
			modules[i]->Tick();
		}
	}
}

CsGoCheat::~CsGoCheat()
{
}
