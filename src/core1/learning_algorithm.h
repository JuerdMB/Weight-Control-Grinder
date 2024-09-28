#pragma once

void initLearningAlgorithm();
float calculateAdjustedTargetWeight(float desiredWeight);
void updateLearningParameters(float desiredWeight, float finalWeight, float measuredWeightAtStop);