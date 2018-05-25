#ifndef MODEM_H_
#define MODEM_H_

#include <cph.h>
#include "usart.h"
#include "modem_defs.h"
#include "telit.h"

#define MODEM_RX_BUFFER_SIZE    128
#define MODEM_BAUD_RATE         115200
#define TOKEN_END '\n'

typedef void (*modem_ondatareceive_func_t)(uint8_t *buffer, uint32_t len);
typedef void (*modem_oneventreceive_func_t) (uint8_t *buffer, uint32_t len);
typedef void (*modem_connect_func_t)(void);
typedef void (*modem_ondisconnect_func_t)(void);

typedef struct{
    modem_ondatareceive_func_t on_datareceive;
    modem_oneventreceive_func_t on_eventreceive;
    modem_connect_func_t on_connect;
    modem_ondisconnect_func_t on_disconnect;
} modem_event_handler_t;

typedef enum{
    SYS_OK = 0,
    SYS_AT_OK,
    SYS_AT_CONNECT,
    SYS_MODEM_TIMEOUT,
    SYS_MODEM_NOCARRIER,
    SYS_MODEM_ERROR,
    SYS_MODEM_FAIL
} sys_result;

typedef enum {
    EVT_WAITING = 0,
    EVT_UNKNOWN,
    EVT_OK,
    EVT_ERROR,
    EVT_CONNECT,
    EVT_NOCARRIER,
    EVT_PROMPT,
    EVT_CMGS,
    EVT_CREG,
    EVT_CSQ,
    EVT_MONI,
    EVT_GPRS_ACT,
    EVT_SGACT,
    EVT_CMGL,
    EVT_CMGR,
    EVT_SOCKETSTATUS,
    EVT_SOCKETTATUS_ID,
    EVT_TIMEOUT,
    EVT_REMOTECMD,
    EVT_RING,
    EVT_ACK
} modem_event_t;

typedef void (*modem_func_t) (void);
// typedef void (*modem_eventhandler_func_t) (uint8_t *);
typedef uint8_t (*modem_eventhandler_func_t) (uint8_t *);

typedef struct
{
	modem_func_t fnc_handler;
	uint32_t timeout;
	uint8_t retries;
	bool waitresp;
    modem_event_t result;
    modem_eventhandler_func_t fnc_eventhandler;
} at_cmd_t;


sys_result modem_init(void);

void modem_set_ondatareceive_func(modem_ondatareceive_func_t fnc);
void modem_set_oneventreceive_func(modem_oneventreceive_func_t fnc);

void modem_tick(void);
void modem_write(char *cmd);
void modem_close(void);

sys_result modem_data_handler(uint8_t *, uint32_t);
modem_event_t modem_identify_event(uint8_t *);

uint8_t modem_data_available(void);
uint8_t modem_data_read(void);
uint8_t modem_get_eventlist_size(void);

uint8_t modem_parse_event(uint8_t *, uint8_t *, uint8_t **);


typedef struct {
    char *token;
    modem_event_t event;
    modem_eventhandler_func_t fnc_handler;
} modem_event_list_t;

typedef struct {
    int rssi;
    int ber;
} modem_signal_t;

typedef struct {
    uint8_t status;
    uint8_t access_technology;
} modem_creg_t;

typedef struct {
    uint8_t cid;
    uint8_t status;
} modem_context_t;

typedef struct
{
    int ip[4];
	uint8_t busy;
    modem_context_t context;
    modem_signal_t signal;
    modem_creg_t creg;
} modem_status_t;

extern modem_status_t modem_status;

typedef struct 
{
    int hh;
    int mm;
    int ss;
} gps_utc_t;

typedef struct
{
    gps_utc_t utc;  /* UTCtime (hhmmss.sss) (referenced to GGA sentence) */
    float lat;  /* Latitude ddmm.mmmm N/S (referenced to GGA sentence) */
    float lng;  /* Longitude dddmm.mmmm E/W (referenced to GGA sentence) */
    int hdop;   /* Horizontal Dilution of Precision */
    int alt;    /* Altitude mean-sea-level in meters */
    int fix;    /* 0 or 1 Invalid or Valid Fix */
    int cog;    /* Course over ground */
    int spkm;   /* Speed over ground Km/Hr */
    int spkn;   /* Speed over ground Kt/Hr */
    int date;   /* Date of fix ddmmyy (referenced to RMC Sentence) */
    int nsat;   /* Total number of satellites in use */
} gps_position_t;

extern gps_position_t gps_position;

typedef enum {
    GPS_RST_FACTORY = 1,
    GPS_RST_COLDSTART,
    GPS_RST_WARMSTART,
    GPS_RST_HOTSTART
} gps_reset_type_t;

typedef enum {
    GPS_POWERDOWN = 0,
    GPS_POWERUP
} gps_powermgt_t;


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
#define MODEM_TOKEN_RING            "SRING: %d"
#define MODEM_TOKEN_CMEERROR        "+CME ERROR:"

#endif