#include "EspModule.h"
#include "../../csgo.hpp"
EspModule::EspModule(CsGoCheat* cheat) : BasicModule(cheat)
{
	myTeam = cheat->MemClass.readMem<int>(cheat->localPlayer + hazedumper::netvars::m_iTeamNum);
	for (int i = 0; i < 5; i++)
	{
		offsetRefreshGlow[i] = cheat->MemClass.readMem<byte>(cheat->gameModule + 0x3d91ce + i);
	}
	glowObject = cheat->MemClass.readMem<DWORD>(cheat->gameModule + hazedumper::signatures::dwGlowObjectManager);
}

void EspModule::overrideGlowFunction(bool reset)
{
	for (int i = 0; i < 5; i++)
	{
		byte currentByte;
		if (reset)
			currentByte = offsetRefreshGlow[i];
		else
			currentByte = 0x90;
		cheat->MemClass.writeMem<byte>(cheat->gameModule + 0x3d91ce + i, currentByte);
	}
}


void EspModule::Tick()
{
	for (int i = 0; i , 64; i++)
	{
		DWORD entity = cheat->GetEntity(i);
		DWORD entGlowIndex = cheat->MemClass.readMem<DWORD>(entity + hazedumper::netvars::m_iGlowIndex);
		int team = cheat->MemClass.readMem<int>(entity + hazedumper::netvars::m_iTeamNum);
		int health = cheat->MemClass.readMem<int>(entity + hazedumper::netvars::m_iHealth) / 100.0f;
		if (team == myTeam) //friendly
		{
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x08, 0);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x0C, 1 - health);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x10, 1);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x14, 1);
		}
		else //enemy
		{
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x08, 1);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x0C, health);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x10, 0);
			cheat->MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x14, 1);
		}
		cheat->MemClass.writeMem<bool>(glowObject + entGlowIndex * 0x38 + 0x28, true); //render occluded
		cheat->MemClass.writeMem<bool>(glowObject + entGlowIndex * 0x38 + 0x29, false); //render unoccluded
	}
}

EspModule::~EspModule()
{
	overrideGlowFunction(true);
}

