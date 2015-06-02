//
// Created by chri on 31.05.15.
//

#ifndef MIA_2015_UTILS_HPP
#define MIA_2015_UTILS_HPP

/**
 * Takes a matrix and returns a row vector containing the data of the matrix.
 *
 * @param in the matrix
 * @return a row vector
 */
template<typename T>
cv::Mat convert_to_row_vector(const cv::Mat& in)
{
    if (in.isContinuous())
    {
        return in.reshape(0, 1);
    }

    cv::Mat out;
    out.create(1, in.rows * in.cols, in.type());
    int i = 0;
    for (int row = 0; row < in.rows; ++row)
    {
        for (int col = 0; col < in.cols; ++col)
        {
            out.at<T>(0, i++) = in.at<T>(col, row);
        }
    }
    return out;
}

#endif //MIA_2015_UTILS_H
