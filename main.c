#include"Req_res.c"
#include"Print_text.c"
#include"Scrn_shot.c"
#include <stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

#include <string.h>
int main() {
    char *home = getenv("HOME");
    if(!home){
        printf("HOME is not set\n");
        exit(EXIT_FAILURE);
    }
    // ScreenShot(0,0,0,0);
    SelectScreen();
    char *json_text = requesting("image.png", "What is in this image describe.");
    printf("JSON Text = %s", json_text);
    char *text = get_text(json_text);
    Notify(text);
    printf("Response = %s\n", text);

    char path[512];
    snprintf(path, sizeof(path), "%s/Documents/STS/LatestResponse.md", home);

    FILE *file = fopen(path, "w");
    if(!file){
        snprintf(path, sizeof(path), "%s/Documents/STS", home);
        if(mkdir(path, 0777) == -1){
            printf("Failed to create directory");
            exit(EXIT_FAILURE);
        }
        snprintf(path, sizeof(path), "%s/Documents/STS/LatestResponse.md", home);
        file = fopen(path, "w");
        if(!file){
            printf("Failed to create file");
            exit(EXIT_FAILURE);
        }
    }
    fprintf(file, "%s", text);
    fclose(file);
  
    // requesting("image.png", "if there is an product in the given image then give links , otherwise just describe image.");
    return 0;
}