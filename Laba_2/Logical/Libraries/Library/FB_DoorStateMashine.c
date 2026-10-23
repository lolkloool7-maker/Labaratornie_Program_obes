
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif

#define V_SEARCH 100
#define V_SLOW 200
#define V_FAST 850

#define ETAD_MASK 0x006F
#define ETAD_OP_ENABLED 0x0027

#define DIR_OPEN (+1)
#define DIR_CLOSE (-1)

void FB_DoorStateMashine(struct FB_DoorStateMashine* inst)
{
	BOOL driveReady = ((inst->state & ETAD_MASK) == ETAD_OP_ENABLED);
	
	BOOL e0 = inst->s0 != inst->s0old;
	BOOL e1 = inst->s1 != inst->s1old;
	BOOL e2 = inst->s2 != inst->s2old;
	BOOL e3 = inst->s3 != inst->s3old;
	
	inst->s0old = inst->s0;
	inst->s1old = inst->s1;
	inst->s2old = inst->s2;
	inst->s3old = inst->s3;
	
	if (!driveReady && inst->doorState != ST_INIT)
	{
		inst->doorState = ST_INIT;
	}

	switch (inst->doorState)
	{
		case ST_INIT:
			inst->speed = 0;
			if (driveReady)
				inst->doorState = ST_UNKNOWN;
			break;

		case ST_UNKNOWN:
			inst->speed = inst->direction ? DIR_CLOSE * V_SEARCH : DIR_OPEN * V_SEARCH;
			if (e0)
				inst->doorState = ST_OPEN;
			else if (e3)
				inst->doorState = ST_CLOSE;
			break;

		case ST_OPEN:
			inst->speed = 0;
			if (inst->direction == 1)
				inst->doorState = ST_ACC_NEG;
			break;

		case ST_CLOSE:
			inst->speed = 0;
			if (inst->direction == 0)
				inst->doorState = ST_ACC_POS;
			break;

		/* ---------- закрытие: s0 -> s1 -> s2 -> s3 ---------- */
		case ST_ACC_NEG:
			inst->speed = DIR_CLOSE * V_SLOW;
			if (inst->direction == 0)
				inst->doorState = ST_DEC_POS;   /* разворот в зоне s0-s1 */
			else if (e1)
				inst->doorState = ST_NEG;
			break;

		case ST_NEG:
			inst->speed = DIR_CLOSE * V_FAST;
			if (inst->direction == 0)
				inst->doorState = ST_POS;       /* разворот в зоне s1-s2 */
			else if (e2)
				inst->doorState = ST_DEC_NEG;
			break;

		case ST_DEC_NEG:
			inst->speed = DIR_CLOSE * V_SLOW;
			if (inst->direction == 0)
				inst->doorState = ST_ACC_POS;   /* разворот в зоне s2-s3 */
			else if (e3)
				inst->doorState = ST_CLOSE;
			break;

		/* ---------- открытие: s3 -> s2 -> s1 -> s0 ---------- */
		case ST_ACC_POS:
			inst->speed = DIR_OPEN * V_SLOW;
			if (inst->direction == 1)
				inst->doorState = ST_DEC_NEG;   /* разворот в зоне s3-s2 */
			else if (e2)
				inst->doorState = ST_POS;
			break;

		case ST_POS:
			inst->speed = DIR_OPEN * V_FAST;
			if (inst->direction == 1)
				inst->doorState = ST_NEG;       /* разворот в зоне s2-s1 */
			else if (e1)
				inst->doorState = ST_DEC_POS;
			break;

		case ST_DEC_POS:
			inst->speed = DIR_OPEN * V_SLOW;
			if (inst->direction == 1)
				inst->doorState = ST_ACC_NEG;   /* разворот в зоне s1-s0 */
			else if (e0)
				inst->doorState = ST_OPEN;
			break;

		default:
			inst->speed = 0;
			inst->doorState = ST_INIT;
			break;
	}
}
