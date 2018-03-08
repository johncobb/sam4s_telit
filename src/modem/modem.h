#ifndef MODEM_H_
#define MODEM_H_

#include <cph.h>
#include "usart.h"
#include "modem_defs.h"

#define MODEM_RX_BUFFER_SIZE    128
#define MODEM_BAUD_RATE         115200
#define TOKEN_END '\n'

typedef void (*modem_ondatareceive_func_t)(char *);
typedef void (*modem_connect_func_t)(void);
typedef void (*modem_ondisconnect_func_t)(void);

typedef struct{
    modem_ondatareceive_func_t on_datareceive;
    modem_connect_func_t on_connect;
    modem_ondisconnect_func_t on_disconnect;
} modem_event_handler_t;

typedef enum{
    SYS_OK = 0,
    SYS_AT_OK,
    SYS_MODEM_TIMEOUT,
    SYS_MODEM_NOCARRIER,
    SYS_MODEM_ERROR,
    SYS_MODEM_FAIL
} sys_result;

typedef void (*modem_func_t) (void);

typedef struct
{
	modem_func_t fnc_handler;
	uint32_t timeout;
	uint8_t retries;
	bool waitresp;
	sys_result result;
} at_cmd_t;


sys_result modem_init(void);

void modem_set_ondatareceive_func(modem_ondatareceive_func_t fnc);

void modem_tick(void);
void modem_write(char *cmd);
void modem_close(void);
sys_result modem_data_handler(char *data);


uint8_t modem_data_available(void);
uint8_t modem_data_read(void);







#define MODEM_TOKEN_OK				"OK"
#define MODEM_TOKEN_ERROR			"ERROR"
#define MODEM_TOKEN_CONNECT			"CONNECT"
#define MODEM_TOKEN_NOCARRIER		"NO CARRIER"
#define MODEM_TOKEN_PROMPT			">"
#define MODEM_TOKEN_CMGS			"+CMGS:"
#define MODEM_TOKEN_CREG			"+CREG:"
#define MODEM_TOKEN_CSQ				"+CSQ:"
#define MODEM_TOKEN_MONI			"#MONI:"
#define MODEM_TOKEN_GPRS_ACT		"+IP:"
#define MODEM_TOKEN_SGACT			"#SGACT:"
#define MODEM_TOKEN_ACK				"ACK"
#define MODEM_TOKEN_CMGL			"+CMGL:"
#define MODEM_TOKEN_CMGR			"+CMGR:"
#define MODEM_TOKEN_SOCKETSTATUS	"#SS:"
#define MODEM_TOKEN_SOCKETSTATUS_ID	"#SS: %d"
#define MODEM_TOKEN_REMOTECMD		"$"

#endif