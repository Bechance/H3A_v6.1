/*********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:���ʼ���
* ����ʱ��:2011-8-29
* ������˾ :����
* �ļ�����:app_WIFImod_redeem.c
* ���� :�ҳ�
* ��������:���߼��׷���ʼ���
******************************************************************************************/
#include"app_WIFImod_inside.h"
static float m_WIFImod_RUL = 1;  	//A��ʵ�ʵ�ѹ
static float m_WIFImod_RIL = 1;  	//B��ʵ�ʵ�ѹ
static float m_WIFImod_RJ  = 0;  	//����ƽ���½Ƕ�
static float m_WIFImod_RJa = 0;  	//����ƽ����A��Ƕ�
static float m_WIFImod_RJb = 0;  	//����ƽ����B��Ƕ�
static float m_WIFImod_RJc = 0;  	//����ƽ����C��Ƕ�
static float m_WIFImod_EJa = 0;  	//a��ʵ�ʽǶ�
static float m_WIFImod_EJb = 0;  	//b��ʵ�ʽǶ�
static float m_WIFImod_EJc = 0;  	//c��ʵ�ʽǶ�
static uint8_t m_redeem_JXMOD = 0;	// 0--3��4��   1--3��3��
/********************************************************************
 * @���� :�ҳ�
 * @���� :�����������µ���׷����
 * @���� :NONE
 * @��� :NONE
********************************************************************/
#define XCRADIAN(angle) (( angle) * 3.1415926 / 180 )
#define POBTAIN(phase)	(phase.f_U*phase.f_I)
static float app_WIFImod_GX_obtain(void)
{
	float TRUE_P=0; //��ʵ�ܹ���
	float ERR_P=0; //��ʵ�ܹ���
	float GX = 0; 	//׷����
	TRUE_P+= m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_RJa));
	if(m_redeem_JXMOD==0)
	{
		TRUE_P+=m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_RJb));
	}
	TRUE_P+=m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_RJc));

	ERR_P+=m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_EJa));
	if(m_redeem_JXMOD==0)
	{
		ERR_P+= m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_EJb));
	}
	ERR_P+=m_WIFImod_RUL*m_WIFImod_RIL*cos(XCRADIAN(m_WIFImod_EJc));
    if(ERR_P==0)
    {
		return 1000;
	}
	GX = TRUE_P / ERR_P;
	return  (GLOBAL_ABS(GX)>1000)?(1000):(GX);
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :���������������ʵ���
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static float app_redeem_AtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUa,"Ua",2))
	base_j+=0;
	else if(global_memcmp(presult->Result_MUa,"Ub",2))
	base_j-=120;
	else if(global_memcmp(presult->Result_MUa,"Uc",2))
	base_j+=120;
	if(global_memcmp(presult->Result_MIa,"ia",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIa,"-ia",3))
	{
		return base_j+180.0;
	}
	else if(global_memcmp(presult->Result_MIa,"ib",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIa,"-ib",3))
	{
		return base_j+120+180.0;
	}
	else if(global_memcmp(presult->Result_MIa,"ic",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIa,"-ic",3))
	{
		return base_j-120+180;
	}	
	return 0;
}
static float app_redeem_BtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUb,"Ua",2))
		base_j+=120;
	else if(global_memcmp(presult->Result_MUb,"Ub",2))
		base_j-=0;
	else if(global_memcmp(presult->Result_MUb,"Uc",2))
		base_j-=120;
	if(global_memcmp(presult->Result_MIb,"ia",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIb,"-ia",3))
	{
		return base_j-120+180;
	}
	else if(global_memcmp(presult->Result_MIb,"ib",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIb,"-ib",3))
	{
		return base_j+180.0;
	}
	else if(global_memcmp(presult->Result_MIb,"ic",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIb,"-ic",3))
	{
		return base_j+120+180.0;
	}	
	return 0;
}

static float app_redeem_CtrueJ_obtain(
	WIRresult_t*presult,
	float angle
)
{
	float  base_j= angle;
	if(global_memcmp(presult->Result_MUc,"Ua",2))
	base_j-=120;
	else if(global_memcmp(presult->Result_MUc,"Ub",2))
	base_j+=120;
	else if(global_memcmp(presult->Result_MUc,"Uc",2))
	base_j-=0;
	if(global_memcmp(presult->Result_MIc,"ia",2))
	{
		return base_j+120;
	}
	else if(global_memcmp(presult->Result_MIc,"-ia",3))
	{
		return base_j+120+180;
	}
	else if(global_memcmp(presult->Result_MIc,"ib",2))
	{
		return base_j-120;
	}
	else if(global_memcmp(presult->Result_MIc,"-ib",3))
	{
		return base_j-120+180;
	}
	else if(global_memcmp(presult->Result_MIc,"ic",2))
	{
		return base_j;
	}
	else if(global_memcmp(presult->Result_MIc,"-ic",3))
	{
		return base_j+180.0;
	}	
	return 0;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :����׷������ʼ��
 * @���� :presult ���߽����Pdl ��ǰ�繤������JXmod ����ģʽ
 * @��� :NONE
********************************************************************/
#define  _ANGLE_TRANS(angle)  (angle>180)?(angle-360):(angle)
static void app_WIFImod_redeem_init(
	WIRresult_t*presult,
	measure_param_t*Pdl,
	uint8_t JXmod
	)
{
	m_redeem_JXMOD = JXmod;
    if(JXmod==0) /*3��4��*/
    {
		m_WIFImod_RJa = m_WIFImod_RJ;
		m_WIFImod_RJb = m_WIFImod_RJ;
		m_WIFImod_RJc = m_WIFImod_RJ;
	    m_WIFImod_EJa=_ANGLE_TRANS(app_redeem_AtrueJ_obtain(presult,m_WIFImod_RJa));
	    m_WIFImod_EJb=_ANGLE_TRANS(app_redeem_BtrueJ_obtain(presult,m_WIFImod_RJb));
	    m_WIFImod_EJc=_ANGLE_TRANS(app_redeem_CtrueJ_obtain(presult,m_WIFImod_RJc));
	}
    else
    {
		m_WIFImod_RJa = m_WIFImod_RJ;
		m_WIFImod_RJb = m_WIFImod_RJ;
		m_WIFImod_RJc = m_WIFImod_RJ;
	    m_WIFImod_EJa=_ANGLE_TRANS(app_redeem_AtrueJ_obtain(presult,m_WIFImod_RJa));
	    m_WIFImod_EJb=_ANGLE_TRANS(app_redeem_BtrueJ_obtain(presult,m_WIFImod_RJb));
	    m_WIFImod_EJc=_ANGLE_TRANS(app_redeem_CtrueJ_obtain(presult,m_WIFImod_RJc));
		m_WIFImod_RJa = 30+m_WIFImod_RJ;
		m_WIFImod_RJc = m_WIFImod_RJ-30;
		if(presult->phaseDir==PHASE_ADD)
		{
			m_WIFImod_EJa += 30;
	        m_WIFImod_EJc -= 30;
		}
		else
		{
			m_WIFImod_EJa -= 30;
	        m_WIFImod_EJc += 30;
		}
	}
	return;
}
/*******************************************************************************/
//Ӧ��APP
/*******************************************************************************/
char 		 m_WIR_cos[10];
char 		 m_WIR_cos_icon[2];
/********************************************************************
 * @���� :�ҳ�
 * @���� :��ȡ������λ��
 * @���� :NONE
 * @��� :NONE
********************************************************************/
static float app_WIFImod_cos_obtain(char*pcos)
{
  char temp[10]="";	
  float angle;
  float fangle;
  global_strcpy(temp,pcos);
  global_string_filter(temp,' ');
  if(m_WIR_cos_icon[0]=='L')
  {
        fangle = 0+acosf(global_atof(temp));
  }
  else if(m_WIR_cos_icon[0]=='C')
  {
     fangle = 0-acosf(global_atof(temp));
  }
  angle = fangle*180/3.1415926;
  return angle;
}
static void _cos_create(WIRresult_t*p)
{
    global_strcpy(m_WIR_cos,p->cos);
	m_WIR_cos_icon[1] = 0;m_WIR_cos_icon[0] = 0;
	if (global_string_locate_first(m_WIR_cos,"C"))
	m_WIR_cos_icon[0] ='C';
	else if (global_string_locate_first(m_WIR_cos,"L"))
	m_WIR_cos_icon[0] ='L';
	global_string_filter(m_WIR_cos,'L');
	global_string_filter(m_WIR_cos,'C');
	return;
}
/********************************************************************
 * @���� :�ҳ�
 * @���� :���ܺ������
 * @���� :NONE
 * @��� :NONE
********************************************************************/
void app_WIFImod_redeem_GX_obtian(WIRresult_t*p)
{
    float GX;
	if(p->cos[0]==0||p->cos[0]==' ')return;
	_cos_create(p);
	m_WIFImod_RJ = app_WIFImod_cos_obtain(m_WIR_cos);
	app_WIFImod_redeem_init(p,&g_DL_param,p->jxmod);
	GX = app_WIFImod_GX_obtain();
	global_ftoa(GX,p->GX,2);
	return;
}
