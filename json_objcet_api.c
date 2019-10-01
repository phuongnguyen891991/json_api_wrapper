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

//create json object
// input parameter:
//  jsRet: output json.
//  key: create json object with key.
//  value: the value will be set

static inline uint8_t jsonCreateObject(json_t **jsRet, const char *key, uint8_t value)
{
    json_t *obj = json_object();
    if (!obj)
    {
        printf("Could not create object \n");
        return RET_ERR;
    }

    json_object_set_new(obj, key, json_integer(value));

    *jsRet = obj;
    return RET_OK;
}

static inline uint8_t jsonGetSubObject(json_t *root_js, char *key, json_t **jsRet)
{
    if (!key)
    {
        printf("[%s-%d] Invalid key for seach \n", __func__, __LINE__);
        return RET_ERR;
    }

    if(root_js)
    {
        *jsRet = json_object_get(root_js, key);
        if(!(*jsRet))
        {
            printf("[%s-%d] Invalid object (with key: %s) \n", __func__, __LINE__, key);
            return RET_ERR;
        }
    }
    else
    {
        printf("[%s-%d] Invalid Input object \n", __func__, __LINE__);
        return RET_ERR;
    }

    return RET_OK;
}

static inline uint8_t jsonObjectGetIntegerValue(json_t *root_js, char *key)
{
    uint8_t ret;
    json_t *js_check = NULL;

    if(!root_js)
    {
        printf("[%s-%d] Invalid Input object\n", __func__, __LINE__);
        return RET_ERR;
    }

    js_check = json_object_get(root_js, key);
    if(!js_check)
    {
        printf("Could not get_object, temporary assign '0' value\n");
        ret = 0;
    }
    else
        ret = json_integer_value(js_check);

    return ret;
}

static inline uint8_t jsonArrayGetIntegerValue(json_t *jsRootArr, uint8_t index)
{
    uint8_t ret = -1;
    json_t *js_check = NULL;

    if (jsRootArr)
    {
        js_check = json_array_get(jsRootArr, index);
        if(!js_check)
        {
            printf("Could not get js array \n");
            ret = -1;
        }
        else
            ret = json_integer_value(js_check);
    }
    return ret;
}

uint8_t jsonCreateArray(json_t **jsRet, uint8_t value)
{
    json_t *js_arr = json_array();
    if (!js_arr)
    {
        printf("Could not create object \n");
        return RET_ERR;
    }
    json_array_append_new(js_arr, json_integer(value));

    *jsRet = js_arr;
    return RET_OK;
}

void main()
{
    json_t *js_obj;
    char *buff;
    uint8_t ret;
    uint8_t tmp = 124;

    ret = jsonCreateObject(&js_obj, "hello", tmp);
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

    json_decref(js_obj);
    return;
}
