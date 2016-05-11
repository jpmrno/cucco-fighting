#include <stdio.h>
#include "../../../comapi/include/library.h"

// las fucniones individuales del switch las podiramos separar, medio feo asi

int process_request(data_t* data){
	opcode_t opt = data->opcode;
	
	switch(opt){
		CASE LIST:{
			listc_cucco_t* list_struct = malloc(sizeof(list_cucco_t));
			if(list_struct ==  NULL){
				return -1;
			}
			
			list_struct-> count = getCuccoAmount();
			list_struct-> cucco_list = getCuccos();
			data->dataStruct = malloc(sizeof(list_cucco_t));
			if(data->dataStruct == null){
				return -1;
			}
			memcpy(data->dataStruct, list_struct, sizeof(list_struct));
			//aca habria que enviar la respuesta, buscar la funcion que hace eso yver que devuelve, si lo hizo con exito o no
			break;		
		}
		CASE BET:{
			
			break;
		}
		CASE WALLET:{
			
			break;
		}
		CASE KACHING:{
			
			break;
		}
		CASE EXIT:{
			
			break;
		}
		CASE LOGIN:{
			
			break;
		}
		CASE NEW_ACC:{
			
			break;
		}
		
	}
}