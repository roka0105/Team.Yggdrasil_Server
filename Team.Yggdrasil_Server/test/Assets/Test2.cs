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
        MainManager.Instance.GetMgr2<PacketManager>();
        MainManager.Instance.GetMgr1<Test>().Printf();
        Test t = Test.Instance;
        Test t2 = Test.Instance;
        Test t3 = new Test();

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
