/*
 * Copyright (c) 2004-2005 Atheros Communications Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/branches/olca/host/tools/regDbGen/regDomainGenDb.c#5 $
 * $DateTime: 2008/12/01 17:16:51 $
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

void generateBinFile(int dbType, char * fileName);
struct dbMasterTable_t * getMasterTableEntry(int tag);
int generate_h_file(char *pBuf, int sz, char *pFileName);
int main();

/*
 * Static mapping of Tags to data structures would be done at the generator
 * by instantiating and populating the dbMasterTable_t
 */
struct dbMasterTable_t g_dbTableGen[MAX_DB_TABLE_TAGS] =
		{
			{0, 0, KEY_BASED_TABLE_SEARCH, "RE",
					sizeof(regDomainPairs), (A_CHAR *)regDomainPairs},
			{0, 0, KEY_BASED_TABLE_SEARCH, "RE",
					sizeof(allCountries), (A_CHAR *)allCountries},
			{0, 0, INDEX_BASED_TABLE_ACCESS, "RE",
					sizeof(regDmn5GhzFreq), (A_CHAR *)regDmn5GhzFreq},
			{0, 0, INDEX_BASED_TABLE_ACCESS, "RE",
					sizeof(regDmn2Ghz_BG_Freq), (A_CHAR *)regDmn2Ghz_BG_Freq},
			{0, 0, KEY_BASED_TABLE_SEARCH, "RE",
					sizeof(regDomains), (A_CHAR *)regDomains}
		};


/*
 * ToDo : Compare functions for all the tables - All tables are saved in sorted
 * form. This would allow us to use binary search during run time in target...
 * This may increase the code size in target for binary search. Speed vs code size!!
 * Since code size is critical AND that this search is infrequent, we will not
 * do binary search in target...
 */


int main()
{

	int	i, numOfEntries, entrySize;
	struct dbMasterTable_t * pTable = NULL;

	/* Populate the numOfEntries in master table */
	for(i = 0; i < MAX_DB_TABLE_TAGS; i++)
	{
		if ( (pTable = getMasterTableEntry(i)) )
		{
			switch(i)
			{
				case REG_DMN_PAIR_MAPPING_TAG:
					numOfEntries = REGDB_NUM_OF_ROWS(regDomainPairs);
					entrySize    = sizeof(regDomainPairs[0]);
					break;

				case REG_COUNTRY_CODE_TO_ENUM_RD_TAG:
					numOfEntries = REGDB_NUM_OF_ROWS(allCountries);
					entrySize    = sizeof(allCountries[0]);
					break;

				case REG_DMN_FREQ_BAND_regDmn5GhzFreq_TAG:
					numOfEntries = REGDB_NUM_OF_ROWS(regDmn5GhzFreq);
					entrySize    = sizeof(regDmn5GhzFreq[0]);
					break;

				case REG_DMN_FREQ_BAND_regDmn2Ghz11_BG_Freq_TAG:
					numOfEntries = REGDB_NUM_OF_ROWS(regDmn2Ghz_BG_Freq);
					entrySize    = sizeof(regDmn2Ghz_BG_Freq[0]);
					break;

				case REG_DOMAIN_TAG:
					numOfEntries = REGDB_NUM_OF_ROWS(regDomains);
					entrySize    = sizeof(regDomains[0]);
					break;

				default:
					assert(0);
					break;
			}

			pTable->numOfEntries = numOfEntries;
			pTable->entrySize = entrySize;
		}
		else
		{
			assert(0);
		}
	}

    printf("Generating : %s\n", REG_DATA_FILE_AG);
	generateBinFile(WLAN_11AG_CAPABILITY, REG_DATA_FILE_AG);

    /* For G only DB, erase the A info in reg codes */
    for(i = 0; i < REGDB_NUM_OF_ROWS(regDomains); i++)
        memset(regDomains[i].chan11a, 0, sizeof(regDomains[i].chan11a));

    printf("Generating : %s\n", REG_DATA_FILE_G);
	generateBinFile(WLAN_11G_CAPABILITY, REG_DATA_FILE_G);

	return 0;
}


void
generateBinFile(int dbType, char *pFileName)
{
	char	fileBuf[9000], *pFileBuf;
	int		i, fd = 0, dbVer;
	struct  dbMasterTable_t *pSuperTable = NULL;
    char    delimitor[] = {0xde, 0xad, 0xbe, 0xef};

	memset(fileBuf, 0, sizeof(fileBuf));
	pFileBuf = &fileBuf[0];

    /* Insert Reg DB magic key */
    memcpy(pFileBuf, REG_DB_KEY, MAGIC_KEY_SZ);
    pFileBuf += MAGIC_KEY_SZ;

    /* Leave space for Regulatory DB version. Initially, it would be my tele extn: 5379 */
    dbVer = REG_DB_VER;
    for(i = 0; i < VERSION_SZ; i++)
    {
         pFileBuf[VERSION_SZ - i - 1] = dbVer % 10;
         dbVer /= 10;
    }

    pFileBuf += VERSION_SZ;

    /* length of the file - To be filled at the end of this fn */
    pFileBuf += FILE_SZ_SZ;

    /* What type of DB is it? G-Only or A... */
    *pFileBuf = dbType;

    /* First byte has data, the rest is for alignment */
    pFileBuf += DB_TYPE_SZ;

	/* Populate individual data structures */
	for(i = 0; i < MAX_DB_TABLE_TAGS; i++)
	{
        /* Ensure that offset is 32-bit */
        assert(!((pFileBuf - fileBuf) & 0x3));
        /* For MODE_11G DB, 
         *   - Don't populate the G freq blocks
         */
        if(dbType == WLAN_11G_CAPABILITY && i == REG_DMN_FREQ_BAND_regDmn5GhzFreq_TAG)
            continue;

        printf("Offset : %d  ", pFileBuf-fileBuf);
		pSuperTable = getMasterTableEntry(i);
		if( !pSuperTable )
			assert(0);

		*pFileBuf = i;
        /* First byte has data, the rest is for alignment */
        pFileBuf += DB_TAG_SZ;

		memcpy(pFileBuf, pSuperTable, sizeof(*pSuperTable));
        /* DataPtr field value has no meaning in target when
         * read from data stream. Populate this with 'deadbeef'
         * as a signature to verify on target
         */
        memcpy(pFileBuf + offsetof(struct  dbMasterTable_t, dataPtr),
                    delimitor, sizeof(pSuperTable->dataPtr));
		pFileBuf += sizeof(*pSuperTable);

		memcpy(pFileBuf, pSuperTable->dataPtr, pSuperTable->tableSize);
		pFileBuf += pSuperTable->tableSize;
		printf("Tag = %d "
				"NumOfEntries = %d "
				"EntrySize = %d "
				"Tablesize = %d\n",
				i, pSuperTable->numOfEntries,
				pSuperTable->entrySize,
				pSuperTable->tableSize);
	}

	/* Update the size */
	REGDB_SET_FILESIZE(fileBuf, (pFileBuf - fileBuf));

	/* Open the output file - for binary file*/
	if( (fd = open(pFileName, O_CREAT | O_RDWR | O_TRUNC,
                                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
	{
		printf("ERROR: opnening file...\n");
		assert(0);
	}

	/* Write the data */
	if( write(fd, (void *)fileBuf, REGDB_GET_FILESIZE(fileBuf)) == -1)
	{
		printf("ERROR: Writing data to file...\n");
		assert(0);
	}

	close(fd);

	generate_h_file(fileBuf, REGDB_GET_FILESIZE(fileBuf), pFileName);

}

int
generate_h_file(char *pBuf, int sz, char *pFileName)
{
	int	    i;
	FILE	*fp;
    char    headerFile[30];

    strcpy(headerFile, pFileName);
    strcat(headerFile, ".h");

	/* Open the output file - for *.h file to be compiled in to target*/
	fp = fopen(headerFile, "w");

    fprintf(fp, "/*\n"
                " * This file is automatically generated data file.\n"
                " * DO NOT EDIT THIS FILE\n"
                " */\n");

	fprintf(fp, "char regStaticData_t[] = {\n\t");

	for(i = 0; i < sz; i++)
	{
		if(i)
			fprintf(fp, ", ");

		if(i && (i % 10) == 0)
			fprintf(fp, "\n\t");

		fprintf(fp, "0x%02x", pBuf[i] & 0xFF); 
	}

	fprintf(fp, "\n};\n");
	fclose(fp);

    return  0;
}


struct dbMasterTable_t *
getMasterTableEntry(int tag)
{

	if(tag < MAX_DB_TABLE_TAGS)
		return	&g_dbTableGen[tag];
	else
	{
		printf("ERROR: Wrong TAG!!! Tag = %d\n", tag);
		return NULL;
	}
		return NULL;
}


