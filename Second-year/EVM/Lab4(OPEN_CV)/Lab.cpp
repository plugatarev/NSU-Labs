#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

int main(int argc, char** argv)
{
    clock_t time;
    clock_t input_time;
    clock_t processing_time;
    clock_t output_time;

    clock_t input_counter = 0;
    clock_t processing_counter = 0;
    clock_t output_counter = 0;
    double counter = 0;

    Mat frame; //empty matrix for storing webcam frames
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        return -1;
    }
    time = clock();

    while (true)
    {
        //input
        input_time = clock();
        cap >> frame; //inserting the webcam frame inside frame matrix
        input_time = clock() - input_time;
        input_counter += input_time;

        processing_time = clock();

        Mat edges;
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        Canny(edges, edges, 30, 60, 5);
        
        Mat sub;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        subtract(frame, edges, sub);
        //imshow("Webcam", sub);

        //ROTATING
        double angle = 45;
        // getting the center coordinates of the image to create the 2D rotation matrix
        Point2f center((frame.cols - 1) / 2.0, (frame.rows - 1) / 2.0);
        //getting the rotation matrix
        Mat rotation_matix = getRotationMatrix2D(center, angle, 1.0);
        Mat rotated_webcam;
        // rotating the sub using rotation matrix
        warpAffine(sub, rotated_webcam, rotation_matix, edges.size());

        processing_time = clock() - processing_time;
        processing_counter += processing_time;

        output_time = clock();
        imshow("Webcam", rotated_webcam);
        output_time = clock() - output_time;
        output_counter += output_time;

        counter++;

        if (waitKey(33) == 27)
            break;
    }
    time = clock() - time - counter * 33;

    double FPS = counter / (((double)time / CLOCKS_PER_SEC));
    double avg_input_time = ((double)input_counter / CLOCKS_PER_SEC) / counter;
    double avg_processing_time = ((double)processing_counter / CLOCKS_PER_SEC) / counter;
    double avg_output_time = ((double)output_counter / CLOCKS_PER_SEC) / counter;
    double avg_full_time = avg_input_time + avg_processing_time + avg_output_time;

    cout << "Frame per second: " << FPS << endl;
    cout << "Average input time: " << avg_input_time << endl;
    cout << "Average processing time: " << avg_processing_time << endl;
    cout << "Average full time: " << avg_full_time << endl;

    return 0;
}
