using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System;
using System.Threading;
using System.Net;

namespace Net
{
    public class NetSession : MonoBehaviour
    {
        private const string SERVERIP = "127.0.0.1";
        private const int SERVERPORT = 9000;
        private const int BUFSIZE = 4096;

        TcpClient m_client;
        NetworkStream m_netstream;

        private byte[] m_recvstream;
        private byte[] m_sendstream;

        private uint m_send_packetNo;
        private uint m_recv_packetNo;

        private Queue<SendPacket> m_send_queue;
        private Queue<RecvPacket> m_recv_queue;

        private PacketManager M_Packet;
        

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

            is_loging=false;
            try
            {
                m_client = new TcpClient();
                IPEndPoint serverAddr = new IPEndPoint(IPAddress.Parse(SERVERIP), SERVERPORT);
                m_client.Connect(serverAddr);
                m_netstream = m_client.GetStream();
            }
            catch(Exception e)
            {
                Debug.Log($"socket error: {e.Message} ");
            }
           
            m_recvstream = new byte[BUFSIZE];
            m_sendstream = new byte[BUFSIZE];
           
        }
        private void __Initialize_State()
        {

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
        }
        public bool Send(SendPacket _sendbuf)
        {
           
        }
        public bool Recv()
        {
            try
            {
                byte[] size_bytes = new byte[sizeof(int)];
                int packet_size = 0;
                m_netstream.Read(size_bytes, 0, sizeof(int));
                packet_size = BitConverter.ToInt32(size_bytes);

                byte[] no_bytes = new byte[sizeof(int)];
                int packet_no = 0;
                m_netstream.Read(no_bytes, 0, sizeof(int));
                packet_no = BitConverter.ToInt32(no_bytes);

                if (packet_no < m_recv_packetNo)
                {
                    return false;
                }

                RecvPacket recvpacket = new RecvPacket();
                recvpacket.__Initialize();

                recvpacket.GetDataFromNetworkStream(m_netstream, packet_size);
                recvpacket.UnPacking();
            }
            catch(Exception e)
            {
                Debug.Log($"recv error ({e.Message})");
                return false;
            }
            return true;
        }
        private int Recvn(RecvPacket _recvbuf, int size)
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
        public void RecvComplete(byte[] _recvpacket)
        {
            //m_curstate->RecvComplete();
        }
    }
}

