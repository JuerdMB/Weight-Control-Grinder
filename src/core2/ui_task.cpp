#include "ui_task.h"
#include "config.h"
#include "input_handler.h"
#include "communication/inter_core_comm.h"
#include "display.h"

void uiTask()
{
    ui_state ui_state = UI_MAIN;

    CommandMessage cmd_msg;
    StatusMessage status_msg;
    float desired_weight = DEFAULT_COFFEE_WEIGHT; // Default weight in grams

    // initDisplay();
    // initInputHandler();

    while (true)
    {
        switch (ui_state)
        {
        case UI_MAIN:
            // Handle rotary encoder input
            int32_t encoder_pos = getEncoderPosition();
            if (encoder_pos != 0)
            {
                desired_weight += encoder_pos * 0.5f; // Adjust step size as needed
                if (desired_weight < 0)
                    desired_weight = 0;

                if (desired_weight > MAX_COFFEE_WEIGHT)
                    desired_weight = MAX_COFFEE_WEIGHT;

                displayDesiredWeight(desired_weight);
                resetEncoderPosition();
            }

            // Handle button press input to start grinding
            if (isButtonPressed())
            {
                // send message to core 2 to start grinding
                cmd_msg.type = START_GRINDING;
                cmd_msg.desiredWeight = desired_weight;

                sendCommandToCore1(cmd_msg);

                // Update UI state
                ui_state = UI_GRINDING_IN_PROGRESS;
                displayGrindingInProgress(0.0f);

                // Turn off encoder so weight cannot be changed during grinding process
                disableEncoder();
            }

            break;

        case UI_GRINDING_IN_PROGRESS:
            // Check for status updates from Core 1
            if (receiveStatusFromCore1(&status_msg))
            {
                if (status_msg.type == WEIGHT_UPDATE)
                {
                    // Update current weight display
                    displayGrindingInProgress(status_msg.finalWeight);
                }
                else if (status_msg.type == GRINDING_COMPLETE)
                {
                    ui_state = UI_GRINDING_COMPLETE;
                    displayGrindingComplete(status_msg.finalWeight);
                }
                else if (status_msg.type == ERROR_OCCURRED)
                {
                    ui_state = UI_ERROR_STATE;
                    displayError(status_msg.errorMessage);
                }
            }

            // Handle button press input to cancel grinding
            if (isButtonPressed())
            {
                // Send command to core 1 to stop grinding
                cmd_msg.type = CANCEL_GRINDING;
                sendCommandToCore1(cmd_msg);

                // Wait to receive confirmation of cancel grinding with final weight
                while (!receiveStatusFromCore1(&status_msg))
                {
                    // Wait until we receive confirmation
                }

                if (status_msg.type == GRINDING_COMPLETE)
                {
                    ui_state = UI_GRINDING_COMPLETE;
                    displayGrindingComplete(status_msg.finalWeight);
                }
            }

            break;

        case UI_GRINDING_COMPLETE:
            // Wait for user acknowledgment or a delay
            if (isButtonPressed())
            {
                enableEncoder();
                ui_state = UI_MAIN;
                displayDesiredWeight(desired_weight);
            }
            break;

        case UI_ERROR_STATE:
            // Wait for user acknowledgment
            if (isButtonPressed())
            {
                enableEncoder();
                ui_state = UI_MAIN;
                displayDesiredWeight(desired_weight);
            }
            break;

        default:
            ui_state = UI_MAIN;
            break;
        }

        vTaskDelay(UI_TASK_DELAY / portTICK_PERIOD_MS);
    }
}