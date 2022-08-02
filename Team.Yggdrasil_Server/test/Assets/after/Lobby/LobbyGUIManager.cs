using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Text;
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
    #region room button object
    private Button[] m_room_btns;
    private const int m_rooms_count = 10;
    #endregion
    #region page button object
    Button m_next_btn;
    Button m_pre_btn;
    #endregion
    #region create room input field object
    [SerializeField]
    TMP_InputField m_input_createname;
    [SerializeField]
    TMP_InputField m_input_pw;
    #endregion
    #region create room button object
    Button m_btn_createroom;
    [SerializeField]
    Button m_btn_ok;
    [SerializeField]
    Button m_btn_cancle;
    #endregion


    #region Initialize
    public void __Initialize()
    {
        __Initialize_Btn();
        __Initialize_Input();
    }
    public void __Initialize_Btn()
    {
        m_room_btns = new Button[m_rooms_count];
        GameObject lobby = GameObject.Find("Canvas").transform.Find("Panel").transform.Find("Lobby").gameObject;
        //GameObject lobbybtn = lobby.transform.Find("LobbyBtn").gameObject;
        m_room_btns = lobby.transform.Find("Room View").GetComponentsInChildren<Button>();
        //m_btn_createroom = lobbybtn.transform.Find("CreateRoomBtn").GetComponent<Button>();
        //m_next_btn = lobbybtn.transform.Find("NextPageBtn").GetComponent<Button>();
        //m_pre_btn = lobbybtn.transform.Find("BeforPageBtn").GetComponent<Button>();
        //m_btn_ok = lobby.transform.Find("CreateRoom").GetComponentsInChildren<Button>()[0];
        //m_btn_cancle = lobby.transform.Find("CreateRoom").GetComponentsInChildren<Button>()[1];
    }
    public void __Initialize_Input()
    {
        GameObject createroom = GameObject.Find("Canvas").transform.Find("Panel").transform.Find("Lobby").transform
            .Find("CreateRoom").gameObject;
        TMP_InputField[] inputs = createroom.GetComponentsInChildren<TMP_InputField>();
        m_input_createname = inputs[0];
        m_input_pw = inputs[1];
    }
    #endregion
    #region button click event
    public void OnClick_BeforMenu()
    {
        MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Menu, true);
        MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Lobby, false);
    }
    public void OnClick_NextPage()
    {
        LobbyManager.Instance.PageReqProcess(true);
    }
    public void OnClick_PrePage()
    {
        LobbyManager.Instance.PageReqProcess(false);
    }
    public void OnClick_CreateRoom()
    {
        m_window_createroom.SetActive(true);
    }
    public void OnClick_CreateRoomOK()
    {
        LobbyManager.Instance.CreateRoomProcess(m_input_createname.text, m_input_pw.text);
        m_window_createroom.SetActive(false);
    }
    public void OnClick_CreateRoomCancle()
    {
        m_window_createroom.SetActive(false);
    }
    #endregion
    public void RoomInfoSetting(int _btn_index, int _id, string _title, int _mode, int _enter_count, int _enter_limit)
    {
        m_room_btns[_btn_index].GetComponent<RoomInfoBtn>().ChageInfo(_id, _title, _mode, _enter_count, _enter_limit);
    }
}
