#define FUSE_USE_VERSION 26

#include "libcsv/csv.h"
#include <libgen.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fuse.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/kernel.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
		

struct postalCode{
	char code[20];
	char neighborhood[20];
	char city[20];
	char district[40];
	char latitude[20];
	char longitude[20];
	int length;
};

// struct csv_state{
// 	FILE *logfile;
//     char *rootdir;
// };

// #define CSV_DATA ((struct csv_state *) fuse_get_context()->private_data)


static const char *names = "/NAMES";
static const char *codes = "/CODES";
struct postalCode postalCodes[40000];
int sizeOfPostalCodes = 0;
FILE *fp;

void cb1 (void *s, size_t len, void *data) {
	char *line = (char *)s;
	struct postalCode new_postal_code={"","","","","",""};
	char separator[2] = "\t";
	char *new_code = strtok(line, separator);
	char *new_neighborhood = strtok(NULL, separator);
	char *new_city = strtok(NULL, separator);
	char *new_district = strtok(NULL, separator);
	char *new_latitude = strtok(NULL, separator);
	char *new_longitude = strtok(NULL, separator);
	strncpy(new_postal_code.code, new_code, 5);
	strcpy(new_postal_code.neighborhood, new_neighborhood);
	strcpy(new_postal_code.city, new_city);
	strcpy(new_postal_code.district, new_district);
	strcpy(new_postal_code.latitude, new_latitude);
	strncpy(new_postal_code.longitude, new_longitude, 7);
	new_postal_code.length = 5 + 7 + strlen(new_postal_code.neighborhood) + strlen(new_postal_code.city) + strlen(new_postal_code.district) + strlen(new_postal_code.latitude);
	/*
	printf("Code: %s\n", new_postal_code.code);
	printf("Neighborhood: %s\n", new_postal_code.neighborhood);
	printf("City: %s\n", new_postal_code.city);
	printf("District: %s\n", new_postal_code.district);
	printf("Latitude: %s\n", new_postal_code.latitude);
	printf("Longitude: %s\n", new_postal_code.longitude);
	printf("----------------------------\n");
	*/ 
	postalCodes[sizeOfPostalCodes++]=new_postal_code;
	return;
}
void cb2 (int c, void *data) {  return; }

static int is_space(unsigned char c) {
  if (c == CSV_SPACE || c == CSV_TAB) return 1;
  return 0;
}

static int is_term(unsigned char c) {
  if (c == CSV_CR || c == CSV_LF) return 1;
  return 0;
}


static int csv_getattr(const char *path, struct stat *stbuf)
{
	// Permission bits
	// 0755 - only the owner of the file could read, write and execute the directory,
	// the group’s users and other users could only read and execute the directory 
	
	// 0644 - the owner could read and write the file,
	// the group’s users and other users could only read the file 

    int res = 0;
	int i, countDepth;
	for (i=0, countDepth=0; path[i]; i++) 
		countDepth += (path[i] == '/');
    
    if (strcmp(path,"/") == 0) 
    {
		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 4;
		// .  itself
		// .. parent
		// names
		// codes
		// there are 4 link.
    }
    else if(strcmp(path,names) == 0)
    {
    	int counter = 0;
    	int last = 0;
    	for(i = 0; i < sizeOfPostalCodes; i++)
		{
			int x = atoi(postalCodes[i].code);
			int new = x/1000;
			if(last != new){
				last = new;
				counter++;
			}
		}

		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2 + counter;
		// 81 cities, kktc , 1 parent, 1 itself
	}
	else if(strcmp(path,codes) == 0)
	{
		int counter = 0;
    	int last = 0;
    	for(i = 0; i < sizeOfPostalCodes; i++)
		{
			int x = atoi(postalCodes[i].code);
			int new = x/1000;
			if(last != new){
				last = new;
				counter++;
			}
		}

		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2 + counter;
		// 81 cities, kktc , 1 parent, 1 itself
	}
	else if((countDepth==2)&&(strstr(path, codes)!=NULL)){
		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2; // Parent, itself
	}
	else if((countDepth == 2)&&(strstr(path, names)!=NULL))
	{
		char* pathString = strdup(path);
		char* cityName = basename(pathString);
		int i = 0;
		int counter = 0;
		char last[50] = "";
    	for(i = 0; i < sizeOfPostalCodes; i++)
		{	
			if(strcmp(postalCodes[i].city, cityName)==0){
				if(strcmp(last, postalCodes[i].district) != 0){
					strcpy(last, postalCodes[i].district);
					counter++;
				}
			}
		}

		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = counter + 2;
		// district number + 2
	}
	else if((countDepth==3)&&(strstr(path, codes)!=NULL))
	{
		// 20600.txt
		char* pathString = strdup(path);
		char* codeName = basename(pathString);
		int i=0;



		for(i = 0; i < sizeOfPostalCodes; i++)
		{
			if(strncmp(postalCodes[i].code, codeName, 5)==0){
				// counter += strlen(buffer);
				// printf("a %d\n", strlen(postalCodes[i].code));
				// printf("b %d\n", strlen(postalCodes[i].neighborhood));
				// printf("c %d\n", strlen(postalCodes[i].city));
				// printf("d %d\n", strlen(postalCodes[i].district));
				// printf("e %d\n", strlen(postalCodes[i].latitude));
				// printf("f %d\n", strlen(postalCodes[i].longitude));

				// stbuf->st_size += strlen(postalCodes[i].code);
				// stbuf->st_size += strlen(postalCodes[i].neighborhood);
				// stbuf->st_size += strlen(postalCodes[i].city);
				// stbuf->st_size += strlen(postalCodes[i].district);
				// stbuf->st_size += strlen(postalCodes[i].latitude);
				// stbuf->st_size += strlen(postalCodes[i].longitude);

				memset(stbuf, 0, sizeof(struct stat));
				stbuf->st_mode = S_IFREG | 0644;
				stbuf->st_nlink = 1;
				stbuf->st_size = 57 + 5 + postalCodes[i].length; // 57 fixed char, 5 End of line
				break;
			}
		}
		
	}
	else if((countDepth==3)&&(strstr(path, names)!=NULL))
	{
		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2; // Parent, itself
	}
	else if(countDepth==4)
	{
		// Names/Istanbul/Sariyer/Maslak.txt

		char* pathString1 = strdup(path);
		char* pathString2 = strdup(path);
		char* pathString3 = strdup(path);

		char* neighborhoodName = basename(pathString1);

		char* dirDistrict = dirname(pathString2);
		char* districtName = basename(dirDistrict);

		char* dirDist = dirname(pathString3);
		char* dirCity = dirname(dirDist);
		char* cityName = basename(dirCity);
		
		int neighborhoodLength = strlen(neighborhoodName) - 4; //.txt

		int i=0;
		for(i = 0; i < sizeOfPostalCodes; i++)
		{
			if(strcmp(postalCodes[i].city, cityName)==0 && strcmp(postalCodes[i].district, districtName)==0 && strncmp(postalCodes[i].neighborhood, neighborhoodName, neighborhoodLength)==0){
				memset(stbuf, 0, sizeof(struct stat));
				stbuf->st_mode = S_IFREG | 0644;
				stbuf->st_nlink = 1;
				stbuf->st_size= 57 + 5 + postalCodes[i].length; // 57 fixed char, 5 End of line;
				break;
			}
		}
	}
    else
        res = -ENOENT;

    return res;
}


static int csv_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
    
    (void) offset;
    (void) fi;
	int i=0;        
	char selected[20] = "0";
	int label = 0;
    if(strcmp(path, "/")==0){
		filler(buf, names + 1, NULL, 0);
		filler(buf, codes + 1, NULL, 0);
	}
	else if(strcmp(path, names) == 0)
	{
		for(i = 0; i < sizeOfPostalCodes; i++)
		{
			if(strcmp(postalCodes[i].city,selected)!=0){
				strcpy(selected,postalCodes[i].city);
				filler(buf, postalCodes[i].city, NULL, 0);
			}
		}
	}
	else if(strcmp(path, codes) == 0)
	{
		for(i = 0; i < sizeOfPostalCodes; i++)
		{
			int x = atoi(postalCodes[i].code);
			int result = x/1000;
			if(label != result){
				label = result;
				sprintf(selected, "%d", label);
				if(strlen(selected)==1){
					selected[1] = selected[0];
					selected[0] = '0';
					filler(buf, selected, NULL, 0);
				}
				else 
					filler(buf, selected, NULL, 0);
			}
		}
	}
	else if(strstr(path, codes) != NULL)
	{
		char *separator = "/";
		char *token = strtok(path, separator);
		char file_name[10];
		token=strtok(NULL, separator);
		for(i=0; i< sizeOfPostalCodes; i++){
			if(strncmp(token, postalCodes[i].code, 2)==0){
				strncpy(file_name, postalCodes[i].code, 5);
				file_name[5] = '.';
				file_name[6] = 't';
				file_name[7] = 'x';
				file_name[8] = 't';
				filler(buf, file_name, NULL, 0);	
			}
		}		
	}
	else if(strstr(path, names) != NULL)
	{
		int count=0;
		for (i=0, count=0; path[i]; i++) 
		count += (path[i] == '/');
		char previous[20] = " ";
		if(count==2){
			char *selectedPath = strdup(path);
			char *selectedCity = basename(selectedPath);
			for(i = 0; i < sizeOfPostalCodes; i++){
				if((strcmp(selectedCity, postalCodes[i].city)==0)&&(strcmp(previous, postalCodes[i].district)!=0)){
					strcpy(previous, postalCodes[i].district);
					filler(buf, postalCodes[i].district, NULL, 0);
				}
			}
		}
		else if(count==3){
			char *selectedPath1 = strdup(path);
			char *selectedPath2 = strdup(path);
			char *x = dirname(selectedPath2);
			char *selectedCity = basename(x);
			char *selectedDistrict = basename(selectedPath1);

			for(i=0; i<sizeOfPostalCodes; i++){
				if((strcmp(selectedCity, postalCodes[i].city)==0)&&(strcmp(selectedDistrict, postalCodes[i].district)==0)){
					char txt[] = ".txt";
					char fileName[30];
					strcpy(fileName, postalCodes[i].neighborhood);
					strcat(fileName, txt);
					filler(buf, fileName, NULL, 0);
				}
			}
		}
	}
	else
		return -ENOENT;
    return 0;
}


static int csv_open(const char *path, struct fuse_file_info *fi)
{
    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;
    return 0;
}


static int csv_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{ 
    (void) fi;
	size_t len;
	int count=0;
	int i=0;
	for (i=0, count=0; path[i]; i++) 
		count += (path[i] == '/');
	char codeSection[] = "code: ";
	char neighborhoodSection[] = "\nneigborhood: ";
	char citySection[] = "\ncity: ";
	char districtSection[] = "\ndistrict: ";
	char latitudeSection[] = "\nlatitude: ";
	char longitudeSection[] = "\nlongitude: ";
	char longString[200];
	if(strstr(path, names)!=NULL){
		char *selectedPath1 = strdup(path);
		char *selectedPath2 = strdup(path);
		char *selectedPath3 = strdup(path);
		char *x = dirname(selectedPath2);
		char *z = dirname(x);
		char *selectedCity = basename(z);
		char *y = dirname(selectedPath1);
		char *selectedDistrict = basename(y);
		char *selectedNeighborhood = basename(selectedPath3);
		int sizeOfToken = strlen(selectedNeighborhood);
		for(i=0; i<sizeOfPostalCodes; i++){
			if((strncmp(selectedNeighborhood, postalCodes[i].neighborhood, sizeOfToken-4)==0)&&(strcmp(selectedDistrict, postalCodes[i].district)==0)&&(strcmp(selectedCity,postalCodes[i].city)==0)){
				strcpy(longString, codeSection);
				strncat(longString, postalCodes[i].code, 5);
				strcat(longString, neighborhoodSection);
				strcat(longString, postalCodes[i].neighborhood);
				strcat(longString, citySection);
				strcat(longString, postalCodes[i].city);
				strcat(longString, districtSection);
				strcat(longString, postalCodes[i].district);
				strcat(longString, latitudeSection);
				strcat(longString, postalCodes[i].latitude);
				strcat(longString, longitudeSection);
				strcat(longString, postalCodes[i].longitude);
				strcpy(buf, longString);
			}
		}
	}
	else if(strstr(path, codes)!=NULL){
		char *selectedPath = strdup(path);
		char *token = basename(selectedPath);
		for(i=0; i<sizeOfPostalCodes; i++){
			if(strncmp(token, postalCodes[i].code, 5)==0) {
				strcpy(longString, codeSection);
				strncat(longString, postalCodes[i].code, 5);
				strcat(longString, neighborhoodSection);
				strcat(longString, postalCodes[i].neighborhood);
				strcat(longString, citySection);
				strcat(longString, postalCodes[i].city);
				strcat(longString, districtSection);
				strcat(longString, postalCodes[i].district);
				strcat(longString, latitudeSection);
				strcat(longString, postalCodes[i].latitude);
				strcat(longString, longitudeSection);
				strcat(longString, postalCodes[i].longitude);
				strcpy(buf, longString);
			}
		}
	}
    return size;
}


void readCsv(){
	struct csv_parser p;
	char buf[1024];
	size_t bytes_read;
	fp = fopen("postal-codes.csv", "rb");
	if (csv_init(&p, 0) != 0) {
		fprintf(stderr, "Failed to initialize csv parser\n");
		exit(EXIT_FAILURE);
	}
	csv_set_space_func(&p, is_space);
	csv_set_term_func(&p, is_term);
	

    if (!fp) {
      fprintf(stderr, "Failed to open %s: %s\n", "postal-codes.csv", strerror(errno));
    }
    

    while ((bytes_read=fread(buf, 1, 1024, fp)) > 0) {
      if (csv_parse(&p, buf, bytes_read, cb1, cb2, &postalCodes) != bytes_read) {
        fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(&p)));
      }
    }
	
    csv_fini(&p, cb1, cb2, &postalCodes);

    if (ferror(fp)) {
      fprintf(stderr, "Error while reading file\n");
      fclose(fp);
    }

    fclose(fp);  

  csv_free(&p);
}

/*
static int csv_unlink(const char *path){
	char fpath[PATH_MAX];
	strcpy(fpath, CSV_DATA->rootdir);
	strncat(fpath, path, PATH_MAX);
	return unlink(fpath);
}


static int csv_rename(const char*path, const char *newpath){
	char fpathOld[PATH_MAX];
	char fpathNew[PATH_MAX];
	strcpy(fpathOld, CSV_DATA->rootdir);
	strcpy(fpathNew, CSV_DATA->rootdir);
	strncat(fpathOld, path, PATH_MAX);
	strncat(fpathNew, newpath, PATH_MAX);
	return rename(path, newpath);
}
*/
static struct fuse_operations csv_oper = {
	.getattr	= csv_getattr,
    .readdir	= csv_readdir,
    .open		= csv_open,
    .read		= csv_read,
 //   .unlink 	= csv_unlink,
 // .rename		= csv_rename,
};

int main(int argc, char *argv[]){
	readCsv();
    
	// struct csv_state *csv_data;
	// csv_data = malloc(sizeof(struct csv_state));
	// csv_data->rootdir = realpath("postal-codes.csv", NULL);
	return fuse_main(argc, argv, &csv_oper, NULL);
}
