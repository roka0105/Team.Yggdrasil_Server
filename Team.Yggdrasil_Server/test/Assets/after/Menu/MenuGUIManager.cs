using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class MenuGUIManager:Singleton_Ver2.Singleton<MenuGUIManager>
{
    public enum EWindowType
    {
        Start,
        Menu,
        Login,
        Join,
        Lobby,
        Room,
    }
    public enum EMenuModeType
    {
        Login,
        Menu,
    }
    #region window object
    [SerializeField]
    GameObject m_window_start;
    [SerializeField]
    GameObject m_window_menu;
    [SerializeField]
    GameObject m_window_login;
    [SerializeField]
    GameObject m_window_join;
    [SerializeField]
    GameObject m_window_lobby;
    #endregion

    #region button object
    [SerializeField] 
    Button m_btn_login;
    [SerializeField] 
    Button m_btn_join;
    [SerializeField] 
    Button m_btn_multi;
    [SerializeField] 
    Button m_btn_single;
    [SerializeField] 
    Button m_btn_option;
    [SerializeField]
    Button m_btn_logout;
    #endregion

    #region Active Window
    public void WindowActive(EWindowType _type,bool _flag)
    {
        switch(_type)
        {
            case EWindowType.Menu:
                m_window_menu.SetActive(_flag);
                break;
            case EWindowType.Start:
                m_window_start.SetActive(_flag);
                break;
            case EWindowType.Login:
                m_window_login.SetActive(_flag);
                break;
            case EWindowType.Join:
                m_window_join.SetActive(_flag);
                break;
            case EWindowType.Lobby:
                m_window_lobby.SetActive(_flag);
                break;
            case EWindowType.Room:
                
                break;
        }
    }
    #endregion
    #region Active object func
    public void MenuChange(EMenuModeType _type)
    {
        switch(_type)
        {
            case EMenuModeType.Login:
                LoginObjectActive(true);
                MenuObjectActive(false);
                break;
            case EMenuModeType.Menu:
                LoginObjectActive(false);
                MenuObjectActive(true);
                break;
        }
    }
    private void LoginObjectActive(bool _flag)
    {
        m_btn_login.gameObject.SetActive(_flag);
        m_btn_join.gameObject.SetActive(_flag);
    }
    private void MenuObjectActive(bool _flag)
    {
        m_btn_multi.gameObject.SetActive(_flag);
        m_btn_single.gameObject.SetActive(_flag);
        m_btn_option.gameObject.SetActive(_flag);
        m_btn_logout.gameObject.SetActive(_flag);
    }
    #endregion
    #region MouseEvent
    private void OnClick_Start(GameObject _selected,MouseArgs _arg)
    {
        m_window_start.SetActive(false);
        m_window_menu.SetActive(true);
    }
    #endregion
    #region Button Click Event
    public void OnClick_Login()
    {
        m_window_menu.SetActive(false);
        m_window_login.SetActive(true);
    }
    public void OnClick_Join()
    {
        m_window_menu.SetActive(false);
        m_window_join.SetActive(true);
    }
    public void OnClick_Multi()
    {
        m_window_menu.SetActive(false);
        m_window_lobby.SetActive(true);
        LobbyManager.Instance.LobbyEnterProcess(true);
    }
    public void OnClick_Single()
    {
        m_window_menu.SetActive(false);
    }
    public void OnClick_Option()
    {
        m_window_menu.SetActive(false);
    }
    public void OnClick_Logout()
    {
        //로그아웃 서버 전송하기
        LoginManager.Instance.LogoutProcess();
    }
    #endregion
    
    private void __Initialize()
    {
        MouseManager.Instance.MouseEvent_Register(MOUSE_TYPE.LEFTDOWN_BTN, TAG_TYPE.MenuStart, OnClick_Start);
        MenuObjectActive(false);
    }
    private void Start()
    {
        __Initialize();  
    }
}
