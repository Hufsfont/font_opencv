package com.example.font_opencv;

import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class activity_sub_05 extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sub_05);

        Button button = findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });

        ImageButton image_button = findViewById(R.id.imageButton2);
        image_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Toast.makeText(activity_sub_05.this, "폰트가 저장되었습니다.", Toast.LENGTH_SHORT).show();
            }
        });
    }
}

