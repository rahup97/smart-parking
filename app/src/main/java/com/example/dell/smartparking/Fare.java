package com.example.dell.smartparking;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

public class Fare extends Activity {
    Spinner spinner;
    EditText duration;
    TextView answer;
    Button calculate;
    int rate=1;

    ArrayAdapter<CharSequence> adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fare);

        calculate = (Button)findViewById(R.id.Calculate);
        duration = (EditText)findViewById(R.id.Duration);
        answer = (TextView)findViewById(R.id.Answer);
        spinner = (Spinner)findViewById(R.id.spinner);
        adapter = ArrayAdapter.createFromResource(this,R.array.cars,android.R.layout.simple_spinner_item);

        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                Toast.makeText(getBaseContext(),adapterView.getItemAtPosition(i)+" is Selected",Toast.LENGTH_SHORT).show();
                rate = i+1;
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        calculate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(duration.length()==0)
                    Toast.makeText(getBaseContext(), "Please Enter Duration Field", Toast.LENGTH_SHORT).show();
                else {
                    float dur = Float.parseFloat(duration.getText().toString());
                    float amt = rate * dur * 150;
                    answer.setText(getString(R.string.rate_is) + amt + " rupees");
                }
            }
        });
    }
}
