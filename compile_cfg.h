#ifndef _PEC_COMPILE_CFG_2010_H
#define  _PEC_COMPILE_CFG_2010_H

/*********************************************************************
*�û��汾����
**********************************************************************/
#define   		MY_SOFT_VERSION          		410u  /*����汾��*/
/*********************************************************************
*��ѡ֧��ģ������
**********************************************************************/
#define   		DEBUG_LOG_EN               		0/*�Ƿ����õ������ģ��*/

#define   		TIMER_MANEGER_EN  				0/*�Ƿ�����Ӳ����ʱ������ģ��*/

#define   		TASK_MONITOR_EN    				0/*�Ƿ������������ģ��*/

#define			TIMER_ARRAY_EN             		1/*�Ƿ����������ʱ������ģ��*/

#define			LINK_LIST_MANEGER_EN  			0/*�Ƿ����ö�̬����֧��ģ��*/

#define			FIFO_EN							1/*�Ƿ�����FIFO֧��ģ��*/

#define     	UC_SHELL_EN                     0/*�Ƿ������û�΢�� shell*/

#define			MEM_MANEGER_EN                 	1/*�Ƿ������ڴ����Ԫ*/

#define    		COMPILER_MALLOC_SUPPORT  	    0/*�������Ƿ��ṩ��̬�����ڴ�Ĺ���*/
/*********************************************************************
*ָ���������ȼ�����
**********************************************************************/
#define        	TASK_HIGHEST_PRO        		0/*Ӧ����ʹ�õ�������ȼ�*/

#define        	MY_TASK_LOWEST_PRO          	(MY_TASK_MAX_NUM-1)  /*Ӧ����ʹ�õ���ߵ��ȼ�*/

#define         CHECK_TASK_PRO                  (MY_TASK_LOWEST_PRO-1)

#define        	LOG_TASK_OUTPUT_PRO        		(MY_TASK_LOWEST_PRO-2)/*DEBUGģ����LOGOUT�������ȼ�*/

#define        	LOG_TASK_INPUT_PRO         		(LOG_TASK_OUTPUT_PRO-2)/*DEBUGģ����LOGIN�������ȼ�*/

#define        	TIMER_ARRAY_TASK_PRO       		(LOG_TASK_INPUT_PRO-2)/*�����ʱ���������ȼ�,ռ��2����ջ*/

#define        	UCSHELL_TASK_PRO            	(TIMER_ARRAY_TASK_PRO-2)/*ucshell�����������ȼ�,ռ��1����ջ*/

#define        	MONITOR_TASK_PRO            	(TASK_HIGHEST_PRO) /*ϵͳ����������ȼ�*/

#define        	MAIN_TASK_PRO            		(TASK_HIGHEST_PRO + TASK_MONITOR_EN+1)/*���������ȼ�*/

#define        	BUZ_TASK_PRO        			(TASK_HIGHEST_PRO + TASK_MONITOR_EN )/*�������������ȼ�*/

#endif
