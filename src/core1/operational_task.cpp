#include "operational_task.h"
#include "loadCell.h"
#include "motor_control.h"
#include "definitions.h"
#include "communication/inter_core_comm.h"
#include "kalman_filter.h"
#include "config.h"
#include "learning_algorithm.h"

void operationalTask(void *pvParameters)
{
    OperationalState op_state = OP_IDLE;
    CommandMessage cmd_msg;
    StatusMessage status_msg;

    float desired_weight = 0.0f;
    float adjusted_target_weight = 0.0f;
    float current_weight = 0.0f;
    float measured_weight_at_stop = 0.0f;
    float final_weight = 0.0f;

    uint32_t grind_start = 0;
    uint32_t grind_duration = 0;

    initMotorControl();
    // initLoadCell();
    initKalmanFilter();
    // initLearningAlgorithm();

    while (true)
    {
        switch (op_state)
        {

        case OP_IDLE:
            // Check for commands from Core 2
            if (receiveCommandFromCore2(&cmd_msg))
            {
                if (cmd_msg.type == START_GRINDING)
                {
                    desired_weight = cmd_msg.desiredWeight;
                    adjusted_target_weight = calculateAdjustedTargetWeight(desired_weight);
                    startMotor();
                    grind_start = millis();
                    op_state = OP_GRINDING;
                }
            }
            break;

        case OP_GRINDING:

            // Check how long we've been grinding
            grind_duration = millis() - grind_start;

            // Monitor weight
            // current_weight = getFilteredWeight(getLoadCellWeight());
            current_weight = (float)grind_duration / 500.f; // Temporary for test

            // Send weight update to UI core
            status_msg.type = WEIGHT_UPDATE;
            status_msg.finalWeight = current_weight;
            sendStatusToCore2(status_msg);

            // Handle reaching desired weight
            if (current_weight >= adjusted_target_weight)
            {
                stopMotor();
                measured_weight_at_stop = current_weight;
                op_state = OP_POST_GRINDING;
            }

            // Handle cancel command
            else if (receiveCommandFromCore2(&cmd_msg))
            {
                if (cmd_msg.type == CANCEL_GRINDING)
                {
                    stopMotor();
                    measured_weight_at_stop = current_weight;
                    op_state = OP_POST_GRINDING;
                }
            }

            break;

        case OP_POST_GRINDING:
            // Wait for weight to stabilize
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1 second
            final_weight = getFilteredWeight(getLoadCellWeight());

            // Update learning parameters
            updateLearningParameters(desired_weight, final_weight, measured_weight_at_stop);

            // Send status update to Core 2
            status_msg.type = GRINDING_COMPLETE;
            status_msg.finalWeight = final_weight;
            sendStatusToCore2(status_msg);

            op_state = OP_IDLE;
            break;

        case OP_ERROR:
            // Handle operational errors
            status_msg.type = ERROR_OCCURRED;
            status_msg.errorMessage = "Operational Error";
            sendStatusToCore2(status_msg);
            op_state = OP_IDLE;
            break;

        default:
            op_state = OP_IDLE;
            break;
        }
    }
}