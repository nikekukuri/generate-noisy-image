#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <iterator>
#include <nlohmann/json.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using json = nlohmann::json;

cv::Mat add_row_noise(cv::Mat img, double stddev, double mean) {
	int height = img.rows;
	int width = img.cols;

	cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);

	std::random_device seed;
	std::mt19937 engine(seed());

	std::normal_distribution<> dist(mean, stddev);

	std::vector<double> row_noise_list(height);
	for (int i = 0; i < height; i++) {
		row_noise_list[i] = dist(engine);
	}

	int i = 0;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			out.at<char>(y, x) = (char)(img.at<char>(y, x) + row_noise_list[i]);
		}
		i++;
	}

	return out;
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		std::cerr << "[Error main.cpp] please, specify `settings.json` for argument" << std::endl;
		return -1;
	} else if(argc > 2) {
		std::cerr << "[Error main.cpp] too many arguments" << std::endl;
		return -1;
	}

	std::string filename = argv[1];
	std::ifstream ifs(filename);

	if (ifs.fail()) {
		std::cerr << "[Error main.cpp] do not read `settings.json`" << std::endl;
		return -1;
	}

	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string json_str(it, last);
	json jobj = json::parse(json_str);

	int height = 1000;
	int width = 1000;
	cv::Mat img = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat noisy_img = add_row_noise(img, 10.0, 100.0);

	cv::imshow("", noisy_img);
	cv::waitKey(0);

	//cv::imwrite("", out);

	cv::destroyAllWindows();

	return 0;
}
