package com.embedded.sad.base;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.embedded.sad.MainActivity;
import com.embedded.sad.add_vehicle;
import com.embedded.sad.contactUs;
import com.embedded.sad.login;
import com.embedded.sad.wallet;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class baseActivity extends AppCompatActivity {

    public String wallet_balance;

    public String getSomeVariable() {
        return wallet_balance;
    }

    public void setSomeVariable(String balance) {
        this.wallet_balance = balance;
    }

    public String readData(String file)
    {
        String data = "";
        try
        {
            FileInputStream fin = openFileInput(file);
            int a;

            StringBuilder temp = new StringBuilder();
            while ((a = fin.read()) != -1)
            {
                temp.append((char)a);
            }
            data = temp.toString();
            fin.close();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        Toast.makeText(this, "reading to file \" + filename + \" completed..", Toast.LENGTH_SHORT).show();
        return data;
    }

    public String writeData(String filename, String file) {

        try {
            FileOutputStream fos = openFileOutput(filename, Context.MODE_PRIVATE);
            fos.write(file.getBytes());
            fos.flush();
            fos.close();
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(this, "Internal storage write error", Toast.LENGTH_SHORT).show();
        }
        return "Success";
    }

    public void goto_home(){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }

    public void goto_contact_us(){
        Intent intent = new Intent(this, contactUs.class);
        startActivity(intent);
        finish();
    }

    public void goto_wallet(){
        Intent intent = new Intent(this, wallet.class);
        startActivity(intent);
        finish();
    }

    public void goto_orders(){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }

    public void goto_vehicles(){
        Intent intent = new Intent(getApplicationContext(), add_vehicle.class );
        startActivity(intent);
        finish();
    }

    public void goto_login(){
        Intent intent = new Intent(this, login.class);
        startActivity(intent);
        finish();
    }


}
