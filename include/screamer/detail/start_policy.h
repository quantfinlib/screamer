#ifndef SCREAMER_DETAIL_START_POLICY_H
#define SCREAMER_DETAIL_START_POLICY_H

#include <string>

namespace screamer {
namespace detail {

enum class StartPolicy {
    Strict,
    Expanding,
    Zero
};

// FOrward declarations
StartPolicy parse_start_policy(const std::string& policy);

} // namespace detail
} // namespace screamer
#endif