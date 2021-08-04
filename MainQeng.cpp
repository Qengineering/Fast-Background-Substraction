#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

void ShowManyImages(string title, int nArgs, ...);

int main(int argc, char* argv[])
{
    float f, Pwhite;
    float FPS[16];
    size_t i,n;
    float Sum;
    int Xcnt,Tcnt,Fcnt=0;
    float Tau=0.05;

    chrono::steady_clock::time_point Tbegin, Tend;

    Mat frame, bw_frame, sum_frame;
    Mat sub_frame, finalImage, fImage;

    for(i=0;i<16;i++) FPS[i]=0.0;

    f=1.0/Tau; Tcnt=f; Xcnt=f;

    VideoCapture capture( "768x576.avi");
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open stream" << endl;
        return 0;
    }
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //make a summation frame when not init before
        if(sum_frame.empty()){
            sum_frame = Mat(frame.rows, frame.cols, CV_32FC1);
        }

        Tbegin = chrono::steady_clock::now();

		//get black/white
		cv::cvtColor(frame, bw_frame, COLOR_BGR2GRAY);
        //go to float
        bw_frame.convertTo(fImage, CV_32FC1);
        //calculate Tau
        f=Tcnt; f/=Xcnt;
        Tcnt--; if(Tcnt<1) Tcnt=1;
        //weighted background
        cv::addWeighted(fImage, f, sum_frame, (1.0-f), 0.0, sum_frame);
        //substrac current image from static background
        cv::subtract(sum_frame,fImage,sub_frame);
        //get a 8 bit black and white image
        cv::convertScaleAbs(sub_frame,finalImage,1.5);

        Tend = chrono::steady_clock::now();

        //get number of light pixels
        n=finalImage.total();
        for(Sum=0,i=0;i<n;i++){
            if(finalImage.data[i]>30) Sum++;
        }
        Pwhite=Sum*100.0; Pwhite/=n;

        //calculate inference time
        f = chrono::duration_cast <chrono::milliseconds> (Tend - Tbegin).count();

        FPS[((Fcnt++)&0x0F)]=f;
        for(f=0.0, i=0;i<16;i++){ f+=FPS[i]; }
        cout << Pwhite << " % - " << f/16 << " mSec" << endl;

        // show if you like
        putText(finalImage, format("%0.2f %%",Pwhite),Point(10,20),FONT_HERSHEY_SIMPLEX,0.6, Scalar(255, 255, 255));
        putText(finalImage, format("%0.2f mSec",f/16),Point(10,50),FONT_HERSHEY_SIMPLEX,0.6, Scalar(255, 255, 255));

        imshow("Frame", frame);
        imshow("Result", finalImage);
        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    return 0;
}
