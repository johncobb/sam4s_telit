#include "telit.h"
#include "modem.h"


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
	modem_write(MODEM_CMD_SELINT);
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

void modem_socketconfig(void)
{
	modem_write(MODEM_CMD_SOCKETCFG);
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

void modem_mobileequiperr(void)
{
	modem_write(MODEM_CMD_MOBILEEQUIPERR);
}
