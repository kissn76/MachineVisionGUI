//============================================================================
// Name        : MachineVisionGUI.cpp
// Author      : Norbert Kiss
// Version     :
// Copyright   : MIT license
// Description : Machine Vision GUI in C++, Ansi-style
//============================================================================

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

std::vector<std::string> operations;
std::string operation_parameter_delimiter = "|";

struct opencv_imread_paramerters
{
    std::string image_path;
    int imread_mode;
};

int opencv_imread(cv::Mat *output_image, opencv_imread_paramerters parameters)
{
    cv::Mat img = cv::imread(parameters.image_path, parameters.imread_mode);
    if (img.empty())
    {
        std::cout << "Could not read the image: " << parameters.image_path << std::endl;
        return 0;
    }

    *output_image = img;
    return 1;
}

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

        if (command == "opencv_imread")
        {
            opencv_imread_paramerters opencv_imread_paramerters;
            if (tags.size() > 1)
                opencv_imread_paramerters.image_path = tags[1];
            if (tags.size() > 2)
                opencv_imread_paramerters.imread_mode = stoi(tags[2]);

            err = opencv_imread(&img, opencv_imread_paramerters);
        }
    }

    imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
}

int main()
{
    operation_add("opencv_imread",
    { "/home/nn/Képek/ocv.jpg", std::to_string(cv::IMREAD_COLOR) });

    operations_process();

    return 0;
}
