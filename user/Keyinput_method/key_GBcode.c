#include 		"key_input method.h"
#define  		MBLIB_INFO_NAME     		"MBINFO"
#define  		MBLIB_FILE_NAME     		"MABIAOU.TXT"
#define         MAX_HZ_NUM                  128

uint8_t  		py_Head_aToz_NUm[26];

char  			PY_hzboxf[MAX_HZ_NUM*2];
int   			PY_nMax=0;


const char* PY_index_a[]={

	"","i","n","ng","o"
};
const char* PY_index_b[]={

	"a","ai","an","ang","ao",
	"ei","en","eng","i","ian",
	"iao","ie","in","ing","o","u"
};
const char* PY_index_c[]={

	"a","ai","an","ang","ao","e","eng","ha","hai",
	"han","hang","hao","he","hen","heng","hi","hong",
	"hou","hu","huai","huan","huang","hui","hun","huo",
	"i","ong","ou","u","uan","ui","un","uo"
};
const char* PY_index_d[]={

	"a","ai","an","ang","ao","e","eng",
	"i","ian","iao","ie","ing","iu","ong",
	"ou","u","uan","ui","un","uo"
};
const char* PY_index_e[]={

	"","n","r"
};
const char* PY_index_f[]={

	"a","an","ang","ei",
	"en","eng","o","ou","u"
};
const char* PY_index_g[]={

	"a","ai","an","ang","ao","e","ei",
	"en","eng","ong","ou","u","ua","uai",
	"uan","uang","ui","un","uo"
};
const char* PY_index_h[]={

	"a","ai","an","ang","ao","e","ei","en",
	"eng","ong","ou","u","ua","uai","uan",
	"uang","ui","un","uo"
};
const char* PY_index_j[]={

	"i","ia","ian","iang","iao","ie","in",
	"ing","iong","iu","u","uan","ue","un"
};
const char* PY_index_k[]={

	"a","ai","an","ang","ao","e","en",
	"eng","ong","ou","u","ua","uai",
	"uan","uang","ui","un","uo"
};
const char* PY_index_l[]={

	"a","ai","an","ang","ao","e","ei",
	"eng","i","ian","iang","iao","ie",
	"in","ing","iu","ong","ou","u",
	"uan","ue","un","uo","v"
};
const char* PY_index_m[]={

	"a","ai","an","ang","ao","e","ei",
	"en","eng","i","ian","iao","ie","in",
	"ing","iu","o","ou","u"
};
const char* PY_index_n[]={

	"a","ai","an","ang","ao","e",
	"ei","en","eng","i","ian","iang",
	"iao","ie","in","ing","iu","ong",
	"u","uan","ue","uo","v"
};
const char* PY_index_o[]={

	"","u"
};
const char* PY_index_p[]={

	"a","ai","an","ang","ao","ei",
	"en","eng","i","ian","iao","ie",
	"in","ing","o","ou","u"
};
const char* PY_index_q[]={

	"i","ia","ian","iang","iao","ie","in",
	"ing","iong","iu","u","uan","ue","un"
};
const char* PY_index_r[]={

	"an","ang","ao","e","en","eng","i","ong",
	"ou","u","uan","ui","un","uo"
};
const char* PY_index_s[]={

	"a","ai","an","ang","ao","e","en","eng",
	"ha","hai","han","hang","hao","he","hen",
	"heng","hi","hou","hu","hua","huai","huan",
	"huang","hui","hun","huo","i","ong","ou","u",
	"uan","ui","un","uo"
};
const char* PY_index_t[]={
	"a","ai","an","ang","ao","e","eng","i","ian","iao",
	"ie","ing","ong","ou","u","uan","ui","un","uo"
};
const char* PY_index_w[]={

	"a","ai","an","ang","ei",
	"en","eng","o","u"
};
const char* PY_index_x[]={

	"i","ia","ian","iang","iao","ie","in",
	"ing","iong","iu","u","uan","ue","un"
};
const char* PY_index_y[]={
	"a","an","ang","ao","e","i","in",
	"ing","o","ong","ou","u","uan","ue","un"
};
const char* PY_index_z[]={

	"a",   "ai",  "an","ang","ao","e","ei","en",
	"eng", "ha",  "hai","han","hang","hao","he",
	"hen", "heng","hi","hong","hou","hu","hua",
	"huai","huan","huang","hui","hun","huo","i",
	"ong", "ou",  "u","uan","ui","un","uo"
};

void PY_num_count(void)
{
#define UINT_SIZE     sizeof(const char*)
	uint8_t ofs = 0;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_a ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_b ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_c ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_d ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_e ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_f ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_g ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_h ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = 0;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_j ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_k ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_l ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_m ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_n ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_o ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_p ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_q ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_r ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_s ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_t ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = 0;
	py_Head_aToz_NUm[ofs++] = 0;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_w ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_x ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_y ) / UINT_SIZE;
	py_Head_aToz_NUm[ofs++] = sizeof( PY_index_z ) / UINT_SIZE;
    PY_nMax = 0;
	for(ofs=0;ofs<26;ofs++)
      	PY_nMax +=py_Head_aToz_NUm[ofs];
	return;
}

const char**PY_path[]=
{
	PY_index_a, PY_index_b, PY_index_c, PY_index_d, PY_index_e, PY_index_f, PY_index_g, PY_index_h,
	PY_index_j, PY_index_j, PY_index_k, PY_index_l, PY_index_m, PY_index_n, PY_index_o, PY_index_p,
	PY_index_q, PY_index_r, PY_index_s, PY_index_t, PY_index_w, PY_index_w, PY_index_w, PY_index_x,
	PY_index_y, PY_index_z
};


static void py_MB_OB( uint16_t ofs,char*HZbox )
{
	uint32_t flseek;
	Bsize_t len;
	MB_member_t temp;
	FRESULT ret;
	UINT r;
    FIL*Pfile=app_malloc(sizeof(FIL));
	if(!Pfile)return;
	if ( ofs >= 396 ) return;
    ret=f_open(Pfile,MBLIB_INFO_NAME,FA_OPEN_EXISTING|FA_READ);
	if(ret!=FR_OK) return;   
	flseek = sizeof(MB_member_t)*ofs;
	GLOBAL_MEMCLR( &temp, sizeof( MB_member_t ) );
	f_lseek(Pfile,flseek);
	f_read(Pfile,&temp,sizeof( MB_member_t ),&r);
	f_close(Pfile);
	flseek = global_atoi( temp.addr );
	len = global_atoi( temp.size );
    ret=f_open(Pfile,MBLIB_FILE_NAME,FA_OPEN_EXISTING|FA_READ);
	if(ret!=FR_OK) return;  
	f_lseek(Pfile,flseek);
	f_read(Pfile,HZbox,len,&r);
	f_close(Pfile);
	app_free(Pfile);
	return;
}


static int py_offset_OB(char*str)
{
	int   xof,yof,strlen;
	int   offset = 0;
	int   ofs;
	char  nch[8]="";
	char**p;
	PY_num_count();
    xof = str[0]-'a';
    p = (char**)PY_path[xof];
    strlen = global_strlen(str);
	for(yof=0;yof<py_Head_aToz_NUm[xof];yof++)
	{
	    nch[0] = str[0];nch[1] = 0;
		global_strcat(nch,p[yof]);
	    if(strlen>global_strlen(nch))continue;
	    if(!global_memcmp(str,nch,strlen))continue;
		break;
	}
	for(ofs=0;ofs<xof;ofs++)
      	offset+=py_Head_aToz_NUm[ofs];
	offset+=yof;
    return offset;
}


uint8_t*py_ime( uint8_t *input_py_val )
{
    int  offset;
	GLOBAL_MEMCLR(PY_hzboxf,sizeof(PY_hzboxf));
    if(input_py_val[0] == 0)   return  (uint8_t*)PY_hzboxf;
    if(input_py_val[0] == 'i') return  (uint8_t*)PY_hzboxf;
    if(input_py_val[0] == 'u') return  (uint8_t*)PY_hzboxf;
    if(input_py_val[0] == 'v') return  (uint8_t*)PY_hzboxf;
	offset=py_offset_OB((char*)input_py_val);
	if(offset<PY_nMax)
	{
		py_MB_OB(offset,PY_hzboxf);
	}
	return (uint8_t*)PY_hzboxf;
}
