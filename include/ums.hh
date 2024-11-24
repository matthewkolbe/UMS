#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>

namespace ums {

template <typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::common_with<T>;
    { a - b } -> std::common_with<T>;
    { a * b } -> std::common_with<T>;
    { a / b } -> std::common_with<T>;
};

template <typename T>
inline constexpr bool always_false = false;

// Define the `data` function that works with various container types and smart pointers
template <typename T>
decltype(auto) data(T& object) {
    if constexpr (requires { object.data(); }) {
        return object.data();  // Use `data()` if available (e.g., std::vector, std::array).
    } else if constexpr (requires { object.get(); }) {
        return object.get();  // Use `get()` if available (e.g., std::unique_ptr, std::shared_ptr).
    } else if constexpr (requires { object.value(); }) {
        return object.value();  // Use `value()` if available (e.g., std::optional).
    } else if constexpr (requires { *object; }) {
        return *object;  // Use dereference if possible (e.g., raw pointers, smart pointers).
    } else {
        static_assert(always_false<T>, "Type does not support data retrieval.");
    }
}

// Define the `at` function that works with any Array-like type
template <typename ARRAY, typename INDEX>
Arithmetic auto at(ARRAY& arr, INDEX index) {
    if constexpr (requires { arr.at(index); }) {
        return arr.at(index);  // Use `at()` if available.
    } else if constexpr (requires { arr[index]; }) {
        return arr[index];  // Use `operator[]` if available.
    } else if constexpr (requires { arr.coeff(index); }) {
        return arr.coeff(index);  // Use `coeff()` if available.
    } else if constexpr (requires { arr(index); }) {
        return arr(index);  // Use `operator()` if available.
    } else if constexpr (requires { ums::data(arr); }) {
        // Use `data()` or `get()` if available, and apply `at` on the underlying data.
        return at(*ums::data(arr), index);
    } else {
        static_assert(always_false<ARRAY>, "Type does not support valid indexing.");
    }
}


// Define the `at` function that works with any Matrix-like type
template <typename MATRIX, typename INDEX>
decltype(auto) at(MATRIX& arr, INDEX i, INDEX j) {
    if constexpr (requires { arr.at(i, j); }) {
        return arr.at(i, j);  // Use `at()` if available.
    } else if constexpr (requires { arr.coeff(i, j); }) {
        return arr.coeff(i, j);  // Use `coeff()` if available.
    } else if constexpr (requires { arr(i, j); }) {
        return arr(i, j);  // Use `operator()` if available.
    } else {
        static_assert(always_false<MATRIX>, "Type does not support valid indexing.");
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
        return sizeof(arr) / sizeof(arr[0]);  // Use `sizeof` for fixed-size arrays.
    } else if constexpr (requires { arr.get(); }) {
        // If `arr` has a `get()` method, assume it is a smart pointer and apply `len` to the underlying object.
        return len(*arr.get());
    } else {
        static_assert(always_false<ARRAY>, "Type does not support length retrieval.");
    }
}

// Define the `dim` function that works with any Matrix-like type
template <typename MATRIX>
auto dim(const MATRIX& mat) {
    if constexpr (requires { mat.rows(); mat.cols(); }) {
        // If the matrix has both `rows()` and `cols()` methods.
        return std::make_pair(mat.rows(), mat.cols());
    } else if constexpr (requires { mat.n_rows; mat.n_cols; }) {
        // For Armadillo-style matrices with `n_rows` and `n_cols`.
        return std::make_pair(mat.n_rows, mat.n_cols);
    } else if constexpr (requires { mat.size1(); mat.size2(); }) {
        // For Boost uBLAS matrices with `size1()` and `size2()`.
        return std::make_pair(mat.size1(), mat.size2());
    } else if constexpr (requires { mat.nr(); mat.nc(); }) {
        // For Dlib-style matrices with `nr()` and `nc()`.
        return std::make_pair(mat.nr(), mat.nc());
    } else if constexpr (requires { mat.rows; mat.cols; }) {
        // For OpenCV-style matrices with `rows` and `cols` attributes.
        return std::make_pair(mat.rows, mat.cols);
    } else if constexpr (std::is_array_v<MATRIX>) {
        // For plain C++ arrays (assuming 2D arrays).
        return std::make_pair(sizeof(mat) / sizeof(mat[0]), sizeof(mat[0]) / sizeof(mat[0][0]));
    } else {
        static_assert(always_false<MATRIX>, "Type does not support dimension retrieval.");
    }
}

// Define a concept for Matrix-like types
template <typename T>
concept MatrixLike = requires(T& mat) {
    { dim(mat) } -> std::same_as<std::pair<size_t, size_t>>;
};

// Define a concept for Vector-like types
template <typename T>
concept VectorLike = requires(T& vec) {
    { len(vec) } -> std::convertible_to<size_t>;
};

// Define the `dot` function for two Array-like types
template <VectorLike A, VectorLike B, typename INDEX>
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
template <VectorLike A, VectorLike B>
auto dot(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return dot(a, b, static_cast<decltype(n)>(0), n);
}

// Define the `sum` function for an Array-like type
template <VectorLike A, typename INDEX>
auto sum(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        result += at(a, i);
    }
    return result;
}

template <VectorLike A>
auto sum(const A& a) {
    auto n = len(a);
    return sum(a, static_cast<decltype(n)>(0), n);
}

// Define the `mean` function for an Array-like type
template <VectorLike A, typename INDEX>
auto mean(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    if (count < 1) {
        throw std::invalid_argument("Mean requires at least 1 element.");
    }

    auto result = sum(a, start, count);
    return result / count;
}

template <VectorLike A>
auto mean(const A& a) {
    auto n = len(a);
    return mean(a, static_cast<decltype(n)>(0), n);
}

// Define the variance function for an Array-like type
template <VectorLike A, typename INDEX>
auto variance(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    if (count < 2) {
        throw std::invalid_argument("Variance requires at least 2 elements.");
    }

    auto m = mean(a, start, count);
    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        auto diff = static_cast<SumType>(at(a, i)) - m;
        result += diff * diff;
    }
    return result / count;
}

template <VectorLike A>
auto variance(const A& a) {
    auto n = len(a);
    return variance(a, static_cast<decltype(n)>(0), n);
}

// Define the skewness function for an Array-like type
template <VectorLike A, typename INDEX>
auto skewness(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    if (count < 3) {
        throw std::invalid_argument("Skewness requires at least 3 elements.");
    }

    auto m = mean(a, start, count);
    auto v = variance(a, start, count);
    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        auto diff = static_cast<SumType>(at(a, i)) - m;
        result += diff * diff * diff;
    }
    return result / (count * v * std::sqrt(v));
}

template <VectorLike A>
auto skewness(const A& a) {
    auto n = len(a);
    return skewness(a, static_cast<decltype(n)>(0), n);
}

// Define the kurtosis function for an Array-like type
template <VectorLike A, typename INDEX>
auto kurtosis(const A& a, INDEX start, INDEX count) {
    using ValueType = std::remove_reference_t<decltype(at(a, 0))>;
    using SumType = std::common_type_t<ValueType, double>;

    if (count < 4) {
        throw std::invalid_argument("Kurtosis requires at least 4 elements.");
    }

    auto m = mean(a, start, count);
    auto v = variance(a, start, count);
    SumType result = 0;
    for (INDEX i = start; i < count; ++i) {
        auto diff = static_cast<SumType>(at(a, i)) - m;
        result += diff * diff * diff * diff;
    }
    return result / (count * v * v);
}

template <VectorLike A>
auto kurtosis(const A& a) {
    auto n = len(a);
    return kurtosis(a, static_cast<decltype(n)>(0), n);
}


template <VectorLike A, typename INDEX>
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

template <VectorLike A>
auto l2(const A& a) {
    auto n = len(a);
    return l2(a, static_cast<decltype(n)>(0), n);
}

template <VectorLike A, VectorLike B, typename INDEX>
auto cosine_similarity(const A& a, const B& b, INDEX begin, INDEX count) {
    auto dot_product = dot(a, b, begin, count);
    auto l2_a = l2(a, begin, count);
    auto l2_b = l2(b, begin, count);
    return dot_product / (l2_a * l2_b);
}

template <VectorLike A, VectorLike B>
auto cosine_similarity(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return cosine_similarity(a, b, static_cast<decltype(n)>(0), n);
}

template <VectorLike A, VectorLike B, typename INDEX>
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

template <VectorLike A, VectorLike B>
auto euclidean_distance(const A& a, const B& b) {
    auto n = len(a);
    if (n != len(b)) {
        throw std::length_error("Arrays must have the same length.");
    }
    return euclidean_distance(a, b, static_cast<decltype(n)>(0), n);
}

template <VectorLike A, VectorLike INDEX>
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

template <VectorLike A>
auto percentile(A& a, double p) {
    auto n = len(a);
    return percentile(a, 0ul, n, p);
}   

template <VectorLike A, VectorLike INDEX>
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

template <VectorLike A>
auto median(A& a) {
    auto n = len(a);
    return median(a, static_cast<decltype(n)>(0), n);
}

template <VectorLike A>
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

// Helper function to convert a value to a JSON-compatible string
template <typename T>
std::string to_json_value(const T& value) {
    if constexpr (std::is_floating_point_v<T>) {
        // Use stringstream to format floating-point numbers
        std::ostringstream oss;
        oss << std::setprecision(6) << std::noshowpoint << value;
        return oss.str();
    } else {
        // Use std::to_string for other types like integers
        return std::to_string(value);
    }
}

// Define the `tojson` function for Vector-like types
template <VectorLike A>
auto tojson(const A& a) {
    std::string result = "[";
    auto n = len(a);
    for (size_t i = 0; i < n; ++i) {
        result += to_json_value(at(a, i));
        if (i < n - 1) {
            result += ", ";
        }
    }
    result += "]";
    return result;
}


} // namespace ums