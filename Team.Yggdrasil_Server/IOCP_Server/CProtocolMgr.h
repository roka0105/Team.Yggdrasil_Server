#pragma once

enum class MAINPROTOCOL
{
	NONE,
	INIT,
	LOGIN,
	LOBBY,
	ROOM,
	GAME,
	TEST,
	MAX
};

class CProtocolMgr
{
public:
	static CProtocolMgr* GetInst();
	static void Create();
	static void Destroy();

	unsigned long GetMainProtocol(unsigned long _protocolmem);
	unsigned long GetSubProtocol(unsigned long _protocolmem);
	unsigned long GetDetailProtocol(unsigned long _protocolmem);

	bool CheckDetailProtocol(unsigned long _protocolmem, unsigned long _protocoltype);

	void AddMainProtocol(unsigned long* _protocolmem, unsigned long _protocoltype);
	void AddSubProtocol(unsigned long* _protocolmem, unsigned long _protocoltype);
	void AddDetailProtocol(unsigned long* _protocolmem, unsigned long _protocoltype);
	void AddPreDetailProtocol(unsigned long* _protocolmem, unsigned long _protocoltype);
	void AddRearDetailProtocol(unsigned long* _protocolmem, unsigned long _protocoltype);
private:
	CProtocolMgr() {}
	~CProtocolMgr() {}
	static CProtocolMgr* instance;

	const int mainshift = 24;
	const int subshift = 16;
	const int byteshift = 8;

	const unsigned long or_mainbin = 0xff000000;
	const unsigned long or_subbin = 0x00ff0000;
	const unsigned long or_detailbin = 0x0000ffff;
	const unsigned long or_predetailbin = 0x0000ff00;
	const unsigned long or_reardetailbin = 0x000000ff;
};

