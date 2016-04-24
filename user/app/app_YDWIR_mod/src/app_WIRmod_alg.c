/*********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称: 查线 管理模块算法
* 创建时间:2010-09-19
* 所属公司 :科荟
* 文件名称:app_WIRmod_alg.c
* 创建人 :揭成
* 功能描述:查线算法实现
*-------------------------------------------------------------------*/
#include 	 	<stdlib.h>
#include		"app_WIRmod_inside.h"
#include 		"../../../support\app_support.h"
/*引用H3A 代码----------------------------------------------------------*/
//参数变量
float Pa = 0;
float Qa = 0;
float Ua = 0;
float Ia = 0;
float Ja = 0;		//UI之间夹角
float Pb = 0;
float Qb = 0;
float Ub = 0;
float Ib = 0;
float Jb = 0;		//UI之间夹角
float Pc = 0;
float Qc = 0;
float Uc = 0;
float Ic = 0;
float Jc = 0;		//UI之间夹角

float JUab = 0;	//电压夹角
float JUcb = 0;
float JUac = 0;
float JIac = 0;	//i之间夹角
float JIab = 0;
float JIbc = 0;

float Pabc = 0;
float Qabc = 0;
float COSabc = 0;
float Frequency = 50;
#define 		DataLine 145
#define 		DGCS_Line 1
#define 		FG_Line 150 //分隔线
char Load_Model = 0;
unsigned char   	cha_x[4][32];//查接线错误结果
unsigned char 	result[5];//存储查线的分析结果
struct cha
{
	int cfuac;			//Uac
	int cfiac;			//Iac
	int cfa;			//UIa  
	int cfc;			//UIc
	char *uxian;
	char *ixian;
};
struct  cha const cha48[48] =
{
	//三线三线48种接线错误的查线表
	-60, -120, -60, -120, "Ua Ub Uc", " ia      ic",
	-60, 120, -180,   0, "Ua Ub Uc", " ic      ia",
	-60, -120, 120,  60, "Ua Ub Uc", "-ia     -ic",
	-60, 120,   0, -180, "Ua Ub Uc", "-ic     -ia",
	-60,  60, 120, -120, "Ua Ub Uc", "-ia      ic",
	-60, -60, -180, -180, "Ua Ub Uc", "ic      -ia", //ic  -ia
	-60,  60, -60,  60, "Ua Ub Uc", " ia     -ic",
	-60, -60,   0,   0, "Ua Ub Uc", " -ic     ia", //-ic ia
	-60, -120, -180, 120, "Ub Uc Ua", " ia      ic",
	-60, 120,  60, -120, "Ub Uc Ua", " ic      ia",
	-60, -120,   0, -60, "Ub Uc Ua", "-ia     -ic",
	-60, 120, -120,  60, "Ub Uc Ua", "-ic     -ia",
	-60,  60,   0, 120, "Ub Uc Ua", "-ia      ic",
	-60, -60,  60,  60, "Ub Uc Ua", " ic     -ia",
	-60,  60, -180, -60, "Ub Uc Ua", " ia     -ic",
	-60, -60, -120, -120, "Ub Uc Ua", "-ic      ia",
	-60, -120,  60,   0, "Uc Ua Ub", " ia      ic",
	-60, 120, -60, 120, "Uc Ua Ub", " ic      ia",
	-60, -120, -120, -180, "Uc Ua Ub", "-ia     -ic",
	-60, 120, 120, -60, "Uc Ua Ub", "-ic     -ia",
	-60,  60, -120,   0, "Uc Ua Ub", "-ia      ic",
	-60, -60, -60, -60, "Uc Ua Ub", " ic     -ia",
	-60,  60,  60, -180, "Uc Ua Ub", " ia     -ic",
	-60, -60, 120, 120, "Uc Ua Ub", "-ic      ia",
	60, -120, -120,  60, "Ua Uc Ub", " ia      ic",
	60, 120, 120, -180, "Ua Uc Ub", " ic      ia",
	60, -120,  60, -120, "Ua Uc Ub", "-ia     -ic",
	60, 120, -60,   0, "Ua Uc Ub", "-ic     -ia",
	60,  60,  60,  60, "Ua Uc Ub", "-ia      ic",
	60, -60, 120,   0, "Ua Uc Ub", " ic     -ia",
	60,  60, -120, -120, "Ua Uc Ub", " ia     -ic",
	60, -60, -60, -180, "Ua Uc Ub", "-ic      ia",
	60, -120, 120, -60, "Ub Ua Uc", " ia      ic",
	60, 120,   0,  60, "Ub Ua Uc", " ic      ia",
	60, -120, -60, 120, "Ub Ua Uc", "-ia     -ic",
	60, 120, -180, -120, "Ub Ua Uc", "-ic     -ia",
	60,  60, -60, -60, "Ub Ua Uc", "-ia      ic",
	60, -60,   0, -120, "Ub Ua Uc", " ic     -ia",
	60,  60, 120, 120, "Ub Ua Uc", " ia     -ic",
	60, -60, -180,  60, "Ub Ua Uc", "-ic      ia",
	60, -120,   0, -180, "Uc Ub Ua", " ia      ic",
	60, 120, -120, -60, "Uc Ub Ua", " ic      ia",
	60, -120, -180,   0, "Uc Ub Ua", "-ia     -ic",
	60, 120,  60, 120, "Uc Ub Ua", "-ic     -ia",
	60,  60, -180, -180, "Uc Ub Ua", "-ia      ic",
	60, -60, -120, 120, "Uc Ub Ua", " ic     -ia",
	60,  60,   0,   0, "Uc Ub Ua", " ia     -ic",
	60, -60,  60, -60, "Uc Ub Ua", "-ic      ia",
};
struct cha2
{
	unsigned char czhen; //方向
	int cfa;	 //UIa
	int cfb;	 //UIb
	int cfc;	 //UIc
	char *uxian;
	char *ixian;
};
struct  cha2 const cha96[97] =  				 //三线四线48种接线错误的查线表
{
	'+',  -90, -90, -90, "Ua Ub Uc", " ia  ib  ic",
	'+',  -90, -90,  90, "Ua Ub Uc", " ia  ib -ic",
	'+',  -90,  90, -90, "Ua Ub Uc", " ia -ib  ic",
	'+',  -90,  90,  90, "Ua Ub Uc", " ia -ib -ic",
	'+',  -90,  30, 150, "Ua Ub Uc", " ia  ic  ib",
	'+',  -90,  30, -30, "Ua Ub Uc", " ia  ic -ib",
	'+',  -90, -150, 150, "Ua Ub Uc", " ia -ic  ib",
	'+',  -90, -150, -30, "Ua Ub Uc", " ia -ic -ib",
	'+',   90, -90, -90, "Ua Ub Uc", "-ia  ib  ic",
	'+',   90, -90,  90, "Ua Ub Uc", "-ia  ib -ic",
	'+',   90,  90, -90, "Ua Ub Uc", "-ia -ib  ic",
	'+',   90,  90,  90, "Ua Ub Uc", "-ia -ib -ic",
	'+',   90,  30, 150, "Ua Ub Uc", "-ia  ic  ib",
	'+',   90,  30, -30, "Ua Ub Uc", "-ia  ic -ib",
	'+',   90, -150, 150, "Ua Ub Uc", "-ia -ic  ib",
	'+',   90, -150, -30, "Ua Ub Uc", "-ia -ic -ib",
	'+',   30, 150, -90, "Ua Ub Uc", " ib  ia  ic",
	'+',   30, 150,  90, "Ua Ub Uc", " ib  ia -ic",
	'+',   30, -30, -90, "Ua Ub Uc", " ib -ia  ic",
	'+' ,  30, -30,  90, "Ua Ub Uc", " ib -ia -ic",
	'+' ,  30,  30,  30, "Ua Ub Uc", " ib  ic  ia",
	'+' ,  30,  30, -150, "Ua ub Uc", " ib  ic -ia",
	'+' ,  30, -150,  30, "Ua Ub Uc", " ib -ic  ia",
	'+' ,  30, -150, -150, "Ua Ub Uc", " ib -ic -ia",
	'+' , -150, 150, -90, "Ua Ub Uc", "-ib  ia  ic",
	'+' , -150, 150,  90, "Ua Ub Uc", "-ib  ia -ic",
	'+' , -150, -30, -90, "Ua Ub Uc", "-ib -ia  ic",
	'+' , -150, -30,  90, "Ua Ub Uc", "-ib -ia -ic",
	'+' , -150,  30,  30, "Ua Ub Uc", "-ib  ic  ia",
	'+' , -150,  30, -150, "Ua Ub Uc", "-ib  ic -ia",
	'+' , -150, -150,  30, "Ua Ub Uc", "-ib -ic  ia",
	'+' , -150, -150, -150, "Ua Ub Uc", "-ib -ic -ia",
	'+' , 150, -90,  30, "Ua Ub Uc", " ic  ib  ia",
	'+' , 150, -90, -150, "Ua Ub Uc", " ic  ib -ia",
	'+' , 150,  90,  30, "Ua Ub Uc", " ic -ib  ia",
	'+' , 150,  90, -150, "Ua Ub Uc", " ic -ib -ia",
	'+' , 150, 150, 150, "Ua Ub Uc", " ic  ia  ib",
	'+' , 150, 150, -30, "Ua Ub Uc", " ic  ia -ib",
	'+' , 150, -30, 150, "Ua Ub Uc", " ic -ia  ib",
	'+' , 150, -30, -30, "Ua Ub Uc", " ic -ia -ib",
	'+' , -30, -90,  30, "Ua Ub Uc", "-ic  ib  ia",
	'+' , -30, -90, -150, "Ua Ub Uc", "-ic  ib -ia",
	'+' , -30,  90,  30, "Ua Ub Uc", "-ic -ib  ia",
	'+' , -30,  90, -150, "Ua Ub Uc", "-ic -ib -ia",
	'+' , -30, 150, 150, "Ua Ub Uc", "-ic  ia  ib",
	'+' , -30, 150, -30, "Ua Ub Uc", "-ic  ia -ib",
	'+' , -30, -30, 150, "Ua Ub Uc", "-ic -ia  ib",
	'+'  , -30, -30, -30, "Ua Ub Uc", "-ic -ia -ib",
	'_'  , -90, 150,  30, "Ua Uc Ub", " ia  ib  ic",
	'_'  , -90, 150, -150, "Ua Uc Ub", " ia  ib -ic",
	'_'  , -90, -30,  30, "Ua Uc Ub", " ia -ib  ic",
	'_' , -90, -30, -150, "Ua Uc Ub", " ia -ib -ic",
	'_' , -90, -90, -90, "Ua Uc Ub", " ia  ic  ib",
	'_' , -90, -90,  90, "Ua Uc Ub", " ia  ic -ib",
	'_' , -90,  90, -90, "Ua Uc Ub", " ia -ic  ib",
	'_' , -90,  90,  90, "Ua Uc Ub", " ia -ic -ib",
	'_' ,  90, 150,  30, "Ua Uc Ub", "-ia  ib  ic",
	'_' ,  90, 150, -150, "Ua Uc Ub", "-ia  ib -ic",
	'_' ,  90, -30,  30, "Ua Uc Ub", "-ia -ib  ic",
	'_' ,  90, -30, -150, "Ua Uc Ub", "-ia -ib -ic",
	'_' ,  90, -90, -90, "Ua Uc Ub", "-ia  ic  ib",
	'_' ,  90, -90,  90, "Ua Uc Ub", "-ia  ic -ib",
	'_' ,  90,  90, -90, "Ua Uc Ub", "-ia -ic  ib",
	'_' ,  90,  90,  90, "Ua Uc Ub", "-ia -ic -ib",
	'_' ,  30,  30,  30, "Ua Uc Ub", " ib  ia  ic",
	'_' ,  30,  30, -150, "Ua Uc Ub", " ib  ia -ic",
	'_' ,  30, -150,  30, "Ua Uc Ub", " ib -ia  ic",
	'_' ,  30, -150, -150, "Ua Uc Ub", " ib -ia -ic",
	'_' ,  30, -90, 150, "Ua Uc Ub", " ib  ic  ia",
	'_' ,  30, -90, -30, "Ua Uc Ub", " ib  ic -ia",
	'_' ,  30,  90, 150, "Ua Uc Ub", " ib -ic  ia",
	'_' ,  30,  90, -30, "Ua Uc Ub", " ib -ic -ia",
	'_' , -150,  30,  30, "Ua Uc Ub", "-ib  ia  ic",
	'_' , -150,  30, -150, "Ua Uc Ub", "-ib  ia -ic",
	'_' , -150, -150,  30, "Ua Uc Ub", "-ib -ia  ic",
	'_' , -150, -150, -150, "Ua Uc Ub", "-ib -ia -ic",
	'_' , -150, -90, 150, "Ua Uc Ub", "-ib  ic  ia",
	'_' , -150, -90, -30, "Ua Uc Ub", "-ib  ic -ia",
	'_' , -150,  90, 150, "Ua Uc Ub", "-ib -ic  ia",
	'_' , -150,  90, -30, "Ua Uc Ub", "-ib -ic -ia",
	'_' , 150, 150, 150, "Ua Uc Ub", " ic  ib  ia",
	'_' , 150, 150, -30, "Ua Uc Ub", " ic  ib -ia",
	'_' , 150, -30, 150, "Ua Uc Ub", " ic -ib  ia",
	'_' , 150, -30, -30, "Ua Uc Ub", " ic -ib -ia",
	'_'  , 150,  30, -90, "Ua Uc Ub", " ic  ia  ib",
	'_' , 150,  30,  90, "Ua Uc Ub", " ic  ia -ib",
	'_' , 150, -150, -90, "Ua Uc Ub", " ic -ia  ib",
	'_' , 150, -150,  90, "Ua Uc Ub", " ic -ia -ib",
	'_' , -30, 150, 150, "Ua Uc Ub", "-ic  ib  ia",
	'_' , -30, 150, -30, "Ua Uc Ub", "-ic  ib -ia",
	'_' , -30, -30, 150, "Ua Uc Ub", "-ic -ib  ia",
	'_' , -30, -30, -30, "Ua Uc Ub", "-ic -ib -ia",
	'_' , -30,  30, -90, "Ua Uc Ub", "-ic  ia  ib",
	'_' , -30,  30,  90, "Ua Uc Ub", "-ic  ia -ib",
	'_' , -30, -150, -90, "Ua Uc Ub", "-ic -ia  ib",
	'_' , -30, -150,  90, "Ua Uc Ub", "-ic -ia -ib",
};
//itoa: int型转换成字符型   返回unsigned char* string地址
char* itoa( long int value, char* string, unsigned int radix )
{
	long int sign;
	unsigned int i, j;
	char c;
	if (( sign = value ) < 0 )      //记录正负号
		value = -value;             //使n为正
	i = 0;
	do                              //得到n的每位数字，并存到数组中，注意顺序是颠倒的
	{
		string[i++] = value % radix + '0';
	}
	while (( value /= radix ) > 0 );
	if ( sign < 0 )
		string[i++] = '-';         //存符号
	string[i] = '\0';               //字符串结束标志
	i--;     			   //将i退回到字符串结束标志前一位

	for ( j = 0;j < i;j++, i-- )		   //将字符串内容颠倒过来
	{
		c = string[j];
		string[j] = string[i];
		string[i] = c;
	}
	return string;
}
int fgui( int x )
{
	if ( x > 180 )  x = x - 360;
	if ( x < ( -180 ) ) x = x + 360;
	if ( x == ( 180 ) ) x = -180;
	return x;
}

void jiao( int cf, int *cf1, int *cf2, int *cf3, int *cf4 )
{
	if ( cf < 0 )
	{
		*cf1 = cf;
		*cf2 = 0;
		*cf3 = 0;
		*cf4 = 180 + cf;
	}
	if ( cf > 0 )
	{
		*cf1 = cf;
		*cf2 = 180;
		*cf3 = -180;
		*cf4 = cf - 180;
	}
	if ( cf == 0 )
	{
		*cf1 = 0;
		*cf2 = 180;
		*cf3 = 0;
		*cf4 = 180;
	}
}

//fuac:Uac
//fiac:Iac
//fa:UIa
//fc:UIc
//cha1~cha4 :各种负载的接线错误显示情况
//pec-8移植过来的
void chasan( int fuac, int fiac, int fa, int fc, unsigned char cha_x[4][32] )
{
	int cfuac, cfiac, famin, famin1, famax, famax1, fcmin, fcmin1, fcmax, fcmax1, i, j, f;//bb,cc;
	double cosf, pi = 3.1415926;//cosf1;
	char coss[20];
	char *coo;
	unsigned char LC_Flag = 0;
	for ( i = 0;i < 28;i++ )
	{
		( cha_x[0][i] ) = ' ';
		( cha_x[1][i] ) = ' ';
		( cha_x[2][i] ) = ' ';
		( cha_x[3][i] ) = ' ';
	}
	( cha_x[0][28] ) = 0x0;
	( cha_x[1][28] ) = 0x0;
	( cha_x[2][28] ) = 0x0;
	( cha_x[3][28] ) = 0x0;
	i = 0;
	if (( fuac > 40 ) && ( fuac < 80 ) )
		cfuac = 60;
	if (( fuac > -80 ) && ( fuac < -40 ) )
		cfuac = -60;
	if (( fiac > 91 ) && ( fiac < 149 ) )
		cfiac = 120;
	if (( fiac > -149 ) && ( fiac < -91 ) )
		cfiac = -120;
	if (( fiac > 31 ) && ( fiac < 89 ) )
		cfiac = 60;
	if (( fiac > -89 ) && ( fiac < -31 ) )
		cfiac = -60;
	for ( j = 0;j < 48;j++ )
	{
		if (( cha48[j].cfuac == cfuac ) && ( cha48[j].cfiac == cfiac ) )
		{
			jiao( cha48[j].cfa, &famin, &famin1, &famax, &famax1 );
			jiao( cha48[j].cfc, &fcmin, &fcmin1, &fcmax, &fcmax1 );
			f = ( fgui( fgui( fa - cha48[j].cfa ) - 90 ) + fgui( fc - fgui( cha48[j].cfc ) - 90 ) ) / 2;
			cosf = cos(( pi / 180 ) * f ) ;
			if ((( fa >= famin ) && ( fa <= famin1 ) ) || (( fa >= famax ) && ( fa <= famax1 ) ) || (( fc >= fcmin ) && ( fc <= fcmin1 ) ) || (( fc >= fcmax ) && ( fc <= fcmax1 ) ) )
				if (( cosf >= 0 ) )
				{
					itoa(( long int )( cosf*100 ), &coss[5], 10 );
					coo = &coss[5];
					if ( strlen( coo ) == 3 )
					{
						global_memcpy( &coss[0], "1.00", 4 );
						LC_Flag++;
					}
					if ( strlen( coo ) == 2 )
					{
						coss[0] = '0';
						coss[1] = '.';
						global_memcpy( &coss[2], &coss[5], 2 );
						if ( f >= 0 )
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'L';
						}
						else
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'C';
						}
					}
					if ( strlen( coo ) == 1 )
					{
						coss[0] = '0';
						coss[1] = '.';
						coss[2] = '0';
						global_memcpy( &coss[3], &coss[5], 1 );
						if ( f >= 0 )                //+ xqs 070525
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'L';          //- xqs 070625 显示功率因数，在大六角图的版本可以不要

						}
						else
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'C';
						}
					}
					global_memcpy( &cha_x[LC_Flag-1][0], &coss[0], 4 );
					global_memcpy( &cha_x[LC_Flag-1][6], cha48[j].uxian, 8 );
					global_memcpy( &cha_x[LC_Flag-1][16], cha48[j].ixian, 11 );
					cha_x[LC_Flag-1][28] = j; //存放错误接线的编号
					cha_x[LC_Flag-1][27] = 0x0;
				}
		}
	}
}

//zhen:接线正负
//fa:UIa
//fa:UIb
//fc:UIc
//cha1~cha4 :各种负载的接线错误显示情况
//pec-8移植过来的
void chashi( unsigned char zhen, int fa, int fb, int fc, unsigned char cha_x[4][32] )
{
	int famin, famin1, famax, famax1, fcmin, fcmin1, fcmax, fcmax1, j, f;//cfuac,cfiac,bb,cc;
	int fbmin, fbmin1, fbmax, fbmax1, jiaoa, jiaob, jiaoc;//fbu;
	double cosf, pi = 3.1415926;
	char coss[20];
	char *coo;              //,cha4me=cosΦ<0.01时无法查线!;
	unsigned char LC_Flag = 0, i;
	for ( i = 0;i < 28;i++ )
	{
		( cha_x[0][i] ) = ' ';
		( cha_x[1][i] ) = ' ';
		( cha_x[2][i] ) = ' ';
		( cha_x[3][i] ) = ' ';
	};
	( cha_x[0][28] ) = 0x0;
	( cha_x[1][28] ) = 0x0;
	( cha_x[2][28] ) = 0x0;
	( cha_x[3][28] ) = 0x0;
	i = 0;
	for ( j = 0;j < 96;j++ )
	{   //计算某种接线情况的角度的区域
		jiao( cha96[j].cfa, &famin, &famin1, &famax, &famax1 );
		jiao( cha96[j].cfc, &fcmin, &fcmin1, &fcmax, &fcmax1 );
		jiao( cha96[j].cfb, &fbmin, &fbmin1, &fbmax, &fbmax1 );
		jiaoa = fgui( fgui( fa - cha96[j].cfa ) - 90 );
		jiaob = fgui( fgui( fb - cha96[j].cfb ) - 90 );
		jiaoc = fgui( fgui( fc - cha96[j].cfc ) - 90 );
		f = ( jiaoa + jiaob + jiaoc ) / 3;
		cosf = cos(( pi / 180 ) * f ) ;
		if (( zhen == cha96[j].czhen ) && ((( fa >= famin ) && ( fa <= famin1 ) ) || (( fa >= famax ) && ( fa <= famax1 ) ) ) && ((( fb >= fbmin ) && ( fb <= fbmin1 ) ) || (( fb >= fbmax ) && ( fb <= fbmax1 ) ) ) && ((( fc >= fcmin ) && ( fc <= fcmin1 ) ) || (( fc >= fcmax ) && ( fc <= fcmax1 ) ) ) )
			if (( cosf >= 0 ) )
				if (( abs( jiaoa - jiaob ) < 60 ) && ( abs( jiaob - jiaoc ) < 60 ) && ( abs( jiaoa - jiaoc ) < 60 ) )
				{// coo=(unsigned char*)itoa((int)(cosf*100),&coss[5],10); //- xqs 070706
					itoa(( long int )( cosf*100 ), &coss[5], 10 );//将当时的功率因素值转换成字符
					coo = &coss[5];
					if ( strlen( coo ) == 3 )
					{
						global_memcpy( &coss[0], "1.00", 4 );
						LC_Flag++;
					}
					if ( strlen( coo ) == 2 )
					{
						coss[0] = '0';
						coss[1] = '.';
						global_memcpy( &coss[2], &coss[5], 2 );
						if ( f >= 0 )
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'L';
						}
						else
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'C';
						}
					}
					if ( strlen( coo ) == 1 )
					{
						coss[0] = '0';
						coss[1] = '.';
						coss[2] = '0';
						global_memcpy( &coss[3], &coss[5], 1 );
						if ( f >= 0 )                //+ xqs 070525
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'L';          //- xqs 070625 显示功率因数，在大六角图的版本可以不要
							//	LC_Flag=1;

						}
						else
						{
							LC_Flag++;
							cha_x[LC_Flag-1][4] = 'C';
						}
					}
					global_memcpy( &cha_x[LC_Flag-1][0], &coss[0], 4 );
					global_memcpy( &cha_x[LC_Flag-1][6], cha96[j].uxian, 8 );
					global_memcpy( &cha_x[LC_Flag-1][16], cha96[j].ixian, 11 );
					cha_x[LC_Flag-1][28] = j; //存放错误接线的编号
					cha_x[LC_Flag-1][27] = 0x0;
				}
	}
}

void  xiangxu( double fuab, double fuac )  //计算并显示相序
{
	Bsize_t zhen = 0;
	if (( fuab > -140.0 ) && ( fuab < -100.0 ) && ( fuac > 100.0 ) && ( fuac < 140.0 ) ) zhen = 2; //逆
	if (( fuab > 100.0 ) && ( fuab < 140.0 ) && ( fuac > -140.0 ) && ( fuac < -100.0 ) ) zhen = 1; //正
	if (( fuac > 40.0 ) && ( fuac < 80.0 ) ) zhen = 2;                   //逆
	if (( fuac > -80.0 ) && ( fuac < -40.0 ) ) zhen = 1;                 //正
	if ( zhen == 1 )
	{
		m_alg_presult->phaseDir = PHASE_ADD;
	}
	else if ( zhen == 2 )
	{
		m_alg_presult->phaseDir = PHASE_SUB;

	}
	else if ( zhen == 0 )
	{
		m_alg_presult->phaseDir = PHASE_INVALID;
	}
	return;
}
//入参:无
//返回：1为有某相失压或者某相无电流
BOOL  CheckLostUorI( DL_flag_t*UIflag )
{
	BOOL ret = TURE;
	if ( UIflag->phase3or4 == JXMOD_3_4 )//三相四线
	{
		if (( UIflag->Ua_is_lost == TURE ) 
			&& ( UIflag->Ub_is_lost == TURE ) 
			&& ( UIflag->Uc_is_lost == TURE ) 
			&& ( UIflag->Ia_is_lost == TURE ) 
			&& ( UIflag->Ia_is_lost == TURE )
			&& ( UIflag->Ia_is_lost == TURE ) )
		{
			m_alg_presult->is_lost_UI = TURE;
			ret = FAIL;
		}
		else
		{
			if ( UIflag->Ua_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ua_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ub_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ub_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Uc_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Uc_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ia_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ia_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ib_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ib_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ic_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ic_lost = TURE;
				ret = FAIL;
			}
		}
	}
	else if ( UIflag->phase3or4 == JXMOD_3_3 )//三相三线
	{
		if (( UIflag->Ua_is_lost == TURE ) 
			&& ( UIflag->Uc_is_lost == TURE )
			&& ( UIflag->Ia_is_lost == TURE )
			&& ( UIflag->Ic_is_lost == TURE ) )
		{
			m_alg_presult->is_lost_UI = TURE;
			ret = FAIL;
		}
		else
		{
			if ( UIflag->Ua_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ua_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Uc_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Uc_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ia_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ia_lost = TURE;
				ret = FAIL;
			}
			if ( UIflag->Ic_is_lost == TURE)
			{
				m_alg_presult->is_lost_UI = TURE;
				m_alg_presult->Ic_lost = TURE;
				ret = FAIL;
			}
		}
	}
	return ret;
}



void Angle0_360To180( float *fAngle )
{
	if ( fabs( *fAngle ) > 180.0 )
	{
		if ( *fAngle > 0 ) *fAngle -= 360;
		else *fAngle += 360;
	}
	return;
}
void AngleDisposal()
{
	Angle0_360To180( &Ja );
	Angle0_360To180( &Jb );
	Angle0_360To180( &Jc );
	Angle0_360To180( &JUab );
	Angle0_360To180( &JUac );
	Angle0_360To180( &JIac );
	Angle0_360To180( &JUcb );
	return;
}

/*以上代码引用H3A*/
uint8_t 		m_WIRmod_Cline_task_pro = INVALID_PRO;
WIR_ALG_t*	m_alg_presult = PNULL; /*存放结果指针*/
a_sem_t *     m_WIRmod_sem   = INVALID_SEM;
a_sem_t *     m_WIRmod_Resem = INVALID_SEM;

/********************************************************************
 * @创建人 :揭成
 * @功能 :主模块从驱动获取测量参数
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
extern dev_id_t              g_DL_dev ;
extern measure_param_t       g_DL_param;
void app_WIRmod_DL_param_obtain(measure_param_t*pDL)
{
	Ua = pDL->phase_a.f_U;
	Ia = pDL->phase_a.f_I;
	Pa = pDL->phase_a.f_P;
	Qa = pDL->phase_a.f_Q;
	Ja = pDL->phase_a.f_J;
	Ub = pDL->phase_b.f_U;
	Ib = pDL->phase_b.f_I;
	Pb = pDL->phase_b.f_P;
	Qb = pDL->phase_b.f_Q;
	Jb = pDL->phase_b.f_J;
	Uc = pDL->phase_c.f_U;
	Ic = pDL->phase_c.f_I;
	Pc = pDL->phase_c.f_P;
	Qc = pDL->phase_c.f_Q;
	Jc = pDL->phase_c.f_J;
	JUab = pDL->f_JUab;
	JUcb = pDL->f_JUbc;
	JUac = pDL->f_JUac;
	JIac = pDL->f_JIac;
	JIab = pDL->f_JIab;
	JIbc = pDL->f_JIbc;
	Pabc = pDL->f_Pabc;
	Qabc = pDL->f_Qabc;
	COSabc = pDL->f_Cos;
	Frequency = pDL->f_Feq;
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线任务，结果保存在m_alg_presult指向的内存区间
 * @输入 :NONE
 * @输出 :NONE
********************************************************************/
void  app_WIRmod_check_err_task( void*arg )
{
	unsigned char  xianxu, i, j;
	char string[20];
	DL_flag_t dl_flag;
	Bsize_t cnt = 0;
	cnt = cnt;
	while ( 1 )
	{
		if ( os_sem_pend( m_WIRmod_Resem,APP_100_MS) == OS_NO_ERR )
		{
		    app_global_DLflag_obtain(&dl_flag);
			app_WIRmod_DL_param_obtain(&g_DL_param);
			GLOBAL_MEMCLR( m_alg_presult, sizeof( WIR_ALG_t ) * 4 );
			AngleDisposal();
			if ( CheckLostUorI( &dl_flag ) )
			{
				AngleDisposal();
				if (dl_flag.phase3or4== JXMOD_3_3 )//三相三线
				{
					if ( fabs( JUac ) >= 40.0 && fabs( JUac ) <= 80.0 && Ua != 0.0 && Uc != 0.0 && fabs( Ub ) <= 0.05 && fabs( Ib ) <= 0.05 )
					{
						chasan(( int )JUac, ( int )JIac, ( int )Ja, ( int )Jc, cha_x );
					}
					xiangxu( JUab, JUac );// Uab Uac 相序计算
					m_alg_presult->jxmod = JXMOD_3_3;
				}
				else if (dl_flag.phase3or4 == JXMOD_3_4 )//三相四线
				{
					if ( JUab < 0.0 )xianxu = '_';
					else 	    xianxu = '+';
					chashi( xianxu, ( int )Ja, ( int )Jb, ( int )Jc, cha_x );
					xiangxu( JUab, JUac );// Uab Uac 相序计算
					m_alg_presult->jxmod = JXMOD_3_4;
				}
				for ( i = 0;i < 4;i++ )
				{
					for ( j = 0;j < 5;j++ )
						string[j] = cha_x[i][j];
					string[j] = 0;
					global_strcpy( m_alg_presult[i].c_cos, string );
					for ( j = 0;j < 8;j++ )
					{
						if ( j == 2 )
						{
							string[j] = ' ';
							string[j+1] = ' ';
						}
						if ( j == 5 )
						{
							string[j+1] = ' ';
							string[j+2] = ' ';
						}
						else if ( j > 4 ) string[j+2] = cha_x[i][j+6];
						else if ( j > 1 ) string[j+1] = cha_x[i][j+6];
						else  string[j] = cha_x[i][j+6];
					}
					string[j+2] = 0;
					global_strcpy( m_alg_presult[i].c_U, string );
					for ( j = 0;j < 11;j++ )
						string[j] = cha_x[i][j+16];
					string[j] = 0;
					global_strcpy( m_alg_presult[i].c_I, string );
				}
			}
			else
			{
				m_alg_presult->jxmod =dl_flag.phase3or4;
			}
			os_sem_post( m_WIRmod_sem ); 
		}
	}
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线算法计算入口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_alg_entery( void )
{
	global_memcpy( cha_x[0], "                             ", 31 );
	global_memcpy( cha_x[1], "                             ", 31 );
	global_memcpy( cha_x[2], "                             ", 31 );
	global_memcpy( cha_x[3], "                             ", 31 );
	os_sem_creat( &m_WIRmod_sem, 0 );
	os_sem_creat( &m_WIRmod_Resem, 0 );
	m_alg_presult = app_malloc( sizeof( WIR_ALG_t ) * 4 );/*存放3组*/
	if (m_alg_presult )
	{
		m_WIRmod_Cline_task_pro
		= os_task_assign_stk_build( app_WIRmod_check_err_task, 4 );
		os_task_name_set( m_WIRmod_Cline_task_pro, "WIRmod--ALG task" );
		app_global_DL_measure_entery();
	}
	return;
}
/********************************************************************
 * @创建人 :揭成
 * @功能 :查线算法计算出口
 *
 * @输入 :NONE
 *
 *@输出  :NONE
********************************************************************/
void app_WIRmod_alg_exit( void )
{
	app_free( m_alg_presult );
	os_task_delete( m_WIRmod_Cline_task_pro );
	app_global_DL_measure_exit();
	return;
}
