using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEngine;
public class GUIManager : Singleton_Ver2.Singleton<GUIManager>
{
    private void Start()
    {
        LobbyGUIManager.Instance.__Initialize();
    }
}
