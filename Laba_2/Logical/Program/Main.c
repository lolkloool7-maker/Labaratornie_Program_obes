
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	driveSM.enable = 0;
	driveSM.speed = 0;
	driveSM.speedSet = 0;
	
	doorSM.doorState = ST_INIT;
	doorSM.speed = 0;
	
	ledSM.state = ST_INIT;
	ledSM.timer = 0;
	ledSM.blinkOn = 0;
}

void _CYCLIC ProgramCyclic(void)
{
	driveSM.enable = 1;
	
	doorSM.state = driveSM.state;
	FB_DoorStateMashine(&doorSM);
	
	driveSM.speedSet = doorSM.speed;
	FB_DriveStateMashine(&driveSM);
	
	ledSM.state = doorSM.doorState;
	FB_LedStateMashine(&ledSM);
	
}

void _EXIT ProgramExit(void)
{

}

