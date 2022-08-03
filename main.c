#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
 int find_size(void *arr, int len) {
    int size = 0;
	for ( int i = 0; i < len; i++) { //parcurgem vectorul
		int type = 0, lentemp = 0;
		memcpy(&type, arr + size, 1); //copiem tipul
		memcpy(&lentemp, arr + size + 1, 4); //copiem lungimea
		size += lentemp + 5; //marim size-ul dupa fiecare copiere
	 }
	 return size;
}

int add_last(void **arr, int *len, data_structure *data)
{   if(!arr) 
	    return -1; //daca este null
	int size = find_size(*arr, *len);
	if((*len) == 0) { //daca vectorul este gol alocam memorie
	   (*arr) = malloc(5 + data->header->len);
    } else { //daca nu este gol 
	    //realocam cat avem nevoie
		void* aux = realloc((*arr), size + data->header->len + 5);
    	if(!aux) {
			printf("realocarea nu a reusit");
			return 0;
		}
		*arr = aux;
	}
	memcpy((*arr) + size, &(data->header->type), sizeof(data->header->type)); 
	memcpy((*arr) + size + sizeof(data->header->type), &(data->header->len),
			 sizeof(data->header->len));
	memcpy((*arr) + size + sizeof(data->header->type) +
			 sizeof(data->header->len), data->data, data->header->len);
	(*len)++; //crestem marimea vectorului
	return 1;  //in cazul in care nu am reusit sa copiem datele
}

int add_at(void **arr, int *len, data_structure *data, int index)
{  
	int size = find_size(*arr, *len); 
    if (index >= *len) { //atunci adaugam la sfarsitul vectorului
    	return add_last(&(*arr), &(*len), data);
	}	
	int pozitie = find_size(*arr, index);//pozitia unde sa adaugam elementul
    (*arr)= realloc((*arr), size + data->header->len + 5);
    if(!(*arr)) {
		printf("realocarea nu a reusit");
		return 0;
	}
	//deplasam de la dreapta la stanga elementele din vector
	/*incepand cu pozitia vectorului pana la pozitia unde se afla
	  elementul pe care vrem sa-l introducem*/
    for(int i = size - 1; i>= pozitie; i--) {
		*(char*)(*arr + i + 5 + data->header->len) = *(char*)(*arr + i);
		memmove((*arr) + i + 5 + data->header->len, *arr + i, 1);
	}
	//incepem sa copiem noile date in vector
	memcpy(*arr + pozitie, &(data->header->type), sizeof(data->header->type));
	memcpy((*arr) + pozitie + sizeof(data->header->type), &(data->header->len), 
			sizeof(data->header->len));
	memcpy((*arr) + pozitie + sizeof(data->header->type) +
			 sizeof(data->header->len), data->data, data->header->len);
	(*len)++; //crestem marimea vectorului
	return 1;  //in cazul in care nu am reusit sa copiem datele
}

void find(void *data_block, int len, int index) 
{
	if(index >= len || index < 0)
		return;
	int size = find_size(data_block, index); //pozitia unde se afla indexul
	head* header = calloc(1, sizeof(head));
	//punem type-ul corespunzator pozitiei pe care vrem sa o gasim
	//respectiv len-ul
	header->type = *(char*)(data_block + size);
	header->len = *(int32_t*)(data_block + size + 1);
	size += 5; //5 reprezentand dimensiunea header-ului

	char* dedicator = calloc(1, strlen((char*)(data_block + size)) + 1);
	strcpy(dedicator, (char*)(data_block + size));//adaugam primul char
	size += strlen(dedicator) + 1; 

	int8_t sum1_8 = 0;
	int8_t sum2_8 = 0;
	int16_t sum1_16 = 0;
	int32_t sum2_16 = 0;
	int32_t sum1_32 = 0;
	int32_t sum2_32 = 0;

	//adaugam int-urile in functie de tipul de date
	if(header->type == 1) {
		memcpy(&sum1_8, data_block + size, sizeof(int8_t)); 
		size += sizeof(int8_t);
		memcpy(&sum2_8 , data_block + size, sizeof(int8_t));
		size += sizeof(int8_t);

	} else if (header->type == 2) {
		memcpy(&sum1_16, data_block + size, sizeof(int16_t));
		size += sizeof(int16_t);
		memcpy(&sum2_16 , data_block + size, sizeof(int32_t));
		size += sizeof(int32_t);

	} else if(header->type == 3) {
		memcpy(&(sum1_32), data_block + size, sizeof(int32_t));
		size += sizeof(int32_t);
		memcpy(&(sum2_32), data_block + size, sizeof(int32_t));
		size += sizeof(int32_t);
	}

	char* dedicatul = calloc(1, strlen((char*)(data_block + size)) + 1); 
	strcpy(dedicatul, (char*)(data_block + size)); //adaugam al doilea char
	size += strlen(dedicatul) + 1;

    printf("Tipul %d\n", header->type); //incepem sa printam pe rand datele
	printf("%s pentru %s\n", dedicator, dedicatul);
	if(header->type == 1) {
		printf("%hhd\n%hhd\n\n", sum1_8, sum2_8);

	} else if (header->type == 2) {
		printf("%hd\n%d\n\n", sum1_16, sum2_16);	

	} else if(header->type == 3) {
		printf("%d\n%d\n\n", sum1_32, sum2_32);

	} 
    //dezalocam memoria alocata
	free(header);
	free(dedicator);
	free(dedicatul);
}

int delete_at(void **arr, int *len, int index)
{  
    int arr_size = find_size(*arr, *len); //lungimea vectorului arr
    int del_size = find_size(*arr, index); //pozitia elem de eliminat
    int deleted_size;

    head* aux = malloc(sizeof(aux));
    if(!aux) {
	   printf("alocarea nu a reusit");
	   return 0;
    }

	aux->type = *(char*)(*arr + del_size);
    aux->len = *(int*)(*arr + del_size + 1);
	deleted_size = aux->len + 5; 
    free(aux);

    memcpy(*arr + del_size, *arr + deleted_size + del_size, 
			arr_size - deleted_size - del_size); //deplasam vectorul la stanga

    if(arr_size - deleted_size > 0) {
		//realocam vectorul daca am eliminat un element
		(*arr)  = realloc(*arr, arr_size - deleted_size); 
		if(!(*arr)) {
			printf("realocarea nu a reusit");
			return 0;
    	}
    } else 
		free(*arr); //eliberam memoria daca era singurul element

    (*len)--; //micsoram lungimea vectorului
    return 1;
}

void print(void *arr, int len) { 
	//aici este acelasi rationament ca la functia de find
	//chiar mai simplist
    int size = 0;
	for(int i = 0; i < len; i++) {
		head* header = malloc(sizeof(head));
		memcpy(&(header->type), arr + size, sizeof(header->type));
		memcpy(&(header->len), arr + size + sizeof(header->type),
				 sizeof(header->len));
		size += sizeof(header->type) + sizeof(header->len);

		char* dedicator = calloc(1, strlen((char*)(arr + size)) + 1);
		strcpy(dedicator, (char*)(arr + size));
		size += strlen(dedicator) + 1;

		int8_t sum1_8 = 0;
		int8_t sum2_8 = 0;
		int16_t sum1_16 = 0;
		int32_t sum2_16 = 0;
		int32_t sum1_32 = 0;
		int32_t sum2_32 = 0;

		if(header->type == 1) {
			memcpy(&sum1_8, arr + size, sizeof(int8_t));
			size += sizeof(int8_t);
			memcpy(&sum2_8 , arr + size, sizeof(int8_t));
			size += sizeof(int8_t);

		} else if (header->type == 2) {
			memcpy(&sum1_16, arr + size, sizeof(int16_t));
			size += sizeof(int16_t);
			memcpy(&sum2_16 , arr + size, sizeof(int32_t));
			size += sizeof(int32_t);

		} else if(header->type == 3) {
			memcpy(&(sum1_32), arr + size, sizeof(int32_t));
			size += sizeof(int32_t);
			memcpy(&(sum2_32), arr + size, sizeof(int32_t));
			size += sizeof(int32_t);
		}

		char* dedicatul = calloc(1, strlen((char*)(arr +  size))+ 1);
		strcpy(dedicatul, (char*)(arr + size));
		size += strlen(dedicatul) + 1;
        printf("Tipul %d\n", header->type);
	    printf("%s pentru %s\n", dedicator, dedicatul);
	    if(header->type == 1) {
			printf("%"PRId8"\n%"PRId8"\n\n", sum1_8, sum2_8);

		} else if (header->type == 2) {
			printf("%"PRId16"\n%"PRId32"\n\n", sum1_16, sum2_16);
			
		} else if(header->type == 3) {
			printf("%"PRId32"\n%"PRId32"\n\n", sum1_32, sum2_32);
		}

		free(header);
		free(dedicator);
		free(dedicatul);
    }
}

int main() { 
	void *arr = NULL;
	int len = 0;
    char* line= NULL;
    size_t buffer = 256;
	data_structure* data = calloc(1, sizeof(*data));
	if(!data) {
		printf("alocarea nu a reusit");
		return 0;
	}

	data->header = calloc(1, sizeof(*data->header));
	if(!data->header) {
		printf("alocarea nu a reusit");
		return 0;
	}

    while(getline(&line, &buffer, stdin) != -1) {
	   line[strcspn(line, "\n")] = '\0';
	   char vector[7][256];
	   int nr = 0;
	   char* token = strtok(line," ");
	   while(token) { //pune in vector elementele de pe fiecare linie citita
			nr++;
		    strcpy(vector[nr - 1], token);
		    token = strtok(NULL, " ");
	    }
	    if(!strcmp(vector[0], "insert")) { 
			//aflam type-ul
			data->header->type = (unsigned char) atoi(vector[1]); 
			//alocam memorie pentru primul char
			data->data = calloc(1, strlen(vector[2]) + 1 );
			if(!data->data) {
				printf("alocarea nu a reusit");
				return 0;
		    }
			//copiem char-ul
		    memcpy(data->data, vector[2], strlen(vector[2]) + 1);
		    data->header->len = strlen(vector[2]) + 1; //crestem size-ul 

		    if(data->header->type == 1) { //pentru tipul 1 de date
				/*pentru fiecare tip de date alocam memorie in functie
				  de ce tip de int este si apoi copiem in data->data*/
		    	data->data= realloc(data->data, data->header->len 
						 					 + 2 * sizeof(int8_t));
				if(!data->data) {
					printf("realocare nu a reusit");
					return 0;
			    }
				int8_t val1 = (int8_t) atoi(vector[3]);
				int8_t val2 = (int8_t) atoi(vector[4]);

				memcpy(data->data + data->header->len, &val1, sizeof(int8_t));
				data->header->len += sizeof(int8_t);
				memcpy(data->data + data->header->len, &val2, sizeof(int8_t));
				data->header->len += sizeof(int8_t);
			}

		    else if(data->header->type == 2) {
		    	data->data = realloc(data->data, data->header->len +
						 sizeof(int16_t) + sizeof(int32_t));
				if(!data->data) {
					printf("realocare nu a reusit");
					return 0;
			    }
				int16_t val2 =(int16_t) atoi(vector[3]);
				int32_t val3 = (int32_t)atoi(vector[4]);

				memcpy(data->data + data->header->len, &val2, sizeof(int16_t));
				data->header->len = data->header->len + sizeof(int16_t);
				memcpy(data->data + data->header->len, &val3, sizeof(int32_t));
				data->header->len = data->header->len + sizeof(int32_t);

		   } else if(data->header->type == 3) {
		    	data->data = realloc(data->data, data->header->len +
						 2 * sizeof(int32_t));
				if(!data->data) {
					printf("realocare nu a reusit");
					return 0;
			    }
				int32_t val4 = (int32_t)atoi(vector[3]);
				int32_t val5 = (int32_t)atoi(vector[4]);
				
				memcpy(data->data + data->header->len, &val4, sizeof(int32_t));
				data->header->len = data->header->len + sizeof(int32_t);
				memcpy(data->data + data->header->len, &val5, sizeof(int32_t));
				data->header->len = data->header->len + sizeof(int32_t);
		    } 

            void* aux = realloc(data->data, data->header->len + 
						strlen(vector[5]) + 1); 
			if(!aux) {
				printf("realocarea nu a reusit");
				return 0;
			}
			data->data = aux;
			memcpy(data->data + data->header->len, vector[5],
					 strlen(vector[5]) + 1);  //copiem si al doilea char
			data->header->len = data->header->len + strlen(vector[5]) + 1;
		    add_last(&arr, &len, data);
			free(data->data);

		} else if(!strcmp(vector[0], "insert_at")) {
			data->header->type = atoi(vector[2]); //tipul de date
			data->data = calloc(1, strlen(vector[3]) + 1 );
			if(!data->data) {
				printf("alocarea nu a reusit");
				return 0;
		    }
		    memcpy(data->data, vector[3], strlen(vector[3]) + 1);
		    data->header->len = strlen(vector[3]) + 1;

		    if(data->header->type == 1) { //pentru tipul 1 de date
		    	data->data= realloc(data->data, data->header->len +
						 2 * sizeof(int8_t)); //realocam pentru 2 int-uri
				if(!data->data) {
					printf("realocare nu a reusit");
					return 0;
			    }
			int8_t val1 = (int8_t) atoi(vector[4]);
            int8_t val2 = (int8_t) atoi(vector[5]);

			memcpy(data->data + data->header->len, &val1, sizeof(int8_t));
			data->header->len += sizeof(int8_t);
			memcpy(data->data + data->header->len, &val2, sizeof(int8_t));
			data->header->len += sizeof(int8_t);

		} else if(data->header->type == 2) {
		    data->data = realloc(data->data, data->header->len +
					 sizeof(int16_t) + sizeof(int32_t));
			if(!data->data) {
				printf("realocare nu a reusit");
				return 0;
			}
			int16_t val2 =(int16_t) atoi(vector[4]);
			int32_t val3 = (int32_t)atoi(vector[5]);

			memcpy(data->data + data->header->len, &val2, sizeof(int16_t));
			data->header->len = data->header->len + sizeof(int16_t);
			memcpy(data->data + data->header->len, &val3, sizeof(int32_t));
			data->header->len = data->header->len + sizeof(int32_t);

		} else if(data->header->type == 3) {
		    data->data = realloc(data->data, data->header->len +
					 2 * sizeof(int32_t));
			if(!data->data) {
				printf("realocare nu a reusit");
				return 0;
			}			
			int32_t val4 = (int32_t)atoi(vector[4]);
			int32_t val5 = (int32_t)atoi(vector[5]);

			memcpy(data->data + data->header->len, &val4, sizeof(val4));
			data->header->len = data->header->len + sizeof(val4);
			memcpy(data->data + data->header->len, &val5, sizeof(val5));
			data->header->len = data->header->len + sizeof(val5);
		} 
        	data->data = realloc(data->data, data->header->len +
					 strlen(vector[6]) + 1); 
			memcpy(data->data + data->header->len, vector[6], 
					 strlen(vector[6]) + 1);  //copiem si al doilea char
			data->header->len = data->header->len + strlen(vector[6]) + 1;
			add_at(&arr, &len, data, atoi(vector[1]));
            free(data->data);

	    } else if(!strcmp(vector[0], "delete_at")) {
			delete_at(&arr, &len, atoi(vector[1]));

	    } else if(!strcmp(vector[0], "find")) {
			find(arr, len, atoi(vector[1]));  

	    } else if(!strcmp(vector[0], "print")) {
			print(arr, len);

	    } else if(!strcmp(vector[0], "exit")) {
			free(data->header);
        	free(data);
			free(line);
			buffer = 0;
			free(arr);
			exit(EXIT_SUCCESS);  
		}  
	}
	 return -1;
}
