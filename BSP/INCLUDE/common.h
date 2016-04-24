void delay( int k );
/////////////////////////////////

#define NULL		0
#define TRUE   		1
#define FALSE  		0

#define BUFFER_LENGTH	8192 //8192
#define MAX_READ_LENGTH 16384//16384
#define MAX_WRITE_LENGTH 2048//2048

//typedef unsigned char       INT8;
//typedef unsigned short      INT16;
//typedef unsigned long       INT32;


typedef union __FLAGS
{
	struct _FLAGS
	{
		Uint16  SLAVE_IS_ATTACHED	;
		Uint16  SLAVE_REMOVED		;
		Uint16  SLAVE_FOUND			;	// Slave USB device found
		Uint16  SLAVE_ENUMERATED	;	// slave USB device enumeration done
		Uint16  SLAVE_ONLINE		;
		Uint16 	TIMEOUT_ERR			;	// timeout error during data endpoint transfer
		Uint16	DATA_STOP			;	// device unplugged during data transfer
		Uint16 	bData1				;
		Uint16 	bUartInDone			;
		Uint16 	bMassDevice			;

	} bits;

} FLAGS;


