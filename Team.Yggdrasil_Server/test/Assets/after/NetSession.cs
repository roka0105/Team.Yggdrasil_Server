using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System;
using System.Threading;
using System.Net;
using System.IO;

namespace Net
{
    public class NetSession : MonoBehaviour
    {
        private const string SERVERIP = "127.0.0.1";
        private const int SERVERPORT = 9000;
        private const int BUFSIZE = 4096;

        TcpClient m_client;
        IState m_curstate;
        LoginState m_Loginstate;
        NetworkStream m_netstream;

        private byte[] m_recvstream;
        private byte[] m_sendstream;

        private int m_send_packetNo;
        private int m_recv_packetNo;

        private Queue<SendPacket> m_send_queue;
        private Queue<RecvPacket> m_recv_queue;


        bool is_loging;

        public void __Initialize()
        {
            __Initialize_Vars();
            __Initialize_State();
        }
        #region Init func
        private void __Initialize_Vars()
        {
            m_send_packetNo = 1;
            m_recv_packetNo = 1;

            is_loging = false;
            try
            {
                m_client = new TcpClient();
                IPEndPoint serverAddr = new IPEndPoint(IPAddress.Parse(SERVERIP), SERVERPORT);
                m_client.Connect(serverAddr);
                m_netstream = m_client.GetStream();
            }
            catch (Exception e)
            {
                Debug.Log($"socket error: {e.Message} ");
            }

            m_recvstream = new byte[BUFSIZE];
            m_sendstream = new byte[BUFSIZE];

            m_send_queue = new Queue<SendPacket>();
            m_recv_queue = new Queue<RecvPacket>();

        }
        private void __Initialize_State()
        {
            m_Loginstate = new LoginState(this);

            m_curstate = m_Loginstate;
        }
        #endregion
        public void __Finalize()
        {
            if (m_netstream != null)
                m_netstream.Close();
            if (m_client != null)
                m_client.Close();
        }
        //큐에서 데이터를 뽑아서 전송한다.
        public void SendQueueProcess()
        {
            if (m_send_queue.Count > 0)
            {
                SendPacket sendbuf = m_send_queue.Dequeue();
                Send(sendbuf);
            }
        }
        //큐에 보낼 패킷을 넣는다.
        public void SendReq(SendPacket _sendpacket)
        {
            m_send_queue.Enqueue(_sendpacket);
        }
        public void SendComplete()
        {
            //cur_state -> sendcomplete()
            m_curstate.SendComplete();
        }
        public void Send(SendPacket _sendbuf)
        {
            int send_size = _sendbuf.Packing(m_sendstream, m_send_packetNo++);
            m_netstream.Write(m_sendstream, 0, send_size);
            SendComplete();
        }
        public void RecvQueueProcess()
        {
            if (m_recv_queue.Count != 0)
            {
                RecvPacket recvpacket = m_recv_queue.Dequeue();
                RecvComplete(recvpacket);
            }
        }
        public bool Recv()
        {
            if(m_netstream.DataAvailable)
            {
                try
                {
                    byte[] size_bytes = new byte[sizeof(int)];
                    int packet_size = 0;

                    Recvn(size_bytes, sizeof(int));
                    packet_size = BitConverter.ToInt32(size_bytes);

                    byte[] no_bytes = new byte[sizeof(int)];
                    int packet_no = 0;
                    Recvn(no_bytes, sizeof(int));
                    packet_no = BitConverter.ToInt32(no_bytes);

                    if (packet_no < m_recv_packetNo)
                    {
                        return false;
                    }

                    RecvPacket recvpacket = new RecvPacket();
                    recvpacket.__Initialize();
                    recvpacket.RecvnFuncRegister(Recvn);
                    recvpacket.GetDataFromNetworkStream(m_netstream, packet_size-sizeof(int)/*packetno size*/);
                    recvpacket.UnPacking();
                    m_recv_queue.Enqueue(recvpacket);
                }
                catch (Exception e)
                {
                    Debug.Log($"recv error ({e.Message})");
                    return false;
                }
            }
            
            return false;
        }
        private int Recvn(byte[] _recvbuf, int size)
        {
            int retval = 0;
            int recvbytes = 0;
            int left = size;
            while (left > 0)
            {
                try
                {
                    retval = m_netstream.Read(_recvbuf, recvbytes, left);
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
        public void RecvComplete(RecvPacket _recvpacket)
        {
            //m_curstate->RecvComplete();
            m_curstate.RecvComplete(_recvpacket);
        }
       
    }
}

