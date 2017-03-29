/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;


int main(int argc, char* argv[])
{
    //open memory card file
    FILE* file = fopen("card.raw", "r");
    
    if (file == NULL) 
    {
        printf("Could not open file card.raw");
        return 1;
    }
    
    
    BYTE buffer[512];
    
    //file number store
    int fileNum = 0;
    char name[10];
    
    //temporary file
    FILE* temporaryFile = NULL;
    
    while (!feof(file)) {
    
        if ( buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] ==0xe1)) {
        
            if (temporaryFile != NULL) {
            
                fclose(temporaryFile);        
            }
        
            sprintf(name, "%03d.jpg", fileNum);
            fileNum++;
            
            temporaryFile = fopen(name, "w");
            
            fwrite(buffer, sizeof(buffer), 1, temporaryFile);
  
        }
        else if (fileNum >0) {
            fwrite(buffer, sizeof(buffer), 1, temporaryFile);
        
        }
    
        fread(buffer, sizeof(buffer), 1, file);
    }
    
    fclose(file);
        
    return 0;
    

}
