using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

    public class CharacterInfo //: Net.ISerialize
    {
        public enum ECharacterType
        {
            Defense,
            Attack,
            Support
        }
        ECharacterType m_character_type;
        #region property
        public ECharacterType GetCharacterType
        {
            get => m_character_type;
        }
        #endregion
        // �� ���� ĳ���Ͱ� �޾ƾ� �� �������� ���߿� ��ȹ���� Ŭ�� ���鼭 �߰�.
        //public int Deserialize(MemoryStream _stream)
        //{
            
        //}

        //public int Serialize(MemoryStream _stream)
        //{
          
        //}
    }

