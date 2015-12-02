
#include "ShapeFromFocus.h"

ShapeFromFocus::ShapeFromFocus()
{

}

ShapeFromFocus::~ShapeFromFocus()
{

}

void ShapeFromFocus::Sobel_in_OpenCV(Mat image, Mat *dstImage)
{

	Mat sobelX, sobelY;

	Sobel(image, sobelX, CV_16S, 1, 0);
	Sobel(image, sobelY, CV_16S, 0, 1);
	Mat sobel;
	sobel = abs(sobelX) + abs(sobelY); // L1 �� ���

	double sobmin, sobmax;
	minMaxLoc(sobel, &sobmin, &sobmax); // �Һ� �ִ� ã��

	// sobelImage = -alpha*sobel + 255 // 8��Ʈ ���� ��ȯ
	sobel.convertTo(*dstImage, CV_8U, 255. / sobmax, 0);


}

void ShapeFromFocus::Laplacian_in_OpenCV(Mat image, Mat *dstImage)
{

	Mat src_gray, dst;

	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;

	// Remove noise by blurring with a Gaussian filter
	//blur(image, image, Size(5, 5), Point(-1, -1), 2);
	//GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);


	/// Apply Laplace function
	Laplacian(image, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(dst, *dstImage);

}

void ShapeFromFocus::LaplacianFilter(Mat image, Mat *dstImage)
{
	int laplacian = 0;

	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);

	for (int j = 1; j < image.rows - 1; j++)
	{
		for (int i = 1; i < image.cols - 1; i++)
		{
			laplacian = 
				( image.data[(j - 1)* image.step + (i)] 
				+ image.data[(j)* image.step + (i - 1)] 
				+ image.data[(j)* image.step + (i + 1)] 
				+ image.data[(j + 1)* image.step + (i)] )
				- (4 * image.data[(j)* image.step + (i)]);
				

			(*dstImage).data[j * image.step + (i)] = (unsigned char)laplacian;
		}
	}

}

void ShapeFromFocus::SumOfModuleDifferent(Mat image, Mat *dstImage)
{
	
	int smdX = 0;
	int smdY = 0;
	int smdF = 0;

	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);
	
	for (int j = 0; j < image.rows - 1; j++)
	{
		for (int i = 0; i < image.cols - 1; i++)
		{
			smdX = abs(image.data[j * image.step + (i)] - image.data[(j) * image.step + (i + 1)]);
			smdY = abs(image.data[j * image.step + (i)] - image.data[(j + 1) * image.step + (i)]);
			smdF = smdX + smdY;

			(*dstImage).data[j * image.step + (i)] = (unsigned char)smdF;
		}
	}
}

void ShapeFromFocus::ModifiedLaplacian(Mat image, Mat *dstImage)
{
	int ml = 0;

	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);

	for (int j = 1; j < image.rows - 1; j++)
	{
		for (int i = 1; i < image.cols - 1; i++)
		{
			ml = abs((2 * image.data[(j) * image.step + (i)]) - image.data[(j) * image.step + (i - 1)] - image.data[(j) * image.step + (i + 1)])
				+ abs((2 * image.data[(j) * image.step + (i)]) - image.data[(j - 1)* image.step + (i)] - image.data[(j + 1)* image.step + (i)]);			

			(*dstImage).data[j * image.step + (i)] = (unsigned char)ml;
		}
	}

}

void ShapeFromFocus::EnergyOfLaplacianOfImage(Mat image, Mat *dstImage)
{
	float ml = 0;

	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);

	for (int j = 1; j < image.rows - 1; j++)
	{
		for (int i = 1; i < image.cols - 1; i++)
		{
			ml = (1/6.) * 
				abs((((1)*image.data[(j-1)* image.step + (i-1)]) + ((4)*image.data[(j-1)* image.step + (i)]) + ((1)*image.data[(j-1)* image.step + (i+1)])) + 
				((4) * image.data[(j)* image.step + (i-1)]) + ((-20)*image.data[(j)* image.step + (i)]) + ((4)*image.data[(j)* image.step + (i+1)]) +
				(((1)*image.data[(j+1)* image.step + (i-1)]) + ((4)*image.data[(j+1)* image.step + (i)]) + ((1)*image.data[(j+1)* image.step + (i+1)])));

			(*dstImage).data[j * image.step + (i)] = (unsigned char)ml;
		}
	}

}

void ShapeFromFocus::GrayLevelVariance(Mat image, Mat *dstImage)
{
	float ml = 0;
	int N = image.cols*(image.rows-200);
	float average = 0;
	float variance = 0;

	float visualize = 800;
	
	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);

	for (int j = 100; j < image.rows-100; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			average += image.data[j * image.step + (i)];
		}
	}
	average = average / N;


	for (int j = 100; j < image.rows-100; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			ml = abs((image.data[j * image.step + (i)] * image.data[j * image.step + (i)]) - (average * average)) / visualize;

			(*dstImage).data[j * image.step + (i)] = (unsigned char)ml;
		}
	}
}


void ShapeFromFocus::HistogramEntropy(Mat image, Mat *dstImage)
{
	int ml = 0;
	float freq[256] = { 0 };
	float visualize = 20000;

	int threshold = 0;

	(*dstImage).create(image.rows, image.cols, CV_8UC1);

		
	for (int j = 100; j < image.rows-100; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			ml = 0;

			freq[image.data[j * image.step + (i)]]++;
			
			//(*dstImage).data[j * image.step + (i)] = (unsigned char)ml;
		}
	}

	//for (int i = 0; i < 255; i++)
		//freq[i] = freq[i] / ((image.rows - 200) * image.cols);

	for (int j = 100; j < image.rows - 100; j++)
	{
		for (int i = 0; i < image.cols; i++)
		{
			ml = visualize / (freq[image.data[j * image.step + (i)]] * log2(freq[image.data[j * image.step + (i)]] ));

			(*dstImage).data[j * image.step + (i)] = (unsigned char)ml;
		}
	}
}