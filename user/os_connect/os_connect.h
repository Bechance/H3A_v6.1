#ifndef _2010_PEC_OS_CONNECT_H
#define _2010_PEC_OS_CONNECT_H
#include "ostype.h"
#include "../../kernel\includes\ucos_ii.h"

#ifndef  OS_DBGERR
#define  OS_DBGERR         	DBGERR
#endif
#define  DLOG_NP            
/*---------------------------------------------------------------------
    os_connect 配置
---------------------------------------------------------------------*/
#define 		MAX_SURPPORT_DRIVER 		25  /*能支持驱动个数*/

#define 		DRIVER_NAM_SIZE				20  /*驱动名称长度*/

#define  		MAX_MOD_NUM    	     		18  /*能支持模块个数*/

#define     	MAX_MOD_NAME         		20  /*模块名称长度*/

#define 		MY_TASK_MAX_NUM   			47  /*USER任务数量7552/ADVISE_STK_SIZE*/

#define 		TYPE_DELAY  		       	3u  /*建议任务睡眠节拍 30MS */

#define 		APP_HALF_SEC  				50u /*0.5S节拍*/

#define 		APP_100_MS  				10u /*100ms节拍*/

#define   		ADVISE_STK_SIZE    			150u/*建议任务使用的堆栈*/

#define   		TYPE_TIME_OUT     			50u /* 建议等待信号时间500ms(溢出时间)*/

/*函数编译配置*/

typedef              OS_EVENT 	a_lock_t;
typedef              OS_EVENT 	a_sem_t;
typedef              OS_EVENT 	a_q_t;

enum
{
	PRO_IS_IDLE 		= 'F',      /*空闲索引*/
	PRO_IS_MAIN_OCCUPY  = 'M',   	/*任务占用*/
	PRO_IS_SLAVE_OCCUPY = 'S'  		/*附属任务占用*/
};
enum
{
	LOCK_IS_CREATE = 0,
	LOCK_IS_INVAILD,
	LOCK_IS_REVERT,
	SEM_IS_CREATE ,
	SEM_IS_INVAILD,
	SEM_IS_REVERT
};
#define  MAX_STK_NBLK           (12)
/*系统接口函数*/
void 			os_lock( a_lock_t*sem );     /*给要保护的资源上锁*/

void 			os_unlock( a_lock_t*sem );  /*给要保护的资源解锁*/

void 			os_lock_revert( a_lock_t*sem );/*将锁恢复到初始状态*/

uint8_t 		os_lock_creat( a_lock_t**sem );/*创建一个锁资源*/

uint8_t 		os_sem_creat( a_sem_t**sem, uint16_t cnt );/*创建一个信号量*/

uint8_t 		os_sem_revert( a_sem_t*sem, uint16_t cnt );/*将信号量恢复到初始状态*/

#define         os_sem_post            		OSSemPost

#define         os_task_delete_req          OSTaskDelReq

uint8_t 		os_sem_pend( a_sem_t*sem, uint16_t timeout );

void  			os_q_creat( a_q_t**sem, void**pbuf, uint16_t size );

#define         os_q_pend               OSQPend

#define         os_q_post               OSQPost

uint8_t 		os_task_assign_stk_build( void ( *task )( void *p_arg ) , Bsize_t nblk );

uint8_t    		os_task_random_build( void ( *task )( void *p_arg ) );

BOOL 			os_task_assign_pro_build( void ( *task )( void *p_arg ), uint8_t pro, Bsize_t nblk );

uint8_t 		os_task_name_set( uint8_t pro, char*pname );

uint8_t 		os_task_name_get( uint8_t pro, char*pname );

uint8_t 		os_task_delete( uint8_t pro );

void  			os_task_delete_all( void );

extern          uint8_t			m_task_pro_array[MY_TASK_MAX_NUM+1] ;  /*任务随机分配时，标识当前任务索引*/
#define 		SELF_PRO		 OS_PRIO_SELF

void 			os_connect_init( void );
extern  		interrupt  void OSTickISR( void );
/*********************************************************************
*应用层无效数据定义
**********************************************************************/
#define        	INVALID_PRO      			190u	/*无效的任务优先级*/

#define 	    INVALID_DEV_ID              0u 		/*无效设备ID*/

#define        	INVALID_SEM                 0u		/*无效的信号*/

#define        	INVALID_LOCK               (INVALID_SEM)/*无效的锁*/

/*********************************************************************
*应用层函数宏替换
**********************************************************************/
#if TASK_MONITOR_EN>0
#define   	TASK_ADD_SELF_CNT    		thread_add_self_cnt
#define   	msleep(val)     		    thread_sleep_subsetp(val)
#else
#define 	TASK_ADD_SELF_CNT()
#define   	msleep(val)     		    OSTimeDly(val)
#endif

#define     OS_DELETE_REQ(PRO)\
	{\
		while ( os_task_delete_req( PRO )\
				!= OS_TASK_NOT_EXIST )\
			msleep( 1 );}

#define        OS_DELETE_REQ_SELF()\
	if ( os_task_delete_req( SELF_PRO ) == OS_TASK_DEL_REQ ){\
		os_task_delete( SELF_PRO );}



/*---------------------------------------------------------------------
    os_driver_maneger
---------------------------------------------------------------------*/	
typedef Bsize_t	( *os_driver_read_t )( uint8_t*buf, Bsize_t size, void*pos );
typedef Bsize_t	( *os_driver_write_t )( uint8_t*buf, Bsize_t size, void*pos );
typedef void	( *os_driver_ioctl_t )( uint8_t cmd, void*arg );
typedef void	( *os_driver_init_t )( void );
typedef void	( *os_driver_uninstall_t )( void );



typedef     	uint8_t  dev_id_t;
typedef struct
{
	os_driver_read_t      os_driver_read;
	os_driver_write_t     os_driver_write;
	os_driver_ioctl_t     os_driver_ioctl;
	os_driver_init_t      os_driver_init;
	os_driver_uninstall_t os_driver_uninstall;
}os_dirver_operate_t;

typedef struct
{
	dev_id_t id;
	os_driver_read_t      os_driver_read;
	os_driver_write_t     os_driver_write;
	os_driver_ioctl_t     os_driver_ioctl;
	os_driver_init_t      os_driver_init;
	os_driver_uninstall_t os_driver_uninstall;
#if DRIVER_NAM_SIZE >0
	char  os_driver_name[DRIVER_NAM_SIZE];
#endif
    BOOL   dev_STATE;
}os_dirver_member_t;

typedef struct
{
	Bsize_t real_size;
	os_dirver_member_t  os_driver_member[MAX_SURPPORT_DRIVER];
}os_dirver_maneger_t;


dev_id_t 	os_driver_add( os_dirver_operate_t *driver_operate, char*pname ); /*将驱动加入到管理列表*/

void 	    os_driver_delete( dev_id_t dev_id ); /*将驱动从管理列表中删除*/

dev_id_t 	os_driver_open(const char*name);     //打开一个设备获取到ID

void 		os_driver_close(dev_id_t id);        //关闭一个设备

void 		os_driver_init( dev_id_t dev_id );   /* 初始化已经注册的驱动*/

void 		os_driver_uninstall( dev_id_t id );  /*卸载一个驱动*/

Bsize_t 	os_driver_read( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos );/*从驱动读入数据*/

Bsize_t 	os_driver_write( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos );/*向驱动写入数据*/

void  		os_driver_ioctl( dev_id_t id, uint8_t cmd, void*arg ); /*驱动参数控制接口*/

void 		os_driver_maneger_tab_init( void );/*驱动管理列表初始化*/

#define   	DRIVER_LOCK_REVERT       0xff

typedef     void ( *driver_post_sem_t )( void );
/*---------------------------------------------------------------------
    os_mod_maneger
---------------------------------------------------------------------*/	
typedef void ( *app_mod_maneger_hook_t )( void*arg );

typedef void( *app_insmod_t )( void );

typedef void( *app_exitmod_t )( void );

#define  	INVALID_MOD_ID   	 0

#define 	MAX_HMSG_SIZE        3  /*模块消息缓存的最大容量*/
typedef struct
{
	Bsize_t 	  mod_id;
	char          name[MAX_MOD_NAME];
	app_insmod_t  insmod_fun;
	app_exitmod_t exitmod_fun;
}app_mod_inlet_t;

typedef struct
{
	Bsize_t real_mod_num;
	app_mod_inlet_t  mod_inlet_list[MAX_MOD_NUM];
}app_mod_maneger_t;

enum  				    /*模块管理类命令梅举*/
{
	APP_MOD_START = 0,   /*启动模块*/
	APP_MOD_EXIT,        /*退出模块*/
	APP_MOD_SWTICH,      /*模块切换*/
	APP_MOD_MANEGER_EXIT /*退出模块管理*/
};

enum       				 /*reboot TYPE*/
{
	APP_CMD_REBOOT = 0,  /*reboot 命令重启*/
	APP_OS_ERR_REBOOT,   /*系统错误重启*/
	APP_CONFIG_REBOOT,   /*CONFIG命令重启*/
	APP_MEM_ERR_REBOOT   /*CONFIG命令重启*/
};
typedef struct
{
	Bsize_t occupy_flag;
	Bsize_t msg_type;
	Bsize_t start_mod_id;
	Bsize_t stop_mod_id;
}app_mod_handle_msg_t;
void 		app_module_handle_hook( void );

Bsize_t 	app_module_add( app_mod_inlet_t*tp_ptr );     /*添加一个模块到管理列表，返回MODID*/

void 		app_module_delete( Bsize_t mod_id );		  /*从管理列表删除一个模块*/

void 		app_module_send_hmsg( Bsize_t msg_type, Bsize_t start_mod_id, Bsize_t stop_mod_id );/*发送模块状态更改消息*/

void 		app_module_state_handle( void*arg );   		  /*进入模块管理API*/

void 		app_module_switch_last_mod( void ); 		  /*切换到上一个模块*/

void 		app_module_assign_mod( Bsize_t start_mod_id );/*切换到指定模块*/

void 		app_module_assignExt(const char*name);       /*切换到指定模块*/

Bsize_t 	app_module_ID_obtain(const char*name);

void 		app_module_set_name(Bsize_t id,const char*name);

void 		app_module_maneger_exit( Bsize_t flag );	  /*强制退出模块管理*/

Bsize_t 	app_module_get_reboot_flag( void );			  /*获取模块退出状态标志*/

Bsize_t 	app_module_get_last_id( void );

Bsize_t 	app_module_pos_name_obtain( char*name );

void 		app_module_CURmod_OB( char*name );

#define   	APP_MOD_START_ID   1

#define  	MOD_SWITCH(start) 	   app_module_assign_mod(start)  /*模块切换宏转换*/

#define  	MOD_SWITCH_STARTMOD()  app_module_assignExt("START")

#define  	MOD_SWITCHEXT(name)    app_module_assignExt(name)

/***********************************************************************
    os_str
***********************************************************************/
void 		os_memcpy( void * pur_add, void*src_ptr, Bsize_t lenth );

void 		os_memset( void * pur_add, uint8_t pad_data, Bsize_t lenth );

BOOL      	os_memcmp( void * pur_add, void*src_ptr, Bsize_t lenth );

Bsize_t 	os_strlen( char*psrc );

void 		os_strcat( char*pstr1, char*pstr2 );

char* 		os_strcpy( char*pdest, char*psrc );

char* 		os_string_locate_first( char*psrc, char*psearch );

#define     OS_MEMCLR(psrc,lenth) 	(os_memset((void*)(psrc),0,(lenth)))

//EXT
typedef  struct
{
	Bsize_t(  * os_mod_add )( void );
	char*  name;
	uint32_t crc;
}mod_member_t;


typedef  struct
{
	dev_id_t(*driver_register )(void);
	uint32_t  crc;
}DR_member_t;

#endif
