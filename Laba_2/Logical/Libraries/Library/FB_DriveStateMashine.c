#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "Library.h"
#ifdef __cplusplus
	};
#endif

void FB_DriveStateMashine(struct FB_DriveStateMashine* inst)
{	
	UINT mask=inst->state & 0x6f;
	
	if (inst->state & 0x08 )
	{
		inst->command = (inst->command== CMD_FAULT_RESET) ? 0 : CMD_FAULT_RESET;
	}
	
	else if (!inst->enable)
	{
		inst->command = CMD_SHUTDOWN;
	}
	
	else
	{
		switch (mask)
		{
			case STATE_DISABLE:
			
				inst->command=CMD_SHUTDOWN;
				break;
			
			case STATE_READY:
			 
				inst->command=CMD_ENABLE;
				break;
			
			case STATE_SWITCH_ON:
			 
				inst->command=CMD_ENABLE;
				break;
			
			case STATE_OP_ENABLE:
			 
				inst->command=CMD_ENABLE;
				break;
			
			default:
				break;
		}
	}
	
	inst->speed = (mask == STATE_OP_ENABLE && inst->enable) ? inst->speedSet : 0;
}