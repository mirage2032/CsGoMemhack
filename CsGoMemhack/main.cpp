#include <iostream>
#include "MemMan.h"
#include "csgo.hpp"
#include "CsGoCheat/CsGoCheat.h"

struct variables
{
	DWORD localPlayer = NULL;
	DWORD gameModule;
} val;

int main()
{
	CsGoCheat cheat;
	MemMan MemClass;
	std::cout << "Identifying game memory\n";
	int procID = MemClass.getProcess(L"csgo.exe");
	val.gameModule = MemClass.getModule(procID, L"client.dll");
	std::cout << "Memory identified\n";

	std::cout << "Finding local player\n";
	while (val.localPlayer == NULL)
	{
		val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + hazedumper::signatures::dwLocalPlayer);
		Sleep(1000);
	}
	std::cout << "Local player found\n";

	DWORD glowObject = MemClass.readMem<DWORD>(val.gameModule + hazedumper::signatures::dwGlowObjectManager);
	int myTeam = MemClass.readMem<int>(val.localPlayer + hazedumper::netvars::m_iTeamNum);

	std::cout << "Overriding game glow function\n";
	byte offsetRefreshGlow[5]; //should be: 0xe8, 0x6d, 0x0c, 0x00
	for (int i = 0; i < 5; i++)
	{
		offsetRefreshGlow[i] = MemClass.readMem<byte>(val.gameModule + 0x3d91ce + i);
		MemClass.writeMem<byte>(val.gameModule + 0x3d91ce + i, 0x90);
	}

	while (true)
	{
		for (int i = 0; i < 64; i++)
		{
			DWORD entity = MemClass.readMem<DWORD>(val.gameModule + hazedumper::signatures::dwEntityList + i * 0x10);
			DWORD entGlowIndex = MemClass.readMem<DWORD>(entity + hazedumper::netvars::m_iGlowIndex);
			int team = MemClass.readMem<int>(entity + hazedumper::netvars::m_iTeamNum);
			int health = MemClass.readMem<int>(entity + hazedumper::netvars::m_iHealth) / 100.0f;
			if (team == myTeam) //friendly
			{
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x08, 0);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x0C, 1 - health);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x10, 1);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x14, 1);
			}
			else //enemy
			{
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x08, 1);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x0C, health);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x10, 0);
				MemClass.writeMem<float>(glowObject + entGlowIndex * 0x38 + 0x14, 1);
			}
			MemClass.writeMem<bool>(glowObject + entGlowIndex * 0x38 + 0x28, true); //render occluded
			MemClass.writeMem<bool>(glowObject + entGlowIndex * 0x38 + 0x29, false); //render unoccluded
		}
		Sleep(100);
	}

	return 0;
}
