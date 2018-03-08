
#include <cph.h>


#include "modem.h"
#include "modem_config.h"

// NO DIALTONE
// BUSY
// NO CARRIER
// CONNECT
// CONNECT 115200

extern void _cph_delay_ms(uint32_t millis)
{
    // millis * micros
    usleep(millis*1000);
}

void ondatareceive_func(char *data)
{
    printf("modem_rx_func: %s\r\n", data);
}


void modem_loop(void)
{
    printf("wait 1 sec.\r\n");
    _cph_delay_ms(1000);

    modem_init();
    modem_set_ondatareceive_func(ondatareceive_func);

    modem_write("ATE0\r\n");

    while (true) {
        modem_write("AT\r\n");
        modem_tick();

        _cph_delay_ms(1000);
    }
}

void config_loop(void)
{
    modem_init();
    modem_config_init();

    while (true) {
        modem_tick();
        if (modem_config_tick() == CFG_COMPLETE){
            break;
        }
        // _cph_delay_ms(10);
    }
}

int main(void)
{
    // modem_loop();
    config_loop();

    while(true) {
        printf("just hanging out in the loop\r\n");
        _cph_delay_ms(1000);
    }
}
