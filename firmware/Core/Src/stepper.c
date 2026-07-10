/*
 * stepper.c
 *
 *  Created on: Jun 18, 2025
 *      Author: psh
 */
#include "stepper.h"

static volatile uint8_t stepper_stop_flag = 0;
static uint8_t current_step = 0;  // 정지 후 재시작 시 위상 연속성 유지

static const uint8_t HALF_STEP_SEQ[8][4] =
{
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 1},
		{1, 0, 0, 1},

};

void stepMotor(uint8_t step)
{
	HAL_GPIO_WritePin(IN1_GPIO_port, IN1_Pin, HALF_STEP_SEQ[step][0]);
	HAL_GPIO_WritePin(IN2_GPIO_port, IN2_Pin, HALF_STEP_SEQ[step][1]);
	HAL_GPIO_WritePin(IN3_GPIO_port, IN3_Pin, HALF_STEP_SEQ[step][2]);
	HAL_GPIO_WritePin(IN4_GPIO_port, IN4_Pin, HALF_STEP_SEQ[step][3]);
}
void stepperOff(void)
{
	HAL_GPIO_WritePin(IN1_GPIO_port, IN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN2_GPIO_port, IN2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN3_GPIO_port, IN3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN4_GPIO_port, IN4_Pin, GPIO_PIN_RESET);
}

// UART ISR 등 인터럽트에서 호출 — 연속 회전 중단 요청
void stepperRequestStop(void)
{
	stepper_stop_flag = 1;
}

// Pi가 ABORT 수신 시 호출: 천천히 연속 회전하다 stepperRequestStop()으로 정지
void rotateCarouselContinuous(uint8_t direction)
{
	stepper_stop_flag = 0;
	uint32_t ramp_i = 0;

	while (!stepper_stop_flag)
	{
		stepMotor(current_step);

		if (direction == DIR_CW)
			current_step = (current_step + 1) % 8;
		else
			current_step = (current_step + 7) % 8;

		// 처음 200스텝 3000us→1600us 가속 (공진 구간을 빠르게 통과)
		uint16_t d = (ramp_i < 200) ? (3000 - (uint16_t)(ramp_i * 7)) : CAROUSEL_STEP_US;
		if (ramp_i < 200) ramp_i++;

		delay_us(d);
	}

	stepperOff();
}

void rotateSteps(uint16_t steps, uint8_t direction)
{
	for(uint16_t i = 0; i < steps; i++)
	{
		uint8_t step = (direction == DIR_CW) ? (i % 8) : (7 - (i % 8));
		stepMotor(step);

		// 처음 200스텝 동안 3000us→1600us로 가속 (종료값 = 정상속도로 연속)
		uint16_t d = (i < 200) ? (3000 - i*7) : 1600;
		delay_us(d);
	}
}

void rotateDegrees(uint16_t degrees, uint8_t direction)
{
	// 각도에 해당하는 스텝수 계산
	uint16_t steps = (uint16_t)((uint32_t)(degrees * STEPS_PER_REVOLUTION) / 360);

	rotateSteps(steps, direction);
}

//void rotateDegrees(uint16_t degrees, uint8_t direction)
//{
//	uint16_t steps = (uint16_t)(((uint32_t)degrees * STEPS_PER_REVOLUTION) / 360);
//	rotateSteps(steps, direction);
//}

