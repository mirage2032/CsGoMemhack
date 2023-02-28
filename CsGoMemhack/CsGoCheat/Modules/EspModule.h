#pragma once
#include "BasicModule.h"
#include "../CsGoCheat.h"

class EspModule :
    public BasicModule
{
private:
    int myTeam;
    byte offsetRefreshGlow[5];
    DWORD glowObject;
    void overrideGlowFunction(bool reset);
public:
    EspModule(CsGoCheat* cheat);
    void Tick() override;
    ~EspModule();
};

