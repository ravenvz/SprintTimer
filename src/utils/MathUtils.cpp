#include "MathUtils.h"


double MathUtils::percentage(unsigned chunk, unsigned total) {
    return total != 0 ? double(chunk) * 100 / total : 0;
}
