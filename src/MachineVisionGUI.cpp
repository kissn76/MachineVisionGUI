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

std::vector<std::string> operations;
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

int opencv_imread(cv::Mat *output_image, std::vector<std::string> parameters)
{
    std::string image_path;
    int imread_mode;

    if (parameters.size() > 0)
        image_path = parameters[0];
    if (parameters.size() > 1)
        imread_mode = stoi(parameters[1]);

    cv::Mat img = cv::imread(image_path, imread_mode);
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 0;
    }

    *output_image = img;
    return 1;
}

int opencv_threshold(cv::Mat *output_image, cv::Mat *input_image, std::vector<std::string> parameters)
{
    double threshold_value;
    double max_binary_value;
    int threshold_type;

    if (parameters.size() > 0)
        threshold_value = stod(parameters[0]);
    if (parameters.size() > 1)
        max_binary_value = stod(parameters[1]);
    if (parameters.size() > 2)
        threshold_type = stoi(parameters[2]);

    cv::Mat img;
    cv::threshold(*input_image, img, threshold_value, max_binary_value, threshold_type);

    *output_image = img;
    return 1;
}

int opencv_cvtColor(cv::Mat *output_image, cv::Mat *input_image, std::vector<std::string> parameters)
{
    int color_conversion_code;
    int output_channels_number = 0;

    if (parameters.size() > 0)
        color_conversion_code = stoi(parameters[0]);
    if (parameters.size() > 1)
        output_channels_number = stoi(parameters[1]);

    cv::Mat img;
    cv::cvtColor(*input_image, img, color_conversion_code, output_channels_number);

    *output_image = img;
    return 1;
}

int opencv_blur(cv::Mat *output_image, cv::Mat *input_image, std::vector<std::string> parameters)
{
    cv::Size kernel_size;
    cv::Point anchor_point(-1, -1);
    int border_type = cv::BORDER_DEFAULT;

    if (parameters.size() > 0)
        kernel_size.width = stoi(parameters[0]);
    if (parameters.size() > 1)
        kernel_size.height = stoi(parameters[1]);
    if (parameters.size() > 2)
        anchor_point.x = stoi(parameters[2]);
    if (parameters.size() > 3)
        anchor_point.y = stoi(parameters[3]);
    if (parameters.size() > 4)
        border_type = stoi(parameters[4]);

    cv::Mat img;
    cv::blur(*input_image, img, kernel_size, anchor_point, border_type);

    *output_image = img;
    return 1;
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
    cv::Mat img;
    int err = 0;

    for (auto operation : operations)
    {
        std::vector<std::string> tags = split(operation, operation_parameter_delimiter);
        std::string command = tags[0];
        tags.erase(tags.begin());

        if (command == "opencv_imread")
        {
            err = opencv_imread(&img, tags);
        }

        if (command == "opencv_threshold")
        {
            err = opencv_threshold(&img, &img, tags);
        }

        if (command == "opencv_cvtColor")
        {
            err = opencv_cvtColor(&img, &img, tags);
        }

        if (command == "opencv_blur")
        {
            err = opencv_blur(&img, &img, tags);
        }
    }

    imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
}

int main()
{
    operation_add("opencv_imread",
    { "/home/nn/Képek/ocv.jpg", std::to_string(cv::IMREAD_COLOR) });

    operation_add("opencv_blur",
    { "5", "5" });

    operation_add("opencv_cvtColor",
    { std::to_string(cv::COLOR_RGB2GRAY) });

    operation_add("opencv_threshold",
    { "100", "255", std::to_string(cv::THRESH_BINARY) });

    operations_process();

    return 0;
}
