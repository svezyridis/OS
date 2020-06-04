package ch4;

import java.net.*;
import java.util.concurrent.TimeUnit;
import java.io.*;

public class Exercise26 {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6013);
            /* now listen for connections */
            while (true) {
                Socket client = sock.accept();
                Thread t = new Thread(() -> print(client));
                t.start();
            }
        } catch (IOException ioe) {
            System.err.println(ioe);
        }
    }

    private static void print(Socket client) {
        // to check if it runs in parallel
        try {
            TimeUnit.SECONDS.sleep(3);
        } catch (InterruptedException e1) {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }
        PrintWriter pout;
        try {
            pout = new PrintWriter(client.getOutputStream(), true);
            pout.println(new java.util.Date().toString());
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
