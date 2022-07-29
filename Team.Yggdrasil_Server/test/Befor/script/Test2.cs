using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
public class Test2 : MonoBehaviour
{

    MainManager M_Main ;
    // Start is called before the first frame update
    void Start()
    {
        MainManager.Instance.GetMgr<PacketManager>();
       
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
