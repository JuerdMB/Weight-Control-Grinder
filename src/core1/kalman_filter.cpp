#include "kalman_filter.h"
#include "config.h"

static float Q = KALMAN_Q;
static float R = KALMAN_R;
static float P = 1.0f;
static float X = 0.0f;
static float K = 0.0f;

void initKalmanFilter() {
    P = 1.0f;
    X = 0.0f;
}

float getFilteredWeight(float rawMeasurement) {

    // Kalman Filter Calculation
    P = P + Q;
    K = P / (P + R);
    X = X + K * (rawMeasurement - X);
    P = (1 - K) * P;

    return X;
}