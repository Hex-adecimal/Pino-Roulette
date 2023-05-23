package com.example.pino_roulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class JoinGroup extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_join_group);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        Button join = findViewById(R.id.btn_sbmt);
        EditText requested_group = findViewById(R.id.edt_group_name);

        join.setOnClickListener(view -> {
            String name = requested_group.getText().toString();
            client.join_Group(name);

            try {
                Thread.sleep(400);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }


            // Controllo (se ok torna in menu se non ok svuota EditText)
            if(client.get_last_transaction()){
                Toast.makeText(getApplicationContext(), "Joined in a Group with Success", Toast.LENGTH_LONG).show();
                startActivity(new Intent(getApplicationContext(), Menu.class));
            }
            else{
                Toast.makeText(getApplicationContext(), "Couldn't join the specific group,please insert a valid group_name", Toast.LENGTH_LONG).show();
                //To Do : il testo in Edit deve azzerarsi, L'applicativo deve poter scegliere se accettare un'altro group_name oppure andare indietro

            }

        });
    }


    // Actually go back
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            startActivity(new Intent(getApplicationContext(), Menu.class));
            // TODO : delete username password
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


}