#include <stdio.h>
#include <opencv2/core/core.hpp>  
#include <opencv/cv.h>
#include <opencv/highgui.h>  
#include <string>  
#include <vector>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <stdio.h>
#include <iostream>
using namespace cv;  
using namespace std;

#define nWidth 1280
#define nHeight 720
#define FrameSize nWidth*nHeight*3/2
#define YSize nWidth*nHeight
#define USize nWidth*nHeight / 4
#define VSize nWidth*nHeight / 4

int main()
{
	int i, k;
	Mat tmp, tmpY;

	FILE* pFileIn = fopen("../test.yuv", "rb+");

	for (int t = 0; t < 250; ++t)
	{
		Mat yMat = Mat(cvSize(nWidth, nHeight), CV_8UC1, cv::Scalar(0, 0, 0));
		Mat uMat = Mat(cvSize(nWidth/2, nHeight/2), CV_8UC1, cv::Scalar(0, 0, 0));
		Mat vMat = Mat(cvSize(nWidth/2, nHeight/2), CV_8UC1, cv::Scalar(0, 0, 0));
		Mat rgbMat = Mat(cvSize(nWidth, nHeight), CV_8UC3, cv::Scalar(0, 0, 0));

		uchar* yPtr = yMat.ptr(0);
		uchar* uPtr = uMat.ptr(0);
		uchar* vPtr = vMat.ptr(0);
		uchar* rgbPtr = rgbMat.ptr(0);

		unsigned char *pBuf = new unsigned char[FrameSize];
	
		fread(pBuf,  sizeof(unsigned char),FrameSize,pFileIn);

		cout << t << endl;
		for(i=0; i<YSize; i++){
			yPtr[i] = pBuf[i];	
		}

		k=0;
		uPtr = &pBuf[YSize];
		vPtr = &uPtr[YSize/4];
		/*for(i=YSize; i< FrameSize;){
			uPtr[k] = pBuf[i];
			vPtr[k] = pBuf[i+1];
			i += 2;
			k += 1;
		}*/
		//resize(yMat, tmpY, cvSize(nWidth/2, nHeight/2));
		//yPtr = tmpY.ptr(0);
		
		for (int x = 0; x < nHeight; ++x)
			for (int y = 0; y < nWidth; ++y)
			{
				int index = x*nWidth+y;
				int indexU = x/2*nWidth/2+y/2;
				int indexV = indexU;
				//rgbPtr[index*3+2] = yPtr[index] + 1.13983 * (uPtr[indexU] - 128); //R
				//rgbPtr[index*3+1] = yPtr[index] - 0.39465 * (uPtr[indexU] - 128) - 0.58060 * (vPtr[indexV] - 128); //G
				//rgbPtr[index*3+0] = yPtr[index] + 2.03211 * (vPtr[indexV] - 128); //B
				rgbPtr[index*3+2] = yPtr[index] + 1.370705f * (vPtr[indexV]-128); //R
				rgbPtr[index*3+1] = yPtr[index] - 0.698001f * (uPtr[indexU]-128) - 0.703125f * (vPtr[indexV]-128); //G
				rgbPtr[index*3+0] = yPtr[index] + 1.732446f * (uPtr[indexU]-128); //B
			}
		imshow("new.jpg", rgbMat);
//		imshow("U", uMat);
//		imshow("v", vMat);
//		imshow("Y", tmpY);
		cvWaitKey(10);
	}
	return 0;
}
