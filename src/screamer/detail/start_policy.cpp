#include <stdexcept>
#include <string>
#include "screamer/detail/start_policy.h"

namespace screamer {
namespace detail {


// Helper function to convert the string to the StartPolicy enum
StartPolicy parse_start_policy(const std::string& policy) {
    if (policy == "strict") return StartPolicy::Strict;
    if (policy == "expanding") return StartPolicy::Expanding;
    if (policy == "zero") return StartPolicy::Zero;
    throw std::invalid_argument("Unknown start policy: [" + policy + "], valid values are: [strict, expanding, zero]");
}

} // namespace detail
} // namespace screamer
