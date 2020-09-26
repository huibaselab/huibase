/**************************=cpphd=******************************************
 *
 * @ File Name: hsocket.h
 *
 * @ Create Author: Tom Hui
 *
 * @ Create Date: Sun Sep 17 22:37 2017
 *
 * @ Brief:
 * 	define the network peer comonent.
 *
 ****************************************************************************/

#ifndef __HSOCKET_H__
#define __HSOCKET_H__

#include "huibase.h"
#include "hstr.h"
#include "haddr.h"
#include "htime.h"

namespace HUIBASE {

	class HCSocketException;

    class HCSocket;

    class HCTcpSocket;

    class HCUdpSock;

    class HCAddr;

	class HCIp4Addr;

	class HCTcpClient;

}



#define SOCK_FD_CHECK 	do{ if (not IsGoodSocket()) { \
		throw HCSocketException(HCStr::Format("It not a good socket. Socket:[%d]", m_fd)); \
	} } while(0)

class HUIBASE::HCSocketException : public HCException {
public:
	HCSocketException(HCStr strError)
		: HCException(strError, errno, HFALSE, HFALSE, -1, HNULLSTR) {}
};

class HUIBASE::HCSocket {
public:
    constexpr static HFD_T SOCKET_DEFA_SOCKET = 0;
    constexpr static HINT SOCKET_MIN_TIMEOUT = 3;
	constexpr static HINT SOCKET_KEEPALIVE_DEF = 1;
	constexpr static HINT SOCKET_KEEPIDLE_DEF = 600;
	constexpr static HINT SOCKET_KEEPINTERVAL_DEF = 5;
	constexpr static HINT SOCKET_KEEPCOUNT_DEF = 3;

    enum SOCK_TYPE {
	st_no,
	st_tcp,
	st_udp,
    };

public:
    HCSocket (HFD_T fd = SOCKET_DEFA_SOCKET)
	: m_fd (fd) {}

    HCSocket (const HCSocket& sock) = delete;

    // move construction
    HCSocket (HCSocket&& sock)
    	: m_fd(sock.m_fd) { sock.m_fd = SOCKET_DEFA_SOCKET; }

    virtual ~HCSocket() {
	    Close();
    }

    HBOOL IsGoodSocket () const { HRET_BOOL(m_fd > SOCKET_DEFA_SOCKET); }

    virtual HRET Init () = 0;

    virtual SOCK_TYPE GetSocketType () const {
	return st_no;
    }

    virtual HRET Bind (const HCAddr& addr) const ;

    virtual HRET Connect (const HCAddr& addr) const ;

    virtual HRET ConnectWithTimeOut (const HCAddr& addr, HINT timeout = SOCKET_MIN_TIMEOUT) const ; 

    virtual HRET Listen (HUINT _count) const;

    virtual HRET Accept (HCSocket& sock, HCAddr& ) const;

    void Close() {

        if (m_fd != SOCKET_DEFA_SOCKET) {

            ::close(m_fd);

            m_fd = SOCKET_DEFA_SOCKET;

        }

    }

    HRET GetAddrInfo (HCAddr& addr) const;

    virtual HRET GetAddrInfo (HSTRR strIp, HINT& iPort) const = 0;

    HCStr ToString () const;

    HRET ShutDown (HSYS_T	how = SHUT_RDWR);

    HRET SetNonblocked (HBOOL bFlag = HTRUE) const;

    HBOOL IsNonblockedSocket ();

	HRET SetKeepAlive (HINT keepAlive = SOCKET_KEEPALIVE_DEF, 
		HINT keepIdle = SOCKET_KEEPIDLE_DEF, 
		HINT keepInterval = SOCKET_KEEPINTERVAL_DEF, 
		HINT keepCount = SOCKET_KEEPCOUNT_DEF);

	HRET SetReuseAddr ();

    HRET SetSockOpt (HSYS_T optName, const void* optVal,
		     socklen_t optLen, HSYS_T level = SOL_SOCKET);

    HRET GetSockOpt (HSYS_T optName, void* optVal,
		     socklen_t* optLen, HSYS_T level = SOL_SOCKET);

    virtual HSYS_RET Write (const void* buf, HSIZE nbyte) ;

    virtual HSYS_RET WriteAll (const void* buf, HSIZE nbyte, HINT timeout = SOCKET_MIN_TIMEOUT) ; 

    virtual HSYS_RET Read (void* buf, HSIZE nbyte) ;

    virtual HSYS_RET ReadWithTimeOut (void* buf, HSIZE nbyte, HINT timeout = SOCKET_MIN_TIMEOUT) ;

    HSYS_RET ReadAllWithTimeout (void* buf, HSIZE nbyte, HINT timeout = SOCKET_MIN_TIMEOUT) ;

    virtual HSYS_RET Send (const void* buf, HSIZE len, HSYS_T flags = 0) ;

    virtual HSYS_RET SendAll (const void* buf, HSIZE len, HSYS_T flags = 0, HINT timeout = SOCKET_MIN_TIMEOUT);

    virtual HSYS_RET Recv (void* buf, HSIZE len, HSYS_T flags = 0) ;

    virtual HSYS_RET RecvWithTimeOut (void* buf, HSIZE len, HSYS_T flags = 0, HINT timeout = SOCKET_MIN_TIMEOUT) ;

	HFD_T GetSocket () const { return m_fd; }

    void Invalid () { m_fd = SOCKET_DEFA_SOCKET; }

	void SetSocket (int fd) { m_fd = fd;}

	static HRET GetLocalIp (HVSTRSR ips) ;

private:    
    HFD_T m_fd = SOCKET_DEFA_SOCKET;
};


class HUIBASE::HCTcpSocket : public HUIBASE::HCSocket {
public:
    HCTcpSocket(HFD_T fd = SOCKET_DEFA_SOCKET)
	: HCSocket (fd) {}

    HCTcpSocket(const HCTcpSocket& sock) = delete;

    virtual ~HCTcpSocket();

    virtual HRET Init () ;

	virtual HRET GetAddrInfo (HSTRR strIp, HINT& iPort) const;

 	virtual HCIp4Addr GetAddr () const;

    SOCK_TYPE GetSocketType () const {
	return st_tcp;
    }


};


class HUIBASE::HCUdpSock : public HUIBASE::HCSocket {
 public:
    HCUdpSock (HFD_T fd = SOCKET_DEFA_SOCKET)
        : HCSocket(fd) { }

    HCUdpSock (const HCUdpSock& sock) = delete;

    virtual ~HCUdpSock ();

    virtual HRET Init () override;

    virtual HRET GetAddrInfo (HSTRR strIp, HINT& iPort) const override;

    virtual SOCK_TYPE GetSocketType () const override { return st_udp;  }

    HRET GetMacAddr (HSTRR strMac) const;

    virtual HCIp4Addr GetAddr () const;

};


class HUIBASE::HCTcpClient : public HUIBASE::HCTcpSocket {
public:
	static constexpr HUINT DEFAULT_LEN = HLEN1024_C;
	static constexpr HUINT MAX_BUF_LEN = HLEN8096_C;

public:
	HCTcpClient (HUINT len = DEFAULT_LEN);

	HCTcpClient(const HCTcpClient& client) = delete;

	HCTcpClient(HCTcpClient&& other);

	~HCTcpClient();

	HCTcpClient& operator= (HCTcpClient&& other);

	void Touch () { SetAccess (time(nullptr)); }

	const HCTime& GetAccess () const { return m_timeAccess; }

	void SetAccess (const HCTime& ct ) { m_timeAccess = ct; }

	HUINT GetBufLen () const { return m_len; }
	HUINT GetDataLen () const { return m_dataLen; }

	HRET ReadData () ;

	HRET WriteData () ;
	
private:
	virtual HSYS_RET Write (const void* buf, HSIZE nbyte) override;

	virtual HSYS_RET Read (void* buf, HSIZE nbyte) override; 
	
	virtual HSYS_RET Send (const void* buf, HSIZE len, HSYS_T flags = 0) override; 

	virtual HSYS_RET Recv (void* buf, HSIZE len, HSYS_T flags = 0) override;	

private:
	void clear();

private:
	HUINT m_len{0};
	HPSZ m_buf{nullptr};
	HUINT m_dataLen {0};
	HCTime m_timeAccess{0};
};

#endif //__HSOCKET_H__













