/*****************************************************************************
 * auth : Created by lizhongxin.
 * date : 2020-02-21
 * function : a tcp client for communicating with tcp sever. 
 *****************************************************************************/

#ifndef _MY_BASE_CLIENT_
#define _MY_BASE_CLIENT_

#include<unistd.h> 
#include<arpa/inet.h>
#include<string.h>

#include <stdio.h>
#include "MyMessageType.h"

#include<iostream>
using namespace std;

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 10240
#endif // !RECV_BUFF_SZIE

class BaseClient
{
private:
	SOCKET m_sock;
	bool m_isConnect;
	fd_set m_fdReads;

public:
	char m_szMsgBuf[RECV_BUFF_SZIE * 5] = {};
	int m_lastPos = 0;
	char m_szRecv[RECV_BUFF_SZIE] = {};
public:
	BaseClient()
	{
		m_sock = INVALID_SOCKET;
		m_isConnect = false;
		//_fdReads = 
	}

	virtual ~BaseClient()
	{
		Close();
	}

	//初始化socket
	void InitSocket()
	{
		if (INVALID_SOCKET != m_sock)
		{
			printf("<socket=%d> close the old connection...\n", m_sock);
			Close();
		}
		m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_sock)
		{
			printf("error，create the socket unsuccessfully...\n");
		}
		else {
			printf("create the Socket=<%d> successfully...\n", m_sock);
		}
	}

	int Connect(const char* ip,unsigned short port)
	{
		if (INVALID_SOCKET == m_sock)
		{
			InitSocket();
		}

		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);
		_sin.sin_addr.s_addr = inet_addr(ip);
		//printf("<socket=%d>正在连接服务器<%s:%d>...\n", _sock, ip, port);
		int ret = connect(m_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		if (SOCKET_ERROR == ret)
		{
			printf("<socket=%d>错误，连接服务器<%s:%d>失败...\n",m_sock, ip, port);
		}
		else {
			m_isConnect = true;
			//printf("<socket=%d>连接服务器<%s:%d>成功...\n",_sock, ip, port);
		}
		return ret;
	}

	//关闭套节字closesocket
	void Close()
	{
		if (m_sock != INVALID_SOCKET)
		{
			close(m_sock);
			m_sock = INVALID_SOCKET;
		}
		m_isConnect = false;
	}

	void OnRun()
	{
		while(isRun())
		{
			fd_set fdReads;
			FD_ZERO(&fdReads);
			FD_SET(m_sock, &fdReads);
			timeval t = { 0,0 };
			int ret = select(m_sock + 1, &fdReads, 0, 0, &t); 
			//printf("select ret : %d\n", ret);
			if (ret < 0)
			{
				printf("<socket=%d>select任务结束1\n", m_sock);
				Close();
				break;
			}
			if (FD_ISSET(m_sock, &fdReads))
			{
				//FD_CLR(_sock, &fdReads);
				int len = RecvData(m_sock);
				printf("RecvData count : %d\n", len);
				if (-1 == len)
				{
					printf("<socket=%d>select任务结束2\n", m_sock);
					Close();
					break;
				}
			}
		
		}
		return;
	}

	SOCKET GetSocket()
	{
		return m_sock;
	}

	bool isRun()
	{
		return m_sock != INVALID_SOCKET && m_isConnect;
	}

	int RecvData(SOCKET cSock)
	{
		int nLen = (int)recv(cSock, m_szRecv, RECV_BUFF_SZIE, 0);
		printf("RecvData nLen=%d\n", nLen);
		if (nLen <= 0)
		{
			printf("<socket=%d> has disconnected，end.\n", cSock);
			return -1;
		}
		
		memcpy(m_szMsgBuf + m_lastPos, m_szRecv, nLen);
		m_lastPos += nLen;
		while (m_lastPos >= sizeof(PacketHeader))
		{
			PacketHeader* header = (PacketHeader*)m_szMsgBuf;
			if (m_lastPos >= header->dataLength)
			{
				int nSize = m_lastPos - header->dataLength;
				OnNetMsg(header);
				memcpy(m_szMsgBuf, m_szMsgBuf + header->dataLength, nSize);
				m_lastPos = nSize;
			}
			else {
				cout << "接收到的信息不完整。" << endl;
				break;
			}
		}
		return 0;
	}

	virtual void OnNetMsg(PacketHeader* header)
	{
		switch (header->cmd)
		{
			case CMD_LOGIN_RESPONSE:
			{
			
				LoginResponse* login = (LoginResponse*)header;
				//printf("<socket=%d>收到服务端消息：CMD_LOGIN_RESULT,数据长度：%d\n", _sock, login->dataLength);
			}
			break;
			case CMD_LOGOUT_RESPONSE:
			{
				LogoutResponse* logout = (LogoutResponse*)header;
				//printf("<socket=%d>收到服务端消息：CMD_LOGOUT_RESULT,数据长度：%d\n", _sock, logout->dataLength);
			}
			break;
			case CMD_NEW_USER_JOIN:
			{
				NewUserJoin* userJoin = (NewUserJoin*)header;
				//printf("<socket=%d>收到服务端消息：CMD_NEW_USER_JOIN,数据长度：%d\n", _sock, userJoin->dataLength);
			}
			break;
			case CMD_TICK_RESPONSE:
			{
				TickResponse* tickResp = (TickResponse*)header;
				cout << "TickResponse...."<<endl;
				//printf("open : %f\n",tick->open);
				printf("instrumentId :　%s, open : %f, high : %f, low : %f, close : %f\n",tickResp->tick.instrumentId,tickResp->tick.open, tickResp->tick.high, tickResp->tick.low, tickResp->tick.close);
				//printf("<socket=%d>收到服务端消息：CMD_ERROR,数据长度：%d\n", _sock, header->dataLength);
			}
			break;
			case CMD_ERROR_RESPONSE:
			{
				printf("<socket=%d>收到服务端消息：CMD_ERROR,数据长度：%d\n", m_sock, header->dataLength);
			}
			break;
			default:
			{
				printf("<socket=%d>收到未定义消息,数据长度：%d\n", m_sock, header->dataLength);
			}
		}
	}

	int SendData(PacketHeader* header,int nLen)
	{
		int ret = SOCKET_ERROR;
		if (isRun() && header)
		{
			ret = send(m_sock, (const char*)header, nLen, 0);
			if (SOCKET_ERROR == ret)
			{
				printf("SendData Close.\n");
				Close();
			}
		}
		return ret;
	}
};

#endif