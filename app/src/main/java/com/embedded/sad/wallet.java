package com.embedded.sad;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.embedded.sad.adapters.RecyclerViewCustomAdapter;
import com.embedded.sad.base.baseActivity;

public class wallet extends baseActivity {

    private RecyclerView recyclerView_history;
    private TextView wallet_balance_tv;
    private RecyclerView.LayoutManager layoutManagerRecyclerView;
    private RecyclerViewCustomAdapter recyclerViewCustomAdapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.wallet_layout);

        recyclerView_history = findViewById(R.id.recyclerview_history);
        wallet_balance_tv = findViewById(R.id.wallet_balance);

        SharedPreferences sharedPreferences = this.getSharedPreferences("DATA", Context.MODE_PRIVATE);
        String balance = sharedPreferences.getString("wallet_balance",null);
        wallet_balance_tv.setText(balance);


        populate_history_data();
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        goto_home();
    }

    private void populate_history_data(){
        String data = readData("history.txt");

        if (data.length() == 0 || data.equalsIgnoreCase(" ")){
            Toast.makeText(this, "No previous orders found", Toast.LENGTH_SHORT).show();
            String[] history_data_array = {"No previous orders found"};
            layoutManagerRecyclerView = new LinearLayoutManager(wallet.this);
            recyclerView_history.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(history_data_array);
            recyclerView_history.setAdapter(recyclerViewCustomAdapter);
        }
        else{
            String[] history_data_array = data.split(";");
            layoutManagerRecyclerView = new LinearLayoutManager(wallet.this);
            recyclerView_history.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(history_data_array);
            recyclerView_history.setAdapter(recyclerViewCustomAdapter);
        }
    }
}