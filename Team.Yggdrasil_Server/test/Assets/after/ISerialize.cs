using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
namespace Net
{
    public interface ISerialize
    {
        public abstract int Serialize(MemoryStream _stream);
        public abstract int Deserialize(MemoryStream _stream);
    }
}