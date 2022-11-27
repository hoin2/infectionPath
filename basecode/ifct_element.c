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

//ifct_ele_t 구조체 
typedef struct ifct_ele_t
{
	int pIndex;
	int age;
	unsigned int time;
	int placeHist[N_HISTORY];
} patient;

patient patients[5];

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

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])
{
	patients[index].pIndex = index;
	patients[index].age = age;
	patients[index].time = detected_time;
	for(int i=0; i<5; i++)
		patients[index].placeHist[i] = history_place[i];
		
	return &patients[index];
}

char* ifctele_getPlaceName(int placeIndex){	
	return countryName[placeIndex];
}

int ifctele_getHistPlaceIndex(void* obj, int index)
{
	return ((patient *)obj)->placeHist[index];
}

unsigned int ifctele_getinfestedTime(void* obj)
{
	return ((patient *)obj)->time;
}

int ifctele_getAge(void* obj)
{
	return ((patient *)obj)->age;
}

void ifctele_printElement(void* obj)
{
	printf("%i번째 환자 번호:%i\n",(((patient *)obj)->pIndex),((patient *)obj)->pIndex);
	printf("%i번째 환자 나이:%i\n",(((patient *)obj)->pIndex),((patient *)obj)->age);
	printf("%i번째 환자 감염 확인일자:%i\n",(((patient *)obj)->pIndex), ((patient *)obj)->time);
	printf("%i번째 환자 이동경로: ",(((patient *)obj)->pIndex));
	for(int i=0; i<5; i++)
		printf("%s ", ifctele_getPlaceName(ifctele_getHistPlaceIndex(obj,i)));
	printf("\n");
}

void enum_printf()
{
	printf("%d", Jeju);
}

