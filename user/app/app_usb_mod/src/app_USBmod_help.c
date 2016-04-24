/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:USB 模块 校验计划
* 创建时间:
* 所属公司 :科荟
* 文件名称:app_USERmod_help.c
* 创建人 :
* 功能描述:实现help文件下载
******************************************************************************************/
#include "app_USBmod_inside.h"



#define      HELP_GAP                   "$PEC:"

static Bsize_t app_help_name_obtain(char*name,char*picon)
{
   char*pend;
   char ch_temp[25]="";
   global_memcpy(ch_temp,picon,24);
   pend=global_string_locate_first(ch_temp,"#");
   *pend = ASCII_NULL;
   global_strcpy(name,(ch_temp + strlen(HELP_GAP)));
   return (Bsize_t)(pend-ch_temp);
}


void app_help_file_paging(void)
{
	uint32_t poffset[30];
	char file_name[20] = "";
	char name[12] = "";
	char*pcon;
	Bsize_t  size;
	Bsize_t  offset;
	Bsize_t  base_size;
	Bsize_t  fleek;
	FIL *pfilenew = app_malloc(sizeof(FIL));
	FIL *pfileold = app_malloc(sizeof(FIL));
	FRESULT ret;
    UINT w;
	ret = ret;
	size = fatfs_file_locate_icon(
			   HELP_GAP,
			   "HELP.TXT",
			   poffset,29,0);
	ret=f_open(pfileold,"HELP.TXT",FA_OPEN_EXISTING|FA_READ);
	f_mkdir("HELP");
	for(offset=0;offset<size;offset++)
	{
		GLOBAL_MEMCLR(file_name,20);
		GLOBAL_MEMCLR(name,20);
		base_size = poffset[offset+1]-poffset[offset]+1;
		pcon = app_malloc(base_size);
		GLOBAL_MEMCLR(pcon,base_size);
  		f_read(pfileold,pcon,(base_size-1),&w);
		fleek=app_help_name_obtain(name,pcon);
		fleek+=poffset[offset]+1;
		f_lseek(pfileold,fleek);
		GLOBAL_MEMCLR(pcon,base_size);
		base_size = poffset[offset+1]-fleek;
  		f_read(pfileold,pcon,base_size,&w);
		global_strcpy(file_name,"HELP/");
		global_strcat(file_name,name);	
		ret=f_open(pfilenew,file_name,FA_CREATE_ALWAYS | FA_WRITE);
        f_write(pfilenew,pcon,base_size,&w);
        f_close(pfilenew);
		app_free(pcon);
	}
	f_close(pfileold);
	app_free( pfilenew);
	app_free( pfileold);
	return;
}
/********************************************************************
 * @创建人: 揭成
 * @功能:在文件中寻找一段字符串
 *
 * @输入:NONE
 * @输出:NONE
********************************************************************/
Bsize_t fatfs_file_locate_icon( char*icon,char*file_name,
									  uint32_t*paddr,
									  Bsize_t addr_size,
									  void(*dis)(Bsize_t num))
{
	FIL   *file;
	Bsize_t icon_size = global_strlen( icon );
	Bsize_t base_size =100 * icon_size;
	Bsize_t num = 0;
	Bsize_t offset;
	Bsize_t ret = 0;
	Bsize_t num_bak;
	uint32_t picon[30];
    int      foffset=0;
	float   fdata;
	UINT    r;
	char*  ptemp=app_malloc(base_size+2);
	file = fatfs_FIobj_obtain();
    if(!file) return 0;	
    if(!ptemp) return 0;	
	if(f_open(file,file_name,
		FA_OPEN_EXISTING | FA_READ)
		!=FR_OK) return 0;
    while(1)
    {
		GLOBAL_MEMCLR( ptemp, base_size );
		f_read(file,ptemp,base_size,&r);
        if(r<icon_size)break;	
		num = global_string_locate( ptemp, ptemp + base_size, icon, picon );
		offset = 0;
		num_bak = num;
		while ( num > 0 )
		{
			if ((paddr)&&(ret<(addr_size-1)))
			{
				paddr[ret] = ( picon[offset] - ( uint32_t )ptemp ) + foffset;
			}
			num--;
			ret++;
			offset++;
		}
		if ( num_bak > 0 )
		{
			foffset += ( picon[num_bak-1] - ( uint32_t )ptemp + icon_size );
		}
		else
		{
			foffset += ( base_size - icon_size );
		}
		f_lseek(file,foffset);
		fdata = foffset;
		fdata = fdata/file->fsize;
		fdata*=100;
		if(dis)dis((Bsize_t)fdata);
	}
	f_close(file);
	app_free(ptemp);
	fatfs_FIobj_put(file);
	return ret;
}
