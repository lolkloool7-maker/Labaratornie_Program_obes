
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif

#define RPM_TO_RADS  0.10471975512f   /* 2*pi/60 */

void FB_Motor(struct FB_Motor* inst)
{
	REAL dw;

	if (inst->Ke == 0.0f) inst->Ke = 1.0f;
	if (inst->Tm <= 0.0f) inst->Tm = 1.0f;
	if (inst->dt <= 0.0f) inst->dt = 0.01f;

	dw = (inst->u / inst->Ke - inst->w) / inst->Tm;

	inst->integrator.dt = inst->dt;
	inst->integrator.in = dw;
	FB_integrator(&inst->integrator);

	inst->w = inst->integrator.out;      /* частота вращения [об/мин] */

	inst->phi = inst->phi + inst->w * RPM_TO_RADS * inst->dt;   /* [рад] */
}
