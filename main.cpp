#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <iterator>
#include <nlohmann/json.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using json = nlohmann::json;

cv::Mat AddRowRandomNoise(cv::Mat, double, double);
cv::Mat AddRowArchNoise(cv::Mat);

class ParseCondition {
 private:
	int n_;
	json jobj_;
	int bit_;
	std::vector<double> gain_vec_;
	std::vector<double> sigma_row_vec_;
	std::vector<double> sigma_all_vec_;
	std::tuple<int, int> point_left_top_;
	std::tuple<int, int> point_right_bottom_;
	std::vector<double> profile_x_;
	std::vector<double> profile_y_;
	std::vector<double> conv_vec_;

 public:
	ParseCondition(int n): n_(n) {
			std::vector<double> gain_vec_(n);
	}

	void parse(std::string filename) {
		std::ifstream ifs(filename);
		if (ifs.fail()) {
			std::cerr << "[Error main.cpp] do not read `settings.json`" << std::endl;
		}

		std::istreambuf_iterator<char> it(ifs);
		std::istreambuf_iterator<char> last;
		std::string json_str(it, last);
		json jobj_ = json::parse(json_str);
		for (auto x : jobj_["gain"]) {
			gain_vec_.push_back(x);
		}
	}

	void print() {
		std::cout << "[DEBUG] json parse :";
		for (int i = 0; i < n_; i++) {
			std::cout << gain_vec_[i] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<double> getGainList() { return gain_vec_; }


};

int main(int argc, char *argv[]) {

	if(argc < 2) {
		std::cerr << "[Error main.cpp] please, specify `settings.json` for argument" << std::endl;
		return -1;
	} else if(argc > 2) {
		std::cerr << "[Error main.cpp] too many arguments" << std::endl;
		return -1;
	}

	std::string filename = argv[1];

	ParseCondition cond(4);
	cond.parse(filename);
	cond.print();

	int height = 480;
	int width = 640;
	int blacklevel = 500
	cv::Mat img = cv::Mat::zeros(height, width, CV_16UC1);
	img = img + blacklevel;
	img = AddRowRandomNoise(img, 100.0, 0.0);

	cv::imwrite("output.png", img);
	//cv::imshow("", noisy_img);
	//cv::waitKey(0);


	//cv::destroyAllWindows();

	return 0;
}

cv::Mat AddRowRandomNoise(cv::Mat img, double stddev, double mean) {
	int height = img.rows;
	int width = img.cols;

	cv::Mat out = cv::Mat::zeros(height, width, CV_16UC1);

	std::random_device seed;
	std::mt19937 engine(seed());

	std::normal_distribution<> dist(mean, stddev);

	std::vector<double> row_noise_list(height);
	for (int y = 0; y < height; y++) {
		row_noise_list[y] = dist(engine);
	}

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			double pix_value = img.at<uint16_t>(y, x) + row_noise_list[y];
			if (pix_value > 1023) {
				pix_value = 1023;
			} else if (pix_value < 0) {
				pix_value = 0;
			}
			out.at<uint16_t>(y, x) = (uint16_t)(pix_value);
		}
	}

	return out;
}

cv::Mat AddRowArchNoise(cv::Mat img) {
	int height = img.rows;
	int width = img.cols;

	cv::Mat out = cv::Mat::zeros(height, width, CV_16UC1);
	std::tuple<uint16_t, uint16_t> left_bottom = (100, 100);
	std::tuple<uint16_t, uint16_t> left_bottom = (100, 100);
	return out;
}
