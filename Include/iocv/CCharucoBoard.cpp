#include <iocv/CCharucoBoard.h>


// ACF includes
#include "iocv/perspectiveoutlierfilter.h"

// OpenCV includes
#include "opencv2/imgproc.hpp"


using namespace cv::aruco;


namespace iocv
{


CCharucoBoard::CCharucoBoard(cv::Size size, float sqlen, float mrklen) :
    board_(size, sqlen, mrklen, getPredefinedDictionary(dict_type_)),
    size_(size.width* sqlen)
{
}


cv::Mat CCharucoBoard::board_image(cv::Size size) const {
    cv::Mat img;
    board_.generateImage(size, img);
    return img;
}


std::shared_ptr<CharucoDetectData> CCharucoBoard::detect(
    cv::Mat img,
    const CharucoDetectionConfig& cfg,
    cv::Mat cammtx,
    cv::Mat dist) const {
    DetectorParameters detpars;
    detpars.adaptiveThreshConstant = cfg.adathreshold_const;

    CharucoParameters chpars;
    chpars.cameraMatrix = cammtx;
    chpars.distCoeffs = dist;
    chpars.minMarkers = cfg.min_markers;
    chpars.tryRefineMarkers = true;

    RefineParameters rfpars;
    // rfpars.checkAllOrders
    // rfpars.errorCorrectionRate
    // rfpars.minRepDistance

    CharucoDetector detector(board_, chpars, detpars, rfpars);

    std::vector<cv::Point2f> corners;
    std::vector<int> ids;
    detector.detectBoard(img, corners, ids);
    if (corners.size() < 4) return nullptr;

    if (cfg.subpixel_refinement) {
        cv::TermCriteria term(
            cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER,
            cfg.subpix_maxcount, cfg.subpix_epsilon);
        const cv::Size win_size(cfg.subpix_winsize, cfg.subpix_winsize);
        const cv::Size zero_zone(-1, -1);
        cv::cornerSubPix(img, corners, win_size, zero_zone, term);
    }

    std::vector<cv::Point2f> bordcorns = border_corners(ids);
    float reprerr = 0.f;
    if (cfg.reject_homography_outliers) {
        iocv::PerspectiveOutlierFilter pof(4, cfg.reprojection_error, 3.f);
        auto nout = pof.runcv(corners, bordcorns, ids);
        //reprerr = reprerr0;
    }
    if (corners.size() < 4) return nullptr;

    CharucoDetectData data{ corners, bordcorns, ids, reprerr };

    return std::make_shared<CharucoDetectData>(data);
}


std::vector<cv::Point2f> CCharucoBoard::border_corners(const std::vector<int>& ids) const {
    std::vector<cv::Point2f> pts(ids.size());
    std::transform(ids.begin(), ids.end(), pts.begin(), [this](auto idx) {
        cv::Point3f pt = board_.getChessboardCorners().at(idx);
        return cv::Point2f{
            pt.x = 0.5 * size_ - pt.x,
            pt.y = pt.y - 0.5 * size_
        };
        });
    return pts;
}


cv::Mat CCharucoBoard::plot_on(cv::Mat img, const CharucoDetectData& data) const {
    cv::Mat oimg = img.clone();
    if (oimg.channels() == 1) cv::cvtColor(oimg, oimg, cv::COLOR_GRAY2BGR);
    // drawDetectedMarkers(oimg, data.pix_corners);
    drawDetectedCornersCharuco(oimg, data.pix_corners, data.ids, cv::Scalar(0, 255, 0));
    return oimg;
}


std::shared_ptr<CCharucoBoard> CCharucoBoard::quiss_template(float cellsize) {
    const cv::Size size(10, 10);
    const float sqlen = cellsize;
    const float mrklen = sqlen / 4 * 3;
    return std::make_shared<CCharucoBoard>(size, sqlen, mrklen);
}

}