#include "MathUtils.h"


double MathUtils::percentage(double chunk, double total) {
    return total > 0 ? double(chunk) * 100 / total : 0;
}
