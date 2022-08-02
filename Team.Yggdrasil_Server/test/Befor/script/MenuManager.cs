using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class MenuManager : Singleton_Ver2.Singleton<MenuManager>
{
    public enum MENUSTATE
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
   

    [SerializeField]
    List<MENUSTATE> States = new List<MENUSTATE>();
    [SerializeField]
    List<GameObject> m_menu_objects = new List<GameObject>();
 
    MainThread M_MainTh;
    LoginManager M_Login;
    MouseManager M_Mouse;

    #region client Input func
    public void OptionBtn()
    {
        Debug.Log("option");
    }

    #endregion

    #region client func
    public void Init()
    {
        M_Login = LoginManager.Instance;
        M_Mouse = MouseManager.Instance;
        M_MainTh = MainThread.Instance;
        ActiveChangeObject(MENUSTATE.MAIN, MENUSTATE.MENU);

        M_Mouse.MouseEvent_Register(MOUSE_TYPE.LEFTDOWN_BTN, TAG_TYPE.MenuStart, MenuStart);
    }
    public void ActivePage()
    {
        if (M_MainTh.GS_Loging)
        {
            ActiveChangeObject(MENUSTATE.MULTI, MENUSTATE.LOGIN);
            ActiveChangeObject(MENUSTATE.SINGLE, MENUSTATE.JOIN);
            ActiveChangeObject(MENUSTATE.OPTION, MENUSTATE.NONE);
            ActiveChangeObject(MENUSTATE.LOGOUT, MENUSTATE.NONE);
        }
        else
        {
            ActiveChangeObject(MENUSTATE.LOGIN, MENUSTATE.MULTI);
            ActiveChangeObject(MENUSTATE.JOIN, MENUSTATE.SINGLE);
            ActiveChangeObject(MENUSTATE.NONE, MENUSTATE.OPTION);
            ActiveChangeObject(MENUSTATE.NONE, MENUSTATE.LOGOUT);
        }
    }
    private void ActiveChangeObject(MENUSTATE _trueobject, MENUSTATE _falseobject)
    {
        if (_trueobject != MENUSTATE.NONE)
            m_menu_objects[(int)_trueobject].SetActive(true);
        if (_falseobject != MENUSTATE.NONE)
            m_menu_objects[(int)_falseobject].SetActive(false);
    }
    private void MenuStart(GameObject _selected, MouseArgs args)
    {
        ActiveChangeObject(MENUSTATE.MENU, MENUSTATE.MAIN);
        ActivePage();
    }

    #endregion
}
