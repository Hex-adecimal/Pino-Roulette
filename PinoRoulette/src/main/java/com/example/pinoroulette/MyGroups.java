package com.example.pinoroulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;

public class MyGroups extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_groups);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        ArrayList<String> groups = client.getAdminGroups();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Display groups
        ListView list_chat = findViewById(R.id.list_groups);
        ArrayAdapter<String> arr = new ArrayAdapter<>(this, R.layout.group, groups);
        list_chat.setAdapter(arr);

        // Select a specific PendingList
        list_chat.setOnItemClickListener((adapterView, view, i, l) -> {
            client.group_name = groups.get(i);
            startActivity(new Intent(getApplicationContext(), PendingList.class));
        });
    }

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