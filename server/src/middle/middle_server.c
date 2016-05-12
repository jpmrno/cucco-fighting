#include <stdio.h>
#include "../../../comapi/include/library.h"


// las fucniones individuales del switch las podiramos separar, medio feo asi

int process_request(data_t* data){
	opcode_t opt = data->opcode;
	
	switch(opt){
		CASE LIST:{
			list_cucco_t* list_struct = malloc(sizeof(list_cucco_t));
			if(list_struct ==  NULL){
				return -1;
			}
			
			//estas fucniones habria que reemplazarlas por las que llaman a la base de datos!
			list_struct-> count = getCuccoAmount(); //////
			list_struct-> cucco_list = getCuccos();	//////
			data->dataStruct = malloc(sizeof(list_cucco_t));
			if(data->dataStruct == null){
				return -1;
			}
			memcpy(data->dataStruct, (void*)list_struct, sizeof(list_struct));
			//aca habria que enviar la respuesta, buscar la funcion que hace eso yver que devuelve, si lo hizo con exito o no
			break;		
		}
		CASE BET:{ //no se si hace falta hacer esta ufncion bet y copiarla me parece que alcanza con castearla desde data directo habria que corroborar... no me acuerdo nada de pi -_-
			money_t * bet = (money_t) data->dataStruct;
			if(bet == NULL){
				return -1;
			}
			
			//funcion de base de datos, dependiendo que pasa en esta fucnion es que devuelvo me imagino
			bet(bet->username, bet->money);
			
			
			//idem de respuesta de case list
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