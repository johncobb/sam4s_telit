#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "serial_driver.h"

#if defined(__arm__)
static const char _tag[] PROGMEM = "serial_driver: "; /* used in embedded gcc */
#else
static const char _tag[] = "serial_driver: ";
#endif

serial_rx_cb_t serial_rx_cb;

char serial_buffer[SERIAL_BUFFER_LEN] = {0};
int file_desc;

// Hold original terminal attributes
static struct termios original_tty_attrs;

void serial_set_callback(serial_rx_cb_t cb)
{
    LOG("serial_set_callback: \r\n");
    serial_rx_cb = cb;
}


// TODO: Not currently working in OSX
// int serial_data_ready(void)
// {
//     int bytes;
//     ioctl(file_desc, FIONREAD, &bytes);

//     return bytes;
// }

int serial_port_init(char *io_path)
{

    memset(serial_buffer, 0, SERIAL_BUFFER_LEN);
    // http://www.cmrr.umn.edu/~strupp/serial.html#3_1_5
    int handshake;
    struct termios options;

    file_desc = open(io_path, O_RDWR | O_NOCTTY | O_NDELAY);
    LOG("io_path: %s\r\n", io_path);
    LOG("file_desc: %d\r\n", file_desc);

    if (file_desc == -1) {
        LOG("error opening port\r\n");
        return -1;
    } else {
        LOG("open_serial_port: successful\r\n");
    }

    // Now that the device is open, clear the O_NONBLOCK flag so subsequent I/O will block.
    fcntl(file_desc, F_SETFL, 0);

    // Get the current options and save them so we can restore the default settings later.
    if (tcgetattr(file_desc, &original_tty_attrs) == -1) {
        LOG("Error getting tty attributes %s - %s(%d).\n", io_path, strerror(errno), errno);
    }

    options = original_tty_attrs;

    // Set baud rates
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    // Print the current input and output baud rates.
    LOG("input_baud_rate: %d\r\n", (int) cfgetispeed(&options));
    LOG("output_baud_rate: %d\r\n", (int) cfgetospeed(&options));

    // Enable receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10; // 1 second read timeout

    LOG("echo_flag: %lu\r\n", options.c_lflag );

    options.c_cflag &= ~CRTSCTS;
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag |= CS8; 

        // Set new options for port
    tcsetattr(file_desc, TCSANOW, &options);

    return file_desc;
}

void serial_port_close(void)
{
    LOG("serial_port_close: file_desc: %d\r\n", file_desc);
    close(file_desc);
}

void serial_port_tick(void)
{
    int bytes = read(file_desc, serial_buffer, SERIAL_BUFFER_LEN);

    if (bytes > 0) {
        for(int i=0; i<bytes; i++) {
            serial_rx_cb(serial_buffer[i]);
        }
    }
}

void serial_port_write(char *buffer, uint8_t len)
{
    LOGT("serial_port_write: %s\r\n", buffer);
    write(file_desc, buffer, len);
}

void serial_port_read(char *buffer, uint8_t len)
{
    LOGT("file_desc: %d\r\n", file_desc);
    int bytes = read(file_desc, serial_buffer, len);
    LOGT("serial_port_read: bytes: %d buffer: %s\r\n", bytes, serial_buffer);

}