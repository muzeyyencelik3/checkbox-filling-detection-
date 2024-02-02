#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // Resmi yükle
    Mat image = imread("/home/muzeyyen/Masaüstü/CheckBox/sample3.png");
    if (image.empty()) {
        std::cerr << "Resim yüklenemedi." << std::endl;
        return -1;
    }

    // Gri tonlamaya dönüştür
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Kenarları algıla (GaussianBlur ve Canny kenar dedektörü kullanıldı)
    Mat blurred;
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    Mat edges;
    Canny(blurred, edges, 50, 150);

    // Kenarları iyileştirme (dilate ve erode)
    Mat dilated;
    dilate(edges, dilated, Mat(), Point(-1, -1), 2);
    Mat eroded;
    erode(dilated, eroded, Mat(), Point(-1, -1), 1);

    // Konturları bul
    std::vector<std::vector<Point>> contours;
    findContours(eroded, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Metin ve kutu vektörleri
    std::vector<Rect> textRectangles;
    std::vector<Rect> boxRectangles;
    std::vector<Rect> choiceRectangles;

    // Her konturu değerlendir
    for (size_t i = 0; i < contours.size(); i++) {
        Rect boundingRect = cv::boundingRect(contours[i]);

        // Belirli bir boyut aralığına sahip dikdörtgenleri kabul et (checkbox'ları filtrele)
        int minCheckboxSize = 10;
        int maxCheckboxSize = 22;

        if (boundingRect.width > minCheckboxSize && boundingRect.width < maxCheckboxSize &&
            boundingRect.height > minCheckboxSize && boundingRect.height < maxCheckboxSize) {
            // Checkbox ise yeşil ile çiz
            rectangle(image, boundingRect, Scalar(0, 255, 0), 2);
            boxRectangles.push_back(boundingRect);
        } else {
            // Diğer konturları mavi ile çiz
            rectangle(image, boundingRect, Scalar(255, 0, 0), 2);

            // Mavi çevrelenen kontürler kutu olarak kabul edilsin
            textRectangles.push_back(boundingRect);
        }
    }

    // Metin ve kutu vektörlerini sırala
    std::sort(textRectangles.begin(), textRectangles.end(), [](const Rect &a, const Rect &b) {
        return a.y < b.y;
    });

    std::sort(boxRectangles.begin(), boxRectangles.end(), [](const Rect &a, const Rect &b) {
        return a.y < b.y;
    });

/// Kontrol et ve etiketle
    for (size_t i = 0; i < boxRectangles.size(); ++i) {
        rectangle(image, boxRectangles[i], Scalar(255, 0, 0), 2);

        // Kutu numarasını ekle
     //   putText(image, "Kutu-" + std::to_string(i + 1), Point(boxRectangles[i].x + boxRectangles[i].width + 5, boxRectangles[i].y + 10),
        //        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);

        // "choice" olarak işaretlenmiş mi?
        bool isChoice = false;

        // Text rectangles ile kontrol et
        bool isCheckbox = true;  
        bool isQuestion = false;
        for (size_t j = 0; j < textRectangles.size(); ++j) 
        {
            if (abs(boxRectangles[i].y - textRectangles[j].y) < 5) 
            {
                // Eğer aynı yükseklikte ise "choice"
                rectangle(image, textRectangles[j], Scalar(0, 255, 0), 2);
                //putText(image, "choice", Point(textRectangles[j].x + textRectangles[j].width + 5, textRectangles[j].y + 10),
                      //  FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
                isChoice = true;
                isQuestion = false;  // Eğer bir seçenek bulunduysa, bu kutu bir soru değildir
                // Checkbox içerisi dolu mu kontrol et
            Mat roi = gray(boxRectangles[i]);  // Checkbox'ın olduğu bölgeyi al (Gri tonlamalı resmi kullan)
           
            // İkili görüntüye çevir
            threshold(roi, roi, 128, 255, THRESH_BINARY);

            // Beyaz piksel sayısını kontrol et
            int whitePixelCount = countNonZero(roi);

            // Eğer beyaz piksel sayısı belirli bir eşik değerinden büyükse "checked" yazısını ekle
                if (whitePixelCount < (roi.rows * roi.cols) * 0.5) {
                    putText(image, "X", Point(boxRectangles[i].x + boxRectangles[i].width + 5, boxRectangles[i].y + 10),
                            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
                            bool isAnswer = true;
            }
            }
             else {

                rectangle(image, boxRectangles[i], Scalar(0, 0, 255), 2);  // Kırmızı çerçeve ekle
                
            }
            // Döngüden çıkmak için ESC tuşuna basılıp basılmadığını kontrol edin
       if (waitKey(1) == 27) {
        break;
       }
            
        }
    }
    

    // Sonucu göster
    imshow("Sıralı Checkbox ve Metin Tespiti", image);
    waitKey(0);

    return 0;
}