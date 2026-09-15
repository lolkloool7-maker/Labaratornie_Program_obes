
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#define DT        0.01f   
#define KE        0.048f  
#define TM        0.10f
#define TG        0.02f
#define UMAX      24.0f

#define STEP_TIME 50
#define SPEED_SET 60.0f

void _INIT ProgramInit(void)
{
	fb_motor.Ke = KE;
	fb_motor.Tm = TM;
	fb_motor.dt = DT;
		fb_motor.integrator.dt = DT;
		fb_motor.w = 0.0f;
	fb_motor.phi = 0.0f;

	fb_motor_2.Ke = KE;
	fb_motor_2.Tm = TM;
	fb_motor_2.dt = DT;
		fb_motor_2.integrator.dt = DT;
		fb_motor_2.w = 0.0f;
	fb_motor_2.phi = 0.0f;

	fb_regulator.Kp = KE * TM / TG;
	fb_regulator.Ki = KE / TG;
	fb_regulator.max_abs_value = UMAX;
	fb_regulator.dt = DT;
		fb_regulator.integrator.dt = 1.0f;
	fb_regulator.iyOld = 0.0f;

	speed   = 0.0f;
	counter = 0;
	enable  = 1;
}

void _CYCLIC ProgramCyclic(void)
{
	counter++;

	if (counter < STEP_TIME)            speed = 0.0f;
	else if (counter < 2 * STEP_TIME)   speed = SPEED_SET;
	else if (counter < 3 * STEP_TIME)   speed = 0.0f;
	else                                counter = 0;

	if (enable)
	{
		fb_regulator.e = speed - fb_motor.w;
		FB_Regulator(&fb_regulator);
		fb_motor.u = fb_regulator.u;
	}
	else
	{
		fb_regulator.e = 0.0f;
		fb_regulator.integrator.out = 0.0f;
		fb_regulator.iyOld = 0.0f;
		fb_regulator.u = 0.0f;
		fb_motor.u = 0.0f;
	}

	FB_Motor(&fb_motor);

	fb_motor_2.u = speed * KE;
	FB_Motor(&fb_motor_2);
}

void _EXIT ProgramExit(void)
{
}

