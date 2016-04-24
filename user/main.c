/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:主函数功能
* 创建时间:2010-06-29
* 所属公司 :科荟
* 文件名称:main.c
* 创建人 :揭成
* 功能描述:主功能，程序入口，将我们系统
* 带到一个合理的状态下。
*----------------------------------------------------------------
修改记录:增加user_main_task
目的:当能保证任务开始后中断才发生的话，可以
不需要该任务，节省一个堆栈空间，为了安全
还是保留该任务，由该任务统一初始化用户应用
模块。
修改日期:7月7日
修改人: 揭成 
*********************************************************************************************************/
#include "includes.h"
/********************************************************************
 * @创建人:揭成
 *
 * @功能:系统故障入口
 * @输入:NONE
 *
 *@输出: 当系统出现异常时候，用户可以调用该函数
********************************************************************/
void mod_err_inlet( void )
{
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:初始化系统将要使用的中断
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void os_isr_init( void )
{
	InitCpuTimers();
	EALLOW;						   			 /*This is needed to write to EALLOW protected registers*/
	PieVectTable.USER12 = &OSCtxSw;			 /*将用户中断12定为正常任务切换入口*/
	IER|=M_INT14;
	PieVectTable.TINT2  = &OSTickISR;		 /*将定时器0作为系统节拍中断*/
	ConfigCpuTimer( &CpuTimer2,150,10000);	 /*系统节拍中断时间为10MS*/
	StartCpuTimer2();                		 /*启动定时器2*/
	ERTM;
	EINT;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:初始化支持模块,编译配置选项，在compile_cfg.h中
 * @输入:NONE
 * @输出:NONE
********************************************************************/
void  support_mod_init( void )
{
#if   TASK_MONITOR_EN>0
	thread_monitor_insmod();/*监视任务初始化*/
#endif
#if  (LINK_LIST_MANEGER_EN >0)
	link_list_maneger_init();/*动态链表管理初始化*/
#endif
#if TIMER_ARRAY_EN>0
	timer_array_mod_init();/*定时器管理任务初始化*/
#endif
#if   DEBUG_LOG_EN    >0
	log_debug_insmod(); /*系统调试口初始化*/
#endif
#if   UC_SHELL_EN    >0
	ucshell_entery(); 	/*系统调试口初始化*/
#endif
#if   MEM_MANEGER_EN    >0
	mem_maneger_init();
#endif
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能:初始化系统功能模块，对各个模块的注册
 * @输入:NONE
 * @输出:NONE
********************************************************************/
#pragma DATA_SECTION (m_mod_list_head,"MOD_LIST_HEAD")
#pragma DATA_SECTION (m_DR_list_head, "DR_LIST_HEAD")
static char m_mod_list_head;
static char m_DR_list_head;
/**********************************************************************/
void app_mod_mount( void )
{
    Bsize_t offset;
	Bsize_t id;
	mod_member_t*plist;
	DR_member_t *pDR;
	char*phead = (char*)&m_mod_list_head;
    plist=(mod_member_t*)(phead+0x10);
	for(offset=0;offset<25;offset++)
	{
		if(plist[offset].crc==0x5a5a)
		{
            id=plist[offset].os_mod_add();
			app_module_set_name(id,plist[offset].name);
		}
	}
	phead = (char*)&m_DR_list_head;
    pDR=(DR_member_t*)(phead+0x10);
	for(offset=0;offset<25;offset++)
	{
		if(pDR[offset].crc==0x5a5a)
		{
            pDR[offset].driver_register();
		}
	}
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:得到第一个运行的模块ID
 * @输入:NONE
 * @输出:NONE
********************************************************************/
Bsize_t app_get_1th_mod_id( void )
{
	return app_module_ID_obtain((char*)"START");
}
/********************************************************************
 * @创建人 :揭成
 * @功能: 启动第一个需要运行的模块准备进入模块管理
 *
 * @输入:NONE
 *
 *@输出:NONE
 当检测到没有启动模块挂载时候，系统将回重新
********************************************************************/
void app_start_module( void )
{
	Bsize_t mod_id = app_get_1th_mod_id();
	if ( !mod_id )return;
	buz_response( 15);
	app_module_send_hmsg( APP_MOD_START, mod_id, 0 ); 
	return;
}
/********************************************************************
 * @创建人:揭成
 * @功能:用户主任务，由该任务创建各应用
 * 模块任务，并进行模块的管理。
 * @输入:p_arg 任务参数
 * @输出:NONE
********************************************************************/
void user_main_task( void *p_arg )
{
	os_isr_init();       	 			/*系统中断 初始化，节拍中断和任务切换中断*/
//	OSStatInit();                       /*初始化统计任务*/
	while ( 1 )
	{
		app_mod_mount();				/*功能应用模块初始化*/
		support_mod_init();				/*初始化支持模块*/
		app_global_resouce_init();		/*全局共享资源初始化*/
		app_start_module();				/*启动第一个应用模块*/
		app_module_state_handle(0);		/*开始管理模块状态*/
		msleep( TYPE_DELAY );
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能:应用模块初始化
 *
 * @输入:NONE
 *
 *@输出:NONE
********************************************************************/
void app_init( void )
{
	os_connect_init();											 /*初始化系统连接层*/

	os_task_assign_pro_build( user_main_task, MAIN_TASK_PRO, 5 );/*创建系统主任务*/

	os_task_name_set( MAIN_TASK_PRO, "Global--Module manege" );  /*为主任务设置名称*/
	return;
}
/********************************************************************
* @创建人:揭成
* @功能:主函数启动ucos,初始化应用模块
*
* @输入:NONE
*
* @输出:NONE
********************************************************************/
void  main( void )
{
	bsp_init();   			/*初始化硬件配置*/
	OSInit();			 	/*初始化系统*/
	app_init();   			/*初始化应用模块*/
	OSStart();             	/*开始任务*/
	return ;
}
