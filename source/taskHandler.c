/*******************************************************************************
@file     taskHandler.c
@brief    HANDLER DE TASKS
@author   Grupo 4
*******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
*******************************************************************************/

// #include "board.h"
#include "taskHandler.h"
#include  <os.h>
#include "hardware.h"
#include "thingspeak_interface.h"

/*******************************************************************************
* TASKS CONSTANTS, VARIABLES AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
// Task 1 - Start/App
#define TASKSTART_PRIO 			       2u
#define TASKSTART_STK_SIZE 		     512u
#define TASKSTART_STK_SIZE_LIMIT   (TASKSTART_STK_SIZE / 10u)
#define TASKSTART_TIMEOUT          0u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];

// Task 2 - LOGGING
#define TASKLOGGING_PRIO           3u
#define TASKLOGGING_STK_SIZE			 256u
#define TASKLOGGING_STK_SIZE_LIMIT (TASKLOGGING_STK_SIZE / 10u)
#define TASKLOGGING_TIMEOUT        0u
static OS_TCB TaskLoggingTCB;
static CPU_STK TaskLoggingStk[TASKLOGGING_STK_SIZE];
/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
*******************************************************************************/
void App_Init (void);
void App_Run (void* msg);
/*******************************************************************************
* VARIABLES WITH GLOBAL SCOPE
*******************************************************************************/
//Cada TASK se crea con su propio semaforo y message queue, messirve !!
// #define OS_CFG_TASK_Q_EN DEF_ENABLED //Creo que necesito esto
// OS_SEM semApp; //Semaforo para la App principal
// OS_Q   qLog; //Semaforo para el Logging de la Nube
/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
*******************************************************************************/
static void TaskStart(void *p_arg);
static void TaskLogging(void *p_arg);
/*******************************************************************************
* ROM CONST VARIABLES WITH FILE LEVEL SCOPE
*******************************************************************************/

/*******************************************************************************
* STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
*******************************************************************************/
static uint8_t msg_counter = 0;
/*******************************************************************************
*                     GLOBAL FUNCTION DEFINITIONS
*******************************************************************************/
void tasksInit(void)
{
  OS_ERR os_err; //Codigo de error del OS

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
  CPU_ERR  cpu_err;
#endif

	OSInit(&os_err);
  if (os_err != OS_ERR_NONE)
  {
      /* Something didn't get initialized correctly ...                    */
      /* ... check os.h for the meaning of the error code, see OS_ERR_xxxx */
  }

#if (OS_CFG_SCHED_ROUND_ROBIN_EN > 0u)
	 // Enable task round robin.
	 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &os_err);
#endif
	 OS_CPU_SysTickInit(SystemCoreClock / ((uint32_t)OSCfg_TickRate_Hz));

	 OSTaskCreate(&TaskStartTCB,
                "App Task Start",
                TaskStart,
                0u,
                TASKSTART_PRIO,
			 	        &TaskStartStk[0u],
                TASKSTART_STK_SIZE_LIMIT,
                TASKSTART_STK_SIZE,
				        0u,
                0u,
                0u,
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                &os_err);

	 OSStart(&os_err);
   if (os_err != OS_ERR_NONE)
   {
       /* Your code is NEVER supposed to come back to this point.              */
   }

   while (1) {/* Should Never Get Here */}
}
/*******************************************************************************
*                     LOCAL FUNCTION DEFINITIONS
*******************************************************************************/
static void TaskStart(void *p_arg)
{
  (void)      p_arg;
  OS_ERR      os_err;
  OS_SEM_CTR  ctr;
  CPU_TS      ts;

  /* Initialize the uC/CPU Services. and APP*/
  hw_DisableInterrupts();
  CPU_Init();
  App_Init(); /* Program-specific setup */

#if OS_CFG_STAT_TASK_EN > 0u
   // (optional) Compute CPU capacity with no task running
  OSStatTaskCPUUsageInit(&os_err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset();
#endif

  hw_EnableInterrupts();

  // Create TaskLogging
  OSTaskCreate(&TaskLoggingTCB,
               "APP",
               TaskLogging,
               0u,
               TASKLOGGING_PRIO,
               &TaskLoggingStk[0u],
               TASKLOGGING_STK_SIZE_LIMIT,
               TASKLOGGING_STK_SIZE,
     	   	     0u,
               0u,
               0u,
               (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               &os_err);


  while (1)
  {
    int msg = 0;
    // OSSemPend(&semApp);
    ctr = OSTaskSemPend(TASKSTART_TIMEOUT,OS_OPT_PEND_BLOCKING,&ts,&os_err);
    App_Run(&msg); //Vamos a buscar las esferas del dragon
    if(msg)
    {
      OSTaskQPost(&TaskLoggingTCB,
                  (void *) msg,
                  sizeof(msg),
                  OS_OPT_POST_LIFO,
                  &os_err);

    }
  }
}

/****************************************************************/
static void TaskLogging(void *p_arg)
{
  (void)        p_arg;
  OS_ERR        os_err;
  int           p_msg;
  OS_MSG_SIZE   msg_size;
  CPU_TS        ts;

  thingspeak_init();

  while(1)
  {
    p_msg = (int) OSTaskQPend(TASKLOGGING_TIMEOUT, OS_OPT_PEND_BLOCKING, &msg_size, &ts, &os_err);
    //Hace lo tuyo Pablo (subilo a la nube)

  }
}
