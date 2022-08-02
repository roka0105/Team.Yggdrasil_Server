using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Text;
using System;
namespace Net
{
    public class NetObjectInfo : ISerialize
    {
        public enum ENetObjectType
        {
            None,
            Room,
            Player,
            Boss,
            Spirit,
            Monster,
        }
        [SerializeField] uint m_id;
        ENetObjectType m_type;

        public uint GetID { get => m_id; }
        public ENetObjectType GetNetObjectType { get => m_type; }
        public NetObjectInfo(ENetObjectType _type)
        {
            m_type = _type;
        }
        virtual public int Deserialize(MemoryStream _stream)
        {
            int size = 0;
            size += Net.StreamReadWriter.ReadFromStream(_stream,out m_id);
            int type = 0;
            size += Net.StreamReadWriter.ReadFromStream(_stream,out type);
            m_type = (ENetObjectType)type;
            return size;
        }

        virtual public int Serialize(MemoryStream _stream)
        {
            int size = 0;
            size += Net.StreamReadWriter.WriteToStream(_stream, m_id);
            size += Net.StreamReadWriter.WriteToStream(_stream, (uint)m_type);
            return size;
        }
    }

}
