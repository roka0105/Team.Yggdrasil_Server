using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProtocolManager : Singleton_Ver2.Singleton<ProtocolManager>
{
    private uint m_mainbit = 0xff000000;
    private uint m_subbit = 0x00ff0000;
    private uint m_detail = 0x0000ffff;
    private int m_mainshift = 24;
    private int m_subshift = 16;
    //���߿� c++���� csv �δ��� ��ũ��Ʈ ���� ����� �ε��ϰ� (����Ƴ� ���߿�)
    public uint GetMainProtocol(uint _protocol)
    {
        return (_protocol & m_mainbit) >> m_mainshift;
    }
    public uint GetSubProtocol(uint _protocol)
    {
        return (_protocol & m_subbit) >> m_subshift;
    }
    public uint GetDetailProtocol(uint _protocol)
    {
        return (_protocol & m_detail);
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
    public void SetDetailProtocol(ref uint _out_protocol,uint _protocol_type)
    {
        _out_protocol = _out_protocol | _protocol_type;
    }
   
}