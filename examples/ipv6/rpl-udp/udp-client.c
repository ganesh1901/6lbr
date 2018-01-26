/*
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
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ip/uip-udp-packet.h"
#include "sys/ctimer.h"
#ifdef WITH_COMPOWER
#include "powertrace.h"
#endif
#include <stdio.h>
#include <string.h>

/** Serial line headers */
#include "dev/serial-line.h"
#include "dev/uart1.h"
extern int serial_data_len;
int Rcvd_len = 0;


#include "net/ipv6/uip-ds6-route.h"

#define UDP_CLIENT_PORT 61616
#define UDP_SERVER_PORT 61616

#define DATA_TIMEOUT 0.4

#define UDP_EXAMPLE_ID  190

#define DEBUG DEBUG_FULL
#include "net/ip/uip-debug.h"

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])


static struct uip_udp_conn *client_conn;
static uip_ipaddr_t server_ipaddr;

#define DATA_BANK_SIZE 16
#define SERIAL_DATA_SIZE 128

#define PACKET_SIZE 64


static unsigned char data_bank[DATA_BANK_SIZE][SERIAL_DATA_SIZE];
static unsigned char data_bank_RcvdLen[DATA_BANK_SIZE];

static uint8_t data_bank_indx = 0;

static uint8_t  wr_indx =0;
static uint8_t rd_indx = 0;

static struct ctimer data_timer;
static process_event_t event_data_ready;

/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
PROCESS(post_data_process, "Push Data on Network");
AUTOSTART_PROCESSES(&udp_client_process,&post_data_process);
/*---------------------------------------------------------------------------*/

static void
tcpip_handler(void)
{
  char *str;
  int x;

  if(uip_newdata()) {
    str = uip_appdata;
    str[uip_datalen()] = '\0';
    for(x=0;x<uip_datalen();x++)
	    printf("%c",str[x]);

    uip_ipaddr_copy(&server_ipaddr, &UIP_IP_BUF->srcipaddr);
  }
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Client IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(uip_ds6_if.addr_list[i].isused &&
       (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
	uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void
set_global_address(void)
{
  uip_ipaddr_t ipaddr;

  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

/* The choice of server address determines its 6LoWPAN header compression.
 * (Our address will be compressed Mode 3 since it is derived from our
 * link-local address)
 * Obviously the choice made here must also be selected in udp-server.c.
 *
 * For correct Wireshark decoding using a sniffer, add the /64 prefix to the
 * 6LowPAN protocol preferences,
 * e.g. set Context 0 to fd00::. At present Wireshark copies Context/128 and
 * then overwrites it.
 * (Setting Context 0 to fd00::1111:2222:3333:4444 will report a 16 bit
 * compressed address of fd00::1111:22ff:fe33:xxxx)
 *
 * Note the IPCMV6 checksum verification depends on the correct uncompressed
 * addresses.
 */
 
#if 1
/* Mode 1 - 64 bits inline */
   uip_ip6addr(&server_ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);
#elif 1
/* Mode 2 - 16 bits inline */
  uip_ip6addr(&server_ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0x00ff, 0xfe00, 1);
#else
/* Mode 3 - derived from server link-local (MAC) address */
  uip_ip6addr(&server_ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0x0250, 0xc2ff, 0xfea8, 0xcd1a); //redbee-econotag
#endif
}
/*--------------------------------------------------------------------------*/
static void timedout_postdata_toroot(void *nodata_cnt_ptr) {


	ctimer_reset(&data_timer);

	if ( ( wr_indx != rd_indx )){
		process_post(&post_data_process, event_data_ready, &data_bank_indx);
	}
}
/*--------------------------------------------------------------------------*/
PROCESS_THREAD(post_data_process,ev,data) {

	int i=0 ;
	int total_packets=0 ,total_bytes =0 ,left_over_bytes =0 ;

	unsigned char rf_data[2][64] ;

	PROCESS_BEGIN();

	printf("in post_data_process \n");
	

	while(1) {

		PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);

#if 1 
		memset(rf_data,0x00,sizeof rf_data);

		total_bytes = data_bank_RcvdLen[rd_indx] ;
		memcpy(&rf_data[0][0],data_bank[rd_indx],total_bytes);

//	        printf("Read index %d  \n",rd_indx);

		total_packets = total_bytes / PACKET_SIZE ;

		left_over_bytes =  total_bytes % PACKET_SIZE;
		
//		printf("Total-packets ==>%d leftoverbytes==>%d \n",total_packets,left_over_bytes);

		for( i =0;i<total_packets ;i++)
		{
			uip_udp_packet_sendto(client_conn,&rf_data[i][0],PACKET_SIZE,&server_ipaddr,UIP_HTONS(UDP_SERVER_PORT));
		}

		if ( left_over_bytes > 0)

		{
			if(PACKET_SIZE < total_bytes) 
				uip_udp_packet_sendto(client_conn,&rf_data[1][0],left_over_bytes,&server_ipaddr,UIP_HTONS(UDP_SERVER_PORT));
			else
				uip_udp_packet_sendto(client_conn,&rf_data[0][0],left_over_bytes,&server_ipaddr,UIP_HTONS(UDP_SERVER_PORT));
		}

#endif
		rd_indx ++ ;
		if( rd_indx >= DATA_BANK_SIZE)
			rd_indx =0 ;
#if 0		
		memset(&serial_data[0],j,PACKET_SIZE);
		j++;
		uip_udp_packet_sendto(&client_conn,&serial_data[0],PACKET_SIZE,&master_hht_addr,UIP_HTONS(UDP_SERVER_PORT));
		if(j > 90 )
			j =65;
#endif

#if 0		
		total_packets = total_serial_datalen / PACKET_SIZE ;

		for(i=0;i<total_packets;i++) {

			packet_len = i*PACKET_SIZE;

			uip_udp_packet_sendto(&client_conn,&serial_data[packet_len],PACKET_SIZE,&master_hht_addr,UIP_HTONS(UDP_SERVER_PORT));
			printf("\n start delay  === [%d]\n",i);
			clock_wait(100);
			printf("\n  end delay \n");

			

			//for(j=packet_len;j<packet_len+PACKET_SIZE;j++)
			//	printf("%c ",serial_data[j]);
			
		}
		total_serial_datalen =0 ;		
#endif		

	}
	PROCESS_END();


}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{

  PROCESS_BEGIN();

  PROCESS_PAUSE();

  set_global_address();
	
  uart1_set_input(serial_line_input_byte);
  serial_line_init();

  PRINTF("UDP client process started nbr:%d routes:%d\n",
         NBR_TABLE_CONF_MAX_NEIGHBORS, UIP_CONF_MAX_ROUTES);

  print_local_addresses();

  /* new connection with remote host */
  client_conn = udp_new(NULL, 0, NULL); 
  //client_conn = udp_new(NULL, UIP_HTONS(UDP_SERVER_PORT), NULL); 
  if(client_conn == NULL) {
    PRINTF("No UDP connection available, exiting the process!\n");
    PROCESS_EXIT();
  }
  udp_bind(client_conn, UIP_HTONS(UDP_CLIENT_PORT)); 

  PRINTF("Created a connection with the server ");
  PRINT6ADDR(&client_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n",
		  UIP_HTONS(client_conn->lport), UIP_HTONS(client_conn->rport));	
  /* allocate process event for serial data place on RF */
  event_data_ready = process_alloc_event(); 

  /* timer for checking the serial port data */
  ctimer_set(&data_timer,CLOCK_SECOND*DATA_TIMEOUT,timedout_postdata_toroot,NULL);

  while(1) {
	  PROCESS_YIELD();
	  if(ev == tcpip_event) {
		  tcpip_handler();
	  }

	  if(ev == serial_line_event_message ) {

		  Rcvd_len = serial_data_len ;
		  serial_data_len = 0 ;

		  memcpy(&data_bank[wr_indx][0],(char *)data+0,Rcvd_len);
		  data_bank_RcvdLen[wr_indx] = Rcvd_len ;
		  wr_indx++;

		  if(wr_indx >= DATA_BANK_SIZE)
			  wr_indx = 0;
	  }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
