package com.example.facemanager;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {




    private ImageView mFaceImage;
    private Bitmap mFaceBitmap;
    private FaceDetection mFaceDetection;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mFaceImage = findViewById(R.id.face_image);
        mFaceBitmap= BitmapFactory.decodeResource(getResources(), R.drawable.face);
        mFaceImage.setImageBitmap(mFaceBitmap );

        mFaceDetection=new FaceDetection();

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public void faceDetection(View view) {
        //识别人脸  保存特征信息

        mFaceDetection.faceDetectionSaceInfo(mFaceBitmap);
    }
}
