using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using System.Text;
namespace Befor
{
    public enum LoginUseWindow
    {
        None = -1,
        Menu,
        Login,
        Join,
        Max
    }
    public class LoginManager : Singleton_Ver2.Singleton<Befor.LoginManager>
    {
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
        #region Manager
        MainThread M_MainTh;
        MenuManager M_Menu;
        ProtocolManager M_Protocol;
        PacketManager M_Packet;
        #endregion

        private delegate void Process();
        private delegate void Result_Process(byte[] _recvdata);
        LoginUseWindow m_cur_usewindow;
        Dictionary<LoginUseWindow, List<InputField>> m_input_fields;
        Dictionary<SUBPROTOCOL, Result_Process> m_ResultProcess;

        #region server send process
        private void LoginSendProcess()
        {
            Debug.Log("LProcess");
            //input field에서 데이터 빼오기
            string ID = m_input_fields[LoginUseWindow.Login][0].text;
            string PW = m_input_fields[LoginUseWindow.Login][1].text;
            ClearInputField(LoginUseWindow.Login);
            //sendqueue에 넣기
            uint protocol = 0;
            M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOGIN);
            M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.LoginInfo);

            byte[] senddata = M_Packet.PackPacking(protocol, ID, PW);
            M_MainTh.SendQueue_Push(senddata);
            MainThread.m_WaitforSendThread.Set();
        }
        private void JoinSendProcess()
        {
            Debug.Log("JProcess");
            //input field에서 데이터 빼오기
            string ID = m_input_fields[LoginUseWindow.Join][0].text;
            string PW = m_input_fields[LoginUseWindow.Join][1].text;
            string NICK = m_input_fields[LoginUseWindow.Join][2].text;
            ClearInputField(LoginUseWindow.Join);
            //sendqueue에 넣기
            uint protocol = 0;
            M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOGIN);
            M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.JoinInfo);

            byte[] senddata = M_Packet.PackPacking(protocol, ID, PW, NICK);
            M_MainTh.SendQueue_Push(senddata);
            MainThread.m_WaitforSendThread.Set();
        }
        private void LogoutSendProcess()
        {
            //sendqueue에 넣기
            uint protocol = 0;
            M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOGIN);
            M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.LogoutInfo);

            byte[] senddata = M_Packet.PackPacking(protocol);
            M_MainTh.SendQueue_Push(senddata);
            MainThread.m_WaitforSendThread.Set();
        }
        #endregion

        #region server recv process
        private void LoginResult(byte[] _recvdata)
        {
            Debug.Log("LoginResult");
            bool result = false;
            string msg = "null";
            //unpack
            UnPackPacket(_recvdata, ref result, ref msg);
            //결과처리
            if (result)
            {
                M_MainTh.GS_Loging = true;
                M_Menu.ActivePage();
                ActiveChangeWindow(LoginUseWindow.Menu, LoginUseWindow.Login);
            }
            //결과출력
            Debug.Log(msg);
        }
        private void JoinResult(byte[] _recvdata)
        {
            Debug.Log("JoinResult");
            bool result = false;
            string msg = "null";
            //unpack
            UnPackPacket(_recvdata, ref result, ref msg);
            //결과체크
            if (result)
            {
                ActiveChangeWindow(LoginUseWindow.Menu, LoginUseWindow.Join);
            }
            //결과
            Debug.Log(msg);
        }
        private void LogoutResult(byte[] _recvdata)
        {
            Debug.Log("LogoutResult");
            string msg = "null";
            //unpack
            UnPackPacket(_recvdata, ref msg);
            Debug.Log(msg);

            M_MainTh.GS_Loging = false;
            M_Menu.ActivePage();
        }

        #endregion

        #region client Input Func
        public void LoginWindowBtn()
        {
            m_cur_usewindow = LoginUseWindow.Login;
            ActiveChangeWindow(LoginUseWindow.Login, LoginUseWindow.Menu);

            Debug.Log("login");
        }
        public void JoinWindowBtn()
        {
            m_cur_usewindow = LoginUseWindow.Join;
            ActiveChangeWindow(LoginUseWindow.Join, LoginUseWindow.Menu);
            Debug.Log("join");
        }
        public void LogoutBtn()
        {
            Debug.Log("logout");
            LogoutSendProcess();
        }

        //login _ join 나가기 버튼
        public void OnClick_ExitBtn()
        {
            ActiveChangeWindow(LoginUseWindow.Menu, m_cur_usewindow);
            m_cur_usewindow = LoginUseWindow.None;
        }
        //login _ join 확인 버튼
        public void OnClick_OkBtn()
        {
            //로그인 or 회원가입시 처리.
            switch (m_cur_usewindow)
            {
                case LoginUseWindow.Login:
                    ExcuteProcess(LoginSendProcess);
                    break;
                case LoginUseWindow.Join:
                    ExcuteProcess(JoinSendProcess);
                    break;
            }
        }
        #endregion

        #region client func
        public void Init()
        {
            M_Menu = MenuManager.Instance;
            M_MainTh = MainThread.Instance;
            M_Protocol = ProtocolManager.Instance;
            M_Packet = PacketManager.Instance;
            m_cur_usewindow = LoginUseWindow.None;
            m_input_fields = new Dictionary<LoginUseWindow, List<InputField>>();
            m_input_fields.Add(LoginUseWindow.Menu, new List<InputField>());
            m_input_fields.Add(LoginUseWindow.Login, new List<InputField>());
            m_input_fields.Add(LoginUseWindow.Join, new List<InputField>());

            SetInputField(LoginUseWindow.Login);
            SetInputField(LoginUseWindow.Join);

            ServerInit();
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
        private void ClearInputField(LoginUseWindow _type)
        {
            foreach (InputField input_item in m_input_fields[_type])
            {
                input_item.text = "";
            }
        }
        #endregion

        #region server func
        private void ServerInit()
        {
            m_ResultProcess = new Dictionary<SUBPROTOCOL, Result_Process>();
            m_ResultProcess.Add(SUBPROTOCOL.LoginResult, LoginResult);
            m_ResultProcess.Add(SUBPROTOCOL.JoinResult, JoinResult);
            m_ResultProcess.Add(SUBPROTOCOL.LogoutResult, LogoutResult);

            M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.LOGIN, ResultProcess);
        }
        //login _ join ok 버튼 처리시 실행할 함수포인터
        private void ExcuteProcess(Process _process)
        {
            _process?.Invoke();
        }

        private void ResultProcess(uint _protocol, byte[] _recvbuf)
        {
            uint subprotocol = M_Protocol.GetSubProtocol(_protocol);
            m_ResultProcess[(SUBPROTOCOL)subprotocol]?.Invoke(_recvbuf);
        }

        #endregion






        private void UnPackPacket(byte[] _recvbuf, ref bool _result, ref string _msg)
        {
            int index = sizeof(uint) + sizeof(int);
            int strsize = 0;
            _result = BitConverter.ToBoolean(_recvbuf, index);
            index += sizeof(bool);
            strsize = BitConverter.ToInt32(_recvbuf, index);
            index += sizeof(int);
            _msg = Encoding.Unicode.GetString(_recvbuf, index, strsize);
        }
        private void UnPackPacket(byte[] _recvbuf, ref string _msg)
        {
            int index = sizeof(uint) + sizeof(int);
            int strsize = 0;
            strsize = BitConverter.ToInt32(_recvbuf, index);
            index += sizeof(int);
            _msg = Encoding.Unicode.GetString(_recvbuf, index, strsize);
        }



    }

}
