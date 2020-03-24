
using UnityEngine;
using System.Collections;
//引入庫
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

public class TCP_Cube:MonoBehaviour
{
    //以下默認都是私有的成員
    Socket serverSocket; //目標socket
    Socket clientSocket; //客户端
    IPEndPoint ipEnd; //偵聽端口
    string recvStr; //接收的字符串
    string sendStr; //發送的字符串
    byte[] recvData=new byte[1024]; //接收的數據，必須為字節
    byte[] sendData=new byte[1024]; //發送的數據，必須為字節
    int recvLen; //接收的數據長度
    Thread connectThread; //連接線程
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

    //連接
    void SocketConnet()
    {
        if(clientSocket!=null)
            clientSocket.Close();
        //控制台輸出偵聽狀態
        //print("Waiting for a client");
        //一旦接受連接，創建一個客戶端
        clientSocket=serverSocket.Accept();
        //獲取客戶端IP和端口
        IPEndPoint ipEndClient=(IPEndPoint)clientSocket.RemoteEndPoint;
        //輸出客戶端的IP和端口
        //print("Connect with "+ipEndClient.Address.ToString()+":"+ipEndClient.Port.ToString());
    }

   /* void SocketSend(string sendStr)
    {
        //清空發送緩存
        sendData=new byte[1024];
        //數據類型轉換
        sendData=Encoding.ASCII.GetBytes(sendStr);
        //發送給指定客戶端
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

    //連接關閉
    void SocketQuit()
    {
        //先關閉客戶端
        if(clientSocket!=null)
            clientSocket.Close();
        //再關閉線程
        if(connectThread!=null)
        {
            connectThread.Interrupt();
            connectThread.Abort();
        }
        //最後關閉socket
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
