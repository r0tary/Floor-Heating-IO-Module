/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: PID0.c
 *
 * Code generated for Simulink model 'PID0'.
 *
 * Model version                  : 1.49
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Sat Dec  9 12:00:31 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

//#include "PID0.h"
#include "main.h"
#include "rtwtypes.h"
#include "IO_Config.h"

/* Block states (default storage) */
DW_PID0_T PID0_DW;

/* External inputs (root inport signals with default storage) */
ExtU_PID0_T PID0_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_PID0_T PID0_Y;

/* Real-time model */
static RT_MODEL_PID0_T PID0_M_;
RT_MODEL_PID0_T *const PID0_M = &PID0_M_;

// Tuning parameters
real_T kP = 0.3;
real_T kI = 0.15;
real_T kD = 0.0;


/* Model step function */
float rtb_Sum = 0;
real_T Input;
real_T SetPoint;
real_T PID0_step(io_module_t *IO)
{
  //Using Zigbee
  if (IO->ThermSetpointTemp != 273)
  {
	  SetPoint = IO->ThermSetpointTemp; //+273.15;
	  Input = SetPoint - IO->ThermTempRoom;   // + 273.15;
  }

  else
  {
	  SetPoint = IO->u16regsHR[SET_POINT_TEMP];
	  Input = SetPoint - (IO->TempRoom + 273.15);
  }

  real_T rtb_FilterCoefficient;

  /*
  // Derivative gain 0.0
  rtb_FilterCoefficient = (kD * PID0_U.u - PID0_DW.Filter_DSTATE) * 100.0;
  */

  // Proportional gain 0.001
  rtb_Sum = (kP * Input + IO->PID_Param.Integrator_DSTATE);


  if (rtb_Sum > 0.9) {
    PID0_Y.y = 0.9;

  } else if (rtb_Sum < 0.0) {
    PID0_Y.y = 0.0;

  } else {
    PID0_Y.y = rtb_Sum;
  }

  // Integral gain 0.001
  IO->PID_Param.Integrator_DSTATE += ((Input + IO->PID_Param.Error) * kI);

  if(IO->PID_Param.Integrator_DSTATE >= 0.9)
	  IO->PID_Param.Integrator_DSTATE = 0.9;

  else if(IO->PID_Param.Integrator_DSTATE < 0)
	  IO->PID_Param.Integrator_DSTATE = 0;

  /*
  PID0_DW.Filter_DSTATE += 50.0 * rtb_FilterCoefficient;
  */

  IO->PID_Param.Error = Input;

  return PID0_Y.y;

}

/* Model initialize function */
void PID0_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void PID0_terminate(void)
{
  /* (no terminate code required) */
}
