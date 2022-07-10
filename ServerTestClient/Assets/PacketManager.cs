using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

/* LayoutKind,MarShalling Study Link
 * https://www.sysnet.pe.kr/Default.aspx?mode=2&sub=0&detail=1&pageno=0&wid=1558&rssMode=1&wtype=0
 */
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct t_LoginInfoPacket
{
    [MarshalAs(UnmanagedType.U4)]
    public uint m_protocol;
    [MarshalAs(UnmanagedType.U4)]
    public uint m_datasize;
    [MarshalAs(UnmanagedType.U4)]
    public int m_idLength;
    [MarshalAs(UnmanagedType.ByValTStr,SizeConst =32)]
    public string m_id;
    [MarshalAs(UnmanagedType.U4)]
    public int m_pwLength;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string m_pw;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
struct t_JoinInfoPacket
{
    [MarshalAs(UnmanagedType.U4)]
    public uint m_protocol;
    [MarshalAs(UnmanagedType.U4)]
    public uint m_datasize;
    [MarshalAs(UnmanagedType.U4)]
    public int m_idLength;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string m_id;
    [MarshalAs(UnmanagedType.U4)]
    public int m_pwLength;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string m_pw;
    [MarshalAs(UnmanagedType.U4)]
    public int m_nickLength;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
    public string m_nick;
}
public class PacketManager : Singleton_Ver2.Singleton<PacketManager>
{
    private t_LoginInfoPacket m_LoginInfoPacket = new t_LoginInfoPacket();
    private t_JoinInfoPacket m_JoinInfoPacket = new t_JoinInfoPacket();

    public Byte[] GetLoginInfoPacket(uint _protocol,string _id,string _pw)
    {
        m_LoginInfoPacket.m_protocol = _protocol;
        m_LoginInfoPacket.m_datasize = (uint)(_id.Length + _pw.Length);
        m_LoginInfoPacket.m_idLength = _id.Length;
        m_LoginInfoPacket.m_id = _id;
        m_LoginInfoPacket.m_pwLength = _pw.Length;
        m_LoginInfoPacket.m_pw = _pw;
        return Serialize<t_LoginInfoPacket>(m_LoginInfoPacket);
    }
    public Byte[] GetJoinInfoPacket(uint _protocol,string _id,string _pw,string _nick)
    {
        m_JoinInfoPacket.m_protocol = _protocol;
        m_JoinInfoPacket.m_datasize = (uint)(_id.Length + _pw.Length + _nick.Length);
        m_JoinInfoPacket.m_idLength = _id.Length;
        m_JoinInfoPacket.m_id = _id;
        m_JoinInfoPacket.m_pwLength = _pw.Length;
        m_JoinInfoPacket.m_pw = _pw;
        m_JoinInfoPacket.m_nickLength = _nick.Length;
        m_JoinInfoPacket.m_nick = _nick;
        return Serialize<t_JoinInfoPacket>(m_JoinInfoPacket);
    }
    public uint GetPROTOCOL(Byte[] _recvbuf)
    {
        string recvbuf = _recvbuf.ToString();
        Byte[] protocol = new Byte[sizeof(uint)];
        Buffer.BlockCopy(_recvbuf, 0, protocol, 0,sizeof(uint));
       
        return uint.Parse(protocol.ToString());
    }
    public int Packing(uint _packetNo,Byte[] _databuf,out Byte[] _sendbuf)
    {
        int datasize = _databuf.Length+sizeof(uint);
        int index = sizeof(int);
        _sendbuf = new Byte[datasize + sizeof(int)];
        
        BitConverter.GetBytes(_packetNo).CopyTo(_sendbuf, index);
        index += sizeof(uint);

        Buffer.BlockCopy(_databuf, 0, _sendbuf, index, _databuf.Length);

        index = 0;
        BitConverter.GetBytes(datasize).CopyTo(_sendbuf, index);
        datasize += sizeof(int);

        return datasize;
    }
    /* GCHandle Ό³Έν
     * https://docs.microsoft.com/ko-kr/dotnet/api/system.runtime.interopservices.gchandle.alloc?view=net-6.0
     */
    private Byte[] Serialize<T>(T _packet)
    {
        Byte[] buffer = new Byte[Marshal.SizeOf(_packet)];
        GCHandle gch = GCHandle.Alloc(buffer, GCHandleType.Pinned);
        IntPtr p_buffer = gch.AddrOfPinnedObject();

        Marshal.StructureToPtr(_packet, p_buffer, false);
        gch.Free();
        return buffer;
    }
}
