
/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc..
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 */


#include <stdio.h>
#include <libxml2/libxml/parser.h>
#include <string.h>

//#define DEBUG 1
#if defined(DEBUG)
#define dprintf printf
#else
#define dprintf
#endif

#define TUNE_EXPORT_MAXLEN 256
#define TUNE_NAME_MAXLEN 64
#define MAX_NUM_CUST 200
#define MAX_NUM_MASTER 200
#define MAX_DESC_SIZE 1024



/* DSETID 0 :Create entry into Tuneenv.sh
 * DSETID 1 :Create ROM DSETS
 * DSETID 2 :Create SYS DSETS
 * DSETID 3 :Create VENDOR DSETS 
 * */
#define MAX_DSETS_SUPPORTED 4

#define EXPORT_STRING     ((const unsigned char *)"export")
#define EXPORT_STRING_LEN (sizeof(unsigned char)*6)


typedef struct tune_cust_element{
    unsigned char name[TUNE_NAME_MAXLEN];
    unsigned long value;
    unsigned char exports[TUNE_EXPORT_MAXLEN];
} TUNE_CUST_ELEMENT;

/*Context structure to store customer information*/
typedef struct ccxt{
    TUNE_CUST_ELEMENT tc_element[MAX_NUM_CUST];
    int cust_cnt;
}CUST_CXT;

typedef struct tune_master_element{
    unsigned char name[TUNE_NAME_MAXLEN];
    unsigned long datatype;
    unsigned long dsetid;
    unsigned long id;
    int value;
    int type;
    unsigned long l_valid;
    unsigned long h_valid;
    int operation;
    int size;
    char desc[MAX_DESC_SIZE];
} TUNE_MASTER_ELEMENT;

/*Context structure to store master list information*/
typedef struct mcxt{
    TUNE_MASTER_ELEMENT tm_element[MAX_NUM_MASTER];
    int master_cnt;
}MASTER_CXT;

CUST_CXT context;
MASTER_CXT mcxt;

/***********************************************************
 * Function tune_parse_param
 * DESC: parse the params in master list and store all 
 * information in a local buffer.
 * Return- 
 *********************************************************/

void tune_parse_param(xmlDocPtr doc,xmlNodePtr cur)
{
    unsigned int validmask =0;
    unsigned int maskbit=1;

    xmlChar *key;
    cur = cur->xmlChildrenNode;

    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"NAME"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(strlen(key) > TUNE_NAME_MAXLEN){
                xmlFree(key);
                goto NEXT;  /*skip over this node*/
            }
            strcpy(mcxt.tm_element[mcxt.master_cnt].name, key);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);

        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"DATATYPE"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].datatype = strtoul(key,0,0);
            validmask |=  maskbit; 
            maskbit= maskbit*2;
            xmlFree(key);
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"DSETID"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].dsetid = strtoul(key,0,0);
            if(strtoul(key,0,0)>MAX_DSETS_SUPPORTED){
                xmlFree(key);
                goto NEXT;
            }
            validmask |=  maskbit; 
            maskbit= maskbit*2;
            xmlFree(key);
        }


        if ((!xmlStrcmp(cur->name, (const xmlChar *)"ID"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].id = strtoul(key,0,0);
            validmask |=  maskbit; 
            maskbit= maskbit*2;


            xmlFree(key);
        }



        if ((!xmlStrcmp(cur->name, (const xmlChar *)"TYPE"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            mcxt.tm_element[mcxt.master_cnt].type = atoi(key);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"VALUE"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].value = strtoul(key,0,0);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"LOWER"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);

            mcxt.tm_element[mcxt.master_cnt].l_valid = strtoul(key,0,0);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"UPPER"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].h_valid = strtoul(key,0,0);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"OPERATION"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].operation = atoi(key);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"SIZE"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            mcxt.tm_element[mcxt.master_cnt].size = atoi(key);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"DESC"))) {
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(strlen(key) > MAX_DESC_SIZE)
                goto NEXT;  /*skip over this node*/
            strcpy(mcxt.tm_element[mcxt.master_cnt].desc, key);
            validmask |=  maskbit; 
            maskbit= maskbit*2;

            xmlFree(key);

        }


NEXT:
        cur = cur->next;
    }
    if(validmask == 0x7ff){
        mcxt.master_cnt++;
    }

    return;
}

/***********************************************************
 * Function tune_parse_master_list
 * DESC: parse the master list and store all information in buffer.
 * Return- -1 if error, 0 if success
 *********************************************************/
int tune_parse_master_list(char* docname)
{
    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);

    if (doc == NULL ) {
        printf("Document not parsed successfully. \n");
        return;
    }
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        printf("empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }

    dprintf("%s\n",cur->name);


    cur = cur->xmlChildrenNode;

    while(cur != NULL){
        tune_parse_param(doc, cur);
        cur = cur->next;
    }
    return 0;
}

/*Parse user provided parameter file*/
int tune_parse_user_input(char* file)
{
    char buf[256];
    FILE* fp;
    char* token;
    int storeName = 1, i;
    if(!file)
        return -1;

    fp = fopen(file, "r");

    if(!fp)
        return -1;
    while(fgets(buf,256,fp)) {
        int len=strlen(buf);
        if((len > EXPORT_STRING_LEN) && (strncmp(buf,EXPORT_STRING,EXPORT_STRING_LEN)==0)){
            //create entry in custlist.
            strncpy(context.tc_element[context.cust_cnt].name, EXPORT_STRING,EXPORT_STRING_LEN);
            strncpy(context.tc_element[context.cust_cnt].exports, buf,len);
            context.tc_element[context.cust_cnt].value=0;
            dprintf("%s %lx %s",context.tc_element[context.cust_cnt].name,context.tc_element[context.cust_cnt].value,context.tc_element[context.cust_cnt].exports);
            context.cust_cnt++;
            
            continue;
        }


        if(buf[0]!='#'&& buf[0]!='\n')         
        {  
            token = strtok(buf," ");

            while(token != NULL){
                if(storeName){
                    strcpy(context.tc_element[context.cust_cnt].name, token);
                    storeName = 0;
                }else{
                    context.tc_element[context.cust_cnt].value = strtoul(token,0,0);
                    context.cust_cnt++;
                    storeName = 1;
                }
                token = strtok(NULL, " ");
            }
            memset(buf,'\0',256);
        }

    }

    for(i=0;i<context.cust_cnt;i++){
        dprintf("%s %lx\n",context.tc_element[i].name,context.tc_element[i].value);
    }

    fclose(fp);
}

int tune_validate_input_and_prep_output()
{
    int user_list_entry;
    int master_list_entry;
    FILE *filep[MAX_DSETS_SUPPORTED];
    char filename[100];

    int dset_id;

    for(dset_id=1;dset_id<=MAX_DSETS_SUPPORTED;dset_id++){
        memset(filename,'\0' ,sizeof(filename)); 
        sprintf(filename,"dset_tunable_%d.bin",dset_id);
        filep[dset_id]=fopen(filename,"wb"); 
    }

    FILE *envfp=fopen("tuneenv.sh","w");

    for(user_list_entry=0;user_list_entry<context.cust_cnt;user_list_entry++){
        struct tune_cust_element * celement=&context.tc_element[user_list_entry];
        if(strncmp(celement->name,EXPORT_STRING,EXPORT_STRING_LEN)==0){
            fprintf(envfp,"%s",celement->exports);
            continue;
        }
        for(master_list_entry=0;master_list_entry<mcxt.master_cnt;master_list_entry++){
            struct tune_master_element* melement=&(mcxt.tm_element[master_list_entry]);
            if(strlen(celement->name)==strlen(melement->name)
                    &&  (strncmp(celement->name,melement->name,strlen(celement->name))==0)){
                if(!(celement->value >=melement->l_valid
                            && celement->value <=melement->h_valid))

                {
                    dprintf("Warning:: Tunable %s: has value=%lx\n",celement->name,celement->value);
                    dprintf("\tTunable %s: valid values are >= %lx and <=%lx\n",
                            melement->name,
                            melement->l_valid,
                            melement->h_valid);
                    dprintf("\tDefaulting Tunable %s to  value= %d\n",celement->name,melement->value);

                    celement->value=melement->value;            
                }else{
                    //valid value
                    //write to bin file
                    unsigned char byteArray[9];


                    memset(byteArray,0,sizeof(byteArray));
                    /*Set the Header*/
                    char op=melement->operation;
                    byteArray[0]|=(melement->operation)<<4;
                    byteArray[0]|=(melement->type)<<3;
                    byteArray[0]|=(melement->size);
                    dprintf("name: %s byte 0 %x\n",melement->name,byteArray[0]);


                    if((melement->dsetid) == 0){
                    }
                    else if((melement->dsetid) == 1){
                        /*4B-Address/value*/
                        byteArray[4]|= (((melement->id)>>24)&0xFF);  // move byte 3 to byte 0
                        byteArray[3]|=  (((melement->id)>>16)&0xFF);  // move byte 2 to byte 1
                        byteArray[2]|=  (((melement->id)>>8)&0xFF);  // move byte 1 to byte 2
                        byteArray[1]|=  (((melement->id))&0xFF); // byte 0 to byte 3

                        dprintf("id %lx byte 1-4 %x %x %x %x \n",melement->id,byteArray[1],byteArray[2],byteArray[3],byteArray[4]);

                        /*Val*/
                        byteArray[8]|= (((celement->value)>>24)&0xFF);  // move byte 3 to byte 0
                        byteArray[7]|=  (((celement->value)>>16)&0xFF);  // move byte 2 to byte 1
                        byteArray[6]|=  (((celement->value)>>8)&0xFF);  // move byte 1 to byte 2
                        byteArray[5]|=  (((celement->value))&0xFF); // byte 0 to byte 3

                        dprintf("value %lx byte 5-8 %d %d %d %d \n",celement->value,byteArray[8],byteArray[7],byteArray[6],byteArray[5]);


                        fwrite(byteArray,sizeof(unsigned char),sizeof(unsigned char)*5+melement->size,filep[melement->dsetid]);
                    } else {
                        /*2B-Offset/value*/
                        byteArray[2]|=  (((melement->id)>>8)&0xFF);  // move byte 1 to byte 0
                        byteArray[1]|=  (((melement->id))&0xFF);     // byte 0 to byte 1

                        dprintf("id %lx byte 1-2 %x %x \n",melement->id,byteArray[1],byteArray[2]);

                        /*Val*/
                        byteArray[6]|= (((celement->value)>>24)&0xFF);  // move byte 3 to byte 0
                        byteArray[5]|=  (((celement->value)>>16)&0xFF);  // move byte 2 to byte 1
                        byteArray[4]|=  (((celement->value)>>8)&0xFF);  // move byte 1 to byte 2
                        byteArray[3]|=  (((celement->value))&0xFF); // byte 0 to byte 3

                        dprintf("value %lx byte 3-6 %x %x %x %x \n",celement->value,byteArray[3],byteArray[4],byteArray[5],byteArray[6]);


                        fwrite(byteArray,sizeof(unsigned char),sizeof(unsigned char)*3+melement->size,filep[melement->dsetid]);
                    }

                    if(melement->datatype==16){
                        fprintf(envfp,"export qtune_%s=0x%lx\n",celement->name,celement->value);
                    }else{
                        fprintf(envfp,"export qtune_%s=%ld\n",celement->name,celement->value);
                    }

                }

            }
        }

    }
    for(dset_id=1;dset_id<MAX_DSETS_SUPPORTED;dset_id++){
        fclose(filep[dset_id]); 
    }

    fclose(envfp);
}

int tune_prepare_input_file(){

    int master_list_entry;
    FILE * filep=fopen("input.txt","wb"); 
    struct tune_master_element* melement;
    fprintf(filep,"# IoE SDK Tunables and Configurables\n");
    for(master_list_entry=0;master_list_entry<mcxt.master_cnt;master_list_entry++){
        melement=&(mcxt.tm_element[master_list_entry]);

        if (melement->desc && strlen(melement->desc) > 1) {
            fprintf(filep, "\n\n");
        }
        fprintf(filep,"%s",melement->desc);
        if(melement->datatype==16){
            fprintf(filep,"# %s 0x%x",melement->name,melement->value);
        }else if(melement->datatype==10){
            fprintf(filep,"# %s %d",melement->name,melement->value);
        }

    }
    fclose(filep);
}
int main(int argc, char* argv[])
{ 
    int i;

    if (argc==2){    
        printf("Preparing Input File \n");
        tune_parse_master_list(argv[1]);
        tune_prepare_input_file();
        return 0;
    }
    if(tune_parse_user_input(argv[1]) == -1){
        printf("Invalid/Missing parameters in customer input\n");
        return 1;
    }

    if(tune_parse_master_list(argv[2]) == -1){
        printf("Invalid/Missing parameters in Master List\n");
        return 1;
    }


    for(i=0;i<mcxt.master_cnt;i++){
        dprintf("%s \n dtype:%ld \n dsetid:%ld\n addr:%lx\n type;%d\n lower:%ld\n upper:%ld \n op:%d\n sz:%d\n \n",( mcxt.tm_element[i]).name,
                mcxt.tm_element[i].datatype,
                mcxt.tm_element[i].dsetid,
                mcxt.tm_element[i].id,
                mcxt.tm_element[i].type,
                mcxt.tm_element[i].l_valid,
                mcxt.tm_element[i].h_valid,
                mcxt.tm_element[i].operation,
                mcxt.tm_element[i].size
               );
    }
    tune_validate_input_and_prep_output();

    return 0;
}
