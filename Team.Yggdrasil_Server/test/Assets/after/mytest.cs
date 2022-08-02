using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class mytest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

        Button[] t = GameObject.Find("my").transform.Find("name").gameObject.GetComponentsInChildren<Button>();
        
        //Button[] buttons = name.transform.GetComponentsInChildren<Button>();//GetComponents<Button>();
        //Debug.Log(buttons.Length);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
