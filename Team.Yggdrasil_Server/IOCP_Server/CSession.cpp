#include "pch.h"
#include "CSession.h"

void CSession::Init()
{
	ZeroMemory(&r_overlap.overlapped, sizeof(OVERLAPPED));
	r_overlap.type = IO_TYPE::RECV;
	r_overlap.session = this;

	ZeroMemory(&s_overlap.overlapped, sizeof(OVERLAPPED));
	s_overlap.type = IO_TYPE::SEND;
	s_overlap.session = this;

	m_player = nullptr;
	
}

void CSession::End() // removesession시 마무리작업하는 함수
{
	CloseSocket();
	//base
	//CPacket::Packing();
	if(m_player!=nullptr)
	delete m_player;
}
