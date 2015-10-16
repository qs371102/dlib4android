package com.example.dlib4android;

import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;

import android.util.Log;

public class dlibDetect {
	
	static {
        System.loadLibrary("dlib4Android");
    }
	
	private long mNativeObj = 0;
	
	public void detect(Mat imageGray, MatOfRect faces) {
		faceDetect(mNativeObj, imageGray.getNativeObjAddr(), faces.getNativeObjAddr());
    }
	public void handle(){
		Log.d("dlib::Activity", "=====handleImage=====");
		handleImage();
	}
	
	public void hello()
	{
		Log.d("dlib::Activity","dlibDetect");
		helloWorld();
	}
	private static native void faceDetect(long thiz, long inputImage, long faces);
	
	private static native void handleImage();
	
	private static native void helloWorld();

}
