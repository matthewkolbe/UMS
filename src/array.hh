#include <concepts>
#include <stdexcept>
#include <type_traits>


namespace ums {


template <typename T>
inline constexpr bool always_false = false;

// Define the `at` function that works with any Array-like type
template <typename ARRAY, typename INDEX>
decltype(auto) at(ARRAY& arr, INDEX index) {
    if constexpr (requires { arr[index]; }) {
        return arr[index];  // Use `operator[]` if available.
    } else if constexpr (requires { arr.at(index); }) {
        return arr.at(index);  // Use `at()` if available.
    } else if constexpr (requires { arr.coeff(index); }) {
        return arr.coeff(index);  // Use `coeff()` if available.
    } else if constexpr (requires { arr(index); }) {
        return arr(index);  // Use `operator()` if available.
    } else {
        static_assert(always_false<ARRAY>, "Type does not support valid indexing.");
    }
}

// Define the `len` function that works with any Array-like type
template <typename ARRAY>
auto len(const ARRAY& arr) {
    if constexpr (requires { arr.size(); }) {
        return arr.size();  // Use `size()` if available.
    } else if constexpr (requires { arr.length(); }) {
        return arr.length();  // Use `length()` if available.
    } else if constexpr (requires { arr.rows(); }) {
        return arr.rows();  // Use `rows()` if available.
    } else if constexpr (std::is_array_v<ARRAY>) {
        return sizeof(arr) / sizeof(arr[0]);  // Use `sizeof` for arrays.
    } else {
        static_assert(always_false<ARRAY>, "Type does not support length retrieval.");
    }
}

// Define the `dot` function for two Array-like types
template <typename A, typename B, typename INDEX>
auto dot(const A& a, const B& b, INDEX n) {
    using ValueTypeA = std::remove_reference_t<decltype(at(a, 0))>;
    using ValueTypeB = std::remove_reference_t<decltype(at(b, 0))>;
    using CommonType = std::common_type_t<ValueTypeA, ValueTypeB>;

    CommonType result = 0;
    for (INDEX i = 0; i < n; ++i) {
        result += static_cast<CommonType>(at(a, i)) * static_cast<CommonType>(at(b, i));
    }
    return result;
}

// Define the `dot` function for two Array-like types
template <typename A, typename B>
auto dot(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return dot(a, b, n);
}

// Define the `mean` function for an Array-like type
template <typename A, typename INDEX>
auto mean(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    auto result = sum(a, start, count);
    return result / count;
}

template <typename A>
auto mean(const A& a) {
    auto n = len(a);
    return mean(a, 0, n);
}

// Define the `sum` function for an Array-like type
template <typename A, typename INDEX>
auto sum(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        result += at(a, i);
    }
    return result;
}

template <typename A>
auto sum(const A& a) {
    auto n = len(a);
    return sum(a, 0, n);
}


} // namespace ums