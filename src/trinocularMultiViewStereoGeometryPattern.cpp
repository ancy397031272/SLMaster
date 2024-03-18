#include "trinocularMultiViewStereoGeometryPattern.h"

#include <opencv2/structured_light.hpp>

using namespace cv;

#ifdef WITH_CUDASTRUCTUREDLIGHT_MODULE
#include <opencv2/cudastructuredlight.hpp>
#endif

namespace slmaster {

TrinocularMultiViewStereoGeometryPattern::TrinocularMultiViewStereoGeometryPattern() {
    params_.reset(new TrinoPatternParams());
}

bool TrinocularMultiViewStereoGeometryPattern::generate(std::vector<cv::Mat>& imgs) const {
#ifdef WITH_CUDASTRUCTUREDLIGHT_MODULE
    auto trinoPatternParams = static_cast<TrinoPatternParams*>(params_.get());

    cv::cuda::MultiViewStereoGeometryPattern::Params params;
    params.shiftTime = trinoPatternParams->shiftTime_;
    params.height = trinoPatternParams->height_;
    params.width = trinoPatternParams->width_;
    params.nbrOfPeriods = trinoPatternParams->cycles_;
    params.horizontal = trinoPatternParams->horizontal_;

    return cuda::MultiViewStereoGeometryPattern::create(params)->generate(imgs);
#else
    return false;
#endif
}

bool TrinocularMultiViewStereoGeometryPattern::decode( const std::vector< std::vector<cv::Mat> >& patternImages, cv::Mat& depthMap, const bool isGpu) const {
    CV_Assert(patternImages.size() == 3);

    auto trinoPatternParams = static_cast<TrinoPatternParams*>(params_.get());

#ifdef WITH_CUDASTRUCTUREDLIGHT_MODULE
    if(isGpu) {
        cv::cuda::MultiViewStereoGeometryPattern::Params params;
        params.shiftTime = trinoPatternParams->shiftTime_;
        params.confidenceThreshold = trinoPatternParams->confidenceThreshold_;
        params.costMaxDiff = trinoPatternParams->costMaxDiff_;
        params.costMinDiff = trinoPatternParams->costMinDiff_;
        params.maxCost = trinoPatternParams->maxCost_;
        params.height = trinoPatternParams->height_;
        params.width = trinoPatternParams->width_;
        params.minDepth = trinoPatternParams->minDepth_;
        params.maxDepth = trinoPatternParams->maxDepth_;
        params.nbrOfPeriods = trinoPatternParams->cycles_;
        params.horizontal = trinoPatternParams->horizontal_;
        params.M1 = trinoPatternParams->M1_;
        params.M2 = trinoPatternParams->M2_;
        params.M3 = trinoPatternParams->M3_;
        params.R12 = trinoPatternParams->R12_;
        params.T12 = trinoPatternParams->T12_;
        params.R13 = trinoPatternParams->R13_;
        params.T13 = trinoPatternParams->T13_;
        params.PL1 = trinoPatternParams->PL1_;
        params.PR2 = trinoPatternParams->PR2_;
        params.PR4 = trinoPatternParams->PR4_;
        params.refUnwrappedMap = trinoPatternParams->refUnwrappedMap_;

        auto pattern = cv::cuda::MultiViewStereoGeometryPattern::create(params);
        return pattern->decode(patternImages, depthMap, cuda::MULTI_VIEW_STEREO_GEOMETRY);
    }
#else
    std::clog << "TrinocularMultiViewStereoGeometryPattern is only used for GPU!" << std::endl;
#endif

    return false;
}
}