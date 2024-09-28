#include "learning_algorithm.h"
#include "config.h"
#include "../sd_card/sd_card.h"

static float overGrindOffset = INITIAL_OGO;
static float alpha = ALPHA;

void initLearningAlgorithm() {
    // Load overGrindOffset from SD card if available
    if (!loadOverGrindOffset(overGrindOffset)) {
        overGrindOffset = INITIAL_OGO;
    }
}

float calculateAdjustedTargetWeight(float desiredWeight) {
    return desiredWeight - overGrindOffset;
}

void updateLearningParameters(float desiredWeight, float finalWeight, float measuredWeightAtStop) {
    float actualOGO = finalWeight - measuredWeightAtStop;
    overGrindOffset = (alpha * actualOGO) + ((1 - alpha) * overGrindOffset);

    // Save updated overGrindOffset to SD card
    saveOverGrindOffset(overGrindOffset);

    // Log session data
    SessionData data = {
        .desiredWeight = desiredWeight,
        .measuredWeightAtStop = measuredWeightAtStop,
        .finalWeight = finalWeight,
        .overGrindOffset = overGrindOffset
    };
    logData(data);
}