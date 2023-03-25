#include "common.h"
namespace Dragon{

    class DiscretePdf{
    private:
        vector<float>  cdf_;
        float sum_, normalization_;
        bool normalized_;
    public:
        void Clear();
        void Normalize();
        void Append(float pdf_value);
        uint Sample(float sample_value, float & pdf) const;
        uint sampleReuse(float &sampleValue) const;
        uint SampleReuse(float &sampleValue, float &pdf) const;
        float normalization()const {return normalization_;}
        float operator[](uint index)const;
    };
}
