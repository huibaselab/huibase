

#include <netdb.h>
#include "haddr.h"

using namespace HUIBASE;


HCIp4Addr::HCIp4Addr(HINT addr_bytes, HINT _port ) {

	Init(addr_bytes, _port);

}

HCIp4Addr::HCIp4Addr (HCSTRR strIp, HINT _port) {

	Init(strIp, _port);
	
}

HCIp4Addr::HCIp4Addr (HCSTRR strIpPort){

	memset(&m_addr, 0, sizeof(m_addr));
	ParseNetFromString(strIpPort, m_addr);
	
}

HCIp4Addr::HCIp4Addr (const struct sockaddr_in& addr) {

	Init(addr);

}


HRET HCIp4Addr::Bind (const HCSocket& _sock) const {
	HSYS_RET cb = ::bind(_sock.GetSocket(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCIp4Addr::Connect (const HCSocket& _sock) const {
	HSYS_RET cb = ::connect(_sock.GetSocket(), (const struct sockaddr*)&m_addr, sizeof(m_addr));
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCIp4Addr::Accept (const HCSocket& server, HCSocket& client) {

	socklen_t len = sizeof(m_addr);
	HSYS_RET cb = ::accept(server.GetSocket(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb > 0, SYS_FAILED);

	client.SetSocket(cb);

	HRETURN_OK;
}

HRET HCIp4Addr::GetFromSocket (const HCSocket& sock) {

	socklen_t len = sizeof(m_addr);
	HSYS_RET cb = getpeername(sock.GetSocket(), (struct sockaddr*)&m_addr, &len);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCIp4Addr::GetAddrInfo (HSTRR strIp, HINT& iPort) const {

	strIp = inet_ntoa(m_addr.sin_addr);
	iPort = ntohs(m_addr.sin_port);
	//iPort = m_addr.sin_port;

	HRETURN_OK;

}


HRET HCIp4Addr::Init (const struct sockaddr_in& addr) {

	memcpy(&m_addr, &addr, sizeof(m_addr));
	HRETURN_OK;

}

HRET HCIp4Addr::Init (HINT iAddr, HINT iPort) {

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(iPort);
	memcpy(&m_addr.sin_addr.s_addr, &iAddr, sizeof(m_addr.sin_addr.s_addr));
	HRETURN_OK;
	
}

HRET HCIp4Addr::Init (HCSTRR strIp, HINT iPort) {

	return setupNetWithIpString(strIp, iPort, m_addr);
	
}


HRET HCIp4Addr::setupNetWithIpString (HCSTRR ip, HINT port, struct sockaddr_in& net) {

	net.sin_family = AF_INET;
	net.sin_port = htons(port);
	net.sin_addr.s_addr = inet_addr(ip.c_str());
	HRETURN_OK;
}


HRET HCIp4Addr::ParseNetFromString (HCSTRR str, struct sockaddr_in& net) {

	memset(&net, 0, sizeof(net));
	HCSTR ip_split = ":";

	// 127.0.0.1:809
	HSIZE pos = str.find(ip_split);
	if (pos == HCStr::npos || pos == str.length() -1) {
		HRETURN(INVL_PARA);
	}

	auto is_ip_str = [] (HCSTRR ip) ->bool {
		return ip.find_first_not_of("1234567890.") == HSTR::npos;
	};

	HSTR ssip = str.substr(0, pos);
	if (!is_ip_str(ssip)) {
		HRETURN(INVL_PARA);
	}

	HINT iPort = HCStr::stoi(str.substr(pos+1));

	return setupNetWithIpString(ssip, iPort, net);
	
}

HRET HCIp4Addr::GetStringFromNet (const struct sockaddr_in& addr, HSTRR ip, HINT& port) {
	port = addr.sin_port;
	ip = inet_ntoa(addr.sin_addr);

	HRETURN_OK;
}



