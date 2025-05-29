#include"Req_res.c"
#include"Print_text.c"
#include"Scrn_shot.c"

#include <stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

#include <string.h>
int main(int argc, char *argv[]) {
    char *home = getenv("HOME");
    char path[512];
    char html[4096]="<html><body><h3><bold>";
    int choice = 2;
    if(argc > 0)
        choice = 1;
    if(!home){
        printf("HOME is not set\n");
        exit(EXIT_FAILURE);
    }
    
    SelectScreen(home);
    snprintf(path, sizeof(path), "%s/Documents/STS/LatestImage.png", home);

    char *json_text = requesting(path, "What is in this image describe (if there is a person try to find who it is, if its a object tell what it is and its use, if its text explain it.). give text not markdown");
    printf("JSON Text = %s", json_text);
    char *text = get_text(json_text);
    printf("Response = %s\n", text);
    if(choice == 1){
        Notify(text);
    }

    strcat(html, text);
    strcat(html, "</bold></h3></body></html>");

    snprintf(path, sizeof(path), "%s/Documents/STS/LatestResponse.html", home);

    FILE *file = fopen(path, "w");
    if(!file){
        printf("Failed to create file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", html);
    fclose(file);
  
    // requesting("image.png", "if there is an product in the given image then give links , otherwise just describe image.");
    return 0;
}