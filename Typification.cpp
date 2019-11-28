
#include "stdafx.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#include "math.h"

 
  

string lang = "09";

string lujingbuilding = "bigbuildingsTYP/afigure"+lang+".bmp";


string centrepointbuilding = "bigbuildingsTYP/tyexamplelscpixelpoint" + lang + ".bmp";

  
string outbuilding = "bigbuildingsTYP/" + lang + "shenyilang.bmp";
string markbuilding = "bigbuildingsTYP/" + lang + "s.jpg";
string markbuilding2 = "bigbuildingsTYP/" + lang + "o.bmp";
string markbuilding3 = "bigbuildingsTYP/" + lang + "p.bmp";





double ju()
{
	Mat src; Mat src_gray;
	int thresh = 30;
	int max_thresh = 255;
	RNG rng(12345);
	src = imread(lujingbuilding, CV_LOAD_IMAGE_COLOR);
	cvtColor(src, src_gray, CV_BGR2GRAY);//灰度化
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	blur(src_gray, src_gray, Size(3, 3)); //滤波
	//namedWindow("image", CV_WINDOW_AUTOSIZE);
	//imshow("image", src);
	//moveWindow("image", 20, 20);
	//定义Canny边缘检测图像
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//利用canny算法检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	//namedWindow("canny", CV_WINDOW_AUTOSIZE);
	//imshow("canny", canny_output);
	//moveWindow("canny", 550, 20);

	//查找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//计算轮廓矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//画轮廓及其质心并显示
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	printf("\t\t features\n");


	//旋转角度
	double angle;
	double ceterx;
	double cetery;
	double sumangle = 0;
	double aveangle;

	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		Scalar color2 = Scalar(0, 0, 255);

		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color2, -1, 8, 0);
		//rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0, 255, 0));


		printf("object%d - area:%.2f - length: %.2f ", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));

		Mat pointsf;
		Mat(contours[i]).convertTo(pointsf, CV_32F);


		Point2d center(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

		double a = mu[i].m20 / mu[i].m00 - center.x*center.x;
		double b = mu[i].m11 / mu[i].m00 - center.x*center.y;
		double c = mu[i].m02 / mu[i].m00 - center.y*center.y;
		double theta = fastAtan2(2 * b, (a - c)) / 2;
		printf("orientation %.1f\n\n", theta);
		char jufang[100];
		sprintf_s(jufang, "%.1f", theta); //图像的矩 主方向

		double jufang_rads = theta*3.1416 / 180;







		//RotatedRect r = fitEllipse(contours[i]);
		//double majorAxis = r.size.height > r.size.width ? r.size.height : r.size.width;//长轴大小
		//double minorAxis = r.size.height > r.size.width ? r.size.width : r.size.height;//短轴大小

		//printf("majorAxis %.1f", majorAxis);

		//printf("minorAxis %.1f", minorAxis);

		//double area = mu[i].m00;//面积
		//int perimeter = arcLength(contours.at(i), true);

		double orientation = theta;

	/*	if (orientation > 90)
		{
			orientation = orientation - 180;

		}
		orientation = orientation + 90;*/
		/*		if (orientation > 45)
		{
		orientation = orientation - 90;

		}*/


		sumangle = sumangle + orientation;
		//接近垂直的 才做变换 如两者垂直后相差十度以内



		//if (orientation > 45)
		//{
		//	orientation = orientation - 45;

		//}
		//求与0 90 180 270 360 的差的最小值，即旋转到水平或垂直方向需要的最小角度

		/*	if (orientation > 45)
		{
		orientation = orientation - 90;

		}*/
		//if (orientation > 45)
		//{
		//	orientation = orientation - 90;

		//}
		//旋转角度
		angle = orientation;
		ceterx = mc[i].x;
		cetery = mc[i].y;

		double orientation_rads = orientation*3.1416 / 180;
		printf("orientation %.1f\n\n", orientation);

	


		line(drawing, Point(mc[i].x, mc[i].y), Point(mc[i].x + 20 * cos(orientation_rads), mc[i].y + 20 * sin(orientation_rads)), cvScalar(0, 0, 255), 2);
		char tam[100];
		sprintf_s(tam, "%.1f", orientation);
		putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 0.9, cvScalar(0, 255, 0), 1.5);

		i = i + 1;
	}
	aveangle = sumangle / (contours.size() / 2);
	printf("sumangle:%.2f ", sumangle);

	printf("aveangle:%.2f ", aveangle);


	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	moveWindow("Contours", 1100, 20);


	cv::imwrite(markbuilding, drawing);




	////图像旋转至水平

	//cv::imshow("src", src);
	//cv::Point2f center(src.cols / 2, src.rows / 2);
	//cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1);
	//cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();

	//rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	//rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

	//cv::Mat dst;
	//cv::warpAffine(src, dst, rot, bbox.size());
	//cv::imshow("dst", dst);

	//for (int row = 0; row < dst.rows; row++)
	//{
	//	for (int col = 0; col < dst.cols; col++)
	//	{
	//		if (dst.at<Vec3b>(row, col)[2] > 0 && dst.at<Vec3b>(row, col)[1] > 0 && dst.at<Vec3b>(row, col)[0]  > 0)
	//		{
	//			dst.at<Vec3b>(row, col) = Vec3b(0, 241, 255);

	//		}

	//		else if (dst.at<Vec3b>(row, col)[2] == 255 && dst.at<Vec3b>(row, col)[1] == 241 && dst.at<Vec3b>(row, col)[0] == 0){

	//			dst.at<Vec3b>(row, col) = Vec3b(0, 241, 255);


	//		}



	//		else{
	//			dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);

	//		}
	//	}
	//}


	//cv::imwrite("area/building093sssss.tif", dst);














	//waitKey(0);
	src.release();
	src_gray.release();
	return aveangle;


}


double averarea()
{
	Mat src; Mat src_gray;
	int thresh = 30;
	int max_thresh = 255;
	RNG rng(12345);
	src = imread(lujingbuilding, CV_LOAD_IMAGE_COLOR);
	cvtColor(src, src_gray, CV_BGR2GRAY);//灰度化
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	blur(src_gray, src_gray, Size(3, 3)); //滤波
	//namedWindow("image", CV_WINDOW_AUTOSIZE);
	//imshow("image", src);
	//moveWindow("image", 20, 20);
	//定义Canny边缘检测图像
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//利用canny算法检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	//namedWindow("canny", CV_WINDOW_AUTOSIZE);
	//imshow("canny", canny_output);
	//moveWindow("canny", 550, 20);

	//查找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//计算轮廓矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//画轮廓及其质心并显示
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	printf("\t\t features\n");


	//旋转角度
	double angle;
	double ceterx;
	double cetery;
	double sumarea = 0;
	double avearea;

	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		Scalar color2 = Scalar(0, 0, 255);

		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color2, -1, 8, 0);
		//rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0, 255, 0));


		printf("object%d - area:%.2f - length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));




		double area = contourArea(contours[i]);//面积





		sumarea = sumarea + area;


		i = i + 1;
	}
	avearea = sumarea / (contours.size() / 2);
	printf("aveangle:%.2f ", avearea);







	src.release();
	src_gray.release();
	return avearea;


}





int zuixiaowanjiejuxing(double aveangle, double hei, double wid, double centerx, double centery)
{


	Mat 原图 = imread(lujingbuilding);
	//imshow("原图", 原图);
	Mat 灰度图, 二值图;
	cvtColor(原图, 二值图, CV_BGR2GRAY);
	//blur(灰度图, 二值图, Size(5, 5));//模糊一下，可以不要
	threshold(二值图, 二值图, 0, 255, CV_THRESH_OTSU);//自适应二值化
	//二值图 = 255 - 二值图;//颜色反转
	//imshow("二值图", 二值图);

	//寻找最外层轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(二值图, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());

	Mat 画布 = Mat::zeros(二值图.size(), CV_8UC1); //最小外接矩形画布  
	for (int i = 0; i<contours.size(); i++)
	{
		//绘制轮廓  
		drawContours(画布, contours, i, Scalar(255), 1, 8, hierarchy);

		//绘制轮廓的最小外结矩形  
		RotatedRect rect = minAreaRect(contours[i]);


		RotatedRect rect2 = RotatedRect(Point2f(centery, centerx), Size2f(hei, wid), aveangle);


		//rectangle(画布, rect2.boundingRect(), Scalar(55));
		Point2f P[4];

		rect2.points(P);
		for (int j = 0; j <= 3; j++)
		{
			line(原图, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			line(画布, P[j], P[(j + 1) % 4], Scalar(111), 2);
		}

		/*
		//绘制轮廓的最小外结圆
		Point2f center; float radius;
		minEnclosingCircle(contours[i], center, radius);
		circle(画布1, center, radius, Scalar(255), 2);
		*/

	}
	//imshow("最小外接矩形", 画布);
	//imshow("标注出矩形",原图 );
	cv::imwrite(outbuilding, 原图);

	waitKey(0);
	return 0;



}












int main(int argc, char** argv)
{
	double centrepointx = 0;
	double centrepointy = 0;

	Mat srccentrepoint = imread(centrepointbuilding);

	//cv::imshow("dst", srccentrepoint);

	for (int row = 0; row < srccentrepoint.rows; row++)
	{
		for (int col = 0; col < srccentrepoint.cols; col++)
		{
			if (srccentrepoint.at<Vec3b>(row, col)[2] == 255 && srccentrepoint.at<Vec3b>(row, col)[1] == 255 && srccentrepoint.at<Vec3b>(row, col)[0] == 255)
			{
				centrepointx = row;
				centrepointy = col;

			}


		}
	}


	double aveangle = ju();
	double maxangle = 0;
	double avearea = averarea();






	//Mat img3 = imread(lujingbuilding);
	//imshow("original", img3);

	//Mat dstImg1, dstImg2, dstImg3, dstImg4, dstImg5, dstImg6, dstImg7;
	//cv::cvtColor(img3, img3, COLOR_RGB2GRAY);//进行，灰度处理  
	//threshold(img3, img3, 20, 255, THRESH_BINARY);
	//Mat element = getStructuringElement( //定义kernel（获取结构元素）  
	//	MORPH_ELLIPSE,//MORPH_RECT，矩形；MORPH_CROSS,交叉形；MORPH_ELLIPSE，椭圆形  
	//	Size(2, 2)//内核尺寸  原始 22
	//	);
	//erode(img3, dstImg1, element, Point(-1, -1), 1);//腐蚀,将高亮部分腐蚀 	
	//imshow("erode", dstImg1);

	//imwrite(markbuilding2, dstImg1);


	////Mat gray;
	//Mat img, gray, edges;
	//img = imread(markbuilding2);
	////cvtColor(dstImg1, gray, CV_BGR2GRAY);
	////GaussianBlur(gray, gray, Size(13, 13), 2, 2);
	//medianBlur(dstImg1, gray, 27);//27


	//imshow("gray", gray);
	//imwrite(markbuilding3, gray);




	Mat src = imread(lujingbuilding);

	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;
	RotatedRect rect;
	double hei = 0;
	double wid = 0;
	double xhei = 0;
	double xwid = 0;
	Mat thr;
	cvtColor(src, thr, COLOR_BGR2GRAY); //Convert to gray
	threshold(thr, thr, 125, 255, THRESH_BINARY); //Threshold the gray
	//bitwise_not(thr, thr); //这里先变反转颜色

	vector<vector<Point> > contours; // Vector for storing contours

	findContours(thr, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE); // Find the contours in the image

	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}





	CvPoint offset;


	for (size_t i = 0; i< contours.size(); i++) // iterate through each contour.
	{
		double area = contourArea(contours[i]);  //  Find the area of contour

		if (area > largest_area)
		{
			largest_area = area;
			largest_contour_index = i;               //Store the index of largest contour
			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			rect = minAreaRect(contours[i]);
			hei = rect.size.height;
			wid = rect.size.width;
			//此处需要加入一个面积值 求最终的长和宽
			double tiaohe = 1;//面积调和参数 线性1.5 格网 2  4 不规则型 数量少2  数量多1.5
			//第一个层次 1.5 
			//第二个层次 2.5
			//第三个层次 4
			double x;
			//面积的两种策略 一种是平均面积 一种是最大面积

			x = sqrt(largest_area*tiaohe / (hei*wid));


			// x = sqrt(avearea*tiaohe / (hei*wid));

			xhei = hei*x;
			xwid = x*wid;


			Point2d center(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

			double a = mu[i].m20 / mu[i].m00 - center.x*center.x;
			double b = mu[i].m11 / mu[i].m00 - center.x*center.y;
			double c = mu[i].m02 / mu[i].m00 - center.y*center.y;
			double theta = fastAtan2(2 * b, (a - c)) / 2;
			printf("orientation %.1f\n\n", theta);
			char jufang[100];
			sprintf_s(jufang, "%.1f", theta); //图像的矩 主方向

			double jufang_rads = theta*3.1416 / 180;//弧度

			maxangle = theta;//角度


	   /* 	RotatedRect r = fitEllipse(contours[i]);
			maxangle = r.angle - 0;*/

			offset = cvPoint(centrepointy - mu[i].m10 / mu[i].m00, centrepointx - mu[i].m01 / mu[i].m00);


		}
	}
	drawContours(src, contours, largest_contour_index, Scalar(0, 255, 0), 2, 8, noArray(), INT_MAX, offset); // Draw the largest contour using previously stored index.
	printf("hei:%.2f ", hei);
	printf("wid:%.2f ", wid);

	imshow("result", src);
	//cv::imwrite(outbuilding, src);







	//可选 平均方向 还是最大面积方向
	zuixiaowanjiejuxing(maxangle,xwid, xhei,  centrepointx, centrepointy);


	//zuixiaowanjiejuxing(aveangle, xwid, xhei, centrepointx, centrepointy);

	waitKey(2000);
	return 0;

}




