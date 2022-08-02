using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


public class PlayerInfo : Net.NetObjectInfo
{
    //id�� ���� Ŭ���� id �ε� �� id�� �α��� ���� id�� �ƴ϶�
    //�濡�� ������ player�� ���� id ��.
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

