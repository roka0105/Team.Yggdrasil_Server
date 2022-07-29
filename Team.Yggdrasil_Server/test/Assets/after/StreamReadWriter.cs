using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using System.Text;
namespace Net
{
    public class StreamReadWriter : MonoBehaviour
    {
        #region read
        public static int ReadFromStream<TItem>(MemoryStream _stream, out TItem _item)
            where TItem : IConvertible
        {
            int read_size = 0;
            Type type = typeof(TItem);
            if (type == typeof(Boolean))
            {
                Boolean result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if (type == typeof(double))
            {
                double result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if (type == typeof(float))
            {
                float result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if (type == typeof(string))
            {
                string result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if (type == typeof(int))
            {
                int result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if(type==typeof(char))
            {
                char result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }
            if(type==typeof(byte))
            {
                byte result;
                read_size = ReadFromStream(_stream, out result);
                _item = (TItem)((object)result);
                return read_size;
            }

            throw new NotImplementedException();
        }
        public static int ReadFromStream(MemoryStream _stream, out Boolean _item)
        {
            int read_size = 0;
            byte[] bool_bytes = new byte[sizeof(Boolean)];
            _stream.Read(bool_bytes, 0, sizeof(Boolean));
            _item = BitConverter.ToBoolean(bool_bytes);
            read_size += sizeof(Boolean);
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream, out double _item)
        {
            int read_size = 0;
            byte[] double_bytes = new byte[sizeof(double)];
            _stream.Read(double_bytes, 0, sizeof(double));
            _item = BitConverter.ToSingle(double_bytes);
            read_size += sizeof(double);
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream, out float _item)
        {
            int read_size = 0;
            byte[] float_bytes = new byte[sizeof(float)];
            _stream.Read(float_bytes, 0, sizeof(float));
            _item = BitConverter.ToSingle(float_bytes);
            read_size += sizeof(float);
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream, out int _item)
        {
            int read_size = 0;
            byte[] size_bytes = new byte[sizeof(int)];
            _stream.Read(size_bytes, 0, sizeof(int));
            _item = BitConverter.ToInt32(size_bytes);
            read_size += sizeof(int);
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream, out string _item)
        {
            int read_size = 0;
            int strsize = 0;

            read_size += ReadFromStream(_stream, out strsize);

            byte[] str_bytes = new byte[strsize];
            _stream.Read(str_bytes, 0, strsize);
            _item = Encoding.Unicode.GetString(str_bytes);
            read_size += strsize;

            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream,out ulong _item)
        {
            int read_size = 0;
            byte[] long_bytes = new byte[sizeof(ulong)];
            _stream.Read(long_bytes, 0, sizeof(ulong));
            _item = BitConverter.ToUInt64(long_bytes);
            read_size += sizeof(ulong);
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream,out char _item)
        {
            int read_size = 0;
            byte[] char_bytes=new byte[sizeof(char)];
            _stream.Read(char_bytes, 0, sizeof(char));
            _item = BitConverter.ToChar(char_bytes);
            read_size += sizeof(char);
           
            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream,out byte _item)
        {
            int read_size = 0;
          
            byte[] byte_bytes = new byte[sizeof(byte)];
            read_size = _stream.Read(byte_bytes, 0, sizeof(byte));
            _item = byte_bytes[0];
            read_size += sizeof(byte);

            return read_size;
        }
        public static int ReadFromStream(MemoryStream _stream,ISerialize _item)
        {
            int read_size = 0;
            read_size = _item.Deserialize(_stream);
            return read_size;
        }
        public static int ReadFromStream<TItem>(MemoryStream _stream,out List<TItem> _list)
            where TItem:IConvertible
        {
            int read_size = 0;
            int list_count = 0;
            ReadFromStream(_stream, out list_count);
            read_size += sizeof(int);
            _list = new List<TItem>();
            for(int i=0;i<list_count;i++)
            {
                TItem item;
                read_size +=ReadFromStream(_stream, out item);
                _list.Add(item);
            }
            return read_size;
        }
        public static int ReadFromStreamSerialize<Derived>(MemoryStream _stream, out Derived _item)
            where Derived : ISerialize, new()
        {
            _item = new Derived();
           return _item.Deserialize(_stream);
        }
        public static int ReadFromStreamSerialize<Derived>(MemoryStream _stream,out List<Derived> _list)
            where Derived:ISerialize, new()
        {
            int read_size = 0;
            int list_count = 0;
            ReadFromStream(_stream, out list_count);
            read_size += sizeof(int);
            _list = new List<Derived>();
            for (int i = 0; i < list_count; i++)
            {
                Derived item;
                read_size += ReadFromStreamSerialize(_stream, out item);
                _list.Add(item);
            }
            return read_size;
        }
        #endregion
        #region write
        public static int WriteToStream<TItem>(MemoryStream _stream, TItem _item)
        {
            if (typeof(string) == typeof(TItem))
            {
                //0 대신에 string 을 메모리에 쓰는 함수 호출
                return WriteToStream(_stream, (string)((object)_item));
            }

            #region 설명
            /*
             http://www.java2s.com/Tutorials/CSharp/System/Type/C_Type_GetMethod_String_Type_.htm
             https://docs.microsoft.com/ko-kr/dotnet/api/system.reflection.methodbase.invoke?view=net-6.0
             */
            #endregion

            byte[] bytes = (byte[])typeof(BitConverter).
                GetMethod("GetBytes", new Type[] { _item.GetType() })
                .Invoke(null, new object[] { _item });

            _stream.Write(bytes, 0, bytes.Length);
            return bytes.Length;
        }
        public static int WriteToStream(MemoryStream _stream, string str)
        {
            int size = 0;
            byte[] strlength_bytes = BitConverter.GetBytes(str.Length);
            _stream.Write(strlength_bytes, 0, sizeof(int));
            size += sizeof(int);

            byte[] strbytes = Encoding.Unicode.GetBytes(str);
            _stream.Write(strbytes, 0, str.Length);
            size += str.Length;
            return size;
        }
        public static int WriteToStream<TItem>(MemoryStream _stream, List<TItem> _list)
            where TItem : IConvertible
        {
            int size = 0;
            int count = _list.Count;
            byte[] count_bytes = BitConverter.GetBytes(count);
            _stream.Write(count_bytes, 0, sizeof(int));
            size += sizeof(int);
            foreach (var item in _list)
            {
                size += WriteToStream(_stream, item);
            }
            return size;
        }
        public static int WriteToStream(MemoryStream _stream, ISerialize _item)
        {
            int size = 0;
            size += _item.Serialize(_stream);
            return size;
        }
        #endregion
    }

}
