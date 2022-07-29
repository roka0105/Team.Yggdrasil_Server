using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
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
        public int Write<T>(T _item) where T : IConvertible
        {
            return Net.StreamReadWriter.WriteToStream(m_stream,_item);
        }
        public int Write<T>(List<T>_item)where T:IConvertible
        {
            return Net.StreamReadWriter.WriteToStream(m_stream, _item);
        }
        public int Write(ISerialize _item)
        {
            return Net.StreamReadWriter.WriteToStream(m_stream, _item);
        }
        public int Packing(byte[] _sendstream,int _packetid)
        {
            //암호화
            //데이터와 패킷 id 합치기.
            int point = 0;
            int total_size = (int)m_stream.Length + sizeof(int)+sizeof(int);
            //총사이즈 넣고
            byte[] totalsize_bytes=BitConverter.GetBytes(total_size);
            Buffer.BlockCopy()
            //데이터 넣고
          
            return point;
        }
    }
    public class RecvPacket : Packet
    {
        public delegate int RecvFunc(byte[] _recvbuf, int _size);
        private RecvFunc Recvn; 
        public void RecvnFuncRegister(RecvFunc _func)
        {
            Recvn = _func;
        }
        public void GetDataFromNetworkStream(NetworkStream _netstream,int _copysize)
        {
            m_stream.SetLength(_copysize);
            m_stream_size = _copysize;
            Recvn.Invoke(m_stream.GetBuffer(), m_stream_size);
            _netstream.Read(m_stream.GetBuffer(), 0, m_stream_size);
            m_stream.Position = 0;
        }
        public int Read<T>(out T _item) where T :IConvertible
        {
            return Net.StreamReadWriter.ReadFromStream(m_stream, out _item);
        }
        public int Read<T>(out List<T> _item) where T:IConvertible
        {
            return Net.StreamReadWriter.ReadFromStream(m_stream, out _item);
        }
        public int ReadSerialize<T>(out T derived) where T:ISerialize,new()
        {
            return Net.StreamReadWriter.ReadFromStreamSerialize(m_stream, out derived);
        }
        public int ReadSerialize<T>(out List<T>derived)where T:ISerialize,new()
        {
            return Net.StreamReadWriter.ReadFromStreamSerialize(m_stream,out derived);
        }
        public void UnPacking()
        {
            // 복호화.
        }

    }

}
