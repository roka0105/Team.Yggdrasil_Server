using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


public class PlayerInfo : Net.NetObjectInfo
{
    //id는 상위 클래스 id 인데 그 id는 로그인 정보 id가 아니라
    //방에서 관리할 player에 관한 id 값.
    private string m_nick;
    private CharacterInfo m_character_info;

    #region property
    public string GetNick
    {
        get => m_nick;
    }
    public CharacterInfo GetCharacterInfo
    {
        get => m_character_info;
    }
    #endregion
    public PlayerInfo() : base(ENetObjectType.Player)
    {

    }
    //public int Deserialize(MemoryStream _stream)
    //{

    //}

    //public int Serialize(MemoryStream _stream)
    //{

    //}
}

