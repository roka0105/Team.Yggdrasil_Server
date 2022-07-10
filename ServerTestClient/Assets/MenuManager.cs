using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum STATE
{
    NONE = -1,
    MAIN,
    MENU,
    LOGIN,
    JOIN,
    LOGOUT,
    MULTI,
    SINGLE,
    OPTION,
    MAX
}
public class MenuManager : Singleton_Ver2.Singleton<MenuManager>
{
    [SerializeField]
    List<STATE> States = new List<STATE>();
    [SerializeField]
    List<GameObject> m_menu_objects = new List<GameObject>();
    [SerializeField]
    Button OKBtn;



    const int page1_readcount = 2;
    LoginManager M_Login;
    MouseManager M_Mouse;
    STATE state;

    bool is_loging;
    public void Init()
    {
        M_Login = LoginManager.Instance;
        M_Mouse = MouseManager.Instance;

        state = STATE.MAIN;
        ActiveChangeObject(STATE.MAIN, STATE.MENU);
    
        is_loging = false;

        M_Mouse.MouseEvent_Register(MOUSE_TYPE.LEFTDOWN_BTN, TAG_TYPE.MenuStart, MenuStart);
    }
    public void ActivePage(bool _isloging)
    {
        if (is_loging)
        {
            ActiveChangeObject(STATE.MULTI, STATE.LOGIN);
            ActiveChangeObject(STATE.SINGLE, STATE.JOIN);
            ActiveChangeObject(STATE.OPTION, STATE.NONE);
            ActiveChangeObject(STATE.LOGOUT, STATE.NONE);
        }
        else
        {
            ActiveChangeObject(STATE.LOGIN,STATE.MULTI);
            ActiveChangeObject(STATE.JOIN,STATE.SINGLE);
            ActiveChangeObject(STATE.NONE, STATE.OPTION);
            ActiveChangeObject(STATE.NONE, STATE.LOGOUT);
        }
    }
    private void ActiveChangeObject(STATE _trueobject, STATE _falseobject)
    {
        if (_trueobject != STATE.NONE)
            m_menu_objects[(int)_trueobject].SetActive(true);
        if (_falseobject != STATE.NONE)
            m_menu_objects[(int)_falseobject].SetActive(false);
    }
    private void MenuStart(GameObject _selected, MouseArgs args)
    {
        ActiveChangeObject(STATE.MENU, STATE.MAIN);
        ActivePage(false);
    }
   
    void Start()
    {

    }
    void Update()
    {
        
    }
}
