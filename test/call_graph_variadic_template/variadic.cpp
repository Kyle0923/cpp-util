// Template function where the first parameter is a function pointer
template <typename Func, typename... Args>
void apply_function(Func func, Args... args) {
    func(args...); // Call the function with the provided arguments
}

// Example function to be used with apply_function
void print_sum(int a, int b) {

}

int main() {
    // Using apply_function to call print_sum
    apply_function(print_sum, 3, 4); // Output: Sum: 7

    // Using apply_function with a lambda function
    apply_function([](int x, int y) {
    }, 5, 6); // Output: Product: 30

    return 0;
}
