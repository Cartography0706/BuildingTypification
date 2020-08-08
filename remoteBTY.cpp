

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
 
	int regionsize = 185; // �ָ�����ݶ�180 �� 150
 
	Ptr<cv::ximgproc::SuperpixelLSC> lsc = cv::ximgproc::createSuperpixelLSC(frame2, regionsize, 0.1);

	lsc->iterate();//����������Ĭ��Ϊ4
	lsc->enforceLabelConnectivity();
	lsc->getLabels(lables2);//��ȡlabels
	lsc->getLabelContourMask(mask2);;//��ȡ�����صı߽�
	int number_lsc = lsc->getNumberOfSuperpixels();//��ȡ�����ص�����

 



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
//	start = clock();//��ʼ��ʱ
//
//	cv::Mat frame = cv::imread(doucments);
//	Mat frameoriginal = cv::imread(doucments);
//	Mat frameout = cv::imread(doucments);
//	Mat overlay = cv::imread(doucments);
//
//	//��Ե���
//	Mat img31 = imread(doucments);
//	Mat DstPic1, edge1, grayImage1;
//
//	//������srcͬ���ͺ�ͬ��С�ľ���
//	DstPic1.create(img31.size(), img31.type());
//
//	//��ԭʼͼת��Ϊ�Ҷ�ͼ
//	cvtColor(img31, grayImage1, COLOR_BGR2GRAY);
//
//	//��ʹ��3*3�ں�������
//	blur(grayImage1, edge1, Size(3, 3));
//
//	//����canny����
//	Canny(edge1, edge1, 3, 9, 3);
//
//
//
//	//�ڵõ��Ķ�ֵͼ����Ѱ������  
//	vector<vector<Point>> contours1;
//	vector<Vec4i> hierarchy1;
//	findContours(edge1, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	//��������  
//	for (int i = 0; i < (int)contours1.size(); i++)
//	{
//		drawContours(edge1, contours1, i, Scalar(255), 1, 8);
//	}
//	imwrite("chulihou1.png", edge1);
//
//	//���㻯������������  
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
//	//��ʼ����ֵ   70  10  iterate30 0.1  800  5650
//	Ptr<cv::ximgproc::SuperpixelSEEDS> seeds = cv::ximgproc::createSuperpixelSEEDS(frame.cols, frame.rows, frame.channels(), 150, 15, 2, 5, true);
//
//	seeds->iterate(frame, 4);//����������Ĭ��Ϊ4  371/597
//	seeds->getLabels(labels);//��ȡlabels
//	seeds->getLabelContourMask(mask);;//��ȡ�����صı߽�
//	int number_seeds = seeds->getNumberOfSuperpixels();//��ȡ�����ص�����
//
//
//	cout << "number_seeds:" << number_seeds << endl;
//
//
//
//	for (int lab = 0; lab < number_seeds; lab++)
//	{
//		int numlabs = 0;//��ȡ�����ر�ǩΪlab��������
//		for (int row = 0; row < labels.rows; row++)
//		{
//			for (int col = 0; col < labels.cols; col++)
//			{
//
//				int currentlabel = labels.at<int>(row, col);
//
//				if (currentlabel == lab){//��ȡ�����ر�ǩΪlab�ĳ����ؼ���	
//
//					numlabs++;
//
//				}
//
//			}
//		}
//
//		//������������Ϊԭʼɫ�ʵ���������
//		int numOriginal = 0;
//		for (int row = 0; row < labels.rows; row++)
//		{
//			for (int col = 0; col < labels.cols; col++)
//			{
//
//				int currentlabel = labels.at<int>(row, col);
//
//				if (currentlabel == lab){//��ȡ�����ر�ǩΪlab�ĳ����ؼ���	
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
//		//�󵥸���������ԭʼ�뻯���ı�ֵ
//
//
//		float ratio = 0;
//		ratio = (float)numOriginal / (float)numlabs;
//
//		ratio = (float)numOriginal / (float)numlabs;
//
//
//		//����ratio����0.5�� ��Ը����б�ǩ����ͼ��ɫ
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
//					if (currentlabel == lab){//��ȡ�����ر�ǩΪlab�ĳ����ؼ���	
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
//	end = clock();//������ʱ
//	cout << "Time:" << end - start << endl;
//
//	imshow("Superpixel Segmentation", frame);
//	//imshow("test2", frameoriginal);
//
//	cv::imwrite("waterareas/demoout.png", frame);
//	cv::imwrite("waterareas/overlay.png", frameoriginal);
//	cv::imwrite("waterareas/result.bmp", frameout);
//}