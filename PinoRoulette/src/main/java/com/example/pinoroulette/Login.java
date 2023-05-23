package com.example.pinoroulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class Login extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        Client client = Client.getInstance();

        EditText edt_username = findViewById(R.id.edt_username);
        EditText edt_password = findViewById(R.id.edt_password);

        Button btn_login = findViewById(R.id.btn_login);
        btn_login.setOnClickListener(view -> {
            String username = edt_username.getText().toString();
            String password = edt_password.getText().toString();

            client.login(username, password);

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if (client.get_last_transaction()) {
                Toast.makeText(getApplicationContext(), "logged in!", Toast.LENGTH_LONG).show();
                startActivity(new Intent(getApplicationContext(), Menu.class));
            } else
                Toast.makeText(getApplicationContext(), "Wrong credential!", Toast.LENGTH_LONG).show();
        });

        TextView txt_signup = findViewById(R.id.txt_signup);
        txt_signup.setOnClickListener(view -> {
            startActivity(new Intent(getApplicationContext(), Signup.class));
        });
    }
}