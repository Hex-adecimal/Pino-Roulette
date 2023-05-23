package com.example.pinoroulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;

import java.util.ArrayList;

public class Group extends AppCompatActivity {

    ArrayList<BaseMessage> messages = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_group);

        // Display the back button on left up corner
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();
        client.getGroupMessages();



        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        messages = client.messages;

        ListView list_chat = findViewById(R.id.list_messages);
        MessageListAdapter arr = new MessageListAdapter(messages, this);
        list_chat.setAdapter(arr);

        Button btn_send = findViewById(R.id.btn_send);
        btn_send.setOnClickListener(view -> {
            EditText edt_actual_message = findViewById(R.id.edt_message);
            client.sendMessage(String.valueOf(edt_actual_message.getText()));
            edt_actual_message.setText("");
        });

    }

    // Actually go back
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            startActivity(new Intent(getApplicationContext(), Menu.class));
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}