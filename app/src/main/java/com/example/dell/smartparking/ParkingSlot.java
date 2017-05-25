package com.example.dell.smartparking;

import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.widget.Button;
import android.widget.Toast;
import android.view.View;
import android.view.View.OnClickListener;

import java.io.IOException;
import java.util.Set;
import java.util.UUID;

public class ParkingSlot extends Activity {

    boolean flag[]={true,true,true};
    Button btn[] = new Button[3];
    FloatingActionButton fab;
    BluetoothAdapter adapter;
    BluetoothDevice mDevice;
    BluetoothSocket btSocket = null;
    boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.parkingslot);

        adapter = BluetoothAdapter.getDefaultAdapter();
        if(adapter==null) {
            Toast.makeText(getBaseContext(), "Device Does Not Support Bluetooth", Toast.LENGTH_SHORT).show();
            finish();
        }

        if(!adapter.isEnabled()){
            Intent enableBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBT,1);
        }

        Set<BluetoothDevice> paired = adapter.getBondedDevices();
        if(paired.size()>0){
            for (BluetoothDevice device : paired ){
                mDevice = device;
            }
        }
        new ConnectBT().execute();


        String name = getIntent().getStringExtra("Username");
        Toast.makeText(getApplicationContext(),"Welcome "+name, Toast.LENGTH_SHORT).show();

        btn[0] = (Button)findViewById(R.id.Button1);
        btn[1] = (Button)findViewById(R.id.Button2);
        btn[2] = (Button)findViewById(R.id.Button3);
        fab = (FloatingActionButton)findViewById(R.id.Fab);

        btn[0].setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View view) {
                if(flag[0])
                    showDialog(view,0);
                else
                    Toast.makeText(getApplicationContext(), "SLOT 1 is reserved already!", Toast.LENGTH_SHORT).show();
            }
        });

        btn[1].setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View view) {
                if(flag[1])
                    showDialog(view,1);
                else
                    Toast.makeText(getApplicationContext(), "SLOT 2 is reserved already!", Toast.LENGTH_SHORT).show();
            }
        });

        btn[2].setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View view) {
                if(flag[2])
                    showDialog(view,2);
                else
                    Toast.makeText(getApplicationContext(), "SLOT 3 is reserved already!", Toast.LENGTH_SHORT).show();
            }
        });

    }

    private void turnOnLed(String str)
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write(str.getBytes());
            }
            catch (IOException e)
            {
                Toast.makeText(this, "Error", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void showDialog(View v, final int num){
        AlertDialog.Builder alertDialog = new AlertDialog.Builder(ParkingSlot.this);

        alertDialog.setTitle("Booking Confirmation");
        alertDialog.setMessage("Want to go ahead with the booking?");

        alertDialog.setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                Toast.makeText(getApplicationContext(),"SLOT " + (num+1) + " BOOKED", Toast.LENGTH_SHORT).show();
                flag[num] = false;
                btn[num].setBackgroundColor(Color.RED);
                String s = "" + (num+1);
                turnOnLed(s);
            }
        });

        alertDialog.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //do nothing
            }
        });

        alertDialog.show();
    }

    public void onFab(View view) {
        if(view.getId()==R.id.Fab){
            Intent i = new Intent(ParkingSlot.this,Fare.class);
            startActivity(i);
        }
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            //do nothing
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    btSocket = mDevice.createInsecureRfcommSocketToServiceRecord(myUUID);//create a (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                //do nothing
            }
            else
            {
                isBtConnected = true;
            }
        }
    }
}
