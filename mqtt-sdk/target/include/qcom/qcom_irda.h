#ifndef __CUST_IRDA_H__
#define __CUST_IRDA_H__

/**
 * This function is used to set IR TX pin
 *	
 * @param pin		       IR TX pin (e.g., 8)
 */
void ir_tx_pin(int pin);

/**
 * This function is used to send IR key value
 *	
 * @param addr1	       custom address 1 
 * @param addr2	       custom address 2 
 * @param key		key value
 */
void ir_send(unsigned char addr1,unsigned char addr2, unsigned char key);

#endif

