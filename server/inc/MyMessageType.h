#ifndef _MY_MESSAGE_TYPE_H_
#define _MY_MESSAGE_TYPE_H_

typedef enum _CMD
{
	CMD_LOGIN_REQUEST,
	CMD_LOGIN_RESPONSE,
	CMD_LOGOUT_REQUEST,
	CMD_LOGOUT_RESPONSE,
	CMD_NEW_USER_JOIN,
	CMD_TICK_REQUEST,
	CMD_TICK_RESPONSE,
	CMD_ERROR_RESPONSE
}CMD;

typedef struct _PacketHeader
{
	_PacketHeader()
	{
		dataLength = sizeof(_PacketHeader);
		cmd = CMD_ERROR_RESPONSE;
	}
	short dataLength;
	short cmd;
	
}PacketHeader;

typedef struct _LoginRequest : public PacketHeader
{
	_LoginRequest()
	{
		dataLength = sizeof(_LoginRequest);
		cmd = CMD_LOGIN_REQUEST;
	}

	char userName[32];
	char PassWord[32];
	char data[32];
}LoginRequest;

typedef struct _LoginResponse : public PacketHeader
{
	_LoginResponse()
	{
		dataLength = sizeof(_LoginResponse);
		cmd = CMD_LOGIN_RESPONSE;
	}

	int result;
	char data[128];
}LoginResponse;

typedef struct _LogoutRequest : public PacketHeader
{
	_LogoutRequest()
	{
		dataLength = sizeof(_LogoutRequest);
		cmd = CMD_LOGOUT_REQUEST;
	}

	char userName[32];
}LogoutRequest;

typedef struct _LogoutResponse: public PacketHeader
{
	_LogoutResponse()
	{
		dataLength = sizeof(_LogoutResponse);
		cmd = CMD_LOGOUT_RESPONSE;
	}

	int result;
}LogoutResponse;

typedef struct _NewUserJoin : public PacketHeader
{
	_NewUserJoin()
	{
		dataLength = sizeof(_NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		sock = 0;
	}
	int sock;
}NewUserJoin;

typedef struct _Tick
{
	char instrumentId[16];
	double open;
	double high;
	double low;
	double close;
	int volume;
}Tick;

typedef struct _TickRequest : public PacketHeader
{
	_TickRequest()
	{
		dataLength = sizeof(_TickRequest);
		cmd = CMD_TICK_REQUEST;
	}

	int type;
}TickRequest;

typedef struct _TickResponse : public PacketHeader
{
	_TickResponse()
	{
		dataLength = sizeof(_TickResponse);
		cmd = CMD_TICK_RESPONSE;
	}

	Tick tick;

}TickResponse;


#endif // _MY_MESSAGE_TYPE_H_