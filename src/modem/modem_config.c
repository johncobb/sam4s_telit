#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "modem_config.h"

// // fnc_handler, timeout, retries, waitingreply
// {modem_factory, 		1000, 0, false},
// {modem_echooff, 		1000, 0, false},
// {modem_setinterface, 	1000, 0, false},
// {modem_setmsgformat, 	1000, 0, false},
// //{modem_setband, 		1000, 0, false},
// {modem_setcontext, 		1000, 0, false},
// {modem_setuserid, 		1000, 0, false},
// {modem_setpassword, 	1000, 0, false},
// {modem_setguardtime, 	1000, 0, false},
// {modem_skipesc, 		1000, 0, false},
// {modem_socketconfig, 	1000, 0, false},
// {modem_mobileequiperr, 	1000, 0, false},
// {NULL, 0, 0, NULL}


at_cmd_t at_cfg_commands[] = {
    // fnc_handler, timeout, retries, waitingreply
    {modem_factory, 		1000, 0, false},
    {modem_echooff, 		1000, 0, false},
    {modem_setinterface, 	1000, 0, false},
    {modem_setmsgformat, 	1000, 0, false},
    //{modem_setband, 		1000, 0, false},
    {modem_setcontext, 		1000, 0, false},
    {modem_setuserid, 		1000, 0, false},
    {modem_setpassword, 	1000, 0, false},
    {modem_setguardtime, 	1000, 0, false},
    {modem_skipesc, 		1000, 0, false},
    {modem_socketconfig, 	1000, 0, false},
    {modem_mobileequiperr, 	1000, 0, false},
    {NULL, 0, 0, NULL}
};

uint8_t _config_index = 0;

at_cmd_t *at_cmd;

sys_result modem_config_tick(void)
{
    at_cmd = &(at_cfg_commands[_config_index]);

    // check to see if we have reached the end of the array
    // if so then proceed to next comm state
    if(at_cmd->fnc_handler == NULL) {
        // enter_state(COMM_CONFIG_SOCKETS);
        //return SYS_OK reporting current state was successful
        return SYS_OK;
    }

    // dispatch the function
    at_cmd->fnc_handler();

    return SYS_OK;
}