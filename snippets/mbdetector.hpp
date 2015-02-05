#ifndef MBDETECTOR_HPP
#define MBDETECTOR_HPP

class MBDetector
{
public:
        MBDetector(const Size& mat_size);
        void detect(const Mat& mat, vector<pair<Rect, double> > &objs);
private:

};

#endif // MBDETECTOR_HPP
