/*
 * Copyright (c) 2015, Swedish Institute of Computer Science.
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

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_



/* Define as minutes */
#define RPL_CONF_DEFAULT_LIFETIME_UNIT   10

/* 10 minutes lifetime of routes */
#define RPL_CONF_DEFAULT_LIFETIME        10

#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME 1

/** newly added */

#define RF_CORE_CONF_CHANNEL 10  //changed successfully

#define SERVER_REPLY 0  //changed successfully

/** for configure server ip as a DAG_ROOT IP enble this flag */
#define ANALOGICS_DAG_ROOT 1 


/** serial line frequency for timedout*/
#define FREQUENCY_CONF 0.1

/** UART_CONF_BAUDRATE  setting */
#define CC26XX_UART_CONF_BAUD_RATE 115200 

/** RSSI enabling */
#define RSSI 0

/** default prefix when udp packet sended */
#define UIP_CONF_DS6_DEFAULT_PREFIX 0xfdaa


#define RPL_CONF_OF rpl_mrhof

#define SEC_CONF 1

#if 0
/**  security in mac layer */
#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED          1
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER              noncoresec_framer
#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC               noncoresec_driver
#undef NONCORESEC_CONF_SEC_LVL
#define NONCORESEC_CONF_SEC_LVL           5
#define NONCORESEC_CONF_KEY { 0x4d,0xe9,0x50,0x24,0x4b,0x76,0x76,0xc3,0x4d,0x4b,0x4d,0x55,0x50,0x4e,0x43,0x5a }
#endif

/* changes done*/

#endif /* PROJECT_CONF_H_ */
