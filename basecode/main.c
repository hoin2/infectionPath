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


int trackInfester(int patient_no); 							//find patient's spreader
int isMet(int p_patient, int i_patient); 					//find present patient and random patient's meet time 
int convertTimeToIndex(int time, int infestedTime); 		//find place history index at the time of meeting

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    //txt's patient information(index, age, infested time) input value variable & pIndex is 1-3's case patient index input value variable
    int pIndex, age, time; 
    int placeHist[N_HISTORY]; 			//txt's patient information(placeindex history) input value variable
    char place[MAX_PLACENAME]; 			//1-3's case 2:MENU_PLACE's input value variable
    int Min_age, Max_age; 				//1-3's case 3:MENU_AGE's input value variable
    int p_patient, spr;					//1-3's case 4:MENU_TRACK's input value variable
	int frs_spr = -1; 					//first spreader's initial value -> to compare with present patient
    
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
    //store txt's patient information in variables 
    while(fscanf(fp,"%d %d %d", &pIndex, &age, &time) == 3)
    {
    	for(int i=0; i<5; i++)
    	{
    		fscanf(fp," %d", &placeHist[i]);
		}
		ifct_element=ifctele_genElement(pIndex, age, time, placeHist);  //create patient information structure -> ifct_element.h
		ifctdb_addTail(ifct_element);									//save linked list -> ifct_database.h
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
					
				//if enter a value out of range
				if(pIndex<0||pIndex>ifctdb_len())
					printf("[ERROR] Your input for the patient index (%d) is wrong! input must be 0 ~4\n",pIndex);
					
				//if enter a value whithin range, print patient's information
				else
					ifctele_printElement(ifctdb_getData(pIndex));
				break;
                
            case MENU_PLACE:
            	printf("Place Name : ");
            	scanf("%s",place);
            	
				int p_num = ifctdb_len(); //variable to find 0 patinet at unspecified place or no patient place
				
				//compare input place with all patient infestedtime's place name -> strcmp
				for(int i=0;i<ifctdb_len();i++)
				{
					if(strcmp(place,ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i),N_HISTORY-1)))==0)
						ifctele_printElement(ifctdb_getData(i));
						
					else
						p_num--;
				}	
				
				//find 0 patinet at unspecified place or no patient place
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
            	
           		int a_num = ifctdb_len(); //variable to find 0 patinet beteween the age range
           		
           		//find patient beteween the age range
		   		for(int i=0;i<ifctdb_len();i++)
				{
					if(Min_age<=ifctele_getAge(ifctdb_getData(i))&&Max_age>=ifctele_getAge(ifctdb_getData(i)))
						ifctele_printElement(ifctdb_getData(i));      	
						
					else
						a_num--;
				}	
    			
    			//find 0 patinet beteween the age range
				if(a_num == 0)
				{
					printf("\n");
					printf("There are 0 patients whose age is beteween %d and %d.\n",Min_age,Max_age); 
				}
                
				break;
                
            case MENU_TRACK:
                printf("Patient index : ");
                scanf("%d",&pIndex);
                
                //if enter a value out of range
                if(pIndex<0||pIndex>=ifctdb_len())
					printf("[ERROR] Your input for the patient index (%d) is wrong! input must be 0 ~4\n",pIndex);
                
				p_patient = pIndex;						//present patient = input value
				
				//find present patient's first spredaer
                while(p_patient != frs_spr) 			//until present patient = first spreader 
                {
                	spr = trackInfester(p_patient);		//spr is present patient's spredaer
					
					//if spreader is present
					if(spr != p_patient)
						printf("-->[Tracking] patient %i is infected by %i (time : %i, place : %s)\n",p_patient,spr,isMet(p_patient,spr),ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),convertTimeToIndex(isMet(p_patient,spr), ifctele_getinfestedTime(ifctdb_getData(p_patient))))));
				
					//if spredaer = present patient, find first spreader 
					else 
						frs_spr = spr;
			
					p_patient = spr;					//turn present patient value into spreader 
				}
				
				//end of while loop, if input patient is not first spreader
				if(trackInfester(p_patient) != pIndex)
					printf("\nThe first infector of %i is %i\n",pIndex, frs_spr);
				
				//end of while loop, if input patient is first spreader
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

//find patient's spreader
int trackInfester(int patient_no)
{
	int meetime;						//present patient and random patient's meet time value varialbe
	int spr = patient_no;				//return value variable
	int p_meetime = 100; 				//variable to compare with present meetime and previous meetime
	
	//match case to comprae with present patient and all patient's time and place history 
	for(int i=0;i<ifctdb_len();i++ )
	{
		meetime = isMet(patient_no, i);	
		
		//if meet 
		if(meetime > 0)
		{
			//if present meetime is earler than previous meetime
			if(meetime < p_meetime)
			{
				spr = i; 		//spreader is i(patient index)
				p_meetime = meetime; 	//turn previous meetime value into meetime
			}
		}
	}
	
	return spr; 
}

//find present patient and random patient's meet time 
int isMet(int p_patient, int i_patient)
{
	int pmov_time, pmov_place; 			//present patient time and place history value varialbe
	int meetime = -1; 					//if meetime is -1, present pateint and random(i) patient didn't meet
	
	for(int i=2;i<N_HISTORY;i++)
	{
		//time history before infection of the present pateint
		pmov_time = ifctele_getinfestedTime(ifctdb_getData(p_patient))-i; 
		//find present pateint's place index at pmov_time
		pmov_place = convertTimeToIndex(pmov_time, ifctele_getinfestedTime(ifctdb_getData(p_patient)));

		//if match infectious time(infested time)
		if(pmov_time == ifctele_getinfestedTime(ifctdb_getData(i_patient)))
		{
			//if match place index of infectious time
			if(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),pmov_place) == ifctele_getHistPlaceIndex(ifctdb_getData(i_patient),4))
			{
				meetime = pmov_time;	
			}
		}
		
		//if match infectious time(infestedn time -1)
		if(pmov_time == ifctele_getinfestedTime(ifctdb_getData(i_patient))-1)
		{
			//if match place index of infectious time
			if(ifctele_getHistPlaceIndex(ifctdb_getData(p_patient),pmov_place) == ifctele_getHistPlaceIndex(ifctdb_getData(i_patient),3))
			{
				meetime = pmov_time;	
			}
		}
	}
	
	return meetime; 
}

//find place history index at the time of meeting
int convertTimeToIndex(int time, int infestedTime)
{
	int index = -1;
	
	//if input time vlaue is within infessted time history range
	if(time <= infestedTime && time > infestedTime-N_HISTORY)
	{
		index = N_HISTORY-(infestedTime - time)-1;
	}
	
	return index;
}
