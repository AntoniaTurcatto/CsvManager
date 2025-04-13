#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h> //_mkdir
#include <io.h> //_access
typedef struct user{
    int id;
    char name[50];
}user;

void addUser();
void addToCsv(FILE* file, user* user_ptr, int position);
void createCsvStructure(FILE* file, user* user_ptr, int position);

int main(){
    addUser();
    
    return 0;
}

void addUser(){
    user* user_ptr = malloc(sizeof(*user_ptr));
    char continue_saving = 's';
    unsigned long int position = 1;
    char path[512];
    FILE* file;

    sprintf(path, "%s\\Documents\\Dynamic", getenv("USERPROFILE"));
    do
    {
        if(position > 1){
            user* new_user_ptr = realloc(user_ptr, sizeof(*user_ptr) * position);
            if(new_user_ptr == NULL){
                printf("Insuficient Memory\n");
                break;
            }
            user_ptr = new_user_ptr;
        }
        printf("name: ");
        fgets(user_ptr[position-1].name, 50, stdin);
        user_ptr[position-1].id = position-1;

        printf("\nDo you want to register more users (S/N)? ");
        scanf(" %c", &continue_saving);
        while (getchar() != '\n');// cleaning buffer
        position++;
    } while (continue_saving == 's' || continue_saving == 'S');
    --position;
    

    if (_access(path, 0) == 0){
        printf("Updating...\n");
        strcat(path, "\\Users.csv");
        file = fopen(path, "a");
        if(file != NULL){
            addToCsv(file, user_ptr, position);
            fclose(file);
        }
    } else {
        printf("Creating...\n");
        if(_mkdir(path) == 0){
            printf("Folder created at: %s\n", path);
            strcat(path, "\\Users.csv");
            file = fopen(path, "w");
            if(file != NULL){
                createCsvStructure(file, user_ptr, position);
                fclose(file);
            }
        } else {
            printf("Error creating folder at: %s\n", path);
        }
    }
    free(user_ptr);
}

void addToCsv(FILE* file, user* user_ptr, int position){
    for(int i = 0; i < position; i++){
        fprintf(file, "%d;%s", user_ptr[i].id, user_ptr[i].name);
    }
}

void createCsvStructure(FILE* file, user* user_ptr, int position){
    fprintf(file, "id;name\n");
    addToCsv(file, user_ptr, position);
}