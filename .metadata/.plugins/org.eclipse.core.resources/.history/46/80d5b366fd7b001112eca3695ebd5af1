/*
 * stepper.c
 *
 *  Created on: Jun 18, 2025
 *      Author: psh
 */
#include "stepper.h"

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
	HAL_GPIO_WritePin(IN1_GPIO_port, IN2_Pin, HALF_STEP_SEQ[step][1]);
	HAL_GPIO_WritePin(IN1_GPIO_port, IN3_Pin, HALF_STEP_SEQ[step][2]);
	HAL_GPIO_WritePin(IN1_GPIO_port, IN4_Pin, HALF_STEP_SEQ[step][3]);
}

void rotateSteps(uint16_t steps, uint8_t direction)
{
	for(uint16_t i = 0; i < steps; i++)
	{
		// 회전 방향에 따라 스텝패턴
		uint8_t step;
		if(direction == DIR_CW)
		{
			step = i % 8;
		}
		else
		{
			step = 7 - (i % 8);
		}
		stepMotor(step);

		delay_us(1000);
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

