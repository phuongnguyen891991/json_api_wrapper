#include <jansson.h>
#include <stdlib.h>
#include <stdint.h>

#define RET_OK  1
#define RET_ERR 0

//#define DO_DEBUG
int debug; // set from conf file at runtime
#ifdef DO_DEBUG
#define DEBUG_I(fmt, args ...) do { if(debug == 1) {printf(__FILE__, __FUNCTION__, __LINE__, ((fmt)), ##__VA_ARGS__);} } while (0)
#else
#define DEBUG_I(fmt,...) print(fmt, ...)
#endif

/*
create json object
    Input parameter:
    jsRet: output json.
    key: create json object with key.
    value: the value will be set
*/
static inline uint8_t jsonCreateObject(json_t **jsRet)
{
    json_t *obj = json_object();
    if (!obj)
    {
        printf("Could not create object \n");
        return RET_ERR;
    }
    else
    {
        *jsRet = obj;
        return RET_OK;
    }
}

/*
Set new value of json object
    Input parameter:
    jsRet: output json.
    key: Object key that will be set to
    value: the value will be set
*/
static inline uint8_t jsonObjectSetInterger(json_t *jsRet, const char *key, uint8_t value)
{
    json_t *jsValue;
    jsValue = NULL;

    if (!jsRet)
    {
        printf("Could not process with null object \n");
        return RET_ERR;
    }

    jsValue = json_integer(value);
    if(!jsValue)
    {
        printf("[%s-%d] could not process value to json interget \n", __func__, __LINE__);
        return RET_ERR;
    }

    json_object_set_new(jsRet, key, jsValue);
    return RET_OK;
}

/*
Function:
    Parameter:
    root_js : input json object
    key     : key of json object need to get
    jsRet   : json bject that will be returned.
*/
static inline uint8_t jsonGetSubObject(json_t *root_js, char *key, json_t **jsRet)
{
    if(!key)
    {
        printf("[%s-%d] Invalid key for seach \n", __func__, __LINE__);
        return RET_ERR;
    }

    if (!root_js)
    {
        printf("[%s-%d] Invalid Input object \n", __func__, __LINE__);
        return RET_ERR;
    }
    else
    {
        *jsRet = json_object_get(root_js, key);
        if(!(*jsRet))
        {
            printf("[%s-%d] Invalid object (with key: %s) \n", __func__, __LINE__, key);
            return RET_ERR;
        }
    }

    return RET_OK;
}

/*
Function:
    Parameter:
    root_js : input json object
    key     : key of json object need to get
    retVal  : interger value will be returned.
*/
static inline uint8_t jsonObjectGetIntegerValue(json_t *root_js, char *key, uint32_t *retVal)
{
    uint8_t ret;
    json_t *js_check = NULL;

    if (!key)
    {
         printf("[%s-%d] Invalid key object\n", __func__, __LINE__);
         return RET_ERR;
    }

    if(!root_js)
    {
        printf("[%s-%d] Invalid Input json object\n", __func__, __LINE__);
        return RET_ERR;
    }

    js_check = json_object_get(root_js, key);
    if(!js_check)
    {
        printf("Could not get_object, temporary assign '0' value\n");
        return RET_ERR;
    }
    else
        (*retVal) = json_integer_value(js_check);

    return RET_OK;
}

/*
Function:
    Parameter:
    root_js : input json array
    index   : key of json object need to get
    retVal  : interger value will be returned.
*/
static inline uint8_t jsonArrayGetIntegerValue(json_t *jsRootArr, uint8_t index, uint32_t *retVal)
{
    uint8_t ret ;
    json_t *js_check = NULL;

    if (jsRootArr)
    {
        js_check = json_array_get(jsRootArr, index);
        if(!js_check)
        {
            printf("Could not get js array \n");
            ret = RET_ERR;
        }
        else
            (*retVal) = json_integer_value(js_check);
    }
    else
    {
        printf("[%s-%d] Invalid Input json object\n", __func__, __LINE__);
        return RET_ERR;
    }

    return RET_OK;
}

/*
Function: Create new json array
    Parameter:
    jsRet   : json array that will be returned.
*/
uint8_t jsonCreateArray(json_t **jsRet)
{
    json_t *js_arr = json_array();
    if (!js_arr)
    {
        printf("Could not create object \n");
        return RET_ERR;
    }

    *jsRet = js_arr;
    return RET_OK;
}

/*
Function: Append new json array
    Parameter:
    jsRet   : json array that will be returned.
    value   : Input value to set new
*/
uint8_t jsonArrayAppendIntegerValue(json_t *jsRet, uint8_t value)
{
    json_t *js_integer = NULL;

    if (!jsRet)
    {
        printf("Could not process with NULL object \n");
        return RET_ERR;
    }

    js_integer = json_integer(value);
    if(NULL ==  js_integer)
    {
        return RET_ERR;
    }

    json_array_append_new(jsRet, js_integer);
    if (0 == json_array_size(jsRet))
    {
        printf("[%s-%d Apped new value failed \n]", __func__, __LINE__);
        json_decref(js_integer);
        return RET_ERR;
    }

    return RET_OK;
}

// /*
// Function: Append new value into existed json array
//     Parameter:
//     jsRet   : json array that will be returned.
//     value   : Input value to set new
// */
// uint8_t jsonArrayAppend(json_t *jsRet, uint8_t value)
// {
//     if (!jsRet)
//     {
//         printf("Could not create object \n");
//         return RET_ERR;
//     }

//     json_array_append_new(jsRet, json_integer(value));
//     if (0 == json_array_size(*jsRet))
//     {
//         printf("[%s-%d Apped new value failed \n]", __func__, __LINE__);
//         return RET_ERR;
//     }

//     return RET_OK;
// }

/*
Function: Load json file and return the json object
Parameter:
    fname : the json file name
    retJson:    return the json
*/
uint8_t jsonLoadFIle(const char* fileName, json_t **retJs)
{
    json_t *f_obj;
    json_error_t error;

    if (!fileName)
    {
        printf("[%s-%d] Invalid input file name \n", __func__, __LINE__);
        return RET_ERR;
    }

    f_obj = json_load_file(fileName, 0 , &error);
    if (!f_obj)
    {
        printf("[%s-%d] Failed to load json file (file name: %s ) \n", __func__, __LINE__, fileName);
        return RET_ERR;
    }

    (*retJs) = f_obj;
    return RET_OK;
}

void main()
{
    json_t *js_obj;
    json_t  *js_arr;
    char *buff;
    uint8_t ret;
    uint8_t tmp = 124;

    ret = jsonCreateObject(&js_obj);
    if(RET_ERR == ret)
    {
        return;
    }

    ret = jsonObjectSetInterger(js_obj, "hello", tmp);
    if (ret == RET_OK)
    {
        buff = json_dumps(js_obj, 0);
        printf("The json value is: %s \n", buff);
        free(buff);
    }
    else
    {
        printf("Failed to create json object \n");
    }

    ret = jsonCreateArray(&js_arr);
    if(RET_ERR == ret)
        goto clean_mem;

    ret = jsonArrayAppendIntegerValue(js_arr, tmp);
    if(RET_ERR == ret)
        goto clean_mem;

    buff = json_dumps(js_arr, 0);
    printf("The json array value: %s \n", buff);
    free(buff);

clean_mem:
    if(js_obj)
        json_decref(js_obj);
    if(js_arr)
        json_decref(js_arr);
    return;
}
