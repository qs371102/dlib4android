#include"com_example_dlib4android_dlibDetect.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/shape_predictor.h>
//#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_loader/load_image.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace dlib;
using namespace std;

#include <string>
#include <ctime>
#include <vector>
#include <android/log.h>

#define LOG_TAG "dlibTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

//inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat)
//{
//	inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat)
//	{
//	    mat = Mat(v_rect, true);
//	}
//	LOGD("Hello !");
//}
static shape_predictor sp;
static bool hasIni=false;
static frontal_face_detector detector;
cv::Scalar color(110, 220, 0 );

void initalShapePredictor()
{
	detector = get_frontal_face_detector();
	deserialize("/sdcard/shape_predictor_68_face_landmarks.dat") >> sp;
}

void getFaces()
{
	try
	    {
			LOGD("VideoCapture");
	        cv::VideoCapture cap(0);
	        //image_window win;
	        // Load face detection and pose estimation models.
	        if(!hasIni)
	        {
	        	initalShapePredictor();
	        	hasIni=true;
	        }else
	        {
	        	LOGD("Has Ini");
	        }
	        // Grab and process frames until the main window is closed by the user.
	        while(cap.isOpened())
	        {
	        	LOGD("cap.isOpened()");
	            // Grab a frame
	            cv::Mat temp;
	            cap >> temp;
	            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
	            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
	            // long as temp is valid.  Also don't do anything to temp that would cause it
	            // to reallocate the memory which stores the image as that will make cimg
	            // contain dangling pointers.  This basically means you shouldn't modify temp
	            // while using cimg.
	            cv_image<bgr_pixel> cimg(temp);

	            // Detect faces
	            std::vector<rectangle> faces = detector(cimg);
	            // Find the pose of each face.
	            std::vector<full_object_detection> shapes;
	            LOGD("faces numb:%lu",faces.size());
	            for (unsigned long i = 0; i < faces.size(); ++i)
	            {
	            	full_object_detection shape = sp(cimg, faces[i]);
	                cout << "number of parts: "<< sp.num_parts() << endl;
	                cout << "pixel position of first part:  " << shape.part(0)<< endl;
	                cout << "pixel position of second part: " << shape.part(1)<< endl;
	                shapes.push_back(shape);
	            }
	        }
	    }
	    catch(serialization_error& e)
	    {
	        cout << "You need dlib's default face landmarking model file to run this example." << endl;
	        cout << "You can get it from the following URL: " << endl;
	        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
	        cout << endl << e.what() << endl;
	        LOGD("%c",e.what());
	    }
	    catch(exception& e)
	    {
	        cout << e.what() << endl;
	    }
}


void lenaImage()
{
	LOGD("lenaImage");
	try{
		if(!hasIni)
		{
			initalShapePredictor();
			hasIni=true;
		}
	    std::string path = "/sdcard/lena.png";
	    cv::Mat inputImage = cv::imread(path);
	    dlib::cv_image<dlib::rgb_pixel> img(inputImage);
	    std::vector<rectangle> dets = detector(img);
	    std::vector<full_object_detection> shapes;
	    LOGD("faces numb:%lu",dets.size());
	    for (unsigned long j = 0; j < dets.size(); ++j)
	    {
	         full_object_detection shape = sp(img, dets[j]);
	         LOGD("j: %lu",j);
	         LOGD("number of parts: %lu",shape.num_parts());
	         for(unsigned long i=0;i<shape.num_parts();i++)
	         {
	        	 LOGD("pixel position of %lu part: X %ld Y %ld",i,shape.part(i).x(),shape.part(i).y());
	         }
	         shapes.push_back(shape);
	    }
	}
	catch (exception& e)
	{
	    cout << "\nexception thrown!" << endl;
	    cout << e.what() << endl;
	}
}

void imageProcess(cv::Mat image,cv::Mat& finalImage)
{
	LOGD("imageProcess");
	try
	    {
	        // TODO: Replace load_image with opencv
			if(!hasIni)
			{
				LOGD("has Ini");
				initalShapePredictor();
				hasIni=true;
			}
			cv_image<rgb_pixel> img(image);
	        const clock_t begin_time = clock();
	        std::vector<rectangle> dets = detector(img);
	        std::vector<full_object_detection> shapes;
	        LOGD("faces numb:%lu",dets.size());
			for (unsigned long j = 0; j < dets.size(); ++j)
			{
				full_object_detection shape = sp(img, dets[j]);
				for (unsigned long i = 1; i <= 16; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				for (unsigned long i = 28; i <= 30; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);

				for (unsigned long i = 18; i <= 21; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				for (unsigned long i = 23; i <= 26; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				for (unsigned long i = 31; i <= 35; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				cv::line(finalImage,cv::Point( shape.part(30).x(),shape.part(30).y()), cv::Point( shape.part(35).x(),shape.part(35).y()), color);
				for (unsigned long i = 37; i <= 41; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				cv::line(finalImage,cv::Point( shape.part(36).x(),shape.part(36).y()), cv::Point( shape.part(41).x(),shape.part(41).y()), color);

				for (unsigned long i = 43; i <= 47; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				cv::line(finalImage,cv::Point( shape.part(42).x(),shape.part(42).y()), cv::Point( shape.part(47).x(),shape.part(47).y()), color);

				for (unsigned long i = 49; i <= 59; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color);
				cv::line(finalImage,cv::Point( shape.part(48).x(),shape.part(48).y()), cv::Point( shape.part(59).x(),shape.part(59).y()), color);

				for (unsigned long i = 61; i <= 67; ++i)
					cv::line(finalImage,cv::Point( shape.part(i).x(),shape.part(i).y()), cv::Point( shape.part(i-1).x(),shape.part(i-1).y()), color );
				cv::line(finalImage,cv::Point( shape.part(60).x(),shape.part(60).y()), cv::Point( shape.part(67).x(),shape.part(67).y()), color);
				shapes.push_back(shape);
				//image=matrix_op<op_array2d_to_mat< cv_image<rgb_pixel>> >(img);
			}
	        const clock_t end_time = clock();
	        LOGD("%f",end_time);
	    }
	    catch (exception& e)
	    {
	    	LOGD("%s",e.what());
	    }
}

JNIEXPORT void JNICALL Java_com_example_dlib4android_dlibDetect_faceDetect
  (JNIEnv * jenv, jclass, jlong thiz, jlong imageRgba, jlong faces)
{
	LOGD("dlibDetect_faceDetect Start");
	try{
		cv::Mat* rgba = (cv::Mat*) imageRgba;
		cv::Mat rgb;
		cv::cvtColor(*rgba,rgb,CV_RGBA2RGB);
		if(rgba == 0) {
		  LOGD("Null matrix");
		}
		else {
		  LOGD("The matrix is not null. It has %i rows and %i columns", (*rgba).rows, (*rgba).cols);
		  LOGD("depth:",(*rgba).depth());
		}
		imageProcess(rgb,*rgba);
	}
	catch(...)
	{
		LOGD("dlibDetect caught unknown exception");
	}
	LOGD("dlibDetect_faceDetect End");
}

JNIEXPORT void JNICALL Java_com_example_dlib4android_dlibDetect_handleImage
  (JNIEnv *, jclass){
	//lenaImage();
	try{
		//getFaces();
	}
	catch(...)
	{
		LOGD("Get Faces error");
	}
}

JNIEXPORT void JNICALL Java_com_example_dlib4android_dlibDetect_helloWorld
  (JNIEnv *, jclass)
{
	LOGD("hello");
}
