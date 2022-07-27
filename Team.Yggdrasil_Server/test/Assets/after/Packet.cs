using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Net.Sockets;
namespace Net
{
    public class Packet
    {
        public const int m_capacity = 4096;
        protected MemoryStream m_stream;
        protected int m_stream_size;
        public virtual void __Initialize()
        {
            m_stream = new MemoryStream(m_capacity);
            m_stream.Position = 0;
        }
    }
    public class SendPacket:Packet
    {
        public void Packing(byte[] _sendstream,uint _packetid)
        {
            //데이터와 패킷 id 합치기.
        }
    }
    public class RecvPacket : Packet
    {
        public void GetDataFromNetworkStream(NetworkStream _netstream,int _copysize)
        {
            m_stream.SetLength(_copysize);
            m_stream_size = _copysize;
            _netstream.Read(m_stream.GetBuffer(), 0, m_stream_size);
            m_stream.Position = 0;
        }
        
        public void UnPacking()
        {
            // 복호화.
        }

    }

}
