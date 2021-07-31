#include <StdH.h>

#if		(_MSC_VER > 1200)

#include "NetClient.h"
#include <boost/array.hpp>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/Util.h>
#include <Engine/Network/CNetwork.h>

#define		BUFF_SIZE		10240

CNetSocket::CNetSocket(boost::asio::io_service& io)
	: _bConnect(false)
	, _socket(io)
{
}

CNetSocket::~CNetSocket()
{
	if (isValid() == true)
		_socket.close();
}

bool CNetSocket::isValid()
{
	if (!_socket.is_open() && _bConnect)   // Ŀ��Ʈ �� ���� ������ �������ȴٸ�
	{
		_bConnect = false;					// Ŀ��Ʈ�� ���� ����
		return false;
	}

	return true;
}

bool CNetSocket::connect( std::string& IP, std::string& port )
{
	boost::asio::ip::tcp::resolver resolver(_socket.get_io_service());
	boost::asio::ip::tcp::resolver::query query(IP, port);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end;

	// resolving�� endpoint�� ������ �õ��Ѵ�.
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
		_socket.close();
		_socket.connect(*endpoint_iterator++, error);
	}

	// ���� �������� Ȯ��
	if (error)
	{
		_bConnect = false;
		return false;
	}

	_bConnect = true;

	return true;
}

void CNetSocket::send(CNetworkMessage &nmMessage)
{
	if (isValid() == false)
		return;

	_pNetwork->Encrypt(nmMessage);

	CPacket packet;

	packet.WriteToPacket((void*)nmMessage.GetBuffer(), nmMessage.nm_slSize, 0, 0, 0, nmMessage.nm_slSize);

	extern CCommunicationInterface _cmiComm;

	_cmiComm.attachCRC(&packet);

	boost::system::error_code error;
	int len = boost::asio::write(_socket, boost::asio::buffer((void*)packet.pa_pubPacketData, packet.pa_slSize + sizeof(int)), error);
}

void CNetSocket::recv()
{
	if (isValid() == false)
		return;

	while(true)
	{
		// �о�� �����͸� ������ array�� �����.
		boost::array<char, BUFF_SIZE> buf;
		boost::system::error_code error;
		// read_some�Լ��� ȣ���� 128��ŭ�� ���ۿ� ä���޶�� ��û�Ѵ�.
		// ( len <=128 �ϰ��̴�. )
		size_t len = _socket.read_some(boost::asio::buffer(buf), error);

		break;
	}
}

//-----------------------------------------------------------------

CNetClient::CNetClient()
	: _pSock(NULL)
{

}

CNetClient::~CNetClient()
{
	SAFE_DELETE(_pSock);
}

bool CNetClient::init(std::string& ip, std::string& port)
{
	_pSock = new CNetSocket(_io_service);
	
	return _pSock->connect(ip, port);
}

void CNetClient::send( CNetworkMessage& nmMessage )
{
	if (_pSock == NULL)
		return;

	_pSock->send(nmMessage);
}

void CNetClient::recv()
{
	if (_pSock == NULL)
		return;

	_pSock->recv();
}

#endif	// (_MSC_VER > 1200)

