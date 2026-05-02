#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;

class CameraCalibrationNode : public rclcpp::Node {
public:
    CameraCalibrationNode() : Node("camera_calibration_node") {
        // Subscriber for raw images
        image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/camera/color/image_raw", 10,
            std::bind(&CameraCalibrationNode::image_callback, this, _1));

        // Publisher for updated camera info
        camera_info_pub_ = this->create_publisher<sensor_msgs::msg::CameraInfo>(
            "/calibrated_camera/camera_info", 10);

        // Service to trigger calibration
        calibrate_srv_ = this->create_service<std_srvs::srv::Trigger>(
            "/trigger_calibration",
            std::bind(&CameraCalibrationNode::calibrate_callback, this, _1, _2));

        // Checkerboard settings 
        board_size_ = cv::Size(10, 7);  // 10x7 inner corners
        square_size_ = 0.04;          // 40mm square size
        RCLCPP_INFO(this->get_logger(), "Camera calibration node initialized. Waiting for images...");
    }

private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
    rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr camera_info_pub_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr calibrate_srv_;

    std::vector<cv::Mat> calibration_images_;
    cv::Size board_size_;
    float square_size_;

    // Image callback: Store images for calibration
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg) {
        try {
            cv::Mat img = cv_bridge::toCvCopy(msg, msg->encoding)->image;
            calibration_images_.push_back(img.clone());
            RCLCPP_INFO(this->get_logger(), "Stored image for calibration (%zu total).", calibration_images_.size());
        } catch (const cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "CV Bridge error: %s", e.what());
        }
    }

    // Service callback: Run calibration and publish results
    void calibrate_callback(
        const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {

        //TODO:
        //Check there are sufficiently many stored images in the calibration_images array

        // Prepare object points (3D coordinates of checkerboard corners)
        std::vector<std::vector<cv::Point3f>> object_points;
        std::vector<std::vector<cv::Point2f>> image_points;
        std::vector<cv::Point3f> obj;
        for (int i = 0; i < board_size_.height; i++) {
            for (int j = 0; j < board_size_.width; j++) {
                obj.push_back(cv::Point3f(j * square_size_, i * square_size_, 0));
            }
        }

        // TODO Detect checkerboard corners in each image
        for (const auto& img : calibration_images_) {
          //Convert image to gray scale
          //call to findChessboardCorners
          //if found
          //  *refine the corner locations using cornerSubPix
          //  *push detected 2D corners into image_points
          //  *push object point 3D locations into object_points
          //  *visualize and display detection
        }        

        //TODO Check there are sufficintly many detections to run a calibration
        RCLCPP_INFO(this->get_logger(), "Calibrating based on %d detected chessboard images", object_points.size());

        //TODO Calibrate camera using cv::calibrateCamera. Calculate re-projection error
        double reprojection_error=0;

        //TODO find the top3 images with highest re-projection error. 
        //Check if user has requested (as a node parameter) to remove these images, if yes, remove them and re-do the calibration without them

        //TODO Fill a camer info message with the estimated parameters and publish it 
        sensor_msgs::msg::CameraInfo camera_info;
        camera_info.header.stamp = this->now();
        camera_info.header.frame_id = "camera";
        camera_info.height = calibration_images_[0].rows;
        camera_info.width = calibration_images_[0].cols;

        // Fill camera matrix (3x3)
        // Fill distortion coefficients (5x1)
        // Projection matrix (3x4)
        camera_info_pub_->publish(camera_info);

        response->success = true;
        response->message = "Calibration completed. Reprojection error: " + std::to_string(reprojection_error) + " pixels";

        //Displaying rectified images
        RCLCPP_INFO(this->get_logger(), "Displaying rectified images...");

        //TODO rectify images and show them to screen. Verify the rectified images look sound. 
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraCalibrationNode>());
    rclcpp::shutdown();
    return 0;
}
