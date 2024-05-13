// Harris Ransom
// IntroCompSys HW5 Q1 Part 1

// Test and Set atomic function
int test_and_set (int *target) {
    int rv = *target;
    (*target)++;
    return rv;
}

// Semaphore struct
typedef struct {
    // value == 0 means the lock is available
    // value != 0 means the lock is not available
    int value;
} semaphore;

// Wait semaphore implementation
void wait(semaphore *s) {
    while (s->value <= 0) {
        // Spin
    }
    s->value--;
}

// Signal semaphore implementation
void signal(semaphore *s) {
    test_and_set(&s->value);
}