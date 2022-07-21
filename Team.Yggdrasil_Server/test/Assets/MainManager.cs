using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;


public class MainManager : Singleton_Ver2.Singleton<MainManager>
{
    private Dictionary<Type, Component> m_ManagerDic ;
    private Dictionary<Type,Singleton_Ver3.SingletonBase> m_classDic;
    private void ManagerInit()
    {
        m_ManagerDic = new Dictionary<Type, Component>();
        m_classDic = new Dictionary<Type, Singleton_Ver3.SingletonBase>();
        //m_ManagerDic.Add(typeof(LoginManager), new LoginManager());
        //m_ManagerDic.Add(typeof(ClientManager), new ClientManager());
        //m_ManagerDic.Add(typeof(RoomManager), RoomManager.Instance);
        //m_ManagerDic.Add(typeof(ProtocolManager), ProtocolManager.Instance);
        //m_ManagerDic.Add(typeof(LobbyManager), LobbyManager.Instance);
        //m_ManagerDic.Add(typeof(MainThread), MainThread.Instance);
        //m_ManagerDic.Add(typeof(MenuManager), MenuManager.Instance);
        //m_ManagerDic.Add(typeof(MouseManager), MouseManager.Instance);
        //m_ManagerDic.Add(typeof(PacketManager), new PacketManager());
        //m_ManagerDic.Add(typeof(Test), Test.Instance);
        m_classDic.Add(typeof(Test), Test.Instance);
    }
    public T GetMgr1<T>() where T:Singleton_Ver3.SingletonBase
    {
        if (!m_classDic.ContainsKey(typeof(T)))
            return null;

        return (T)m_classDic[typeof(T)];
    }
    public T GetMgr2<T>() where T:Component
    {
        if (!m_ManagerDic.ContainsKey(typeof(T)))
            return null;

        return (T)m_ManagerDic[typeof(T)];
    }
  
    private void Start()
    {
        ManagerInit();
       // GetMgr<PacketManager>().Packing();
    }
   
}
