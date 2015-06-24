/*
 * driver_dblink_list.c
 *
 *  Created on: Jun 3, 2015
 *      Author: hoeselm
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dblink_list.h"

int main(void){
	long array[4];
	array[0] = 1;
	array[1] = 7;
	array[2] = 15;
	array[3] = 69;
	clear((create_sorted_list(array,4)));
	return 0;
}
