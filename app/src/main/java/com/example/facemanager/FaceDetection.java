package com.example.facemanager;

import android.graphics.Bitmap;

public class FaceDetection {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public native void faceDetectionSaceInfo(Bitmap mFaceBitmap);
}
