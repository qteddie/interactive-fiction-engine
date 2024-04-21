#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <jansson.h>
#include <toml.h>
#include <yaml.h>
#include <unistd.h>

static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

json_t *toml_to_json(toml_table_t *toml)
{
    json_t *json = json_object();
    const char *key;
    toml_table_t *tab;
    toml_array_t *arr;
    char *raw;
    for (int i = 0; 0 != (key = toml_key_in(toml, i)); i++)
    {
        if ((tab = toml_table_in(toml, key)))
        {
            json_object_set_new(json, key, toml_to_json(tab));
        }
        else if ((arr = toml_array_in(toml, key)))
        {
            json_t *json_arr = json_array();
            for (int j = 0; j < toml_array_nelem(arr); j++)
            {
                if (toml_rtos(toml_raw_at(arr, j), &raw) == 0)
                {
                    json_array_append_new(json_arr, json_string(raw));
                    free(raw);
                }
            }
            json_object_set_new(json, key, json_arr);
        }
        else if (toml_rtos(toml_raw_in(toml, key), &raw) == 0)
        {
            json_object_set_new(json, key, json_string(raw));
            free(raw);
        }
    }
    return json;
}

int main()
{
    FILE *fp = fopen("static/toml/test.toml", "r");
    if (!fp)
    {
        error("Failed to open file: ", strerror(errno));
    }

    char errbuf[256];
    toml_table_t *toml = toml_parse_file(fp, errbuf, sizeof(errbuf));
    if (!toml)
    {
        error("Failed to parse TOML: ", errbuf);
    }

    json_t *json = toml_to_json(toml);
    json_dump_file(json, "static/json/output.json", JSON_INDENT(4));

    toml_free(toml);
    json_decref(json);
    fclose(fp);

    return 0;
}