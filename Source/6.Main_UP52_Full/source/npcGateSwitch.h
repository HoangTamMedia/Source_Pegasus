
#ifndef __NPC_GATE_SWITCH_H__
#define __NPC_GATE_SWITCH_H__

namespace   npcGateSwitch
{

    void    GateOpen ( CHARACTER* c, OBJECT* o );
    void    GateClose ( CHARACTER* c, OBJECT* o );
    void    DoInterfaceOpen ( int Key );
    void    ProcessState ( int Key, BYTE GateOnOff, BYTE State );

	void	SendToggleGate();
	bool	IsGateOpened();
};

using namespace npcGateSwitch;

#endif