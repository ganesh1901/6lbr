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
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/rpl/rpl.h"

#include "net/netstack.h"
#include "dev/button-sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <net/packetbuf.h>


#include "net/ipv6/uip-ds6.h"


/** serial line headers*/
#include "dev/serial-line.h"
#include "dev/uart1.h"
extern int serial_data_len ;
int Rcvd_len = 0 ;
 
/** debug headers*/
#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"
#define harish 0

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#include "net/ipv6/uip-ds6-route.h"

/** UDP headers */
static struct uip_udp_conn *server_conn;
#define UDP_CLIENT_PORT	61616
#define UDP_SERVER_PORT	61616

#define UDP_EXAMPLE_ID  190

/** LPR FIRMWARE VERSION*/
#define NODE_VERSION "S.1.0"


#define SUPPRESS_BROADCAST_SEQNO 0
#define SUPPRESS_UNICAST_SEQNO 0


uip_ipaddr_t paired_node_addr;
unsigned char Mac_id[16]; 

#define PRINTADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7])

/*-------------------------------------------------------------------------*/
PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process);
/*---------------------------------------------------------------------------*/
void nbr_lookup_add(const uip_ipaddr_t *ipaddr)
{
	uip_ds6_nbr_t *nbr;
	nbr = uip_ds6_nbr_lookup(ipaddr);

	if(nbr == NULL)
	{
#if SER_DEBUG
		printf("neighbour Not found  adding to lookup\n");
#endif
		uip_lladdr_t *lladdr = (uip_lladdr_t *)uip_ds6_nbr_get_ll(nbr);
		uip_ds6_nbr_add(ipaddr,lladdr,0,NBR_REACHABLE,NBR_TABLE_REASON_IPV6_ND,NULL);
	}
}
/*---------------------------------------------------------------------------*/
static void 
Mac_Id_Set(unsigned char *MACID){
	uip_ip6addr(&paired_node_addr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0,(uint16_t)((MACID[0])<<8|MACID[1]),(uint16_t)((MACID[2]<<8)|MACID[3]),(uint16_t)((MACID[4]<<8)|MACID[5]),(uint16_t)((MACID[6]<<8)|MACID[7]));
	nbr_lookup_add(&paired_node_addr);
#ifdef DEBUG
	PRINT6ADDR(&paired_node_addr);
#endif
}
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
  char *appdata;
  int x ;

  if(uip_newdata()) {
    appdata = (char *)uip_appdata;
    appdata[uip_datalen()] = 0;

    for(x=0;x<uip_datalen();x++)
	    PRINTF("%c",appdata[x]);
#if RSSI
    printf("RSSI-->[%d]dBm\n",(signed short)packetbuf_attr(PACKETBUF_ATTR_RSSI)); 
#endif

#if SERVER_REPLY
    PRINTF("DATA sending reply\n");
    uip_ipaddr_copy(&server_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
    uip_udp_packet_send(server_conn, "Reply", sizeof("Reply"));
    uip_create_unspecified(&server_conn->ripaddr);
#endif
  }
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Server IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(state == ADDR_TENTATIVE || state == ADDR_PREFERRED) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
	uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
static void
Print_route_info(){

	uip_ds6_route_t *r;
	for(r = uip_ds6_route_head();r != NULL;r = uip_ds6_route_next(r)) {
		PRINT6ADDR(&r->ipaddr);
		PRINTF("\n");

	}
}
/*---------------------------------------------------------------*/
static void
custom_frame()
{
  frame802154_t params;
  int hdr_len;


  /* init to zeros */
  memset(&params, 0, sizeof(params));

  /* Build the FCF. */
  params.fcf.frame_type = packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE);
  params.fcf.frame_pending = packetbuf_attr(PACKETBUF_ATTR_PENDING);
  if(packetbuf_holds_broadcast()) {
	  params.fcf.ack_required = 0;
	  params.fcf.sequence_number_suppression = SUPPRESS_BROADCAST_SEQNO;
  } else {
	  params.fcf.ack_required = packetbuf_attr(PACKETBUF_ATTR_MAC_ACK);
	  params.fcf.sequence_number_suppression = SUPPRESS_UNICAST_SEQNO;
  }
  /* We do not compress PAN ID in outgoing frames, i.e. include one PAN ID (dest by default)
   *    * There is one exception, seemingly a typo in Table 2a: rows 2 and 3: when there is no
   *       * source nor destination address, we have dest PAN ID iff compression is *set*. */
  params.fcf.panid_compression = 0;

  /* Insert IEEE 802.15.4 version bits. */
  params.fcf.frame_version = FRAME802154_VERSION;

#if LLSEC802154_USES_AUX_HEADER
  if(packetbuf_attr(PACKETBUF_ATTR_SECURITY_LEVEL)) {
	  params.fcf.security_enabled = 1;
  }
  /* Setting security-related attributes */
  params.aux_hdr.security_control.security_level = packetbuf_attr(PACKETBUF_ATTR_SECURITY_LEVEL);
#if LLSEC802154_USES_FRAME_COUNTER
  params.aux_hdr.frame_counter.u16[0] = packetbuf_attr(PACKETBUF_ATTR_FRAME_COUNTER_BYTES_0_1);
  params.aux_hdr.frame_counter.u16[1] = packetbuf_attr(PACKETBUF_ATTR_FRAME_COUNTER_BYTES_2_3);
#else /* LLSEC802154_USES_FRAME_COUNTER */
  params.aux_hdr.security_control.frame_counter_suppression = 1;
  params.aux_hdr.security_control.frame_counter_size = 1;
#endif /* LLSEC802154_USES_FRAME_COUNTER */
#if LLSEC802154_USES_EXPLICIT_KEYS
  params.aux_hdr.security_control.key_id_mode = packetbuf_attr(PACKETBUF_ATTR_KEY_ID_MODE);
  params.aux_hdr.key_index = packetbuf_attr(PACKETBUF_ATTR_KEY_INDEX);
  params.aux_hdr.key_source.u16[0] = packetbuf_attr(PACKETBUF_ATTR_KEY_SOURCE_BYTES_0_1);
#endif /* LLSEC802154_USES_EXPLICIT_KEYS */
#endif /* LLSEC802154_USES_AUX_HEADER */

  /* sequence number. */
  params.seq = packetbuf_attr(PACKETBUF_ATTR_MAC_SEQNO);

  /* Complete the addressing fields. */
  /**
   *      \todo For phase 1 the addresses are all long. We'll need a mechanism
   *           in the rime attributes to tell the mac to use long or short for phase 2.
   *              */
  if(LINKADDR_SIZE == 2) {
	  /* Use short address mode if linkaddr size is short. */
	  params.fcf.src_addr_mode = FRAME802154_SHORTADDRMODE;
  } else {
	  params.fcf.src_addr_mode = FRAME802154_LONGADDRMODE;
  }
  params.dest_pid = frame802154_get_pan_id();

  if(packetbuf_holds_broadcast()) {
	  /* Broadcast requires short address mode. */
	  params.fcf.dest_addr_mode = FRAME802154_SHORTADDRMODE;
	  params.dest_addr[0] = 0xFF;
	  params.dest_addr[1] = 0xFF;
  } else {
	  linkaddr_copy((linkaddr_t *)&params.dest_addr,
			  packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
	  /* Use short address mode if linkaddr size is small */
	  if(LINKADDR_SIZE == 2) {
		  params.fcf.dest_addr_mode = FRAME802154_SHORTADDRMODE;
	  } else {
		  params.fcf.dest_addr_mode = FRAME802154_LONGADDRMODE;
	  }
  }

  /* Set the source PAN ID to the global variable. */
  params.src_pid = frame802154_get_pan_id();

  /*
   *    * Set up the source address using only the long address mode for
   *       * phase 1.
   *          */
  linkaddr_copy((linkaddr_t *)&params.src_addr,
		  packetbuf_addr(PACKETBUF_ADDR_SENDER));

//  uip_create_linklocal_allnodes_mcast(&params.dest_addr);

  params.payload = packetbuf_dataptr();
  params.payload_len = packetbuf_datalen();
  hdr_len = frame802154_hdrlen(&params);
  if(packetbuf_hdralloc(hdr_len)) {
	  frame802154_create(&params, packetbuf_hdrptr());

	  PRINTF("15.4-OUT: %2X", params.fcf.frame_type);
	  PRINTADDR(params.dest_addr);
	  PRINTF("%d %u (%u)\n", hdr_len, packetbuf_datalen(), packetbuf_totlen());

	 // return hdr_len;
  } else {
	  PRINTF("15.4-OUT: too large header: %u\n", hdr_len);
	 // return FRAMER_FAILED;
  }

 NETSTACK_RADIO.send(packetbuf_hdrptr(),packetbuf_totlen());
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  uip_ipaddr_t ipaddr;
  struct uip_ds6_addr *root_if;

  PROCESS_BEGIN();

  PROCESS_PAUSE();
 
 /** print the self address */
  print_local_addresses();

  PRINTF("UDP server started. nbr:%d routes:%d\n",
         NBR_TABLE_CONF_MAX_NEIGHBORS, UIP_CONF_MAX_ROUTES);

/** By default UIP_CONF_ROUTER  added to rpl-udp  */
#if UIP_CONF_ROUTER

#ifdef ANALOGICS_DAG_ROOT
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
#else
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
#endif
  
  uip_ds6_addr_t *return_value =  uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);

#if harish
  printf(" return from uip_ds6_addr_add ");
  printf("--->ISSUED-[%d] -- STATE-[%d] --type--[%d] --isinfinite[%d]-- ",return_value->isused,return_value->state,return_value->type,return_value->isinfinite);
  PRINT6ADDR(&return_value->ipaddr);
  printf("\n");
#endif
  
  root_if = uip_ds6_addr_lookup(&ipaddr);
  if(root_if != NULL) {
    rpl_dag_t *dag;
    dag = rpl_set_root(RPL_DEFAULT_INSTANCE,(uip_ip6addr_t *)&ipaddr);
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &ipaddr, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
#endif /* UIP_CONF_ROUTER */
  

  /* The data sink runs with a 100% duty cycle in order to ensure high 
     packet reception rates. */
  NETSTACK_MAC.off(1);
  server_conn = udp_new(NULL, UIP_HTONS(0), NULL);
  if(server_conn == NULL) {
    PRINTF("No UDP connection available, exiting the process!\n");
    PROCESS_EXIT();
  }
  udp_bind(server_conn, UIP_HTONS(UDP_CLIENT_PORT));

  PRINTF("Created a server connection with remote address ");
  PRINT6ADDR(&server_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n", UIP_HTONS(server_conn->lport),
         UIP_HTONS(server_conn->rport));

   /* initialize serial line */
  uart1_set_input(serial_line_input_byte);
  serial_line_init();


  while(1) {

	  PROCESS_YIELD();
	  if(ev == tcpip_event) {
#if harish
		  PRINTF(" TCP_IP EVNET WOKE UP\n");
#endif
		  tcpip_handler();
	  } 
	  else if ( ev == serial_line_event_message ) {

		 
		  Rcvd_len = serial_data_len ;
		  serial_data_len = 0 ;

		  if ( strncmp ( data ,"RPR#",4) == 0 ){
#if harish
			  printf(" initiate the local route repair \n");
#endif
			  rpl_repair_root(RPL_DEFAULT_INSTANCE);
		  }
		  else if ( strncmp (data,"MA1#" , 4) == 0 ) {			  
			  memcpy(Mac_id,(char *)data+4,Rcvd_len-4);
			  Mac_Id_Set( Mac_id );

		  }
		  else if( strncmp (data,"IPP#" ,4) == 0 ){
			 uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
			 uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
			 PRINT6ADDR(&ipaddr);
		  }
		  else if( strncmp (data,"RTI#",4) == 0 ){
			  //printf(" No .of nodes configured--%d \n",uip_ds6_get_addr_number(-1));
			  Print_route_info();

		  }
		  else if( strncmp (data,"VER#",4) == 0 ){
			  printf("%s\n",NODE_VERSION);
		  }
		  else if(strncmp(data,"CUS#",4) == 0){
			  printf("In customframe \n");
			  custom_frame();
			  
		  }
		  else if(){
		  }
		  else {
			  uip_udp_packet_sendto( server_conn,(char *)data+0,Rcvd_len,&paired_node_addr,UIP_HTONS(UDP_SERVER_PORT) );
		  }

	  }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
