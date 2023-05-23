package com.example.pino_roulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class CreateGroup extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_group);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        Button create = findViewById(R.id.btn_sbmt);
        EditText edt_groupName = findViewById(R.id.edt_group_name);

        create.setOnClickListener( view -> {
            String groupName = edt_groupName.getText().toString();
            client.create_Group(groupName);

            try {
                Thread.sleep(400);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }


            // Controllo (se ok torna in menu se non ok svuota EditText)
            if(client.get_last_transaction()){
                Toast.makeText(getApplicationContext(), "Group Created", Toast.LENGTH_LONG).show();
                startActivity(new Intent(getApplicationContext(), Menu.class));
            }
            else{
                Toast.makeText(getApplicationContext(), "Group could not be created, try with a different name", Toast.LENGTH_LONG).show();
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