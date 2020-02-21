#include "MyBaseServer.h"
#include<thread>

bool g_bRun = true;
void cmdThread()
{
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			g_bRun = false;
			printf("退出cmdThread线程\n");
			break;
		}
		else {
			printf("不支持的命令。\n");
		}
	}
}

class MyServer : public BaseServer
{
public:

	virtual void OnNetJoin(ClientSocketObject* pClient)
	{
		m_clientCount++;
		printf("client<%d> join\n", pClient->sockfd());
	}

	virtual void OnNetLeave(ClientSocketObject* pClient)
	{
		m_clientCount--;
		printf("client<%d> leave\n", pClient->sockfd());
	}

	virtual void OnNetMsg(ClientSocketObject* pClient, PacketHeader* header)
	{
		m_recvCount++;
		switch (header->cmd)
		{
		case CMD_LOGIN_REQUEST:
		{
			LoginRequest* login = (LoginRequest*)header;
			printf("收到客户端<>请求：CMD_LOGIN,数据长度：%d,userName=%s PassWord=%s\n",  login->dataLength, login->userName, login->PassWord);
			//忽略判断用户密码是否正确的过程
			LoginResponse ret;
			strcpy(ret.data,"request successfully.");
			int len = sizeof(ret);
			pClient->SendData(&ret,len);
		}
		break;
		case CMD_LOGOUT_REQUEST:
		{
			LoginResponse* logout = (LoginResponse*)header;
			//printf("收到客户端<Socket=%d>请求：CMD_LOGOUT,数据长度：%d,userName=%s \n", cSock, logout->dataLength, logout->userName);
			//忽略判断用户密码是否正确的过程
			//LogoutResult ret;
			//SendData(cSock, &ret);
		}
		break;
		case CMD_TICK_REQUEST:
		{
			TickRequest* tickreq = (TickRequest*)header;
			printf("tick type : %d\n",tickreq->type);

			// 回应客户端tick的信息
			cout << "response the msg to client....."<<endl;
			int i=1;
			while(i < 5)
			{
				TickResponse response;
				strcpy(response.tick.instrumentId,"60000.sh");
				response.tick.open=123.0+i;
				response.tick.high=153.0+i;
				response.tick.low=113.0+i;
				response.tick.close=133.0+i;
				
				int len = sizeof(TickResponse);
				int result=pClient->SendData(&response,len);
				cout << "response the msg to client end..... len : "<<len<< " result : "<<result<< endl;
				i++;
				sleep(1);
			}
			
		}
		break;
		default:
		{
			printf("<socket=%d>收到未定义消息,数据长度：%d\n", pClient->sockfd(), header->dataLength);
			//DataHeader ret;
			//SendData(cSock, &ret);
		}
		break;
		}
	}
private:

};

int main()
{

	MyServer server;
	server.InitSocket();
	server.Bind(nullptr, 4567);
	server.Listen(5);
	server.Start(4);

	std::thread t1(cmdThread);
	t1.detach();

	while (g_bRun)
	{
		server.OnRun();
	}
	server.Close();
	printf("已退出。\n");
	getchar();
	return 0;
}