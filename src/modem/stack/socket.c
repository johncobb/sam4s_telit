#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "socket.h"

/* Keep track of our current socket pool index */
int _socket_pool_index = -1;

/* Initialize our socket pool as array of NULL sockets */
socket_t* socket_pool[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

/*
 * Reserve the next socket in the pool and assign the socket variable a pointer
 * to that entry in the array. Assign the event handling and other configuration
 * variables that are required to establish a connection. 
 */
socket_status_t socket_reserve(socket_t *socket)
{
    /*
     * Make sure we don't overflow the socket pool.
     */
    if (_socket_pool_index++ == (SOCKET_POOL_LEN-1)) {
        printf("socket_reserve: max socket connections reached!\r\n");
        _socket_pool_index = 5;
        socket = NULL;
        return SCK_ERR_MAX_POOL;
    }

    /*
     * Grab the next socket and assign state and event handler variables.
     * Assign the socket variable to the socket indexed in the array.
     */
    printf("socket_reserve: _socket_pool_index %d\r\n", _socket_pool_index);
    socket_pool[_socket_pool_index] = socket;

    return SCK_SUCCESS;
    
}

/*
 * The socket_new function is called to reserve a new socket connection with the modem. 
 * When a request to reserve a new socket is made: protocol, address, port and firewall settings are assigned.
 * These varialbes are used to configure the socket connectivity.
 */ 
socket_status_t socket_new(socket_t *socket)
{
    /*
     * Reserve next socket from the pool
     */
    socket_status_t status = socket_reserve(socket);

    if (status == SCK_SUCCESS) {
        socket->modem_socket->connection_id = _socket_pool_index;
    } 

    return status;
}

void log_socket_settings(socket_t *socket)
{
    printf("*** socket configuration ***:\r\n");
    printf(" - connection_id: %d\r\n", socket->modem_socket->connection_id);
    printf(" - protocol: %d (0:tcpip 1:udp)\r\n", socket->modem_socket->protocol);
    printf(" - port: %d\r\n", socket->modem_socket->port);
    printf(" - address: %s\r\n", socket->modem_socket->address);
    printf(" - cid: %d\r\n", socket->socket_config->cid);
    printf(" - pakcet_size: %d\r\n", socket->socket_config->packet_size);
    printf(" - max_to: %d\r\n", socket->socket_config->max_to);
    printf(" - conn_to: %d\r\n", socket->socket_config->conn_to);
    printf(" - tx_to: %d\r\n", socket->socket_config->tx_to);
    if (socket->firewall != NULL) {
        printf("*** socket firewall ***:\r\n");
        printf(" - action: %d (0:remove 1:accept 2:drop)\r\n", socket->firewall->action);
        printf(" - ip_address: %s\r\n", socket->firewall->ip_address);
        printf(" - net_mask: %s\r\n", socket->firewall->net_mask);
    }    
}

/*
 * The socket_free function is used to free up the socket that is no longer in use.
 */
uint8_t socket_free()
{
    /* Sanity check... yep your crazy */
    if(_socket_pool_index-- == -1) {
		_socket_pool_index = 0;
		return 0;
    }
    
    /* I knight you sir NULL */
    socket_pool[_socket_pool_index] = NULL;

    return _socket_pool_index;
}

socket_state_t socket_tick(socket_t *socket)
{

    modem_tick();

    /*
     * TODO: Implement state management for socket
     * configuration and connectivity
     */

     /*
      * The following functions need to be implemented to configure the socket
      * and firewall entries. Just pasting here for future reference.
      *
      * Configure:
      * modem_socketconfig(socket_config);
      *
      * Firewall:
      * modem_firewallcfg(entry);
      *
      * Socket listen example:
      * modem_socketlisten(socket); 
      *
      * Socket open example:
      * modem_socketopen(socket);
      */


    return SCK_INIT;
}