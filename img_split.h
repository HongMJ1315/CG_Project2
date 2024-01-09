#ifndef IMG_SPLIT_H
#define IMG_SPLIT_H


#include <opencv2/opencv.hpp>
#include <iostream>
class ImageDivider{
private:
    cv::Mat img;
public:
    ImageDivider(const std::string &imgPath){
        img = cv::imread(imgPath, cv::IMREAD_UNCHANGED);
        if(img.empty()){
            std::cout << "Could not open or find the image" << std::endl;
        }
    }

    std::vector<cv::Mat> divide(int horizontal, int vertical){
        std::vector<cv::Mat> dividedImages;
        int width = img.cols / horizontal;
        int height = img.rows / vertical;
        for(int i = 0; i < vertical; i++){
            for(int j = 0; j < horizontal; j++){
                std::cout << j * width << " " << i * height << " " << width << " " << height << std::endl;
                cv::Mat dividedImage = img(cv::Rect(j * width, i * height, width, height));
                dividedImages.push_back(dividedImage);
            }
        }
        return dividedImages;
    }
};

#endif