
regDomainDbGen:
    - This tool produces regulatory DB in binary encoded form. The input to
      this generator is the tables in file regTableData.h. It genererates
      two databases in two different formats.
      
      Types of Databases:
        - AG Database - has information for A an G regulatory info for
                        each domain. Files are regulatoryData_AG.bin.h and
                        regulatoryData_AG.bin

        - G Database -  has information for G-ONLY regulatory info for
                        each domain. Files are regulatory_G.bin.h and
                        regulatoryData_G.bin

       Types of format for DB:
        - *.bin.h   - This is in header file format, which can be included
                      in the target code and compiled in to the executatble. 
                      One of the DB file(regulatoryData_AG.bin.h or
                      regulatory_G.bin.h) should be copied to
                      etna/target/wlan/include/regStaticData.h to get it
                      compiled in to target.

        - *.bin     - This can be used to flash the regulatory DB in target
                      and accessed in target via DataSets application.
