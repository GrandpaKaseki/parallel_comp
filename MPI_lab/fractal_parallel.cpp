#include <iostream>
#include <opencv2/opencv.hpp>
#include <complex>
#include <mpi.h>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {


    int width = 1200;
    int height = 800;
    int n_iter = 1500;

    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int start = rank * (height / size);
    int end = min((rank + 1) * (height / size), height);

    int r, g, b;
    Mat img = Mat::zeros(end - start, width, CV_8UC3);

    for (int y = start; y < end; y++)
    {
        for (int x = 0; x <width; x++)
        {
            double a = -2.5 + (2.5 - (-2.5)) * x / width;
            double b = -2.5 + (2.5 - (-2.5)) * y / height;

            complex <double> z(0, 0);
            complex<double> c(a, b);
            Vec3b color;
            int n = 0;
            while (abs(z) <= 4 && n < n_iter)
            {
                z = z * z + c;
                n++;
            }
                if (n == n_iter - 1)
                {
                    img.at<Vec3b>(y - start, x) = Vec3b(255, 255, 255);

                }
                else 
                {
                    img.at<Vec3b>(y - start, x) = Vec3b((n % 2) * 32 + 64, (n % 8) * 64 + 32, (n % 2) * 16 + 264);
                }   
        }
    }
    Mat fractal(height, width, CV_8UC3);

    MPI_Gather(img.data, (end - start) * width * 3, MPI_UNSIGNED_CHAR,
        fractal.data, (end - start) * width * 3, MPI_UNSIGNED_CHAR,
        0, MPI_COMM_WORLD);
    fractal = fractal(Range(100, height - 100), Range(50, width * 0.7));
    if (rank == 0) {
        imwrite("frac_res.jpg", fractal);
        waitKey(0);
    }

    MPI_Finalize();
    return 0;
}