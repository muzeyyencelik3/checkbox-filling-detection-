#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // Resmi yükle
    Mat image = imread("/home/kali/Documents/image/images/deneme.png");

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

    // Her konturu değerlendir
    for (size_t i = 0; i < contours.size(); i++) {
        Rect boundingRect = cv::boundingRect(contours[i]);

        // Belirli bir boyut aralığına sahip dikdörtgenleri kabul et (checkbox'ları filtrele)
        int minCheckboxSize = 10;
        int maxCheckboxSize = 22;

        if (boundingRect.width > minCheckboxSize && boundingRect.width < maxCheckboxSize &&
            boundingRect.height > minCheckboxSize && boundingRect.height < maxCheckboxSize) {
            // Checkbox dışında kalan diğer konturları yeşil ile çiz
            rectangle(image, boundingRect, Scalar(0, 255, 0), 2);

            // Yeşil çevrelenen kontürler metin olarak kabul edilsin
            boxRectangles.push_back(boundingRect);
        } else {
            // Köşegen noktalarını çiz
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

    // Kontrol et ve etiketle
    for (size_t i = 0; i < boxRectangles.size(); ++i) {
        rectangle(image, boxRectangles[i], Scalar(255, 0, 0), 2);

        // Kutu numarasını ekle
        putText(image, "Kutu-" + std::to_string(i + 1), Point(boxRectangles[i].x + boxRectangles[i].width + 5, boxRectangles[i].y + 10),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);

        // "choice" olarak işaretlenmiş mi?
        bool isChoice = false;

        // Text rectangles ile kontrol et
        for (size_t j = 0; j < textRectangles.size(); ++j) {
            if (abs(boxRectangles[i].y - textRectangles[j].y) < 5) {
                // Eğer aynı yükseklikte ise "choice"
                rectangle(image, textRectangles[j], Scalar(0, 255, 0), 2);
                putText(image, "choice", Point(textRectangles[j].x + textRectangles[j].width + 5, textRectangles[j].y + 10),
                        FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
                isChoice = true;
            }
        }
    }

    // Sonucu göster
    imshow("Sıralı Checkbox ve Metin Tespiti", image);
    waitKey(0);

    return 0;
}
