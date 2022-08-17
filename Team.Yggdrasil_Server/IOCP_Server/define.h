#pragma once

enum class SOC
{
    SOC_TRUE,
    SOC_FALSE,
};

#define HOST_IP "127.0.0.1"
#define USER "root"
#define PASSWORD "6869"
#define DATABASE "Yggdrasil"


#define STRINGSIZE 32 

enum class ENetObjectType
{
    None,
    Room,
    Player,
    Boss,
    Spirit,
    Monster,
};
enum class E_GameObject
{
    None = 1,
    Player,
    Boss,
    Spirit,
    Monster,
    Max
};
enum class E_NodeType
{
    Left,
    LeftUp,
    LeftDown,
    Right,
    RightUp,
    RightDown,
    Up,
    Down,
};
struct Vector3
{
    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    void operator=(const Vector3& _ref)
    {
        x = _ref.x;;
        y = _ref.y;
        z = _ref.z;
    }
    float x;
    float y;
    float z;
};
