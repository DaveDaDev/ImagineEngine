#ifndef IE_CORE_CONFIG
#define IE_CORE_CONFIG

#include "core/iecoredefinitions.h"

#include <stdio.h>

typedef struct {
    ie_data_type      type;
    size_t            size;
    void*             pdata; 
} ie_config_result;

FILE* ie_config_read(char*);
void  ie_config_close(FILE*);

int   ie_config_is_key(FILE*);
int   ie_config_is_section(FILE*);

int   ie_config_key_find(FILE*, char*);
int   ie_config_key_find_next(FILE*, char*);
char* ie_config_key_name_get(FILE*);
int   ie_config_key_next(FILE*);

ie_config_result* ie_config_result_get(FILE*, ie_data_type);
void              ie_config_result_free(ie_config_result*);
int               ie_config_result_ok(ie_config_result*);

void  ie_config_rewind(FILE*);

int   ie_config_section_find(FILE*, char*);
int   ie_config_section_find_next(FILE*, char*);
int   ie_config_section_key_find(FILE*, char*, char*);
int   ie_config_section_key_find_next(FILE*, char*, char*);
char* ie_config_section_name_get(FILE*);
int   ie_config_section_next(FILE*);

#endif
