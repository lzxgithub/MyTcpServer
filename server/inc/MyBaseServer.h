/*****************************************************************************
 * auth : Created by lizhongxin.
 * date : 2020-02-21
 * function : a tcp server which every one client connect to it and server them. 
 *****************************************************************************/

#ifndef _MY_BASE_CLIENT_
#define _MY_BASE_CLIENT_

#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#include <stdio.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <iostream>

#include "MyMessageType.h"
#include "MyTimestamp.h"

#ifndef RECV_BUFF_SIZE
#define RECV_BUFF_SIZE 10240
#endif

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR			(-1)

using namespace std;
#ifndef RECV_BUFF_SZIE
#define RECV_BUFF_SZIE 10240
#endif // !RECV_BUFF_SZIE

class ClientSocketObject
{
public:
	ClientSocketObject(SOCKET sockfd = INVALID_SOCKET)
	{
		m_sockfd = sockfd;
		memset(m_szMsgBuf,0,sizeof(m_szMsgBuf));
		m_lastPost = 0;
	}

	~ClientSocketObject()
	{
		cout << "~ClientSocketObject m_sockfd : " << m_sockfd << endl;
	}

	SOCKET sockfd()
	{
		return m_sockfd;
	}

	char* msgBuf()
	{
		return m_szMsgBuf;
	}

	int getLastPost()
	{
		return m_lastPost;
	}

	void setLastPost(int post)
	{
		m_lastPost = post;
	}

	int SendData(PacketHeader* header, int nLen)
	{
		int ret = SOCKET_ERROR;
		if(header)
		{
			ret = send(m_sockfd, (const char*)header, nLen, 0);
		}

		return ret;
	}
private:
	SOCKET m_sockfd;
	char m_szMsgBuf[RECV_BUFF_SIZE * 5];
	int m_lastPost;
};

class INetEvent
{
public:
	virtual void OnNetJoin(ClientSocketObject* pClient) = 0;
	virtual void OnNetLeave(ClientSocketObject* pClient) = 0;
	virtual void OnNetMsg(ClientSocketObject* pClient, PacketHeader* header) = 0;
};

class ServerObject
{
public:
	ServerObject(int num,SOCKET sock = INVALID_SOCKET)
	{
		m_sockfd = sock;
		m_pNetEvent = nullptr;
		serverNum = num;
	}

	~ServerObject()
	{
		cout << "~ServerObject..." << endl;
		Close();
		m_sockfd = INVALID_SOCKET;
	}

	void setEventObj(INetEvent* event)
	{
		m_pNetEvent = event;
	}

	void Close()
	{
		cout << "Close()...." << endl;
		if(m_sockfd != INVALID_SOCKET)
		{
			for(int n = (int)m_clients.size() - 1; n >= 0; n--)
			{
				close(m_clients[n]->sockfd());
				delete m_clients[n];
			}

			close(m_sockfd);
			m_clients.clear();
		}
	}

	bool isRun()
	{
		return m_sockfd != INVALID_SOCKET;
	}

	bool OnRun()
	{
		//cout << "serverNum : " << serverNum<<endl;
		while(isRun())
		{
			if(m_clientsBuff.size() > 0)
			{
				lock_guard<mutex> lock(m_mutex);
				for(auto pClient : m_clientsBuff)
				{
					m_clients.push_back(pClient);
				}
				m_clientsBuff.clear();
			}

			if(m_clients.empty())
			{
				std::chrono::milliseconds t(1);
				std::this_thread::sleep_for(t);
				continue;
			}

			fd_set fdRead;
			FD_ZERO(&fdRead);
			SOCKET maxSock = m_clients[0]->sockfd();
			for(int n = (int)m_clients.size() - 1; n >= 0; n--)
			{
				FD_SET(m_clients[n]->sockfd(),&fdRead);
				if (maxSock < m_clients[n]->sockfd())
				{
					maxSock = m_clients[n]->sockfd();
				}
			}

			int ret = select(maxSock + 1,&fdRead, 0,0,nullptr);
			if(ret < 0)
			{
				printf("select task is over.\n");
				Close();
				return false;
			}

			for(int n = m_clients.size() - 1; n >= 0; n--)
			{
				if(FD_ISSET(m_clients[n]->sockfd(), &fdRead))
				{
					int readcount = RecvData(m_clients[n]);
					if(-1 == readcount)
					{
						cout << "readcount : " << readcount<<endl;
						auto iter = m_clients.begin() + n;
						if(iter != m_clients.end())
						{
							if(m_pNetEvent)
								m_pNetEvent->OnNetLeave(m_clients[n]);
							delete m_clients[n];
							m_clients.erase(iter);
						}
					}
				}
			}
		}

	}

	int RecvData(ClientSocketObject* pClient)
	{
		int nLen = (int)recv(pClient->sockfd(),m_szRecv,RECV_BUFF_SZIE,0);
		cout << "nLen : " << nLen << endl;
		if(nLen <= 0)
			return -1;

		memcpy(pClient->msgBuf() + pClient->getLastPost(), m_szRecv, nLen);
		pClient->setLastPost(pClient->getLastPost() + nLen);

		while(pClient->getLastPost() >= sizeof(PacketHeader))
		{
			PacketHeader* header = (PacketHeader*)pClient->msgBuf();
			if(pClient->getLastPost() >= header->dataLength)
			{
				int nSize = pClient->getLastPost() - header->dataLength;
				OnNetMsg(pClient,header);
				memcpy(pClient->msgBuf(), pClient->msgBuf() + header->dataLength,nSize);
				pClient->setLastPost(nSize);
			}else
			{
				cout << "the recv data is not complete data."<<endl;
			}
		}

		return 0;
	}

	void addClient(ClientSocketObject* pClient)
	{
		cout << "addClient serverNum : "<<serverNum<<endl;
		lock_guard<mutex> lock(m_mutex);
		m_clientsBuff.push_back(pClient);
	}

	void Start()
	{
		m_thread = thread(mem_fn(&ServerObject::OnRun),this);
	}

	size_t getClientCount()
	{
		return m_clients.size() + m_clientsBuff.size();
	}

	virtual void OnNetMsg(ClientSocketObject* pClient, PacketHeader* header)
	{
		m_pNetEvent->OnNetMsg(pClient,header);
	}

public:
	char m_szRecv[RECV_BUFF_SZIE] = {};

private:
	int serverNum;
	SOCKET m_sockfd;
	vector<ClientSocketObject*> m_clients;
	vector<ClientSocketObject*> m_clientsBuff;
	mutex m_mutex;
	thread m_thread;
	INetEvent* m_pNetEvent;
};

class BaseServer : public INetEvent
{
private:
	SOCKET m_sock;
	std::vector<ServerObject*> m_serverObjects;
	MyTimestamp m_tTime;

protected:
	atomic_int m_recvCount;
	atomic_int m_clientCount;
public:
	BaseServer()
	{
		m_sock = INVALID_SOCKET;
		m_recvCount = 0;
		m_clientCount = 0;
	}

	virtual ~BaseServer()
	{
		Close();
	}

	SOCKET InitSocket()
	{
		if(INVALID_SOCKET != m_sock)
		{
			printf("<socket=%d> close the old connction...\n",(int)m_sock);
			Close();
		}

		m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(INVALID_SOCKET == m_sock)
		{
			printf("error, creating socket unsuccessfully\n");
		}else
		{
			printf("creating socket=<%d> is successfully\n",(int)m_sock);
		}

		return m_sock;
	}

	int Bind(const char* ip, unsigned short port)
	{
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);

		if(ip){
			_sin.sin_addr.s_addr = inet_addr(ip);
		}else{
			_sin.sin_addr.s_addr = INADDR_ANY;
		}

		int ret = bind(m_sock,(sockaddr*)&_sin,sizeof(_sin));
		if(SOCKET_ERROR == ret)
			printf("error, binding port=<%d> is unsuccessfully\n",(int)port);
		else
			printf("binding port=<%d> is successfully\n",(int)port);

		return ret;
	}

	int Listen(int n)
	{
		int ret = listen(m_sock,n);
		if(SOCKET_ERROR == ret)
			printf("socket=<%d> erorr, listen net port unsuccessfully\n", m_sock);
		else
			printf("socket=<%d> listen net port successfully\n",m_sock);

		return ret;
	}

	SOCKET Accept()
	{
		sockaddr_in clientAddr = {};
		int nAddrLen = sizeof(sockaddr_in);
		SOCKET cSock = INVALID_SOCKET;

		cSock = accept(m_sock, (sockaddr *)&clientAddr,(socklen_t *)&nAddrLen);
		if(INVALID_SOCKET == cSock)
			printf("socket=<%d> error, it is a invalid client..\n", (int)m_sock);
		else
			addClientToServerObject(new ClientSocketObject(cSock));

		return cSock;
	}

	void addClientToServerObject(ClientSocketObject* pClient)
	{
		auto pMinServer = m_serverObjects[0];
		for(auto pServerObject : m_serverObjects)
		{
			if(pMinServer->getClientCount() > pServerObject->getClientCount())
				pMinServer = pServerObject;
		}
		pMinServer->addClient(pClient);
		OnNetJoin(pClient);
	}

	void Start(int nServerObject)
	{
		for(int n = 0; n < nServerObject; n++)
		{
			auto ser = new ServerObject(n,m_sock);
			m_serverObjects.push_back(ser);
			ser->setEventObj(this);
			ser->Start();
		}
	}

	void Close()
	{
		if(m_sock != INVALID_SOCKET)
			close(m_sock);
	}

	bool OnRun()
	{
		if(isRun())
		{
			fd_set fdRead;
			FD_ZERO(&fdRead);
			FD_SET(m_sock, &fdRead);
			timeval t= {0,10};
			int ret = select(m_sock + 1,&fdRead,0,0,&t);
			if(ret < 0)
			{
				printf("Accept Select task is over.\n");
				Close();
				return false;
			}

			if(FD_ISSET(m_sock, &fdRead))
			{
				FD_CLR(m_sock,&fdRead);
				Accept();
				return true;
			}

			return true;
		}

		return false;
	}

	bool isRun()
	{
		return m_sock != INVALID_SOCKET;
	}

	void time4msg()
	{
		auto t1 = m_tTime.getElapsedSecond();
		if (t1 >= 1.0)
		{
			//printf("thread<%d>,time<%lf>,socket<%d>,clients<%d>,recvCount<%d>\n", _baseServers.size(), t1, _sock,(int)_clientCount, (int)(_recvCount/ t1));
			m_recvCount = 0;
			m_tTime.update();
		}
	}

	virtual void OnNetJoin(ClientSocketObject* pClient)
	{
		m_clientCount++;
	}

	virtual void OnNetLeave(ClientSocketObject* pClient)
	{
		m_clientCount++;
	}

	virtual void OnNetMsg(ClientSocketObject* pClient, PacketHeader* header)
	{
		m_recvCount++;
	}
};

#endif // _MY_BASE_CLIENT_