# Fast Background Substraction


https://user-images.githubusercontent.com/44409029/128219655-52ff1c72-8ad9-4323-a0a4-0eba52268144.mp4



## A fast background substraction in C++

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)<br/>

This C++ application filters the background from a static image. It's written for a Raspberry Pi but can be used on any machine with OpenCV. It's a fast algorithm to detect moving objects in an image given a static background. The available OpenCV routines were too slow for our Raspberry Pi Zero, hence this faster solution.<br/>
The given times are with a Raspberry Pi 4 with a 32-bit OS, running at 1500 MHz.

------------

## Dependencies.
To run the application, you have to:
- OpenCV installed. [Install OpenCV 4.5](https://qengineering.eu/install-opencv-4.5-on-raspberry-64-os.html) <br/>
- Code::Blocks installed. (`$ sudo apt-get install codeblocks`)

------------

## Installing the app.
To extract and run the application in Code::Blocks <br/>
$ mkdir *MyDir* <br/>
$ cd *MyDir* <br/>
$ wget https://github.com/Qengineering/Fast-Background-Substraction/archive/refs/heads/main.zip <br/>
$ unzip -j master.zip <br/>
Remove master.zip, LICENSE and README.md as they are no longer needed. <br/> 
$ rm master.zip <br/>
$ rm LICENSE <br/>
$ rm README.md <br/> <br/>
Your *MyDir* folder must now look like this: <br/> 
768x576.avi (example movie)<br/>
Background.cbp (code::blocks project file) <br/>
MainQeng.cpp (C++ source file with the new algorithm) <br/>
MainMOG2.cpp (C++ source file with exisiting OpenCV algorithms)

------------

## Running the app.
To run the application load the project file Background.cbp in Code::Blocks.<br/> 
You can load and run either MainQeng.cpp or MainMOG2.cpp in the IDE.<br/>
The code speaks for itself. Here the bare bones.<br/>
### Qeng
```
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

```
### MOG2
```
  //create Background Subtractor objects
  Ptr<BackgroundSubtractor> pBackSub;
  pBackSub = createBackgroundSubtractorMOG2();
....
  //update the background model
  pBackSub->apply(frame, finalImage);
```
### KNN
```
  //create Background Subtractor objects
  Ptr<BackgroundSubtractor> pBackSub;
  pBackSub = createBackgroundSubtractorKNN();
....
  //update the background model
  pBackSub->apply(frame, finalImage);
```

------------

## Tau
Our algorithm adapts an average background over time. Every single frame updates this `sum_frame` a little bit. The adaptation is needed for slowly changing (lighting) conditions like moving shadows. It will also remove static objects from the scene. A parked car will vanish after some time in the background. Until, of course, it drives away. The constant Tau defines the adaption rate. The higher this number, the faster something disappears in the background.<br/><br/>
Rule of the thumb: **T = 5/(Tau * FPS)**<br/>
Given an FPS (Frames Per Second) of 30 and a Tau of 0.05, an object will completely vanish in the background in 3.3 Sec.<br/><br/>
The given percentage is the ratio of white pixels, in other words, the moving object.<br/>
Because the algorithm works with grey values, it is sensitive to contrast. A person wearing a green sweater on the green grass will give a not that high response as the same person walking on the street. However, we gladly pay this small price for the speed the routine achieves.

------------


## RaspiCam.
If you want to use a camera please alter line 253 in main.cpp to<br/>
`cv::VideoCapture cap(0);                //RaspiCam`<br/>
If you want to run a movie please alter line 253 in main.cpp to<br/>
`cv::VideoCapture cap( "768x576.avi");   //Movie`<br/>

------------

## Papers.
- Zoran Zivkovic and Ferdinand van der Heijden. Efficient adaptive density estimation per image pixel for the task of background subtraction. Pattern recognition letters, 27(7):773–780, 2006. [link](https://www.academia.edu/33302633/Efficient_adaptive_density_estimation_per_image_pixel_for_the_task_of_background_subtraction)<br/>
- Zoran Zivkovic. Improved adaptive gaussian mixture model for background subtraction. In Pattern Recognition, 2004. ICPR 2004. Proceedings of the 17th International Conference on, volume 2, pages 28–31. IEEE, 2004. [link](https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.1.4658)<br/>

------------

[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 

