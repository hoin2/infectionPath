//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

//ifct_ele_t ±¸Á¶Ã¼ 
typedef struct ifs_ele
{
	int pIndex;
	int age;
	unsigned int time;
	int placeHist[N_HISTORY];
} ifct_ele_t;

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])
{
	int p_num = 5;
	ifct_ele_t *ifct_element;
	ifct_element=(ifct_ele_t *)malloc(sizeof(ifct_ele_t)*p_num);
	
	if(ifct_element == NULL)
	{
		printf("error\n");
		return 1;
	}
	
	ifct_element[index].pIndex = index;
	ifct_element[index].age = age;
	ifct_element[index].time = detected_time;
	for(int i=0; i<5; i++)
		ifct_element[index].placeHist[i] = history_place[i];
	
	return &ifct_element[index];
	
	free(ifct_element);
}

char* ifctele_getPlaceName(int placeIndex){	
	return countryName[placeIndex];
}

int ifctele_getHistPlaceIndex(void* obj, int index)
{
	ifct_ele_t *strPtr=(ifct_ele_t *)obj;
	
	return strPtr->placeHist[index];
}

unsigned int ifctele_getinfestedTime(void* obj)
{
	ifct_ele_t *strPtr=(ifct_ele_t *)obj;
	
	return strPtr->time;
}

int ifctele_getAge(void* obj)
{
	ifct_ele_t *strPtr=(ifct_ele_t *)obj;
	
	return strPtr->age;
}

void ifctele_printElement(void* obj)
{
	ifct_ele_t *strPtr=(ifct_ele_t *)obj;
	
	printf("-------------------------------------------------------------------------------\n");
	printf("Patient index : %i\n",strPtr->pIndex);
	printf("Patient age : %i\n",strPtr->age);
	printf("Detected time : %i\n",strPtr->time);
	printf("Path History : ");
	for(int i=0; i<5; i++)
	{
		printf("%s(%i)", ifctele_getPlaceName(ifctele_getHistPlaceIndex(obj,i)),strPtr->placeHist[i]);
		if(i>=0&&i<4)
			printf("-> ");
	}
	printf("\n");	
	printf("-------------------------------------------------------------------------------\n");
}

