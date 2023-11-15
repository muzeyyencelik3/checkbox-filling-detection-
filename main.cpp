#include <opencv2/opencv.hpp>

int main() {
    // Initialize video capture
    cv::VideoCapture cap(0);

    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // Create a window to display the camera feed
    cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL);

    // Main loop
    while (true) {
        // Capture frame-by-frame
        cv::Mat frame;
        cap >> frame;

        // Check if the frame is empty (end of video stream)
        if (frame.empty()) {
            std::cerr << "Error: End of video stream." << std::endl;
            break;
        }

        // Display the frame in the window
        cv::imshow("Camera Feed", frame);



       //kodu buraya eklicez.

          // Convert the frame to grayscale
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        
        // Apply thresholding to detect white regions (adjust threshold value as needed)
        cv::Mat whiteMask;
        cv::threshold(gray, whiteMask, 170, 200, cv::THRESH_BINARY);

        // Find contours in the binary image
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(whiteMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Draw contours on the original frame
        cv::drawContours(frame, contours, -1, cv::Scalar(0, 255, 0), 2);

      // Display the frame in the window
        cv::imshow("Camera Feed", frame);


        // Check if ESC key is pressed to exit the loop
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Release the camera and close the window
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
