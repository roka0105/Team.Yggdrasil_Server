using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System;
using UnityEngine;

/*dontdestroy ���� ������ ������ ������ ���� ���ϰ� �� ������ �����ϰ� ���ٵ� ������ �����͸� �ʱ�ȭ ���� �ʱ� ���ؼ�.
  ������ ���� ������ �� ������ �̷�� �� �״ϱ� �ش� ������ ������ �Ŵ����鿡 ��� ������ �������� �ڵ� �ʱ�ȭ �ǵ���.
 */
namespace Singleton_Ver1
{
    public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
    {
        [SerializeField]

        protected bool flag;
        private static T instance;
        public static T Instance
        {
            get
            {
                Singleton_Ver1.Singleton<T>[] objs = FindObjectsOfType<Singleton_Ver1.Singleton<T>>(true);
                GameObject obj;
                if (objs.Length < 0)
                {
                    obj = objs.Where(item => item.flag == true).FirstOrDefault().gameObject;
                }
                else obj = null;
                if (obj == null)
                {
                    if (objs.Length > 0)
                    {
                        return objs[0].GetComponent<T>();
                    }
                    obj = new GameObject(typeof(T).Name);
                    obj.AddComponent<T>();
                }
                else
                {
                    instance = obj.GetComponent<T>();
                }
                return instance;
            }
        }
    }

}

//Lazy singleton ��Ƽ ������ ȯ�濡�� lock��� ����ϴµ� ���� ��� ������ �ƴ϶� ȣ�� ������ ����,���� ������ ���� �� �� ����.
/*1.������ �����ɸ��� ū ������Ʈ �ʿ��Ҷ��� �����ϰ� ������
  2.���ҽ��� ���� ����ϴ� ������ �ʿ��Ҷ��� �Ҷ�
  3.�ڿ� ������ ��Ƽ ������ ȯ�濡�� �����ϰ� �ؾ� �� ��
  ���ذ� �ɶ����� �� �𸣰ڴ�. ���庸��
  ���࿡ ������ �ְ� �Ǹ� Ŭ�󿡵� ���α׷� ������,���� ������,���ú� ������ �������� ������ �Ǹ�
  ���� ��� ���� ���� Ÿ���� 3������ �ִµ� 1������ �������� �ɸ��� �ý����̶�� Ŭ�󿡼� ���� Ÿ���� �����ؼ�
  ������ ��û�ϰ� ���� ���� ���� �ε� �ð����� �ش� ������ ���� �ش� �����͸� �ε�޾Ƽ� ���������� �����ϰ� ���� �Ŵ����� ������
  ����ϴ°� �ƴѰ�? ��ó�� ������������ ������ �����Ѵٸ� �ش� ������������ �ʿ��� 
  �޾ƾ��� ������ Ŭ�󿡼� ������ ��û�ϰ� �������� Ŭ��� �����͸� ������ �ش� ������ �Ŵ����� �����ϴ� �� �ƴұ�..
  �ش� ���������� �ʿ� ���� �����ͱ��� �Ѳ����� �ε��ϸ� �ȵǴϱ�.
  */
namespace Singleton_Ver2
{
    public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
    {
        [SerializeField]
        protected bool flag;
        private static readonly Lazy<T> _instance =
        new Lazy<T>(() =>
        {
            Singleton_Ver2.Singleton<T>[] objs = FindObjectsOfType<Singleton_Ver2.Singleton<T>>(true);
            GameObject obj = objs.Where(item => item.flag == true).FirstOrDefault().gameObject;
            if (obj == null)
            {
                if (objs.Length > 0)
                {
                    return objs[0].GetComponent<T>();
                }
                obj = new GameObject(typeof(T).Name);
                obj.AddComponent<T>();
            }
            return obj.GetComponent<T>();
        });
        public static T Instance
        {
            get
            {
                return _instance.Value;
            }
        }
    }
}