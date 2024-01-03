#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    // Kamera bağlantısını oluştur
    VideoCapture cap(0); // 0 varsayılan kamera

    if (!cap.isOpened()) {
        std::cout << "Kamera bağlantısı açılamadı." << std::endl;
        return -1;
    }

    const float A4_WIDTH_MM = 210.0f; // A4 kağıdın genişliği mm cinsinden
    const float A4_HEIGHT_MM = 297.0f; // A4 kağıdın yüksekliği mm cinsinden

    const float ASPECT_RATIO = A4_WIDTH_MM / A4_HEIGHT_MM; // A4 kağıdın en boy oranı
     // Droid came çözünürlüğü düşürdüğü için kamera çözünürlüğünü ayarlama(başarısız)
    cap.set(CAP_PROP_FRAME_WIDTH, 1080); // Yükseklik
    cap.set(CAP_PROP_FRAME_HEIGHT, 1920); // Genişlik

    Mat frame;
    while (true) {
        cap >> frame; // Kameradan bir kare al

        // Droidcam kullanıldığı için saat yönünde 90 derece döndürme 
        transpose(frame, frame);
        flip(frame, frame, 1); // Yatay ekseni etrafında döndürme

        // Gri tonlamalı görüntü oluşturma
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Kenarları tespit etme
        Mat edges;
        Canny(gray, edges, 50, 150, 3);

        // Contourları bulma
        std::vector<std::vector<Point>> contours;
        findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++) {
            // Algılanan alanın A4 boyutlarına yakın olup olmadığını kontrol etme
            double area = contourArea(contours[i]);
            Rect rect = boundingRect(contours[i]);
            float aspectRatio = (float)rect.width / (float)rect.height;

            // Algılanan alanın genişlik ve boy oranı A4'e yakınsa
            if (aspectRatio > ASPECT_RATIO * 0.9 && aspectRatio < ASPECT_RATIO * 1.1 &&
                area > 0.9 * A4_WIDTH_MM * A4_HEIGHT_MM && area < 1.1 * A4_WIDTH_MM * A4_HEIGHT_MM) {
                // Belgeyi dikdörtgen içine alma ve çizgi ile işaretleme
                rectangle(frame, rect, Scalar(0, 255, 0), 2);

                // Algılanan dikdörtgeni 'detected_page.png' olarak kaydet
                Mat detectedPage = frame(rect);
                //imwrite("detected_page.png", detectedPage);

                //std::cout << "Sayfa algılandı ve 'detected_page.png' olarak kaydedildi." << std::endl;

                // Ekranda gösterme
                imshow("Sayfa Algılama", frame);
                waitKey(0);

                return 0;
            }
        }

        // Ekranda görüntüyü gösterme
        imshow("Sayfa Algılama", frame);
        if (waitKey(30) >= 0)
            break;
    }

    return 0;
}
 
