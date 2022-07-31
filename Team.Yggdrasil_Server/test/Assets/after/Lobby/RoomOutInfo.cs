using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class RoomOutInfo : Net.NetObjectInfo
{
    //방 아이디 -> 상위 클래스에 저장될거임.
    private string m_title;        //방 이름
    private string m_pw;           //비밀 번호
    private int m_cur_count;       //방 현재 인원 정보
    private int m_maxenter_count;  //방 입장 가능 수
    private int m_game_mode;       //게임 모드 정보

    #region property
    public string GetTitle
    {
        get => m_title;
    }
    public string GetPw
    {
        get => m_pw;
    }
    public int GetCurCount
    {
        get => m_cur_count;
    }
    public int GetMaxEnterCount
    {
        get => m_maxenter_count;
    }
    public int GetGameMode
    {
        get => m_game_mode;
    }
    #endregion
    public RoomOutInfo() : base(ENetObjectType.Room)
    {

    }
    override public int Deserialize(MemoryStream _stream)
    {
        int size = 0;
        size += base.Serialize(_stream);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_title);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_pw);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_cur_count);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_maxenter_count);
        size += Net.StreamReadWriter.ReadFromStream(_stream, out m_game_mode);
        return size;
    }

    override public int Serialize(MemoryStream _stream)
    {
        int size = 0;
        size += base.Serialize(_stream);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_title);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_pw);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_cur_count);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_maxenter_count);
        size += Net.StreamReadWriter.WriteToStream(_stream, m_game_mode);
        return size;
    }
}

