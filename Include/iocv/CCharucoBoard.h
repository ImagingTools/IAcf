#pragma once


// STL includes
#include <vector>
#include <memory>
#include <map>
#include <algorithm>

// OpenCV includes
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>


namespace iocv
{


inline std::vector<std::map<char, float>> ptstovecmap(const std::vector<cv::Point2f>& pts)
{
	std::vector<std::map<char, float>> data(pts.size());
	std::transform(pts.begin(), pts.end(), data.begin(), [](const cv::Point2f& pt) {
		return std::map<char, float>{{'x', pt.x}, {'y', pt.y}};
	});
	return data;
}


inline std::vector<cv::Point2f> vecmaptopts(const std::vector<std::map<char, float>>& data)
{
	std::vector<cv::Point2f> pts(data.size());
	std::transform(data.begin(), data.end(), pts.begin(), [](const auto& item) {
		return cv::Point2f{item.at('x'), item.at('y')};
	});
	return pts;
}


struct CharucoDetectData
{
	std::vector<cv::Point2f> pix_corners;
	std::vector<cv::Point2f> obj_corners;
	std::vector<int> ids;
	float reprojection_error;
};


struct CharucoDetectionConfig
{
	size_t min_markers;
	float adathreshold_const;
	int subpix_maxcount;
	float subpix_epsilon;
	int subpix_winsize;
	float cell_size;
	float reprojection_error;
	bool reject_homography_outliers;
	bool subpixel_refinement;
};


class CCharucoBoard
{
	static constexpr auto dict_type_ = cv::aruco::DICT_4X4_250;
	cv::aruco::CharucoBoard board_;
	const float size_;

public:
	CCharucoBoard(cv::Size size, float sqlen, float mrklen);

	std::vector<cv::Point2f> border_corners(const std::vector<int>& ids) const;

	std::shared_ptr<CharucoDetectData> detect(
				cv::Mat img,
				const CharucoDetectionConfig& cfg,
				cv::Mat cammtx = cv::Mat(),
				cv::Mat dist = cv::Mat()) const;
	cv::Mat plot_on(cv::Mat img, const CharucoDetectData& data) const;
	cv::Mat board_image(cv::Size size) const;
	static std::shared_ptr<CCharucoBoard> quiss_template(float cellsize = 10.5f);
};


}


