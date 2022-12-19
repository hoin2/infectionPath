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


int trackInfester(int patient_no);
int isMet(int p_patient, int i_patient);
int convertTimeToIndex(int time, int infestedTime);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    char place[100];
    int Min_age, Max_age;
    int arr1[30000];
    int arr2[200000];
    int p_patient, spr;
	int frs_spr = -1;
    
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
		ifct_element=ifctele_genElement(pIndex, age, time, placeHist);
		
		ifctdb_addTail(ifct_element);
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
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
                printf("Patient index : ");
  				scanf("%d",&pIndex);
					
				if(pIndex<0||pIndex>ifctdb_len())
					printf("[ERROR] Your input for the patient index (%d) is wrong! input must be 0 ~4\n",pIndex);
					
				else
					ifctele_printElement(ifctdb_getData(pIndex));
				break;
                
            case MENU_PLACE:
            	printf("Place Name : ");
            	scanf("%s",place);
            	
				int p_num = ifctdb_len();
				
				for(int i=0;i<ifctdb_len();i++)
				{
					if(strcmp(place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i),N_HISTORY-1)))==0)
						ifctele_printElement(ifctdb_getData(i));
						
					else
						p_num--;
				}	
				
				if(p_num == 0)
				{
					printf("\n");
					printf("There are 0 patients detected in %s",place);
				}
				printf("\n");

                break;
                
            case MENU_AGE:
                printf("minimal age : ");
                scanf("%d",&Min_age);
                printf("maximal age : ");
            	scanf("%d",&Max_age);
            	
           		int a_num = ifctdb_len();
           		
		   		for(int i=0;i<ifctdb_len();i++)
				{
					if(Min_age<=ifctele_getAge(ifctdb_getData(i))&&Max_age>=ifctele_getAge(ifctdb_getData(i)))
						ifctele_printElement(ifctdb_getData(i));      	
						
					else
						a_num--;
				}	
    			
				if(a_num == 0)
				{
					printf("\n");
					printf("There are 0 patients whose age is beteween %d and %d.\n",Min_age,Max_age); 
				}
                
				break;
                
            case MENU_TRACK:
                printf("Patient index : ");
                scanf("%d",&pIndex);
                
                if(pIndex<0||pIndex>=ifctdb_len())
					printf("[ERROR] Your input for the patient index (%d) is wrong! input must be 0 ~4\n",pIndex);
                
				p_patient = pIndex; 
				
                while(p_patient != frs_spr)
                {
                	spr = trackInfester(p_patient);
					
					if(spr != p_patient)//if spreader
						printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n",p_patient,spr,isMet(p_patient,spr),ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),convertTimeToIndex(isMet(p_patient,spr), ifctele_getinfestedTime(ifctdb_getData(p_patient))))));
					else
					{
						frs_spr = spr;
					}
					
					p_patient = spr; 
				}
				
				if(trackInfester(p_patient) != pIndex)
					printf("\nThe first infector of %i is %i\n",pIndex, frs_spr);
						
				else
					printf("%i is the first infector!!\n",frs_spr);
				
				break;
    
            default:
                printf("[ERROR] Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}


int trackInfester(int patient_no)
{
	int meetime;
	int spr = patient_no;
	int cnt = 100; //first meetime compare  
	
	for(int i=0;i<ifctdb_len();i++ )
	{
		if(i != patient_no)
		{
			meetime = isMet(patient_no, i);	
			
			if(meetime > 0)//if meet 
			{
				if(meetime < cnt)
				{
					spr = i; 
					cnt = meetime;
				}
			}
		}
	}
	
	return spr; 
}

int isMet(int p_patient, int i_patient)
{
	int pmov_time, pmov_place;
	int meetime = -1;
	
	for(int i=2;i<N_HISTORY;i++)
	{
		pmov_time = ifctele_getinfestedTime(ifctdb_getData(p_patient))-i;//present patient's moving time
		
		pmov_place = convertTimeToIndex(pmov_time, ifctele_getinfestedTime(ifctdb_getData(p_patient)));//moving time's place

		if(pmov_time == ifctele_getinfestedTime(ifctdb_getData(i_patient)))
		{
			if(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),pmov_place) == ifctele_getHistPlaceIndex(ifctdb_getData(i_patient),4))
			{
				meetime = pmov_time;	
			}
		}
		
		if(pmov_time == ifctele_getinfestedTime(ifctdb_getData(i_patient))-1)
		{
			if(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),pmov_place) == ifctele_getHistPlaceIndex(ifctdb_getData(i_patient),3))
			{
				meetime = pmov_time;	
			}
		}
	}
	return meetime; 
}

int convertTimeToIndex(int time, int infestedTime)
{
	int index = -1;
	
	if(time <= infestedTime && time > infestedTime-N_HISTORY)
	{
		index = N_HISTORY-(infestedTime - time)-1;
	}
	
	return index;
}
