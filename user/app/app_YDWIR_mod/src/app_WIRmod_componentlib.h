#ifndef _PEC_APP_WIRMANEGE_COMP_2010_H
#define _PEC_APP_WIRMANEGE_COMP_2010_H
#include "../../../support\app_support.h"
enum
{
	UHGQ_3_4  = 0,
	UHGQ_3_3,
	METER_POINT,
	IHGQ,
	GROUND,
	ELE_POINT,
	INVALID_COMP = 0xFF
};
extern     const char*m_ELELIB[];
#define   GET_COMPONENT(ID) (m_ELELIB[ID])
#define   GET_COMPONENT_WIDE(ID) (m_ELELIB[ID][0])
#define   GET_COMPONENT_HIGH(ID) (m_ELELIB[ID][1])
/*3-4 电压互感器*/
#define 	GET_UHGQ3_4_X1(x)  		(x+2)
#define 	GET_UHGQ3_4_X2(x)  		(x+21)
#define 	GET_UHGQ3_4_X3(x)  	 	(x+25)
#define 	GET_UHGQ3_4_X4(x)  	 	(x+22+21)
#define 	GET_UHGQ3_4_X5(x)  	 	(x+47)
#define 	GET_UHGQ3_4_X6(x)  	 	(x+48+17)
#define 	GET_UHGQ3_4_Y1_1(y)  		(y)
#define 	GET_UHGQ3_4_Y1_2(y)  		(y+4)
#define 	GET_UHGQ3_4_Y1_3(y)  		(y+8)
#define 	GET_UHGQ3_4_Y2(y)  		(y+m_ELELIB[UHGQ_3_4][1])
/*3-3 电压互感器*/
#define 	GET_UHGQ3_3_X1(x)  (x+2)
#define 	GET_UHGQ3_3_X2(x)  (x+4+m_ELELIB[UHGQ_3_3][0]/2-3)
#define 	GET_UHGQ3_3_X3(x)  (x+2+m_ELELIB[UHGQ_3_3][0]-3)
#define 	GET_UHGQ3_3_Y1(y)  (y+1)
#define 	GET_UHGQ3_3_Y2(y)  (y+m_ELELIB[UHGQ_3_3][1])
/*电表连接点*/
#define 	GET_MP_X1(x)  	(x+2)
#define 	GET_MP_X2(x)  	(x+m_ELELIB[METER_POINT][0]/2)
#define 	GET_MP_X3(x)  	(x+m_ELELIB[METER_POINT][0]-2)
#define 	GET_MP_Y1(y)  	(y)
#define 	GET_MP_Y2(y)  	(y+m_ELELIB[METER_POINT][1]-1)
/*互感器连接点*/
#define 	GET_HGQ_X1(x)  	(x+4)
#define 	GET_HGQ_X2(x)  	(x+2+m_ELELIB[IHGQ][0]-2)
#define 	GET_HGQ_Y(y)  	(y+1)
typedef struct
{
	uint8_t 	type;
	uint16_t 	exp;
	uint16_t x;
	uint16_t y;
}ELEcomponent_t;

#define   		MAX_ELE_COMP_SIZE   20
void 			ELEcomponent_ponit_display( uint16_t x, uint16_t y );
void  			ELEcomponent_display( ELEcomponent_t*pcomp );
void 			ELEcomponent_link( ELEcomponent_t*pcomp1, uint8_t branch1, ELEcomponent_t*pcomp2, uint8_t branch2 );
void 			ELEcomponent_link_EXT( ELEcomponent_t*pcomp1, uint8_t branch1, ELEcomponent_t*pcomp2, uint8_t branch2 );
void 			ELEcomponent_point_link( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 );
ELEcomponent_t*	ELEcomponent_creat( uint16_t x, uint16_t y, uint8_t type, uint16_t exp );
void 			ELEcomponent_destory_all( void );
void 			ELEcomponent_destory( ELEcomponent_t*pcomp );
void 			ELEcomponent_maneger_init( void );
#endif
