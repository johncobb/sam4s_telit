#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "modem_config.h"

at_cmd_t at_cfg_commands[] = {
    // fnc_handler, timeout, retries, waitingreply
    {modem_factory,  		    1000, 0, false, SYS_OK},
    {modem_echooff,   		    1000, 0, false, SYS_OK},
    {modem_setinterface,  	    1000, 0, false, SYS_OK},
    {modem_setmsgformat,  	    1000, 0, false, SYS_OK},
    {modem_setcontext,   	    1000, 0, false, SYS_OK},
    {modem_setuserid,   	    1000, 0, false, SYS_OK},
    {modem_setpassword,  	    1000, 0, false, SYS_OK},
    {modem_setguardtime, 	    1000, 0, false, SYS_OK},
    {modem_skipesc, 		    1000, 0, false, SYS_OK},
    {modem_socketconfig, 	    1000, 0, false, SYS_OK},
    {modem_mobileequiperr,      1000, 0, false, SYS_OK},
    {NULL, 0, 0, NULL}
};

uint8_t _config_index = 0;
at_cmd_t *at_cmd;

void modem_cfg_ondatareceive_func(char *data)
{
    // printf("modem_config.ondatareceive_func: %s\r\n", data);

    at_cmd->result = modem_data_handler(data);
}

void modem_config_init()
{
    at_cmd = NULL;
    modem_set_ondatareceive_func(modem_cfg_ondatareceive_func);
}

modem_cfg_state_t modem_config_tick(void)
{
    /*
     * Check to see if we have a command to process
     */
    if (at_cmd != NULL) {
        /*
         * Check to see if we're awaiting a response from
         * a command sent to the modem.
         */
        if (at_cmd->waitresp) {
            // printf("at_cmd->result: %d\r\n", at_cmd->result);

            /*
             * Check to see if we have a response greater than SYS_OK
             */
            if (at_cmd->result > SYS_OK) {
                /* we are no longer waiting for a reply. */
                at_cmd->waitresp = false;

                /*
                 * Everything went as expected...  proceed
                 */
                if (at_cmd->result == SYS_AT_OK) {
                    // printf("next command\r\n");
                    printf("OK\r\n");
                    /* Moving on to next function*/
                    _config_index++;
                    return CFG_INPROC;
                } else {
                    /* WTF went wrong? */
                    return CFG_ERROR;
                }
            } else {
                /*
                 * Did we receive a response within the alloted window?
                 */
                at_cmd->waitresp = true;
                return CFG_INPROC;
            }
        }
    }
    // printf("setting at_cmd:\r\n");

    /*
     * If we made it this far this is the first apss through the lop
     * and we need to assign the first command for processing.
     */
    at_cmd = &(at_cfg_commands[_config_index]);

    /*
     * Congratulations! You reached the end.
     */
    if(at_cmd->fnc_handler == NULL) {

        printf("modem_config: completed successfully!\r\n");
        return CFG_COMPLETE;
    }

    /*
     * Dispatch the current function.
     */
    // printf("dispatch func_handler: \r\n");
    at_cmd->fnc_handler();
    at_cmd->waitresp = true;

    /*
     * We are currently in process
     */
    return CFG_INPROC;
}