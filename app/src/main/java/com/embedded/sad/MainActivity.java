package com.embedded.sad;
import android.annotation.SuppressLint;
import android.content.AsyncQueryHandler;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.StrictMode;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.embedded.sad.adapters.RecyclerViewCustomAdapter;
import com.embedded.sad.base.baseActivity;

import androidx.appcompat.app.AlertDialog;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Calendar;


public class MainActivity extends baseActivity {

    private RecyclerView recyclerView_vehicle;
    private RecyclerView recyclerView_orders;
    private RecyclerView.LayoutManager layoutManagerRecyclerView;
    private RecyclerViewCustomAdapter recyclerViewCustomAdapter;
    private RelativeLayout card_wallet;
    private RelativeLayout card_vehicles;
    private RelativeLayout card_orders;

    private TextView wallet_text,vehicles_text, orders_text, wallet_balance_text;

    MyThread_send myThread_send;

    String prev_order_data = "";
    String prev_history_data = "";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.content_main);

        card_wallet = findViewById(R.id.card1);
        card_vehicles = findViewById(R.id.card2);
        card_orders =findViewById(R.id.card3);

        wallet_text = findViewById(R.id.text_wallet);
        vehicles_text = findViewById(R.id.text_vehicle);
        orders_text = findViewById(R.id.text_order);
        wallet_balance_text = findViewById(R.id.wallet_balance);

        recyclerView_vehicle = findViewById(R.id.recyclerview_vehicles);
        recyclerView_orders = findViewById(R.id.recyclerview_orders);


        SharedPreferences sharedPreferences = this.getSharedPreferences("DATA",Context.MODE_PRIVATE);
        String balance = sharedPreferences.getString("wallet_balance",null);
        wallet_balance_text.setText(balance);

        populate_vehicle_data();
        populate_orders_data();

        card_wallet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goto_wallet();
            }
        });
        wallet_text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goto_wallet();
            }
        });
        wallet_balance_text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goto_wallet();
            }
        });
        vehicles_text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goto_vehicles();
            }
        });
        orders_text.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goto_orders();
            }
        });

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        myThread_send = new MyThread_send();
       // new Thread(myThread_send).start();

        Thread myThread_rec = new Thread(new MyThread_receive());
        myThread_rec.start();

    }

    private class MyThread_send implements Runnable{

        private volatile String msg = "";
        Socket sock;
        PrintWriter pw;
        DataOutputStream dos;
        @Override
        public void run() {
            try {
                Toast.makeText(MainActivity.this, "wallet balance sent in", Toast.LENGTH_SHORT).show();
                sock = new Socket("192.168.29.84", 8009);
//                dos = new DataOutputStream(socket.getOutputStream());
//                dos.writeUTF(msg);
//                dos.close();
//                dos.flush();
                pw = new PrintWriter(sock.getOutputStream());
                pw.write(msg);
                pw.flush();
                pw.close();
                sock.close();
                Toast.makeText(MainActivity.this, "wallet balance sent out", Toast.LENGTH_SHORT).show();

            }catch (IOException e){
                e.printStackTrace();
                Toast.makeText(MainActivity.this, e.toString(), Toast.LENGTH_SHORT).show();
            }
        }

        public void sendMsg(String msg){
            this.msg = msg;
            run();
        }
    }

    private class MyThread_receive implements Runnable{
        Socket s;
        ServerSocket ss;
        InputStreamReader isr;
        BufferedReader bufferedReader;
        Handler h = new Handler();
        String Message;

        @Override
        public void run() {

            try {
                ss = new ServerSocket(port_receive);
                while (true){
                    s=ss.accept();
                    isr = new InputStreamReader(s.getInputStream());
                    bufferedReader = new BufferedReader(isr);
                    Message = bufferedReader.readLine();

                    h.post(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                if(Message == null){
                                    Toast.makeText(MainActivity.this, "null message received", Toast.LENGTH_SHORT).show();
                                }
                                else if(Message.equalsIgnoreCase("get balance")){
                                    Toast.makeText(MainActivity.this, "get balance received", Toast.LENGTH_SHORT).show();
                                    SharedPreferences sharedPreferences = MainActivity.this.getSharedPreferences("DATA",Context.MODE_PRIVATE);
                                    String msg = sharedPreferences.getString("wallet_balance",null);
                                    myThread_send.sendMsg(msg);

                                } else {
                                    Toast.makeText(MainActivity.this, Message, Toast.LENGTH_SHORT).show();
                                    String[] order_msg = Message.split(",");

                                    prev_order_data = readData("orders.txt");
                                    String final_order_details = prev_order_data+order_msg[0]+"    "+order_msg[1]+"    £"+order_msg[2]+";";
                                    String resp = writeData("orders.txt", final_order_details);


                                    Calendar calendar = Calendar.getInstance();
                                    @SuppressLint("SimpleDateFormat") SimpleDateFormat mdFormat = new SimpleDateFormat("HH:mm:ss");
                                    String strTime = mdFormat.format(calendar.getTime());

                                    prev_history_data = readData("history.txt");
                                    String final_history_details = prev_history_data+order_msg[0]+"    "+strTime+"    £"+order_msg[2]+";";
                                    String resp1 = writeData("history.txt", final_history_details);

                                    if(resp.equalsIgnoreCase("success") && resp1.equalsIgnoreCase("success")){

                                        Toast.makeText(MainActivity.this, "Orders and Transaction history are updated succesfully", Toast.LENGTH_SHORT).show();

                                        SharedPreferences sharedPreferences = getSharedPreferences("DATA",Context.MODE_PRIVATE);
                                        String balance = sharedPreferences.getString("wallet_balance",null);
                                        String final_balance = String.valueOf(Double.parseDouble(balance) - Double.parseDouble(order_msg[2]));
                                        sharedPreferences.edit().putString("wallet_balance",final_balance).apply();

                                        finish();
                                        startActivity(getIntent());
                                    } else if (resp.equalsIgnoreCase("success") && !("success").equalsIgnoreCase(resp1)) {

                                        Toast.makeText(MainActivity.this, "ERROR : Transaction history", Toast.LENGTH_SHORT).show();

                                    }else if (resp1.equalsIgnoreCase("success") && !("success").equalsIgnoreCase(resp)) {

                                        Toast.makeText(MainActivity.this, "ERROR : Orders", Toast.LENGTH_SHORT).show();


                                    }

                                }
                            }catch (Exception e){
                                e.printStackTrace();
                            }

                            //Toast.makeText(MainActivity.this, Message, Toast.LENGTH_SHORT).show();
                        }
                    });
                }

            }catch (IOException e){
                e.printStackTrace();
            }

        }
    }

    private void populate_vehicle_data(){
        String data = readData("vehicles.txt");


        if (data.length() == 0 || data.equalsIgnoreCase(" ")){
            Toast.makeText(this, "Please Register Your Vehicle", Toast.LENGTH_SHORT).show();
            String[] vehicle_data_array = {"Please Register Your Vehicle"};
            layoutManagerRecyclerView = new LinearLayoutManager(MainActivity.this);
            recyclerView_vehicle.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(vehicle_data_array);
            recyclerView_vehicle.setAdapter(recyclerViewCustomAdapter);
        }
        else{
            String[] vehicle_data_array = data.split(",");
            layoutManagerRecyclerView = new LinearLayoutManager(MainActivity.this);
            recyclerView_vehicle.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(vehicle_data_array);
            recyclerView_vehicle.setAdapter(recyclerViewCustomAdapter);
        }

    }

    private void populate_orders_data(){
        String data = readData("orders.txt");

        if(data.length() == 0 || data.equalsIgnoreCase(" ")){
            Toast.makeText(this, "No Previous Orders Found", Toast.LENGTH_SHORT).show();
            String[] orders_data_array = {"No Previous Orders Found"};
            layoutManagerRecyclerView = new LinearLayoutManager(MainActivity.this);
            recyclerView_orders.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(orders_data_array);
            recyclerView_orders.setAdapter(recyclerViewCustomAdapter);
        }
        else{
            String[] orders_data_array = data.split(",");

            layoutManagerRecyclerView = new LinearLayoutManager(MainActivity.this);
            recyclerView_orders.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(orders_data_array);
            recyclerView_orders.setAdapter(recyclerViewCustomAdapter);
        }

    }


    @Override
    public void onBackPressed() {
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(this)
                .setIcon(R.drawable.sad_logo)
                .setTitle("Exit")
                .setMessage("Are you sure to Exit")
                .setPositiveButton("Yes", new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        //set what would happen when positive button is clicked
                        finish();
                    }
                })

                .setNegativeButton("No", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        //set what should happen when negative button is clicked
                        Toast.makeText(getApplicationContext(),"Thanks for staying!!",Toast.LENGTH_LONG).show();
                    }
                });

        alertDialog.show();
    }
}