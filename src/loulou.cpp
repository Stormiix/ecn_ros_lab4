#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ecn_common/token_handle.h>

/* Global Variables */
cv_bridge::CvImagePtr cv_ptr; // A pointer to the latest recieved image
bool initialized = false;
bool debug = false;
std::string outputTopic;

// Text styling
std::string text = "Anas & Henrique";
int font = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
double fontScale = 2.1;
int fontStroke = 2;
cv::Scalar textColor = CV_RGB(255, 255, 255);
int baseline = 0;


void callback(const sensor_msgs::ImageConstPtr &msg)
{ // Listener callback
  try
  {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    initialized = true;
  }
  catch (cv_bridge::Exception &e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  // Apply a colorMap to the image
  // applyColorMap(cv_ptr->image, cv_ptr->image, 1);
  // Put a center text on the video stream
  cv::Size textSize = cv::getTextSize(text, font, fontScale, fontStroke, &baseline);
  cv::putText(cv_ptr->image, text, cv::Point((cv_ptr->image.cols - textSize.width) / 2, (cv_ptr->image.rows + textSize.height) / 2), font, fontScale, textColor, fontStroke);
  // Resize to match the screen resolution
  cv::resize(cv_ptr->image, cv_ptr->image, cv::Size(1024, 600));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Lab4 - loulou");
  ecn::TokenHandle token("Groupe4");

  ros::NodeHandle node("~");
  image_transport::ImageTransport transport(node);
  image_transport::Subscriber imageSub;
  image_transport::Publisher imagePub;

  if(!node.getParam("text", text) )
  {
    ROS_INFO("Couldn't find parameter: text\n");
  } else {
    ROS_INFO("Showing: %s\n",text.c_str()) ;
  }

  imageSub = transport.subscribe("/image_in", 1, callback);
  outputTopic = debug ?  "/debug_output" : "/robot/xdisplay";
  imagePub = transport.advertise(outputTopic, 1);

  while (node.ok())
  {
    if(debug){
      ROS_INFO("Publishing the image");
    }
    { // Check if the shared ptr (cv_ptr) was initialized
    if (initialized)
      // Output the modified images
      imagePub.publish(cv_ptr->toImageMsg());
    }
    token.update();
    ros::spinOnce();
  }
}
