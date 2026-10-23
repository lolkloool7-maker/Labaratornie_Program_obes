#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "Library.h"
#ifdef __cplusplus
	};
#endif

#define CYCLE_MS      100
#define HALF_PERIOD   (500 / CYCLE_MS)

#define LED_0000  0x0
#define LED_0001  0x1
#define LED_0011  0x3
#define LED_0111  0x7
#define LED_1111  0xF

void FB_LedStateMashine(struct FB_LedStateMashine* inst)
{
	USINT pattern = LED_0000;

	switch (inst->state)
	{
		case ST_INIT:     pattern = LED_0000; break;

		case ST_UNKNOWN:
			inst->timer++;
			if (inst->timer >= HALF_PERIOD)
			{
				inst->timer = 0;
				inst->blinkOn = !inst->blinkOn;
			}
			pattern = inst->blinkOn ? LED_1111 : LED_0000;
			break;

		case ST_OPEN:     pattern = LED_0000; break;
		case ST_CLOSE:    pattern = LED_1111; break;

		/* закрытие */
		case ST_ACC_NEG:  pattern = LED_0001; break;
		case ST_NEG:      pattern = LED_0011; break;
		case ST_DEC_NEG:  pattern = LED_0111; break;

		/* открытие */
		case ST_ACC_POS:  pattern = LED_0111; break;
		case ST_POS:      pattern = LED_0011; break;
		case ST_DEC_POS:  pattern = LED_0001; break;

		default:          pattern = LED_0000; break;
	}

	if (inst->state != ST_UNKNOWN)
	{
		inst->timer = 0;
		inst->blinkOn = 0;
	}

	inst->led1 = (pattern & 0x1) != 0;
	inst->led2 = (pattern & 0x2) != 0;
	inst->led3 = (pattern & 0x4) != 0;
	inst->led4 = (pattern & 0x8) != 0;
}
