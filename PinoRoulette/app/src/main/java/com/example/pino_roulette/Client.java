package com.example.pino_roulette;

import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Objects;

public class Client {
    private static Client client = null;
    private static String serverName = "192.168.1.16";
    private static int serverPort = 50000;

    private Socket socket;
    private BufferedReader in;
    private PrintWriter out;

    private boolean last_transaction;

    private String username;
    private String password;

    String group_name;

    ArrayList<BaseMessage> messages = null;

    private Client() {
        System.out.println("Client created!");
    }

    public static Client getInstance() {
        if (client == null)
            client = new Client();

        return client;
    }

    public boolean get_last_transaction() {
        boolean ret = this.last_transaction;
        last_transaction = false;
        return ret;
    }

    private void startConnection() {
        try {
            // Creazione socket
            socket = new Socket(serverName, serverPort);
            // creazione stream di input da socket
            in = new BufferedReader(new InputStreamReader( socket.getInputStream()));
            // creazione stream di output su socket
            out = new PrintWriter(new BufferedWriter(
                    new OutputStreamWriter( socket.getOutputStream())), true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void closeConnection() {
        try {
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String communication(String data) {
        String result = null;
        try {
            System.out.println("Sending " + data);
            out.println(data);
            System.out.println("Sended " + data);

            System.out.println("Receiving");
            result = in.readLine();
            System.out.println("Received " + result);
        } catch (IOException e) { e.printStackTrace(); }
        return result;
    }

    void login(String username, String password) {
        new Thread(() -> {
            startConnection();
            communication("login");
            if (Objects.equals(communication(username), "username ok"))
                if (Objects.equals(communication(password), "password ok"))
                    if (Objects.equals(communication("ok"), "login ok")) {
                        last_transaction = true;
                        this.username = username;
                        this.password = password;
                    }
            closeConnection();

        }).start();
    }

    void signup(String username, String password) {
        new Thread(() -> {
            startConnection();
            communication("signup");//change in v2
            if (Objects.equals(communication(username), "username ok"))
                if (Objects.equals(communication(password), "password ok"))
                    if (Objects.equals(communication("ok"), "signup ok"))
                        last_transaction = true;
            closeConnection();
        }).start();
    }

    ArrayList<String> getGroups() {
        ArrayList<String> groups = new ArrayList<>();

        new Thread(() -> {
            startConnection();
            communication("getGroups");
            int nGroups = Integer.parseInt(communication(username));
            for(int i=0; i<nGroups; i++)
                groups.add(communication("gimmeNext"));
            closeConnection();
        }).start();

        return groups;
    }

    void getGroupMessages() {
        ArrayList<BaseMessage> messages = new ArrayList<>();

        new Thread( () -> {
                startConnection();
                communication("recvMessage");
                int nMessages = Integer.parseInt(communication(group_name));
                for (int i=0; i<nMessages; i++) {
                    BaseMessage baseMessage = new BaseMessage();

                    baseMessage.sender = communication("gimmeSender");
                    baseMessage.createdAt = communication("gimmeSentTime");
                    baseMessage.message = communication("gimmeActualMessage");

                    messages.add(baseMessage);
                }

                closeConnection();
        }).start();
        this.messages = messages;
    }

    void sendMessage(String message) {
        new Thread( () -> {
            startConnection();

            communication("sendMessage");
            communication(group_name);
            communication(username);
            communication(message);
            communication(LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss.000")));

            closeConnection();
        }).start();
    }
    public String getUsername(){
        return username;
    }

    void create_Group(String new_group_name) { // nome gruppo
        new Thread(() -> {
            startConnection();;
            communication("createGroup");
            communication(new_group_name);

            int success = Integer.parseInt(communication(username));
            last_transaction = success == 0;

        }).start();
    }

    void join_Group(String name){ // name e' un gruppo
        new Thread(() -> {
            startConnection();;
            communication("joinGroup");
            communication(name);

            int success = Integer.parseInt(communication(username));
            last_transaction = success == 0;

        }).start();
    }

    void add_Member(String name){   // name e' un utente
        new Thread(() -> {
            startConnection();;
            communication("acceptUser");
            communication(name);

            int success = Integer.parseInt(communication(group_name));
            last_transaction = success == 0;

        }).start();
    }

    void remove_from_Pending_List(String name){ // nome utente
        new Thread(() -> {
            startConnection();;
            communication("deleteUser");
            communication(name);

            int success = Integer.parseInt(communication(group_name));
            last_transaction = success == 0;

        }).start();
    }

    ArrayList <String> getPending_List(){
        ArrayList<String> members = new ArrayList<>();
        new Thread(() -> {
            startConnection();
            communication("getPendingList");
            int nUsers = Integer.parseInt(communication(group_name)); // TO-DO
            for (int i=0; i<nUsers;i++){
                members.add(communication("nextUser"));// TO-DO
            }
            closeConnection();

        }).start();
        return members;
    }

    public ArrayList<String> getGroups_of_an_Admin() {
        return null;
    }
}
