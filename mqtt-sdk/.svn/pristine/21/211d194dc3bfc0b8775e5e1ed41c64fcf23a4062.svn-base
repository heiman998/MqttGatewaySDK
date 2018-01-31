/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>

#include "athdefs.h"
#include "wlan_defs.h"
#include "athtypes.h"
#include "regDb.h"
#ifdef FPGA
#include "fpgaRegTableData.h"
#else
#include "regTableData.h"
#endif

typedef enum{
	false,
	true,
}bool;

typedef enum{
	HT20 = 0,
	HT40PLUS,
	HT40MINUS,
	HT80,
}HTMODE;

/*for rate */
static char *rateStr[] = {"F", "H", "Q"};
#define RATE_F  0
#define RATE_H  1
#define RATE_Q  2

/*mode string it is defined according to:
* #define MODE_11A	0
* #define MODE_11G	1
* #define MODE_11B	2
*/
static char *modeStr[] = {"A", "G", "B", "NA", "NG"};

/*return center frequency by HT mode and band
  return 0 if error or not support now*/
unsigned short centerFreq(unsigned short freq, HTMODE HT, bool is5G)
{
	unsigned short cFreq = 0;

	switch(HT) {
		case HT20:
			cFreq = freq;	
			break;
		case HT40PLUS:
		case HT40MINUS:
			if(is5G) {  /*don't care about plus or minus*/
				if(freq <= 5720 && freq >= 5180) { /*channel 36~144*/
					if((freq % 8) == 0) {  /*mod by eight, center freq = freq - 10*/
						cFreq = freq - 10;
					}else {
						cFreq = freq + 10;
					}
				}else { /*channel 149 ~ 161*/
					if(freq <= 5765)
						cFreq = 5755;	/*5745(149), 5765(153)*/
					else
						cFreq = 5795;   /*5785(157), 5805(161)*/
				}	
			}else {
				if(HT == HT40PLUS){
					if(freq <= 2452 && freq >= 2412) {  /*channel 1~9*/
						cFreq = freq + 10;
					}
				}else{
					if(freq <=2472 && freq >= 2432) {  /*channel 5~13*/
						cFreq = freq - 10;
					}
				}
			}
			break;
		case HT80:
			/*TODO not support yet*/
			break;
		default:	
			break;
	}

	return cFreq;
}

REG_DMN_PAIR_MAPPING *findRegDmnPairByEnum(unsigned short regDmnEnum)
{
	int i;

	for(i=0; i<sizeof(regDomainPairs)/sizeof(REG_DMN_PAIR_MAPPING); i++){
		if(regDomainPairs[i].regDmnEnum == regDmnEnum){
			/*found*/
			return &regDomainPairs[i];
		}
	}
	/*not found*/
	return NULL;
}

REG_DOMAIN *findRegDomainByEnum(unsigned regDmnEnum)
{
	int i;

	for(i=0; i<sizeof(regDomains)/sizeof(REG_DOMAIN); i++){
		if(regDomains[i].regDmnEnum == regDmnEnum){
			/*found*/
			return &regDomains[i];
		}
	}
	/*not found*/
	return NULL;
}

void printFreqInfo(REG_DMN_PAIR_MAPPING *regDomainPair, COUNTRY_CODE_TO_ENUM_RD *country, 
		   REG_DOMAIN *regDmnCur, REG_DMN_FREQ_BAND *freqBand, int *channel_no)
{
	int freq, idx;
	bool pscan = false, adhoc = true, dfs=false, ht20=false;
	char *ht40CapStr = NULL;
	unsigned char mode;
	unsigned short cFreq = 0;
	int rate = RATE_F;

	for(freq=freqBand->lowChannel, idx=0;
			freq<=freqBand->highChannel; 
			freq+=freqBand->channelSep, idx++){

		mode = freqBand->mode;
		
		/*
		 * If the current regcode is country code AND
		 * Country code does not allow 11g AND
		 * the current channel is a 11G channel,
		 * then force the mode as 11B
		 */
		if(country){
			if( !(country->allowMode & OFDM_YES) && (mode == MODE_11G)) {
				mode = MODE_11B;
			}
		}

		ht40CapStr = NULL; /* stupid code....I am sorry*/

		if(mode == MODE_11A) {
			if((cFreq = centerFreq(freq, HT40PLUS, true)) != 0) {  /*5G don't care + or -*/
				if((freqBand->ht40ChanMask >> ((cFreq - freqBand->lowChannel) / 
								freqBand->channelSep)) & 0x1) {
					ht40CapStr = "HT40";
				}
			}
		}else {
			if((cFreq = centerFreq(freq, HT40MINUS, false)) != 0) {
				if((freqBand->ht40ChanMask >> ((cFreq - freqBand->lowChannel) / 
								freqBand->channelSep)) & 0x1) {
					ht40CapStr = "HT40-";
				}
			}
			if((cFreq = centerFreq(freq, HT40PLUS, false)) != 0) {
				if((freqBand->ht40ChanMask >> ((cFreq - freqBand->lowChannel) / 
								freqBand->channelSep)) & 0x1) {
					if(ht40CapStr == NULL) {
						ht40CapStr = "HT40+";
					}else {
						ht40CapStr = "HT40+/-";
					}
				}
			}
		}
		if(ht40CapStr) {
			ht20 = true;
		}

		if(freqBand->usePassScan & regDmnCur->pscan & regDomainPair->pscanMask){
			pscan = true;
		}

		if(mode == MODE_11A) {  /*11A 5G mode*/

			if((regDmnCur->flags | regDomainPair->flags5GHz) &
					(DISALLOW_ADHOC_11A | ADHOC_NO_11A) )
				adhoc = false;			
			if(regDmnCur->dfsMask & freqBand->useDfs)
				dfs = true;		
			if(country){
				if(!(country->allowMode & MCS_HT40_A_YES))
					ht40CapStr = NULL;
				if(country->allowMode & MCS_HT20_YES)
					ht20 = true;
			}
			if ( freqBand->ht40ChanMask & FREQ_QUARTER_RATE ) {
				rate = RATE_Q; /*Quarter rate index in rateStr*/
			}
			else if ( freqBand->ht40ChanMask & FREQ_HALF_RATE ) {
				rate = RATE_H; /*Half rate string index in rateStr*/
			}

		}else if((mode == MODE_11G) ||  /*11bg 2.4G mode*/
				(mode == MODE_11B) ||
				(mode == MODE_11GONLY)) {

			if((regDmnCur->flags | regDomainPair->flags2GHz) &
					DISALLOW_ADHOC_11G )
				adhoc = false;
			if(country){
				if(!(country->allowMode & MCS_HT40_G_YES))
					ht40CapStr = NULL;
				if((mode == MODE_11G) && (country->allowMode & MCS_HT20_YES))
					ht20 = true;
			}
		}else {
			printf("Error! unknow 80211 mode\n");
			exit(0);
		}
		printf("%-2d %-4d%-2s %-2.1f %-7s %-6s  %-1s  %-1s  %-1s\n", 
				++(*channel_no), 
				freq, 
				ht20? modeStr[(mode & 0x3)+3]: modeStr[mode],
				(double)freqBand->power,
				pscan? "PASSIVE": "ACTIVE",
				dfs? "DFS": "NO_DFS",
				adhoc? "Y": "N",
				ht40CapStr? "Y": "N",
				rateStr[rate]
			  );
	}
}


void printRegDmnPair( REG_DMN_PAIR_MAPPING *regDomainPair, 
			COUNTRY_CODE_TO_ENUM_RD *country)
{
	REG_DMN_FREQ_BAND *regDmnBand = NULL;
	REG_DOMAIN *regDmnCur = NULL;
	REG_DMN_FREQ_BAND *freqBand = NULL;
	int regDmnBandSize;
	unsigned int *chann_band = NULL;
	int i, channNu;
	unsigned char chann;
	int channel_no = 0;

	regDmnCur = findRegDomainByEnum(regDomainPair->regDmn5GHz);	
	regDmnBand = regDmn5GhzFreq;
	regDmnBandSize = sizeof(regDmn5GhzFreq)/sizeof(REG_DMN_FREQ_BAND);
	chann_band = regDmnCur->chan11a;
	
	for(i=0; i<BMLEN; i++){
		for(chann=0; chann<32; chann++){
			if(chann_band[i] & (1<<chann)){
				channNu = chann+(i*32);
				if(channNu >= regDmnBandSize){
					break;
				}
				freqBand = &regDmnBand[channNu];
				printFreqInfo(regDomainPair, country, regDmnCur,
					freqBand, &channel_no);
			}
		}
	}


	/*print 2.4 G channel list after 5G*/
	regDmnCur = findRegDomainByEnum(regDomainPair->regDmn2GHz);
	regDmnBand =    regDmn2Ghz_BG_Freq;
	regDmnBandSize = sizeof(regDmn2Ghz_BG_Freq)/sizeof(REG_DMN_FREQ_BAND);
	chann_band = regDmnCur->chan11bg;

	for(i=0; i<BMLEN; i++){
		for(chann=0; chann<32; chann++){
			if(chann_band[i] & (1<<chann)){
				channNu = chann+(i*32);
				if(channNu >= regDmnBandSize){
					break;
				}
				freqBand = &regDmnBand[channNu];
				printFreqInfo(regDomainPair, country, regDmnCur,
					freqBand, &channel_no);
			}
		}
	}

	return;
}

int main(int argc, char* argv[])
{	
	COUNTRY_CODE_TO_ENUM_RD *country = NULL;
	REG_DMN_PAIR_MAPPING *regDomainPair = NULL;
	int i;

	/*dump all country */
	for(i=0; i<sizeof(allCountries)/sizeof(COUNTRY_CODE_TO_ENUM_RD); i++){
		printf("Country code = %s, EEPROM code 0x%-4x\n",
				allCountries[i].isoName, allCountries[i].countryCode+0x8000);

		country = &allCountries[i];
		regDomainPair = findRegDmnPairByEnum(country->regDmnEnum);
		printRegDmnPair(regDomainPair, country);
		printf("\n");
	}

	/*dump the regulatory domain*/
	for(i=0; i<sizeof(regDomainPairs)/sizeof(REG_DMN_PAIR_MAPPING); i++){
		printf("Regcode = 0x%x, EEPROM code 0x%x\n",  
				regDomainPairs[i].regDmnEnum, regDomainPairs[i].regDmnEnum);
		regDomainPair = findRegDmnPairByEnum(regDomainPairs[i].regDmnEnum);
		printRegDmnPair(regDomainPair, NULL);
		printf("\n");
	}	
	return 0;
}


