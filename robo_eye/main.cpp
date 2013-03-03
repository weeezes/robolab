#if defined(__linux__) || defined(LINUX) || defined(__APPLE__) || defined(ANDROID)

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>

#include <stdio.h>

#include <string>
#include <vector>


#include "camera_controller.h"

using namespace std;
using namespace cv;

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
    public:
        CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
        {
            CV_Assert(!detector.empty());
        }

        void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects)
        {
            Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
        }

        virtual ~CascadeDetectorAdapter()
        {}

    private:
        CascadeDetectorAdapter();
        cv::Ptr<cv::CascadeClassifier> Detector;
};

int main(int , char** )
{
    namedWindow(WindowName);

    VideoCapture VideoStream(0);

    if (!VideoStream.isOpened())
    {
        printf("Error: Cannot open video stream from camera\n");
        return 1;
    }

    std::string cascadeFrontalfilename = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
    cv::Ptr<cv::CascadeClassifier> cascade = new cv::CascadeClassifier(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = new CascadeDetectorAdapter(cascade);

    cascade = new cv::CascadeClassifier(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = new CascadeDetectorAdapter(cascade);

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    CameraController controller("/dev/ttyACM0", 320, 240);
    
    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    Mat ReferenceFrame;
    Mat GrayFrame;
    vector<Rect> Faces;

    while(true)
    {
        VideoStream >> ReferenceFrame;
        cvtColor(ReferenceFrame, GrayFrame, COLOR_RGB2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

        printf("Looking for a face... ");
        if (Faces.size() > 0)
        {
            printf("Found one!\n");
            int face_x = Faces[0].x+Faces[0].width/2;
            int face_y = Faces[0].y+Faces[0].height/2;
            
            controller.turnCamera(face_x, face_y);
            circle(ReferenceFrame, cvPoint(face_x, face_y), 50, cvScalar(0, 238, 255), 5);
        }
        else
        {
            printf("No face found...\n");
        }

        imshow(WindowName, ReferenceFrame);

        if (cvWaitKey(30) >= 0)
            break;
    }

    Detector.stop();

    return 0;
}

#else

#include <stdio.h>
int main()
{
    printf("This sample works for UNIX or ANDROID only\n");
    return 0;
}

#endif
