using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;


namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private string serverIp = "192.168.0.5";
        private string clientIp = "192.168.0.11";
        private int port;
        private Thread listenThread;
        private Thread reThread;        
        private Socket serverSocket;
               
        public byte[] nomarlMode = new byte[6] { 1, 0, 0, 0, 0, 0 };                                            //데이터 보내는 방법
        public byte[] nomarlModeF = new byte[6] { 1, 1, 0, 0, 0, 0 };                         //    모드      동작      센서Front     센서Back      도킹Search        도킹상태
        public byte[] nomarlModeL = new byte[6] { 1, 2, 0, 0, 0, 0 };                         //      0         0           0           0             0(off)              0
        public byte[] nomarlModeR = new byte[6] { 1, 3, 0, 0, 0, 0 };                         //   1(노말)    1(F)          x           x            1(Start)             x
        public byte[] nomarlModeB = new byte[6] { 1, 4, 0, 0, 0, 0 };                         //   2(도킹)    2(L)          x           x             2(on)               x
        public byte[] nomarlModeS = new byte[6] { 1, 5, 0, 0, 0, 0 };                         //              3(R)          x           x            3(clear)             x
                                                                                              //              4(B)          x           x                                 x
        public byte[] dockingMode = new byte[6] { 2, 0, 0, 0, 2, 255 };                       //              5(S)          x           x                                 x
        public byte[] dockingModeF = new byte[6] { 2, 1, 0, 0, 2, 255 };
        public byte[] dockingModeL = new byte[6] { 2, 2, 0, 0, 2, 255 };
        public byte[] dockingModeR = new byte[6] { 2, 3, 0, 0, 2, 255 };
        public byte[] dockingModeB = new byte[6] { 2, 4, 0, 0, 2, 255 };
        public byte[] dockingModeS = new byte[6] { 2, 5, 0, 0, 2, 255 };

        public byte[] dockingSearch = new byte[6] { 1, 5, 0, 0, 1, 0 };
        //public byte[] dockingStart = new byte[6] { 1, 5, 0, 0, 2, 0 };
        public byte[] dockingClear = new byte[6] { 1, 5, 0, 0, 3, 255 };

        public byte[] exit = new byte[6] { 3, 0, 0, 0, 0, 0 };

        public byte[] re = new byte[512];       //받는배열
           
        int flagTimer = 0;
        int length;
        int modeFlag = 1;
        string result = null;       
               
        public Form1()
        {           
            InitializeComponent();        
        }

        private void Listen()
        {
            if (clientIp == textBox1.Text)
            {

                port = int.Parse(textBox2.Text);
                //종단점
                IPAddress ipaddress = IPAddress.Parse(serverIp);
                IPEndPoint endPoint = new IPEndPoint(ipaddress, port);
                //소켓생성
                Socket listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                //바인드
                listenSocket.Bind(endPoint);
                //준비
                listenSocket.Listen(10);
                //수신대기

                serverSocket = listenSocket.Accept();
                reThread = new Thread(new ThreadStart(ReceiveData));
                reThread.IsBackground = true;
                reThread.Start();
            }   

        }  
        
        private void ReceiveData()
        {
            while (true)
            {
                if (serverSocket != null)
                {
                    if (serverSocket.Connected == true)
                    {
                        length = serverSocket.Receive(re, re.Length, SocketFlags.None);
                        result = Encoding.UTF8.GetString(re, 0, length);
                    }
                }
            }          
        }   

        private void button1_Click(object sender, EventArgs e)      //connect button
        {
            
            flagTimer = 1;
            listenThread = new Thread(new ThreadStart(Listen));
            listenThread.IsBackground = true;
            listenThread.Start();
            /*

           */

        }
                      
        private void button3_Click(object sender, EventArgs e)
        {                    
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true)
                {                    
                     serverSocket.Send(nomarlMode);
                }
            }
        }

        private void button17_Click(object sender, EventArgs e)
        {            
            textBox8.Text = "Docking Mode";
            textBox4.Text = null;
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true)
                    serverSocket.Send(dockingMode);
            }
        }     
              
        private void timer1_Tick(object sender, EventArgs e)    //timer1
        {
            if (flagTimer == 1 )
            {
                textBox3.Text = "connect";
            }
            else
            {
                textBox3.Text = "disconnect";               
            }

            if (re[0] == 1)
            {
               // modeFlag = 1;
                textBox4.Text = "Nomarl Mode";
                textBox8.Text = null;
            }
            else if(re[0] == 2)
            {
               // modeFlag = 2;
                textBox8.Text = "Docking Mode";
                textBox4.Text = null;
            }

            if (re[4] == 2)
            {
                modeFlag = 2;
                textBox5.Text = "Docking Done";
            }
            else if (re[4] == 0)
            {
                modeFlag = 1;
                textBox5.Text = "Clear Done";
            }

            textBox6.Text = re[2].ToString();
            textBox7.Text = re[3].ToString();
                           
        }

        private void button2_Click(object sender, EventArgs e)  //disconnet button
        {
            //스레드 종료시키고 소켓 비우기
            flagTimer = 0;
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true)
                    serverSocket.Send(exit);
            }
            serverSocket.Close(); 
        }

        

        private void button4_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag ==1)
                    serverSocket.Send(nomarlModeF);                
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 1)
                    serverSocket.Send(nomarlModeR);
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 1)
                    serverSocket.Send(nomarlModeB);
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 1)
                    serverSocket.Send(nomarlModeL);
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 1)
                    serverSocket.Send(nomarlModeS);
            }
        }

        private void button16_Click(object sender, EventArgs e)
        {
            /*
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeF);
            }
            */
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeR);
            }
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeB);
            }
        }

        private void button13_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeL);
            }
        }       

        private void button11_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeS);
            }
        }
                
        private void button15_Click(object sender, EventArgs e)
        {

            if (serverSocket != null)
            {
                if (serverSocket.Connected == true)
                    serverSocket.Send(dockingSearch);
            }
        }
        private void button9_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true)
                    serverSocket.Send(dockingClear);
            }
        }

        private void button14_Click(object sender, EventArgs e)
        {
            if (serverSocket != null)
            {
                if (serverSocket.Connected == true && modeFlag == 2)
                    serverSocket.Send(dockingModeF);
            }           
        }
        private void groupBox5_Enter(object sender, EventArgs e)
        {

        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void timer2_Tick(object sender, EventArgs e)
        {

        }

        private void textBox6_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
