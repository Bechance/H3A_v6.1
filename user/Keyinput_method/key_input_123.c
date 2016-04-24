#include 		"key_input method.h"

int key_method_123( uint8_t code_0,char*buf )
{
	if ( !g_key_input_keep_secret )
		buf[0] =  app_global_decide_inputkey(code_0);
	else
		buf[0] = '*';
	return 1;
}
