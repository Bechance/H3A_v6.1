/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI光标
* 创建时间:2012-10-18
* 所属公司 :科荟
* 文件名称:GUI_libext.c
* 创建人 :揭成
* 功能描述:GUI外部字库管理，HZK16 ucdos标准
*----------------------------------------------------------------
修改记录:
修改日期:
修改人:
********************************************************************/
#include "../../Gui.h"
#include "../../../Fatfs\FatFs.h"

FIL *m_GUI_hzk16        = 0;
FIL *m_GUI_hzk24        = 0;
FIL *m_GUI_ack16        = 0;
FIL *m_GUI_fp           = 0;

void GUI_local_ASC_data_obtain( GUI_front_t*pfront, uint16_t ASCcode );


uint16_t RecNum_OB(uint16_t ASCcode)
{
	uint8_t   ch;
	uint16_t  QH_num;
	uint16_t  WH_num;
	uint16_t  RecNum;
	ch =  ASCcode >> 8;
	if ( ch >= 0xa1 )
	{
		QH_num = ( ch - 160 );
	}
	else QH_num = ch;

	ch = ASCcode & 0xff;

	if ( QH_num < 0x81 )
	{
		WH_num = ( ch - 160 );
		RecNum = ( QH_num - 1 ) * 94 + ( WH_num - 1 );
	}
	else
	{
		WH_num = ch;
		RecNum = (( QH_num << 8 ) ) | ( WH_num );
	}
    return RecNum;
}


uint16_t RecNum_OB1(uint16_t ASCcode)
{
	uint8_t   ch;
	uint16_t  QH_num;
	uint16_t  WH_num;
	uint16_t  RecNum;
	ch =  ASCcode >> 8;
	if ( ch >= 0xa1 )
	{
		QH_num = ( ch - 160 )-0xf;
	}
	else QH_num = ch;

	ch = ASCcode & 0xff;

	if ( QH_num < 0x81 )
	{
		WH_num = ( ch - 160 );
		RecNum = ( QH_num - 1 ) * 94 + ( WH_num - 1 );
	}
	else
	{
		WH_num = ch;
		RecNum = (( QH_num << 8 ) ) | ( WH_num );
	}
    return RecNum;
}



void   GUI_FIL_Rbyte( int32_t pos, char *buf, Bsize_t size )
{
	FIL* fp = m_GUI_fp;
	UINT     R;
	f_lseek( fp, pos );
	f_read( fp, buf, size, &R );
	return;
}


static void GUI_ASC_COM(void*fp,uint16_t ofp,uint8_t *buf,int rsize)
{
	m_GUI_fp = fp;
	if ( !m_GUI_fp ) return;
	GUI_FIL_Rbyte((int32_t)ofp*rsize,(char*)buf,rsize);
    return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能   :获取当前外部字符数据
 * @输入   :
 * @输出   :
********************************************************************/
static void GUI_ASC1_data_obtain( GUI_front_t*pfront, uint16_t ASCcode )
{
    if(GUI_conText.CUR_Front == &GUI_HZK16_EXTASCII)
    {
		pfront->wide   =  8;
		pfront->high   =  16;
		pfront->nbyte  =  1;
		pfront->CHwide =  8;
		GUI_ASC_COM( m_GUI_ack16,ASCcode,(uint8_t*)pfront->Dbuf,16);
    }
	else if(GUI_conText.CUR_Front == &GUI_HZK24_EXTASCII)
	{
		GUI_SetFront( &GUI_F16X24_ASCII );
		pfront->way = GUI_F16X24_ASCII.way;
		GUI_local_ASC_data_obtain( pfront, ASCcode );
		GUI_SetFront( &GUI_HZK24_EXTASCII );
	}
	return;
}


static void GUI_ASC2_data_obtain( GUI_front_t*pfront, uint16_t ASCcode )
{

    if(GUI_conText.CUR_Front == &GUI_HZK16_EXTASCII)
    {
		pfront->wide   =  16;
		pfront->high   =  16;
		pfront->nbyte  =  2;
		pfront->CHwide =  16;
		GUI_ASC_COM(m_GUI_hzk16,RecNum_OB(ASCcode),(uint8_t*)pfront->Dbuf,32);
    }
    else if(GUI_conText.CUR_Front == &GUI_HZK24_EXTASCII)
    {
		if (((ASCcode >> 8)-160) > 0X0F )
		{
			pfront->wide   =  24;
			pfront->high   =  24;
			pfront->nbyte  =  2;
			pfront->CHwide =  24;
			GUI_ASC_COM( m_GUI_hzk24,RecNum_OB1(ASCcode),(uint8_t*)pfront->Dbuf,72);
		}
		else
		{
			GUI_SetFront( &GUI_F16X24_ASCII );
			GUI_local_ASC_data_obtain( pfront, ASCcode );
			GUI_SetFront( &GUI_HZK24_EXTASCII );
		}
    }
	return;
}


static void HZK_obtain( GUI_front_t*pfront, uint16_t ASCcode, int size )
{
	if ( size == 1 )
	{
		pfront->way    =  GUI_conText.CUR_Front->way;
		GUI_ASC1_data_obtain( pfront, ASCcode );
	}
	else
	{
		pfront->way    =  GUI_conText.CUR_Front->way;
		GUI_ASC2_data_obtain( pfront, ASCcode );
	}
	return;
}


void GUI_extlib_init( void )
{
	if ( !m_GUI_hzk16 )
	{
		m_GUI_ack16 = fatfs_open( "Asc16", FA_OPEN_EXISTING | FA_READ );
		m_GUI_hzk24 = fatfs_open( "HZK24", FA_OPEN_EXISTING | FA_READ );
		m_GUI_hzk16 = fatfs_open( "hzk16", FA_OPEN_EXISTING | FA_READ );
	}
	return;
}

const GUI_FONT GUI_HZK16_EXTASCII =
{
	"!"
	, "~"
	, 2
	, 16                       /* wide*/
	, 16                      /* high*/
	, 8
	, 'H'
	, HZK_obtain
	, 0
	, ( const GUI_FONT* )0
};


const GUI_FONT GUI_HZK24_EXTASCII =
{
	"!"
	, "~"
	, 2
	, 24                       /* wide*/
	, 24                       /* high*/
	, 12
	, 'V'
	, HZK_obtain
	, 0
	, ( const GUI_FONT* )0
};
