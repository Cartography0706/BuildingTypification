// ConsoleApplication2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<iostream>


#include <string>
#include <list>
#include <vector>
#include <map>
#include <stack>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/ximgproc.hpp>
#include<ctime>

using namespace std;

using namespace cv;



void Seed_Filling(const cv::Mat& binImg, cv::Mat& lableImg)   //������䷨
{
	// 4�ڽӷ���


	if (binImg.empty() ||
		binImg.type() != CV_8UC1)
	{
		return;
	}

	lableImg.release();
	binImg.convertTo(lableImg, CV_32SC1);

	int label = 1;

	int rows = binImg.rows - 1;
	int cols = binImg.cols - 1;
	for (int i = 1; i < rows - 1; i++)
	{
		int* data = lableImg.ptr<int>(i);
		for (int j = 1; j < cols - 1; j++)
		{
			if (data[j] == 1)
			{
				std::stack<std::pair<int, int>> neighborPixels;
				neighborPixels.push(std::pair<int, int>(i, j));     // ����λ��: <i,j>
				++label;  // û���ظ����ţ���ʼ�µı�ǩ
				while (!neighborPixels.empty())
				{
					std::pair<int, int> curPixel = neighborPixels.top(); //�������һ����һ�������غ���������һ�е��Ǹ��ŵı�Ÿ�����
					int curX = curPixel.first;
					int curY = curPixel.second;
					lableImg.at<int>(curX, curY) = label;

					neighborPixels.pop();

					if (lableImg.at<int>(curX, curY - 1) == 1)
					{//���
						neighborPixels.push(std::pair<int, int>(curX, curY - 1));
					}
					if (lableImg.at<int>(curX, curY + 1) == 1)
					{// �ұ�
						neighborPixels.push(std::pair<int, int>(curX, curY + 1));
					}
					if (lableImg.at<int>(curX - 1, curY) == 1)
					{// �ϱ�
						neighborPixels.push(std::pair<int, int>(curX - 1, curY));
					}
					if (lableImg.at<int>(curX + 1, curY) == 1)
					{// �±�
						neighborPixels.push(std::pair<int, int>(curX + 1, curY));
					}
				}
			}
		}
	}

}

void Two_Pass(const cv::Mat& binImg, cv::Mat& lableImg)    //����ɨ�跨
{
	if (binImg.empty() ||
		binImg.type() != CV_8UC1)
	{
		return;
	}

	// ��һ��ͨ·

	lableImg.release();
	binImg.convertTo(lableImg, CV_32SC1);

	int label = 1;
	std::vector<int> labelSet;
	labelSet.push_back(0);
	labelSet.push_back(1);

	int rows = binImg.rows - 1;
	int cols = binImg.cols - 1;
	for (int i = 1; i < rows; i++)
	{
		int* data_preRow = lableImg.ptr<int>(i - 1);
		int* data_curRow = lableImg.ptr<int>(i);
		for (int j = 1; j < cols; j++)
		{
			if (data_curRow[j] == 1)
			{
				std::vector<int> neighborLabels;
				neighborLabels.reserve(2);
				int leftPixel = data_curRow[j - 1];
				int upPixel = data_preRow[j];
				if (leftPixel > 1)
				{
					neighborLabels.push_back(leftPixel);
				}
				if (upPixel > 1)
				{
					neighborLabels.push_back(upPixel);
				}

				if (neighborLabels.empty())
				{
					labelSet.push_back(++label);  // ����ͨ����ǩ+1
					data_curRow[j] = label;
					labelSet[label] = label;
				}
				else
				{
					std::sort(neighborLabels.begin(), neighborLabels.end());
					int smallestLabel = neighborLabels[0];
					data_curRow[j] = smallestLabel;

					// ������С�ȼ۱�
					for (size_t k = 1; k < neighborLabels.size(); k++)
					{
						int tempLabel = neighborLabels[k];
						int& oldSmallestLabel = labelSet[tempLabel];
						if (oldSmallestLabel > smallestLabel)
						{
							labelSet[oldSmallestLabel] = smallestLabel;
							oldSmallestLabel = smallestLabel;
						}
						else if (oldSmallestLabel < smallestLabel)
						{
							labelSet[smallestLabel] = oldSmallestLabel;
						}
					}
				}
			}
		}
	}

	// ���µȼ۶��б�
	// ����С��Ÿ��ظ�����
	for (size_t i = 2; i < labelSet.size(); i++)
	{
		int curLabel = labelSet[i];
		int preLabel = labelSet[curLabel];
		while (preLabel != curLabel)
		{
			curLabel = preLabel;
			preLabel = labelSet[preLabel];
		}
		labelSet[i] = curLabel;
	};

	for (int i = 0; i < rows; i++)
	{
		int* data = lableImg.ptr<int>(i);
		for (int j = 0; j < cols; j++)
		{
			int& pixelLabel = data[j];
			pixelLabel = labelSet[pixelLabel];
		}
	}
}
//��ɫ��ʾ
cv::Scalar GetRandomColor()
{
	uchar r = 255 * (rand() / (1.0 + RAND_MAX));
	uchar g = 255 * (rand() / (1.0 + RAND_MAX));
	uchar b = 255 * (rand() / (1.0 + RAND_MAX));
	return cv::Scalar(b, g, r);
}


void LabelColor(const cv::Mat& labelImg, cv::Mat& colorLabelImg)
{
	if (labelImg.empty() ||
		labelImg.type() != CV_32SC1)
	{
		return;
	}

	std::map<int, cv::Scalar> colors;

	int rows = labelImg.rows;
	int cols = labelImg.cols;

	colorLabelImg.release();
	colorLabelImg.create(rows, cols, CV_8UC3);
	colorLabelImg = cv::Scalar::all(0);

	for (int i = 0; i < rows; i++)
	{
		const int* data_src = (int*)labelImg.ptr<int>(i);
		uchar* data_dst = colorLabelImg.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int pixelValue = data_src[j];
			if (pixelValue > 1)
			{
				if (colors.count(pixelValue) <= 0)
				{
					colors[pixelValue] = GetRandomColor();
				}

				cv::Scalar color = colors[pixelValue];
				*data_dst++ = color[0];
				*data_dst++ = color[1];
				*data_dst++ = color[2];
			}
			else
			{
				data_dst++;
				data_dst++;
				data_dst++;
			}
		}
	}
}
















int _tmain(int argc, _TCHAR* argv[])
{
	cv::Mat binImageoriginal = cv::imread("bigbuildingsTYP/tyexample.bmp", 0);
	//��һ������
	Mat binImage;
	//��ȡ�Զ����
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	//���Ͳ���
	erode(binImageoriginal, binImage, element);

	cv::threshold(binImage, binImage, 50, 1, CV_THRESH_BINARY_INV);
	cv::Mat labelImg;
	//Two_Pass(binImage, labelImg, num);
	Seed_Filling(binImage, labelImg);
	//��ɫ��ʾ
	cv::Mat colorLabelImg;
	LabelColor(labelImg, colorLabelImg);
	//cv::imshow("��ͨ����", colorLabelImg);

	imwrite("bigbuildingsTYP/liantongyu.bmp", colorLabelImg);


	clock_t start;
	clock_t end;
	Mat frame, labels;
	int regionsize = 30;// 
	
	frame = imread("bigbuildingsTYP/tyexample.bmp");
	Mat mask;

	start = clock();//��ʼ��ʱ
	Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(frame, 101, regionsize, 10);//����һ������


	slic->iterate();//����������Ĭ��Ϊ10
	slic->enforceLabelConnectivity();
	slic->getLabelContourMask(mask);//��ȡ�����صı߽�
	slic->getLabels(labels);//��ȡ
	int number = slic->getNumberOfSuperpixels();//��ȡ�����ص�����


	cout << "�����ش�С:" << regionsize << endl;
	cout << "����������:" << number << endl;

	int size = 0;

	for (int row = 0; row < labels.rows; row++)
	{
		for (int col = 0; col < labels.cols; col++)
		{
			/* ע�� Mat::at �����Ǹ�ģ�庯��, ��Ҫָ����������, ��Ϊ����ͼ�Ǿ��к�������ͨ����ͼ,
			�������Ĳ������Ϳ��Դ���һ�� Vec3b, ����һ����� 3 �� uchar ���ݵ� Vec(����). ����
			�ṩ����������, [2]��ʾ���Ƿ��ص�����ͨ��, �������� Red ͨ��, ��һ��ͨ��(Blue)��[0]����  R[2] G[1] B[0] */
			//if (labels.at<Vec3b>(row, col)[1] == 241 && labels.at<Vec3b>(row, col)[0] == 0)
			//	//labels.at<Vec3b>(row, col) = Vec3b(255, 255, 255);


			int temp = labels.at<int>(row, col);//temp ��ÿ�����أ�row��col����Ӧ�ķ����ǩ�����һ�ࣻ�ڶ��ࣻһֱ����number�ࣻ


		}
	}



	frame.setTo(Scalar(0, 0, 255), mask);//���ó����ر�Ե����ɫ��

	end = clock();//������ʱ
	cout << "ʱ��:" << end - start << endl;

	//imshow("test", frame);
	imwrite("bigbuildingsTYP/slic.bmp", frame);





	//��ÿһ�������أ����������ڵľ������ͨ��ĸ�������Ϊ1������Ϊ��ɫ����Ϊ2������Ϊ��ɫ����Ϊ3�������ϣ�����Ϊ��ɫ

	for (int flag = 0; flag < number; flag++)
	{//�ҵ����Ϊflag�ĳ����ص���������
		cout << "��ǰ:" << flag << "   ��:" << number << endl;

		int xlnum = 0;

		int firstColorR = 0;
		int firstColorG = 0;
		int firstColorB = 0;
		int secondColorR = 0;
		int secondColorG = 0;
		int secondColorB = 0;

		for (int i = 0; i < labels.rows; i++)
		{
			for (int j = 0; j < labels.cols; j++)
			{
				int tempflag = labels.at<int>(i, j);





				if (flag == tempflag){
					//������ͨ����ɫ����ͼ �ж�����i j����������ɫ
					int r = colorLabelImg.at<Vec3b>(i, j)[2];
					int g = colorLabelImg.at<Vec3b>(i, j)[1];
					int b = colorLabelImg.at<Vec3b>(i, j)[0];

					if (r != 0 && g != 0 && b != 0)
					{
						//cout << "R:" << r << "G:" << g << "B:" << b << endl;
						if (xlnum == 0)
						{

							firstColorR = r;

							firstColorG = g;

							firstColorB = b;
							xlnum++;
						}

						if (xlnum == 1 && r != firstColorR && g != firstColorG && b != firstColorB)
						{
							secondColorR = r;

							secondColorR = g;

							secondColorR = b;
							xlnum++;


						}

					}






				}


			}
		}


		if (xlnum == 1)//��Ե
		{

			for (int i = 0; i < labels.rows; i++)
			{
				for (int j = 0; j < labels.cols; j++)
				{
					if (flag == labels.at<int>(i, j))
					{
						if (frame.at<Vec3b>(i, j) != Vec3b(0, 0, 255))

							frame.at<Vec3b>(i, j) = Vec3b(150, 100, 100);


					}


				}
			}

		}





		if (xlnum == 2)//��������
		{

			for (int i = 0; i < labels.rows; i++)
			{
				for (int j = 0; j < labels.cols; j++)
				{
					if (flag == labels.at<int>(i, j))
					{
						if (frame.at<Vec3b>(i, j) != Vec3b(0, 0, 255))

							frame.at<Vec3b>(i, j) = Vec3b(0, 255, 0);


					}


				}
			}

		}


	}


	//��ͼ�����ͨ��

	for (int i = 0; i < labels.rows; i++)
	{
		for (int j = 0; j < labels.cols; j++)
		{

			if (colorLabelImg.at<Vec3b>(i, j) != Vec3b(0, 0, 0))

				frame.at<Vec3b>(i, j) = colorLabelImg.at<Vec3b>(i, j);




		}
	}

	imshow("xiangling", frame);
	imwrite("bigbuildingsTYP/xiangling.bmp", frame);



	waitKey(0);


	return 0;
}

