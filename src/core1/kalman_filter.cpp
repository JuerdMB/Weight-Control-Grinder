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

float getFilteredWeight(long rawValue) {
    // Convert rawValue to weight in grams (calibration needed)
    float measurement = (float)rawValue; // Replace with actual conversion

    //TODO: Do the conversion from raw loadcell output to weight in grams

    // Kalman Filter Calculation
    P = P + Q;
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;

    return X;
}