package ch3;

import java.net.*;
import java.io.*;

public class QuoteOfTheDayServer {
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6017);
            /* now listen for connections */
            while (true) {
                Socket client = sock.accept();
                PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                /* write the quote of the day to the socket */
                pout.println("This is the fcking quote \n with two fcking lines");
                /* close the socket and resume */
                /* listening for connections */
                client.close();
            }
        } catch (IOException ioe) {
            System.err.println(ioe);
        }

    }
}
