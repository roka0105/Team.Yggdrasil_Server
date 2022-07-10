using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public enum LoginUseWindow
{
    None = -1,
    Menu,
    Login,
    Join,
    Max
}
public class LoginManager : Singleton_Ver2.Singleton<LoginManager>
{
	enum MAINPROTOCOL
	{
		NONE,
		LOGIN,
		JOIN,
		LOGOUT,
		LOBBY,
		MAX
	}
	enum SUBPROTOCOL
	{
		NONE,
		LoginInfo,
		LoginResult,
		JoinInfo,
		JoinResult,
		LogoutInfo,
		LogoutResult,
		LobbyEnter,
		LobbyResult,
		MAX
	}
    [SerializeField]
    List<GameObject> m_Windows;
    [SerializeField]
    List<LoginUseWindow> m_window_type;

    private delegate void Process();
    
    MenuManager M_Menu;
    LoginUseWindow m_cur_usewindow;
    Dictionary<LoginUseWindow, List<InputField>> m_input_fields;
    
    public void LoginWindowProcess()
    {
        m_cur_usewindow = LoginUseWindow.Login;
        ActiveChangeWindow(LoginUseWindow.Login, LoginUseWindow.Menu);

        Debug.Log("login");
    }
    public void JoinWindowProcess()
    {
        m_cur_usewindow = LoginUseWindow.Join;
        ActiveChangeWindow(LoginUseWindow.Join, LoginUseWindow.Menu);
        Debug.Log("join");
    }
    public void LogoutProcess()
    {
        M_Menu.ActivePage(false);
        Debug.Log("logout");
    }
    public void LobbyEnterProcess()
    {
        Debug.Log("lobbyenter");
    }

    public void OnClick_ExitBtn()
    {
        ActiveChangeWindow(LoginUseWindow.Menu, m_cur_usewindow);
        m_cur_usewindow = LoginUseWindow.None;
    }
    public void OnClick_OkBtn()
    {
        //로그인 or 회원가입시 처리.
        switch (m_cur_usewindow)
        {
            case LoginUseWindow.Login:
                ExcuteProcess(LoginProcess);
                break;
            case LoginUseWindow.Join:
                ExcuteProcess(JoinProcess);
                break;
        }
    }

    private void LoginProcess()
    {
        Debug.Log("LProcess");
		//input field에서 데이터 빼오기
		string ID = m_input_fields[LoginUseWindow.Login][0].text;
		string PW = m_input_fields[LoginUseWindow.Login][1].text;

		//sendqueue에 넣기

	}
	private void JoinProcess()
    {
        Debug.Log("JProcess");
        //input field에서 데이터 빼오기
        //sendqueue에 넣기
       
    }
	private void ResultProcess()
	{
		//결과 응답 대기
		//결과 출력 y:메뉴돌아감  n:다시입력 에러메세지 뜨고 머무름.
	}
	private void ExcuteProcess(Process _process)
    {
        _process?.Invoke();
    }
   
    private void ActiveChangeWindow(LoginUseWindow _truewindow, LoginUseWindow _falsewindow)
    {
        m_Windows[(int)_truewindow].SetActive(true);
        m_Windows[(int)_falsewindow].SetActive(false);
    }
    private void SetInputField(LoginUseWindow _type)
    {
        InputField[] inputs = m_Windows[(int)_type].GetComponentsInChildren<InputField>();
        foreach (InputField input_item in inputs)
        {
            m_input_fields[_type].Add(input_item);
        }
    }
    public void Init()
    {
        M_Menu = MenuManager.Instance;
        m_cur_usewindow = LoginUseWindow.None;
        m_input_fields = new Dictionary<LoginUseWindow, List<InputField>>();
        m_input_fields.Add(LoginUseWindow.Menu, new List<InputField>());
        m_input_fields.Add(LoginUseWindow.Login, new List<InputField>());
        m_input_fields.Add(LoginUseWindow.Join, new List<InputField>());

        SetInputField(LoginUseWindow.Login);
        SetInputField(LoginUseWindow.Join);
    }
  
}
