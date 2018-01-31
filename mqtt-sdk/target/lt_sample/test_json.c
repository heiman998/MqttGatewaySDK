/*
    qca_aes test case

    Add the code below to you project to test

>    extern void test_aes(void);
>    test_aes();
 */
#include "sample_main.h"
#include "qca_json.h"

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;
	json_t *json;
	
	json = json_decode(text);
	if (!json) {
		printf("Error before: [%s]\n", json_get_error_ptr());
	}
	else{
		json_t *format = json_get_object_item(json, "format"); 
		if(format && !format->child){
				printf("%s\n", format->valuestring);
		}

		out = json_encode(json);
		json_delete(json);
		printf("%s\n",out);
		json_free(out);
	}
}

/* Used by some code below as an example datatype. */
struct record { 
	const char *precision;
	double lat,lon;
	const char *address,*city,*state,*zip,*country;
};

/* Create a bunch of objects as demonstration. */
void create_objects()
{
	json_t *root,*fmt,*img,*thm,*fld;
	char *out;
	int i;	/* declare a few. */
	/* Our "days of the week" array: */
	const char *strings[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	/* Our matrix: */
	int numbers[3][3] = {{0,-1,0},{1,0,0},{0,0,1}};
	/* Our "gallery" item: */
	int ids[4] = {116,943,234,38793};
	/* Our array of "records": */
	struct record fields[2] = {
		{"zip",37.7668,-1.223959e+2, "", "SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2, "", "SUNNYVALE","CA","94085","US"}
	};

	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
	root = json_create_object();	
	json_add_item_to_object(root, "name", json_create_string("Jack (\"Bee\") Nimble"));
	json_add_item_to_object(root, "format", fmt = json_create_object());
	json_add_string_toObject(fmt, "type", "rect");
	json_add_number_toObject(fmt, "width", 1920);
	json_add_number_toObject(fmt, "height", 1080);
	json_add_false_toObject (fmt, "interlace");
	json_add_number_toObject(fmt, "frame rate", 24);
	
	out = json_encode(root);	
	json_delete(root);
	printf("%s\n",out);	
	json_free(out);	/* Print to text, Delete the json_t, print it, release the string. */

	/* Our "days of the week" array: */
	root = json_create_string_array(strings, 7);

	out = json_encode(root);	
	json_delete(root);	
	printf("%s\n", out);	
	json_free(out);

	/* Our matrix: */
	root = json_create_array();
	for(i = 0; i < 3; i++){
		json_add_item_toarray(root,json_create_int_array(numbers[i], 3));
	}

	/*	cJSON_ReplaceItemInArray(root,1,json_create_string("Replacement")); */
	out = json_encode(root);	
	json_delete(root);	
	printf("%s\n",out);
	json_free(out);

	/* Our "gallery" item: */
	root = json_create_object();
	json_add_item_to_object(root, "Image", img = json_create_object());
	json_add_number_toObject(img, "Width", 800);
	json_add_number_toObject(img, "Height", 600);
	json_add_string_toObject(img, "Title", "View from 15th Floor");
	json_add_item_to_object(img, "Thumbnail", thm = json_create_object());
	json_add_string_toObject(thm, "Url", "http:/*www.example.com/image/481989943");
	json_add_number_toObject(thm, "Height", 125);
	json_add_string_toObject(thm, "Width", "100");
	json_add_item_to_object(img, "IDs", json_create_int_array(ids, 4));

	out = json_encode(root);	
	json_delete(root);	
	printf("%s\n", out);	
	json_free(out);

	/* Our array of "records": */
	root = json_create_array();
	for(i = 0; i < 2; i++){
		json_add_item_toarray(root,fld = json_create_object());
		json_add_string_toObject(fld, "precision", fields[i].precision);
		json_add_number_toObject(fld, "Latitude", fields[i].lat);
		json_add_number_toObject(fld, "Longitude", fields[i].lon);
		json_add_string_toObject(fld, "Address", fields[i].address);
		json_add_string_toObject(fld, "City", fields[i].city);
		json_add_string_toObject(fld, "State", fields[i].state);
		json_add_string_toObject(fld, "Zip", fields[i].zip);
		json_add_string_toObject(fld, "Country", fields[i].country);
	}
	
	/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",json_create_int_array(ids,4)); */
	out = json_encode(root);	
	json_delete(root);	
	printf("%s\n", out);	
	json_free(out);
}

void test_json(void) 
{
	printf("json testing......\r\n");
	
	/* a bunch of json: */
	char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
	char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[] = "[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[] = "{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[] = "[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";

	/* Process each json textblock by parsing, then rebuilding: */
	doit(text1);
	doit(text2);	
	doit(text3);
	doit(text4);
	doit(text5);

	/* Now some samplecode for building objects concisely: */
	//create_objects();
	
	printf("json test done.\r\n");
	return 0;
}

