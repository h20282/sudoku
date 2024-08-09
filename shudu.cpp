#include <iostream>
#include <vector>

template <typename T>
inline void print(T f) {
    std::cout << f;
}

template <typename First, typename... Rest>
inline void print(const char *format, const First &first, const Rest &...rest) {
    if (*format == '\0') { return; }
    while (*format && !(format[0] == '{' && (format[1] == '}'))) {
        putchar(*format++);
    }
    std::cout << first;
    print(format + 2, rest...);
}

#define println(...)    \
    print(__VA_ARGS__); \
    print('\n');

#define LOG(...)                              \
    print("{}:{} ", __FILE_NAME__, __LINE__); \
    println(__VA_ARGS__);

constexpr const int n = 9;
constexpr const bool print_detail = true;

int cell[n][n] = {0};

struct Pos {
    int x;
    int y;
};

int &CellAt(int x, int y) {
    return cell[y][x];
}

void Input(const std::string &str) {
    int i = 0;
    for (auto c : str) {
        if (c >= '0' && c <= '9') {
            CellAt(i % n, i / n) = c - '0';
            ++i;
        }

        if (i >= n * n) { break; }
    }
}

void PrintCell() {
    for (int y = 0; y < n; ++y) {
        print(y % 3 ? "" : "\n");
        for (int x = 0; x < n; ++x) {
            print(x % 3 ? "{} " : " {} ", CellAt(x, y));
        }
        println("");
    }
    println("");
}

Pos EmptyPos() {
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            if (CellAt(x, y) == 0) { return {x, y}; }
        }
    }
    return {-1, -1};
}

std::vector<int> AvaliableNumber(int x, int y) {
    bool has[n + 1] = {false};
    auto check = [&](int x, int y) {
        has[CellAt(x, y)] = true;
        // LOG("({}, {}), has[{}] = true", x, y, CellAt(x, y))
    };

    for (int i = 0; i < n; ++i) {
        check(x, i);
        check(i, y);
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) { check((x - x % 3) + i, (y - y % 3) + j); }
    }

    std::vector<int> avaliable_number;
    for (int i = 1; i <= n; ++i) {
        if (has[i] == false) { avaliable_number.push_back(i); }
    }
    return avaliable_number;
}

int solution = 0;
int call_cnt = 0;
int step_cnt = 0;

void dfs(int deep = 0) {
    ++call_cnt;

    auto [x, y] = EmptyPos();
    if (x == -1 && y == -1) {
        ++solution;
        println("solution {}: ", solution);
        PrintCell();
        return;
    }

    auto avaliable_number = AvaliableNumber(x, y);
    if (avaliable_number.size() == 0) {

        if (print_detail) {
            for (int i = 0; i < deep; ++i) { print(" "); }
            LOG("no avaliable number at ({}, {})", x, y);
        }

        return;
    }

    for (auto number : avaliable_number) {
        CellAt(x, y) = number;

        if (print_detail) {
            for (int i = 0; i < deep; ++i) { print(" "); }
            LOG("step {}, fill ({}, {}) = {}", ++step_cnt, x, y, number);
            PrintCell();
        }

        dfs(deep + 1);

        if (print_detail) {
            for (int i = 0; i < deep; ++i) { print(" "); }
            LOG("step {}, clear ({}, {}) = 0", ++step_cnt, x, y);
            PrintCell();
        }

        CellAt(x, y) = 0;
    }
}

int main() {
    auto easy = R"(
        7 0 0  0 1 0  5 4 0
        9 0 4  0 5 0  8 3 0
        0 0 3  9 0 4  6 0 0

        8 2 0  0 4 5  0 0 0
        0 0 0  8 0 2  1 0 7
        0 7 5  1 0 0  2 0 0

        2 9 6  0 7 0  0 0 0
        5 1 0  0 0 0  7 6 3
        0 0 7  0 6 1  0 2 0
    )";

    auto hard = R"(
        {0,0,0, 1,7,5, 0,0,0},
        {0,0,0, 0,0,8, 3,0,0},
        {0,0,0, 0,0,2, 8,9,0},

        {0,0,8, 0,6,0, 0,0,0},
        {0,9,0, 0,0,0, 4,0,6},
        {0,2,0, 7,0,0, 0,0,0},

        {7,0,0, 0,2,0, 0,5,0},
        {0,0,0, 0,0,0, 0,1,0},
        {5,0,1, 0,0,0, 0,0,8},
    )";

    Input(easy);

    PrintCell();

    dfs();

    LOG("end, call_cnt = {}, total solution = {}", call_cnt, solution);

    return 0;
}
