#include "GrayLevelCooccurrenceMatrix.h"
#include "FindFocus.h"
#include "ShapeFromFocus.h"

//video
int main()
{
	Mat image;
	Mat grayImage;

	Mat modifiedLaplacian;
	Mat laplacianImage_in_OpenCV;
	Mat sumOfModuleDifferent;

	VideoCapture cap("Paris tilt shift.mp4");		

	for (;;)
	{
		cap >> image;		


		cvtColor			(image, grayImage, CV_BGR2GRAY);
		//FindFocus findFocus	(grayImage, GLCM_ANGLE_ALL, HARALICK_ASM);
		//findFocus.Sequential(grayImage, image, 1);

			

		//GrayLevelCoOccurrenceMatrix grayLevelCoOccurrenceMatrix	(GLCM_ANGLE_ALL, HARALICK_CORRELATION);
		//float test =  grayLevelCoOccurrenceMatrix.CoOccurrence(grayImage, 1);
		//std::cout << test << std::endl;


		ShapeFromFocus shapeFromFocus;
		shapeFromFocus.LaplacianFilter(grayImage, &laplacianImage_in_OpenCV);
		shapeFromFocus.ModifiedLaplacian(grayImage, &modifiedLaplacian);
		shapeFromFocus.SumOfModuleDifferent(grayImage, &sumOfModuleDifferent);

		imshow("modifiedLaplacian", modifiedLaplacian);
		imshow("laplacianImage_in_OpenCV", laplacianImage_in_OpenCV);
		imshow("sumOfModuleDifferent", sumOfModuleDifferent);

		imshow("grayImage", grayImage);
		imshow("srcImage", image);
		
	 	std::cout << "test" <<std::endl;

		//waitKey(0);
		if (waitKey(1) >= 0)
			break;;
	}

	return 0;

}


////image
//void main()
//{
//	mat image[20];
//	mat grayimage[20];
//	mat dstimage[20];
//
//	for (int i = 0; i < 19; i++)
//	{
//		char tmp[4];
//
//		string str1 = "c:\\users\\kwk\\documents\\sample\\11-16\\";;
//		string str2 = _itoa(i + 1, tmp, 10);
//		string str3 = ".bmp";
//		string str = str1 + str2 + str3;
//
//		image[i] = imread(str, 1);
//	}
//
//	for (int i = 0; i < 19; i++)
//	{
//		cvtcolor(image[i], grayimage[i], cv_bgr2gray);
//		
//		gaussianblur(grayimage[i], grayimage[i], size(3, 3), 0, 0, border_default);
//
//		//highpass_sobel(grayimage[i], &image[i]);
//		//highpass_laplacian(grayimage[i], &image[i]);
//		canny(grayimage[i], image[i], 150, 150, 3);
//		//medianblur(image[i], image[i], 3);
//
//		//findfocus findfocus(image[i], glcm_angle_all, haralick_contrast);
//		//findfocus.sequential(grayimage[i], image[i], i);		
//		//findfocus.binarysearch(grayimage[i], image[i]);
//
//	}
//
//	for (int i = 0; i < 19; i++)
//	{
//		char windownamed[10];
//		_itoa(i + 20, windownamed, 10);
//		namedwindow(windownamed);
//		movewindow(windownamed, (i * 200), 500);
//		imshow(windownamed, image[i]);
//
//
//		/*char tmp[4];
//
//		string str1 = "c:\\users\\kwk\\documents\\과제\\소형 3차원 스캐너를 위한 confocal 광학모듈 기술 개발\\dstimage\\";
//		string str2 = windownamed;
//		string str3 = ".bmp";
//		string str = str1 + str2 + str3;
//		
//		imwrite(str, image[i]);*/
//		
//		
//		char windownamed2[10];
//		_itoa(i + 1, windownamed2, 10);
//		namedwindow(windownamed2);
//		movewindow(windownamed2, (i  * 200), 10);
//		imshow(windownamed2, grayimage[i]);		
//
//	}	
//
//	waitkey(0);
//
//
//
//}



