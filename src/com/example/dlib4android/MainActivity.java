package com.example.dlib4android;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;

import android.app.Activity;

import android.os.Bundle;
import android.util.Log;

import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
//import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
//import android.widget.Button;

public class MainActivity extends Activity implements CvCameraViewListener2 {

	private static final String    TAG                 = "dlib::Activity";
	private CameraBridgeViewBase  mOpenCvCameraView;
	private Mat                   mRgba;
    private Mat                   mGray;
    private dlibDetect            mDlibDetect;
    private boolean 			  mHandleVideo=false;
    private boolean 			  mHandleOnce=true;	
    
   // private GestureDetector detector = new GestureDetector((android.view.GestureDetector.OnGestureListener) this);
    
    
    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };
    
    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
    }
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		//super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_main);
		Log.i(TAG, "called onCreate");
	    super.onCreate(savedInstanceState);
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	    setContentView(R.layout.activity_main);
	    mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.HelloOpenCvView);
	    mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
	    mOpenCvCameraView.setCvCameraViewListener(this);
	    
	    mOpenCvCameraView.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				mHandleVideo=true;
				mHandleOnce=true;
				Log.i(TAG, "Touch---------");
				mDlibDetect.handle();
				return false;
			}
		});
	    
	    mDlibDetect=new dlibDetect();
//	    Button btn=(Button)findViewById(R.id.btn);
//	    btn.setOnClickListener(new Button.OnClickListener(){
//
//			@Override
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				mDlibDetect.handle();
//				Log.i(TAG, "handle");
//			}});
//	    //mOpenCvCameraView.enableView();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	
	@Override
	 public void onPause()
	 {
	     super.onPause();
	     if (mOpenCvCameraView != null)
	         mOpenCvCameraView.disableView();
	 }
	@Override
	 public void onDestroy() {
	     super.onDestroy();
	     if (mOpenCvCameraView != null)
	         mOpenCvCameraView.disableView();
	 }
	 @Override
	 public void onCameraViewStarted(int width, int height) {
	 }
	 @Override
	 public void onCameraViewStopped() {
	 }
	 @Override
	 public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		 mRgba = inputFrame.rgba();
		 
		 
	     mGray = inputFrame.gray();
	     
	     MatOfRect faces = new MatOfRect();
	     
	     if (mDlibDetect != null&& mHandleVideo)
	    	 if(mHandleOnce||true)
	    	 {
	    		 Log.d(TAG, "detect============");
	    		 mHandleOnce=false;
	    		 mDlibDetect.detect(mRgba, faces);
	    	 }
	     return inputFrame.rgba();
	 }
}
