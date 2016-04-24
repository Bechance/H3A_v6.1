/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2005, Jean J. Labrosse, Weston, FIL
*                                           All Rights Reserved
*
* File    : uCOS_II.C
* By      : Jean J. Labrosse
* Version : V2.82
*********************************************************************************************************
*/
#define  OS_GLOBALS                           /* Declare GLOBAL variables                              */
#include "includes\ucos_ii.h"
#define  OS_MASTER_FILE                       /* Prevent the following files from including includes.h */
#include "os\os_core.c"
#include "os\os_flag.c"
#include "os\os_mbox.c"
#include "os\os_mem.c"
#include "os\os_mutex.c"
#include "os\os_q.c"
#include "os\os_sem.c"
#include "os\os_task.c"
#include "os\os_time.c"
#include "os\os_tmr.c"
