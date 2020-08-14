/**The intensity values of the pixels are written into the file using the image processing library OpenCV.
*/
#include<opencv2/opencv.hpp>				
#include <fstream> 

using namespace std;
using namespace cv;

int main() {
	///The image is read using the img object.
	Mat gravel = imread("gravel.jpg");
	ofstream fout;
	fout.open("gravel.txt");
	///Dimensions written.
	fout << "100 100" << endl;
	///Traversing through pixels in each row and writng the intensity values.
	for (int i = 0;i < 100;i++) {
		for (int j = 0;j < 100;j++) {
			///These values are between 0 to 255.
			int intensity = gravel.at<uchar>(i, j);
			fout << intensity << endl;
		}
	}
	fout.close();
	Mat abstract = imread("abstract970.jpg");
	fout.open("abstract970.txt");
	fout << "970 970" << endl;
	///We will only be using the first 100X100 part.
	for (int i = 0;i < 100;i++) {
		for (int j = 0;j < 100;j++) {
			int intensity = abstract.at<uchar>(i, j);
			fout << intensity << endl;
		}
	}
	fout.close();
	return 0;
}
