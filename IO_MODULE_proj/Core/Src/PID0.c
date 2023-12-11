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

#include "PID0.h"
#include "rtwtypes.h"

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
real_T kP = 0.002;
real_T kI = 0.001;
real_T kD = 0.0;

float rtb_Sum = 0;
float temp_err = 0;

/* Model step function */
void PID0_step(double Temp)
{
  real_T Input = 293.15 - (Temp + 273.15);
  real_T rtb_FilterCoefficient;
  PID0_U.u = Input;
  /* Gain: '<S36>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S28>/Filter'
   *  Gain: '<S27>/Derivative Gain'
   *  Inport: '<Root>/u'
   *  Sum: '<S28>/SumD'
   */
  // Derivative gain 0.0
  rtb_FilterCoefficient = (kD * PID0_U.u - PID0_DW.Filter_DSTATE) * 100.0;

  /* Sum: '<S42>/Sum' incorporates:
   *  DiscreteIntegrator: '<S33>/Integrator's
   *  Gain: '<S38>/Proportional Gain'
   *  Inport: '<Root>/u'
   */
  // Proportional gain 0.001
  rtb_Sum = (kP * PID0_U.u + PID0_DW.Integrator_DSTATE) +
    rtb_FilterCoefficient;


  /* Saturate: '<S40>/Saturation' */
  if (rtb_Sum > 1.0) {
    /* Saturate: '<S40>/Saturation' */
    PID0_Y.y = 1.0;
  } else if (rtb_Sum < 0.0) {
    /* Saturate: '<S40>/Saturation' */
    PID0_Y.y = 0.0;
  } else {
    /* Saturate: '<S40>/Saturation' */
    PID0_Y.y = rtb_Sum;
  }

  /* End of Saturate: '<S40>/Saturation' */

  /* Update for DiscreteIntegrator: '<S33>/Integrator' incorporates:
   *  Gain: '<S30>/Integral Gain'
   *  Inport: '<Root>/u'
   *  Sum: '<S26>/SumI2'
   *  Sum: '<S26>/SumI4'
   */
  // Integral gain 0.001
  PID0_DW.Integrator_DSTATE += ((Input + temp_err) * kI);

  /* Update for DiscreteIntegrator: '<S28>/Filter' */
  PID0_DW.Filter_DSTATE += 50.0 * rtb_FilterCoefficient;

  temp_err = Input;
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
