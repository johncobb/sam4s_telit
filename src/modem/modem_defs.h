#ifndef MODEM_DEFS_H_
#define MODEM_DEFS_H_


#include <cph.h>

/*
 * Do this to get connected to a socket from terminal
 * ATZ
 * AT#SELINT=2
 * AT+CMGF=1
 * AT+CGDCONT=1,"IP","c1.korem2m.com" or "11583.mcs" for telit
 * AT#SCFG=1,1,512,90,600,2
 * ATS12=2
 * AT#SKIPESC=1
 * AT+CMEE=2
 * AT#SGACT=1,1
 * AT#SD=1,0,80,"www.google.com",0,0,1
 * AT#FRWL=1,"172.18.1.1","255.255.0.0"
 * AT+CPSMS Power Savings Mode
 * AT+CEDRXS Extended Discontinuous Reception
 */

 // Return loss of 20db 1% of signal reflected
 // Return loss of 10db 10% of signal reflected
 // Return loss of 3db  50% of signal reflected


#define MODEM_CMEE_LOGLEVEL         1                   /* 0 - Error, 1 - Numeric Format, 2 - Verbose Format */
#define MODEM_PDPCONTEXT_ID         1
#define MODEM_PDPCONTEXT_TYPE       "IP"
#define MODEM_PDPCONTEXT_APN        "11583.mcs"         /* Telit Public */
// #define MODEM_PDPCONTEXT_APN        "a10.korem2m.com"   /* Kore VPN */
// #define MODEM_PDPCONTEXT_APN        "c1.korem2m.com"    /* Kore Public */

#define MODEM_IPENDPOINT	        "bs.cphandheld.com"

#define MODEM_HTTPSERVER	        "www.google.com"
#define MODEM_HTTPREQUEST	        "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: keep-alive\r\n\r\n"
#define MODEM_TOKEN_HTTPOK			"HTTP/1.1 200 OK"
#define MODEM_TOKEN_HTTPFORBIDDEN	"HTTP/1.1 403"
#define MODEM_TOKEN_HTTPENDOFFILE	"</html>"

#define MODEM_SOCKETOPEN_TIMEOUT		20000
#define MODEM_SOCKETWRITE_TIMEOUT		10000
#define MODEM_SOCKETSUSPEND_TIMEOUT		5000
// TODO: tunable parameter must change based on S12: MODEM_CMD_SETGUARDTIME
#define MODEM_ESCAPEGUARD_TIMEOUT		50
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
#define MODEM_CMD_QUERYSIGNAL		"AT+CSQ\r"
#define MODEM_CMD_QUERYFIREWALL		"AT#FRWL?\r"
#define MODEM_CMD_DROPFIREWALL      "AT#FRWL=2\r"
#define MODEM_CMD_MOBILEEQUIPERR	"AT+CMEE=%d\r"
#define MODEM_CMD_FIREWALLCFG		"AT#FRWL=%d,\"%s\",\"%s\"\r"
#define MODEM_CMD_DIAL				"ATD*99***1#\r"
#define MODEM_CMD_SETCONTEXT        "AT+CGDCONT=%d,\"%s\",\"%s\"\r"
#define MODEM_CMD_SETBAND			"AT#BND=1\r"	// 850/1900 default
#define MODEM_CMD_SETUSERID			"AT#USERID=\"\"\r"
#define MODEM_CMD_SETPASSWORD		"AT#PASSW=\"\"\r"

/* Power Savings Mode (Requested Periodic-TAU)
 * AT+CPSMS=<mode>,,,"<T3324>","<T3412>"
 * T3412 - Timer Duration (how long we'll be asleep)
 * Binary:
 * 000 10 minutres
 * 001 1 hour
 * 010 10 hours
 * 011 2 seconds
 * 100 30 seconds
 * 101 1 minute
 * 110 320 hours
 * 111 deactivated
 * 
 * Example: 10100010 = decimal 162 = 2 minutes
 * AT+CPSMS=1,,,\"10100010\",\"00100001\"\r"
 * 
 * Power Savings Mode (Requested Active-Time)
 * AT+CPSMS=<mode>,,,"<T3324>","<T3412>"
 * T3324 - Time Until PSM
 * Binary:
 * 000 2 seconds
 * 001 1 minute
 * 010 decihours or 6 minutes
 * 111 deactivated
 * 
 * Example: 00100001 = decimal 33 = 1 minute
 * AT+CPSMS=1,,,\"10100010\",\"00100001\"\r"
 */
#define MODEM_CMD_DISABLEPSM		"AT+CPSMS=0\r"
#define MODEM_CMD_ENABLEPSM		    "AT+CPSMS=1,,,\"10100010\",\"00100001\"\r"

#define MODEM_CMD_ACTIVATECONTEXT	"AT#SGACT=1,1\r"

#define MODEM_CMD_QUERYCONTEXT		"AT#SGACT?\r"
#define MODEM_CMD_DEACTCONTEXT		"AT#SGACT=1,0\r"
#define MODEM_CMD_SKIPESC			"AT#SKIPESC=1\r"	
#define MODEM_CMD_SOCKETCFG 		"AT#SCFG=%d,%d,%d,%d,%d,%d\r"
#define MODEM_CMD_SETGUARDTIME		"ATS12=2\r" // fiftieth of a second so (.02*2)=.04 seconds
#define MODEM_CMD_AUTOCTX			"AT#SGACTCFG=1,3\r"
#define MODEM_CMD_SENDSMS			"AT+CMGS=\"8126290240\"\r\nHello World\r\n\032"

// socket commands
#define MODEM_CMD_SOCKETOPEN		    "AT#SD=%d,%d,%d,\"%s\",0,0\r"
#define MODEM_CMD_SOCKETCLOSE		    "AT#SH=%d\r"
#define MODEM_CMD_SOCKETRESUME		    "AT#SO=%d\r"
#define MODEM_CMD_SOCKETLISTEN		    "AT#SL=%d,1,%d\r"
#define MODEM_CMD_SOCKETACCEPT		    "AT#SA=%d\r"
#define MODEM_CMD_SOCKETSUSPEND		    "+++"
#define MODEM_CMD_SOCKETSTATUS		    "AT#SS\r"
#define MODEM_CMD_SOCKETSTATUS_CONNID   "AT#SS=%d\r"
#define MODEM_CMD_SOCKETINFO            "AT#SI\r"
#define MODEM_CMD_SOCKETINFO_CONNID     "AT#SI=%d\r"
#define MODEM_CMD_SOCKETTYPE            "AT#ST\r"
#define MODEM_CMD_SOCKETTYPE_CONNID     "AT#ST=%d\r"
#define MODEM_CMD_LISTENUDP			"AT#SLUDP=1,1,3500\r"

/*
 * 3GPP TS 27.007 AT Commands
 * 5.4.1 General
 */

#define MODEM_CMD_MANUFACTURERID   "AT+CGMI\r"
#define MODEM_CMD_MODELID          "AT+CGMM\r"
#define MODEM_CMD_REVISIONID       "AT+CGMR\r"
#define MODEM_CMD_SERIALNUMBER     "AT+CGSN\r"

/*
 * 5.1.4.3 Network Service Handling
 */
#define MODEM_CMD_SUBSCRIBERNUM       "AT+CNUM\r"
#define MODEM_CMD_OPERATORNAMES       "AT+COPN\r"
#define MODEM_CMD_NETWORKREGSATUS     "AT+CREG?\r"
#define MODEM_CMD_OPERATORSELECTION   "AT+COPS?\r"

/*
 * 5.1.4.6 Commands for GPRS
 */
#define MODEM_CMD_MOBILESTATIONCLASS  "AT+CGCLASS?\r"
#define MODEM_CMD_GPRSSTATE           "AT+CGATT?\r"
#define MODEM_CMD_GPRSSTATE           "AT+CGATT?\r"

/*
 * Battery Charger
 */
#define MODEM_CMD_BATTERYCHARGE        "AT+CBC\r"


#endif