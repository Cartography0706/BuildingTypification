// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
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



void Seed_Filling(const cv::Mat& binImg, cv::Mat& lableImg)   //种子填充法
{
	// 4邻接方法


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
				neighborPixels.push(std::pair<int, int>(i, j));     // 像素位置: <i,j>
				++label;  // 没有重复的团，开始新的标签
				while (!neighborPixels.empty())
				{
					std::pair<int, int> curPixel = neighborPixels.top(); //如果与上一行中一个团有重合区域，则将上一行的那个团的标号赋给它
					int curX = curPixel.first;
					int curY = curPixel.second;
					lableImg.at<int>(curX, curY) = label;

					neighborPixels.pop();

					if (lableImg.at<int>(curX, curY - 1) == 1)
					{//左边
						neighborPixels.push(std::pair<int, int>(curX, curY - 1));
					}
					if (lableImg.at<int>(curX, curY + 1) == 1)
					{// 右边
						neighborPixels.push(std::pair<int, int>(curX, curY + 1));
					}
					if (lableImg.at<int>(curX - 1, curY) == 1)
					{// 上边
						neighborPixels.push(std::pair<int, int>(curX - 1, curY));
					}
					if (lableImg.at<int>(curX + 1, curY) == 1)
					{// 下边
						neighborPixels.push(std::pair<int, int>(curX + 1, curY));
					}
				}
			}
		}
	}

}

void Two_Pass(const cv::Mat& binImg, cv::Mat& lableImg)    //两遍扫描法
{
	if (binImg.empty() ||
		binImg.type() != CV_8UC1)
	{
		return;
	}

	// 第一个通路

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
					labelSet.push_back(++label);  // 不连通，标签+1
					data_curRow[j] = label;
					labelSet[label] = label;
				}
				else
				{
					std::sort(neighborLabels.begin(), neighborLabels.end());
					int smallestLabel = neighborLabels[0];
					data_curRow[j] = smallestLabel;

					// 保存最小等价表
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

	// 更新等价对列表
	// 将最小标号给重复区域
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
//彩色显示
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
	//做一次膨胀
	Mat binImage;
	//获取自定义核
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	//膨胀操作
	erode(binImageoriginal, binImage, element);

	cv::threshold(binImage, binImage, 50, 1, CV_THRESH_BINARY_INV);
	cv::Mat labelImg;
	//Two_Pass(binImage, labelImg, num);
	Seed_Filling(binImage, labelImg);
	//彩色显示
	cv::Mat colorLabelImg;
	LabelColor(labelImg, colorLabelImg);
	//cv::imshow("连通域标记", colorLabelImg);

	imwrite("bigbuildingsTYP/liantongyu.bmp", colorLabelImg);


	clock_t start;
	clock_t end;
	Mat frame, labels;
	int regionsize = 30;// 
	
	frame = imread("bigbuildingsTYP/tyexample.bmp");
	Mat mask;

	start = clock();//开始计时
	Ptr<cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(frame, 101, regionsize, 10);//创建一个对象


	slic->iterate();//迭代次数，默认为10
	slic->enforceLabelConnectivity();
	slic->getLabelContourMask(mask);//获取超像素的边界
	slic->getLabels(labels);//获取
	int number = slic->getNumberOfSuperpixels();//获取超像素的数量


	cout << "超像素大小:" << regionsize << endl;
	cout << "超像素数量:" << number << endl;

	int size = 0;

	for (int row = 0; row < labels.rows; row++)
	{
		for (int col = 0; col < labels.cols; col++)
		{
			/* 注意 Mat::at 函数是个模板函数, 需要指明参数类型, 因为这张图是具有红蓝绿三通道的图,
			所以它的参数类型可以传递一个 Vec3b, 这是一个存放 3 个 uchar 数据的 Vec(向量). 这里
			提供了索引重载, [2]表示的是返回第三个通道, 在这里是 Red 通道, 第一个通道(Blue)用[0]返回  R[2] G[1] B[0] */
			//if (labels.at<Vec3b>(row, col)[1] == 241 && labels.at<Vec3b>(row, col)[0] == 0)
			//	//labels.at<Vec3b>(row, col) = Vec3b(255, 255, 255);


			int temp = labels.at<int>(row, col);//temp 即每个像素（row，col）对应的分类标签；如第一类；第二类；一直到第number类；


		}
	}



	frame.setTo(Scalar(0, 0, 255), mask);//设置超像素边缘的颜色；

	end = clock();//结束计时
	cout << "时间:" << end - start << endl;

	//imshow("test", frame);
	imwrite("bigbuildingsTYP/slic.bmp", frame);





	//对每一个超像素，计算其相邻的居民地连通域的个数，如为1，则标记为黄色；如为2；则标记为绿色；如为3个或以上；则标记为蓝色

	for (int flag = 0; flag < number; flag++)
	{//找到类别为flag的超像素的所有像素
		cout << "当前:" << flag << "   总:" << number << endl;

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
					//输入连通域颜色分类图 判断坐标i j处的像素颜色
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


		if (xlnum == 1)//边缘
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





		if (xlnum == 2)//两个相邻
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


	//求图像的连通域

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

