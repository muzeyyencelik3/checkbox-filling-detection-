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
        //*************cv::drawContours(frame, contours, -1, cv::Scalar(0, 255, 0), 2);
        
        
        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area > 100) {
                // Calculate centroid of the contour
                cv::Moments moments = cv::moments(contour);
                cv::Point centroid(moments.m10 / moments.m00, moments.m01 / moments.m00);

                // TODO: Match each checkbox contour to a specific question
                // For simplicity, this example draws rectangles around the identified checkboxes
                cv::Rect boundingBox = cv::boundingRect(contour);
                cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2);

                // You can use the centroid position or other criteria to determine the question
                // Here, it simply prints the centroid coordinates as an example
                
                
                // *****In this simple example, we assume that checkboxes on the left belong to question 1,
                // and checkboxes on the right belong to question 2.
                // You may need a more sophisticated approach or a machine learning model in a real scenario.
                
                 int questionNumber = (centroid.x < frame.cols / 2) ? 1 : 2;

                cv::Rect boundingBox = cv::boundingRect(contour);
                cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2);
                std::cout << "Checkbox at (" << centroid.x << ", " << centroid.y << ") belongs to a question." << std::endl;
            }
        }   //Deneme

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
