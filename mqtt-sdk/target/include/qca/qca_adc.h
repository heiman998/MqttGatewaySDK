#ifndef _QCA_ADC_H_JLASJFDLKSJKLS_
#define _QCA_ADC_H_JLASJFDLKSJKLS_

/**
* @fn void qca_adc_init(int channel)
* @brief init adc
* @param channel adc channel, could be 0-7
* @return
*/
void qca_adc_init(int channel);

/**
* @fn int qca_adc_read(int channel)
* @brief adc read
* @param channel adc channel, could be 0-7
* @return
*/
int qca_adc_read(int channel);

/**
* @fn void qca_adc_close(void)
* @brief close adc
* @return
*/
void qca_adc_close(void);

#endif
