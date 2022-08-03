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
    TMP_InputField m_input_createname;
    TMP_InputField m_input_pw;
    #endregion
    #region create room button object
    Button m_btn_createroom;
    Button m_btn_ok;
    Button m_btn_cancle;
    #endregion
    #region chat object
    [SerializeField]
    Transform m_Content;
    [SerializeField]
    GameObject m_TextPrefeb;

    TMP_InputField m_input_chat;
    #endregion
    [SerializeField]
    Canvas m_canvas;

   

    #region Initialize
    public void __Initialize()
    {
        __Initialize_Btn();
        __Initialize_Input();
    }
    public void __Initialize_Btn()
    {
        m_room_btns = new Button[m_rooms_count];
        m_room_btns = m_canvas.transform.GetChild("Room View").GetComponentsInChildren<Button>();
        #region buttons init
        //m_btn_createroom = m_canvas.transform.GetChild("CreateRoomBtn").GetComponent<Button>();
        //m_next_btn = m_canvas.transform.GetChild("NextPageBtn").GetComponent<Button>();
        //m_pre_btn = m_canvas.transform.GetChild("BeforPageBtn").GetComponent<Button>();
        //m_btn_ok = m_canvas.transform.GetChild("CreateRoom").GetComponentsInChildren<Button>()[0];
        //m_btn_cancle = m_canvas.transform.GetChild("CreateRoom").GetComponentsInChildren<Button>()[1];
        #endregion
    }
    public void __Initialize_Input()
    {
        TMP_InputField[] inputs = m_canvas.transform.GetChild("CreateRoom")
                                                    .GetComponentsInChildren<TMP_InputField>();
        m_input_createname = inputs[0];
        m_input_pw = inputs[1];

        Transform ChatParent = m_canvas.transform.GetChild("Chatting");
        m_input_chat = ChatParent.GetComponentInChildren<TMP_InputField>();
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
    public void OnClick_ChatSend()
    {
        LobbyManager.Instance.ChattingProcess(m_input_chat.text);
    }
    #endregion
    #region page update func
    public void RoomInfoSetting(int _btn_index, int _id, string _title, int _mode, int _enter_count, int _enter_limit)
    {
        m_room_btns[_btn_index].GetComponent<RoomInfoBtn>().ChageInfo(_id, _title, _mode, _enter_count, _enter_limit);
    }
    #endregion
    #region chat update func
    public void EnterEventFocuse_Chat()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            LobbyManager.Instance.ChattingProcess(m_input_chat.text);
        }
    }
    public void UpdateChat(string _text)
    {
        GameObject clone = Instantiate(m_TextPrefeb, m_Content);
        clone.GetComponent<TextMeshProUGUI>().text =_text;
        m_input_chat.text = "";

        if(m_input_chat.isFocused==false)
        {
            m_input_chat.ActivateInputField();
        }
    }
    private void RearTimeRepit()
    {
        if(Input.GetKeyDown(KeyCode.Return)&&m_input_chat.isFocused==false)
        {
            m_input_chat.ActivateInputField();
        }
    }
    #endregion

    private void Update()
    {
        EnterEventFocuse_Chat();
        //RearTimeRepit();
    }
}
