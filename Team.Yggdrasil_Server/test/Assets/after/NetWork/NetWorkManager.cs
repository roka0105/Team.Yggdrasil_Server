using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using System;

namespace Net
{
    public class NetWorkManager :Singleton_Ver2.Singleton<NetWorkManager>
    {
        private NetSession m_session;
        private bool m_thread_end_flag;
        private Thread m_netthread;
        private Thread m_recvthread;

        private EventWaitHandle m_sendevent;
      
        
        public void __Initialize()
        {
            m_thread_end_flag = false;
            m_netthread = new Thread(NetThread);
            m_netthread.Start();

            __Initialize_Mgr();
        }
        /*임시로 쓰고 나중에 클라 코드 붙일때 GUI 부분들은 클라에서 하기.*/
        private void __Initialize_Mgr()
        {
            LoginGUIManager.__Initialize();
            LoginManager.__Initialize();
            MenuGUIManager.__Initialize();
        }
        public void __Finalize()
        {
            m_thread_end_flag = true;
            m_session.__Finalize();
        }

        private void Start()
        {
            __Initialize();
        }
        private void OnApplicationQuit()
        {
            __Finalize();
        }
        public void Send(SendPacket _packet)
        {
            m_session.SendReq(_packet);
            m_sendevent.Set();
        }

        private void NetThread()
        {
            m_session = new NetSession();
            m_session.__Initialize();

            m_sendevent = new EventWaitHandle(false, EventResetMode.AutoReset);

            m_recvthread = new Thread(RecvThread);
            m_recvthread.Start();
            while(true)
            {
                if (m_thread_end_flag)
                    break;
                m_sendevent.WaitOne();
                m_session.SendQueueProcess();
            }
            m_recvthread.Join();
        }
        private void RecvThread()
        {
            while(true)
            {
                m_session.Recv();
            }
        }
        private void Update()
        {
            m_session.RecvQueueProcess();
        }
    }

}
