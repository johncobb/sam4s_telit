#include <cph.h>
#include "modem_defs.h"
#include "modem.h"
#include "telit.h"
#include "modem_config.h"


#if defined(__arm__)
static const char _tag[] PROGMEM = "modem_config: "; /* used in embedded gcc */
#else
static const char _tag[] = "modem_config: ";
#endif

at_cmd_t at_cfg_commands[] = {
    // fnc_handler, timeout, retries, waitingreply
    {modem_factory,  		    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_echooff,   		    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setinterface,  	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setmsgformat,  	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setcontext,   	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setuserid,   	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setpassword,  	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_setguardtime, 	    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_skipesc, 		    1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_mobileequiperr,      1000, 0, false, EVT_WAITING, modem_handle_default},
    {modem_activatecontext,      1000, 0, false, EVT_WAITING, modem_handle_activatecontext},
    {NULL, 0, 0, NULL}
};

uint8_t _config_index = 0;
at_cmd_t *at_cmd;

void modem_cfg_ondatareceive_func(uint8_t *buffer, uint32_t len)
{
    LOG("modem_cfg_ondatareceive_func: bytes: %d buffer: %s\r\n", len, buffer);
}

void modem_cfg_oneventreceive_func(uint8_t *buffer, uint32_t len)
{
    LOG("modem_cfg_oneventreceive_func: bytes: %d buffer: %s\r\n", len, buffer);

    /*
     * Make sure the funciton handler is assigned
     */
    if (at_cmd->fnc_eventhandler != NULL) {
        LOG("calling fnc_eventhandler: \r\n");
        uint8_t result = at_cmd->fnc_eventhandler(buffer);

        if (result > 0) {
            at_cmd->result = EVT_OK;
        } else {
            at_cmd->result = EVT_ERROR;
        }  
    } else {
        LOGE("*** undefined modem_eventreceived function handler ***\r\n");
    }
    
}

void modem_config_init()
{
    at_cmd = NULL;
    /*
     * modem event handler for receiving bulk data from servers.
     */
    // modem_set_ondatareceive_func(modem_cfg_ondatareceive_func);
    
    /*
     * modem event handler for receiving modem events
     * Example: \r\nOK\r\n \r\nCONNECT\r\n
     */
    modem_set_oneventreceive_func(modem_cfg_oneventreceive_func);
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
            
            /*
             * TODO: Implement a freaking timeout already, we don't have all day :\
             */

            /*
             * Check to see if we have a response greater than EVT_WAITING
             */
            if (at_cmd->result > EVT_WAITING) {
                /* we are no longer waiting for a reply. */
                at_cmd->waitresp = false;

                /*
                 * Everything went as expected...  proceed
                 */
                if (at_cmd->result == EVT_OK) {

                    LOG("OK\r\n");
                    /* moving on to next function */
                    _config_index++;
                    return CFG_INPROC;
                } else {
                    /* wtf went wrong? */
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