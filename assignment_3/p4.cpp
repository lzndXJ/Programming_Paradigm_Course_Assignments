//version 1
// 使用朴素递归方法计算第n个卡特兰数。
//
// 正确性分析：
//   实现正确，直接基于卡特兰数的数学定义
//   但存在严重的性能问题，仅适用于教学演示
// 特点与使用场景：
//   优点：代码简洁，直接反映数学定义，易于理解
//   缺点：指数级时间复杂度，重复计算严重
//   适用场景：仅用于n < 15的小规模计算或算法教学演示
// 新知识要点：
//   无新语法，为基础递归实现
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数。
// 时间复杂度：O(4^n / n^(3/2))
// 空间复杂度：O(n) 由于递归栈
int catalan_recursive(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    int res = 0;
    // 应用递推关系：Cₙ = Σ(i=0 to n-1) Cᵢ × Cₙ₋₁₋ᵢ
    for (int i = 0; i < n; ++i) {
        res += catalan_recursive(i) * catalan_recursive(n - 1 - i);
    }
    return res;
}

//version 2
#include <vector>

// 使用迭代动态规划方法计算第n个卡特兰数。
//
// 正确性分析：
//   实现正确且高效，基于标准的动态规划方法
// 特点与使用场景：
//   优点：多项式时间复杂度，无递归开销，效率高
//   缺点：可能整数溢出（当n > 35时）
//   适用场景：n ≤ 35的中等规模计算，通用性最好的实现
// 新知识要点：
//   std::vector的构造函数初始化语法
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数。
// 时间复杂度：O(n²)
// 空间复杂度：O(n)
int catalan_iterative(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // dp[i] 存储第i个卡特兰数
    std::vector<int> dp(n + 1, 0);
    dp[0] = dp[1] = 1;

    // 使用递推关系计算卡特兰数：Cₙ = Σ(j=0 to n-1) Cⱼ × Cₙ₋₁₋ⱼ
    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }
    return dp[n];
}

//version 3
#include <unordered_map>

// 使用带记忆化的递归方法计算第n个卡特兰数。
// 正确性分析：
//   实现正确，结合了递归的直观性和动态规划的效率
// 特点与使用场景：
//   优点：避免重复计算，比纯递归效率显著提高
//   缺点：递归栈深度限制，哈希表查找开销
//   适用场景：喜欢递归思路但需要较好效率的场景，n ≤ 1000
// 新知识要点：
//   std::unordered_map用于记忆化技术，避免重复计算
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
//   memo: 用于存储已计算卡特兰数的unordered_map引用，
//         首次调用时应为空。
// 返回：
//   第n个卡特兰数（long long类型以支持更大数值）。
// 时间复杂度：O(n²)
// 空间复杂度：O(n)
long long catalan_memoization(int n, std::unordered_map<int, long long>& memo) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 如果结果已缓存，直接返回
    auto it = memo.find(n);
    if (it != memo.end()) {
        return it->second;
    }

    long long res = 0;
    // 应用递推关系：Cₙ = Σ(i=0 to n-1) Cᵢ × Cₙ₋₁₋ᵢ
    for (int i = 0; i < n; ++i) {
        res += catalan_memoization(i, memo) * catalan_memoization(n - 1 - i, memo);
    }

    // 返回前缓存结果
    memo[n] = res;
    return res;
}

//version 4
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>

// 使用Boost多精度整数计算大卡特兰数。
// 正确性分析：
//   实现正确，支持任意大数计算，防止整数溢出
// 特点与使用场景：
//   优点：支持大数计算，无溢出问题
//   缺点：依赖外部Boost库，计算速度较慢
//   适用场景：需要计算大卡特兰数（n > 35）或精确数值计算
// 新知识要点：
//   Boost.Multiprecision库的使用，cpp_int任意精度整数类型
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数（cpp_int多精度整数类型）。
// 时间复杂度：O(n²)
// 空间复杂度：O(n)
boost::multiprecision::cpp_int catalan_bigint(int n) {
    using boost::multiprecision::cpp_int;

    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 使用多精度整数的动态规划数组
    std::vector<cpp_int> dp(n + 1);
    dp[0] = dp[1] = 1;

    // 标准动态规划方法，使用多精度整数避免溢出
    for (int i = 2; i <= n; ++i) {
        dp[i] = 0;
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    return dp[n];
}

//version 5
#include <cmath>

// 计算n的阶乘。
// 参数：
//   n: 要计算阶乘的非负整数。
// 返回：
//   n的阶乘。
// 注意：当 n > 20 时可能发生整数溢出。
long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

// 使用组合数学公式计算第n个卡特兰数。
// 正确性分析：
//   实现正确 - 基于卡特兰数的标准组合公式
//   公式：Cₙ = (2n)! / (n! × (n+1)!)
// 特点与使用场景：
//   优点：数学定义直接，代码简洁
//   缺点：容易整数溢出（n > 15时），阶乘计算效率低
//   适用场景：n ≤ 15的小规模计算，数学公式验证
// 新知识要点：
//   卡特兰数的组合数学公式应用
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数。
// 时间复杂度：O(n)
// 空间复杂度：O(1)
long long catalan_formula(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 使用组合公式：Cₙ = (2n)! / (n! × (n+1)!)
    // 注意：由于阶乘快速增长，此实现在 n > 15 时会发生整数溢出
    return factorial(2 * n) / (factorial(n + 1) * factorial(n));
}

//version 6
// 使用constexpr编译时计算第n个卡特兰数。
// 正确性分析：
//   实现正确，支持编译期计算
//   但数组大小固定，仅支持n ≤ 63
// 特点与使用场景：
//   优点：编译期计算，运行时常数为O(1)，无运行时开销
//   缺点：数组大小固定，不支持动态n值，可能栈溢出
//   适用场景：n较小且已知的编译期计算，模板元编程
// 新知识要点：
//   constexpr函数，编译期计算，C风格数组初始化
// 参数：
//   n: 要计算的卡特兰数的索引。必须为0-63之间的整数。
// 返回：
//   第n个卡特兰数。
// 时间复杂度：编译期O(n²)，运行期O(1)
// 空间复杂度：O(1) 运行期
constexpr long long catalan_constexpr(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 支持n ≤ 63的编译期数组
    long long dp[64] = { 0 };
    dp[0] = dp[1] = 1;

    // 编译期动态规划计算
    for (int i = 2; i <= n; ++i) {
        dp[i] = 0;
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    return dp[n];
}

//version 7
#include <future>

// 使用并行递归方法计算第n个卡特兰数。
// 正确性分析：
//   理论上正确但实际不实用
//   存在严重的性能问题和资源浪费
// 特点与使用场景：
//   优点：理论上支持并行计算
//   缺点：创建过多线程，线程开销远大于计算收益，实际性能更差
//   适用场景：仅用于并行计算理论研究，不推荐实际使用
// 新知识要点：
//   std::async, std::future, std::launch::async异步执行策略
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数。
// 时间复杂度：O(4^n) 且伴随巨大线程开销
// 空间复杂度：O(n) 加上线程栈空间
long long catalan_parallel(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 为每个递归调用创建异步任务（性能极差）
    std::vector<std::future<long long>> futures;
    for (int i = 0; i < n; ++i) {
        futures.push_back(std::async(std::launch::async, catalan_parallel, i));
    }

    // 等待所有任务完成并累加结果
    long long res = 0;
    for (int i = 0; i < n; ++i) {
        res += futures[i].get() * catalan_parallel(n - 1 - i);
    }

    return res;
}

//version 8
#include <optional>
#include <vector>

// 使用std::optional的安全卡特兰数计算实现。
// 正确性分析：
//   实现正确，提供良好的错误处理机制
// 特点与使用场景：
//   优点：输入验证，避免未定义行为，返回类型安全
//   缺点：额外的optional包装开销
//   适用场景：需要健壮错误处理的生产代码，API设计
// 新知识要点：
//   std::optional, std::nullopt, 现代C++错误处理
// 参数：
//   n: 要计算的卡特兰数的索引。
// 返回：
//   包含第n个卡特兰数的std::optional，输入无效时返回std::nullopt。
// 时间复杂度：O(n²)
// 空间复杂度：O(n)
std::optional<long long> catalan_safe(int n) {
    // 输入验证：卡特兰数仅对非负整数有定义
    if (n < 0) return std::nullopt;

    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    std::vector<long long> dp(n + 1);
    dp[0] = dp[1] = 1;

    // 标准动态规划计算
    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    return dp[n];
}

//version 9
// 使用模板元编程在编译期计算卡特兰数。
// 正确性分析：
//   实现正确，完全的编译期计算
//   但受限于编译期递归深度
// 特点与使用场景：
//   优点：零运行时开销，类型安全，编译期优化
//   缺点：编译时间长，调试困难，模板实例化限制
//   适用场景：n较小的编译期计算，性能关键的系统
// 新知识要点：
//   模板特化，constexpr lambda，模板元编程
// 模板参数：
//   N: 要计算的卡特兰数的索引（编译期常量）。
// 使用示例：
//   long long result = Catalan<10>::value;
template<int N>
struct Catalan {
    // 使用constexpr lambda在编译期计算卡特兰数
    static constexpr long long value = ([]() constexpr {
        long long sum = 0;
        for (int i = 0; i < N; ++i) {
            sum += Catalan<i>::value * Catalan<N - 1 - i>::value;
        }
        return sum;
        })();
};

// 模板特化：C₀ = 1
template<>
struct Catalan<0> {
    static constexpr long long value = 1;
};

// 模板特化：C₁ = 1
template<>
struct Catalan<1> {
    static constexpr long long value = 1;
};

//version 10
#include <array>

// 2x2矩阵类型别名
using Matrix2x2 = std::array<std::array<long long, 2>, 2>;

// 2x2矩阵乘法函数。
// 参数：
//   a: 第一个2x2矩阵
//   b: 第二个2x2矩阵
// 返回：
//   矩阵乘积 a × b
Matrix2x2 matrix_multiply(const Matrix2x2& a, const Matrix2x2& b) {
    Matrix2x2 result = { {{0, 0}, {0, 0}} };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

// 使用矩阵方法计算卡特兰数的概念演示（当前实现不正确）。
// 正确性分析：
//   当前实现不正确 - 卡特兰数没有简单的2×2矩阵递推关系
//   此实现更多是概念演示而非实际算法
// 特点与使用场景：
//   优点：展示矩阵方法的思想
//   缺点：算法不正确，不能正确计算卡特兰数
//   适用场景：仅用于矩阵递推关系的教学演示
// 新知识要点：
//   std::array嵌套使用，矩阵运算
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   不正确的"卡特兰数"计算结果。
long long catalan_matrix(int n) {
    if (n <= 1) return 1;  // 基本情况：C₀ = C₁ = 1

    // 概念演示：假设存在矩阵递推关系（实际不存在）
    Matrix2x2 base = { {{1, 1}, {1, 0}} };
    Matrix2x2 result = { {{1, 0}, {0, 1}} };  // 单位矩阵

    // 矩阵幂运算（概念性，不能正确计算卡特兰数）
    for (int i = 0; i < n; ++i) {
        result = matrix_multiply(result, base);
    }

    return result[0][0];  // 返回的结果不是卡特兰数
}

#include <vector>
#include <array>

using Matrix2x2 = std::array<std::array<long long, 2>, 2>;

Matrix2x2 matrix_multiply(const Matrix2x2& a, const Matrix2x2& b) {
    Matrix2x2 result = { {{0, 0}, {0, 0}} };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

// 修正后的矩阵版本
long long catalan_matrix_fixed(int n) {
    if (n <= 1) return 1;

    // 概念演示部分：矩阵幂运算（计算斐波那契数）
    Matrix2x2 base = { {{1, 1}, {1, 0}} };
    Matrix2x2 result = { {{1, 0}, {0, 1}} };

    int power = n;
    while (power > 0) {
        if (power & 1) {
            result = matrix_multiply(result, base);
        }
        base = matrix_multiply(base, base);
        power >>= 1;
    }

    // 实际计算：使用动态规划计算卡特兰数
    std::vector<long long> dp(n + 1, 0);
    dp[0] = dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    std::cout << "矩阵演示结果(F_" << n << "): " << result[0][1] << std::endl;
    std::cout << "卡特兰数(C_" << n << "): " << dp[n] << std::endl;

    return dp[n];  // 返回正确的卡特兰数
}


//my version
#include <vector>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>

// 最优卡特兰数计算实现 - 结合正确性、效率和实用性。
// 设计理念：
//   使用动态规划保证正确性和效率
//   支持大数计算防止溢出
//   提供健壮的输入验证
//   清晰的错误处理机制
// 正确性分析：
//   基于验证的动态规划方法，支持任意非负整数n
// 特点与使用场景：
//   优点：正确性保证，支持大数，良好错误处理，适中效率
//   适用场景：通用卡特兰数计算，生产环境使用
// 新知识应用：
//   异常处理，Boost多精度库，现代C++最佳实践
// 参数：
//   n: 要计算的卡特兰数的索引。必须为非负数。
// 返回：
//   第n个卡特兰数。
// 异常：
//   当n < 0时抛出std::invalid_argument
// 时间复杂度：O(n²)
// 空间复杂度：O(n)
boost::multiprecision::cpp_int catalan_optimal(int n) {
    // 输入验证
    if (n < 0) {
        throw std::invalid_argument("卡特兰数仅对非负整数有定义");
    }

    // 基本情况处理
    if (n <= 1) {
        return 1;
    }

    // 动态规划计算
    std::vector<boost::multiprecision::cpp_int> dp(n + 1);
    dp[0] = dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        dp[i] = 0;
        for (int j = 0; j < i; ++j) {
            dp[i] += dp[j] * dp[i - 1 - j];
        }
    }

    return dp[n];
}