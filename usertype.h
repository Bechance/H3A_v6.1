#ifndef USER_TYPE_H_2010
#define  USER_TYPE_H_2010
/*********************************************************************
*用户类型定义
**********************************************************************/
#ifndef         uint8_t
typedef 		unsigned char    		uint8_t;
#define         uint8_t               	unsigned char
#endif
#ifndef         uint16_t
typedef 		unsigned int      		uint16_t;
#define         uint16_t              	unsigned int
#endif
#ifndef         uint32_t
typedef 		unsigned long       	uint32_t;
#define         uint32_t             	unsigned long
#endif
#ifndef         uint64_t
typedef 		unsigned long long    	uint64_t;
#define         uint64_t              	unsigned long long
#endif
#ifndef         int8_t
typedef 		char    				int8_t;
#define         int8_t            	 	char
#endif
#ifndef         int16_t
typedef 		int      		   		int16_t;
#define         int16_t                 int
#endif

#ifndef         int32_t
typedef 		 long	        		int32_t;
#define         int32_t                	long
#endif

#ifndef         int64_t
typedef 		long long	        	int64_t;
#define         int64_t                	long long
#endif


#ifndef         TYPE_EXPEND
#define         TYPE_EXPEND
#define  		room_t      		const
typedef  	    uint16_t    		Bsize_t;
typedef 		uint8_t    			BOOL;
typedef 		uint32_t			pint;      /*指针数据整形长度*/
enum
{
	FAIL = 0,
	TURE = 1,
	IDLE = 0,
	BUSY = 1,
	PNULL = 0,
	ASCII_NULL = '\0'
};
#endif
#endif

