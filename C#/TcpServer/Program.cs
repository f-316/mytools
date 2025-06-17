using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

class TcpServer
{
    private const int Port = 12345;

    public static async Task StartServer()
    {
        TcpListener listener = new TcpListener(IPAddress.Any, Port);
        listener.Start();
        Console.WriteLine($"Server started on port {Port}");

        while (true)
        {
            TcpClient client = await listener.AcceptTcpClientAsync();
            _ = Task.Run(() => HandleClientAsync(client));
        }
    }

    private static async Task HandleClientAsync(TcpClient client)
    {
        Console.WriteLine("Client connected");
        using (NetworkStream stream = client.GetStream())
        {
            byte[] buffer = new byte[1024];
            while (true)
            {
                int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
                if (bytesRead == 0) break;

                string message = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                Console.WriteLine($"Received: {message}");

                byte[] response = Encoding.UTF8.GetBytes("Message received");
                await stream.WriteAsync(response, 0, response.Length);
            }
        }
        client.Close();
        Console.WriteLine("Client disconnected");
    }

    public static async Task Main()
    {
        await StartServer();
    }
}