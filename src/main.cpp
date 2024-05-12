#include <iostream>
#include <utils.h>
#include <Wrapper.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

int undistortFeed();

int main() {
    MyClass c;
    double seconds = c.secondsSince1970();
    std::cout << "Seconds since 1970: " << seconds << std::endl;

    // const char* filePath = "test.mp3";
    // c.playAudio(filePath);
    // std::cout << "Done playing " << filePath << std::endl;

    std::cout << "Hello, CMake!" << std::endl;
    std::cout << "Adding 5 and 3: " << add(5, 3) << std::endl;

        // Create a 2x2 matrix
    Eigen::Matrix2d mat;
    mat << 1, 2,
           3, 4;
    std::cout << "Matrix:\n" << mat << std::endl;

    // Create a vector
    Eigen::Vector2d vec(1, 0);
    std::cout << "Vector:\n" << vec << std::endl;

    // Perform matrix-vector multiplication
    Eigen::Vector2d result = mat * vec;
    std::cout << "Result:\n" << result << std::endl;

    // captureFrames();
    // getCalibratedValues();

    // Now, try to read from the file
    undistortFeed();
    return 0;
}

int captureFrames(){
    // Open webcam
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error opening camera." << std::endl;
        return -1;
    }

    // Prepare variables for calibration
    std::vector<std::vector<cv::Point2f>> imagePoints;
    cv::Size patternSize(9, 6); // Size of the checkerboard pattern
    cv::Mat frame;

    int imagesToCapture = 15;
    bool capturingImages = false;

    std::cout << "Press 'c' to start capturing images for calibration." << std::endl;

    while (true) {
        cap >> frame; // Capture frame from the webcam

        if (capturingImages) {
            // Find checkerboard corners
            std::vector<cv::Point2f> corners;
            bool found = cv::findChessboardCorners(frame, patternSize, corners,
                                                    cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

            // Draw checkerboard corners on the frame
            cv::drawChessboardCorners(frame, patternSize, corners, found);

            // Display frame
            cv::imshow("Calibration", frame);

            // If checkerboard is found, collect image points
            if (found) {
                imagePoints.push_back(corners);
                imagesToCapture--;

                std::cout << "Images remaining: " << imagesToCapture << std::endl;
                if (imagesToCapture == 0) {
                    std::cout << "Finished capturing images." << std::endl;
                    capturingImages = false;
                }
            }
        } else {
            cv::imshow("Calibration", frame);
        }

        // Check for calibration start (press 'c' key)
        char key = cv::waitKey(1);
        if (key == 'c' && !capturingImages) {
            imagesToCapture = 15;
            imagePoints.clear();
            capturingImages = true;
            std::cout << "Press 'c' to start capturing images for calibration." << std::endl;
        } else if (key == 'q') {
            break;
        }
    }

    // Release the camera
    cap.release();
    cv::destroyAllWindows();

    // Save the image points to a file for later calibration
    cv::FileStorage fs("calibration_images.yaml", cv::FileStorage::WRITE);
    fs << "image_points" << imagePoints;
    fs.release();
    return 0;
}

int getCalibratedValues(){
        // Load the image points from the YAML file
    cv::FileStorage fs("calibration_images.yaml", cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cerr << "Failed to open calibration file." << std::endl;
        return -1;
    }

    std::vector<std::vector<cv::Point2f>> imagePoints;
    fs["image_points"] >> imagePoints;
    fs.release();

    // Define the size of the checkerboard pattern
    cv::Size patternSize(9, 6); // Size of the checkerboard pattern

    // Create object points for the chessboard corners
    std::vector<std::vector<cv::Point3f>> objectPoints(15);
    for(int q = 0; q < 15; q++){
        for (int i = 0; i < patternSize.height; ++i) {
            for (int j = 0; j < patternSize.width; ++j) {
                objectPoints[q].push_back(cv::Point3f(j, i, 0));
            }
        }
    }

    // Calibrate camera
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    double rms = cv::calibrateCamera(objectPoints, imagePoints, cv::Size(640, 480),
                                      cameraMatrix, distCoeffs, rvecs, tvecs);

    // Display calibration results
    std::cout << "RMS error: " << rms << std::endl;
    std::cout << "Camera matrix: " << cameraMatrix << std::endl;
    std::cout << "Distortion coefficients: " << distCoeffs << std::endl;
    
    cv::FileStorage fs1("calibration_params1.yaml", cv::FileStorage::WRITE);
    if (!fs1.isOpened()) {
        std::cerr << "Failed to open file 'calibration_params1.yaml'" << std::endl;
        return -1;
    }
    fs1 << "camera_matrix" << cameraMatrix;
    fs1 << "distortion_coefficients" << distCoeffs;
    fs1.release();
    return 0;
}

int undistortFeed(){
    cv::FileStorage fs1;
    fs1.open("calibration_params1.yaml", cv::FileStorage::READ);
    if (!fs1.isOpened()) {
        std::cerr << "Failed to open file 'calibration_params1.yaml' for reading" << std::endl;
        return -1;
    }
    cv::Mat loadedCameraMatrix, loadedDistCoeffs;
    fs1["camera_matrix"] >> loadedCameraMatrix;
    fs1["distortion_coefficients"] >> loadedDistCoeffs;
    std::cout << "Loaded Camera matrix: " << loadedCameraMatrix << std::endl;
    std::cout << "Loaded Distortion coefficients: " << loadedDistCoeffs << std::endl;

    cv::VideoCapture cap(0);
    cv::Mat frame, undistortedFrame;

    while (true) {
        cap.read(frame);
        cv::undistort(frame, undistortedFrame, loadedCameraMatrix, loadedDistCoeffs);
        cv::imshow("Undistorted Frame", undistortedFrame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}