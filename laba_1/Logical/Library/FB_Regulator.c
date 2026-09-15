
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif

void FB_Regulator(struct FB_Regulator* inst)
{
	REAL up;       
	REAL ui;       
	REAL u_raw;  
	REAL u_lim;

	if (inst->dt <= 0.0f) inst->dt = 0.01f;
	if (inst->max_abs_value <= 0.0f) inst->max_abs_value = 24.0f;

	up = inst->Kp * inst->e;

	if (up >  inst->max_abs_value) up =  inst->max_abs_value;
	if (up < -inst->max_abs_value) up = -inst->max_abs_value;

	inst->integrator.dt = 1.0f;
	inst->integrator.in = inst->Ki * inst->dt * inst->e + inst->iyOld;
	FB_integrator(&inst->integrator);
	ui = inst->integrator.out;

	u_raw = up + ui;

	u_lim = u_raw;
	if (u_lim >  inst->max_abs_value) u_lim =  inst->max_abs_value;
	if (u_lim < -inst->max_abs_value) u_lim = -inst->max_abs_value;

	inst->iyOld = u_lim - u_raw;

	inst->u = u_lim;
}
