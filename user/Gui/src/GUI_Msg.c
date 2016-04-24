/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:GUI消息处理
* 创建时间:2012-07-22
* 所属公司:科荟
* 文件名称:GUI_MSG.c
* 创建人 :揭成
* 功能描述:GUI消息处理
*----------------------------------------------------------------*/
#include "../Gui.h"

static GUI_MsgTCB_t * GUI_msg_init( Bsize_t buffer_size )
{
	GUI_MsgTCB_t *  	result = 0;
	Bsize_t 		total_struct_size = sizeof( GUI_MsgTCB_t ) + buffer_size - 1;
	result = ( GUI_MsgTCB_t * ) GUI_malloc( total_struct_size );
	if ( result )
	{
		GUI_memset(( uint8_t* )result, 0, total_struct_size );
		result->buffer_size = buffer_size;
	}
	return result;
}


static void GUI_msg_destroy( GUI_MsgTCB_t * * self_ptr )
{
	if (( NULL == self_ptr ) || ( NULL == *self_ptr ) )
	{
		return ;
	}
	GUI_free( *self_ptr );
	*self_ptr = NULL;
	return;
}


static Bsize_t  GUI_msg_shift( GUI_MsgTCB_t * self, Bsize_t pos )
{
	if (( pos + 1 ) == self->buffer_size )
	{
		return ( 0 );
	}
	else
	{
		return ( pos + 1 );
	}
}

static void GUI_msg_push( GUI_MsgTCB_t * self, unsigned char*data, Bsize_t size )
{
	uint16_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		self->buffer_ptr[self->end_pos ] = data[cur_size];
		self->end_pos  = GUI_msg_shift( self, self->end_pos );

		if ( self->end_pos == self->start_pos )
		{
			self->start_pos  = GUI_msg_shift( self, self->start_pos );
		}
	}
	return;
}


static void GUI_msg_pop( GUI_MsgTCB_t * self, uint8_t*data, Bsize_t size )
{
	uint16_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		data[cur_size] = self->buffer_ptr[self->start_pos];
		self->start_pos = GUI_msg_shift( self, self->start_pos );
	}
	return ;
}


static Bsize_t GUI_msg_get_size( GUI_MsgTCB_t * self )
{
	if ( self->end_pos >= self->start_pos )
	{
		return self->end_pos - self->start_pos;
	}
	else
	{
		return self->buffer_size - ( self->start_pos - self->end_pos );
	}
}

static void GUI_msg_empty( GUI_MsgTCB_t * self )
{
	self->start_pos = 0;
	self->end_pos = 0;
	return;
}

Bsize_t GUI_MsgFIFO_empty( hwin_t*hp )
{
	GUI_msg_empty( hp->pMSG );
	return 0;
}

Bsize_t GUI_MsgFIFO_write( hwin_t*hp, uint8_t*data, Bsize_t size )
{
	GUI_msg_push( hp->pMSG, data, size );
	return size;
}

Bsize_t GUI_MsgFIFO_read( hwin_t*hp, uint8_t*data, Bsize_t size )
{
	Bsize_t real_size;
	real_size = GUI_msg_get_size( hp->pMSG );
	if ( real_size == 0 )
		return 0;
	if ( real_size >= size )
	{
		real_size = size;
	}
	GUI_msg_pop( hp->pMSG, data, real_size );
	return real_size;
}

BOOL  GUI_MsgFIFO_del( hwin_t*hp )
{
	GUI_msg_destroy( &( hp->pMSG ) );
	return TURE;
}

void  GUI_WinID_post( hwin_t*hp, uint8_t id )
{
	Bsize_t depth;
	hwin_t* ptemp;
	hwin_t* pFWIN;
	if ( hp )
	{
		if ( hp->pMSG )
		{
			GUI_MsgFIFO_write( hp, &id, 1 );
			GUI_SetEXTkey( id );
			return;
		}
		if (( hp->flag&WIN_MSG_EN ) )
		{
			depth = hp->win_depth;
			ptemp = hp;
			while ( depth != 0 )
			{
				pFWIN = ptemp->father_hwin;
				ptemp = pFWIN;
				depth --;
				if ( pFWIN->pMSG )
				{
					GUI_MsgFIFO_write( pFWIN, &id, 1 );
					GUI_SetEXTkey( id );
					return;
				}
			}
		}
	}
	GUI_dummy_press_Ekey( id );
	return;
}



BOOL  GUI_MsgFIFO_create( hwin_t*hp, Bsize_t size )
{
	if ( hp->pMSG ) return FAIL;
	hp->pMSG = GUI_msg_init( size );
	return TURE;
}

void GUI_keyMSG_post( hwin_t*hp, uint8_t key)
{
	 GUI_MsgFIFO_write( hp,&key,1);
	 return;
}

Bsize_t GUI_key_read( hwin_t*hp, uint8_t*data, Bsize_t size )
{
	return GUI_MsgFIFO_read( hp, data, size );
}

//2013 11-22 by jiecheng
void GUI_mbox_post( hwin_t*hp, uint8_t key)
{
	 GUI_MsgFIFO_write( hp,&key,1);
	 return;
}

Bsize_t GUI_mbox_read( hwin_t*hp, uint8_t*data, Bsize_t size )
{
	return GUI_MsgFIFO_read( hp, data, size );
}

