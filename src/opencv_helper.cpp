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

std::vector<std::string> operations;
std::map<std::string, cv::Mat> images;
std::string operation_parameter_delimiter = "|";

auto split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> vs;
    size_t pos
    { };

    for (size_t fd = 0; (fd = str.find(delim, pos)) != std::string::npos; pos = fd + delim.size())
        vs.emplace_back(str.data() + pos, str.data() + fd);

    vs.emplace_back(str.data() + pos, str.data() + str.size());
    return vs;
}

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

    images.insert(
    { output_name, img_out });

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

    images.insert(
    { output_name, img_out });
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

    images.insert(
    { output_name, img_out });
    return 1;
}

int opencv_blur(std::vector<std::string> parameters)
{
    std::string input_name;
    std::string output_name;
    cv::Size kernel_size;
    cv::Point anchor_point(-1, -1);
    int border_type = cv::BORDER_DEFAULT;

    long unsigned int par = 0;
    if (parameters.size() > par)
        input_name = parameters[par++];
    if (parameters.size() > par)
        output_name = parameters[par++];
    if (parameters.size() > par)
        kernel_size.width = stoi(parameters[par++]);
    if (parameters.size() > par)
        kernel_size.height = stoi(parameters[par++]);
    if (parameters.size() > par)
        anchor_point.x = stoi(parameters[par++]);
    if (parameters.size() > par)
        anchor_point.y = stoi(parameters[par++]);
    if (parameters.size() > par)
        border_type = stoi(parameters[par++]);

    cv::Mat img_out;
    cv::blur(images[input_name], img_out, kernel_size, anchor_point, border_type);

    images.insert(
    { output_name, img_out });
    return 1;
}

void opencv_imshow(std::vector<std::string> parameters)
{
    std::string window_name;
    std::string input_name;

    long unsigned int par = 0;
    if (parameters.size() > par)
        window_name = parameters[par++];
    if (parameters.size() > par)
        input_name = parameters[par++];

    imshow(window_name, images[input_name]);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
}

void operation_add(std::string command, std::vector<std::string> parameters)
{
    std::string operation = command;

    for (auto parameter : parameters)
    {
        operation += operation_parameter_delimiter;
        operation += parameter;
    }

    operations.push_back(operation);
}

void operations_process()
{
    int err = 0;

    for (auto operation : operations)
    {
        std::vector<std::string> parameters = split(operation, operation_parameter_delimiter);
        std::string command = parameters[0];
        parameters.erase(parameters.begin());

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
            err = opencv_blur(parameters);
        }

        if (command == "opencv_imshow")
        {
            opencv_imshow(parameters);
        }
    }
}

//int main()
//{
//    operation_add("opencv_imread",
//    { "/home/nn/Képek/ocv.jpg", "original", std::to_string(cv::IMREAD_COLOR) });
//
//    operation_add("opencv_blur",
//    { "original", "blured", "5", "5" });
//
//    operation_add("opencv_cvtColor",
//    { "blured", "recolored", std::to_string(cv::COLOR_RGB2GRAY) });
//
//    operation_add("opencv_threshold",
//    { "recolored", "threshold", "100", "255", std::to_string(cv::THRESH_BINARY) });
//
//    operation_add("opencv_imshow",
//    { "Display window", "threshold" });
//
//    operations_process();
//
//    return 0;
//}
