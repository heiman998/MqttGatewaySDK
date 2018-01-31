/*
 * Copyright (c) 2011 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_TARGET_C$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
                     
#include "lz.h"

#define LZSUFFIX ".z77"

char *ifname;
char ofname[1024];

char *progname;
int compressing; /* 1-->compressing; 0-->uncompressing */

int
main( int argc, char *argv[] )
{
  FILE          *ifp, *ofp;
  int            nbytes;
  struct stat    statbuf;
  unsigned char *inbuf, *outbuf;
  unsigned int   inbufsz, outbufsz;

  progname = argv[0];
  
  if( argc != 2 )
  {
    fprintf(stderr, "Usage: %s filename (to compress into filename.z77)\n", progname);
    fprintf(stderr, "       %s filename.z77 (to uncompress into filename)\n", progname);
    exit(1);
  }

  ifname = argv[1];
  
  if( stat( ifname, &statbuf ) )
  {
    fprintf(stderr, "Cannot stat file %s", ifname);
    perror(ifname);
    exit(1);
  }
  
  if( (ifp=fopen(ifname, "r")) == NULL  )
  {
    fprintf(stderr, "Cannot open input file: %s\n", ifname);
    exit(1);
  }

  if (strcmp(&(ifname[strlen(ifname)-strlen(LZSUFFIX)]), LZSUFFIX) == 0)
  {
    compressing = 0; /* uncompressing */
    strncpy(ofname, ifname, strlen(ifname)-strlen(LZSUFFIX)); /* DROP suffix */
  } else {
    compressing = 1; /* compressing */
    strcpy(ofname, ifname);
    strcat(ofname, LZSUFFIX); /* ADD suffix */
  }

  if( (ofp=fopen(ofname,"w")) == NULL )
  {
    fprintf(stderr, "Cannot create output file: %s\n", ofname);
    perror(ofname);
    exit(1);
  }
  
  inbufsz = statbuf.st_size;
  inbuf  = (unsigned char *)malloc(inbufsz);

  outbufsz = compressing ? (statbuf.st_size + 257)
                         : (4*statbuf.st_size);
  outbuf = (unsigned char *)malloc(outbufsz);
  
  if( !( inbuf && outbuf ) )
  {
    fprintf(stderr, "Insufficient memory (%d+%d).\n", inbufsz, outbufsz);
    exit(1);
  }
  
  if(fread(inbuf, 1, inbufsz, ifp) != inbufsz)
  {
    fprintf(stderr, "Error reading %d bytes from file %s\n", inbufsz, ifname);
    perror(progname);
    exit(1);
  }

  fclose( ifp );

  if (compressing) { 
      unsigned int *work = (unsigned *)
          malloc(sizeof(int) * (inbufsz + 65536));
      if (!work) { 
          fprintf(stderr, "Cannot allocate fast comp buff\n");
          nbytes = LZ_Compress(inbuf, outbuf, inbufsz);
      } else {
          nbytes = LZ_CompressFast(inbuf, outbuf, inbufsz, work);
      }
  } else {
      nbytes = LZ_Uncompress(inbuf, outbuf, inbufsz);
  }  


  
#if 0
  nbytes = compressing ? LZ_Compress(inbuf, outbuf, inbufsz)
                       : LZ_Uncompress(inbuf, outbuf, inbufsz);
#endif

  /* sanity check that we didn't overrun the output buffer */
  if (nbytes > outbufsz) {
    /*
     * We'll get this on highly compressed test data;
     * but it should't occur in real life situations.
     */
    fprintf(stderr, "%s error -- output buffer too small for input\n", progname);
    exit(1);
  }
  
  if( fwrite(outbuf, 1, nbytes, ofp) != nbytes )
  {
    fprintf(stderr, "Error writing %d bytes to file %s\n", nbytes, ofname);
    perror(progname);
    exit(1);
  }
  
  fclose( ofp );
}
