package com.embedded.sad;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.embedded.sad.R;
import com.embedded.sad.RegisterActivity;

public class login extends AppCompatActivity {

    Button LogInButton, RegisterButton ;
    EditText Email, Password ;
    String EmailHolder, PasswordHolder;
    Boolean EditTextEmptyHolder;
    Cursor cursor;
    String TempPassword = "NOT_FOUND" ;
    String TempNum = "NOT_FOUND" ;
    public static final String UserEmail = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);

        LogInButton = (Button)findViewById(R.id.buttonLogin);
        RegisterButton = (Button)findViewById(R.id.buttonRegister);
        Email = (EditText)findViewById(R.id.editEmail);
        Password = (EditText)findViewById(R.id.editPassword);

        //Adding click listener to log in button.
        LogInButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CheckEditTextStatus();
                LoginFunction();

            }
        });

        // Adding click listener to register button.
        RegisterButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                // Opening new user registration activity using intent on button click.
                Intent intent = new Intent(login.this, RegisterActivity.class);
                startActivity(intent);

            }
        });

    }

    // Login function starts from here.
    public void LoginFunction(){

        if(EditTextEmptyHolder) {

            while (cursor.moveToNext()) {

                if (cursor.isFirst()) {

                    cursor.moveToFirst();

                    TempNum = "9999999999";
                    TempPassword = "0000";

                    // Closing cursor.
                    cursor.close();
                }
            }
            CheckFinalResult();
        }
        else {
            Toast.makeText(login.this,"Please Enter UserName or Password.",Toast.LENGTH_LONG).show();
        }

    }

    public void CheckEditTextStatus(){

        // Getting value from All EditText and storing into String Variables.
        EmailHolder = Email.getText().toString();
        PasswordHolder = Password.getText().toString();

        // Checking EditText is empty or no using TextUtils.
        EditTextEmptyHolder = !TextUtils.isEmpty(EmailHolder) && !TextUtils.isEmpty(PasswordHolder);
    }

    public void CheckFinalResult(){
        if(TempPassword.equalsIgnoreCase(PasswordHolder) && TempNum.equals(EmailHolder))
        {
            Toast.makeText(login.this,"Login Successful",Toast.LENGTH_LONG).show();
            Intent intent = new Intent(login.this, MainActivity.class);
            intent.putExtra(UserEmail, EmailHolder);
            startActivity(intent);
        }
        else {
            Toast.makeText(login.this,"UserName or Password is Wrong, Please Try Again.",Toast.LENGTH_LONG).show();
        }
        TempPassword = "NOT_FOUND" ;

    }

}