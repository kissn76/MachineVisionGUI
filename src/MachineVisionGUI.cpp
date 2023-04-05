//============================================================================
// Name        : MachineVisionGUI.cpp
// Author      : Norbert Kiss
// Version     :
// Copyright   : MIT license
// Description : Machine Vision GUI in C++, Ansi-style
//============================================================================

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <map>

std::vector<std::pair<std::string, int>> operations;
std::map<std::string, cv::Mat> images;

int opencv_imread(std::vector<std::string> parameters)
{
    std::string output_name;
    std::string image_path;
    int imread_mode;

    long unsigned int par = 0;
    if (parameters.size() > par)
        image_path = parameters[par++];
    if (parameters.size() > par)
        output_name = parameters[par++];
    if (parameters.size() > par)
        imread_mode = stoi(parameters[par++]);

    cv::Mat img_out = cv::imread(image_path, imread_mode);
    if (img_out.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 0;
    }

    images.insert({output_name, img_out});

    return 1;
}

int opencv_threshold(std::vector<std::string> parameters)
{
    std::string input_name;
    std::string output_name;
    double threshold_value;
    double max_binary_value;
    int threshold_type;

    long unsigned int par = 0;
    if (parameters.size() > par)
        input_name = parameters[par++];
    if (parameters.size() > par)
        output_name = parameters[par++];
    if (parameters.size() > par)
        threshold_value = stod(parameters[par++]);
    if (parameters.size() > par)
        max_binary_value = stod(parameters[par++]);
    if (parameters.size() > par)
        threshold_type = stoi(parameters[par++]);

    cv::Mat img_out;
    cv::threshold(images[input_name], img_out, threshold_value, max_binary_value, threshold_type);

    images.insert({output_name, img_out});
    return 1;
}

int opencv_cvtColor(std::vector<std::string> parameters)
{
    std::string input_name;
    std::string output_name;
    int color_conversion_code;
    int output_channels_number = 0;

    long unsigned int par = 0;
    if (parameters.size() > par)
        input_name = parameters[par++];
    if (parameters.size() > par)
        output_name = parameters[par++];
    if (parameters.size() > par)
        color_conversion_code = stoi(parameters[par++]);
    if (parameters.size() > par)
        output_channels_number = stoi(parameters[par++]);

    cv::Mat img_out;
    cv::cvtColor(images[input_name], img_out, color_conversion_code, output_channels_number);

    images.insert({output_name, img_out});
    return 1;
}

int opencv_blur_counter = 0;
struct opencv_blur_parameters
{
    std::string input_name;
    std::string output_name;
    cv::Size kernel_size;
    cv::Point anchor_point;
    int border_type;
};
std::map<int, opencv_blur_parameters> opencv_blur_parameters_list;
int opencv_blur(cv::Mat image_input, cv::Mat image_output, opencv_blur_parameters parameters)
{
    cv::blur(image_input, image_output, parameters.kernel_size, parameters.anchor_point, parameters.border_type);

    return 1;
}

void opencv_imshow(std::string window_name, cv::Mat image)
{

    imshow(window_name, image);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if (k == 'q')
    {
        exit(0);
    }
}

void operation_add(std::string command, std::map<std::string, std::string> parameters)
{
    int counter = 0;

    if (command == "opencv_blur")
    {
        counter = opencv_blur_counter++;

        opencv_blur_parameters param;
        param.kernel_size.width = 3;
        param.kernel_size.height = 3;
        param.anchor_point.x = -1;
        param.anchor_point.y = -1;
        param.border_type = cv::BORDER_DEFAULT;

        std::map<std::string, std::string>::iterator it;

        it = parameters.find("input_name");
        if (it != parameters.end())
            param.input_name = parameters["input_name"];

        it = parameters.find("output_name");
        if (it != parameters.end())
            param.output_name = parameters["output_name"];

        it = parameters.find("kernel_size_width");
        if (it != parameters.end())
            param.kernel_size.width = stoi(parameters["kernel_size_width"]);

        it = parameters.find("kernel_size_width");
        if (it != parameters.end())
            param.kernel_size.width = stoi(parameters["kernel_size_width"]);

        it = parameters.find("kernel_size_height");
        if (it != parameters.end())
            param.kernel_size.height = stoi(parameters["kernel_size_height"]);

        it = parameters.find("anchor_point_x");
        if (it != parameters.end())
            param.anchor_point.x = stoi(parameters["anchor_point_x"]);

        it = parameters.find("anchor_point_y");
        if (it != parameters.end())
            param.anchor_point.y = stoi(parameters["anchor_point_y"]);

        it = parameters.find("border_type");
        if (it != parameters.end())
            param.border_type = stoi(parameters["border_type"]);

        opencv_blur_parameters_list.insert(std::pair<int, opencv_blur_parameters>(counter, param));
    }

    operations.push_back(std::pair<std::string, int>(command, counter));
}

void operations_process()
{
    int err = 0;

    while (true)
    {
        for (auto operation : operations)
        {
            std::string command = operation.first;
            int counter = operation.second;

            if (command == "opencv_imread")
            {
                err = opencv_imread(parameters);
            }

            if (command == "opencv_threshold")
            {
                err = opencv_threshold(parameters);
            }

            if (command == "opencv_cvtColor")
            {
                err = opencv_cvtColor(parameters);
            }

            if (command == "opencv_blur")
            {
                opencv_blur_parameters parameters = opencv_blur_parameters_list[counter]

                cv::Mat image_output;
                err = opencv_blur(images[parameters.input_name], image_output, parameters);

                images.insert(std::pair<std::string, cv::Mat>(parameters.output_name, image_output));
            }

            if (command == "opencv_imshow")
            {
                std::string window_name;
                std::string input_name;

                long unsigned int par = 0;
                if (parameters.size() > par)
                    window_name = parameters[par++];
                if (parameters.size() > par)
                    input_name = parameters[par++];

                opencv_imshow(window_name, images[input_name]);
            }
        }
    }
}

int main()
{
    operation_add("opencv_imread", {"/home/nn/Képek/ocv.jpg", "original", std::to_string(cv::IMREAD_COLOR)});

    std::map<std::string, std::string> parameters;
    parameters.insert({"input_name", "original"});
    parameters.insert({"output_name", "blured"});
    parameters.insert({"kernel_size_width", "5"});
    parameters.insert({"kernel_size_height", "5"});
    operation_add("opencv_blur", parameters);

    operation_add("opencv_cvtColor", {"blured", "recolored", std::to_string(cv::COLOR_RGB2GRAY)});

    operation_add("opencv_threshold", {"recolored", "threshold", "100", "255", std::to_string(cv::THRESH_BINARY)});

    operation_add("opencv_imshow", {"Display window", "threshold"});

    operations_process();

    return 0;
}
