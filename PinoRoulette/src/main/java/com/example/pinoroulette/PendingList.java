package com.example.pinoroulette;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.Arrays;

public class PendingList extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pending_list);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        Client client = Client.getInstance();

        ArrayList<String> members = client.getPendingList();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        ListView listView = findViewById(R.id.listview);
        listView.setAdapter(new MyCustomAdapter(members, this, client) );

        /*
        LinearLayout pop_up = findViewById(R.id.pop_up);


        // Display groups
        ListView list_chat = findViewById(R.id.pending_list);
        ArrayAdapter<String> arr = new ArrayAdapter<>(this, R.layout.group, members);
        list_chat.setAdapter(arr);

        list_chat.setOnItemClickListener((adapterView, view, i, l) -> {
            pop_up.setVisibility(View.VISIBLE);
            Button yes =findViewById(R.id.btn_yes);
            Button no = findViewById(R.id.btn_no);

            yes.setOnClickListener(view1 -> {
                String subject = members.get(i);
                client.add_Member(subject);
                pop_up.setVisibility(View.INVISIBLE);


                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // non so se serve farlo



                if(client.get_last_transaction()){
                    Toast.makeText(getApplicationContext(), "Selected member added", Toast.LENGTH_LONG).show();
                    startActivity(new Intent(getApplicationContext(),PendingList.class));
                }
                else{
                    Toast.makeText(getApplicationContext(), "Could not add selected member", Toast.LENGTH_LONG).show();
                }
            });
            no.setOnClickListener(view1-> {
                String subject = members.get(i);
                client.remove_from_Pending_List(subject);
                pop_up.setVisibility(View.INVISIBLE);

                try {
                    Thread.sleep(400);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // non so se serve farlo



                if(client.get_last_transaction()){
                    Toast.makeText(getApplicationContext(), "Selected member removed", Toast.LENGTH_LONG).show();
                    startActivity(new Intent(getApplicationContext(),PendingList.class));
                }
                else{
                    Toast.makeText(getApplicationContext(), "Could not remove selected member", Toast.LENGTH_LONG).show();
                }
            });
        });
         */
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            startActivity(new Intent(getApplicationContext(), MyGroups.class));
            // TODO : delete username password
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
