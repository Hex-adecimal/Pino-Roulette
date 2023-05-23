package com.example.pinoroulette;

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
            client.createGroup(groupName);

            try {
                Thread.sleep(1500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if(client.get_last_transaction()){
                Toast.makeText(getApplicationContext(), "Group Created", Toast.LENGTH_LONG).show();
                startActivity(new Intent(getApplicationContext(), Menu.class));
            }
            else
                Toast.makeText(getApplicationContext(), "Group could not be created, try with a different name", Toast.LENGTH_LONG).show();
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