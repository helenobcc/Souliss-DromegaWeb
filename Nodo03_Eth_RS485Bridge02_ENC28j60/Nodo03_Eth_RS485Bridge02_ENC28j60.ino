
/**************************************************************************
*
* Progetto   :    Nodo03 - Arduino Pro Mini + Eth ENC28j60
* Autore     :    DromegaWeb
* Scopo      :    Questo nodo è un Bridge ethrnet con RS485 verso i peer
* Data       :    21 Giugno 2015   -  Prima bozza
*                 
*                 
*
****************************************************************************/

#define USARTDRIVER_INSKETCH
#define	USARTDRIVER		Serial	//Dico al driver vNet di usare la seriale 0 dell'UNO
#define USART_TXENABLE		1
#define USART_TXENPIN		3
#define USART_DEBUG  		0

// Indirizzo MAC per Scheda Ethernet Nodo03
#define MAC_INSKETCH
uint8_t MAC_ADDRESS[] = {0x1A, 0xA6, 0x49, 0x6B, 0xBF, 0xBD};
#define AUTO_MAC         0
#define MAC_DEBUG        0

// Configure the framework
#include "bconf/StandardArduino.h"    // Uso un Arduino standard
#include "conf/ethENC28J60.h"         // Uso connessione Ethernet ENC28J60
#include "conf/usart.h"	              // uso la seriale
#include "conf/SuperNode.h"           // The main node is the SuperNode



#include <SPI.h>
#include "Souliss.h"

// -------------INIZIO definizione della configurazione di rete --DromegaWeb------------------------------------

#define ip_Router		10
#define	ip_Gateway		11			// indirizzo ip Gateway Souliss   
#define ip_Eth_N2		12			// indirizzo ip su Ethernet Nodo02
#define	ip_Eth_N3		13			// indirizzo ip su Ethernet Nodo03

uint8_t ip_address_Gw[4]  = {192, 168, 2, ip_Gateway};		// indirizzo Gateway souliss su Ethernet Arduino Mega
uint8_t ip_address_E2[4]  = {192, 168, 2, ip_Eth_N2};		// indirizzo su Ethernet Nodo02
uint8_t ip_address_E3[4]  = {192, 168, 2, ip_Eth_N3};		// indirizzo su Ethernet Nodo03
uint8_t subnet_mask[4]    = {255, 255, 255, 0};
uint8_t ip_gateway_Router[4] = {192, 168, 2, ip_Router};	// indirizzo Gateway router

#define myvNet_address		ip_address_Gw[3]		// ultimo byte dell' IP address (11) è anche l'indirizzo vNet
#define	myvNet_subnet		0xFF00
#define myvNet_supern		0x0000
#define	myvNet_supern_E2	Nodo02_Bridge01_RS485
#define	myvNet_supern_E3	Nodo03_Bridge02_RS485

#define Nodo02_Bridge01_RS485		0xCE01	// Indirizzo Nodo02 bridge01 della Seriale RS485
#define Nodo04_Bridge01_Peer01			0xCE02	// Indirizzo Nodo04 peer01 su bridge01 remota Seriale RS485
#define Nodo05_Bridge01_Peer02			0xCE03	// Indirizzo Nodo05 peer02 su bridge01 remota Seriale RS485

#define Nodo03_Bridge02_RS485		0xCF01	// Indirizzo Nodo03 bridge02 della Seriale RS485
#define Nodo06_Bridge02_Peer01			0xCF02	// Indirizzo Nodo06 peer01 su bridge02 remota Seriale RS485
#define Nodo07_Bridge02_Peer02			0xCF03	// Indirizzo Nodo07 peer02 su bridge02 remota Seriale RS485
#define Nodo08_Bridge02_Peer03			0xCF04	// Indirizzo Nodo08 peer03 su bridge02 remota Seriale RS485

// -------------FINE definizione della configurazione di rete --DromegaWeb------------------------------------

#define LIGHT1_N3			0


void setup()
{
  Souliss_SetIPAddress(ip_address_E3, subnet_mask, ip_gateway_Router);                   // set IP a 13
  Souliss_SetAddress(Nodo03_Bridge02_RS485, myvNet_subnet, myvNet_supern);    //definisce l'indirizzo del gateway RS485

  Set_T11(LIGHT1_N3);


  // Define inputs, outputs pins - Hardware pulldown required
  //pinMode(3, INPUT);   //Riservato a seriale RS 485 pin Enable
  pinMode(4, INPUT);
  
  pinMode(5, OUTPUT);
  
}


void loop()
{
  EXECUTEFAST() {
    UPDATEFAST();

    FAST_30ms()  {

      DigIn(4, Souliss_T1n_ToggleCmd, LIGHT1_N3);
     
      Logic_T11(LIGHT1_N3);
     
      DigOut(5, Souliss_T1n_Coil, LIGHT1_N3);
      
    }
    
    FAST_PeerComms();
        
    START_PeerJoin();
  }
  EXECUTESLOW() {
    UPDATESLOW();
    SLOW_10s() {
      Timer_T11(LIGHT1_N3);
      
    }
    SLOW_PeerJoin();        // riconnette se il Gateway viene riavviato
  }
}




