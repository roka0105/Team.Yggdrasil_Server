using System.Collections;
using System.Collections.Generic;
using UnityEngine;

struct RoomOutInfo
{
    uint id;
    string name;
    uint mode;
    uint enter_limit;
    uint enter_count;
}
struct RoomInInfo
{
    //입장한 유저들 렌더링,아이디 정보
    //stage 정보 
    //유저들의 레디 상태
    //방장 정보
}
public class RoomManager : Singleton_Ver2.Singleton<RoomManager>
{
    enum SUBPROTOCOL
    {
        NONE = -1,
		SetRoomInfo,
		MAX
    };
    enum DETAILPROTOCOL
    {
        NONE = -1,
		PageInfo,
        MAX
    };


    private MainThread M_MainTh;
    private ProtocolManager M_Protocol;
    //n 페이지에 해당하는 room 정보들.
    private Dictionary<int,List<RoomOutInfo>> m_rooms;
    //한 페이지당 가지고 있을 room 제한 수.
    private const uint m_pagelimit_room = 10;
    private RoomInInfo m_InroomInfo;

    private delegate void Result_Process(uint _protocol , byte[] _recvdata);
    private Dictionary<SUBPROTOCOL, Result_Process> m_ResultProcess;
  
    //방 정보 빼오는 함수 있어야함 (로비용)
    //방 정보 빼오는 함수 있어야함 (게임룸용)
    private void ResultProcess(uint _protocol, byte[] _recvbuf)
    {
        uint subprotocol = M_Protocol.GetSubProtocol(_protocol);
        m_ResultProcess[(SUBPROTOCOL)subprotocol].Invoke(_protocol,_recvbuf);
    }
    private void RoomInfoResult(uint _protocol,byte[] _recvbuf)
    {
        uint detailprotocol = M_Protocol.GetDetailProtocol(_protocol);
        switch((DETAILPROTOCOL)detailprotocol)
        {
            case DETAILPROTOCOL.PageInfo:
                //unpack 페이지정보. 방정보
                break;
        }
    }
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;

        m_ResultProcess = new Dictionary<SUBPROTOCOL, Result_Process>();
        m_rooms = new Dictionary<int, List<RoomOutInfo>>();

        M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.ROOM,ResultProcess);
        
        m_ResultProcess.Add(SUBPROTOCOL.SetRoomInfo, RoomInfoResult);
    }
    public void End()
    {

    }
    void Start()
    {
        
    }

    void Update()
    {
        
    }

}
