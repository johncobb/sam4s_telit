#ifndef TELIT_H_
#define TELIT_H_

#include <cph.h>
#include "modem_defs.h"

typedef enum {
    TCPIP = 0,
    UDP
} socket_protocol_t;

typedef enum {
    FW_REMOVE = 0,
    FW_ACCEPT,
    FW_DROP
} firewall_action;


typedef struct
{
    uint8_t connection_id;
    uint8_t cid;
    uint16_t packet_size;
    uint16_t  max_to;
    uint16_t conn_to;
    uint16_t tx_to;
} socket_config_t;

typedef struct
{
    uint8_t connection_id;
    socket_protocol_t protocol;
    uint16_t port;
	char *address;
} modem_socket_t;

typedef struct
{
    firewall_action action;
    char *ip_address;
    char *net_mask;
} firewall_entry_t;

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
void modem_socketconfig(socket_config_t config);
void modem_firewallcfg(firewall_entry_t entry);



void modem_querycontext(void);
void modem_autocontext(void);
void modem_activatecontext(void);
void modem_querynetwork(void);
void modem_mobileequiperr(void);
void modem_querysignal(void);
void modem_queryfirewall(void);
void modem_dropfirewall(void);
void modem_mobileequiperr(void);



void modem_socketopen(modem_socket_t socket);
void modem_socketlisten(modem_socket_t socket);
void modem_socketclose(modem_socket_t socket);
void modem_socketresume(modem_socket_t socket);
void modem_socketsuspend(void);
void modem_socketsend(char * cmd);
void modem_socketstatus(modem_socket_t socket);
void modem_socketaccept(modem_socket_t socket);


#endif