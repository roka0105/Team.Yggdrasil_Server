using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class Test2 : MonoBehaviour
{
    MainManager M_Main = MainManager.Instance;
    // Start is called before the first frame update
    void Start()
    {
        MainManager.Instance.GetMgr<Test>().Printf();
       
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
