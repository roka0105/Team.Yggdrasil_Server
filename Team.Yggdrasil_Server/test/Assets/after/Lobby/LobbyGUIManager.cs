using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
public class LobbyGUIManager : Singleton_Ver2.Singleton<LobbyGUIManager>
{
    #region window object
    [SerializeField]
    GameObject m_window_createroom;
    #endregion
    #region input field object
    [SerializeField]
    TMP_InputField m_intput_chat;
    #endregion
    #region create room input field object
    [SerializeField]
    TMP_InputField m_input_createname;
    [SerializeField]
    TMP_InputField m_input_pw;
    #endregion

    public void OnClick_BeforMenu()
    {
        MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Menu, true);
        MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Lobby, false);
    }
}
