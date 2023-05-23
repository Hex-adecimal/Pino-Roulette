package com.example.pino_roulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;

public class Menu extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        // Display the back button on left up corner
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        ArrayList<String> groups = client.getGroups();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Display groups
        ListView list_chat = findViewById(R.id.list_chat);
        ArrayAdapter<String> arr = new ArrayAdapter<>(this, R.layout.group, groups);
        list_chat.setAdapter(arr);

        // Go to a specific group
        list_chat.setOnItemClickListener((adapterView, view, i, l) -> {
            client.group_name = groups.get(i);
            startActivity(new Intent(getApplicationContext(), Group.class));
        });

        //Button tendina = findViewById(R.id.btn_tendina);
        Button creation_group = findViewById(R.id.btn_Create_Group);
        Button join_a_group = findViewById(R.id.btn_Join_Group);
        Button my_groups = findViewById(R.id.btn_My_Groups);

        /*
        tendina.setOnClickListener(view -> {
            tendina.setVisibility(View.INVISIBLE);
            creation_group.setVisibility(View.VISIBLE);
            join_a_group.setVisibility(View.VISIBLE);
            my_groups.setVisibility(View.VISIBLE);
        });*/


        creation_group.setOnClickListener(view -> {
            startActivity(new Intent(getApplicationContext(),CreateGroup.class));
        });
        join_a_group.setOnClickListener(view -> {
            startActivity(new Intent(getApplicationContext(), JoinGroup.class));
        });
        my_groups.setOnClickListener(view -> {
            startActivity(new Intent(getApplicationContext(), MyGroups.class));
        });




    }

    // Actually go back
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            startActivity(new Intent(getApplicationContext(), Login.class));
            // TODO : delete username password
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    /*
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.three_dots, menu);


        return true;
    }*/
}