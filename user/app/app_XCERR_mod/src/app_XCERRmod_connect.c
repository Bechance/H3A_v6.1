/********************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: test模块对外连接层
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_XCerrmod_connect.c
* 创建人 :揭成
* 功能描述:实现该模块的对外连接
*----------------------------------------------------------------*/
#include "app_XCerrmod_inside.h"
#include "../../../support\app_support.h"
uint16_t LCD_RGB_565(uint16_t R,uint16_t G,uint16_t B);
/********************************************************************
 * @创建人 :
 * @功能 :start模块资源初始化
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
#define      WIN_BG              	8452
#define      WIN_GR_BG              901

static void _RGB_view(int pos,int R,int G,int B)
{
	GUI_window_hwin_req(GET_GUIWIN);
	GUI_set_Fgcolor(0XFFFF);
	GUI_set_Bgcolor(LCD_RGB_565(R,G,B));  
	GUI_fill_Rect(100,100,200,100);
	GUI_draw_Rect(100,100,200,100);
	GUI_set_Bgcolor(0);
	GUI_int_display_At(100,50,pos);
	GUI_int_display_At(150,50,R);
	GUI_int_display_At(200,50,G);
	GUI_int_display_At(250,50,B);
	GUI_window_hwin_release(GET_GUIWIN);
    return;
}


void _GetRGB(void)
{
	uint8_t  key;
	uint8_t  RGB[3]={63,188,239};
	int      pos = 0;
	GUI_set_Bgcolor(0);    
	GUI_clean_screen();
	GUI_string_display_At(150,20,"R");
	GUI_string_display_At(200,20,"G");
	GUI_string_display_At(250,20,"B");
	_RGB_view(pos,RGB[0],RGB[1],RGB[2]);
    while(1)
    {
		msleep(1);
		if ( app_global_key_obtain( &key, 1 ) != 1 )
		{
			continue;
		}
		if(key==KEY_DOWN)
		{
		    if(RGB[pos]>0)RGB[pos]--;
		}
		else if(key==KEY_UP)
		{
		    if(RGB[pos]<255)RGB[pos]++;
		}
		else if(key==KEY_RIGHT)
		{
            pos = (pos==2)?(0):(pos+1);
		}
		else if(key==KEY_LEFT)
		{
            pos = (pos==0)?(2):(pos-1);
		}
		_RGB_view(pos,RGB[0],RGB[1],RGB[2]);

	}
}

void app_XCerrmod_resource_init( void )
{
	app_module_color_select(2);
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 : start模块加载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_XCerrmod_insmod( void )
{
	app_global_resouce_req( 0 );  	/*申请占用公共资源*/
	app_XCerrmod_resource_init();	/*初始化本模块资源*/
	app_XCerrmod_fun_entry();    	/*本模块主功能入口*/
	DBGMSG("XCerrmod....\n",LOG_NP,LOG_NP);
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 : start模块卸载入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_XCerrmod_exit( void )
{
	app_XCerrmod_fun_exit();					/*退出本模块主功能*/
	app_global_resouce_release( 0 );          	/*释放占用公共资源*/
	DBGMSG( "XCerrmod=>", LOG_NP, LOG_NP );
	return;
}
/********************************************************************
 * @创建人 :
 * @功能 : 把模块加到管理列表
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
Bsize_t app_XCerrmod_add( void )
{
	app_mod_inlet_t temp;
	Bsize_t mod_id;
	temp.insmod_fun = app_XCerrmod_insmod;/*模块入口*/
	temp.exitmod_fun = app_XCerrmod_exit;/*模块出口*/
	mod_id = app_module_add( &temp );
	return mod_id;
}
/*module AUTO Register*/
#pragma DATA_SECTION (app_XCerrmod_list,"MOD_LIST_ADDR")
const mod_member_t app_XCerrmod_list[]=
{
	app_XCerrmod_add,
	"START",
	0x5a5a
};
