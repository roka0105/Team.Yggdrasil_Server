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

        private Queue<byte[]> m_send_queue;
        private Queue<byte[]> m_recv_queue;

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
                byte[] sendbuf = m_send_queue.Dequeue();
                Send(sendbuf);
            }
        }
        //큐에 보낼 패킷을 넣는다.
        public void SendReq(byte[] _sendpacket)
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
        public bool Recv(RecvPacket _recvbuf)
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

                Debug.Log("packno:" + BitConverter.ToInt32(packetno));
                //if (m_rpacketNo != BitConverter.ToInt32(packetno))
                //{
                //    delete_packet = new Byte[int.Parse(sizebuffer.ToString())];
                //    size = Recvn(_recvbuf, int.Parse(sizebuffer.ToString()));
                //    return false;
                //}
                //else
                //{ 
                //packno는 읽어왔기때문에 sizeof(int)를 빼준다
                size = Recvn(_recvbuf, BitConverter.ToInt32(sizebuffer) - sizeof(int));
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
        public void RecvComplete(byte[] _recvpacket)
        {
            //m_curstate->RecvComplete();
        }
    }
}

