#ifndef TELIT_H_
#define TELIT_H_

#include <cph.h>
#include "modem_defs.h"


void modem_factory(void);
void modem_echooff(void);
void modem_setinterface(void);
void modem_setmsgformat(void);
void modem_setband(void);
void modem_setcontext(void);
void modem_setuserid(void);
void modem_setpassword(void);
void modem_setguardtime();
void modem_skipesc(void);
void modem_socketconfig(void);


void modem_querycontext(void);
void modem_autocontext(void);
void modem_activatecontext(void);
void modem_querynetwork(void);
void modem_mobileequiperr(void);
void modem_querysignal(void);
void modem_mobileequiperr(void);


void modem_udpsocketopen(void);
void modem_udpsocketclose(void);
void modem_udpsocketresume(void);
void modem_udpsocketsuspend(void);
void modem_udpsocketsend(uint8_t * cmd);


#endif