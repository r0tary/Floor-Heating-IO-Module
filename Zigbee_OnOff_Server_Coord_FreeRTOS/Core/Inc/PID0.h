/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: PID0.h
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

#ifndef RTW_HEADER_PID0_h_
#define RTW_HEADER_PID0_h_
#ifndef PID0_COMMON_INCLUDES_
#define PID0_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* PID0_COMMON_INCLUDES_ */

#include "PID0_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S33>/Integrator' */
  real_T Filter_DSTATE;                /* '<S28>/Filter' */
  real_T Error;
} DW_PID0_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T u;                            /* '<Root>/u' */
} ExtU_PID0_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T y;                            /* '<Root>/y' */
} ExtY_PID0_T;

/* Real-time Model Data Structure */
struct tag_RTM_PID0_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_PID0_T PID0_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_PID0_T PID0_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_PID0_T PID0_Y;

/* Model entry point functions */
extern void PID0_initialize(void);
//extern real_T PID0_step(io_module_t *IO);
extern void PID0_terminate(void);

/* Real-time Model object */
extern RT_MODEL_PID0_T *const PID0_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S26>/Kb' : Eliminated nontunable gain of 1
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('IO_module_Design/PID Controller')    - opens subsystem IO_module_Design/PID Controller
 * hilite_system('IO_module_Design/PID Controller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'IO_module_Design'
 * '<S1>'   : 'IO_module_Design/PID Controller'
 * '<S2>'   : 'IO_module_Design/PID Controller/Anti-windup'
 * '<S3>'   : 'IO_module_Design/PID Controller/D Gain'
 * '<S4>'   : 'IO_module_Design/PID Controller/Filter'
 * '<S5>'   : 'IO_module_Design/PID Controller/Filter ICs'
 * '<S6>'   : 'IO_module_Design/PID Controller/I Gain'
 * '<S7>'   : 'IO_module_Design/PID Controller/Ideal P Gain'
 * '<S8>'   : 'IO_module_Design/PID Controller/Ideal P Gain Fdbk'
 * '<S9>'   : 'IO_module_Design/PID Controller/Integrator'
 * '<S10>'  : 'IO_module_Design/PID Controller/Integrator ICs'
 * '<S11>'  : 'IO_module_Design/PID Controller/N Copy'
 * '<S12>'  : 'IO_module_Design/PID Controller/N Gain'
 * '<S13>'  : 'IO_module_Design/PID Controller/P Copy'
 * '<S14>'  : 'IO_module_Design/PID Controller/Parallel P Gain'
 * '<S15>'  : 'IO_module_Design/PID Controller/Reset Signal'
 * '<S16>'  : 'IO_module_Design/PID Controller/Saturation'
 * '<S17>'  : 'IO_module_Design/PID Controller/Saturation Fdbk'
 * '<S18>'  : 'IO_module_Design/PID Controller/Sum'
 * '<S19>'  : 'IO_module_Design/PID Controller/Sum Fdbk'
 * '<S20>'  : 'IO_module_Design/PID Controller/Tracking Mode'
 * '<S21>'  : 'IO_module_Design/PID Controller/Tracking Mode Sum'
 * '<S22>'  : 'IO_module_Design/PID Controller/Tsamp - Integral'
 * '<S23>'  : 'IO_module_Design/PID Controller/Tsamp - Ngain'
 * '<S24>'  : 'IO_module_Design/PID Controller/postSat Signal'
 * '<S25>'  : 'IO_module_Design/PID Controller/preSat Signal'
 * '<S26>'  : 'IO_module_Design/PID Controller/Anti-windup/Back Calculation'
 * '<S27>'  : 'IO_module_Design/PID Controller/D Gain/Internal Parameters'
 * '<S28>'  : 'IO_module_Design/PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S29>'  : 'IO_module_Design/PID Controller/Filter ICs/Internal IC - Filter'
 * '<S30>'  : 'IO_module_Design/PID Controller/I Gain/Internal Parameters'
 * '<S31>'  : 'IO_module_Design/PID Controller/Ideal P Gain/Passthrough'
 * '<S32>'  : 'IO_module_Design/PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S33>'  : 'IO_module_Design/PID Controller/Integrator/Discrete'
 * '<S34>'  : 'IO_module_Design/PID Controller/Integrator ICs/Internal IC'
 * '<S35>'  : 'IO_module_Design/PID Controller/N Copy/Disabled'
 * '<S36>'  : 'IO_module_Design/PID Controller/N Gain/Internal Parameters'
 * '<S37>'  : 'IO_module_Design/PID Controller/P Copy/Disabled'
 * '<S38>'  : 'IO_module_Design/PID Controller/Parallel P Gain/Internal Parameters'
 * '<S39>'  : 'IO_module_Design/PID Controller/Reset Signal/Disabled'
 * '<S40>'  : 'IO_module_Design/PID Controller/Saturation/Enabled'
 * '<S41>'  : 'IO_module_Design/PID Controller/Saturation Fdbk/Disabled'
 * '<S42>'  : 'IO_module_Design/PID Controller/Sum/Sum_PID'
 * '<S43>'  : 'IO_module_Design/PID Controller/Sum Fdbk/Disabled'
 * '<S44>'  : 'IO_module_Design/PID Controller/Tracking Mode/Disabled'
 * '<S45>'  : 'IO_module_Design/PID Controller/Tracking Mode Sum/Passthrough'
 * '<S46>'  : 'IO_module_Design/PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S47>'  : 'IO_module_Design/PID Controller/Tsamp - Ngain/Passthrough'
 * '<S48>'  : 'IO_module_Design/PID Controller/postSat Signal/Forward_Path'
 * '<S49>'  : 'IO_module_Design/PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* RTW_HEADER_PID0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
