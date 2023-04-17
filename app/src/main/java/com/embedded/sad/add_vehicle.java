package com.embedded.sad;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;

import com.embedded.sad.base.baseActivity;

import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.ArrayList;

public class add_vehicle extends baseActivity {

    ArrayAdapter<String> adapterItems;
    String[] item = {"Petrol","Diesel","Electric"};

    AutoCompleteTextView dropdown;
    Button btn_add;
    EditText owner_name,vehicle_name,vehicle_num;
    String prev_vehicle_data = "";

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add_vehicle);

        owner_name = findViewById(R.id.owner_name_edit);
        vehicle_num = findViewById(R.id.vehicle_num_edit);
        vehicle_name = findViewById(R.id.vehicle_name_edit);
        btn_add = findViewById(R.id.btn_add);
        dropdown = findViewById(R.id.dropdown_fueltype_auto);

        adapterItems = new ArrayAdapter<String>(this,R.layout.list_item, item);
        dropdown.setAdapter(adapterItems);

        dropdown.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String item = parent.getItemAtPosition(position).toString();
            }
        });

        btn_add.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String owner =owner_name.getText().toString();
                String vehicle_number = vehicle_num.getText().toString();
                String vehicle_nam = vehicle_name.getText().toString();

                prev_vehicle_data = readData("vehicles.txt");
                String final_vehicle_details = prev_vehicle_data+vehicle_number+"   "+owner+"   "+vehicle_nam+";";

                String resp = writeData("vehicles.txt", final_vehicle_details);


                if (resp.equalsIgnoreCase("success")){
                    AlertDialog.Builder alertDialog = new AlertDialog.Builder(add_vehicle.this)
                            .setIcon(R.drawable.LOGO_1)
                            .setTitle("Success")
                            .setMessage("Vehicle Added Successfully." +
                                    "Do you want to Add another Vehicle?")
                            .setPositiveButton("Yes", new DialogInterface.OnClickListener() {

                                @Override
                                public void onClick(DialogInterface dialogInterface, int i) {
                                    owner_name.getText().clear();
                                    vehicle_name.getText().clear();
                                    vehicle_num.getText().clear();
                                    dropdown.getText().clear();
                                }
                            })

                            .setNegativeButton("No", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialogInterface, int i) {
                                    goto_home();
                                }
                            });

                    alertDialog.show();
                }else{
                    Toast.makeText(add_vehicle.this, "Vehicle cannot be added. Please contact Administrator", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        goto_home();
    }
}
