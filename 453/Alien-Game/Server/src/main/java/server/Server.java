package main.java.server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

public class Server {
    private final static int SERVER_PORT = 8082;

    static Vector<ClientHandler> clientHandlers = new Vector<>();

    static Integer clientNumber = 0;
    static Integer counter = 0;

    public static void main(String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(SERVER_PORT);

        Socket s;

        while (true) {
            s = ss.accept();

            System.out.println("New client request received : " + s);

            DataInputStream dis = new DataInputStream(s.getInputStream());
            DataOutputStream dos = new DataOutputStream(s.getOutputStream());

            System.out.println("Creating a new handler for this client...");

            ClientHandler clientHandler = new ClientHandler(s, "client " + clientNumber, dis, dos);

            Thread t = new Thread(clientHandler);

            System.out.println("Adding this client to active client list");
            System.out.println("client " + clientNumber);
            clientHandlers.add(clientHandler);


            clientNumber++;
            if (clientNumber == 2) {
                clientNumber = 0;
            }

            t.start();

        }
    }
}

class ClientHandler implements Runnable {
    private String name;
    private final DataInputStream dis;
    private final DataOutputStream dos;
    private Socket s;

    ClientHandler(Socket s, String name,
                  DataInputStream dis, DataOutputStream dos) {
        this.dis = dis;
        this.dos = dos;
        this.name = name;
        this.s = s;
    }

    @Override
    public void run() {

        String received;
        while (true) {
            try {
                received = dis.readUTF();

                System.out.println(received);
                if (received.equals("logout")) {
                    for (ClientHandler mc : Server.clientHandlers) {
                        mc.s.close();
                        mc.dis.close();
                        mc.dos.close();
                    }
                    Server.clientHandlers.clear();
                    break;
                } else if (received.contains("close")) {
                    this.dos.close();
                    this.dis.close();
                    this.s.close();
                    Server.clientHandlers.remove(this);
                    if (Server.counter == 1 && received.split("-")[1].equals("4")) {
                        Server.counter = 0;
                    }
                    if (this.name.equals("client 1")) {
                        Server.clientNumber = 1;
                    }
                } else if (received.equals("level4")) {
                    Server.counter++;
                    if (Server.counter == 2) {
                        System.out.println("starting level 4");
                        Server.clientHandlers.get(0).dos.writeUTF("start");
                        Server.clientHandlers.get(1).dos.writeUTF("start");
                        Server.counter = 0;
                    }
                } else if (received.contains("user")) {
                    if (Server.clientHandlers.get(0).name.equals("client 1")) {
                        ClientHandler clientHandler = Server.clientHandlers.get(1);
                        Server.clientHandlers.set(1, Server.clientHandlers.get(0));
                        Server.clientHandlers.set(0, clientHandler);
                    }
                    if (this.name.equals("client 0")) {
                        Server.clientHandlers.get(1).dos.writeUTF(received);
                        System.out.println("0dan 1e" + received);
                    } else if (this.name.equals("client 1")) {
                        Server.clientHandlers.get(0).dos.writeUTF(received);
                        System.out.println("1den 0a " + received);
                    }
                } else {
                    if (this.name.equals("client 0")) {
                        Server.clientHandlers.get(1).dos.writeUTF(received);
                        System.out.println("0dan 1e");

                    } else if (this.name.equals("client 1")) {
                        Server.clientHandlers.get(0).dos.writeUTF(received);
                        System.out.println("1den 0a");
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
                break;
            }

        }

    }
}

