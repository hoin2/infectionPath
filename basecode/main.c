//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    char place[100];
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
	if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }

    fp=fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }

    //1-2. loading each patient informations
    while(fscanf(fp,"%d %d %d", &pIndex, &age, &time) == 3)
    {
    	for(int i=0; i<5; i++)
    	{
    		fscanf(fp," %d", &placeHist[i]);
		}
		ifctdb_addTail(ifctele_genElement(pIndex, age, time, placeHist));
	}
		
    //1-3. FILE pointer close
	fclose(fp);
    
	do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
			int Min_age, Max_age;
        	
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
                printf("Patient index: ");
  				scanf("%d",&pIndex);
				
				ifctele_printElement(ifctdb_getData(pIndex));
				break;
                
            case MENU_PLACE:
            	printf("Place Name: ");
            	scanf("%s",place);
            
            	for(int i=0;i<5;i++)
				{
					if(strcmp(place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i),4)))==0)
						ifctele_printElement(ifctdb_getData(i));
				}
				printf("\n");

				
//				if(*(int*)ifct_element != ifctele_getHistPlaceIndex(obj,N_HISTORY-1))
//            			printf("There are 0 patients detected in %s",&place);
            	
                break;
                
            case MENU_AGE:
                printf("minimal age : ");
                scanf("%d",&Min_age);
                printf("maximal age : ");
            	scanf("%d",&Max_age);
           
		   		for(int i=0;i<N_HISTORY;i++)
				{
					if(Min_age<=ifctele_getAge(ifctdb_getData(i))&&Max_age>=ifctele_getAge(ifctdb_getData(i)))
						ifctele_printElement(ifctdb_getData(i));
            	}	
            	
                break;
                
            case MENU_TRACK:
                    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

int trackInfester(int patient_no, int *detected_time, int *place)
{
	
}
