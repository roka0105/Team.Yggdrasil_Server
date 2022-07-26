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
        protected uint m_stream_size;
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

        }
    }
    public class RecvPacket : Packet
    {
        public void GetDataFromNetworkStream(NetworkStream _netstream,int _copysize)
        {

        }
        public void UnPacking()
        {

        }

    }

}
