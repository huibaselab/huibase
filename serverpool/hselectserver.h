

#ifndef __HSELECTSERVER_H__
#define __HSELECTSERVER_H__

#include "huibase.h"
#include "hsocket.h"

namespace HUIBASE {


class HCSelectServer {
public:
	HCSelectServer(HCSTR strIp, HUINT uPort, HUINT uClientLen = 128, 
		HUINT uListenLen = 32, HUINT clientTimeout = 3);

	~HCSelectServer ();

	HRET Init () throw (HCException);

	HRET Run () throw (HCException);

	HRET Stop ();

private:
	HRET ProcessRecv (HCTcpClient& client);

	HRET AcceptConnect();
	
	
protected:
	HRET checkClientTimeout ();

	virtual HRET checkOthers () { HRETURN_OK; }

	virtual HRET workAccept (HCTcpClient& client) { (void)client; HRETURN_OK;}

	virtual HRET workRead (HCTcpClient& client) throw (HCException) = 0;

	virtual HRET workTimeout () { HRETURN_OK;}

	virtual HRET workError () { HRETURN_OK;}

public:
	void SetServerIp (HCSTRR strServerIp) { m_strServerIp = strServerIp; }

	void SetPort (HUINT port) { m_port = port; }

	void SetClientLen (HUINT client_len) { m_clientLen = client_len; }

	void SetListen (HUINT listen_len) { m_uListenLen = listen_len; }

	void SetClientTimeout (HUINT clientTimeout) { m_client_timeout = clientTimeout; }

private:	
	HSTR m_strServerIp;	
	
	HCTcpClient* m_pListens{nullptr};

	HUINT m_port{0};
	
	HUINT m_clientLen{0};	
	HUINT m_uListenLen{0};
	HUINT m_client_timeout{0};

	HCTcpSocket m_server;

	volatile bool m_running{true};
};

}

#endif //__HSELECTSERVER_H__

