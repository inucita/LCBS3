#include "stdh.h"

#include <Engine/Base/CTString.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/CPacket.h>

#include <Engine/Base/Listiterator.inl>

#include <Engine/Network/CommunicationInterface.h>

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>

// how many acknowledges can fit into one UDP packet
//#define MAX_ACKS_PER_PACKET (net_iUDPBlockSize/sizeof(ULONG))

extern FLOAT net_fDropPackets;
extern INDEX net_bReportPackets;
//0105
extern BOOL _bTcp4ClientInterface = FALSE;   // Ŭ���̾�Ʈ�� ��Ʈ�� �������̽��� TCP���� UDP���� �����ϴ� �Τ�.

CClientInterface::CClientInterface(void)
{
  TRACKMEM(Mem,"Network");
  Clear();
};

CClientInterface::~CClientInterface(void)
{
  Clear();
};


void CClientInterface::Clear(void) 
{
	ci_bUsed = FALSE;

	ci_bReliableComplete = FALSE;
	ci_pbInputBuffer.Clear();
	ci_pbOutputBuffer.Clear();
	ci_pbReliableInputBuffer.Clear();
	ci_pbWaitAckBuffer.Clear();

	ci_adrAddress.Clear();
	ci_strAddress = "";
	
	ci_pciOther = NULL;
	ci_ulSequence = 0;
};

// Get the size of all packets in the output buffers, including headers
ULONG CClientInterface::GetPendingDataSize()
{
  if (!ci_bUsed) {
    return 0;
  }
  return ci_pbOutputBuffer.GetTotalDataSize() + ci_pbWaitAckBuffer.GetTotalDataSize();
};

// mark the client interface as local for this computer
void CClientInterface::SetLocal(CClientInterface *pciOther)
{
  Clear();

	ci_bUsed = TRUE;

  ci_bClientLocal = TRUE;
  ci_pciOther = pciOther;
  if (pciOther!=NULL) {
    pciOther->ci_pciOther = this;
  }

	ci_adrAddress.Clear();
		
};

//0311 kwon�Լ��߰�. ������ ���۷� ���� ��Ŷ ������.
// send a message through this client interface
void CClientInterface::SendNew(const void *pvSend, SLONG slSize)
{
	//0524 kwon �ӽ� ����.
	//ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);
	
	UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
	SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
	ULONG ulHead,ulBody,ulTail;
	
	
    ulHead = UDP_PACKET_UNRELIABLE_HEAD;
    ulBody = UDP_PACKET_UNRELIABLE;
    ulTail = UDP_PACKET_UNRELIABLE_TAIL;
	
	
	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}
	
	
	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
	slTransferSize = slSizeToSend;
	
	
	//0707
#ifdef SECRET_KEY
	if(_pNetwork->MyCharacterInfo.secretkey != 0)
	{
		int seckey = ((_pNetwork->MyCharacterInfo.job + _pNetwork->MyCharacterInfo.secretkey) << 1);

		ci_adrAddress.adr_uwID = seckey;
	}
#endif

	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();
		
		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
		//0309 �����͹��۷� �ٷ� ������. send to real server
		if (_cmiComm.cci_pbMasterOutput.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
			ASSERT(FALSE);
		}
		
		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;
		
		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}
	
	// what remains is a tail of a message, or an unreliable packet
	uwPacketReliable |= ulTail;
	
	// so send it
	ppaNewPacket = CreatePacket();
	
	ulSequence = _pNetwork->getSeq();
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
	if (_cmiComm.cci_pbMasterOutput.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
		ASSERT(FALSE);
	}      
};


// send a message through this client interface
void CClientInterface::Send(const void *pvSend, SLONG slSize,BOOL bReliable)
{
	ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);

  UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
  SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
  ULONG ulHead,ulBody,ulTail;

  if (bReliable) {
    ulHead = UDP_PACKET_RELIABLE_HEAD;
    ulBody = UDP_PACKET_RELIABLE;
    ulTail = UDP_PACKET_RELIABLE_TAIL;
  } else {
    ulHead = UDP_PACKET_UNRELIABLE_HEAD;
    ulBody = UDP_PACKET_UNRELIABLE;
    ulTail = UDP_PACKET_UNRELIABLE_TAIL;
  }

	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}

 
	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
  slTransferSize = slSizeToSend;
	
	
	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();

		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
    if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
      ASSERT(FALSE);
    }

		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;

		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}

	// what remains is a tail of a message, or an unreliable packet
  uwPacketReliable |= ulTail;
	
	// so send it
	ppaNewPacket = CreatePacket();

	ulSequence = (++ci_ulSequence);
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
  if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
    ASSERT(FALSE);
  }    

  
};

// send a message through this client interface, to the provided address
void CClientInterface::SendTo(const void *pvSend, SLONG slSize,const CAddress adrAdress,BOOL bReliable)
{
	ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);

	UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
  SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
  ULONG ulHead,ulBody,ulTail;

  if (bReliable) {
    ulHead = UDP_PACKET_RELIABLE_HEAD;
    ulBody = UDP_PACKET_RELIABLE;
    ulTail = UDP_PACKET_RELIABLE_TAIL;
  } else {
    ulHead = UDP_PACKET_UNRELIABLE_HEAD;
    ulBody = UDP_PACKET_UNRELIABLE;
    ulTail = UDP_PACKET_UNRELIABLE_TAIL;
  }

	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}


	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
  slTransferSize = slSizeToSend;
	

	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();

		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,adrAdress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = adrAdress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = adrAdress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = adrAdress.adr_uwID;
    if(ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
      ASSERT(FALSE);
    }
		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;

		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}

	// what remains is a tail of a reliable message, or an unreliable packet
	uwPacketReliable |= ulTail;

	ppaNewPacket = CreatePacket();

	ulSequence = (++ci_ulSequence);
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,adrAdress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = adrAdress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = adrAdress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = adrAdress.adr_uwID;
  if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
    ASSERT(FALSE);
  }
};


// receive a message through the interface, discard originating address
BOOL CClientInterface::Receive(void *pvReceive, SLONG &slSize,BOOL bReliable)
{
	ASSERT (slSize>0);
	ASSERT (pvReceive != NULL);

	// we'll use the other receive procedure, and tell it to ignore the address
	return ReceiveFrom(pvReceive,slSize,NULL,bReliable);
};
//! �������̽��� ���� �޽����� �޴´�.
// receive a message through the interface, and fill in the originating address
BOOL CClientInterface::ReceiveFrom(void *pvReceive, SLONG &slSize, CAddress *padrAdress,BOOL bReliable)
{
	CPacket* ppaPacket;
	UBYTE* pubData = (UBYTE*) pvReceive;
	SLONG slDummySize;
	UWORD uwReliable;
	//! �����̾�� �޽������,
	// if a reliable message is requested
	if (bReliable) {
		//! ������ �����̾�� �޽������� üũ
		// if there is no complete reliable message ready
		if (ci_pbReliableInputBuffer.CheckSequence(slDummySize,TRUE) == FALSE) {
			return FALSE;
        //! �غ�� �޽����� ����� ������� ũ�ٸ� ���� FALSE
		// if the ready message is longer than the expected size
		} else if ( GetCurrentReliableSize() > slSize) {
			return FALSE;
		// if everything is ok, compose the message and kill the packets
		} else {
			//! ��ε�ĳ��Ʈ������ ��巹���� NULL�� �ƴϴ�. �������� �Ⱦ�.
			//! �����̾�� input���ۿ��� ù��° ��Ŷ�� �о�ͼ� �ּ�,��Ʈ,���̵� ����.
			// fill in the originating address (if necessary)
			if (padrAdress != NULL) {
				ppaPacket = ci_pbReliableInputBuffer.PeekFirstPacket();
				padrAdress->adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
				padrAdress->adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
				padrAdress->adr_uwID = ppaPacket->pa_adrAddress.adr_uwID;
			}

			slSize = 0;
			//! uwReliable�÷��� ���� ������ ���ö����� �����̾�� input���ۿ��� ��Ŷ�� �о�ͼ�
			//! pubData�� �ű��.
			do {
				ppaPacket = ci_pbReliableInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				slDummySize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				ppaPacket->ReadFromPacket(pubData,slDummySize);
				pubData += slDummySize;
				slSize += slDummySize;
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_RELIABLE_TAIL));
			return TRUE;
		}
	//! �𸱶��̾�� �޽������,
	// if an unreliable message is requested
	} else {
		// if there are no packets in the input buffer, return
		if (ci_pbInputBuffer.pb_ulNumOfPackets == 0) {
			return FALSE;
			//! �����̾�� input���۰� ������� �ʴٸ�, input���۷κ��� �ƹ��͵� ������ ����.			
			// if the reliable buffer is not empty, nothing can be accepted from the input buffer
			// because it would be accepted out-of order (before earlier sequences have been read)
		} else if (ci_pbReliableInputBuffer.pb_ulNumOfPackets != 0) {
			return FALSE;
		}
		
		ppaPacket = ci_pbInputBuffer.PeekFirstPacket();
		
		//! input���ۿ� �ִ� ù��° ��Ŷ�� �𸱸������ �ƴ϶�� ���� FALSE
		// if the first packet in the input buffer is not unreliable
		if (!ppaPacket->IsUnreliable()) {
			return FALSE;
			//! ����ġ ���� �޽������ ���� FALSE
			// if there is no complete message ready
		} else if (ci_pbInputBuffer.CheckSequence(slDummySize,FALSE) == FALSE) {
			return FALSE;
			// if the ready message is longer than the expected size
		} else if ( ppaPacket->pa_slTransferSize > slSize) {
			return FALSE;
			// if everything is ok, read the packet data, and kill the packet
		} else {
			// fill in the originating address (if necessary)
			if (padrAdress != NULL) {
				padrAdress->adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
				padrAdress->adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
				padrAdress->adr_uwID = ppaPacket->pa_adrAddress.adr_uwID;
			}
			
			slSize = 0;
			//! ������ ���ö����� ��Ŷ�� �޾Ƽ� ����� ����.
			do {
				ppaPacket = ci_pbInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				slDummySize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				ppaPacket->ReadFromPacket(pubData,slDummySize);
				pubData += slDummySize;
				slSize += slDummySize;
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_UNRELIABLE_TAIL));
			
			return TRUE;
		}
		
	}

	return FALSE;
};


// receive a message through the interface, discard originating address
BOOL CClientInterface::Receive(CTStream &strmReceive,UBYTE bReliable)
{
	CPacket* ppaPacket;
	UWORD uwReliable;
  SLONG slDummySize;

	// if a reliable message is requested
	if (bReliable) {
		// if there is no complete reliable message ready
		if (ci_pbReliableInputBuffer.CheckSequence(slDummySize,TRUE) == FALSE) {
			return FALSE;	
		// if everything is ok, compose the message and kill the packets
		} else {
			do {
				ppaPacket = ci_pbReliableInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				strmReceive.Write_t(ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE,ppaPacket->pa_slSize - MAX_HEADER_SIZE);
        if (ci_pbInputBuffer.pb_ulLastSequenceOut < ppaPacket->pa_ulSequence) {
          ci_pbInputBuffer.pb_ulLastSequenceOut = ppaPacket->pa_ulSequence;
        }
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_RELIABLE_TAIL));
			return TRUE;
		}
	// if an unreliable message is requested
	} else {
    // if there are no packets in the input buffer, return
		if (ci_pbInputBuffer.pb_ulNumOfPackets == 0) {
			return FALSE;
    // if the reliable buffer is not empty, nothing can be accepted from the input buffer
	  // because it would be accepted out-of order (before earlier sequences have been read)
    } if (ci_pbReliableInputBuffer.pb_ulNumOfPackets != 0) {
			return FALSE;
    }

		ppaPacket = ci_pbInputBuffer.PeekFirstPacket();				
		// if the first packet in the input buffer is not unreliable
		if (!ppaPacket->IsUnreliable()) {
			return FALSE;
    // if there is no complete message ready
    } else if (ci_pbInputBuffer.CheckSequence(slDummySize,FALSE) == FALSE) {
			return FALSE;
		// if everything is ok, read the packet data, and kill the packet
		} else {
			do {
				ppaPacket = ci_pbInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				strmReceive.Write_t(ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE,ppaPacket->pa_slSize - MAX_HEADER_SIZE);
        if (ci_pbInputBuffer.pb_ulLastSequenceOut < ppaPacket->pa_ulSequence) {
          ci_pbInputBuffer.pb_ulLastSequenceOut = ppaPacket->pa_ulSequence;
        }
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_UNRELIABLE_TAIL));
			return TRUE;
		}
		 
	}

	return FALSE;
};


// exchanges packets beetween this socket and it's local partner
// from output of this buffet to the input of the other and vice versa
void CClientInterface::ExchangeBuffers(void)
{
	ASSERT (ci_pciOther != NULL);
	CPacket* ppaPacket;
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	// take the output from this interface and give it to it's partner socket
	while (ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pbOutputBuffer.PeekFirstPacket();
		//if (ppaPacket->pa_tvSendWhen < tvNow) {
			ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
			if (ci_pciOther->ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
				DeletePacket(ppaPacket);
			}
		//} else {
//			break;
//		}
	}

	// and the other way around
	while (ci_pciOther->ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pciOther->ci_pbOutputBuffer.PeekFirstPacket();
//		if (ppaPacket->pa_tvSendWhen < tvNow) {
			ppaPacket = ci_pciOther->ci_pbOutputBuffer.GetFirstPacket();
			if (ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
				DeletePacket(ppaPacket);
			};

//		}	else {
			//break;
//		}
	}

};


// update interface's input buffer (transfer from input buffer to the reliable buffer...),
// for incoming acknowledge packets, remove acknowledged packets from the output buffers,
// and generate acknowledge messages for incoming reliable packets
BOOL CClientInterface::UpdateInputBuffers(void)
{
	BOOL bSomethingDone;
	INDEX iMaxAcks = 50;
//	ULONG pulGenAck[50];
	ULONG ulAckCount=0;
	CTimerValue tvNow;
	
	// if there are packets in the input buffer, process them
	FORDELETELIST(CPacket,pa_lnListNode,ci_pbInputBuffer.pb_lhPacketStorage,ppaPacket) {
		CPacket &paPacket = *ppaPacket;
		//! ��ũ��Ŷ�̶��,
		// if it's an acknowledge packet, remove the acknowledged packets from the wait acknowledge buffer
		if (ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) {
			ULONG *pulAck;
			SLONG slSize;
			ULONG ulSequence;
			
			slSize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
			// if slSize isn't rounded to the size of ulSequence, abort 
			ASSERT (slSize % sizeof(ULONG) == 0);
			
			// get the pointer to the start of acknowledged sequences
			pulAck = (ULONG*) (ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE);
			// for each acknowledged sequence number
			while (slSize>0) {
				ulSequence = *pulAck;
				
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Received acknowledge for packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ulSequence);
				}
				//! ��ũ���ۿ��� �ش� ������ ��Ŷ�� �����Ѵ�.
				// remove the matching packet from the wait acknowledge buffer
				ci_pbWaitAckBuffer.RemovePacket(ulSequence,TRUE);
				//! �ƿ����ۿ��� �������� ��ٸ��� �ִ� �ش� ������ ��Ŷ�� �����Ѵ�.
				// if the packet is waiting to be resent it's in the outgoing buffer, so remove it
				ci_pbOutputBuffer.RemovePacket(ulSequence,TRUE);
				pulAck++;
				slSize -= sizeof(ULONG);
			}
			//! input������ �� ��Ŷ�� �����Ѵ�.
			// take this packet out of the input buffer and kill it
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
			DeletePacket(ppaPacket);
			
			bSomethingDone = TRUE;
			//! �����̾�� ��Ŷ�̶��,
			// if the packet is reliable
		} else if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
			
			//! �� ��Ŷ�� ��ε�ĳ��Ʈ ��巡���� ���� �°Ŷ�� ��ũ��Ŷ�� �����Ͽ� ������.
			// generate packet acknowledge
			// if the packet is from the broadcast address, send the acknowledge for that packet only
			if (ppaPacket->pa_adrAddress.adr_uwID == '//' || ppaPacket->pa_adrAddress.adr_uwID == 0) {
				//0105 ACK��Ŷ �����
				if(!_bTcp4ClientInterface){
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(&(ppaPacket->pa_ulSequence),sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,'//',sizeof(ULONG));				
					if (net_bReportPackets) {
						CPrintF("Acknowledging broadcast packet sequence %d\n",ppaPacket->pa_ulSequence);
					}
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
					}
				}
				//..					
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);          
			}	else {
/* //0311 ����
				//! ��ũ��Ŷ�� �ִ�ġ50���� �������ٸ�,
				// if we have filled the packet to the maximum with acknowledges (an extremely rare event)
				// finish this packet and start the next one
				if (ulAckCount == iMaxAcks) {
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(pulGenAck,ulAckCount*sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,ci_adrAddress.adr_uwID,ulAckCount*sizeof(ULONG));
					ulAckCount = 0;
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
						break;
					}						
				}	

				//! ����Ŷ�� ��������  �����ϰ�,
				// add the acknowledge for this packet
				pulGenAck[ulAckCount] = ppaPacket->pa_ulSequence;
				
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Acknowledging packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ppaPacket->pa_ulSequence);
				}
				//! ��ũī��Ʈ�� ������Ų��.
				ulAckCount++;
*/
			}
			
			//! �ش� ��Ŷ�� input���ۿ��� �����Ѵ�.
			// take this packet out of the input buffer
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);		
			
			//! �� ��Ŷ�� ��ũ��Ŷ�̰ų�
			//! �Ǵ� ����Ȯ�� ���� ��Ŷ�̰� Ŭ���̾�Ʈ�� ���� ������ ���� �ʾҰų�, 
			//! ����Ŭ���̾�Ʈ���,		
			// a packet can be accepted from the broadcast ID only if it is an acknowledge packet or 
			// if it is a connection confirmation response packet and the client isn't already connected
			if (ppaPacket->pa_adrAddress.adr_uwID == '//' || ppaPacket->pa_adrAddress.adr_uwID == 0) {
				if  (((!ci_bUsed) && (ppaPacket->pa_uwReliable & UDP_PACKET_CONNECT_RESPONSE)) ||
					(ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) || ci_bClientLocal) {
					//! �� ��Ŷ�� �����̾�� input���ۿ� �����Ѵ�.
					ppaPacket->pa_ulSequence = 0;
					if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
						DeletePacket(ppaPacket);
					}
				}	else {
					DeletePacket(ppaPacket);
				}
				// reject duplicates
				//! �ش� ��Ŷ�� �������� �����̾�� input���ۿ��� ���������� ���� ���������� ũ��,
				//! �����̾�� input���۾ȿ� �ش� �������� ���ٸ�, �����̾�� ���ۿ� ��Ŷ�� �����Ѵ�.
			} else if (ppaPacket->pa_ulSequence > ci_pbReliableInputBuffer.pb_ulLastSequenceOut &&
				!(ci_pbReliableInputBuffer.IsSequenceInBuffer(ppaPacket->pa_ulSequence))) {
				if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}				
			} else {
				DeletePacket(ppaPacket);
			}
			
			// if the packet is unreliable, leave it in the input buffer
			// when it is needed, the message will be pulled from there
		} else {
			//! �ش� ��Ŷ�� �������� input���ۿ��� ���������� ���� ���������� ũ�ٸ�,
			// reject duplicates			
			if (ppaPacket->pa_ulSequence > ci_pbInputBuffer.pb_ulLastSequenceOut) { 
				//! �ش� ��Ŷ�� �������� ���� ��Ŷ�� 1������ ���ٸ�, ��Ŷ ����.
				if (ci_pbReliableInputBuffer.GetSequenceRepeat(ppaPacket->pa_ulSequence) > 1) {										
					ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,TRUE);
				}				
			} else {
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,TRUE);
			}
			
		}
	}
/* //0311 ����
	// if there are any remaining unsent acknowldges, put them into a packet and send it
	//0105 ack��Ŷ ���ֱ�.	
	if(!_bTcp4ClientInterface){
		//! ��ũ ī��Ʈ�� �ִٸ�, ��ũ��Ŷ�� �����Ͽ� output���ۿ� �ִ´�. 
		//! ��ũī��Ʈ�� ������ �־ �ѹ��� ������.
		if (ulAckCount >0) {
			CPacket *ppaAckPacket = CreatePacket();
			ppaAckPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
			ppaAckPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
			ppaAckPacket->WriteToPacket(pulGenAck,ulAckCount*sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,ci_adrAddress.adr_uwID,ulAckCount*sizeof(ULONG));
			if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
				DeletePacket(ppaAckPacket);
			}
		}	
	}
*/
	//..
	return TRUE;
	
};


// update socket input buffer (transfer from input buffer to the reliable buffer...),
// for incoming acknowledge packets, remove acknowledged packets from the output buffers,
// and generate acknowledge messages for incoming reliable packets
// this method is different than the previous becoause it sends acknowledges for each 
// packet separately, instead of grouping them together
BOOL CClientInterface::UpdateInputBuffersBroadcast(void)
{
	BOOL bSomethingDone;
	CTimerValue tvNow;
	//! ��ε�ĳ��Ʈ�� Input���ۿ� ��Ŷ�� �ִٸ�,
	// if there are packets in the input buffer, process them
	FORDELETELIST(CPacket,pa_lnListNode,ci_pbInputBuffer.pb_lhPacketStorage,ppaPacket) {
		CPacket &paPacket = *ppaPacket;
			//! �� ��Ŷ�� ��ٸ��� ��ũ��Ŷ�̶�� ��ũ���ۿ� �ִ� �ش���Ŷ�� �����Ѵ�.
			// if it's an acknowledge packet, remove the acknowledged packets from the wait acknowledge buffer
			if (ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) {
				ULONG *pulAck;
				SLONG slSize;
				ULONG ulSequence;

				slSize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				// if slSize isn't rounded to the size of ulSequence, abort 
				ASSERT (slSize % sizeof(ULONG) == 0);
				//!��ũ��Ŷ�� ������ ����.
				// get the pointer to the start of acknowledged sequences
				pulAck = (ULONG*) (ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE);
				// for each acknowledged sequence number
				while (slSize>0) {
					ulSequence = *pulAck;

					// report the packet info to the console
					if (net_bReportPackets) {
						tvNow = _pTimer->GetHighPrecisionTimer();
						CPrintF("%lu: Received acknowledge for broadcast packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ulSequence);
					}
					//! ��ũ���ۿ��� �� ��Ŷ�� �����Ѵ�.
					// remove the matching packet from the wait acknowledge buffer
					ci_pbWaitAckBuffer.RemovePacket(ulSequence,TRUE);
					//! �� ��Ŷ�� �������� ���� �ƿ����ۿ� �ִٸ� �� ��Ŷ�� �����Ѵ�.
					// if the packet is waiting to be resent it's in the outgoing buffer, so remove it
					ci_pbOutputBuffer.RemovePacket(ulSequence,TRUE);
					pulAck++;
					slSize -= sizeof(ULONG);
				}
                //! input������ ��Ŷ �����Ѵ�.
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
				bSomethingDone = TRUE;
				DeletePacket(ppaPacket);
			//! �����̾�� ��Ŷ�̶��,
			// if the packet is reliable
			} else if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
//0105 Ack��Ŷ �������� �ʱ� ���� ��� �����.
				if(!_bTcp4ClientInterface){				
					//! ��ũ ��Ŷ�� �����Ͽ� �ƿ����۷� ������.
					// generate packet acknowledge (each reliable broadcast packet is acknowledged separately 
					// because the broadcast interface can receive packets from any number of different addresses
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(&(ppaPacket->pa_ulSequence),sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,ci_ulSequence++,ppaPacket->pa_adrAddress.adr_uwID,sizeof(ULONG));
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
					}
				}
//..
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Acknowledging broadcast packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ppaPacket->pa_ulSequence);
				}
				//! �׷��Ŀ� input������ �� ��Ŷ�� �����ϰ�,
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
				//! �����̾�� input���ۿ� �ִ´�.
				if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}				
			} else {
			//! �� ��Ŷ�� �𸱶��̾�� ��Ŷ�̶�� input ���ۿ� ���ܳ��´�.
			// if the packet is unreliable, leave it in the input buffer
			// when it is needed, the message will be pulled from there
			// have to check for duplicates	
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
			if (!(ci_pbReliableInputBuffer.IsSequenceInBuffer(ppaPacket->pa_ulSequence))) {						
				//! input ���ۿ� �ִ´�.
				if (ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}
			} else {
				DeletePacket(ppaPacket);
			}

		}
	}

	return TRUE;
							
};


// take a look at the wait acknowledge buffer and resend any packets that heve reached the timeout
// if there is a packet that can't be sent sucessfully (RS_NOTATALL), signal it
BOOL CClientInterface::UpdateOutputBuffers(void)
{
	CPacket* ppaPacket;
	UBYTE ubRetry;

	//! ��ũ���ۿ� ��Ŷ�� �ִٸ� 
	// handle resends
	while (ci_pbWaitAckBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pbWaitAckBuffer.PeekFirstPacket();
		
		ubRetry = ppaPacket->CanRetry();
		switch (ubRetry) {
			//! ��ũ���ۿ��� ��Ŷ�� ������ Output���ۿ� �ִ´�.
			// if it's time to retry sending the packet
			case RS_NOW: {	ci_pbWaitAckBuffer.RemoveFirstPacket(FALSE);
											ci_pbOutputBuffer.Retry(*ppaPacket);
											break;
									 }
			// if the packet cannot be sent now, no other packets can be sent, so exit
			case RS_NOTNOW: { return TRUE; }
			// if the packet has reached the retry limit - close the client's connection
			case RS_NOTATALL: { 
//0105 2line �����
//				Clear(); 
//									return FALSE;
//0105
				if(!_bTcp4ClientInterface){
					Clear(); // ACK 
					return FALSE;
				}else{
					//! ���� ������ �������� �� ��Ŷ�� ��ũ���ۿ��� ����������.
					ci_pbWaitAckBuffer.Clear();
					return TRUE;
				}
//..
								
												}
			

		}
	}
	return TRUE;
};


// get the next available packet from the output buffer
CPacket* CClientInterface::GetPendingPacket(void)
{
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	if (ci_pbOutputBuffer.pb_ulNumOfPackets == 0) {
		return NULL;
	}

	CPacket* ppaPacket = ci_pbOutputBuffer.PeekFirstPacket();
	
	// if it's time to send the packet
	if (ppaPacket->pa_tvSendWhen <= tvNow) {
		ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
		return ppaPacket;
	}

	return NULL;

};

//! �����̾�� input������ ù��° ��Ŷ�� pa_slTransferSize�� �����Ѵ�.
// reads the expected size of current realiable message in the reliable input buffer
SLONG CClientInterface::GetExpectedReliableSize(void)
{
  if (ci_pbReliableInputBuffer.pb_ulNumOfPackets == 0) {
    return 0;
  }
  CPacket* ppaPacket = ci_pbReliableInputBuffer.PeekFirstPacket();
	return ppaPacket->pa_slTransferSize;
};

//! ���� �������� �ִ� �����̾�� ��Ŷ�� �� ����� �����Ѵ�.
// reads the expected size of current realiable message in the reliable input buffer
SLONG CClientInterface::GetCurrentReliableSize(void)
{
  SLONG slSize;
  ci_pbReliableInputBuffer.CheckSequence(slSize,TRUE);
	return slSize;
};
