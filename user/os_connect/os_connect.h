#ifndef _2010_PEC_OS_CONNECT_H
#define _2010_PEC_OS_CONNECT_H
#include "ostype.h"
#include "../../kernel\includes\ucos_ii.h"

#ifndef  OS_DBGERR
#define  OS_DBGERR         	DBGERR
#endif
#define  DLOG_NP            
/*---------------------------------------------------------------------
    os_connect ����
---------------------------------------------------------------------*/
#define 		MAX_SURPPORT_DRIVER 		25  /*��֧����������*/

#define 		DRIVER_NAM_SIZE				20  /*�������Ƴ���*/

#define  		MAX_MOD_NUM    	     		18  /*��֧��ģ�����*/

#define     	MAX_MOD_NAME         		20  /*ģ�����Ƴ���*/

#define 		MY_TASK_MAX_NUM   			47  /*USER��������7552/ADVISE_STK_SIZE*/

#define 		TYPE_DELAY  		       	3u  /*��������˯�߽��� 30MS */

#define 		APP_HALF_SEC  				50u /*0.5S����*/

#define 		APP_100_MS  				10u /*100ms����*/

#define   		ADVISE_STK_SIZE    			150u/*��������ʹ�õĶ�ջ*/

#define   		TYPE_TIME_OUT     			50u /* ����ȴ��ź�ʱ��500ms(���ʱ��)*/

/*������������*/

typedef              OS_EVENT 	a_lock_t;
typedef              OS_EVENT 	a_sem_t;
typedef              OS_EVENT 	a_q_t;

enum
{
	PRO_IS_IDLE 		= 'F',      /*��������*/
	PRO_IS_MAIN_OCCUPY  = 'M',   	/*����ռ��*/
	PRO_IS_SLAVE_OCCUPY = 'S'  		/*��������ռ��*/
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
/*ϵͳ�ӿں���*/
void 			os_lock( a_lock_t*sem );     /*��Ҫ��������Դ����*/

void 			os_unlock( a_lock_t*sem );  /*��Ҫ��������Դ����*/

void 			os_lock_revert( a_lock_t*sem );/*�����ָ�����ʼ״̬*/

uint8_t 		os_lock_creat( a_lock_t**sem );/*����һ������Դ*/

uint8_t 		os_sem_creat( a_sem_t**sem, uint16_t cnt );/*����һ���ź���*/

uint8_t 		os_sem_revert( a_sem_t*sem, uint16_t cnt );/*���ź����ָ�����ʼ״̬*/

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

extern          uint8_t			m_task_pro_array[MY_TASK_MAX_NUM+1] ;  /*�����������ʱ����ʶ��ǰ��������*/
#define 		SELF_PRO		 OS_PRIO_SELF

void 			os_connect_init( void );
extern  		interrupt  void OSTickISR( void );
/*********************************************************************
*Ӧ�ò���Ч���ݶ���
**********************************************************************/
#define        	INVALID_PRO      			190u	/*��Ч���������ȼ�*/

#define 	    INVALID_DEV_ID              0u 		/*��Ч�豸ID*/

#define        	INVALID_SEM                 0u		/*��Ч���ź�*/

#define        	INVALID_LOCK               (INVALID_SEM)/*��Ч����*/

/*********************************************************************
*Ӧ�ò㺯�����滻
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


dev_id_t 	os_driver_add( os_dirver_operate_t *driver_operate, char*pname ); /*���������뵽�����б�*/

void 	    os_driver_delete( dev_id_t dev_id ); /*�������ӹ����б���ɾ��*/

dev_id_t 	os_driver_open(const char*name);     //��һ���豸��ȡ��ID

void 		os_driver_close(dev_id_t id);        //�ر�һ���豸

void 		os_driver_init( dev_id_t dev_id );   /* ��ʼ���Ѿ�ע�������*/

void 		os_driver_uninstall( dev_id_t id );  /*ж��һ������*/

Bsize_t 	os_driver_read( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos );/*��������������*/

Bsize_t 	os_driver_write( dev_id_t id, uint8_t*buf, Bsize_t size, void*pos );/*������д������*/

void  		os_driver_ioctl( dev_id_t id, uint8_t cmd, void*arg ); /*�����������ƽӿ�*/

void 		os_driver_maneger_tab_init( void );/*���������б��ʼ��*/

#define   	DRIVER_LOCK_REVERT       0xff

typedef     void ( *driver_post_sem_t )( void );
/*---------------------------------------------------------------------
    os_mod_maneger
---------------------------------------------------------------------*/	
typedef void ( *app_mod_maneger_hook_t )( void*arg );

typedef void( *app_insmod_t )( void );

typedef void( *app_exitmod_t )( void );

#define  	INVALID_MOD_ID   	 0

#define 	MAX_HMSG_SIZE        3  /*ģ����Ϣ������������*/
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

enum  				    /*ģ�����������÷��*/
{
	APP_MOD_START = 0,   /*����ģ��*/
	APP_MOD_EXIT,        /*�˳�ģ��*/
	APP_MOD_SWTICH,      /*ģ���л�*/
	APP_MOD_MANEGER_EXIT /*�˳�ģ�����*/
};

enum       				 /*reboot TYPE*/
{
	APP_CMD_REBOOT = 0,  /*reboot ��������*/
	APP_OS_ERR_REBOOT,   /*ϵͳ��������*/
	APP_CONFIG_REBOOT,   /*CONFIG��������*/
	APP_MEM_ERR_REBOOT   /*CONFIG��������*/
};
typedef struct
{
	Bsize_t occupy_flag;
	Bsize_t msg_type;
	Bsize_t start_mod_id;
	Bsize_t stop_mod_id;
}app_mod_handle_msg_t;
void 		app_module_handle_hook( void );

Bsize_t 	app_module_add( app_mod_inlet_t*tp_ptr );     /*���һ��ģ�鵽�����б�����MODID*/

void 		app_module_delete( Bsize_t mod_id );		  /*�ӹ����б�ɾ��һ��ģ��*/

void 		app_module_send_hmsg( Bsize_t msg_type, Bsize_t start_mod_id, Bsize_t stop_mod_id );/*����ģ��״̬������Ϣ*/

void 		app_module_state_handle( void*arg );   		  /*����ģ�����API*/

void 		app_module_switch_last_mod( void ); 		  /*�л�����һ��ģ��*/

void 		app_module_assign_mod( Bsize_t start_mod_id );/*�л���ָ��ģ��*/

void 		app_module_assignExt(const char*name);       /*�л���ָ��ģ��*/

Bsize_t 	app_module_ID_obtain(const char*name);

void 		app_module_set_name(Bsize_t id,const char*name);

void 		app_module_maneger_exit( Bsize_t flag );	  /*ǿ���˳�ģ�����*/

Bsize_t 	app_module_get_reboot_flag( void );			  /*��ȡģ���˳�״̬��־*/

Bsize_t 	app_module_get_last_id( void );

Bsize_t 	app_module_pos_name_obtain( char*name );

void 		app_module_CURmod_OB( char*name );

#define   	APP_MOD_START_ID   1

#define  	MOD_SWITCH(start) 	   app_module_assign_mod(start)  /*ģ���л���ת��*/

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
