#pragma once

class GameObject
{
public:
    GameObject()
    {
    }
    GameObject(int _index)
    {
        m_index = _index;
    }
    Vector3 GetVector()
    {
        return m_position;
    }
    void SetVector(Vector3 _pos)
    {
        m_position = _pos;
    }
    int GetIndex()
    {
        return m_index;
    }
    E_GameObject GetType()
    {
        return m_obj_type;
    }
    void SetType(E_GameObject _type)
    {
        m_obj_type = _type;
    }
    virtual ~GameObject()
    {

    }
protected:
    Vector3 m_position;
    int m_index;
    E_GameObject m_obj_type;
};


