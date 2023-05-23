package com.example.pinoroulette;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class MyCustomAdapter extends BaseAdapter implements ListAdapter {
    private ArrayList<String> list;
    private Context context;
    private Client client;

    public MyCustomAdapter(ArrayList<String> list, Context context, Client client) {
        this.list = list;
        this.context = context;
        this.client = client;
    }

    @Override
    public int getCount() {
        return list.size();
    }

    @Override
    public Object getItem(int pos) {
        return list.get(pos);
    }

    @Override
    public long getItemId(int pos) {
        return 0;
        //just return 0 if your list items do not have an Id variable.
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        View view = convertView;
        if (view == null) {
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflater.inflate(R.layout.xxx_custom_layout, null);
        }

        //Handle TextView and display string from your list
        TextView username= view.findViewById(R.id.txt_username);
        username.setText(list.get(position));

        //Handle buttons and add onClickListeners
        Button addBtn= view.findViewById(R.id.btn_add);
        addBtn.setOnClickListener(v -> {
            client.addMember(username.getText().toString());
        });

        Button delBtn = view.findViewById(R.id.btn_delete);
        delBtn.setOnClickListener( v -> {
            client.removeFromPendingList(username.getText().toString());
        } );

        return view;
    }
}
