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


void CSession::RenderQueue_Push(HexTile* tile)
{
	//렌더를 해야하는 타일이 갱신되는 시점은 플레이어가 이동했을 때인데 
	//이동하고 해당하는 타일들의 시간 갱신 후 queue에 넣어주고 
	//time 기준으로 제일 갱신이 안된지 오래된거가 맨뒤로 밀려나게해서
	//용량이 꽉 차면 맨 뒤에있는걸 빼고 새로들어온걸 넣고 정렬.
}
