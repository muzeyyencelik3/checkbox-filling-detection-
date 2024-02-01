#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;

int main() {
    
   

	// Fotoğrafın bulunduğu dizin ve adı
	std::string path = "/home/kali/Documents/image/images/photo.jpg";

	// Fotoğrafı okuyup detectedA4 değişkenine atıyoruz
	cv::Mat detectedA4 = cv::imread(path);

	// Gri tonlamalı görüntü oluşturma (gerekli olabilir ve bundan sonraki bikaç satır sonra silinecek)
    Mat gray;
    cvtColor(detectedA4, gray, COLOR_BGR2GRAY);

    // Kenarları tespit etme
    Mat edges;
    Canny(gray, edges, 50, 150, 3);

    // Contourları bulma
    std::vector<std::vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    //Kare ve daire vektörleri oluşturma
    std::vector<std::vector<Point>> squares;
    std::vector<std::vector<Point>> circles;

    for (size_t i = 0; i < contours.size(); i++) {
        // Konturun alanını hesaplama
        double area = contourArea(contours[i]);

        // Kare ya da daire olup olmadığını kontrol etme
        if (area > 500) { // Eşik değeri, algılanacak şeklin alanına bağlı olarak değişebilir
            std::vector<Point> approx;
            approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);

            if (approx.size() == 4) {
                // Kare
                squares.push_back(approx);
            } else {
                // Daire
                Point2f center;
                float radius;
                minEnclosingCircle(contours[i], center, radius);
                if (std::abs(area - CV_PI * radius * radius) < 100) {
                    circles.push_back(contours[i]);
                }
            }
        }
    }

    // Kare ve daireleri fotoğraf üzerinde çizme
    // Kırmızı renkli ve 2 piksel kalınlığında bir kalem oluşturma
    Scalar color = Scalar(0, 0, 255);
    int thickness = 2;

    // Kare vektöründeki her bir eleman için
    for (size_t i = 0; i < squares.size(); ++i) {
        // Fotoğraf üzerinde kare çizme
        polylines(detectedA4, squares[i], true, color, thickness);
    }
    

    // Daire vektöründeki her bir eleman için
    for (size_t i = 0; i < circles.size(); ++i) {
        // Fotoğraf üzerinde daire çizme
        Point2f center;
        float radius;
        minEnclosingCircle(circles[i], center, radius);
        circle(detectedA4, center, radius, color, thickness);
    }

    // Fotoğrafı ekrana gösterme
    imshow("Detected Shapes", detectedA4);
    waitKey(0);

    return 0;
}
