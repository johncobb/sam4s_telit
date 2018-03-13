#include "telit.h"
#include "modem.h"


void modem_factory(void)
{
	modem_write(MODEM_CMD_ATZ);
}

void modem_echooff(void)
{
	modem_write(MODEM_CMD_ECHOOFF);
}

void modem_setinterface(void)
{
	modem_write(MODEM_CMD_SELINT);
}

void modem_setmsgformat(void)
{
	modem_write(MODEM_CMD_SELINT);
}

void modem_setband(void)
{
	modem_write(MODEM_CMD_SETBAND);
}

void modem_setcontext(void)
{
	modem_write(MODEM_CMD_SETCONTEXT);
}

void modem_setuserid(void)
{
	modem_write(MODEM_CMD_SETUSERID);
}

void modem_setpassword(void)
{
	modem_write(MODEM_CMD_SETPASSWORD);
}

void modem_setguardtime(void)
{
	modem_write(MODEM_CMD_SETGUARDTIME);
}

void modem_skipesc(void)
{
	modem_write(MODEM_CMD_SKIPESC);
}

void modem_socketconfig(socket_config_t config)
{
	/* "AT#SCFG=1,1,512,90,600,2\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETCFG, config.connection_id, config.cid, config.packet_size, config.max_to, config.conn_to, config.tx_to);
	modem_write(buffer);
}

void modem_firewallcfg(firewall_entry_t entry)
{
	/* "AT#FRWL=1,\"172.18.1.1\",\"255.255.0.0\"\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_FIREWALLCFG, entry.action, entry.ip_address, entry.net_mask);

	modem_write(buffer);
}

void modem_querycontext(void)
{
	modem_write(MODEM_CMD_QUERYCONTEXT);
}

void modem_autocontext(void)
{
	modem_write(MODEM_CMD_AUTOCTX);
}

void modem_activatecontext(void)
{
	modem_write(MODEM_CMD_ACTIVATECONTEXT);
}

void modem_querynetwork(void)
{
	modem_write(MODEM_CMD_QUERYNETWORK);
}

void modem_querysignal(void)
{
	modem_write(MODEM_CMD_QUERYSIGNAL);
}

void modem_queryfirewall(void)
{
	modem_write(MODEM_CMD_QUERYFIREWALL);
}

void modem_dropfirewall(void)
{
	modem_write(MODEM_CMD_DROPFIREWALL);
}

void modem_mobileequiperr(void)
{
	modem_write(MODEM_CMD_MOBILEEQUIPERR);
}

void modem_socketlisten(modem_socket_t socket)
{
	/* "AT#SL=2,1,1337\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETLISTEN, socket.connection_id, socket.protocol, socket.port);
	modem_write(buffer);
}

void modem_socketopen(modem_socket_t socket)
{
	/* "AT#SD=1,1,2012,\"vq1.cphandheld.com\"\r" */
    char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETOPEN, socket.connection_id, socket.protocol, socket.port, socket.address);
	modem_write(buffer);
}

void modem_socketclose(modem_socket_t socket)
{
	/* "AT#SH=1\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETCLOSE, socket.connection_id);
	modem_write(buffer);
}

void modem_socketresume(modem_socket_t socket)
{
	/* "AT#SO=1\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETRESUME, socket.connection_id);
	modem_write(buffer);
}
	
void modem_socketsuspend(void)
{
	/* "+++" */
	modem_write(MODEM_CMD_SOCKETSUSPEND);
}

void modem_socketsend(char *cmd)
{
	modem_write(cmd);
}


