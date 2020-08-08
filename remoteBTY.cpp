

//
#include "stdafx.h"
//#include<iostream>
//
//
//#include <string>
//#include <list>
//#include <vector>
//#include <map>
//#include <stack>
//
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include <opencv2/ximgproc.hpp>
//#include<ctime>





#include<opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <io.h>  
#include <iostream>
#include <vector>
#include<ctime>
#include <fstream>


#include <stdlib.h>
using namespace cv;
using namespace std;



int main()
{
	clock_t start;
	clock_t end;

 	string  doucments = "bigbuildingsTYP/AggFinall01.png";
 
	Mat frame, labels, frame2, lables2, frame3, lables3;

	Mat mask, mask2, mask3;

 	frame2 = cv::imread(doucments);
 
	int regionsize = 185; // 分割参数暂定180 和 150
 
	Ptr<cv::ximgproc::SuperpixelLSC> lsc = cv::ximgproc::createSuperpixelLSC(frame2, regionsize, 0.1);

	lsc->iterate();//迭代次数，默认为4
	lsc->enforceLabelConnectivity();
	lsc->getLabels(lables2);//获取labels
	lsc->getLabelContourMask(mask2);;//获取超像素的边界
	int number_lsc = lsc->getNumberOfSuperpixels();//获取超像素的数量

 



 	frame2.setTo(Scalar(255, 255, 255), mask2);
 

  
 
 
	imshow("test", frame2);
	imwrite("bigbuildingsTYP/lscsuperpixel01185.png", frame2);


  
	waitKey();

	return 0;
}






//
//
//
//int _tmain(int argc)
//{
//
//
//
//
//
//
//	string  doucments = "waterareas/tuan01.bmp";
//
//
//
//
//
//
//
//
//	clock_t start;
//	clock_t end;
//	Mat  labels;
//	Mat mask;
//
//	start = clock();//开始计时
//
//	cv::Mat frame = cv::imread(doucments);
//	Mat frameoriginal = cv::imread(doucments);
//	Mat frameout = cv::imread(doucments);
//	Mat overlay = cv::imread(doucments);
//
//	//边缘检测
//	Mat img31 = imread(doucments);
//	Mat DstPic1, edge1, grayImage1;
//
//	//创建与src同类型和同大小的矩阵
//	DstPic1.create(img31.size(), img31.type());
//
//	//将原始图转化为灰度图
//	cvtColor(img31, grayImage1, COLOR_BGR2GRAY);
//
//	//先使用3*3内核来降噪
//	blur(grayImage1, edge1, Size(3, 3));
//
//	//运行canny算子
//	Canny(edge1, edge1, 3, 9, 3);
//
//
//
//	//在得到的二值图像中寻找轮廓  
//	vector<vector<Point>> contours1;
//	vector<Vec4i> hierarchy1;
//	findContours(edge1, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	//绘制轮廓  
//	for (int i = 0; i < (int)contours1.size(); i++)
//	{
//		drawContours(edge1, contours1, i, Scalar(255), 1, 8);
//	}
//	imwrite("chulihou1.png", edge1);
//
//	//计算化简后轮廓的面积  
//	double g_dConArea1;
//
//
//
//	for (int i = 0; i < (int)contours1.size(); i++)
//	{
//		g_dConArea1 = contourArea(contours1[0], true);
//		//cout << "Object:" << i << "Area:" << g_dConArea1 << endl;
//	}
//
//
//
//	for (int row = 0; row < frameout.rows; row++)
//	{
//		for (int col = 0; col < frameout.cols; col++)
//		{
//			frameout.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
//		}
//	}
//
//
//	//初始聚类值   70  10  iterate30 0.1  800  5650
//	Ptr<cv::ximgproc::SuperpixelSEEDS> seeds = cv::ximgproc::createSuperpixelSEEDS(frame.cols, frame.rows, frame.channels(), 150, 15, 2, 5, true);
//
//	seeds->iterate(frame, 4);//迭代次数，默认为4  371/597
//	seeds->getLabels(labels);//获取labels
//	seeds->getLabelContourMask(mask);;//获取超像素的边界
//	int number_seeds = seeds->getNumberOfSuperpixels();//获取超像素的数量
//
//
//	cout << "number_seeds:" << number_seeds << endl;
//
//
//
//	for (int lab = 0; lab < number_seeds; lab++)
//	{
//		int numlabs = 0;//获取超像素标签为lab的总数量
//		for (int row = 0; row < labels.rows; row++)
//		{
//			for (int col = 0; col < labels.cols; col++)
//			{
//
//				int currentlabel = labels.at<int>(row, col);
//
//				if (currentlabel == lab){//获取超像素标签为lab的超像素集团	
//
//					numlabs++;
//
//				}
//
//			}
//		}
//
//		//查找行列数中为原始色彩的像素数量
//		int numOriginal = 0;
//		for (int row = 0; row < labels.rows; row++)
//		{
//			for (int col = 0; col < labels.cols; col++)
//			{
//
//				int currentlabel = labels.at<int>(row, col);
//
//				if (currentlabel == lab){//获取超像素标签为lab的超像素集团	
//
//					if (frameoriginal.at<Vec3b>(row, col)[2] == 255 && frameoriginal.at<Vec3b>(row, col)[1] == 241 && frameoriginal.at<Vec3b>(row, col)[0] == 0){
//
//						numOriginal++;
//
//					}
//
//				}
//
//			}
//		}
//
//		//求单个超像素内原始与化简后的比值
//
//
//		float ratio = 0;
//		ratio = (float)numOriginal / (float)numlabs;
//
//		ratio = (float)numOriginal / (float)numlabs;
//
//
//		//若果ratio大于0.5， 则对该所有标签的老图上色
//		float sizeratio = 0;
//
//
//
//		if (ratio > sizeratio)
//		{
//			//cout << "ratio:" << ratio << endl;
//			cout << "lab:" << lab << endl;
//
//
//			for (int row = 0; row < labels.rows; row++)
//			{
//				for (int col = 0; col < labels.cols; col++)
//				{
//
//					int currentlabel = labels.at<int>(row, col);
//
//					if (currentlabel == lab){//获取超像素标签为lab的超像素集团	
//
//						frameoriginal.at<Vec3b>(row, col) = Vec3b(0, 0, 255);
//						frameout.at<Vec3b>(row, col) = Vec3b(0, 0, 255);
//					}
//
//				}
//			}
//		}
//
//	}
//
//
//	frame.setTo(Scalar(255, 0, 255), mask);
//	end = clock();//结束计时
//	cout << "Time:" << end - start << endl;
//
//	imshow("Superpixel Segmentation", frame);
//	//imshow("test2", frameoriginal);
//
//	cv::imwrite("waterareas/demoout.png", frame);
//	cv::imwrite("waterareas/overlay.png", frameoriginal);
//	cv::imwrite("waterareas/result.bmp", frameout);
//}