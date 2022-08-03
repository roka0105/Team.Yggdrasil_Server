using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class ExpansionMethod
{
    public static Transform GetChild(this Transform _transform, string _name)
    {
        int count = _transform.childCount;
        Transform child_transform;
        for (int i = 0; i < count; i++)
        {
            child_transform = _transform.GetChild(i);
            if (child_transform.name == _name)
            {
                return child_transform;
            }
            else if (child_transform.childCount > 0)
            {
                child_transform = GetChild(child_transform, _name);
                if (child_transform != null)
                    return child_transform;
            }
        }
        return null;
    }
    public static Transform[] GetChilderen(this Transform _transform,string _name)
    {
        int count = _transform.childCount;
        List<Transform> t_list = new List<Transform>();

        if(_transform.name == _name)
        {
            t_list.Add(_transform);
        }
        else if(count == 0)
            return null;
        
        for(int i=0;i<count;i++)
        {
            Transform[] arr = _transform.GetChild(i).GetChilderen(_name);
            if (arr != null)
                t_list.AddRange(arr);
        }
        return t_list.ToArray();
    }
}
