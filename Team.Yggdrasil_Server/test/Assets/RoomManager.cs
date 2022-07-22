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
    //������ ������ ������,���̵� ����
    //stage ���� 
    //�������� ���� ����
    //���� ����
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
    //n �������� �ش��ϴ� room ������.
    //private Dictionary<int,List<RoomOutInfo>> m_rooms;
    private List<RoomOutInfo> m_rooms;
    //�� �������� ������ ���� room ���� ��.
    private const uint m_pagelimit_room = 10;
    private const uint m_enterlimit = 3;
    private RoomInInfo m_InroomInfo;

    private delegate void Result_Process(uint _protocol , byte[] _recvdata);
    private Dictionary<SUBPROTOCOL, Result_Process> m_ResultProcess;
  
    //�� ���� ������ �Լ� �־���� (�κ��)
    //�� ���� ������ �Լ� �־���� (���ӷ��)
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
                //unpack ����������. ������
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
            //���ȣ
            room.m_id = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(uint);
            //���̸�
            strsize = BitConverter.ToInt32(_recvdata, index);
            index += sizeof(uint);
            room.m_name = Encoding.Unicode.GetString(_recvdata, index, strsize);
            index += strsize;
            //����
            strsize = BitConverter.ToInt32(_recvdata, index);
            index += sizeof(int);
            room.m_password = Encoding.Unicode.GetString(_recvdata, index, strsize);
            index += strsize;
            //����
            room.m_mode = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(uint);
            //���ο�
            room.m_enter_count = BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(int);
            //�����ο�
            room.m_enter_limit= BitConverter.ToUInt32(_recvdata, index);
            index += sizeof(int);
            
            _rooms.Add(room);
        }
       
    }
}
