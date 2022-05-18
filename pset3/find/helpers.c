/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n<=0)
    {
        return false;
    }
    
    int min = 0;
    int max = n;
    
    while(max != min) //when max = min binary search should have finished with needle not found
    {
        if(values[(min+max)/2] == value)    //needle found
        {
            return true;
        } else if (values[(min+max)/2] > value) {   //if number checked is too big set this position as the new max to look at
            max = (min+max)/2;
        } else {    //if number checked is too small set this position as the new minimum to look at
            min = (min+max+1)/2;
        }
    }
         
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)  //bubble sort
{
    bool changed = true;
    int value;
    
    while (changed) //checks if any changes had to be made on this pass through the array
    {
        changed = false;
        for(int i = 0; i<n-1; i++)  //iterates along length of array
        {
            if(values[i]>values[i+1])   //checks each pair in array whether they should be swapped
            {
                value = values[i];      //swaps the values
                values[i] = values[i+1];
                values[i+1] = value;
                changed = true;         //marks that changes were made in this pass
            }
        }
        n--;
    }
    
    return;
}
