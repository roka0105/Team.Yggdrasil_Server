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
    GameObject m_start_window;
    [SerializeField]
    GameObject m_menu_window;
    [SerializeField]
    GameObject m_login_window;
    [SerializeField]
    GameObject m_join_window;
    [SerializeField]
    GameObject m_lobby_window;
    #endregion

    #region button object
    [SerializeField] 
    Button m_loin_btn;
    [SerializeField] 
    Button m_join_btn;
    [SerializeField] 
    Button m_multi_btn;
    [SerializeField] 
    Button m_single_btn;
    [SerializeField] 
    Button m_option_btn;
    [SerializeField]
    Button m_logout_btn;
    #endregion

    #region Active Window
    public void WindowActive(EWindowType _type,bool _flag)
    {
        switch(_type)
        {
            case EWindowType.Menu:
                m_menu_window.SetActive(_flag);
                break;
            case EWindowType.Start:
                m_start_window.SetActive(_flag);
                break;
            case EWindowType.Login:
                m_login_window.SetActive(_flag);
                break;
            case EWindowType.Join:
                m_join_window.SetActive(_flag);
                break;
            case EWindowType.Lobby:
                m_lobby_window.SetActive(_flag);
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
        m_loin_btn.gameObject.SetActive(_flag);
        m_join_btn.gameObject.SetActive(_flag);
    }
    private void MenuObjectActive(bool _flag)
    {
        m_multi_btn.gameObject.SetActive(_flag);
        m_single_btn.gameObject.SetActive(_flag);
        m_option_btn.gameObject.SetActive(_flag);
        m_logout_btn.gameObject.SetActive(_flag);
    }
    #endregion
    #region MouseEvent
    private void OnClick_Start(GameObject _selected,MouseArgs _arg)
    {
        m_start_window.SetActive(false);
        m_menu_window.SetActive(true);
    }
    #endregion
    #region Button Click Event
    public void OnClick_Login()
    {
        m_menu_window.SetActive(false);
        m_login_window.SetActive(true);
    }
    public void OnClick_Join()
    {
        m_menu_window.SetActive(false);
        m_join_window.SetActive(true);
    }
    public void OnClick_Multi()
    {
        m_menu_window.SetActive(false);
        m_lobby_window.SetActive(true);
    }
    public void OnClick_Single()
    {
        m_menu_window.SetActive(false);
    }
    public void OnClick_Option()
    {
        m_menu_window.SetActive(false);
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
