/*
 * Copyright (c) 2015, CETIC.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         6LBR-Demo Project Configuration
 * \author
 *         6LBR Team <6lbr@cetic.be>
 */

#ifndef SENSORTAG_RESOURCES_H
#define SENSORTAG_RESOURCES_H

#define REST_CONF_PLATFORM_HAS_AMBIENT_TEMP 1

#define REST_CONF_RES_AMBIENT_TEMP_PERIODIC 1
#define REST_CONF_RES_AMBIENT_TEMP_SIMPLE 0
#define REST_CONF_RES_AMBIENT_TEMP_IF IF_SENSOR
#define REST_CONF_RES_AMBIENT_TEMP_TYPE TEMPERATURE_SENSOR_RT
#define REST_CONF_RES_AMBIENT_TEMP_ID TEMPERATURE_SENSOR_RES_ID
#define REST_CONF_RES_AMBIENT_TEMP_FORMAT COAP_RESOURCE_TYPE_DECIMAL_THREE
#define REST_CONF_RES_AMBIENT_TEMP_SO_TYPE TEMPERATURE_SENSOR
#define REST_CONF_RES_AMBIENT_TEMP_SO_INSTANCE_ID "0"
#define REST_CONF_RES_AMBIENT_TEMP_IPSO_APP_FW_ID SENSOR_PATH "amb_temp"

#define REST_CONF_PLATFORM_HAS_OBJECT_TEMP 1

#define REST_CONF_RES_OBJECT_TEMP_PERIODIC 1
#define REST_CONF_RES_OBJECT_TEMP_SIMPLE 0
#define REST_CONF_RES_OBJECT_TEMP_IF IF_SENSOR
#define REST_CONF_RES_OBJECT_TEMP_TYPE TEMPERATURE_SENSOR_RT
#define REST_CONF_RES_OBJECT_TEMP_ID TEMPERATURE_SENSOR_RES_ID
#define REST_CONF_RES_OBJECT_TEMP_FORMAT COAP_RESOURCE_TYPE_DECIMAL_THREE
#define REST_CONF_RES_OBJECT_TEMP_SO_TYPE TEMPERATURE_SENSOR
#define REST_CONF_RES_OBJECT_TEMP_SO_INSTANCE_ID "1"
#define REST_CONF_RES_OBJECT_TEMP_IPSO_APP_FW_ID SENSOR_PATH "obj_temp"

#define REST_CONF_PLATFORM_HAS_HUMIDITY 1

#define REST_CONF_RES_HUMIDITY_PERIODIC 1
#define REST_CONF_RES_HUMIDITY_SIMPLE 0
#define REST_CONF_RES_HUMIDITY_IF IF_SENSOR
#define REST_CONF_RES_HUMIDITY_TYPE RELATIVE_HUMIDITY_SENSOR_RT
#define REST_CONF_RES_HUMIDITY_ID HUMIDITY_SENSOR_RES_ID
#define REST_CONF_RES_HUMIDITY_FORMAT COAP_RESOURCE_TYPE_DECIMAL_TWO
#define REST_CONF_RES_HUMIDITY_SO_TYPE HUMIDITY_SENSOR
#define REST_CONF_RES_HUMIDITY_SO_INSTANCE_ID "0"
#define REST_CONF_RES_HUMIDITY_IPSO_APP_FW_ID SENSOR_PATH "humidity"

#define REST_CONF_PLATFORM_HAS_HUMIDITY_TEMP 1

#define REST_CONF_RES_HUMIDITY_TEMP_PERIODIC 1
#define REST_CONF_RES_HUMIDITY_TEMP_SIMPLE 0
#define REST_CONF_RES_HUMIDITY_TEMP_IF IF_SENSOR
#define REST_CONF_RES_HUMIDITY_TEMP_TYPE TEMPERATURE_SENSOR_RT
#define REST_CONF_RES_HUMIDITY_TEMP_ID TEMPERATURE_SENSOR_RES_ID
#define REST_CONF_RES_HUMIDITY_TEMP_FORMAT COAP_RESOURCE_TYPE_DECIMAL_TWO
#define REST_CONF_RES_HUMIDITY_TEMP_SO_TYPE TEMPERATURE_SENSOR
#define REST_CONF_RES_HUMIDITY_TEMP_SO_INSTANCE_ID "2"
#define REST_CONF_RES_HUMIDITY_TEMP_IPSO_APP_FW_ID SENSOR_PATH "hum_temp"

#define REST_CONF_PLATFORM_HAS_PRESSURE 1

#define REST_CONF_RES_PRESSURE_PERIODIC 1
#define REST_CONF_RES_PRESSURE_SIMPLE 0
#define REST_CONF_RES_PRESSURE_IF IF_SENSOR
#define REST_CONF_RES_PRESSURE_TYPE PRESSURE_SENSOR_RT
#define REST_CONF_RES_PRESSURE_ID PRESSURE_SENSOR_RES_ID
#define REST_CONF_RES_PRESSURE_FORMAT COAP_RESOURCE_TYPE_DECIMAL_TWO
#define REST_CONF_RES_PRESSURE_SO_TYPE BAROMETER
#define REST_CONF_RES_PRESSURE_SO_INSTANCE_ID "0"
#define REST_CONF_RES_PRESSURE_IPSO_APP_FW_ID SENSOR_PATH "pressure"

#define REST_CONF_PLATFORM_HAS_PRESSURE_TEMP 1

#define REST_CONF_RES_PRESSURE_TEMP_PERIODIC 1
#define REST_CONF_RES_PRESSURE_TEMP_SIMPLE 0
#define REST_CONF_RES_PRESSURE_TEMP_IF IF_SENSOR
#define REST_CONF_RES_PRESSURE_TEMP_TYPE TEMPERATURE_SENSOR_RT
#define REST_CONF_RES_PRESSURE_TEMP_ID TEMPERATURE_SENSOR_RES_ID
#define REST_CONF_RES_PRESSURE_TEMP_FORMAT COAP_RESOURCE_TYPE_DECIMAL_TWO
#define REST_CONF_RES_PRESSURE_TEMP_SO_TYPE TEMPERATURE_SENSOR
#define REST_CONF_RES_PRESSURE_TEMP_SO_INSTANCE_ID "3"
#define REST_CONF_RES_PRESSURE_TEMP_IPSO_APP_FW_ID SENSOR_PATH"pressure_temp"

#define REST_CONF_PLATFORM_HAS_ILLUMINANCE 1

#define REST_CONF_RES_ILLUMINANCE_PERIODIC 1
#define REST_CONF_RES_ILLUMINANCE_SIMPLE 0
#define REST_CONF_RES_ILLUMINANCE_IF IF_SENSOR
#define REST_CONF_RES_ILLUMINANCE_TYPE LIGHT_SENSOR_RT
#define REST_CONF_RES_ILLUMINANCE_ID ILLUMINANCE_SENSOR_RES_ID
#define REST_CONF_RES_ILLUMINANCE_FORMAT COAP_RESOURCE_TYPE_DECIMAL_TWO
#define REST_CONF_RES_ILLUMINANCE_SO_TYPE ILLUMINANCE_SENSOR
#define REST_CONF_RES_ILLUMINANCE_SO_INSTANCE_ID "0"
#define REST_CONF_RES_ILLUMINANCE_IPSO_APP_FW_ID SENSOR_PATH "light"

#define REST_CONF_PLATFORM_HAS_BATMON 1

#define REST_CONF_RES_BATMON_PERIODIC 1
#define REST_CONF_RES_BATMON_SIMPLE 0
#define REST_CONF_RES_BATMON_IF IF_SENSOR
#define REST_CONF_RES_BATMON_TYPE DEVICE_POWER_SUPPLY_VOLTAGE_RT
#define REST_CONF_RES_BATMON_ID DEVICE_POWER_VOLTAGE_RES_ID
#define REST_CONF_RES_BATMON_FORMAT COAP_RESOURCE_TYPE_DECIMAL_THREE

#define REST_CONF_PLATFORM_HAS_BATMON_TEMP 1

#define REST_CONF_RES_BATMON_TEMP_PERIODIC 1
#define REST_CONF_RES_BATMON_TEMP_SIMPLE 0
#define REST_CONF_RES_BATMON_TEMP_IF IF_SENSOR
#define REST_CONF_RES_BATMON_TEMP_TYPE TEMPERATURE_SENSOR_RT
#define REST_CONF_RES_BATMON_TEMP_ID TEMPERATURE_SENSOR_RES_ID
#define REST_CONF_RES_BATMON_TEMP_FORMAT COAP_RESOURCE_TYPE_SIGNED_INT
#define REST_CONF_RES_BATMON_TEMP_SO_TYPE TEMPERATURE_SENSOR
#define REST_CONF_RES_BATMON_TEMP_SO_INSTANCE_ID "4"
#define REST_CONF_RES_BATMON_TEMP_IPSO_APP_FW_ID SENSOR_PATH "batmon_temp"

#define REST_CONF_PLATFORM_HAS_BUZZER 1

#define REST_CONF_RES_BUZZER_ACTUATOR 1
#define REST_CONF_RES_BUZZER_IF IF_ACTUATOR
#define REST_CONF_RES_BUZZER_TYPE BOOLEAN_RT
#define REST_CONF_RES_BUZZER_ID STATE_SENSOR_RES_ID
#define REST_CONF_RES_BUZZER_FORMAT COAP_RESOURCE_TYPE_BOOLEAN
#define REST_CONF_RES_BUZZER_SO_TYPE BUZZER
#define REST_CONF_RES_BUZZER_SO_INSTANCE 0
#define REST_CONF_RES_BUZZER_SO_INSTANCE_ID "0"
#define REST_CONF_RES_BUZZER_IPSO_APP_FW_ID ACTUATOR_PATH "buzzer"

#endif
