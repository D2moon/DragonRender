#include "dragon/core/sampler_class.h"
#include "ext/pcg32.h"

namespace Dragon {

    class PcgRandom : public Sampler {
    public:
        pcg32 random_;
        std:: mutex m;

    public:
        explicit PcgRandom(const PropertyList &props) {

        }

        float Next1D() override {
            //std::scoped_lock lock(m);
            return random_.nextFloat();
        }

        PcgRandom() = default;

        Vector2f Next2D() override {
            //std::scoped_lock lock(m);
            return {random_.nextFloat(), random_.nextFloat()};
        }

    };

    DRAGON_REGISTER_CLASS(PcgRandom, "pcg_random");

}

