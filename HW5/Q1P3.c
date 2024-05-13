// Harris Ransom
// IntroCompSys HW5 Q1 Part 3

// Test and Set atomic function
int test_and_set (int *target) {
    int rv = *target;
    *target = 1;
    return rv;
}

// Compare and Swap atomic function
int compare_and_swap(int *value, int expected, int new_value) {
    int temp = *value;

    if (*value == expected)
        *value = new_value;

    return temp;
}

// Spinlock struct
typedef struct {
    // available == 0 means the lock is available
    // available == 1 means the lock is not available
    int available;
} lock;


// Mutex acquire function
void acquire(lock *mutex) {
    while (mutex->available == 1) {
        // Spin
    }
    test_and_set(&mutex->available); // Lock the mutex
}

// Mutex release function
void release(lock *mutex) {
    compare_and_swap(&mutex->available, 1, 0);
}