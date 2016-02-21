#include "MathUtils.h"


double MathUtils::percentage(int chunk, int total) {
    return total != 0 ? double(chunk) * 100 / total : 0;
}
