/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
#define  TASK_START_PRIO    5

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[TASK_STK_SIZE];
OS_STK        AppStartTaskStk1[TASK_STK_SIZE];
OS_STK        AppStartTaskStk2[TASK_STK_SIZE];
struct ti{
	short c;
	short p;
	INT8U dataGroup;
	INT8U dataPrio;
};

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
#endif

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
	INT8U  err;


#if 0
    BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */

	struct ti arg0 = { 1, 4, 0, 1 };
	struct ti arg1 = { 2, 5, 1, 1 };
	struct ti arg2 = { 2, 10, 1, 0};

    OSTaskCreateExt(AppStartTask,
                    (void *)&arg0,
                    (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
                    0,
                    0,
                    (OS_STK *)&AppStartTaskStk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(AppStartTask,
					(void*)&arg1,
					(OS_STK*)&AppStartTaskStk1[TASK_STK_SIZE - 1],
					1,
					1,
					(OS_STK*)&AppStartTaskStk1[0],
					TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(AppStartTask,
					(void*)&arg2,
					(OS_STK*)&AppStartTaskStk2[TASK_STK_SIZE - 1],
					2,
					2,
					(OS_STK*)&AppStartTaskStk2[0],
					TASK_STK_SIZE,
					(void*)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif

#if OS_TASK_NAME_SIZE > 14
    OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif

    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

void  AppStartTask (void *p_arg)
{
    struct ti *arg = (struct ti*)p_arg;

#if 0
    BSP_Init();                                  /* For embedded targets, initialize BSP functions                             */
#endif


#if OS_TASK_STAT_EN > 0
    OSStatInit();                                /* Determine CPU capacity                                                     */
#endif

 //   while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	//{      
	//	OSTimeDlyHMSM(0, 0, 1, 0);
 //   }
	OSTCBCur->compTime = arg->c;
	OSTCBCur->dataGroup = arg->dataGroup;
	OSTCBCur->dataPrio = arg->dataPrio;
	while (TRUE)                                 /* Task body, always written as an infinite loop.                             */
	{
		int start = OSTimeGet();
		while (OSTCBCur->compTime > 0) {
			// do nothing
		}
		int end = OSTimeGet();
		int to_delay = arg->p - (end - start);
		OSTCBCur->compTime = arg->c;
		if (to_delay < 0)
			continue;
		OSTimeDly(to_delay);
	}

}