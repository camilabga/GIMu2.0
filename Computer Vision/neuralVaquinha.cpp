#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/ml/ml.hpp>


using namespace cv;
using namespace std;
//using namespace cv::ml;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// LIMITES DOS HISTOGRAMAS
const int LIMIT_MINOR = 55;
const int LIMIT_MAJOR = 200;

Mat frame, HSV, canny, mitPunkte, mitLines, src_grey, pontoLinha, mitNeural;

vector<Vec4i> lParalelas;
vector<Point2f> corners, pontos, punkte;
vector<float> angles, anglesP;


// ---------  COISAS PARA K-MEANS  -----------
// SET DE DADOS PARA IA

const int K = 2; // NUMERO DE CLUSTERS
const int nCICLOS = 1000;
bool first_time = true;
int c[2][5];

//////////////////////////////////////////////////////////////////////////////////////////////

void select_hist(){
  unsigned int tam = pontos.size(); // pegar tamanho de algum vetor

  long pixels;

  int trainingData[tam][5];
  /*
  [0] -> x
  [1] -> y
  [2] -> intensidade branco
  [3] -> medio
  [4] -> intensidade preto
  */

  int rows = src_grey.rows;
  int cols = src_grey.cols;

  for (unsigned int i = 0; i < tam; i++) {
    trainingData[i][0] = pontos[i].x;
    trainingData[i][1] = pontos[i].y;

    //GET HISTOGRAMAS
      int x1 = trainingData[i][0]-5, x2= trainingData[i][0]+5, y1= trainingData[i][1]-5, y2= trainingData[i][1]+5;

      if (x1<0) {
        x1 = 0;
      }

      if (x2>cols) {
        x2 = cols-1;
      }

      if (y1<0) {
        y1 = 0;
      }

      if (y2>rows) {
        y2 = rows-1;
      }

      trainingData[i][2] = 0;
      trainingData[i][3] = 0;
      trainingData[i][4] = 0;

      MatIterator_<uchar> it, end;
        for( it = src_grey.begin<uchar>(), end = src_grey.end<uchar>(); it != end; ++it){

          if (*it <= LIMIT_MINOR) {
            trainingData[i][2]++;
          } else if (*it < LIMIT_MAJOR) {
            trainingData[i][3]++;
          } else {
            trainingData[i][4]++;
          }
        }
      }

      float porcentagem;

      for (unsigned int i = 0; i < tam; i++) {
        pixels = trainingData[i][2] + trainingData[i][3] + trainingData[i][4];
        porcentagem = (trainingData[i][2]+trainingData[i][4])/(1.0*pixels);
        cout << porcentagem << endl;
        if (porcentagem > 0.5) {
          punkte.push_back(Point(trainingData[i][0],trainingData[i][1]));
        }
      }

      mitPunkte = frame.clone();
}

void istInDerLinie(){
  float y1, x1, y2, x2, x, y;
  float A, B;

  for (unsigned int i = 0; i < corners.size(); i++) {
    x = corners[i].x;
    y = corners[i].y;

    for(unsigned int j = 0; j < lParalelas.size(); j++ ){
        x1 = lParalelas[j][0];
        y1 = lParalelas[j][1];
        x2 = lParalelas[j][2];
        y2 = lParalelas[j][3];

        A = (y2-y1)/(x2-x1);
        B = y1 - A*x1;


        if (A*x + B - y < 1 || A*x + B - y > -1) {
            line(pontoLinha, Point(x1,y1),
      				  Point(x2, y2), Scalar(0,0,255), 5, 8 );
            circle(pontoLinha, Point(x1,y1), 5, Scalar(0, 255, 0), -1, 8, 0 );

            circle(pontoLinha, Point(x2,y2), 5, Scalar(0, 255, 0), -1, 8, 0 );

            pontos.push_back(Point(x,y));
            break;
      }
    }

  }
  // CHAMAR ML

  punkte.clear();
  if (pontos.size() > 10) {
      select_hist();
  }

  if (punkte.size() > 10) {
    cout << "aqui" << endl;
    for(unsigned int i = 0; i < punkte.size(); i++ ){
      circle(mitPunkte, punkte[i], 10, Scalar(173, 0, 0), -1, 8, 0 );
      circle(mitPunkte, punkte[i], 12, Scalar(0, 0, 255), -1, 8, 0 );
    }
  }

}

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

  }

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
          if (angles[i] > 85 && angles[i] < 95){
            lParalelas.push_back(lines[i]);
            lParalelas.push_back(lines[i+1]);
            anglesP.push_back(angles[i]);
            anglesP.push_back(angles[i+1]);
            line( mitLines, Point(lines[i][0], lines[i][1]),
      					Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
      			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
      					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,0,255), 3, 8 );

                pontos.push_back(Point(lines[i][0], lines[i][1]));
                pontos.push_back(Point(lines[i+1][0], lines[i+1][1]));
          }
  		}
  	}
  }



  /*if (pontos.size() > 10) {
      kmeans_training(pontos);
  }*/

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

    /*Mat hough = src_grey;
    for (unsigned int i = 0; i < lines.size(); i++){
      line( hough, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }

    namedWindow("Hough Lines", WINDOW_NORMAL);
	  resizeWindow("Hough Lines", 640,480);
    imshow( "Hough Lines", hough );*/


    filtrar_linhas(lines);

		/*for( size_t i = 0; i < lines.size(); i++ ){
				line( grey, Point(lines[i][0], lines[i][1]),
						Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
		}*/

	#endif
}

void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
  erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
  dilate(thresh,thresh,dilateElement);

}

void find_corners(){ // NAO MEXE NOS PARAMETROS PELO AMOR DE DEUS
  mitPunkte = src_grey.clone();
  pontoLinha = src_grey.clone();

  corners.clear();
  double qualityLevel = 0.05;
  double minDistance = 100;
  int blockSize = 3;
  bool useHarrisDetector = false;
  double k = 0.04;
  int MAX_QUINAS = 40;

  /// Apply corner detection
  goodFeaturesToTrack(mitPunkte, corners, MAX_QUINAS,
    qualityLevel, minDistance, Mat(), blockSize,
    useHarrisDetector, k);

    cvtColor(mitPunkte, mitPunkte, CV_GRAY2BGR);
    cvtColor(pontoLinha, pontoLinha, CV_GRAY2BGR);

  for(unsigned int i = 0; i < corners.size(); i++ ){
    pontos.push_back(corners[i]);

  }

  /*if (corners.size() > 10) {
      kmeans_training(corners);
  }*/
}

int main(){
  VideoCapture capture(0);
  ///./neu  VideoCapture capture("vaquinha_melhor.mp4");
  if ( !capture.isOpened() ){
  	cout << "Cannot open the video file. \n";
  	return -1;
  }

  /*int width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
  int height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

  Size frameSize(static_cast<int>(width), static_cast<int>(height));

  VideoWriter oVideoWriter ("out.avi", CV_FOURCC('P','I','M','1'), 20, frameSize,true);

   if ( !oVideoWriter.isOpened() )
   {
      cout << "ERROR: Failed to write the video" << endl;
      return -1;
    }*/


  while(1){
  	if (!capture.read(frame)) {
  		cout<<"\n Cannot read the video file. \n";
      break;
    }

    cvtColor(frame, src_grey, CV_BGR2GRAY );
		cvtColor(frame,HSV,COLOR_BGR2HSV);
		morphOps(frame);

    lParalelas.clear();

		namedWindow("Detected Quinas", WINDOW_NORMAL);
	  resizeWindow("Detected Quinas", 640,480);
		namedWindow("Mit Neural", WINDOW_NORMAL);
    resizeWindow("Mit Neural", 640, 480);


    // FUNCOES PRINCIPAIS
    pontos.clear();
		all_lines(); // pega todas as linhas e coloca num vec4f e dps chama filtrar_linhas(lines) pra selecionar só as paralelas
    find_corners(); // usa o algoritmo shi pra achar pontos de interesse (quinas)
    istInDerLinie(); // mantém as linhas que cruzam os pontos achados na funcao anterior


    imshow( "Detected Quinas", mitPunkte);
    imshow( "Mit Neural", mitNeural);

     /*oVideoWriter.write(mitNeural);

     if (waitKey(10) == 27)
         {
            cout << "esc key is pressed by user" << endl;
            break;
         }*/

		if(waitKey(30) == 27){
      break;
    }
  }

  return 0;
}
