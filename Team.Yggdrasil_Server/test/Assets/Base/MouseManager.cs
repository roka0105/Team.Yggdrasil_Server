using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using System;

//왼,오 다운,드래그 만 구현됨.
public enum TAG_TYPE
{
    NONE,
    MenuStart,
    MAX
}
public enum MOUSE_TYPE
{
    NONE,
    LEFTDOWN_BTN,
    LEFTUP_BTN,
    LEFTDRAG_BTN,
    RIGHTDOWN_BTN,
    RIGHTUP_BTN,
    MAX
}
// 이벤트 호출시 넘겨줄 마우스 정보 구조체.
public struct MouseArgs
{
    public Vector2 beforPos;
    public Vector2 currentPos;
    public GameObject beforGameObject;
    public TAG_TYPE befortag;
    public bool is_LeftDown;
    public bool is_RightDown;
}
public class MouseManager : Singleton_Ver2.Singleton<MouseManager>
{
    [SerializeField]
    Canvas m_canvas;
    GraphicRaycaster m_Raycaster;
    PointerEventData m_PointerEventData;
    EventSystem m_EventSystem;

    public delegate void MouseEventHandler(GameObject _selected, MouseArgs args);

    Dictionary<MOUSE_TYPE, Dictionary<TAG_TYPE, List<MouseEventHandler>>> MouseEvents;

    Dictionary<MOUSE_TYPE, List<TAG_TYPE>> MouseTagList;

    GameObject m_currentGameObject;
    MouseArgs m_current_args;
    float current_mousedown_time;

    TAG_TYPE m_currenttag;
    bool m_isFind;
    object temptag;
    //마우스 이벤트 등록
    public void MouseEvent_Register(MOUSE_TYPE p_mouse, TAG_TYPE p_tag, MouseEventHandler p_event)
    {
        MouseTagList[p_mouse].Add(p_tag);
        if (!MouseEvents.ContainsKey(p_mouse))
        {
            MouseEvents.Add(p_mouse, new Dictionary<TAG_TYPE, List<MouseEventHandler>>());
            MouseEvents[p_mouse].Add(p_tag, new List<MouseEventHandler>());
        }
        MouseEvents[p_mouse][p_tag].Add(p_event);
        return;
    }

    public Vector3 MousePos
    {
        get => Input.mousePosition;
    }
    public void Init()
    {
        m_canvas = GameObject.FindWithTag("MainCanvas").GetComponent<Canvas>();
        m_current_args = new MouseArgs();
        current_mousedown_time = Time.time;

        MouseEvents = new Dictionary<MOUSE_TYPE, Dictionary<TAG_TYPE, List<MouseEventHandler>>>();
        MouseTagList = new Dictionary<MOUSE_TYPE, List<TAG_TYPE>>();
        m_Raycaster = m_canvas.GetComponent<GraphicRaycaster>();
        m_EventSystem = m_canvas.GetComponent<EventSystem>();

        MouseTagList.Add(MOUSE_TYPE.LEFTDOWN_BTN, new List<TAG_TYPE>());
        MouseTagList.Add(MOUSE_TYPE.LEFTDRAG_BTN, new List<TAG_TYPE>());
        MouseTagList.Add(MOUSE_TYPE.LEFTUP_BTN, new List<TAG_TYPE>());
        MouseTagList.Add(MOUSE_TYPE.RIGHTDOWN_BTN, new List<TAG_TYPE>());
        MouseTagList.Add(MOUSE_TYPE.RIGHTUP_BTN, new List<TAG_TYPE>());
    }
    private void Awake()
    {
        Init();
    }
    void Start()
    {
       
    }
    private void Update()
    {
        m_current_args.beforPos = m_current_args.currentPos;
        m_current_args.currentPos = Input.mousePosition;
        //왼클릭 다운
        if (Input.GetKeyDown(KeyCode.Mouse0))
        {
            m_current_args.is_LeftDown = true;
            m_PointerEventData = new PointerEventData(m_EventSystem);
            m_PointerEventData.position = Input.mousePosition;

            List<RaycastResult> results = new List<RaycastResult>();
            m_Raycaster.Raycast(m_PointerEventData, results);

            if (results.Count > 0)
            {
                for (int i = 0; i < results.Count; ++i)
                {

                    bool is_suc = Enum.TryParse(typeof(TAG_TYPE), results[i].gameObject.tag.ToString(), out temptag);
                    if (is_suc)
                    {
                        if (MouseTagList[MOUSE_TYPE.LEFTDOWN_BTN].Contains((TAG_TYPE)temptag))
                        {
                            m_isFind = true;
                            m_current_args.befortag = (TAG_TYPE)temptag;
                        }

                    }
                    else m_isFind = false;

                    if (m_isFind)
                    {
                        m_currentGameObject = results[i].gameObject;
                        m_current_args.beforGameObject = m_currentGameObject;
                        foreach (MouseEventHandler eventfunc in MouseEvents[MOUSE_TYPE.LEFTDOWN_BTN][(TAG_TYPE)temptag])
                        {
                            eventfunc?.Invoke(m_currentGameObject, m_current_args);
                        }

                        m_isFind = false;
                        //클릭된곳에 여러 오브젝트가 있을때 해당 오브젝트들의 이벤트를 모두 처리하려면 그냥 두고
                        //한가지 오브젝트에 대한 처리만 하고 나갔으면 하면 여기에 break를 넣는다.
                        break;
                    }

                }
            }
        }
        //왼클릭 업
        if (Input.GetKeyUp(KeyCode.Mouse0))
        {
            m_current_args.is_LeftDown = false;
            m_PointerEventData = new PointerEventData(m_EventSystem);
            m_PointerEventData.position = Input.mousePosition;

            List<RaycastResult> results = new List<RaycastResult>();
            m_Raycaster.Raycast(m_PointerEventData, results);

            if (results.Count > 0)
            {
                for (int i = 0; i < results.Count; ++i)
                {

                    bool is_suc = Enum.TryParse(typeof(TAG_TYPE), results[i].gameObject.tag.ToString(), out temptag);
                    if (is_suc)
                    {
                        if (MouseTagList[MOUSE_TYPE.LEFTUP_BTN].Contains((TAG_TYPE)temptag))
                        {
                            m_isFind = true;
                        }

                    }
                    else
                    {
                        m_isFind = false;
                    }
                    if (m_isFind)
                    {

                        break;
                    }

                }

                if (!m_isFind)
                {
                    if (m_current_args.befortag != null)
                    {

                    }
                }
                m_isFind = false;
            }
        }
        //오른 다운
        if (Input.GetKeyDown(KeyCode.Mouse1))
        {
            m_current_args.is_LeftDown = true;
            m_PointerEventData = new PointerEventData(m_EventSystem);
            m_PointerEventData.position = Input.mousePosition;

            List<RaycastResult> results = new List<RaycastResult>();
            m_Raycaster.Raycast(m_PointerEventData, results);

            if (results.Count > 0)
            {
                for (int i = 0; i < results.Count; ++i)
                {

                    bool is_suc = Enum.TryParse(typeof(TAG_TYPE), results[i].gameObject.tag.ToString(), out temptag);
                    if (is_suc)
                    {
                        if (MouseTagList[MOUSE_TYPE.RIGHTDOWN_BTN].Contains((TAG_TYPE)temptag))
                        {
                            m_isFind = true;
                        }

                    }
                    else m_isFind = false;

                    if (m_isFind)
                    {
                        m_currentGameObject = results[i].gameObject;
                        foreach (MouseEventHandler eventfunc in MouseEvents[MOUSE_TYPE.RIGHTDOWN_BTN][(TAG_TYPE)temptag])
                        {
                            eventfunc?.Invoke(m_currentGameObject, m_current_args);
                        }
                        m_isFind = false;
                        break;
                    }

                }

            }


        }
        //오른 업
        if (Input.GetKeyUp(KeyCode.Mouse1))
        {
            m_current_args.is_LeftDown = false;
            m_PointerEventData = new PointerEventData(m_EventSystem);
            m_PointerEventData.position = Input.mousePosition;

            List<RaycastResult> results = new List<RaycastResult>();
            m_Raycaster.Raycast(m_PointerEventData, results);

            if (results.Count > 0)
            {
                for (int i = 0; i < results.Count; ++i)
                {

                    bool is_suc = Enum.TryParse(typeof(TAG_TYPE), results[i].gameObject.tag.ToString(), out temptag);
                    if (is_suc)
                    {
                        if (MouseTagList[MOUSE_TYPE.RIGHTUP_BTN].Contains((TAG_TYPE)temptag))
                        {
                            m_isFind = true;
                        }

                    }
                    else m_isFind = false;

                    if (m_isFind)
                    {

                        break;
                    }

                }
                if (!m_isFind)
                {

                }
                m_isFind = false;
            }
            else // 아무것도 못 맞췄을 때
            {

            }
        }
       
        // 왼클릭 누르는 중일 때
        if (m_current_args.is_LeftDown)
        {
            m_current_args.currentPos = Input.mousePosition;
            if (temptag != null && MouseTagList[MOUSE_TYPE.LEFTDRAG_BTN].Contains((TAG_TYPE)temptag))
            {
                foreach (MouseEventHandler eventfunc in MouseEvents[MOUSE_TYPE.LEFTDRAG_BTN][(TAG_TYPE)temptag])
                {
                    eventfunc?.Invoke(m_currentGameObject, m_current_args);
                }
            }

        }
    }
}
