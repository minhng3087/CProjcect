#include "inc/btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inc/bc.h"
#include "inc/bt.h"
#include "inc/btree_int.h"

int main(){
    FILE *f1 = fopen("../data/anhviet.txt", "r");
    BTA *dt;
    dt = btcrt("../data/words.dat", 0, FALSE);
  
    
    btdups(dt, FALSE);

    
    char tmp[500];
    char anh[500], viet[10000];
 
    while(!feof(f1))
    {
        fgets(tmp, 5000, f1);
        
        if(tmp[0] == '@')
        {
            if(strlen(viet) && strlen(anh))
            {
                if(viet[strlen(viet)-1] == '\n')
                    viet[strlen(viet)-1] = '\0';
                
                
                btins(dt, anh, viet, strlen(viet)+1);
                
                anh[0] = '\0';
                viet[0] = '\0';
            }
            
            if(tmp[strlen(tmp)-1] == '\n')
                tmp[strlen(tmp)-1] = '\0';
            
            strcpy(anh, tmp + 1);
            
            for(int i=0; i < strlen(anh); i++)
                if(anh[i] == '/'){
                    strcat(viet, anh + i);
                    strcat(viet, "\n");
                    
                    anh[i-1] = '\0';
                }
        }
        else{
            if(tmp[0] != '\n')
            {
                strcat(viet, tmp);
            }
        }
    }
    
    viet[strlen(viet)-1] = '\0';
    btins(dt, anh, viet, strlen(viet)+1);
    
    anh[0] = '\0';
    viet[0] = '\0';
    
    printf("Create successfully  \n");

    fclose(f1);
    btcls(dt);
 
    
    return 0;
}
