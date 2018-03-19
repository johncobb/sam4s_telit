#ifndef SERIAL_DRIVER_H_
#define SERIAL_DRIVER_H_

#include <cph.h>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define SERIAL_BUFFER_LEN   128

#define SERIAL_IO_PATH               "/dev/tty.usbserial-FTVTIU6Z"

typedef void (*serial_rx_cb_t)(uint8_t);

void serial_set_callback(serial_rx_cb_t cb);
int serial_port_init(char *io_path);
void serial_port_close(void);
void serial_port_tick(void);
void serial_port_write(char *buffer, uint8_t len);
void serial_port_read(char *buffer, uint8_t len);
// void serial_write(char *buffer, uint8_t len);
// void serial_read(char *buffer, uint8_t len);



#endif