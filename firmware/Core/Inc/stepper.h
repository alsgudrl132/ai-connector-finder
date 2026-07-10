/*
 * stempper.h
 *
 *  Created on: Jun 18, 2025
 *      Author: psh
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"
#include "delay_us.h"

#define STEPS_PER_REVOLUTION	4096		// Half step
#define DIR_CW					0			// 시계 방향
#define DIR_CCW					1			// 반시계 방향

#define CAROUSEL_STEP_US		1600		// 연속 회전 정상 속도 (공진 구간 5000µs 회피)

#define IN1_Pin					GPIO_PIN_1
#define IN1_GPIO_port			GPIOB
#define IN2_Pin					GPIO_PIN_15
#define IN2_GPIO_port			GPIOB
#define IN3_Pin					GPIO_PIN_14
#define IN3_GPIO_port			GPIOB
#define IN4_Pin					GPIO_PIN_13
#define IN4_GPIO_port			GPIOB

void stepMotor(uint8_t step);
void stepperOff(void);
void stepperRequestStop(void);
void rotateSteps(uint16_t steps, uint8_t direction);
void rotateDegrees(uint16_t degrees, uint8_t direction);
void rotateCarouselContinuous(uint8_t direction);


#endif /* INC_STEPPER_H_ */
