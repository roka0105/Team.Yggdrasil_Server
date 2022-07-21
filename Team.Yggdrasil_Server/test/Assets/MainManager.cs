using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;


public class MainManager : Singleton_Ver2.Singleton<MainManager>
{
    private Dictionary<Type, Component> m_ManagerDic ;
    private void ManagerInit()
    {
        m_ManagerDic = new Dictionary<Type, Component>();
        m_ManagerDic.Add(typeof(LoginManager), new LoginManager());
        m_ManagerDic.Add(typeof(ClientManager), new ClientManager());
        m_ManagerDic.Add(typeof(RoomManager), RoomManager.Instance);
        m_ManagerDic.Add(typeof(ProtocolManager), ProtocolManager.Instance);
        m_ManagerDic.Add(typeof(LobbyManager), LobbyManager.Instance);
        m_ManagerDic.Add(typeof(MainThread), MainThread.Instance);
        m_ManagerDic.Add(typeof(MenuManager), MenuManager.Instance);
        m_ManagerDic.Add(typeof(MouseManager), MouseManager.Instance);
        m_ManagerDic.Add(typeof(PacketManager), new PacketManager() );
        m_ManagerDic.Add(typeof(Test), Test.Instance);
    }
    public T GetMgr<T>() where T:Component
    {
        if (!m_ManagerDic.ContainsKey(typeof(T)))
            return null;

        return (T)m_ManagerDic[typeof(T)];
    }
  
    private void Start()
    {
        PacketManager mytest1 = new PacketManager();
        PacketManager mytest2 = new PacketManager();
        ManagerInit();
       // GetMgr<PacketManager>().Packing();
    }
   
}
