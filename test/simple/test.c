#include <stdio.h>

// Test basic arithmetic operations
int test_arithmetic(int a, int b) {
    int sum = a + b;        // Test MOV_ADD
    int diff = a - b;       // Test MOV_SUB
    int prod = a * b;       // Test MOV_MUL
    return sum + diff + prod;
}

// Test conditional operations
int test_conditional(int a, int b) {
    if (a > b) {           // Test MOV_CMP_GT
        return a;
    } else {
        return b;
    }
}

// Test memory operations
void test_memory(int* arr, int size) {
    int temp;
    for (int i = 0; i < size; i++) {
        temp = arr[i];     // Test MOV_mr
        arr[i] = temp * 2; // Test MOV_rm and MOV_MUL
    }
}

int main() {
    // Test arithmetic
    int result1 = test_arithmetic(5, 3);
    printf("Arithmetic test result: %d\n", result1);

    // Test conditional
    int result2 = test_conditional(10, 5);
    printf("Conditional test result: %d\n", result2);

    // Test memory
    int arr[] = {1, 2, 3, 4, 5};
    test_memory(arr, 5);
    printf("Memory test result: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
