using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Net.Sockets;
using System.Net;
using System.IO;
using System.Text;
public class ClientManager : Singleton_Ver2.Singleton<ClientManager>,IDisposable
{
    struct t_Client
    {
        public Byte[] m_SendBuf;
        public Byte[] m_RecvBuf;
    }
    #region
    private PacketManager M_Packet;
    private const string SERVERIP = "127.0.0.1";
    private const int SERVERPORT = 9000;
    private const int BUFSIZE = 4096;

    bool is_connected;
    TcpClient m_socket;
    NetworkStream m_stream;



    private t_Client m_client;
    private uint m_spacketNo;
    private uint m_rpacketNo;
    #endregion

    public void ConnectToTcpServer()
    {
        if (is_connected)
            return;
        try
        {
            m_socket = new TcpClient(SERVERIP, SERVERPORT);
            m_stream = m_socket.GetStream();
            is_connected = true;
        }
        catch (Exception e)
        {
            Debug.Log($"sokcet error: {e.Message} ");
        }
    }
    public bool Send(Byte[] _sendbuf)
    {
        Byte[] sendpacket;
        // packetno 붙이고 전송.
        int size = M_Packet.Packing(m_spacketNo, _sendbuf, out sendpacket);
        m_spacketNo++;
        try
        {
            m_stream.Write(sendpacket, 0, sendpacket.Length);
            m_stream.Flush();
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
        Byte[] packetno = new Byte[sizeof(int)];
        Byte[] delete_packet;
        int packetnumber = -1;
        int size = 0;
        _recvbuf = new byte[BUFSIZE];

        try
        {
            size = Recvn(sizebuffer, sizeof(int));
            if (size == 0)
                return false;
            size = Recvn(packetno, sizeof(int));
            if (size == 0)
                return false;
            
            Debug.Log("packno:"+ BitConverter.ToInt32(packetno));
            //if (m_rpacketNo != BitConverter.ToInt32(packetno))
            //{
            //    delete_packet = new Byte[int.Parse(sizebuffer.ToString())];
            //    size = Recvn(_recvbuf, int.Parse(sizebuffer.ToString()));
            //    return false;
            //}
            //else
            //{ 
                //packno는 읽어왔기때문에 sizeof(int)를 빼준다
                size = Recvn(_recvbuf, BitConverter.ToInt32(sizebuffer)-sizeof(int));
                if (size == 0)
                    return false;
            //}
            //비워주는거 있어야함 stream. 

            m_rpacketNo++;
            return true;
        }
        catch (Exception ex)
        {
            Debug.Log(ex.ToString());
            return false;
        }

    }

    public void Init()
    {
        M_Packet = PacketManager.Instance;
        m_spacketNo = 1;
        m_rpacketNo = 1;
        m_client.m_SendBuf = new Byte[BUFSIZE];
        m_client.m_RecvBuf = new Byte[BUFSIZE];
        Array.Clear(m_client.m_SendBuf, 0x0, m_client.m_SendBuf.Length);
        Array.Clear(m_client.m_RecvBuf, 0x0, m_client.m_RecvBuf.Length);
    }
    public void End()
    {
      
    }
    private void CloseSocket()
    {
        if (!is_connected)
            return;
        m_stream.Close();
        m_socket.Close();
        m_stream.Dispose();
        m_socket.Dispose();
        is_connected = false;
    }
    private int Recvn(Byte[] _recvbuf, int size)
    {
        int retval = 0;
        int recvbytes = 0;
        int left = size;
        while (left > 0)
        {
            try
            {
                retval = m_stream.Read(_recvbuf, recvbytes, left);
                left -= retval;
                recvbytes += retval;
            }
            catch (Exception ex)
            {
                Debug.Log($"read error : {ex.Message}");
                return 0;
            }
        }
        return (size - left);
    }

    public void Dispose()
    {
        CloseSocket();
    }
}
