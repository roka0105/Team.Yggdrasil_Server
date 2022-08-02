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
        // 그 외의 캐릭터가 받아야 할 정보들은 나중에 기획서와 클라 보면서 추가.
        //public int Deserialize(MemoryStream _stream)
        //{
            
        //}

        //public int Serialize(MemoryStream _stream)
        //{
          
        //}
    }

