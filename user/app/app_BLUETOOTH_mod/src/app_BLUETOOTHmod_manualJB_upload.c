
#include "app_BLUETOOTHmod_manualJB_upload.h"


const char* BTmod_meterDJ_opt[]={
		"1��","2��","3��","0.5��", "0.5S��","0.2��","0.2S��"};


/* �ֶ�У���¼�ϴ���Ϣǰ׺*/
const char *BTmod_manualJB_prefix_tbl[] =
{
	"WORKID=",	"UID=", 	"UNAME=",	"BH=",		"WORKER=",	
	"TIME=",	"ORDER=",	"DLSR=",	"BB=",		"CS=",		
	"QS=",		"WAY=",		"PQOPT=",	"LOAD=",	"DJ=",
	"TEMP=",	"HNM=",		"UA=",		"UB=",		"UC=",		
	"IA=",		"IB=",		"IC=",		"JA=",		"JB=",		
	"JC=",		"PA=",		"PB=",		"PC=",		"QA=",		
	"QB=",		"QC=",		"JIAC=",	"JIBC=",	"JIAB=",	
	"JUAC=",	"JUBC=",	"JUAB=",	"COS=",		"PABC=",	
	"QABC=",	"ERR1=",	"ERR2=",	"ERR3=",	"ERR4=",	
	"FEQ=",		"COSLC=",	
};



const char* BTmod_manual_JB_not_defined_item_prefix_tbl[]=
{
	"ERR=",		"JL=",		"MODEL=",	"USLAVER=",		"UAB=",		
	"UBC=",		"UAC=",
};
/***********************************************
* �洢ǰ׺�Ե�Ӧ��Ա�ڽṹ���е����
* ˳�����ϴ��ı�manual_check_prefix_tbl����һ��
************************************************/
const int8_t BTmod_manualJB_member_offset_index_tbl[][2]=
{
	{0,0}, {0,1}, {0,2}, {0,3}, {0,5},
	{0,6}, {0,8}, {1,0}, {1,1}, {1,3},
	{1,6}, {1,9}, {1,10},{1,12},{1,13},
	{0,11},{0,12},{2,0}, {2,1},{2,2},
	{2,0}, {2,1}, {2,2}, {2,0}, {2,1},
	{2,2}, {2,0}, {2,1}, {2,2}, {2,0},
	{2,1}, {2,2}, {2,3}, {2,4}, {2,5},
	{2,6}, {2,7}, {2,8}, {2,14},{2,9},
	{2,10},{2,11},{2,11},{2,11},{2,11},
	{2,13},{2,15},
};


/********************************************************************
* @������ :������
* @���� : ��ȡ�Զ�У��洢�ṹһ����Աƫ�Ƶ�ַ
* @���� : ƫ�Ƶ�ַ���棬��Ա������
* @��� : 
********************************************************************/
static void app_BLUETOOTHmod_get_manualJB_top_member_offset
(
	uint16_t (*pos)[UARTMOD_MAX_MEMBERS],
	uint8_t index
) 
{ 
	uint16_t count = 0;
	pos[index][count++] = MEMBER_OFFSET(Errsave_t,DL_app_user_info);
	pos[index][count++] = MEMBER_OFFSET(Errsave_t,DL_app_user_set); 
	pos[index][count++] = MEMBER_OFFSET(Errsave_t,DLarg); 
}

/********************************************************************
* @������ :������
* @���� : ��ȡ�Զ�У��洢�ṹ������Ա1ƫ�Ƶ�ַ
* @���� : ƫ�Ƶ�ַ���棬��Ա������
* @��� : 
********************************************************************/
static void app_BLUETOOTHmod_get_manualJB_member1_offset(uint16_t (*pos)[UARTMOD_MAX_MEMBERS], uint8_t index) 
{ 
	uint16_t count = 0;
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,wrok_list);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,user_id);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,user_name);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,meter_id1);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,meter_id2);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,worker);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,time_buf);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,TQ_id);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,data_order);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,data_ext);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,meter_dj); 
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,temp_buf);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_info_t,hnm_buf);
	return;
}
/********************************************************************
* @������ :������
* @���� : ��ȡ�Զ�У��洢�ṹ������Ա2ƫ�Ƶ�ַ
* @���� : ƫ�Ƶ�ַ���棬��Ա������
* @��� : 
********************************************************************/
static void app_BLUETOOTHmod_get_manualJB_member2_offset(uint16_t (*pos)[UARTMOD_MAX_MEMBERS], uint8_t index) 
{	
	uint16_t count = 0; 
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,dlsr);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,bb1);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,bb2);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,cs);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,cs2);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,TMRcs);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,qs);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,qs2);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,qs3); 
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,handOR_GD);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,PorQ);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,meter_1OR3);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,ERRtype);
	pos[index][count++] = MEMBER_OFFSET(DL_app_user_set_t,meterDJ);
	return;
}
/********************************************************************
* @������ :������
* @���� : ��ȡ�Զ�У��洢�ṹ������Ա3ƫ�Ƶ�ַ
* @���� : ƫ�Ƶ�ַ���棬��Ա������
* @��� : 
********************************************************************/
static void app_BLUETOOTHmod_get_manualJB_member3_offset(uint16_t (*pos)[UARTMOD_MAX_MEMBERS], uint8_t index) 
{ 
	int16_t count = 0;
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,phase_a); 
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,phase_b);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,phase_c); 
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JIac);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JIbc);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JIab);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JUac);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JUbc);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_JUab);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_Pabc);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_Qabc);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_err_1);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_err_2);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_Feq);
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,c_Cos );
	pos[index][count++] = MEMBER_OFFSET(DL_app_param_str_t,cos_LC);
	return;
}



/********************************************************************
* @������ :������
* @���� : ��ȡ�Զ�У��洢�ṹ���г�Ա���ƫ�Ƶ�ַ
* @���� : ƫ�Ƶ�ַ����
* @��� : 
********************************************************************/
static void app_BLUETOOTHmod_initialize_pos_array(uint16_t (*pos)[UARTMOD_MAX_MEMBERS])
{
	uint8_t index = 0;
	app_BLUETOOTHmod_get_manualJB_top_member_offset(pos,index++);
	app_BLUETOOTHmod_get_manualJB_member1_offset(pos,index++);
	app_BLUETOOTHmod_get_manualJB_member2_offset(pos,index++);
	app_BLUETOOTHmod_get_manualJB_member3_offset(pos,index++);
	return;
}


/********************************************************************
* @������ :������
* @���� : ����洢����δ�ϴ���У���¼����
* @���� : ָ���ֶ�У���¼�ļ���ָ�룬�洢����������־λ
* @��� : ��
********************************************************************/
static void app_BLUETOOTHmod_cal_manualJB_total_items(FIL *filp, char *psrc_start,BOOL flag)
{
	uint16_t	count = 0;
	Errsave_t	*p_Errsave_t = PNULL;
	uint16_t	file_rw_size = 0;
	
	f_lseek(filp, 0);
	BT_total_JB_items_uploaded = 0;
	GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	for(count = 0; count <BT_total_record_counts; count++)	
	{
		f_read(filp,psrc_start,sizeof(Errsave_t),&file_rw_size);
		p_Errsave_t = (Errsave_t *)psrc_start;
		if(flag == UPLOAD_NEW_RECORD)
		{
			//�ж��Ƿ��Ѿ��ϴ�
			if((p_Errsave_t->DL_app_user_info.data_ext[0] != UPDATED_FALG)||(p_Errsave_t->DL_app_user_info.data_ext[1] != UPDATED_FALG))
			{
				if(p_Errsave_t->DL_app_user_set.meter_1OR3 == SINGEL_METER)
				{
					BT_total_JB_items_uploaded += 1;
				}
				else
				{
					BT_total_JB_items_uploaded += 2;
				}
			}
		}
		else
		{
			if(p_Errsave_t->DL_app_user_set.meter_1OR3 == SINGEL_METER)
			{
				BT_total_JB_items_uploaded += 1;
			}
			else
			{
				BT_total_JB_items_uploaded += 2;
			}
		}
		GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	}	
	GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
}

/********************************************************************
* @������ :������
* @���� : ��ǰ׺�������ҳ�Ա�ڴ洢�ṹ���е�ƫ����������ƫ����ֵ
* @���� : ƫ�����洢��ַ������
* @��� : 
********************************************************************/
static uint16_t app_BLUETOOTHmod_manualJB_cal_offset(
	uint16_t offset_array[][UARTMOD_MAX_MEMBERS],
	const uint8_t index
)
{
	uint16_t	member_base_offset = 0;
	uint16_t	member_insied_offset = 0;
	uint16_t	member_total_offset = 0;
	uint16_t	phase_param_t_member_offset = 0;
	uint8_t		member_num  =0;
	uint8_t		inside_member_num = 0;
	uint16_t	error_member_offset = 0;
	
	//1���ṹ��Աƫ��
	member_num 			= BTmod_manualJB_member_offset_index_tbl[index][0];
	member_base_offset 	= offset_array[0][member_num];
	//2���ṹ��Աƫ��
	//BH,BB,CS,QS,ERR1,ERR2,ERR3,ERR4��2����
	if((index == 3)  || (index == 8) || (index ==9)  || (index == 10) ||	\
		(index == 41) || (index == 42)|| (index == 43)|| (index == 44))
	{
		inside_member_num 	= BTmod_manualJB_member_offset_index_tbl[index][1] + BT_current_meter_number -1;
		member_insied_offset = offset_array[member_num + 1][inside_member_num];
	}
	else
	{
		inside_member_num 	= BTmod_manualJB_member_offset_index_tbl[index][1];
		member_insied_offset = offset_array[member_num + 1][inside_member_num];
	}
	//3���ṹ��Աƫ��
	if((index == 20) || (index == 21) ||(index == 22))			//ia,ib,ic,
	{
		phase_param_t_member_offset = 1*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else if((index == 23) || (index == 24) ||(index == 25))	//ja,jb,jc
	{
		phase_param_t_member_offset = 2*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else if((index == 26) || (index == 27) ||(index == 28))	//pa,pb,pc
	{
		phase_param_t_member_offset = 3*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else if((index == 29) || (index == 30) ||(index == 31))	//qa,qb,qc
	{
		phase_param_t_member_offset = 4*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else
	{
		phase_param_t_member_offset = 0;
	}
	if(index == 42)				//err2
	{
		error_member_offset = 1*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else if(index == 43)		//err3
	{
		error_member_offset = 2*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else if(index == 44)		//err4
	{
		error_member_offset = 3*sizeof(char)*UARTmod_DL_DSIZE;
	}
	else
	{
		error_member_offset = 0;//err1
	}
	member_total_offset = 	member_base_offset + member_insied_offset + 
		phase_param_t_member_offset +error_member_offset ;
	return member_total_offset;
}
/********************************************************************
* @������ :������
* @���� : ��ͨѶЭ�鴴���Զ�У���ϴ�֡���ݶ�
* @���� : ����֡�洢������,��¼�洢��������ƫ�Ƶ�ַ������
* @��� : 
********************************************************************/	
static uint16_t app_BLUETOOTHmod_create_manualJB_data_sector(
	char *pdes_start, 
	char *psrc_start,
	uint16_t offset_array[][UARTMOD_MAX_MEMBERS]
)
{
	uint16_t 	total_member_offset = 0;
	uint16_t	index = 0;
	uint16_t 	size  = 0;
	char		char_temp = 0;
	
	char *psrc_temp = psrc_start;
	char *pdes_temp = pdes_start;

	for(index = 0; index < ARRAY_SIZE(BTmod_manualJB_prefix_tbl);index++ )
	{
		//����ǰ׺
		global_strcat(pdes_temp, (char *)BTmod_manualJB_prefix_tbl[index]);
		pdes_temp = pdes_start + global_strlen(pdes_start);
		//���㱾�ֶ�ƫ��
		total_member_offset = 	\
			app_BLUETOOTHmod_manualJB_cal_offset(offset_array,index);
		psrc_temp = psrc_start + total_member_offset;	
		//�ַ�����Ϣ����
		if( (index == 7)||(index == 11) ||(index == 12) ||(index == 13)	||	\
		    (index == 14) ||(index == 46))
		{
			char_temp = *psrc_temp;
			if(index == 46)			//coslc�洢��ΪΪ�ַ�'L'��'C'
			{
				global_memcpy(pdes_temp,&char_temp,sizeof(char));
			}
			else if(index == 14)	//���ȼ�
			{
				global_strcat(pdes_temp,(char*)BTmod_meterDJ_opt[char_temp]);
			}
			else 
			{
				global_itoa(char_temp,pdes_temp);
			}
				
		}
		else
		{
			if(index == 6)					//��order��Ϊ���
			{
				global_itoa(BT_current_meter_number,pdes_temp);
			}
			else
			{
				global_strcat(pdes_temp,psrc_temp);
			}
		}
		global_strcat(pdes_temp, ",");		//����','
		pdes_temp = pdes_start + global_strlen(pdes_start);
	}
	//δ�����ֶΣ����
	for(index = 0; index <ARRAY_SIZE(BTmod_manual_JB_not_defined_item_prefix_tbl);index++)
	{
		global_strcat(pdes_temp, (char *)BTmod_manual_JB_not_defined_item_prefix_tbl[index]);
		//global_strcat(pdes_start,"");
		global_strcat(pdes_temp, ",");		//����','
	}
	
	size = global_strlen(pdes_start);
	return size;
}


/********************************************************************
* @������ :������
* @���� : ��ͨѶЭ�鴴���Զ�У���ϴ�֡���ݶ�,�������ݳ���
* @���� : ����֡�洢������,��¼�洢��������ƫ�Ƶ�ַ������
* @��� : 
********************************************************************/
static uint16_t app_BLUETOOTHmod_create_manualJB_frame(char *pdes_start,char *psrc_start,uint8_t *m_recv_buf ) 
{ 
	uint16_t 	total_send_size  = 0;
	uint16_t	total_data_len  = 0;
	uint8_t		m_mod256_sum 	= 0;
	char		*pdes_temp  	= pdes_start;

	uint16_t 	offset_array[5][UARTMOD_MAX_MEMBERS] = {{0,0}};
	
	app_BLUETOOTHmod_initialize_pos_array(offset_array);			//���Ա��ַƫ����

	m_recv_buf[8] |= 0x80;									//��ΪHAD����Ӧ��
	//��������bit5��1��ʾ�޺���֡
	if(BT_current_JB_item_num == BT_total_JB_items_uploaded)
	{
		m_recv_buf[CTL_CODE_POS] |= 0x20;
	}
	global_memcpy(pdes_temp, m_recv_buf,FRAME_HEAD_LEN);	//����֡ͷ
	pdes_temp = pdes_start + FRAME_HEAD_LEN;

	global_strcat(pdes_temp,"RECORD=");
	pdes_temp += global_strlen("RECORD=");
	global_itoa(BT_current_JB_item_num,pdes_temp);			//��ǰ�ϴ�������
	global_strcat(pdes_temp,"/");
	pdes_temp = pdes_start + global_strlen(pdes_start+FRAME_HEAD_LEN)+FRAME_HEAD_LEN;
	global_itoa(BT_total_JB_items_uploaded,pdes_temp);		//���ϴ���������
	global_strcat(pdes_temp,",");
	//ע: ���ɴӻ������׵�ַͳ�Ƴ��ȣ���Ϊ֡ͷ�������п��ܰ���0 
	pdes_temp = pdes_start + global_strlen(pdes_start + FRAME_HEAD_LEN) + FRAME_HEAD_LEN;
	app_BLUETOOTHmod_create_manualJB_data_sector(pdes_temp,psrc_start,offset_array);
	
	pdes_temp = pdes_start + global_strlen(pdes_start+FRAME_HEAD_LEN)+FRAME_HEAD_LEN;
	
	//���ӽ�����";\n"
	global_strcat(pdes_temp,";\n");
	pdes_temp += global_strlen(";\n");
	
	total_data_len = global_strlen(pdes_start+FRAME_HEAD_LEN) + 2;	//�������ݱ�ʶ��2���ֽ�
	pdes_start[DATA_LEN_HIGH_POS] = (total_data_len & 0xff00) >> 8;		//�������ݳ���
	pdes_start[DATA_LEN_LOW_POS] =  total_data_len  & 0xff;
	
	total_send_size =  global_strlen(pdes_start+FRAME_HEAD_LEN)+FRAME_HEAD_LEN;
	m_mod256_sum = app_BLUETOOTHmod_cal_mod256_sum((uint8_t *)pdes_start, total_send_size);
	pdes_start[total_send_size++]	= m_mod256_sum;
	pdes_start[total_send_size++]	= 0x16;
	return total_send_size;
} 




/********************************************************************
* @������ :������
* @���� : �ϴ������ֶ�У���¼
* @���� : ���ڽ��ջ������ַ
* @��� : 
********************************************************************/
void app_BLUETOOTHmod_manualJB_upload_new(uint8_t *m_recv_buf)
{
	uint16_t 	file_rw_size  	= 0;
	uint16_t 	total_send_size = 0;
	DWORD		total_file_size = 0;
	uint16_t    count 	 		= 0;
	DWORD		file_pos_temp	= 0;
	uint8_t		meter_id_num 	= 0;
	uint16_t	new_record_sented = 0;
	
	char		*pdes_start  = PNULL;
	char		*psrc_start  = PNULL;
	FIL			*pfile = PNULL;
	Errsave_t	*p_Errsave_t = PNULL;
	
	
	pdes_start =	(char *)app_malloc(sizeof(Errsave_t)*2);
	psrc_start =  (char *)app_malloc( sizeof(Errsave_t));
	if((psrc_start == PNULL) || (pdes_start == PNULL))
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_NO_MEM);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}
	GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
	GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	if(f_open(pfile, MANUALJB_SAVED_FILE_NAME,FA_READ|FA_WRITE|FA_OPEN_EXISTING)!=FR_OK)
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_MANUALJB_FR_OPEN);
		f_close(pfile);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}
	//ע�ļ�ϵͳΪ8λ��DSPΪ16λ����������2
	total_file_size = pfile->fsize;			
	BT_total_record_counts = total_file_size/sizeof(Errsave_t);
	if(BT_total_record_counts == 0)
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_NO_MANUALJB_RECORD);
		f_close(pfile);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}
	app_BLUETOOTHmod_cal_manualJB_total_items(pfile,psrc_start,UPLOAD_NEW_RECORD);
	f_lseek(pfile,0);
	BT_current_JB_item_num = 0;
	for(count = 0; count <BT_total_record_counts; count++)	
	{
		f_read(pfile,psrc_start,sizeof(Errsave_t),&file_rw_size);
		//�ж��Ƿ��Ѿ��ϴ�
		p_Errsave_t = (Errsave_t *)psrc_start;
		if((p_Errsave_t->DL_app_user_info.data_ext[0] != UPDATED_FALG)||(p_Errsave_t->DL_app_user_info.data_ext[1] != UPDATED_FALG))
		{
			//��λ����һ�Σ�˫λ����2��
			for(meter_id_num = 0; meter_id_num < 2; meter_id_num++)
			{
				BT_current_meter_number = meter_id_num + 1;
				BT_current_JB_item_num++;
				//ˢ�½�����ʾ
				app_BLUETOOTHmod_window_refresh_comm_stat(MANUALJB_NEW_RECORD);
				total_send_size = app_BLUETOOTHmod_create_manualJB_frame(pdes_start,psrc_start,m_recv_buf);
				app_BLUETOOTHmod_uart_send_by512((const uint8_t*)pdes_start,total_send_size);
				new_record_sented++;
				GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
				if(p_Errsave_t->DL_app_user_set.meter_1OR3 == SINGEL_METER)
				{
					break;
				}	
			}
			//��д��־����0x55,0x55
			p_Errsave_t->DL_app_user_info.data_ext[0] = UPDATED_FALG;
			p_Errsave_t->DL_app_user_info.data_ext[1] = UPDATED_FALG;
			file_pos_temp = count * sizeof(Errsave_t);
			f_lseek(pfile,file_pos_temp);	
			f_write(pfile,psrc_start,sizeof(Errsave_t),&file_rw_size);
			file_pos_temp = (count+1) * sizeof(Errsave_t);
			f_lseek(pfile,file_pos_temp);
		}
		GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
		GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	}	
	if(new_record_sented == 0)
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_NO_NEW_MANUALJB_RECORD);
	}
	f_close(pfile);
	app_free(pdes_start);
	app_free(psrc_start);
	app_BLUETOOTHmod_window_refresh_comm_stat(RECORD_UPLOAD_FINISHED);
	return;
}



/********************************************************************
* @������ :������
* @���� : �ϴ������ֶ�У���¼
* @���� : ���ڽ��ջ���
* @��� : 
********************************************************************/
void app_BLUETOOTHmod_manualJB_upload_all(uint8_t *m_recv_buf)
{
	uint16_t 	file_rw_size 	= 0;
	uint16_t 	total_send_size = 0;
	DWORD		total_file_size = 0;
	uint16_t    count 	 		= 0;
	uint8_t		meter_id_num 	= 0;
	DWORD		file_pos_temp	= 0;
	
	char		*pdes_start  = PNULL;
	char		*psrc_start  = PNULL;
	FIL			*pfile = PNULL;
	Errsave_t	*p_Errsave_t = PNULL;
	
	pdes_start =	(char *)app_malloc(sizeof(Errsave_t)*2);
	psrc_start =  (char *)app_malloc( sizeof(Errsave_t));
	if((psrc_start == PNULL) || (pdes_start == PNULL))
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_UART_NO_MEM);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}
	GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
	GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	if(f_open(pfile, MANUALJB_SAVED_FILE_NAME,FA_READ|FA_WRITE|FA_OPEN_EXISTING)!=FR_OK)
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_MANUALJB_FR_OPEN);
		f_close(pfile);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}

	//ע�ļ�ϵͳΪ8λ��DSPΪ16λ����������2
	total_file_size = pfile->fsize;			
	BT_total_record_counts = total_file_size/sizeof(Errsave_t);
	
	if(BT_total_record_counts == 0)
	{
		app_BLUETOOTHmod_err_echo(m_recv_buf,ERR_NO_MANUALJB_RECORD);
		f_close(pfile);
		app_free(pdes_start);
		app_free(psrc_start);
		return;
	}
	
	app_BLUETOOTHmod_cal_manualJB_total_items(pfile,psrc_start,UPLOAD_ALL_RERORD);
	f_lseek(pfile,0);
	BT_current_JB_item_num  = 0;
	for(count = 0; count <BT_total_record_counts; count++)
	{
		f_read(pfile,psrc_start,sizeof(Errsave_t),&file_rw_size);
		for(meter_id_num = 0; meter_id_num < 2; meter_id_num++)
		{
			BT_current_JB_item_num++;
			BT_current_meter_number = meter_id_num + 1;
			//ˢ�½�����ʾ
			app_BLUETOOTHmod_window_refresh_comm_stat(MANUALJB_ALL_RECORD);
			total_send_size = app_BLUETOOTHmod_create_manualJB_frame(pdes_start,psrc_start,m_recv_buf);
			app_BLUETOOTHmod_uart_send_by512((const uint8_t *)pdes_start,total_send_size);	
			//������һ�Σ�3����3��
			GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
			p_Errsave_t = (Errsave_t *)psrc_start;
			if(p_Errsave_t->DL_app_user_set.meter_1OR3 == SINGEL_METER)
			{
				break;
			}
		}
		if((p_Errsave_t->DL_app_user_info.data_ext[0] != UPDATED_FALG)||(p_Errsave_t->DL_app_user_info.data_ext[1] != UPDATED_FALG))
		{
			//��д��־����0x55,0x55
			p_Errsave_t->DL_app_user_info.data_ext[0] = UPDATED_FALG;
			p_Errsave_t->DL_app_user_info.data_ext[1] = UPDATED_FALG;
			file_pos_temp = count * sizeof(Errsave_t);
			f_lseek(pfile,file_pos_temp);	
			f_write(pfile,psrc_start,sizeof(Errsave_t),&file_rw_size);
			file_pos_temp = (count+1) * sizeof(Errsave_t);
			f_lseek(pfile,file_pos_temp);
		}
		GLOBAL_MEMCLR(pdes_start, sizeof(Errsave_t)*2);
		GLOBAL_MEMCLR(psrc_start, sizeof(Errsave_t));
	}	
	
	f_close(pfile);
	app_free(pdes_start);
	app_free(psrc_start);
	//app_UARTmod_window_refresh_comm_stat(RECORD_UPLOAD_FINISHED);
	//msleep( 100 );
	return;
}


/********************************************************************
* @������ :������
* @���� : ʵ��У��PC����У��ƻ�
* @���� : ���ڽ��ջ��������������ݴ�С
* @��� : 
********************************************************************/
void app_BLUETOOTHmod_manualJB_plan_download(  
		uint8_t *m_recv_buf,	
		const uint16_t size)
{
	return;
}




