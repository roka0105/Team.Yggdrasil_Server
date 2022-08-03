#pragma once
struct t_vector
{
public:
    float x;
    float y;
    float z;
};
class CGameObject
{
public:
    
private:
    t_vector m_position;
    bool m_moveflag;
    bool m_onoff_flag;
};

