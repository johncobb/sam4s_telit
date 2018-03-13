#ifndef MODEM_DEFS_H_
#define MODEM_DEFS_H_


#include <cph.h>


#define MODEM_SOCKET_IPENDPOINT	"bs.cphandheld.com" // 1888

#define MODEM_DEFAULT_HTTPSERVER	"www.google.com"
// #define MODEM_DEFAULT_HTTPSERVER	"bs.cphandheld.com" // 1888
#define MODEM_DEFAULT_HTTPREQUEST	"GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: keep-alive\r\n\r\n"
#define MODEM_TOKEN_HTTPOK			"HTTP/1.1 200 OK"
#define MODEM_TOKEN_HTTPFORBIDDEN	"HTTP/1.1 403"
#define MODEM_TOKEN_HTTPENDOFFILE	"</html>"

#define MODEM_DEFAULT_SOCKETOPEN_TIMEOUT		20000
#define MODEM_DEFAULT_SOCKETWRITE_TIMEOUT		10000
#define MODEM_DEFAULT_SOCKETSUSPEND_TIMEOUT		5000

// TODO: tunable parameter must change based on S12: MODEM_CMD_SETGUARDTIME
#define MODEM_DEFUALT_ESCAPEGUARD_TIMEOUT		50

#define MODEM_DEFAULT_ATTIMEOUT			1000

// *** modem tokens ***
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

// *** modem at commands ***
#define CR							'\r'
#define LF							'\n'
#define MODEM_CMD_CTRLZ				0x1a
#define MODEM_CMD_ESCAPE			0x1b
#define MODEM_CMD_AT				"AT\r"
#define MODEM_CMD_ATF				"AT&F\r"
#define MODEM_CMD_ECHOOFF			"ATE0\r"
#define MODEM_CMD_SELINT			"AT#SELINT=2\r"
#define MODEM_CMD_ATZ				"ATZ\r"

#define MODEM_CMD_MSGFMT			"AT+CMGF=1\r"
#define MODEM_CMD_MONI				"AT#MONI\r"
#define MODEM_CMD_CFUN				"AT+CFUN=1\r"
#define MODEM_CMD_CREG				"AT+CREG=1\r"
#define MODEM_CMD_GAUTH				"AT#GAUTH=0\r"
#define MODEM_CMD_QUERYNETWORK		"AT+CREG?\r"
#define MODEM_CMD_QUERYSIGNAL		"AT+CSQ?\r"
#define MODEM_CMD_QUERYFIREWALL		"AT#FRWL?\r"
#define MODEM_CMD_DROPFIREWALL      "AT#FRWL=2\r"
#define MODEM_CMD_MOBILEEQUIPERR	"AT+CMEE=2\r"
// #define MODEM_CMD_FIREWALLCFG		"AT#FRWL=1,\"172.18.1.1\",\"255.255.0.0\"\r"
#define MODEM_CMD_FIREWALLCFG		"AT#FRWL=%d,\"%s\",\"%s\"\r"

#define MODEM_CMD_SETCONTEXT		"AT+CGDCONT=1,\"IP\",\"a10.korem2m.com\"\r"
#define MODEM_CMD_DIAL				"ATD*99***1#\r"

//#define MODEM_CMD_SETCONTEXT		"AT+CGDCONT=1,\"IP\",\"c1.korem2m.com\"\r"
#define MODEM_CMD_ACTIVATECONTEXT	"AT#SGACT=1,1\r"
#define MODEM_CMD_QUERYCONTEXT		"AT#SGACT?\r"
#define MODEM_CMD_DEACTCONTEXT		"AT#SGACT=1,0\r"
#define MODEM_CMD_LISTENUDP			"AT#SLUDP=1,1,3500\r"
#define MODEM_CMD_SETGUARDTIME		"ATS12=2\r" // fiftiety of a second so (.02*2)=.04 seconds
#define MODEM_CMD_SKIPESC			"AT#SKIPESC=1\r"
// #define MODEM_CMD_SOCKETCFG			"AT#SCFG=1,1,512,90,600,2\r"
#define MODEM_CMD_SOCKETCFG 		"AT#SCFG=%d,%d,%d,%d,%d,%d\r"
#define MODEM_CMD_AUTOCTX			"AT#SGACTCFG=1,3\r"
#define MODEM_CMD_SENDSMS			"AT+CMGS=\"8126290240\"\r\nHello World\r\n\032"

// socket commands
#define MODEM_CMD_SOCKETOPEN		"AT#SD=%d,%d,%d,\"%s\",0,0\r"
#define MODEM_CMD_SOCKETCLOSE		"AT#SH=%d\r"
#define MODEM_CMD_SOCKETRESUME		"AT#SO=%d\r"
#define MODEM_CMD_SOCKETLISTEN		"AT#SL=%d,%d,%d\r"
#define MODEM_CMD_SOCKETACCEPT		"AT#SA=%d\r"
#define MODEM_CMD_SOCKETSUSPEND		"+++"
#define MODEM_CMD_SOCKETSTATUS		"AT#SS\r"
#define MODEM_CMD_SOCKETLISTEN_EX	"AT#SL=2,1,1337\r"



#define MODEM_CMD_SETBAND			"AT#BND=1\r"	// 850/1900 default
//#define MODEM_CMD_SETCONTEXT		"AT+CGDCONT=1,\"IP\",\"%s\"\r"
#define MODEM_CMD_SETUSERID			"AT#USERID=\"\"\r"
#define MODEM_CMD_SETPASSWORD		"AT#PASSW=\"\"\r"
//#define MODEM_CMD_SETUSERID		"AT#USERID=\"%s\"\r"
//#define MODEM_CMD_SETPASSWORD	"AT#PASSW=\"%s\"\r"

// socket udp commands
// #define MODEM_CMD_UDPSOCKETOPEN		"AT#SD=1,1,%d,\"%s\"\r"
// #define MODEM_CMD_UDPSOCKETCLOSE	"AT#SH=1\r"
// #define MODEM_CMD_UDPSOCKETRESUME	"AT#SO=1\r"
// #define MODEM_CMD_UDPSOCKETSUSPEND	"+++"

// socket http commands
#define MODEM_CMD_HTTPSOCKETOPEN	"AT#SD=1,0,80,\"www.google.com\",0,0\r"
#define MODEM_CMD_HTTPSOCKETCLOSE	"AT#SH=1\r"
#define MODEM_CMD_HTTPSOCKETRESUME	"AT#SO=1\r"
#define MODEM_CMD_HTTPSOCKETSUSPEND	"+++"
#define MODEM_CMD_HTTPSOCKETGET		"GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: keep-alive\r\n\r\n"




// typedef struct
// {
// 	modem_func_t fnc_handler;
// 	uint32_t timeout;
// 	uint8_t retries;
// 	bool waitingreply;
// } at_cmd_t;









#endif