using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProtocolManager : Singleton_Ver2.Singleton<ProtocolManager>
{
    private uint m_mainbit = 0xff000000;
    private uint m_subbit = 0x00ff0000;
    private int m_mainshift = 24;
    private int m_subshift = 16;
    //나중에 c++에서 csv 로더로 스크립트 만들어서 여기다 로드하게 (좀어렵네 나중에)
    public uint GetMainProtocol(uint _protocol)
    {
        return (_protocol & m_mainbit) >> m_mainshift;
    }
    public uint GetSubProtocol(uint _protocol)
    {
        return (_protocol & m_subbit) >> m_subshift;
    }
    public void SetMainProtocol(ref uint _out_protocol,uint _protocol_type)
    {
        _protocol_type = _protocol_type << m_mainshift;
        _out_protocol = _out_protocol | _protocol_type;
    }
    public void SetSubProtocol(ref uint _out_protocol,uint _protocol_type)
    {
        _protocol_type = _protocol_type << m_subshift;
        _out_protocol = _out_protocol | _protocol_type;
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}