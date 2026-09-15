
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif
/* TODO: Add your comment here */
void FB_integrator(struct FB_integrator* inst)
{
		inst->out = inst->out + inst->in * inst->dt;
}
