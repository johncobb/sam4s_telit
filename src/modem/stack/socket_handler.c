#include "socket.h"
#include "socket_handler.h"


socket_handler_state_t _handler_state = SCK_H_INIT;
int _socket_index;
socket_t *_socket;


void sck_enterstate(socket_t *socket, socket_handler_state_t state)
{
    _handler_state = state;

    if (socket == NULL) {
        return;
    }
}

void sck_setstate(socket_handler_state_t state)
{
    _handler_state = state;
}

socket_handler_state_t socket_handler_tick(void)
{
    /* Increment and process each socket as we tick through the loop */
    if (++_socket_index == (SOCKET_POOL_LEN-1)) {
        _socket_index = 0;
    }

    /* Assign socket */
    _socket = socket_pool[_socket_index];

    /* Make sure it's been allocated */
    if (_socket == NULL) {
        return SCK_H_NEXTSOCKET;
    }

    return SCK_H_INIT;
}

sys_result sck_init(void)
{
    sys_result result = modem_init();

    if (result == SYS_OK) {
        sck_enterstate(_socket, SCK_H_REGISTER);
    }

    return result;
}


