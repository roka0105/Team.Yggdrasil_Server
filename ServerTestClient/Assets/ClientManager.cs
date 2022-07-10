using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Net.Sockets;
using System.Net;
public class ClientManager : Singleton_Ver2.Singleton<ClientManager>
{
    struct t_Client
    {
        public Byte[] m_SendBuf;
        public Byte[] m_RecvBuf;
    }
    #region
    private PacketManager M_Packet = PacketManager.Instance;
    private const string SERVERIP = "127.0.0.1";
    private const int SERVERPORT = 9000;
    private const int BUFSIZE = 4096;

    private Socket m_Sock;
    private IPEndPoint m_serverIpEndPoint;

    private t_Client m_client;
    private uint m_spacketNo;
    private uint m_rpacketNo;
    #endregion

    public void ConnectToTcpServer()
    {
        try
        {
            m_serverIpEndPoint = new IPEndPoint(IPAddress.Parse(SERVERIP), SERVERPORT);
            m_Sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            m_Sock.Connect(m_serverIpEndPoint);
        }
        catch (Exception e)
        {
            Debug.Log("On Client connect exception " + e);
        }
    }
    public bool Send(Byte[] _sendbuf)
    {
        Byte[] sendpakcet;
        // packetno 붙이고 전송.
        int size = M_Packet.Packing(m_spacketNo, _sendbuf, out sendpakcet);
        try
        {
            m_Sock.Send(sendpakcet, 0, size, SocketFlags.None);
            return true;
        }
        catch (Exception ex)
        {
            Debug.Log(ex.ToString());
            return false;
        }

    }
    public bool Recv(out Byte[] _recvbuf)
    {
        Byte[] sizebuffer = new Byte[sizeof(int)];
        int size = 0;
        _recvbuf = new byte[BUFSIZE];
        if (m_Sock.Available != 0)
        {
            try
            {
                size = m_Sock.Receive(sizebuffer, sizeof(int), SocketFlags.None);
                if (size == 0)
                    return false;
                size = m_Sock.Receive(_recvbuf, int.Parse(sizebuffer.ToString()), SocketFlags.None);
                if (size == 0)
                    return false;

                return true;
            }
            catch (Exception ex)
            {
                Debug.Log(ex.ToString());
                return false;
            }
        }
        return false;
    }

    public void Init()
    {
        m_spacketNo = 0;
        m_rpacketNo = 0;
        m_client.m_SendBuf = new Byte[BUFSIZE];
        m_client.m_RecvBuf = new Byte[BUFSIZE];
        Array.Clear(m_client.m_SendBuf, 0x0, m_client.m_SendBuf.Length);
        Array.Clear(m_client.m_RecvBuf, 0x0, m_client.m_RecvBuf.Length);
    }


}
