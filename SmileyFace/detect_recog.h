#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
//////////////////////////////////s///////////////////////////////////
#include <opencv2\contrib\contrib.hpp>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp> 
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace cv;

#ifndef DETECT_RECOG_H
#define DETECT_RECOG_H
//test

//extern CvMemStorage* storage = 0;;
//extern CvHaarClassifierCascade* cascade = NULL;
//extern CvHaarClassifierCascade* nested_cascade = NULL;
//extern int use_nested_cascade;
//extern const char* cascade_name;
//extern const char* nested_cascade_name;
extern double scale;
//extern IplImage *resizeRes = NULL;//存放检测到的人脸
//extern IplImage *faceGray = NULL;//存放检测到的人脸
extern cv::Ptr<cv::FaceRecognizer> model;
//extern vector<Mat> images;
//extern vector<int> labels;
extern double dConfidence;
extern int predictedLabel;

void detect_and_draw( IplImage* img ); // 检测和绘画 
//void recog_and_draw( IplImage* img ); // 检测和绘画 
bool recog_and_draw_new( IplImage* img ,CascadeClassifier& face_cascade ,int& takeSample,int& sampleNum); // 新识别函数
void read_csv(const string &filename, vector<Mat> &images, vector<int> &labels, char separator = ';');
bool read_img(vector<Mat> &images, vector<int> &labels);
int read_img_number();
bool delete_img();
Mat norm_0_255(cv::InputArray _src);
void cvText(IplImage* img, const char* text, int x, int y);
#endif