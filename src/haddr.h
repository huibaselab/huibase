/**************************=cpphd=******************************************
 * 
 * @ File Name: haddr.h
 * 
 * @ Create Author: Tom Hui
 * 
 * @ Create Date: Sun Sep 17 16:41 2017
 * 
 * @ Brief:
 * 	define the network address component.
 * 
 ****************************************************************************/


#ifndef __HADDR_H__
#define __HADDR_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>


#include "huibase.h"
#include "hsocket.h"

namespace HUIBASE {

    class HCSocket;

    class HCAddr {
    public:
	friend class HCSocket;
	constexpr static HSYS_T ADDR_DEFA_PORT = 0;
	constexpr static HSYS_T ADDR_DEFA_ADDR = 0;

	enum ADDR_TYPE {
	    at_no,
	    at_ip4,
	    at_ip6,
	};
	
    public:
    HCAddr() = default;

    virtual ~HCAddr () {}

    virtual HRET Bind (const HCSocket& _socket) const = 0;

    virtual HRET Connect (const HCSocket& _socket) const = 0;

    virtual HRET Accept (const HCSocket& server, HCSocket& client) = 0;

    virtual HRET GetFromSocket (const HCSocket& sock) = 0;
	
    virtual HRET GetAddrInfo (HSTRR strIp, HINT& iPort) const = 0;

    virtual ADDR_TYPE GetAddrType () const {
	return at_no;
    }

    HSTR ToString () const {

		HSTR ret;
		HINT port;
		GetAddrInfo (ret, port);

		ret.append(":");
		ret.append(HCStr::itos(port));

		return ret;
	
    }

	protected:
		
		virtual HRET Init (const struct sockaddr_in& addr) = 0;

		virtual HRET Init (HINT iAddr, HINT iPort) = 0;

		virtual HRET Init (HCSTRR strIp, HINT iPort) = 0;
    };

    class HCIp4Addr : public HCAddr {	
    public:
	HCIp4Addr(HINT addr_bytes = ADDR_DEFA_ADDR, HINT _port = ADDR_DEFA_PORT);

	HCIp4Addr (HCSTRR strIp, HINT _port);

	HCIp4Addr (HCSTRR strIpPort);

	explicit HCIp4Addr (const struct sockaddr_in& addr);

	virtual ~ HCIp4Addr() {}

	HRET Bind (const HCSocket& _sock) const ;

	HRET Connect (const HCSocket& _sock) const;

	HRET Accept (const HCSocket& server, HCSocket& client);

	HRET GetFromSocket (const HCSocket& sock);

	HRET GetAddrInfo (HSTRR strIp, HINT& iPort) const;

	ADDR_TYPE GetAddrType () const {
	    return at_ip4;
	}


    protected:

		virtual HRET Init (const struct sockaddr_in& addr);

		virtual HRET Init (HINT iAddr, HINT iPort);

		virtual HRET Init (HCSTRR strIp, HINT iPort);

		static HRET setupNetWithIpString (HCSTRR ip, HINT port, struct sockaddr_in& net); 

		static HRET ParseNetFromString (HCSTRR str, struct sockaddr_in& net); 

		static HRET GetStringFromNet (const struct sockaddr_in& addr, HSTRR ip, HINT& port);

	    
    private:
		struct sockaddr_in m_addr;
    };

}

#endif //__HADDR_H__














