#ifndef   _2010_PEC_GLOBAL_GUI_CONFIG_H
#define   _2010_PEC_GLOBAL_GUI_CONFIG_H
#define             PECGUI_VER				    102  //V1.02
#define  			GUI_CORLOR_BIT    			16
/*GUI ģ������*/
#define             GUI_ARROW_EN                0/*�Ƿ�����������*/
#define 			GUI_CSR_EN                  1/*�Ƿ�����������*/
#define 			GUI_WIDGET_FORM_EN     		0/*�Ƿ���������ؼ�*/
#define 			GUI_WIDGET_EDIT_EN       	1/*�Ƿ����ñ༭��ؼ�*/
#define         	GUI_WIDGET_MSGBOX_EN   		1/*�Ƿ�������Ϣ�Ի���*/
#define             GUI_BUTTON_EN               1/*�Ƿ����ð����ؼ�*/
#define             GUI_TXTBOX_EN               1/*�Ƿ������ı���ؼ�*/
#define             GUI_SCROLLBAR_EN            1/*�Ƿ����ù������ؼ�*/
#define             GUI_SLIDER_EN               1/*�Ƿ����û�����*/
#define             GUI_3D_WINDOW_EN            1/*�Ƿ�����windows���ڿؼ�*/
#define             GUI_PROGRESS_BAR_EN         1/*������*/
#define   			GUI_CHECK_BOX_EN       		1/*��ѡ��*/
#define   			GUI_RADIO_EN       			1/*��ѡ��*/
#define   			GUI_DROP_DOWN_LIST_EN       1/*�����˵�*/
#define   			GUI_LISTVIEW_EN       		1
#define   			GUI_DIALOG_EN				1
#define             GUI_TOUCH_EN                0/*�Ƿ����ô�����*/
#define             GUI_LIBCONNECT_EN           1/*�Ƿ����ñ����Զ����*/
#define             GUI_WIDGET_3D_EN            1/*�Ƿ����ؼ�3DЧ��*/
/*LCD �ߴ�����*/
#define             GUI_LOOP_SIZE               10
#define             LCD_WIDE                    642
#define             LCD_HIGH                    480
#define  			GUI_LCD_WIDE        		LCD_WIDE
#define  			GUI_LCD_HIGH        		LCD_HIGH
#define  			GUI_DIS_WIDE        		(GUI_LCD_WIDE-7)
#define  			GUI_DIS_HIGH        		(GUI_LCD_HIGH-10)
#define             GUI_WIN_X                   3
#define             GUI_WIN_Y                   0
#define   			HZ_WIDE               		24
#define   			HZ_HIGH               		24
#define 			BORGAP               		1

/*��ɫ����*/
#define C_GUI_BlACK  		0x0000
#define C_GUI_LOW_BLUE  	13
#define C_GUI_HIGH_BLUE     31
#define C_GUI_BLUE    		17
#define C_GUI_WHITE			0xffff
#define C_GUI_YELLOW 		65504
#define C_GUI_GREEN   		2016
#define C_GUI_RED 			63488
#define C_GUI_NAVY          0x0010
#define C_GUI_TEAL       	0x0410
#define C_GUI_MAROON     	0x8000
#define C_GUI_PURPLE     	0x8010
#define C_GUI_OLIVE      	0x8400
#define C_GUI_GRAY       	0x8410
#define C_GUI_SILVER     	0xC618
#define C_GUI_LIME       	0x07E0
#define C_GUI_AQUA       	0x07FF
#define C_GUI_FUCHSIA    	0xF81F
#define C_GUI_ORANGE	 	0xfc00
#define C_GUI_DARK_GREEN 	0x2a40
#define C_GUI_DARK_GRAY	 	0x4208
#define	C_GUI_DARK_BLUE	 	0x0018


#define C_GUI_DLA_COLOR     C_GUI_YELLOW  /*������ҵ�̶���ɫ*/
#define C_GUI_DLB_COLOR     C_GUI_GREEN   /*������ҵ�̶���ɫ*/
#define C_GUI_DLC_COLOR     C_GUI_RED      /*������ҵ�̶���ɫ*/
/*�ؼ���ɫ����*/
#define   EDIT_FRAME_COLOR              C_GUI_FG_COLOR
#define   EDIT_BG_COLOR   			 	C_GUI_HIGH_BLUE

#define   FORM_FRAME_COLOR   			C_GUI_WHITE
#define   FORM_BG_COLOR   			    C_GUI_BlACK

#define   MSGBOX_FRAME_COLOR         	C_GUI_WHITE
#define   MSGBOX_TITLE_BG_COLOR         C_GUI_LOW_BLUE// C_GUI_HIGH_BLUE//	14
#define   MSGBOX_BG_COLOR           	C_GUI_WHITE-1//10
#define   MSGBOX_FG_COLOR           	C_GUI_HIGH_BLUE



#define   BUTTON_BG_COLOR    			0x841F//(0xf714)
#define   BUTTON_FRAME_COLOR   			C_GUI_BlACK
#define   BUTTON_TXT_COLOR       		C_GUI_BlACK


#define   PROGRESS_BAR_BG_COLR      	C_GUI_LOW_BLUE
#define   PROGRESS_BAR_FG_COLR      	C_GUI_WHITE
#define   PROGRESS_BAR_STEP_COLR    	C_GUI_GREEN

/*��������*/
/*********************************************************************
*�û����Ͷ���
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

typedef 		uint16_t                Corlor_t;
typedef 		uint16_t                BLKmem_t;  

#ifndef         TYPE_EXPEND
#define         TYPE_EXPEND
#define  		room_t      		const
typedef  	    uint16_t    		Bsize_t;
typedef 		uint8_t    			BOOL;
typedef 		uint32_t			pint;      /*ָ���������γ���*/
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
