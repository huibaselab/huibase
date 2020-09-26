


#include "hselectserver.h"

namespace HUIBASE {

HCSelectServer::HCSelectServer(HCSTR strIp, HUINT uPort, HUINT uClientLen, 
		HUINT uListenLen, HUINT clientTimeout) 
	: m_strServerIp(strIp), m_pListens(nullptr), m_port(uPort),
		m_clientLen(uClientLen), m_uListenLen(uListenLen), 
		m_client_timeout(clientTimeout), m_running(true){

	assert(m_clientLen > 0);
	

}

HCSelectServer::~HCSelectServer() {

	HDEL_ARR(m_pListens);

}


HRET HCSelectServer::Init () throw (HCException){

	m_pListens = new HCTcpClient[m_clientLen];
	assert(m_pListens);

	HNOTOK_RETURN(m_server.Init());

	HNOTOK_RETURN(m_server.SetNonblocked(HTRUE));

	HNOTOK_RETURN(m_server.SetReuseAddr());

	HCIp4Addr addr(m_strServerIp, m_port);
	HNOTOK_RETURN(m_server.Bind(addr));
	
	HNOTOK_RETURN(m_server.Listen(m_uListenLen));

	HRETURN_OK;

}

HRET HCSelectServer::Run () throw (HCException) {

	while (m_running) {

		HFAILED_THROW(checkClientTimeout());

		HFAILED_THROW(checkOthers());

		fd_set read_set;
		HINT max_sock = m_server.GetSocket();
		FD_ZERO(&read_set);
		FD_SET(m_server.GetSocket(), &read_set);

		for (HUINT i = 0; i < m_clientLen; ++i) {

			HCTcpClient& client = m_pListens[i];

			if(HIS_TRUE(client.IsGoodSocket())) {

				FD_SET(client.GetSocket(), &read_set);

				max_sock = HMAX(client.GetSocket(), max_sock);

			}

		}

		timeval tv = {0,0};
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		auto cb = select (max_sock + 1, &read_set, nullptr, nullptr, &tv);	
		if (cb > 0) {

			for (HUINT i = 0; i < m_clientLen; ++i) {

				HCTcpClient& client = m_pListens[i];

				if (HIS_FALSE(client.IsGoodSocket())) {

					continue;
			
				}


				if (FD_ISSET(client.GetSocket(), &read_set)) {

					ProcessRecv(client);

				}				
				
			}

			if (FD_ISSET(m_server.GetSocket(), &read_set)) {

				AcceptConnect();

			}

		}else if (cb == 0) {

			workTimeout();
		
		} else {

			workError();
	
		}

	}


	HRETURN_OK;
}


HRET HCSelectServer::Stop () {

	m_running = true;

	HRETURN_OK;
	
}


HRET HCSelectServer::ProcessRecv (HCTcpClient& client) {

	client.Touch();

	try {

		HRET ret = client.ReadData();

		HIF_NOTOK(ret) {

			workError();
			client.Close();
			HRETURN(ERR_STATUS);

		}

		auto wcb = workRead(client);
		HIF_NOTOK(wcb){
			workError();
			client.Close();
			HRETURN(ERR_STATUS);
		}

		if ()

		auto cb = client.WriteData();
		HIF_NOTOK(cb){
			workError();
			client.Close();
			HRETURN(ERR_STATUS);
		}


	} catch(HCException& e) {

		workError();
		client.Close();	
		HRETURN(ERR_STATUS);
	}
	

	HRETURN_OK;

}


HRET HCSelectServer::AcceptConnect() {

	HCIp4Addr addr{0};

	for (HUINT i = 0; i < m_clientLen; ++i) {

		HCTcpClient& client = m_pListens[i];

		if (HIS_FALSE( client.IsGoodSocket())) {

		
			m_server.Accept (client, addr);

			client.SetNonblocked(HTRUE);

			client.Touch();

			workAccept(client);

			HRETURN_OK;
		
		}

	}
	
	HCTcpClient _sock;
	m_server.Accept(_sock, addr);
	_sock.Close();

	HRETURN(SRC_FAIL);

}


HRET HCSelectServer::checkClientTimeout () {

	static HTIME s_lastChecktime = 0;

	HTIME tnow = time(nullptr);
	if (tnow - s_lastChecktime < m_client_timeout) {

		HRETURN_OK;

	}

	s_lastChecktime = time(nullptr);

	for (HUINT i = 0; i < m_clientLen; ++i) {

		HCTcpClient& client = m_pListens[i];

		IF_FALSE(client.IsGoodSocket()) {

			continue;

		}


		if (tnow - client.GetAccess() > m_client_timeout) {

			client.Close();

		}

	}

	HRETURN_OK;

}



}

