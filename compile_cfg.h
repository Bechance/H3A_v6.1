#ifndef _PEC_COMPILE_CFG_2010_H
#define  _PEC_COMPILE_CFG_2010_H

/*********************************************************************
*用户版本控制
**********************************************************************/
#define   		MY_SOFT_VERSION          		410u  /*软件版本号*/
/*********************************************************************
*可选支持模块配置
**********************************************************************/
#define   		DEBUG_LOG_EN               		0/*是否起用调试输出模块*/

#define   		TIMER_MANEGER_EN  				0/*是否起用硬件定时器管理模块*/

#define   		TASK_MONITOR_EN    				0/*是否启用任务监视模块*/

#define			TIMER_ARRAY_EN             		1/*是否启用软件定时器管理模块*/

#define			LINK_LIST_MANEGER_EN  			0/*是否起用动态链表支持模块*/

#define			FIFO_EN							1/*是否起用FIFO支持模块*/

#define     	UC_SHELL_EN                     0/*是否起用用户微型 shell*/

#define			MEM_MANEGER_EN                 	1/*是否起用内存管理单元*/

#define    		COMPILER_MALLOC_SUPPORT  	    0/*编译器是否提供动态申请内存的功能*/
/*********************************************************************
*指定任务优先级安排
**********************************************************************/
#define        	TASK_HIGHEST_PRO        		0/*应用中使用的最高优先级*/

#define        	MY_TASK_LOWEST_PRO          	(MY_TASK_MAX_NUM-1)  /*应用中使用的最高低先级*/

#define         CHECK_TASK_PRO                  (MY_TASK_LOWEST_PRO-1)

#define        	LOG_TASK_OUTPUT_PRO        		(MY_TASK_LOWEST_PRO-2)/*DEBUG模块中LOGOUT任务优先级*/

#define        	LOG_TASK_INPUT_PRO         		(LOG_TASK_OUTPUT_PRO-2)/*DEBUG模块中LOGIN任务优先级*/

#define        	TIMER_ARRAY_TASK_PRO       		(LOG_TASK_INPUT_PRO-2)/*软件定时器任务优先级,占用2个堆栈*/

#define        	UCSHELL_TASK_PRO            	(TIMER_ARRAY_TASK_PRO-2)/*ucshell管理任务优先级,占用1个堆栈*/

#define        	MONITOR_TASK_PRO            	(TASK_HIGHEST_PRO) /*系统任务监视优先级*/

#define        	MAIN_TASK_PRO            		(TASK_HIGHEST_PRO + TASK_MONITOR_EN+1)/*主任务优先级*/

#define        	BUZ_TASK_PRO        			(TASK_HIGHEST_PRO + TASK_MONITOR_EN )/*蜂鸣器任务优先级*/

#endif
