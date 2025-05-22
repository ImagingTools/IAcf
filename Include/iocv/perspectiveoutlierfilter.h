#pragma once


// std
#include <vector>

// opencv
#include <opencv2/core.hpp>


namespace iocv {
    /**
     * @brief Outlier filter for a perspective transformation
     *
     */
    class PerspectiveOutlierFilter {
        const size_t minsize_;
        const float maxoff_;
        const float nsigma_;

    public:
        /**
         * @brief Construct a new Perspective Outlier Filter object
         *
         * @param minsize minimal output size of the point set (currently not used)
         * @param maxoff maximal allowed offset in the xy space, mm
         * @param nsigma number of standard deviations for the cutting threshold
         */
        PerspectiveOutlierFilter(size_t minsize, float maxoff, float nsigma) :
            minsize_(minsize), maxoff_(maxoff), nsigma_(nsigma) {}

        /**
         * @brief Filter out (in place) points that do not fit in a perspective transformation
         *
         * @param src pixel points
         * @param trg corresponding space points with the same z coordinate
         * @return size_t number of filtered out points
         */


        size_t runcv(std::vector<cv::Point2f>& src,
            std::vector<cv::Point2f>& trg,
            std::vector<int>& ids) const;
    };
}