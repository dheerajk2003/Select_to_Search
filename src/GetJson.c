#include<stdio.h>
#include<cjson/cJSON.h>
#include<curl/curl.h>
#include <stdlib.h>
#include<string.h>

struct string{
    char *ptr;
    int len;
};

void init_string(struct string *s){
    s->len = 0; 
    s->ptr = malloc(s->len+1);
    if(s->ptr == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s){
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if(s->ptr == NULL){
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size*nmemb;
}

char *get_text(char *json_string){
    if(json_string == NULL)
    {
        fprintf(stderr, "json_string is NULL\n");
        exit(EXIT_FAILURE);
    }

    cJSON *json = cJSON_Parse(json_string);
    if(json == NULL){
        fprintf(stderr, "cJSON_Parse() failed\n");
        exit(EXIT_FAILURE);
    }
    cJSON *candidates = cJSON_GetObjectItemCaseSensitive(json, "candidates");
    if(!candidates){
        fprintf(stderr, "candidates is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    cJSON *first_candidate = cJSON_GetArrayItem(candidates, 0);
    if(!first_candidate){
        fprintf(stderr, "first_candidate is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    cJSON *content = cJSON_GetObjectItemCaseSensitive(first_candidate, "content");
    if(!content){
        fprintf(stderr, "content is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    cJSON *parts = cJSON_GetObjectItemCaseSensitive(content, "parts");
    if(!parts){
        fprintf(stderr, "parts is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    cJSON *first_part = cJSON_GetArrayItem(parts, 0);
    if(!parts){
        fprintf(stderr, "first part is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }
    cJSON *text = cJSON_GetObjectItemCaseSensitive(first_part, "text");
    if(!text){
        fprintf(stderr, "text is NULL\n");
        cJSON_Delete(json);
        exit(EXIT_FAILURE);
    }

    if (cJSON_IsString(text) && text->valuestring != NULL) {
        char *result = strdup(text->valuestring);
        cJSON_Delete(json);
        return result;
    } else {
        printf("No valid text found.\n");
    }

    cJSON_Delete(json);
    return NULL;
}