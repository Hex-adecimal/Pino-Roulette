package com.example.pinoroulette;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;


import java.util.ArrayList;
import java.util.List;

public class MessageListAdapter extends ArrayAdapter<BaseMessage> {
    private Context mContext;
    private List<BaseMessage> mMessageList;
    Client client = Client.getInstance();


    // View lookup cache
    private static class ViewHolder {
        TextView actual_message;
        TextView sender;
        TextView sent_time;
    }

    public MessageListAdapter(ArrayList<BaseMessage> data, Context context) {
        super(context, R.layout.message, data);
        this.mContext=context;
        this.mMessageList = data;
    }


    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // Get the data item for this position
        BaseMessage dataModel = getItem(position);
        // Check if an existing view is being reused, otherwise inflate the view
        ViewHolder viewHolder; // view lookup cache stored in tag

        final View result;

        if (convertView == null) {

            viewHolder = new ViewHolder();
            LayoutInflater inflater = LayoutInflater.from(getContext());
            convertView = inflater.inflate(R.layout.message, parent, false);
            viewHolder.actual_message = (TextView) convertView.findViewById(R.id.txt_actual_message);
            viewHolder.sender = (TextView) convertView.findViewById(R.id.txt_sender);
            viewHolder.sent_time = (TextView) convertView.findViewById(R.id.txt_sent_time);

            result=convertView;

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
            result=convertView;
        }

        //int lastPosition=-1;
        //Animation animation = AnimationUtils.loadAnimation(mContext, (position > lastPosition) ? R.anim.up_from_bottom : R.anim.down_from_top);
        //result.startAnimation(animation);
        //lastPosition = position;

        viewHolder.sender.setText(dataModel.getSender());
        viewHolder.actual_message.setText(dataModel.getMessage());
        viewHolder.sent_time.setText(dataModel.getCreatedAt());

        // Return the completed view to render on screen
        return convertView;
    }

}