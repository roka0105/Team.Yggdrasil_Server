using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
namespace Net
{
    public class Protocol
    {
        private uint m_mainbit = 0xff000000;
        private uint m_subbit = 0x00ff0000;
        private uint m_detailbit = 0x0000ffff;
        private uint m_pre_detailbit = 0x0000ff00;
        private uint m_rear_detailbit = 0x000000ff;

        private int m_mainshift = 24;
        private int m_subshift = 16;
        private int m_pre_dshitf = 8;

        private uint protocol;
        public enum EProtocolType
        {
            None,
            Main,
            Sub,
            Detail,
            Pre_D,
            Raer_D,
            Max,
        }
        public enum EMainProtocol
        {
            NONE,
            LOGIN,
            LOBBY,
            ROOM,
            GAME,
            MAX
        }
        public Protocol()
        {
            protocol = 0;
        }
        public Protocol(uint _protocol)
        {
            protocol = _protocol;
        }
        #region get
        public uint GetProtocol()
        {
            return protocol;
        }
        public uint GetProtocol(EProtocolType _type)
        {
            switch (_type)
            {
                case EProtocolType.Main:
                    return GetMainProtocol(protocol);
                case EProtocolType.Sub:
                    return GetSubProtocol(protocol);
                case EProtocolType.Detail:
                    return GetDetailProtocol(protocol);
                case EProtocolType.Pre_D:
                    return GetPreDetailProtocol(protocol);
                case EProtocolType.Raer_D:
                    return GetRearDetailProtocol(protocol);
            }
            throw new NotImplementedException();
        }

        private uint GetMainProtocol(uint _protocol)
        {
            return (_protocol & m_mainbit) >> m_mainshift;
        }
        private uint GetSubProtocol(uint _protocol)
        {
            return (_protocol & m_subbit) >> m_subshift;
        }
        private uint GetDetailProtocol(uint _protocol)
        {
            return (_protocol & m_detailbit);
        }
        private uint GetPreDetailProtocol(uint _protocol)
        {
            return (_protocol & m_pre_detailbit);
        }
        private uint GetRearDetailProtocol(uint _protocol)
        {
            return (_protocol & m_rear_detailbit);
        }
        #endregion

        #region set
        public void SetProtocol(uint _add_protocol, EProtocolType _type)
        {
            switch (_type)
            {
                case EProtocolType.Main:
                    SetMainProtocol(ref protocol, _add_protocol);
                    break;
                case EProtocolType.Sub:
                    SetSubProtocol(ref protocol, _add_protocol);
                    break;
                case EProtocolType.Detail:
                    SetDetailProtocol(ref protocol, _add_protocol);
                    break;
                case EProtocolType.Pre_D:
                    SetPreDetailProtocol(ref protocol, _add_protocol);
                    break;
                case EProtocolType.Raer_D:
                    SetDetailProtocol(ref protocol, _add_protocol);
                    break;
                default:
                    throw new NotImplementedException();
            }
        }
        #endregion
        private void SetMainProtocol(ref uint _out_protocol, uint _protocol_type)
        {
            _protocol_type = _protocol_type << m_mainshift;
            _out_protocol = _out_protocol | _protocol_type;
        }
        private void SetSubProtocol(ref uint _out_protocol, uint _protocol_type)
        {
            _protocol_type = _protocol_type << m_subshift;
            _out_protocol = _out_protocol | _protocol_type;
        }
        private void SetPreDetailProtocol(ref uint _out_protocol, uint _protocol_type)
        {
            _protocol_type = _protocol_type << m_pre_dshitf;
            _out_protocol = _out_protocol | _protocol_type;
        }
        private void SetDetailProtocol(ref uint _out_protocol, uint _protocol_type)
        {
            _out_protocol = _out_protocol | _protocol_type;
        }

    }
}


