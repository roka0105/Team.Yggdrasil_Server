using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Onclick_Toggle : MonoBehaviour
{
    Toggle myToggle;
    private void Start()
    {
        myToggle = this.gameObject.GetComponent<Toggle>();
    }
    public void IsOnFalse(bool _click)
    {
        if(_click)
        {
            myToggle.isOn = false;
        }
    }
}
