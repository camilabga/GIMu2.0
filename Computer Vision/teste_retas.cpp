#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

Mat frame, HSV, canny, mitPunkte, mitLines, src_grey, pontoLinha;

vector<Vec4i> lParalelas;
vector<Point2f> corners;
vector<float> angles, anglesP;

void swap(int i,int j, vector<float> &a, vector<Vec4i> &lines){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;

    temp = lines[i][0];
    lines[i][0] = lines[j][0];
    lines[j][0] = temp;

    temp = lines[i][1];
    lines[i][1] = lines[j][1];
    lines[j][1] = temp;

    temp = lines[i][2];
    lines[i][2] = lines[j][2];
    lines[j][2] = temp;

    temp = lines[i][3];
    lines[i][3] = lines[j][3];
    lines[j][3] = temp;

}

void quickSort(vector<Vec4i> &lines, vector<float> &arr, int left, int right){
    int min = (left+right)/2;
    int i = left;
    int j = right;
    float pivot = arr[min];

    while(left<j || i<right) {
        while(arr[i]<pivot)
        i++;
        while(arr[j]>pivot)
        j--;

        if(i<=j){
            swap(i,j,arr, lines);
            i++;
            j--;
        }
        else{
            if(left<j)
                quickSort(lines, arr, left, j);
            if(i<right)
                quickSort(lines, arr,i,right);
            return;
        }
    }
}

void filtrar_linhas (vector<Vec4i> &lines) {
  // TANGENTE DAS LINHAS
  angles.clear();
  anglesP.clear();
  lParalelas.clear();
  double theta = 0.0;
  float y1, x1, y2, x2;

  int tam = lines.size();

  for(int i = 0; i < tam; i++ ){
      theta = 0.0;
      x1 = lines[i][0]; //x1
      y1 = lines[i][1]; //y1
      x2 = lines[i][2]; //x2
      y2 = lines[i][3]; //y2

      theta = (atan2(y2-y1, x2-x1)) * 180.0 / CV_PI;
      angles.push_back(abs(theta));
      // TODAS OS VALORES DE TANGENTES SAO ADD NESSE VETOR

      cout << angles[i] << "   ";
  }

  cout << endl << endl;

  bool find_angles = false;

  if (tam > 2) { // ORDENA O VETOR P DEIXAR MAIS FÁCIL DE COMPARAR
    find_angles = true;
    quickSort(lines, angles, 0, tam-1);
  }

  // COMPARA OS ELEMENTOS DO VETOR, SE FOREM IGUAIS -> DESENHA
  // E ADICIONA NO VETOR lParalelas para uso posterior
  if (find_angles){
  	for (int i = 0; i < tam-1; i=i+2) {
  		if (angles[i] - angles[i+1] < 5 && angles[i] - angles[i+1] > -5) {
          lParalelas.push_back(lines[i]);
          lParalelas.push_back(lines[i+1]);
          cout << angles[i] << "   " << angles[i+1] << "   " << endl;
          anglesP.push_back(angles[i]);
          anglesP.push_back(angles[i+1]);

        // COMPARAR AQUI PRA ELIMINAR LINHAS MTO PROXIMAS

        // Pensar nas seguintes situacoes ::: vaca mto perto
                                          //  vaca mto longe

        // fazer um degrade de cores pra conferir as linhas paralelas
        // havendo padrao entre as que identificam a vaca
                              // tentar identificar e tchau migs

          if (angles[i] > 85 && angles[i] < 95){
            line( mitLines, Point(lines[i][0], lines[i][1]),
      					Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
      			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
      					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,0,255), 3, 8 );

          } else {
            line( mitLines, Point(lines[i][0], lines[i][1]),
      					Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 3, 8 );
      			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
      					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,255,0), 3, 8 );
          }

  		}
  	}
    cout << endl;
  }
}

void all_lines() {
	Canny(frame, canny, 50, 200, 3 );
  cvtColor(canny, mitLines, CV_GRAY2BGR );

	vector<Vec2f> lines0;
	vector<Vec4i> lines;


	//TRANSFORMADA DE HOUGH --- ACHAR LINHAS
	#if 0
		HoughLines(canny, lines0, 1, CV_PI/180, 100 );

		for( size_t i = 0; i < lines0.size(); i++ )
		{
				float rho = lines0[i][0];
				float theta = lines0[i][1];
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				Point pt1(cvRound(x0 + 1000*(-b)),
									cvRound(y0 + 1000*(a)));
				Point pt2(cvRound(x0 - 1000*(-b)),
									cvRound(y0 - 1000*(a)));
				line( grey, pt1, pt2, Scalar(0,0,255), 3, 8 );
		}
	#else
  // ultimos tres elementos                  (threshold, minLineLength, maxLineGap)
		HoughLinesP( canny, lines, 1, CV_PI/180, 75, 50, 30);
    filtrar_linhas(lines);

		/*for( size_t i = 0; i < lines.size(); i++ ){
				line( grey, Point(lines[i][0], lines[i][1]),
						Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
		}*/

	#endif
}

int main(int argc, char** argv){
	char* imageName = argv[1];

	frame = imread(imageName, 1);

	if(argc != 2 || !frame.data){
		printf( " No image data \n " );
		return -1;
	}

  cvtColor(frame, src_grey, CV_BGR2GRAY );
  cvtColor(frame,HSV,COLOR_BGR2HSV);

  all_lines();

  namedWindow("Original", WINDOW_NORMAL);
  imshow( "Original", frame );

  namedWindow("Detected Lines", WINDOW_NORMAL);
  imshow( "Detected Lines", mitLines );

	waitKey(0);

	return 0;
}
