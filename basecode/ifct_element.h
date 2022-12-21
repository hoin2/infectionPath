//
//  ifct_element.h
//  infection path
//
//  Created by Juyeop Kim on 2022/10/20.
//

#ifndef ifct_element_h
#define ifct_element_h

#define N_PLACE                     40
#define N_HISTORY                   5
#define MAX_PLACENAME               100

// main -> ifctele_genElement() 
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]);
char* ifctele_getPlaceName(int placeIndex);					//place index -> place name
int ifctele_getHistPlaceIndex(void* obj, int index);		//(0,3) -> patient index is 0, place history's 4th place index
unsigned int ifctele_getinfestedTime(void* obj);			//get infested time
int ifctele_getAge(void* obj);								//get age
void ifctele_printElement(void* obj);						//main(obj)->print

#endif /* ifct_element_h */
