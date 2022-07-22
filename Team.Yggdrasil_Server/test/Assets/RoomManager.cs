using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;
using System;
struct RoomOutInfo
{
    public uint m_id;
    public string m_name;
    public string m_password;
    public uint m_mode;
    public uint m_enter_limit;
    public uint m_enter_count;
}
struct RoomInInfo
{
    RoomOutInfo m_outinfo;
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
    private LobbyManager M_Lobby;
    //n 페이지에 해당하는 room 정보들.
    //private Dictionary<int,List<RoomOutInfo>> m_rooms;
    private List<RoomOutInfo> m_rooms;
    //한 페이지당 가지고 있을 room 제한 수.
    private const uint m_pagelimit_room = 10;
    private const uint m_enterlimit = 3;
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
                m_rooms.Clear();
                int page = 0;
                UnPacking(_recvbuf,ref page, ref m_rooms);
                M_Lobby.Page=page;

                break;
        }
    }
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;

        m_ResultProcess = new Dictionary<SUBPROTOCOL, Result_Process>();
        // m_rooms = new Dictionary<int, List<RoomOutInfo>>();
        m_rooms = new List<RoomOutInfo>();
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
    void UnPacking(byte[]_recvdata,ref int _page,ref List<RoomOutInfo> _rooms)
    {
        int index = sizeof(uint) + sizeof(int);
        int page = 0;
        int forsize = 0;
        int strsize = 0;
        page = BitConverter.ToInt32(_recvdata, index);
        index += sizeof(int);
        forsize = BitConverter.ToInt32(_recvdata, index);
        index += sizeof(int);

        for(int i=0;i<forsize;i++)
        {
            RoomOutInfo room;
            //방번호
            room.m_id = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(uint);
            //방이름
            strsize = BitConverter.ToInt32(_recvdata, index);
            index += sizeof(uint);
            room.m_name = Encoding.Unicode.GetString(_recvdata, index, strsize);
            index += strsize;
            //방비번
            strsize = BitConverter.ToInt32(_recvdata, index);
            index += sizeof(int);
            room.m_password = Encoding.Unicode.GetString(_recvdata, index, strsize);
            index += strsize;
            //방모드
            room.m_mode = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(uint);
            //방인원
            room.m_enter_count = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(int);
            //제한인원
            room.m_enter_limit= BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(int);
            
            _rooms.Add(room);
        }
       
    }
}
