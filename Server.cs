using UnityEngine;
using System.Net.Sockets;
using System.Collections;

public class Server : MonoBehaviour
{
    public int RotateX, RotateY, RotateZ;
    private ServerThread st;
    private bool isSend;//儲存是否發送訊息完畢
    System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();//引用stopwatch物件
    public GameObject cube;
    private void Start()
    {
        sw.Reset();//碼表歸零
        //開始連線，設定使用網路、串流、TCP
        st = new ServerThread(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp, "192.168.1.113", 27);
        st.Listen();//讓Server socket開始監聽連線
        st.StartConnect();//開啟Server socket
        isSend = true;
    }

    private void Update()
    {
        if (st.receiveMessage != null)
        {   
            Debug.Log(st.receiveMessage);
            char[] splitChar = { ' ', ',', ':', '\t', ';' };
            string[] dataRaw = st.receiveMessage.Split(splitChar);
            RotateX = int.Parse(dataRaw[0]);
            RotateY = int.Parse(dataRaw[1]);
            RotateZ = int.Parse(dataRaw[2]);
        
            st.receiveMessage = null;
        }

        st.Receive();
        //cube.transform.rotation = Quaternion.Euler(RotateZ, -RotateX , -RotateY);//cjmcu-055
    }

    /* private IEnumerator delaySend()
    {
        isSend = false;
        yield return new WaitForSeconds(1);//延遲1秒後才發送
        st.Send("Hello~ My name is Server");
        isSend = true;
    }*/

    private void OnApplicationQuit()//應用程式結束時自動關閉連線
    {
        st.StopConnect();
    }
}

