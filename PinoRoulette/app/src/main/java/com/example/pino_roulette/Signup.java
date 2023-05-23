package com.example.pino_roulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Signup extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        EditText edt_username = findViewById(R.id.edt_username);
        EditText edt_passwrod = findViewById(R.id.edt_password);
        EditText edt_passwrod_confirm = findViewById(R.id.edt_password2);

        Button btn_signup = findViewById(R.id.btn_signup);
        btn_signup.setOnClickListener(view -> {
            String username = edt_username.getText().toString();
            String password = edt_passwrod.getText().toString();
            String ControllPassword = edt_passwrod_confirm.getText().toString();
            if (!(password.equals(ControllPassword))) {
                edt_passwrod_confirm.setError("Le password non coincidono");
                edt_passwrod_confirm.setText("");
            }
            else {
                client.signup(username, password);
                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            if (client.get_last_transaction()) {
                Toast.makeText(getApplicationContext(), "signed up!", Toast.LENGTH_LONG).show();
                startActivity(new Intent(getApplicationContext(), Login.class));
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                startActivity(new Intent(getApplicationContext(), Login.class));
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

}