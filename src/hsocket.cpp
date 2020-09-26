

#include <sys/select.h>
#include "hsocket.h"

#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>

namespace HUIBASE  {

HRET HCSocket::Bind (const HCAddr& addr) const  {

	SOCK_FD_CHECK;

	return addr.Bind(*this);

}

HRET HCSocket::Connect (const HCAddr& addr) const {

	SOCK_FD_CHECK;

	return addr.Connect(*this);

}

HRET HCSocket::ConnectWithTimeOut (const HCAddr& addr, HINT timeout) const {

	SOCK_FD_CHECK;

	if (timeout <= 0) timeout = SOCKET_MIN_TIMEOUT;

	SetNonblocked();
	
	while (not (HIS_OK(addr.Connect(*this)))) {
		fd_set wfds;
		struct timeval tval = {0, 0};

		FD_ZERO(&wfds);
		FD_SET(GetSocket(), &wfds);

		tval.tv_sec = timeout;

		HINT cb = 0;

		while((cb = select(GetSocket() + 1, NULL, &wfds, NULL, &tval)) < 0) {

			if (errno == EINTR) {
				continue;
			} else {
				throw HCSocketException("ConnectWithTimeOut select error");
			}
			
		}

		if (cb == 0) {
			SetNonblocked(HFALSE);
			throw HCSocketException("ConnectWithTimeOut select timeout");
		} else {

			HSYS_T iError = -1;
			HSIZE slen = sizeof(iError);
			getsockopt(GetSocket(), SOL_SOCKET, SO_ERROR, &iError, (socklen_t*)&slen);
			if (iError != 0) {
				SetNonblocked(HFALSE);
				throw HCSocketException(HCStr::Format("ConnectWithTimeOut error: %s", strerror(iError)));
			}
			
		}
		
	}

	SetNonblocked(HFALSE);
	HRETURN_OK;

}

HRET HCSocket::Listen (HUINT _count) const{

	HSYS_RET cb = ::listen(GetSocket(), _count);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCSocket::Accept (HCSocket& sock, HCAddr& addr)  const{

	HRET cb = addr.Accept(*this, sock);
	HASSERT_RETURN(HIS_OK(cb), HBASE_FAILED);
	
	HRETURN_OK;
}

HRET HCSocket::GetAddrInfo (HCAddr& addr) const {

	HRET cb = addr.GetFromSocket(*this);
	HASSERT_RETURN(HIS_OK(cb), HBASE_FAILED);

	HRETURN_OK;
}

HCStr HCSocket::ToString () const {

	HCStr ret;
	HINT port;

	GetAddrInfo(ret, port);

	ret.appendFormat(":%d", port);

	return ret;
}

HRET HCSocket::ShutDown (HSYS_T	how) {

	HSYS_RET cb = ::shutdown (m_fd, how);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}

HRET HCSocket::SetNonblocked (HBOOL bFlag) const{

	HSYS_T flags = 0;
	if ((flags = fcntl(m_fd, F_GETFL, 0)) < 0) {
		HRETURN(SYS_FAILED);
	}

	if (HIS_TRUE(bFlag) && flags & O_NONBLOCK) {
		HRETURN_OK;
	}

	if (HIS_FALSE(bFlag) && not (flags & O_NONBLOCK)) {
		HRETURN_OK;
	}

	if (HIS_TRUE(bFlag)) {
		// set to non-block mode
		if (fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
			HRETURN(SYS_FAILED);
		}

	} else {

		if (fcntl(m_fd, F_SETFL, flags | ~O_NONBLOCK) < 0) {
			HRETURN(SYS_FAILED);
		}

	}

	HRETURN_OK;
}

HBOOL HCSocket::IsNonblockedSocket () {
	HSYS_T flags = 0;
	if ((flags = fcntl(m_fd, F_GETFL, 0)) < 0) {
		return HFALSE;
	}

	HRET_BOOL(flags & O_NONBLOCK);
}


/**
	keepAlive: trun on keepalive
	keepIdle: try to network plumbing if no data transafe in 600's
	keepInterval: timeval 
	keepCount: times for network plumbing
**/
HRET HCSocket::SetKeepAlive (HINT keepAlive, HINT keepIdle, HINT keepInterval, HINT keepCount) {

	HASSERT_THROW(HIS_TRUE(IsGoodSocket()), INVAIL_DE);
	/*if (HIS_TRUE(IsGoodSocket())) {
		return HERR_NO(INVAIL_DE);
	}*/

	(void)SetSockOpt(SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive), SOL_SOCKET);

#ifndef HWINDOWS
    (void)SetSockOpt(TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle), SOL_TCP);
    (void)SetSockOpt(TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval), SOL_TCP);
    (void)SetSockOpt(TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount), SOL_TCP);
#else 
	(void)keepIdle;
	(void)keepInterval;
	(void)keepCount;
#endif


	HRETURN_OK;
}


HRET HCSocket::SetReuseAddr () {

	int rea = 1;
	return SetSockOpt(SO_REUSEADDR, (void*)&rea, sizeof(rea), SOL_SOCKET);

}


HRET HCSocket::SetSockOpt (HSYS_T optName, const void* optVal,
		     socklen_t optLen, HSYS_T level ) {

	HSYS_RET cb = setsockopt(m_fd, level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;
}

HRET HCSocket::GetSockOpt (HSYS_T optName, void* optVal, socklen_t* optLen, HSYS_T level) {
	HSYS_RET cb = getsockopt(m_fd, level, optName, optVal, optLen);
	HASSERT_RETURN(cb == 0, SYS_FAILED);

	HRETURN_OK;	
}

HSYS_RET HCSocket::Write (const void* buf, HSIZE nbyte) {

	SOCK_FD_CHECK;

	HSYS_RET cb = ::write(m_fd, buf, nbyte);
	if (cb < 0) {
		throw HCSocketException(HCStr::Format("Write socket failed. Socket:[%d]", m_fd));
	}
	return cb;
		
}

HSYS_RET HCSocket::WriteAll (const void* buf, HSIZE nbyte, HINT timeout) {

	SOCK_FD_CHECK;

	const char* pos = (const char*)buf;

	HSIZE sent_bytes = 0; 
	HSYS_T this_send = 0;
	while (sent_bytes < nbyte) {
		//do {
			if (timeout > 0) {
				fd_set write_fds;
				FD_ZERO(&write_fds);
				FD_SET(m_fd, &write_fds);

				timeval tval = {0,0};
				tval.tv_sec = timeout;
				tval.tv_usec = 0;

				HSYS_RET cb = select(m_fd +1, NULL, &write_fds, NULL, &tval);
				if (cb == 0) {
					throw HCSocketException("write all time out");
				} else if (cb < 0) {
					throw HCSocketException("select failed");
				}
			}

			this_send = ::write(GetSocket(), pos, nbyte - sent_bytes);
            //} while(this_send >= 0);

		if (this_send <= 0) {
			return this_send;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;
}

HSYS_RET HCSocket::Read (void* buf, HSIZE nbyte) {

	SOCK_FD_CHECK;

	HSYS_RET cb = ::read(m_fd, buf, nbyte);
	if (cb < 0) {
		throw HCSocketException(HCStr::Format("Read socket failed. Socket:[%d]", m_fd));
	}
	return cb;

}

HSYS_RET HCSocket::ReadWithTimeOut (void* buf, HSIZE nbyte, HINT timeout) 
	{
	
	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(m_fd, &read_fds);

		timeval tval = {0,0};
		tval.tv_sec = timeout;
		tval.tv_usec = 0;

		HSYS_RET cb = select(m_fd +1, &read_fds, NULL, NULL, &tval);
		if (cb == 0) {
			throw HCSocketException("read all time out");
		} else if (cb < 0) {
			throw HCSocketException("select failed");
		}

	}

    HSYS_RET ret = ::read(GetSocket(), buf, nbyte);

	return ret;
	
}

HSYS_RET HCSocket::ReadAllWithTimeout (void* buf, HSIZE nbyte, HINT timeout) {

	HSYS_RET cb = 0, ret = 0;
	char * pos = static_cast<char*>(buf);
	HSIZE temp_nbyte = nbyte;

	do {
		cb = ReadWithTimeOut(pos, temp_nbyte, timeout);

		if (cb > 0) {
			ret += cb;
		}

		pos += cb;
		temp_nbyte -= cb;

	} while(cb > 0);

	return ret;

}

HSYS_RET HCSocket::Send (const void* buf, HSIZE len, HSYS_T flags) {
	SOCK_FD_CHECK;
		
	HSYS_RET cb = ::send(GetSocket(), buf, len, flags);
	if (cb < 0) {
		throw HCSocketException(HCStr::Format("send socket failed. Socket:[%d]", m_fd));
	}
	return cb;

}

HSYS_RET HCSocket::SendAll (const void* buf, HSIZE len, HSYS_T flags, HINT timeout) {

	SOCK_FD_CHECK;

	const char* pos = (const char*)buf;

	HSIZE sent_bytes = 0;
	HINT this_send = 0;
	while (sent_bytes < len) {
		do {
			if (timeout > 0) {
				fd_set write_fds;
				FD_ZERO(&write_fds);
				FD_SET(m_fd, &write_fds);

				timeval tval = {0,0};
				tval.tv_sec = timeout;
				tval.tv_usec = 0;

				HSYS_RET cb = select(m_fd +1, NULL, &write_fds, NULL, &tval);
				if (cb == 0) {
					throw HCSocketException("sendall all time out");
				} else if (cb < 0) {
					throw HCSocketException("select failed");
				}
			}

			this_send = ::send(GetSocket(),pos, len - sent_bytes, flags);
		} while((this_send < 0) && (errno == EINTR) );

		if (this_send <= 0) {
			return this_send;
		}

		sent_bytes += this_send;
		pos += this_send;
	}

	return sent_bytes;

}

HSYS_RET HCSocket::Recv (void* buf, HSIZE len, HSYS_T flags) {
	SOCK_FD_CHECK;

	HSYS_RET cb = ::recv(GetSocket(), buf, len, flags);
	if (cb < 0) {
		throw HCSocketException(HCStr::Format("Recv socket failed. Socket:[%d]", m_fd));
	}
	return cb;

}

HSYS_RET HCSocket::RecvWithTimeOut (void* buf, HSIZE len, HSYS_T flags , HINT timeout ) 
	{

	SOCK_FD_CHECK;

	if (timeout > 0) {
	
		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(m_fd, &read_fds);

		timeval tval = {0,0};
		tval.tv_sec = timeout;
		tval.tv_usec = 0;

		HSYS_RET cb = select(m_fd +1, &read_fds, NULL, NULL, &tval);
		if (cb == 0) {
			throw HCSocketException("read all time out");
		} else if (cb < 0) {
			throw HCSocketException("select failed");
		}
		
	}

	HSYS_RET ret = ::recv(GetSocket(), buf, len, flags);

	return ret;
}

HRET HCSocket::GetLocalIp (HVSTRSR ips) {

	ips.clear();
	//TODO: .....

	HRETURN_OK;
}

HCTcpSocket::~HCTcpSocket() { }


HRET HCTcpSocket::Init ()  {
	
	HASSERT_THROW(GetSocket() == SOCKET_DEFA_SOCKET, RECREATE);

	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	HASSERT_THROW(sock > 0, SYS_FAILED);
	SetSocket(sock);

	HRETURN_OK;

}


HRET HCTcpSocket::GetAddrInfo (HSTRR strIp, HINT& iPort) const {

	HCIp4Addr addr = GetAddr();

	return addr.GetAddrInfo(strIp, iPort);
	
}


HCIp4Addr HCTcpSocket::GetAddr () const {
	HCIp4Addr ret ;

	ret.GetFromSocket(*this);

	return ret;
}


HCUdpSock::~HCUdpSock () {
}


HRET HCUdpSock::Init() {

    HASSERT_THROW_MSG(GetSocket() == SOCKET_DEFA_SOCKET, "socket is invalid", RECREATE);

    auto sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    HASSERT_THROW_MSG(sock > 0, "call socket failed!", SYS_FAILED);

    SetSocket(sock);

    HRETURN_OK;

}


HRET HCUdpSock::GetAddrInfo(HSTRR strIp, HINT &iPort) const {

    HCIp4Addr addr = GetAddr();

    return addr.GetAddrInfo(strIp, iPort);

}


HCIp4Addr HCUdpSock::GetAddr () const {
    HCIp4Addr ret ;

    ret.GetFromSocket(*this);

    return ret;

}


HRET HCUdpSock::GetMacAddr(HSTRR strMac) const {

    // if fd is invalid, we return error.
    // Do not throw an exception.
    HASSERT_RETURN(GetSocket() != SOCKET_DEFA_SOCKET, UN_INIT);

    int success = 0;

    static HCHAR buf[HLEN8_C] = {0};
    ZERO_MEM(buf, HLEN8_C);

    struct ifreq ifr;
    struct ifconf ifc;

    ifc.ifc_len = HLEN8_C;
    ifc.ifc_buf = buf;

    HASSERT_THROW_MSG(ioctl(GetSocket(), SIOCGIFCONF, &ifc) != -1, "ioctl failed", SYS_FAILED);

    struct ifreq* it = ifc.ifc_req;

    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
         strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(GetSocket(), SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(GetSocket(), SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else { /* handle error */ }
    }

    unsigned char mac_address[6] = {0};

    if (success) memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);

    sprintf(buf, "%02x%02x%02x%02x%02x%02x",
            mac_address[0], mac_address[1], mac_address[2],
            mac_address[3], mac_address[4], mac_address[5]);

    strMac = buf;
    HRETURN_OK;
}



HCTcpClient::HCTcpClient (HUINT len) 
	: m_len(len), m_buf(nullptr), m_dataLen(0), m_timeAccess(time(NULL)){

	if (len >= MAX_BUF_LEN) {
		len = DEFAULT_LEN;
	}

	m_buf = (HPSZ)malloc(m_len);
	CHECK_NEWPOINT(m_buf);

}


HCTcpClient::HCTcpClient(HCTcpClient&& other) 
	: m_len(other.m_len), m_buf(other.m_buf),m_dataLen(other.m_dataLen),
	m_timeAccess(other.m_timeAccess){

	other.m_len = 0;
	other.m_buf = nullptr;
	other.m_dataLen = 0;
	other.m_timeAccess = 0;

}


HCTcpClient::~HCTcpClient() {

	HFREE(m_buf);

}


HCTcpClient& HCTcpClient::operator= (HCTcpClient&& other) {

	HFREE(m_buf);

	m_len = other.m_len;
	m_buf =	other.m_buf;
	m_dataLen = other.m_dataLen;
	m_timeAccess.Touch();

	other.m_len = 0;
	other.m_buf = nullptr;
	other.m_dataLen = 0;
	other.m_timeAccess.Touch();

	return *this;
}

HRET HCTcpClient::ReadData ()  {

	clear();

	HPSZ pos = m_buf;

	HRET cb = 0, len = m_len - cb;

	do {

		cb = Read(pos, len);

		m_dataLen += cb;

		pos += cb;

		len -= m_len - cb;		

		if (cb > 0 && len == 0) {

			HRETURN(INVL_RES);
		
		}

	} while (cb > 0);	

	Touch();


	HRETURN_OK;

}


HRET HCTcpClient::WriteData () {

	(void)Write (m_buf, m_dataLen);

	Touch();

	HRETURN_OK;

}


HSYS_RET HCTcpClient::Write (const void* buf, HSIZE nbyte) {

	auto cb = HCTcpSocket::Write(buf, nbyte);

	Touch();

	return cb;

}

HSYS_RET HCTcpClient::Read (void* buf, HSIZE nbyte) {

	auto cb = HCTcpSocket::Read(buf, nbyte);

	Touch();

	return cb;

}


HSYS_RET HCTcpClient::Send (const void* buf, HSIZE len, HSYS_T flags) {

	auto cb = HCTcpSocket::Send(buf, len, flags);

	Touch();

	return cb;

}

HSYS_RET HCTcpClient::Recv (void* buf, HSIZE len, HSYS_T flags	) {

	auto cb = HCTcpSocket::Recv(buf, len, flags);

	Touch();

	return cb;

}


void HCTcpClient::clear() {

	memset(m_buf, 0, m_len);

	m_dataLen = 0;

	m_timeAccess.Touch();

}


}


