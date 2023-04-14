package com.embedded.sad;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.embedded.sad.adapters.RecyclerViewCustomAdapter;
import com.embedded.sad.base.baseActivity;
import androidx.appcompat.app.AlertDialog;
import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;


public class MainActivity extends baseActivity {

    private RecyclerView recyclerView_vehicle;
    private RecyclerView recyclerView_orders;
    private RecyclerView.LayoutManager layoutManagerRecyclerView;
    private RecyclerViewCustomAdapter recyclerViewCustomAdapter;
    private RelativeLayout card_wallet;
    private RelativeLayout card_vehicles;
    private RelativeLayout card_orders;

    private TextView wallet_text,vehicles_text, orders_text, wallet_balance_text;

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
            String[] vehicle_data_array = data.split(";");
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
                .setIcon(R.drawable.ic_launcher_background)
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