#include "ieconfig.h"

#include "core/iecoredefinitions.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 2048

void _ie_config_comment_strip(char*);
int  _ie_config_token_count(char*);
void _ie_config_tokenize(char*, char**);

ie_config_result* _ie_config_result_bool_get(FILE*);
ie_config_result* _ie_config_result_int_get(FILE*);
ie_config_result* _ie_config_result_float_get(FILE*);
ie_config_result* _ie_config_result_string_get(FILE*);
ie_config_result* _ie_config_result_array_int_get(FILE*);
ie_config_result* _ie_config_result_array_float_get(FILE*);
ie_config_result* _ie_config_result_array_string_get(FILE*);

FILE* ie_config_read(char* path)
{
    return fopen(path, "r");
}


void ie_config_close(FILE* config_file)
{
    fclose(config_file);
}


int ie_config_is_key(FILE* config_file)
{
    if ( ie_config_is_section(config_file) )
        return IE_FALSE;

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);

    int token_count = _ie_config_token_count(buffer);

    if (token_count <= 0)
        return IE_FALSE;

    return IE_TRUE;
}


int ie_config_key_find(FILE* config_file, char* search)
{
    rewind(config_file);

    char* key_name = NULL; 
    if ( ie_config_is_key(config_file) ) {
        key_name = ie_config_key_name_get(config_file);
        if ( strcmp(key_name, search) == 0 ) {
            free(key_name);
            return IE_TRUE;
        }
    }

    free(key_name);
    return ie_config_key_find_next(config_file, search);
}


int ie_config_key_find_next(FILE* config_file, char* search)
{

    char* key_name = NULL; 
    while( ie_config_key_next(config_file) ) {

       key_name = ie_config_key_name_get(config_file);

       if ( strcmp(key_name, search) == 0 ) {
           free(key_name);
           return IE_TRUE;
       }

       free(key_name);
    }
   
    return IE_FALSE;
}


char* ie_config_key_name_get(FILE* config_file)
{
    if ( !ie_config_is_key(config_file) )
        return NULL;
    
    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[0];
    size_t key_name_size = strlen(token); 
    char* key_name = (char*) malloc(key_name_size + 1);
    strncpy(key_name, token, key_name_size);
    key_name[key_name_size] = '\0';
    
    free(tokens);

    return key_name;
}


int ie_config_key_next(FILE* config_file)
{
    char buffer[LINE_BUFFER_SIZE];

    while ( fgets(buffer, LINE_BUFFER_SIZE, config_file) != NULL ) { 
        
        if( ie_config_is_key(config_file) )
            return IE_TRUE;

    }

    return IE_FALSE;
}


ie_config_result* ie_config_result_get(FILE* config_file, ie_data_type type)
{
    switch (type) {
        case IE_BOOL:
            return _ie_config_result_bool_get(config_file);
        break;

        case IE_INT:
            return _ie_config_result_int_get(config_file);
        break;

        case IE_FLOAT:
            return _ie_config_result_float_get(config_file);
        break;

        case IE_STRING:
            return _ie_config_result_string_get(config_file);
        break;

        case IE_ARRAY_INT:
            return _ie_config_result_array_int_get(config_file);
        break;
        
        case IE_ARRAY_FLOAT:
            return _ie_config_result_array_float_get(config_file);
        break;
        
        case IE_ARRAY_STRING:
            return _ie_config_result_array_string_get(config_file);
        break;

        default:
            return NULL;
        break;
    }

    return NULL;
}


ie_config_result* _ie_config_result_bool_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_BOOL;
    result->size = sizeof(int);

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count == 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[1];

    free(tokens);

    if ( strcmp(token, "true") == 0 ||
         strcmp(token, "TRUE") == 0 ||
         strcmp(token, "True") == 0 ||
         strcmp(token, "1") == 0) {

        result->pdata = (int*) malloc(sizeof(int));
        *(int*)(result->pdata) = IE_TRUE;

    }
    else if ( strcmp(token, "false") == 0 ||
              strcmp(token, "FALSE") == 0 ||
              strcmp(token, "False") == 0 ||
              strcmp(token, "0") == 0) {

        result->pdata = (int*) malloc(sizeof(int));
        *(int*)(result->pdata) = IE_FALSE;

    }
    else {
        result->pdata = NULL;
    }

    return result;
}


ie_config_result* _ie_config_result_int_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_INT;
    result->size = sizeof(int);

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count == 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[1];

    free(tokens);
    
    result->pdata = (int*) malloc(sizeof(int));

    *(int*)(result->pdata) = atoi(token);

    return result;
}


ie_config_result* _ie_config_result_float_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_FLOAT;
    result->size = sizeof(float);

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count == 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[1];

    free(tokens);
    
    result->pdata = (float*) malloc(sizeof(float));

    *(float*)(result->pdata) = atof(token);

    return result;
}


ie_config_result* _ie_config_result_string_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_STRING;

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count == 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[1];

    free(tokens);
    
    result->size = sizeof(strlen(token) + 1);

    result->pdata = (char*) malloc(result->size);
    memset(result->pdata, '\0', result->size);
    strcpy(result->pdata, token);

    return result;
}


ie_config_result* _ie_config_result_array_int_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_ARRAY_INT;

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count >= 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }
    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);
    
    result->size = sizeof(int) * (token_count - 1);
    result->pdata = (int*) malloc(result->size);

    int* data = (int*) result->pdata;

    int i;
    for (i = 0; i < (token_count - 1); ++i) {
        int token = atoi(tokens[i + 1]);
        data[i] = token;
    }

    free(tokens);

    return result;
}


ie_config_result* _ie_config_result_array_float_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_ARRAY_FLOAT;

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count >= 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);
    
    result->size = sizeof(float) * (token_count - 1);
    result->pdata = (float*) malloc(result->size);

    float* data = (float*) result->pdata;

    int i;
    for (i = 0; i < (token_count - 1); ++i) {
        float token = atof(tokens[i + 1]);
        data[i] = token;
    }

    free(tokens);

    return result;

}


ie_config_result* _ie_config_result_array_string_get(FILE* config_file)
{
    ie_config_result* result = (ie_config_result*) malloc(sizeof(ie_config_result));
    result->type = IE_ARRAY_STRING;

    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);
    
    if ( !(token_count >= 2) ) {
        result->size = 0;
        result->pdata = NULL;
        return result;
    }

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);
 
    result->size = sizeof(char*) * token_count - 1;

    result->pdata = (char**) malloc(sizeof(char*) * result->size);

    char** data = (char**) result->pdata;

    int i;
    for (i = 0; i < (token_count - 1); ++i) {
        int token_size = strlen(tokens[i + 1]);
        data[i] = (char*) malloc(token_size + 1);
        memset(data[i], '\0', token_size + 1);
        strcpy(data[i], tokens[i + 1]);
    }

    free(tokens);

    return result;
}


void ie_config_result_free(ie_config_result* result)
{

    if (result->type == IE_ARRAY_STRING) {
        char** data = result->pdata; 
        int i;
        for (i = 0; i < result->size; ++i) {
            free(data[i]);
        }
    }

    free(result->pdata);
    free(result);
}


int ie_config_result_ok(ie_config_result* result)
{
    if (result->pdata == NULL)
        return IE_FALSE;

    return IE_TRUE;
}


int ie_config_is_section(FILE* config_file)
{
    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);

    if (token_count <= 0 || token_count > 1)
        return IE_FALSE;

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[0];

    free(tokens);

    char* bracket_left = strchr(token, '[');
    char* bracket_right = strchr(token, ']');
    if (bracket_left == NULL || bracket_right == NULL)
        return IE_FALSE;

    if (bracket_right < bracket_left)
        return IE_FALSE;

    if ( !(bracket_left == token) || !(bracket_right == token + strlen(token) - 1) )
        return IE_FALSE;


    return IE_TRUE;
}


void ie_config_rewind(FILE* config_file)
{
    rewind(config_file);
}


int ie_config_section_find(FILE* config_file, char* search)
{
    rewind(config_file);

    char* section_name = NULL; 
    if ( ie_config_is_section(config_file) ) {
        section_name = ie_config_section_name_get(config_file);
        if ( strcmp(section_name, search) == 0 ) {
            free(section_name);
            return IE_TRUE;
        }
    }
    
    free(section_name);
    return ie_config_section_find_next(config_file, search);
}


int ie_config_section_find_next(FILE* config_file, char* search)
{
    char* section_name = NULL; 
    while( ie_config_section_next(config_file) ) {

       section_name = ie_config_section_name_get(config_file);

       if ( strcmp(section_name, search) == 0 ) {
           free(section_name);
           return IE_TRUE;
       }

       free(section_name);
    }
   
    return IE_FALSE;
}


int ie_config_section_key_find(FILE* config_file, char* section, char* key)
{
    int result = ie_config_section_find(config_file, section);
    if (result == IE_FALSE)
        return IE_FALSE;

    return ie_config_section_key_find_next(config_file, section, key);
}


int ie_config_section_key_find_next(FILE* config_file, char* section, char* key)
{
    long int position_current = ftell(config_file);

    int result = ie_config_section_find(config_file, section);
    if (result == IE_FALSE)
        return IE_FALSE;

    long int position_section = ftell(config_file);

    result = ie_config_section_next(config_file);
    if (result == IE_FALSE) 
        fseek(config_file, 0, SEEK_END);

    long int position_section_next = ftell(config_file); 
    
    if ( !((position_current >= position_section) && (position_current < position_section_next)) ) 
        position_current = position_section;

    fseek(config_file, position_current, SEEK_SET);

    result = ie_config_key_find_next(config_file, key);
    if (result == IE_FALSE)
        return IE_FALSE;

    position_current = ftell(config_file);

    if (position_current >= position_section_next)
        return IE_FALSE;

    return IE_TRUE;
}


char* ie_config_section_name_get(FILE* config_file)
{
    if ( !ie_config_is_section(config_file) )
        return NULL;
    
    fpos_t position_current;
    fgetpos(config_file, &position_current);

    char buffer[LINE_BUFFER_SIZE];
    fgets(buffer, LINE_BUFFER_SIZE, config_file);

    fsetpos(config_file, &position_current);

    _ie_config_comment_strip(buffer);
    
    int token_count = _ie_config_token_count(buffer);

    char** tokens = (char**) malloc(sizeof(char*) * token_count);
    _ie_config_tokenize(buffer, tokens);

    char* token = tokens[0];
    size_t section_name_size = strlen(token) - 2; 
    char* section_name = (char*) malloc(section_name_size + 1);
    strncpy(section_name, token + 1, section_name_size);
    section_name[section_name_size] = '\0';
    
    free(tokens);

    return section_name;
}


int ie_config_section_next(FILE* config_file)
{
    char buffer[LINE_BUFFER_SIZE];

    while ( fgets(buffer, LINE_BUFFER_SIZE, config_file) != NULL ) { 
        
        if( ie_config_is_section(config_file) )
            return IE_TRUE;

    }

    return IE_FALSE;
}


void _ie_config_comment_strip(char* buffer)
{
    char* comment_start = strchr(buffer, '#');
    if (comment_start == NULL)
        return;

    (*comment_start) = '\0';
}


int _ie_config_token_count(char* buffer)
{
    int token_count = 0;
    int i;
    for (i = 0; i < strlen(buffer); ++i) {

        if ( !isspace(buffer[i]) )
            ++token_count;

        while ( !isspace(buffer[i]) )
            ++i;
    }
    return token_count;
}


void _ie_config_tokenize(char* buffer, char** tokens)
{
    int token_index = 0;
    char* buffer_end = buffer + strlen(buffer);
    char* i;
    for (i = buffer; i < buffer_end; ++i) {

        if ( !isspace(*i) ) {
            tokens[token_index] = i;
            ++token_index;
        }

        while ( !isspace(*i) )
            ++i;

        (*i) = '\0';
    }
}

