#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
    uint16_t nn;
} gps_utc_t;

typedef struct {
    int deg;
    float min;
} gps_coordinate_t;

typedef struct {
    gps_utc_t utc;
    uint32_t millis;
    gps_coordinate_t lat;
    gps_coordinate_t lng;
	uint8_t lat_deg;
	uint32_t lat_min;
	uint8_t lon_deg;
	uint32_t lon_min;
	int32_t alt;
    uint8_t fix;
    char ns;
    char ew;
} gps_position;

static volatile gps_position _position;
static volatile gps_position _p;

char *ignore(char *str, char match);
char *read(char *str, char *buf, char delimiter);
void parse_gps(char *gps_msg);
void parse_degrees(char *str, uint8_t *degree, uint32_t *minutes);

void parse_position(gps_position *pos, char *lat_str, char *lon_str, char *alt_str);

const char GPS_LOGOUTPUT[] = "%lu:%lu %d.%lu %d.%lu %li\n";

int main(void) 
{

    printf("Welcome to the GPS parser ...\r\n");
    char * buffer = "$GPSACP: 120631.999,5433.9472N,00954.8768E,1.0,46.5,3,167.28,0.36,0.19,130707,11\r";

    parse_gps(buffer);
    return 0;
}


char *ignore(char *str, char match) {
	uint8_t c = 0;
	while (1) {
		c = *str++;
		if ((c == match) || (c == '\0')) {
			break;
		}
	}
	return str;
}


char *read(char *str, char *buf, char delimiter) {
	uint8_t c = 0;
	while (1) {
		c = *str++;
		if ((c == delimiter) || (c == '\0')) {
			break;
		}
		else if (c != ' ') {
			*buf++ = c;
		}
	}
	*buf = '\0';
	return str;
}


/*
 * Parse and convert the given string into degrees and minutes.
 * Example: 5333.9472N --> 53 degrees, 33.9472 minutes
 * converted to: 53.565786 degrees 
 */
void parse_degrees(char *str, uint8_t *degree, uint32_t *minutes) {
	char buf[6];
	uint8_t c = 0;
	uint8_t i = 0;
	char *tmp_str;
	
	tmp_str = str;
	while ((c = *tmp_str++) != '.') i++;
	strlcpy(buf, str, i-1);
	*degree = atoi(buf);
	 	
	tmp_str -= 3;
	i = 0;
	while (1) {
		c = *tmp_str++;
		if ((c == '\0') || (i == 5)) {
			break;
		}
		else if (c != '.') {
			buf[i++] = c;
		}
	}
	buf[i] = 0;
	*minutes = atol(buf);
	*minutes *= 16667;
	*minutes /= 1000;
}

/*
 * Parse and convert the position tokens. 
 */
void parse_position(gps_position *pos, char *lat_str, char *lon_str, char *alt_str) {
	char buf[10];
	parse_degrees(lat_str, &pos->lat_deg, &pos->lat_min);
	parse_degrees(lon_str, &pos->lon_deg, &pos->lon_min);
	read(alt_str, buf, '.');
	pos->alt = atol(buf);
}


/*
 * Parse the given string into a position record.
 * example:
 * $GPSACP: 120631.999,5433.9472N,00954.8768E,1.0,46.5,3,167.28,0.36,0.19,130707,11\r
 */
void parse_gps(char *gps_msg) {


    if (!sscanf(gps_msg, "$GPSACP: %2d%2d%2d.%3d,%2d%7f%s,%3d",&_p.utc.hh, &_p.utc.mm, &_p.utc.ss, &_p.utc.nn, &_p.lat.deg, &_p.lat.min, &_p.ns, &_p.lng.deg)) {
        printf("sscanf error occured!\r\n");
    }

    printf("hh:mm:ss:nn deg:mm %d %d %d %d %d %f %c %d\r\n", _p.utc.hh, _p.utc.mm, _p.utc.ss, _p.utc.nn, _p.lat.deg, _p.lat.min, _p.ns, _p.lng.deg);

    return;
    char utc[7];
    char millis[3];
	char lat_buf[12];
	char lon_buf[12];
	char alt_buf[7];
	char fix;
	char date[7];
	char nr_sat[4];
	
	gps_msg = ignore(gps_msg, ':');					// skip prolog
    gps_msg = read(gps_msg, utc, '.');		// time, hhmmss
    printf("utc: %s\r\n", utc);
    printf("utc[0]: %c\r\n", utc[0]);
    printf("utc[1]: %c\r\n", utc[1]);

    uint32_t tmp =  atoi(utc);
    uint32_t op = atoi(utc);
    op = op/
    printf("tmp: %d\r\n", tmp/10);
    printf("tmp: %d\r\n", tmp/10);
    printf("tmp: %d\r\n", tmp/100);
    // printf("tmp: %d\r\n", tmp/100);
    return;


    // printf("utc: %s\n", utc);
    // gps_msg = ignore(gps_msg, ',');					// skip ms
    gps_msg = read(gps_msg, millis, ',');					// skip ms
    // printf("millis: %s\n", millis);
	gps_msg = read(gps_msg, lat_buf, ',');	// latitude
	gps_msg = read(gps_msg, lon_buf, ',');	// longitude
	gps_msg = ignore(gps_msg, ',');					// hdop
	gps_msg = read(gps_msg, alt_buf, ',');	// altitude
	fix = *gps_msg++;								// fix, 0, 2d, 3d
	gps_msg++;
	gps_msg = ignore(gps_msg, ',');					// cog, cource over ground
	gps_msg = ignore(gps_msg, ',');					// speed [km]
	gps_msg = ignore(gps_msg, ',');					// speed [kn]
	gps_msg = read(gps_msg, date, ',');		// date ddmmyy
	gps_msg = read(gps_msg, nr_sat, '\n');	// number of sats

	if (fix != '0') {
        uint8_t byte_0 = atoi(utc[0]);
        // int byte_1 = atoi(utc[1]);
        // printf("here is my utc: %d\r\n", byte_0);
        // _position.utc.hh = atoi(utc[0]);
        _position.millis = atol(millis);
		parse_position(&_position, lat_buf, lon_buf, alt_buf);
		_position.fix = fix;
    }
    

    if (_position.fix > 0) {
            // printf(GPS_LOGOUTPUT, _position.utc, _position.millis, _position.lat_deg, _position.lat_min, _position.lon_deg, _position.lon_min, _position.alt, _position.utc, _position.millis);
            printf(GPS_LOGOUTPUT, _position.utc.hh, _position.millis, _position.lat_deg, _position.lat_min, _position.lon_deg, _position.lon_min, _position.alt);
    }

}

