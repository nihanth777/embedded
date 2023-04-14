package com.embedded.sad;

import android.os.Bundle;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.embedded.sad.adapters.RecyclerViewCustomAdapter;
import com.embedded.sad.base.baseActivity;

public class wallet extends baseActivity {

    private RecyclerView recyclerView_history;
    private RecyclerView.LayoutManager layoutManagerRecyclerView;
    private RecyclerViewCustomAdapter recyclerViewCustomAdapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.wallet_layout);
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
            Toast.makeText(this, "Please Register Your Vehicle", Toast.LENGTH_SHORT).show();
            String[] history_data_array = {"Please Register Your Vehicle"};
            layoutManagerRecyclerView = new LinearLayoutManager(wallet.this);
            recyclerView_history.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(history_data_array);
            recyclerView_history.setAdapter(recyclerViewCustomAdapter);
        }
        else{
            String[] history_data_array = data.split(",");
            layoutManagerRecyclerView = new LinearLayoutManager(wallet.this);
            recyclerView_history.setLayoutManager(layoutManagerRecyclerView);
            recyclerViewCustomAdapter = new RecyclerViewCustomAdapter(history_data_array);
            recyclerView_history.setAdapter(recyclerViewCustomAdapter);
        }
    }
}