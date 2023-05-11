#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <iterator>
#include <nlohmann/json.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using json = nlohmann::json;

cv::Mat add_row_noise(cv::Mat, double, double);

class ParseCondition {
public:
	ParseCondition(int n):
		n_(n) {
			std::vector<int> gain_list_(n);
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
			gain_list_.push_back(x);
		}
	}

	void print() {
		std::cout << "[DEBUG] json parse :";
		for (int i = 0; i < n_; i++) {
			std::cout << gain_list_[i] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<int> getGainList() { return gain_list_; }

private:
	std::vector<int> gain_list_;
	int n_;
	json jobj_;


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
	cv::Mat img = cv::Mat::zeros(height, width, CV_16U);
	img = add_row_noise(img, 100.0, 500.0);

	cv::imwrite("output.png", img);
	//cv::imshow("", noisy_img);
	//cv::waitKey(0);


	//cv::destroyAllWindows();

	return 0;
}

cv::Mat add_row_noise(cv::Mat img, double stddev, double mean) {
	int height = img.rows;
	int width = img.cols;

	cv::Mat out = cv::Mat::zeros(height, width, CV_16U);

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

