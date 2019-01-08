
using UnityEngine;
using System.Collections;
//引入库
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class TCP_Cube:MonoBehaviour
{
    //以下默认都是私有的成员
    Socket serverSocket; //服务器端socket
    Socket clientSocket; //客户端socket
    IPEndPoint ipEnd; //侦听端口
    string recvStr; //接收的字符串
    string sendStr; //发送的字符串
    byte[] recvData=new byte[1024]; //接收的数据，必须为字节
    byte[] sendData=new byte[1024]; //发送的数据，必须为字节
    int recvLen; //接收的数据长度
    Thread connectThread; //连接线程
    public int RotateX, RotateY, RotateZ;
    System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();//引用stopwatch物件

    public GameObject cube;

    //初始化
    void InitSocket()
    {
        //定義監聽端口,監聽任何IP
        ipEnd=new IPEndPoint(IPAddress.Any,27);
        //定義套接字類型,在主線程中定義
        serverSocket=new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);
        //伺服端绑定ip
        serverSocket.Bind(ipEnd);
        //開始監聽,最多10個同時監聽
        serverSocket.Listen(10);
        //使用新的執行緒執行連線，否則遊戲會卡死
        connectThread=new Thread(new ThreadStart(SocketReceive));
        connectThread.Start();
    }

    //连接
    void SocketConnet()
    {
        if(clientSocket!=null)
            clientSocket.Close();
        //控制台输出侦听状态
        //print("Waiting for a client");
        //一旦接受连接，创建一个客户端
        clientSocket=serverSocket.Accept();
        //获取客户端的IP和端口
        IPEndPoint ipEndClient=(IPEndPoint)clientSocket.RemoteEndPoint;
        //输出客户端的IP和端口
        //print("Connect with "+ipEndClient.Address.ToString()+":"+ipEndClient.Port.ToString());
    }

   /* void SocketSend(string sendStr)
    {
        //清空发送缓存
        sendData=new byte[1024];
        //数据类型转换
        sendData=Encoding.ASCII.GetBytes(sendStr);
        //发送
        clientSocket.Send(sendData,sendData.Length,SocketFlags.None);
    }*/

    //伺服器接收
    void SocketReceive()
    {
        //連接
        SocketConnet();      
        //接收迴圈
        while(true)
        {
            sw.Start();
            for(int i=0;i<10;i++)
            {
            var filter = new LowPassFilter(0.95f);
            //清空數據
            recvData=new byte[1024];
            //獲取收到的數據長度
            recvLen=clientSocket.Receive(recvData);
            //如果收到的數據長度為0，則重新連線進入下一個循環
              if(recvLen==0)
            {
                SocketConnet();
                continue;
            }
            //輸出接收到的數據
            recvStr=Encoding.ASCII.GetString(recvData,0,recvLen);
            print(recvStr);
			  //分割字串
            char[] splitChar = { ' ', ',', ':', '\t', ';' };
            string[] dataRaw = recvStr.Split(splitChar);
            RotateX = int.Parse(dataRaw[0]);
            RotateY = int.Parse(dataRaw[1]);
            RotateZ = int.Parse(dataRaw[2]);
            filter.Step(RotateX);
            filter.Step(RotateY);
            filter.Step(RotateZ);
            }
            sw.Stop();//碼錶停止
            string result1 = sw.Elapsed.TotalMilliseconds.ToString();
            print(result1); 
        }
    }

    //连接关闭
    void SocketQuit()
    {
        //先关闭客户端
        if(clientSocket!=null)
            clientSocket.Close();
        //再关闭线程
        if(connectThread!=null)
        {
            connectThread.Interrupt();
            connectThread.Abort();
        }
        //最后关闭服务器
        serverSocket.Close();
        print("diconnect");
    }

    // Use this for initialization
    void Start()
    {        
        sw.Reset();//碼表歸零
        InitSocket(); //在这里初始化server
    }


    // Update is called once per frame
    void FixedUpdate()
    {
		//SocketReceive();
        cube.transform.rotation = Quaternion.Euler(RotateZ, -RotateX , -RotateY);//cjmcu-055
    }

    void OnApplicationQuit()
    {
        SocketQuit();
    }
        public class LowPassFilter
    {
        private float _smoothingFactor;
        public float SmoothedValue;
        public LowPassFilter(float smoothingFactor)
        {
            _smoothingFactor = smoothingFactor;
        }

        public void Step(float sensorValue)
        {
            SmoothedValue = _smoothingFactor * sensorValue + (1 - _smoothingFactor) * SmoothedValue;
        }
    }
}
