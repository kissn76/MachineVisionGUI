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

int opencv_imread_counter = 0;
struct opencv_imread_parameters
{
        std::string image_path;
        std::string output_name;
        int imread_mode;
};
std::map<int, opencv_imread_parameters> opencv_imread_parameters_list;
int opencv_imread(cv::Mat image_output, opencv_imread_parameters parameters)
{
    image_output = cv::imread(parameters.image_path, parameters.imread_mode);
    if (image_output.empty())
    {
        std::cout << "Could not read the image: " << parameters.image_path << std::endl;
        return 0;
    }

    return 1;
}

int opencv_threshold_counter = 0;
struct opencv_threshold_parameters
{
        std::string input_name;
        std::string output_name;
        double threshold_value;
        double max_binary_value;
        int threshold_type;
};
std::map<int, opencv_threshold_parameters> opencv_threshold_parameters_list;
int opencv_threshold(cv::Mat image_input, cv::Mat image_output, opencv_threshold_parameters parameters)
{
    cv::threshold(image_input, image_output, parameters.threshold_value, parameters.max_binary_value, parameters.threshold_type);

    return 1;
}

int opencv_cvtColor_counter = 0;
struct opencv_cvtColor_parameters
{
        std::string input_name;
        std::string output_name;
        int color_conversion_code;
        int output_channels_number;
};
std::map<int, opencv_cvtColor_parameters> opencv_cvtColor_parameters_list;
int opencv_cvtColor(cv::Mat image_input, cv::Mat image_output, opencv_cvtColor_parameters parameters)
{
    cv::cvtColor(image_input, image_output, parameters.color_conversion_code, parameters.output_channels_number);

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

int opencv_imshow_counter = 0;
struct opencv_imshow_parameters
{
        std::string input_name;
        std::string window_name;
};
std::map<int, opencv_imshow_parameters> opencv_imshow_parameters_list;
void opencv_imshow(cv::Mat image_input, opencv_imshow_parameters parameters)
{
    imshow(parameters.window_name, image_input);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if (k == 'q')
    {
        exit(0);
    }
}

void operation_add(std::string command, std::map<std::string, std::string> parameters)
{
    int counter = 0;

    if (command == "opencv_imread")
    {
        counter = opencv_imread_counter++;

        opencv_imread_parameters param;
        param.imread_mode = cv::IMREAD_UNCHANGED;

        std::map<std::string, std::string>::iterator it;

        it = parameters.find("image_path");
        if (it != parameters.end())
            param.image_path = parameters["image_path"];

        it = parameters.find("output_name");
        if (it != parameters.end())
            param.output_name = parameters["output_name"];

        it = parameters.find("imread_mode");
        if (it != parameters.end())
            param.imread_mode = stoi(parameters["imread_mode"]);

        opencv_imread_parameters_list.insert({counter, param});
    }

    if (command == "opencv_threshold")
    {
        counter = opencv_threshold_counter++;

        opencv_threshold_parameters param;
        param.threshold_value = 50;
        param.max_binary_value = 255;
        param.threshold_type = cv::THRESH_BINARY;

        std::map<std::string, std::string>::iterator it;

        it = parameters.find("input_name");
        if (it != parameters.end())
            param.input_name = parameters["input_name"];

        it = parameters.find("output_name");
        if (it != parameters.end())
            param.output_name = parameters["output_name"];

        it = parameters.find("threshold_value");
        if (it != parameters.end())
            param.threshold_value = stod(parameters["threshold_value"]);

        it = parameters.find("max_binary_value");
        if (it != parameters.end())
            param.max_binary_value = stod(parameters["max_binary_value"]);

        it = parameters.find("threshold_type");
        if (it != parameters.end())
            param.threshold_type = stoi(parameters["threshold_type"]);

        opencv_threshold_parameters_list.insert({counter, param});
    }

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

        opencv_blur_parameters_list.insert({counter, param});
    }

    if (command == "opencv_cvtColor")
    {
        counter = opencv_cvtColor_counter++;

        opencv_cvtColor_parameters param;
        param.output_channels_number = 0;

        std::map<std::string, std::string>::iterator it;

        it = parameters.find("input_name");
        if (it != parameters.end())
            param.input_name = parameters["input_name"];

        it = parameters.find("output_name");
        if (it != parameters.end())
            param.output_name = parameters["output_name"];

        it = parameters.find("color_conversion_code");
        if (it != parameters.end())
            param.color_conversion_code = stoi(parameters["color_conversion_code"]);

        it = parameters.find("output_channels_number");
        if (it != parameters.end())
            param.output_channels_number = stoi(parameters["output_channels_number"]);

        opencv_cvtColor_parameters_list.insert({counter, param});
    }

    if (command == "opencv_imshow")
    {
        counter = opencv_imshow_counter++;

        opencv_imshow_parameters param;
        param.window_name = "Display";

        std::map<std::string, std::string>::iterator it;

        it = parameters.find("window_name");
        if (it != parameters.end())
            param.window_name = parameters["window_name"];

        opencv_imshow_parameters_list.insert({counter, param});
    }

    operations.push_back({command, counter});
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
                opencv_imread_parameters parameters = opencv_imread_parameters_list[counter];

                cv::Mat image_output;
                err = opencv_imread(image_output, parameters);

                images.insert({parameters.output_name, image_output});
            }

            if (command == "opencv_threshold")
            {
                opencv_threshold_parameters parameters = opencv_threshold_parameters_list[counter];

                cv::Mat image_output;
                err = opencv_threshold(images[parameters.input_name], image_output, parameters);

                images.insert({parameters.output_name, image_output});
            }

            if (command == "opencv_cvtColor")
            {
                opencv_cvtColor_parameters parameters = opencv_cvtColor_parameters_list[counter];

                cv::Mat image_output;
                err = opencv_cvtColor(images[parameters.input_name], image_output, parameters);

                images.insert({parameters.output_name, image_output});
            }

            if (command == "opencv_blur")
            {
                opencv_blur_parameters parameters = opencv_blur_parameters_list[counter];

                cv::Mat image_output;
                err = opencv_blur(images[parameters.input_name], image_output, parameters);

                images.insert({parameters.output_name, image_output});
            }

            if (command == "opencv_imshow")
            {
                opencv_imshow_parameters parameters = opencv_imshow_parameters_list[counter];

                opencv_imshow(images[parameters.input_name], parameters);
            }
        }
    }
}

int main()
{
    std::map<std::string, std::string> parameters;
    parameters.insert({"image_path", "/home/nn/Képek/ocv.jpg"});
    parameters.insert({"output_name", "original"});
    parameters.insert({"imread_mode", std::to_string(cv::IMREAD_COLOR)});
    operation_add("opencv_imread", parameters);

    parameters.clear();
    parameters.insert({"input_name", "original"});
    parameters.insert({"output_name", "blured"});
    parameters.insert({"kernel_size_width", "5"});
    parameters.insert({"kernel_size_height", "5"});
    operation_add("opencv_blur", parameters);

    parameters.clear();
    parameters.insert({"input_name", "blured"});
    parameters.insert({"output_name", "recolored"});
    parameters.insert({"color_conversion_code", std::to_string(cv::COLOR_RGB2GRAY)});
    operation_add("opencv_cvtColor", parameters);

    parameters.clear();
    parameters.insert({"input_name", "recolored"});
    parameters.insert({"output_name", "threshold"});
    parameters.insert({"threshold_value", "100"});
    parameters.insert({"max_binary_value", "255"});
    parameters.insert({"threshold_type", std::to_string(cv::THRESH_BINARY)});
    operation_add("opencv_threshold", parameters);

    parameters.clear();
    parameters.insert({"input_name", "threshold"});
    parameters.insert({"window_name", "Display window"});
    operation_add("opencv_imshow", parameters);

    operations_process();

    return 0;
}
