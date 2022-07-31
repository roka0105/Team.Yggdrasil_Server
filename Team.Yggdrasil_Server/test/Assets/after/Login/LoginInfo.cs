using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;


public class LoginInfo : Net.ISerialize
{
    [SerializeField] string m_id;
    [SerializeField] string m_pw;
    [SerializeField] string m_nick;

    public LoginInfo(string _id,string _pw,string _nick)
    {
        m_id = _id;
        m_pw = _pw;
        m_nick = _nick;
    }
    public string GetID
    {
        get => m_id;
    }
    public string GetNick
    {
        get => m_nick;
    }
    public int Deserialize(MemoryStream _stream)
    {
        int size = 0;
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_id);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_pw);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_nick);
        return size;
    }

    public int Serialize(MemoryStream _stream)
    {
        int size = 0;
        size += Net.StreamReadWriter.WriteToStream(_stream, m_id);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_pw);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_nick);
        return size;
    }
    public void Clear()
    {
        m_id = string.Empty;
        m_pw = string.Empty;
        m_nick = string.Empty;
    }
    public bool Empty()
    {
        if(m_id!=string.Empty)
        {
            return true;
        }
        return false;
    }
}


