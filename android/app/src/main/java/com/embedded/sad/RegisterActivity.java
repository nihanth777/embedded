package com.embedded.sad;

import android.content.Context;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.embedded.sad.base.baseActivity;

public class RegisterActivity extends baseActivity {

    EditText Email, Password, Name ;
    Button Register;
    String NameHolder, EmailHolder, PasswordHolder, TempNum, TempPassword;
    Boolean EditTextEmptyHolder;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);


        Register = (Button)findViewById(R.id.buttonRegister);
        Email = (EditText)findViewById(R.id.editEmail);
        Password = (EditText)findViewById(R.id.editPassword);
        Name = (EditText)findViewById(R.id.editName);

        // Adding click listener to register button.
        Register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CheckEditTextStatus();

            }
        });
    }

    // Empty edittext after done inserting process method.
    public void EmptyEditTextAfterDataInsert(){
        Name.getText().clear();
        Email.getText().clear();
        Password.getText().clear();
    }

    // Method to check EditText is empty or Not.
    public void CheckEditTextStatus(){

        // Getting value from All EditText and storing into String Variables.
        NameHolder = Name.getText().toString() ;
        EmailHolder = Email.getText().toString();
        PasswordHolder = Password.getText().toString();

        EditTextEmptyHolder = !TextUtils.isEmpty(NameHolder) && !TextUtils.isEmpty(EmailHolder) && !TextUtils.isEmpty(PasswordHolder);

        SharedPreferences prefs = getSharedPreferences("sharedPref", Context.MODE_PRIVATE);
        TempNum = prefs.getString("number","");
        TempPassword = prefs.getString("password","");

        if (NameHolder != null && EmailHolder != null && PasswordHolder != null){

            if(EmailHolder.length()==10){

                if(TempNum == EmailHolder){
                    Toast.makeText(this, "Account already Exists", Toast.LENGTH_SHORT).show();
                }else{

                    SharedPreferences.Editor edit = prefs.edit();
                    edit.putString("number",EmailHolder);
                    edit.putString("password", PasswordHolder);
                    edit.putString("name", NameHolder);
                    edit.apply();

                    goto_login();
                    finish();
                }

            }else{
                Toast.makeText(this, "Please check the Phone Number", Toast.LENGTH_SHORT).show();
                EmptyEditTextAfterDataInsert();
            }
        }else{
            Toast.makeText(this, "Please fill all details", Toast.LENGTH_SHORT).show();
            EmptyEditTextAfterDataInsert();
        }
    }

}