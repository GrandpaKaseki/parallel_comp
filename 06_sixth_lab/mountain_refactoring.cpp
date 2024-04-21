#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
using namespace cv;
using namespace std;



int main() {
    Mat img = cv::imread("mountains.jpg", IMREAD_COLOR);

    if (img.empty()) {
        cout << "Error! Failed to open img file" << endl;
        return -1;
    }

    Mat gray = img.clone();
    Mat sepia = img.clone();
    Mat negative = img.clone();
    Mat contours = img.clone();

#pragma omp parallel sections num_threads(4)
    {
#pragma omp section
        //gray
        {
            int rows = img.rows;
            int cols = img.cols;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    Vec3b pixel = img.at<Vec3b>(i, j);
                    uchar grayValue = (pixel[0] + pixel[1] + pixel[2]) / 3;
                    gray.at<Vec3b>(i, j) = Vec3b(grayValue, grayValue, grayValue);
                }
            }
        }
#pragma omp section
        //sepia
        {
            int rows = img.rows;
            int cols = img.cols;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    Vec3b pixel = img.at<Vec3b>(i, j);
                    // b = pixel[0], g = pixel[1], r = pixel[2]
                    int sepiaR = (int)(0.393 * pixel[0] + 0.769 * pixel[1] + 0.189 * pixel[2]);
                    int sepiaG = (int)(0.349 * pixel[0] + 0.686 * pixel[1] + 0.168 * pixel[2]);
                    int sepiaB = (int)(0.272 * pixel[0] + 0.534 * pixel[1] + 0.131 * pixel[2]);
                    sepia.at<Vec3b>(i, j) = Vec3b(min(sepiaB, 255), min(sepiaG, 255), min(sepiaR, 255));
                }
            }
        }
#pragma omp section
        //negative
        {
            int rows = img.rows;
            int cols = img.cols;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    Vec3b pixel = img.at<Vec3b>(i, j);
                    negative.at<Vec3b>(i, j) = Vec3b(255 - pixel[0], 255 - pixel[1], 255 - pixel[2]);
                }
            }
        }
#pragma omp section
        //contuor
        {
            Mat gray;
            cvtColor(img, gray, COLOR_BGR2GRAY);
            Mat edges = Mat(gray.rows, gray.cols, CV_8U);

            for (int i = 1; i < gray.rows - 1; i++) {
                for (int j = 1; j < gray.cols - 1; j++) {
                    float gx = gray.at<uchar>(i + 1, j + 1) + 2 * gray.at<uchar>(i, j + 1) + gray.at<uchar>(i - 1, j + 1) - gray.at<uchar>(i + 1, j - 1) - 2 * gray.at<uchar>(i, j - 1) - gray.at<uchar>(i - 1, j - 1);
                    float gy = gray.at<uchar>(i + 1, j + 1) + 2 * gray.at<uchar>(i + 1, j) + gray.at<uchar>(i + 1, j - 1) - gray.at<uchar>(i - 1, j - 1) - 2 * gray.at<uchar>(i - 1, j) - gray.at<uchar>(i - 1, j + 1);
                    edges.at<uchar>(i, j) = 255 - sqrt(pow(gx, 2) + pow(gy, 2));
                }
            }

            contours = edges.clone();
        }
    }

    imshow("Original img", img);
    imshow("Gray img", gray);
    imshow("Sepia img", sepia);
    imshow("Negative img", negative);
    imshow("Contour img", contours);

    waitKey(0);
    destroyAllWindows();
    return 0;
}