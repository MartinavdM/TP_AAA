/**
 * \file shell.c
 * \brief Ce fichier contient deux fonctions qui permettent de créer un shell avec lequel on interagit grâce à Putty. Le fonction qui tourne en permanence (Shell_Loop), appelle la fonction set_motor_speed() qui permet de régler la vitesse du moteur, d'afficher le courant et la vitesse.
 * \author Paul-Etienne Rétaux
 * \date 30 octobre 2023
 *
 *
 */
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t prompt[] = "user@Nucleo-STM32G474RET6>>";
uint8_t started[] = "\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G474 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[] = "\r\n";
uint8_t backspace[] = "\b \b";
uint8_t cmdNotFound[] = "Command not found\r\n";
uint8_t brian[] = "Brian is in the kitchen\r\n";
uint8_t uartRxReceived;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];
uint8_t idx = 0;

extern uint32_t ADC_buffer[ADC_BUF_SIZE];
extern uint8_t current_flag; // flag used for the current computation callback
extern uint8_t speed_flag; // flag used for the speed computation callback
extern float measured_speed;
extern float filtered_measured_speed;

int order_speed;
float current;

char cmdBuffer[CMD_BUFFER_SIZE];
int idx_cmd;
char *argv[MAX_ARGS];
int argc = 0;
char *token;
int newCmdReady = 0;
int onOffCmd;

void Shell_Init(void) {
	memset(argv, (int) NULL, MAX_ARGS * sizeof(char*));
	memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(char));
	memset(uartRxBuffer, 0, UART_RX_BUFFER_SIZE * sizeof(char));
	memset(uartTxBuffer, 0, UART_TX_BUFFER_SIZE * sizeof(char));

	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
	HAL_UART_Transmit(&huart2, started, strlen((char*) started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, prompt, strlen((char*) prompt), HAL_MAX_DELAY);
}

void Shell_Loop(void) {
	if (uartRxReceived) {
		switch (uartRxBuffer[0]) {
		case ASCII_CR: // Nouvelle ligne, instruction à traiter
			HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
			cmdBuffer[idx_cmd] = '\0';
			argc = 0;
			token = strtok(cmdBuffer, " ");
			while (token != NULL) {
				argv[argc++] = token;
				token = strtok(NULL, " ");
			}
			idx_cmd = 0;
			newCmdReady = 1;
			break;
		case ASCII_BACK: // Suppression du dernier caractère
			cmdBuffer[idx_cmd--] = '\0';
			HAL_UART_Transmit(&huart2, backspace, sizeof(backspace),
			HAL_MAX_DELAY);
			break;

		default: // Nouveau caractère
			cmdBuffer[idx_cmd++] = uartRxBuffer[0];
			HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE,
			HAL_MAX_DELAY);
		}
		uartRxReceived = 0;
	}

	if (newCmdReady) {
		if (strcmp(argv[0], "WhereisBrian?") == 0) {
			HAL_UART_Transmit(&huart2, brian, sizeof(brian), HAL_MAX_DELAY);
		} else if (strcmp(argv[0], "help") == 0) {
			int uartTxStringLength = snprintf((char*) uartTxBuffer,
			UART_TX_BUFFER_SIZE, "Print all available functions here\r\n");
			HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength,
			HAL_MAX_DELAY);
		} else if (strcmp(argv[0], "start") == 0) {
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
			order_speed = 50;
			set_motor_speed(order_speed);
		} else if (strcmp(argv[0], "stop") == 0) {
			order_speed = 50;
			set_motor_speed(order_speed);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
			HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
		} else if (strcmp(argv[0], "speed") == 0) {
			if (atoi(argv[1]) > MAX_SPEED_HIGH) {
				int uartTxStringLength = snprintf((char*) uartTxBuffer,
				UART_TX_BUFFER_SIZE,
						"Input speed superior to maximum speed.\r\n");
				HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength,
				HAL_MAX_DELAY);
				order_speed = MAX_SPEED_HIGH;
				set_motor_speed(order_speed);
			}
			if (atoi(argv[1]) < MAX_SPEED_LOW) {
				int uartTxStringLength = snprintf((char*) uartTxBuffer,
				UART_TX_BUFFER_SIZE,
						"Input speed superior to maximum speed.\r\n");
				HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength,
				HAL_MAX_DELAY);
				order_speed = MAX_SPEED_LOW;
				set_motor_speed(order_speed);
			}
			order_speed = atoi(argv[1]);
			set_motor_speed(order_speed);
//			set_motor_speed(order_alpha); // corrected speed but we didn't have time to make tests with the motor
			if (speed_flag) {
				printf("Vitesse du moteur : %f tours/minute\r\n",
						measured_speed); // printf enables to retrieve strings on the UART terminal as some lines have been added to the stm32g4xx_hal_msp.c file
				speed_flag = 0;
			}
			if (current_flag) {
				uint32_t voltage = ADC_buffer[0];
				current = compute_current(voltage);
				printf("Valeur du courant : %f A\r\n", current);
				current_flag = 0;
			}
		} else {
			HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound),
			HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
		newCmdReady = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
