#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <cmath>


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
auto dot(const A& a, const B& b, INDEX begin, INDEX count) {
    using ValueTypeA = std::remove_reference_t<decltype(at(a, 0))>;
    using ValueTypeB = std::remove_reference_t<decltype(at(b, 0))>;
    using CommonType = std::common_type_t<ValueTypeA, ValueTypeB>;

    CommonType result = 0;
    for (INDEX i = begin; i < count; ++i) {
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
    return dot(a, b, static_cast<decltype(n)>(0), n);
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
    return sum(a, static_cast<decltype(n)>(0), n);
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
    return mean(a, static_cast<decltype(n)>(0), n);
}

template <typename A, typename INDEX>
auto l2(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        auto value = at(a, i);
        result += value * value;
    }
    return std::sqrt(result);
}

template <typename A>
auto l2(const A& a) {
    auto n = len(a);
    return l2(a, static_cast<decltype(n)>(0), n);
}

template <typename A, typename B, typename INDEX>
auto cosine_similarity(const A& a, const B& b, INDEX begin, INDEX count) {
    auto dot_product = dot(a, b, begin, count);
    auto l2_a = l2(a, begin, count);
    auto l2_b = l2(b, begin, count);
    return dot_product / (l2_a * l2_b);
}

template <typename A, typename B>
auto cosine_similarity(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return cosine_similarity(a, b, static_cast<decltype(n)>(0), n);
}

template <typename A, typename B, typename INDEX>
auto euclidean_distance(const A& a, const B& b, INDEX begin, INDEX count) {
    using ValueTypeA = std::remove_reference_t<decltype(at(a, 0))>;
    using ValueTypeB = std::remove_reference_t<decltype(at(b, 0))>;
    using CommonType = std::common_type_t<ValueTypeA, ValueTypeB>;

    CommonType result = 0;
    for (INDEX i = begin; i < count; ++i) {
        auto diff = static_cast<CommonType>(at(a, i)) - static_cast<CommonType>(at(b, i));
        result += diff * diff;
    }
    return std::sqrt(result);
}

template <typename A, typename B>
auto euclidean_distance(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return euclidean_distance(a, b, static_cast<decltype(n)>(0), n);
}

template <typename A, typename INDEX>
auto percentile(A& a, INDEX start, INDEX count, double p) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    std::vector<ValueType> copy(count);
    for (INDEX i = 0; i < count; ++i) {
        copy[i] = at(a, start + i);
    }
    std::sort(copy.begin(), copy.end());
    auto index = static_cast<INDEX>(p * count);
    return copy[index];
}

template <typename A>
auto percentile(A& a, double p) {
    auto n = len(a);
    return percentile(a, 0ul, n, p);
}   

template <typename A, typename INDEX>
auto median(A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    std::vector<ValueType> copy(count);
    for (INDEX i = 0; i < count; ++i) {
        copy[i] = at(a, start + i);
    }
    std::sort(copy.begin(), copy.end());
    if (count % 2 == 0) {
        return (copy[count / 2 - 1] + copy[count / 2]) / 2;
    } else {
        return copy[count / 2];
    }
}

template <typename A>
auto median(A& a) {
    auto n = len(a);
    return median(a, static_cast<decltype(n)>(0), n);
}

template <typename A>
void print(const A& a, std::ostream& os = std::cout) {
    os << "[";
    auto n = len(a);
    for (size_t i = 0; i < n; ++i) {
        os << at(a, i);
        if (i < n - 1) {
            os << ", ";
        }
    }
    os << "]";
}

template <typename A>
auto tojson(const A& a) {
    std::string result = "[";
    auto n = len(a);
    for (size_t i = 0; i < n; ++i) {
        result += std::to_string(at(a, i));
        if (i < n - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}



} // namespace ums