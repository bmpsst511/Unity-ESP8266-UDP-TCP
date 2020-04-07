using UnityEngine;
using System.Collections;  
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class UDPserver_send : MonoBehaviour
{
    string editString="hello wolrd"; //UI文字
    Socket socket;
    EndPoint clientEnd; 
    IPEndPoint ipEnd; 
    string recvStr; 
    string sendStr; 
    byte[] recvData = new byte[1024]; 
    byte[] sendData = new byte[1024]; 
    int recvLen; 
    Thread connectThread; 




    void InitSocket()
    {

        ipEnd = new IPEndPoint(IPAddress.Any, 0);
    
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
   
        socket.Bind(ipEnd);
 
        IPEndPoint sender = new IPEndPoint(IPAddress.Parse("192.168.0.102"), 8001);//綁定客戶端IP位址及Port端口
        clientEnd = (EndPoint)sender;
        print("waiting for UDP dgram");

   
        connectThread = new Thread(new ThreadStart(SocketReceive));
        connectThread.Start();
    }

    void SocketSend(string sendStr)
    {

        sendData = new byte[1024];

        

        sendData = Encoding.ASCII.GetBytes(sendStr);
 
        socket.SendTo(sendData, sendData.Length, SocketFlags.None, clientEnd);
    }

 
    void SocketReceive()
    {

        while (true)
        {
  
            recvData = new byte[1024];
  
            recvLen = socket.ReceiveFrom(recvData, ref clientEnd);
            //print(recvLen);
            print("message from: " + clientEnd.ToString()); 
 
            recvStr = Encoding.ASCII.GetString(recvData, 0, recvLen);
            print(recvStr);
            sendStr="123";
            SocketSend(sendStr);

        }
    }


    void SocketQuit()
    {
    
        if (connectThread != null)
        {
            connectThread.Interrupt();
            connectThread.Abort();
        }

        if (socket != null)
            socket.Close();
        print("disconnect");
    }

    // Use this for initialization  
    void Start()
    {
        InitSocket(); 
    }


    // Update is called once per frame  
    void FixedUpdate()
    {
    }
    void OnGUI()
    {
        editString=GUI.TextField(new Rect(10,10,100,20),editString);
        if(GUI.Button(new Rect(10,30,60,20),"send"))
            SocketSend(editString);//從伺服端發送給客戶端想要的資料
    }
    void OnApplicationQuit()
    {
        SocketQuit();
    }
}
