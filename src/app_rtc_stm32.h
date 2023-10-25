/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef APP_RTC_STM32_H_
#define APP_RTC_STM32_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/posix/time.h>
#include <zephyr/drivers/counter.h>

#include <string.h>

#define TIMER DT_INST(0, st_stm32_rtc)

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   10000

int8_t app_rtc_stm32_init(const struct device *dev);
int8_t app_rtc_stm32_handler(const struct device *dev);

#endif /* APP_RTC_STM32_H_ */
