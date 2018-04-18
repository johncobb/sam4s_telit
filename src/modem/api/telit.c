#include "telit.h"
#include "modem.h"

#if defined(__arm__)
static const char _tag[] PROGMEM = "telit: "; /* used in embedded gcc */
#else
static const char _tag[] = "telit: ";
#endif

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
	modem_write(MODEM_CMD_MSGFMT);
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

// void modem_socketconfig(socket_config_t config)
// {
// 	/* "AT#SCFG=1,1,512,90,600,2\r" */
// 	char buffer[128] = {0};

// 	sprintf(buffer, MODEM_CMD_SOCKETCFG, config.connection_id, config.cid, config.packet_size, config.max_to, config.conn_to, config.tx_to);
// 	modem_write(buffer);
// }

void modem_socketconfig(socket_config_t *config)
{
	/* "AT#SCFG=1,1,512,90,600,2\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETCFG, config->connection_id, config->cid, config->packet_size, config->max_to, config->conn_to, config->tx_to);
	modem_write(buffer);
}

// void modem_firewallcfg(firewall_entry_t entry)
// {
// 	/* "AT#FRWL=1,\"172.18.1.1\",\"255.255.0.0\"\r" */
// 	char buffer[128] = {0};

// 	sprintf(buffer, MODEM_CMD_FIREWALLCFG, entry.action, entry.ip_address, entry.net_mask);

// 	modem_write(buffer);
// }

void modem_firewallcfg(firewall_entry_t *entry)
{
	/* "AT#FRWL=1,\"172.18.1.1\",\"255.255.0.0\"\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_FIREWALLCFG, entry->action, entry->ip_address, entry->net_mask);

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

// void modem_socketlisten(modem_socket_t socket)
// {
// 	/* "AT#SL=2,1,1337\r" */
// 	char buffer[128] = {0};

// 	sprintf(buffer, MODEM_CMD_SOCKETLISTEN, socket.connection_id, socket.protocol, socket.port);
// 	modem_write(buffer);
// }

void modem_socketlisten(modem_socket_t *socket)
{
	/* "AT#SL=2,1,1337\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETLISTEN, socket->connection_id, socket->port);
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

void modem_socketstatus(modem_socket_t socket)
{
	/* "AT#SS\r" */
	modem_write(MODEM_CMD_SOCKETSTATUS);
}

void modem_socketaccept(modem_socket_t socket)
{
	/* "AT#SA=1\r" */
	char buffer[128] = {0};

	sprintf(buffer, MODEM_CMD_SOCKETACCEPT, socket.connection_id);
	modem_write(buffer);
}

uint8_t modem_handle_default(uint8_t *buffer)
{
	uint8_t *ptr = NULL;
	return modem_parse_event(MODEM_TOKEN_OK, buffer, &ptr);	
}


uint8_t modem_handle_activatecontext(uint8_t *buffer)
{
	uint8_t *ptr = NULL;
	uint8_t result = modem_parse_event(MODEM_TOKEN_SGACT, buffer, &ptr);
		
	/*
	 * https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
	 * Example Response:	#SGACT: 10.117.64.31
	 */		
	if (result > 0) {
		if (!sscanf(ptr, "#SGACT: %3d.%3d.%3d.%3d", &modem_status.ip[0], &modem_status.ip[1], &modem_status.ip[2], &modem_status.ip[3])) {

			LOGE("modem_handle_activatecontext: error parsing event!\r\n");
			return 0;
		}	
	}

	LOG("modem_handle_activatecontext: ip: %d.%d.%d.%d\r\n", modem_status.ip[0], modem_status.ip[1], modem_status.ip[2], modem_status.ip[3]);

	return 1;
}


uint8_t modem_handle_querycontext(uint8_t *buffer)
{
	uint8_t *ptr = NULL;
	uint8_t result = modem_parse_event(MODEM_TOKEN_SGACT, buffer, &ptr);

	/*
	 * https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
	 * Example Response:	#SGACT: 1,0
	 */		
	if (result > 0) {
		if (!sscanf(ptr, "#SGACT: %1d,%1d", &modem_status.context.cid, &modem_status.context.status)) {

			LOGE("modem_handle_querycontext: error parsing event!\r\n");
			return 0;
		}		
	}

	LOG("modem_handle_context: cid: %d status: %d\r\n", modem_status.context.cid, modem_status.context.status);
	
	return 1;
}

uint8_t modem_handle_creg(uint8_t *buffer)
{
	uint8_t *ptr = NULL;
	uint8_t result = modem_parse_event(MODEM_TOKEN_CREG, buffer, &ptr);


	/*
	 * https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
	 * Example Response "+CREG: 0,1,":  Module not registered not searching
	 * Example Response "+CREG: 1,1,":  Registered, home network
	 * Example Response "+CREG: 2,1,":  Not registered but currently searching...
	 */	
	if (result > 0) {
		if (!sscanf(ptr, "+CREG: %1d,%1d", &modem_status.creg.status, &modem_status.creg.access_technology)) {

			LOGE("modem_handle_creg: error parsing event!\r\n");
			return 0;
		}	
	}

	LOG("modem_handle_creg: status: %d access_technology: %d\r\n", modem_status.creg.status, modem_status.creg.access_technology);

	return 1;
}

uint8_t modem_handle_signalstrength(uint8_t *buffer)
{
	uint8_t *ptr = NULL;
	uint8_t result = modem_parse_event(MODEM_TOKEN_CSQ, buffer, &ptr);

	/*
	 * https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
	 * Example Response:	+CSQ: 14,2
	 */
	if (result > 0) {
		if (!sscanf(ptr, "+CSQ: %2d,%2d", &modem_status.signal.rssi, &modem_status.signal.ber)) {

			LOGE("modem_handle_signalstrength: error parsing event!\r\n");
			return 0;
		}
	}

	LOG("modem_handle_signalstrength: rssi: %d ber: %d\r\n", modem_status.signal.rssi, modem_status.signal.ber);

	return 1;
}



