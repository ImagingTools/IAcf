#include "iocv/perspectiveoutlierfilter.h"

// std
#include <iostream>
#include <algorithm>

// opencv
#include <opencv2/calib3d.hpp>


namespace iocv {

    template<typename T>
    void filter_by_mask(std::vector<T>& vec, const std::vector<bool>& mask) {
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [idx = 0, &mask](auto&) mutable { return !mask[idx++]; }), vec.end());
    }


    size_t PerspectiveOutlierFilter::runcv(std::vector<cv::Point2f>& src,
        std::vector<cv::Point2f>& trg,
        std::vector<int>& ids) const {
        const size_t initsize = src.size();
        if (initsize < minsize_) return 0;

        cv::Mat mask;
        cv::Mat homo = cv::findHomography(src, trg, mask, cv::LMEDS, maxoff_);
        std::vector<bool> bmask(mask.rows);

        for (size_t idx = 0; idx < bmask.size(); ++idx) {
            bmask[idx] = mask.at<int>(idx) != 0;
        }

        filter_by_mask(src, bmask);
        filter_by_mask(trg, bmask);
        filter_by_mask(ids, bmask);

        return initsize - src.size();
    }
}
