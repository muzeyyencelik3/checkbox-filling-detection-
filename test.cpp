// OpenCV kütüphanesini dahil etmek
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/text.hpp>

// OCR için Tesseract kütüphanesini dahil etmek
#include <tesseract/baseapi.h>

// Dosya işlemleri için standart kütüphaneleri dahil etmek
#include <iostream>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

// Ana fonksiyon
int main()
{
    // Görüntüyü dosyadan okumak
    Mat img = imread("sayfa.jpg");

    // Görüntüyü griye çevirmek
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Görüntüyü bulanıklaştırmak
    Mat blur;
    GaussianBlur(gray, blur, Size(5, 5), 0);

    // Görüntüyü eşiklemek
    Mat thresh;
    adaptiveThreshold(blur, thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 11, 2);

    // Görüntü üzerinde kontur bulmak
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Konturları sınıflandırmak
    vector<vector<Point>> checkbox_contours; // Checkbox konturları
    vector<vector<Point>> question_contours; // Soru konturları
    vector<Rect> checkbox_rects; // Checkbox dikdörtgenleri
    vector<Rect> question_rects; // Soru dikdörtgenleri

    for (int i = 0; i < contours.size(); i++)
    {
        // Konturun alanını ve dikdörtgenini hesaplamak
        double area = contourArea(contours[i]);
        Rect rect = boundingRect(contours[i]);

        // Alanı çok küçük veya çok büyük olan konturları atlamak
        if (area < 100 || area > 10000)
            continue;

        // Dikdörtgenin en-boy oranını hesaplamak
        double ratio = (double)rect.width / rect.height;

        // En-boy oranı 0.8 ile 1.2 arasında olan konturları checkbox olarak kabul etmek
        if (ratio > 0.8 && ratio < 1.2)
        {
            checkbox_contours.push_back(contours[i]);
            checkbox_rects.push_back(rect);
        }
        // Diğer konturları soru olarak kabul etmek
        else
        {
            question_contours.push_back(contours[i]);
            question_rects.push_back(rect);
        }
    }

    // OCR için Tesseract nesnesi oluşturmak
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "tur", tesseract::OEM_LSTM_ONLY);
    tess.SetPageSegMode(tesseract::PSM_AUTO);

    // Txt dosyası oluşturmak
    ofstream file;
    file.open("sonuc.txt");

    // Soru ve cevapları okumak ve yazmak
    for (int i = 0; i < question_contours.size(); i++)
    {
        // Sorunun dikdörtgenini almak
        Rect q_rect = question_rects[i];

        // Sorunun görüntüsünü kesmek
        Mat q_img = img(q_rect);

        // Sorunun metnini OCR ile okumak
        tess.SetImage(q_img.data, q_img.cols, q_img.rows, 3, q_img.step);
        string q_text = tess.GetUTF8Text();

        // Sorunun metnini txt dosyasına yazmak
        file << "Soru " << i + 1 << ": " << q_text << endl;

        // Soru ile aynı yatay seviyede olan checkboxları bulmak
        vector<Rect> c_rects; // Checkbox dikdörtgenleri
        for (int j = 0; j < checkbox_rects.size(); j++)
        {
            // Checkboxun dikdörtgenini almak
            Rect c_rect = checkbox_rects[j];

            // Checkboxun y koordinatını almak
            int c_y = c_rect.y + c_rect.height / 2;

            // Checkboxun y koordinatı, sorunun yüksekliği içindeyse, checkboxu listeye eklemek
            if (c_y > q_rect.y && c_y < q_rect.y + q_rect.height)
            {
                c_rects.push_back(c_rect);
            }
        }

        // Checkboxları soldan sağa sıralamak
        sort(c_rects.begin(), c_rects.end(), {
            return a.x < b.x;
            });

        // Checkboxların işaretli olup olmadığını kontrol etmek
        vector<bool> c_checks; // Checkbox işaretleri
        for (int j = 0; j < c_rects.size(); j++)
        {
            // Checkboxun dikdörtgenini almak
            Rect c_rect = c_rects[j];

            // Checkboxun görüntüsünü kesmek
            Mat c_img = thresh(c_rect);

            // Checkboxun içindeki beyaz piksel sayısını bulmak
            int c_count = countNonZero(c_img);

            // Beyaz piksel sayısı belirli bir eşikten büyükse, checkboxun işaretli olduğunu kabul etmek
            bool c_check = c_count > 50;
            c_checks.push_back(c_check);
        }

        // Checkboxların işaretlerini txt dosyasına yazmak
        file << "Cevap " << i + 1 << ": ";
        for (int j = 0; j < c_checks.size(); j++)
        {
            file << (c_checks[j] ? "X" : "O") << " ";
        }
        file << endl << endl;
    }

    // Txt dosyasını kapatmak
    file.close();

    // Sonucu ekrana yazdırmak
    cout << "Sonuc txt dosyasına yazıldı." << endl;

    return 0;
}
