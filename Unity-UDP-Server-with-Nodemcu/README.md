# Unity-UDP-Server-with-Nodemcu
It's about how to achieve wireless BNO55(9DOF) transfor data to Unity through UDP Server.
It took me two weeks to finish that, there still have some problems, like when device connect and transfor too long,
the Unity UI may crushed. So need some time make it more smooth and perfect. 

UDPServer_send.cs UDPClient_receive.ino 為Unity是主伺服端(Server)傳送給WemosD1 mini客戶端(Client) 數據
UDPClient_send.cs UDPServer_receive.ino 為Unity是客戶端(Client)傳送給WemosD1 mini主伺服端(Server)數據
