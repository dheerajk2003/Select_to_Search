// #include <curl/easy.h>
// #include <curl/easy.h>
#include<stdio.h>
#include<curl/curl.h>
#include<stdlib.h>
#include <time.h>
#include"ToBase64.c"

unsigned char *read_file_to_bytes(const char *filepath, long *file_size) {
    FILE *fp = fopen(filepath, "rb"); // Open in binary mode
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *file_size = ftell(fp);
    if (*file_size < 0) {
        perror("ftell");
        fclose(fp);
        return NULL;
    }
    rewind(fp);

    unsigned char *buffer = malloc(*file_size);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for file buffer\n");
        fclose(fp);
        return NULL;
    }

    if (fread(buffer, 1, *file_size, fp) != (size_t)*file_size) {
        fprintf(stderr, "Failed to read entire file\n");
        free(buffer);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return buffer;
}

void requesting(const char *image_path, const char *prompt_text){
    CURL *curl;
    CURLcode res;
     
    char *GEMINI_API_KEY = getenv("GEMINI_API_KEY");
    if(GEMINI_API_KEY == NULL){
        fprintf(stderr, "Error: GEMINI_API_KEY is not set\n");
        return;
    }
    char *url = malloc(180);
    sprintf(url, "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%s", GEMINI_API_KEY);
    printf("%s", url);

    long image_file_size;
    unsigned char *image_bytes = read_file_to_bytes(image_path, &image_file_size);
    if(!image_bytes){
        free(url);
        printf("didn't get image bytes \n");
        return;
    }

    char *base64_encode_image = base64_encode(image_bytes, image_file_size);
    free(image_bytes);
    if(!base64_encode_image){
        printf("failed to encode\n");
        free(url);
        return;
    }

    size_t json_size = strlen(base64_encode_image) + 1024;
    char *json_data = malloc(json_size);

    if (!json_data) {
        printf("failed to allocate json_data\n");
        free(base64_encode_image);
        free(url);
        return;
    }

    snprintf(json_data, json_size,
        "{"
        "  \"contents\": ["
        "    {"
        "      \"parts\": ["
        "        {"
        "          \"text\": \"%s\""
        "        },"
        "        {"
        "          \"inline_data\": {"
        "            \"mime_type\": \"image/png\","
        "            \"data\": \"%s\""
        "          }"
        "        }"
        "      ]"
        "    }"
        "  ]"
        "}", prompt_text, base64_encode_image);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl){

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl,  CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            printf("curl not ok \n\n\n\n");
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    free(json_data);
    free(base64_encode_image);
    free(url);
    return;
}

