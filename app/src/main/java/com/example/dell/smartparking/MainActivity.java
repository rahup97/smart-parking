package com.example.dell.smartparking;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    Button Login, Register;
    EditText Name;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Login = (Button)findViewById(R.id.Login);
        Register = (Button)findViewById(R.id.Register);
        Name = (EditText)findViewById(R.id.Name);
    }

    public void onLogin(View view) {
        if(view.getId()==R.id.Login){
            String str = Name.getText().toString();
            Intent i =new Intent(MainActivity.this,ParkingSlot.class);
            i.putExtra("Username",str);
            startActivity(i);
        }
    }

    public void onRegister(View view) {
        if(view.getId()==R.id.Register){
            Toast.makeText(this, "Use Login For Now:>", Toast.LENGTH_SHORT).show();
        }
    }
}
