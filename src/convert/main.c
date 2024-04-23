#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <toml.h>
// #include <yaml.h>
#include <jansson.h>
// void toml_to_yaml(toml_table_t *toml, yaml_document_t *document, int node);
static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}
// void process_table(toml_table_t *toml, yaml_document_t *document, int node, const char *key) {
//     toml_table_t *tab = toml_table_in(toml, key);
//     int new_node_id = yaml_document_add_mapping(document, NULL, YAML_BLOCK_MAPPING_STYLE);
//     toml_to_yaml(tab, document, new_node_id);  // Recursively process the table
//     yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
// }

// void process_array(toml_table_t *toml, yaml_document_t *document, int node, const char *key) {
//     toml_array_t *arr = toml_array_in(toml, key);
//     int new_node_id = yaml_document_add_sequence(document, NULL, YAML_BLOCK_SEQUENCE_STYLE);
//     char *raw;
//     for (int j = 0; j < toml_array_nelem(arr); j++) {
//         if (toml_rtos(toml_raw_at(arr, j), &raw) == 0) {
//             yaml_document_append_sequence_item(document, new_node_id, yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
//             free(raw);
//         }
//     }
//     yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
// }

// void process_scalar(toml_table_t *toml, yaml_document_t *document, int node, const char *key) {
//     char *raw;
//     if (toml_rtos(toml_raw_in(toml, key), &raw) == 0) {
//         yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
//         free(raw);
//     }
// }

// void toml_to_yaml(toml_table_t *toml, yaml_document_t *document, int node) {
//     const char *key;
//     for (int i = 0; 0 != (key = toml_key_in(toml, i)); i++) {
//         if (toml_table_in(toml, key)) {
//             process_table(toml, document, node, key);
//         } else if (toml_array_in(toml, key)) {
//             process_array(toml, document, node, key);
//         } else {
//             process_scalar(toml, document, node, key);
//         }
//     }
// }
// // void toml_to_yaml(toml_table_t *toml, yaml_document_t *document, int node)
// // {
// //     const char *key;
// //     toml_table_t *tab;
// //     toml_array_t *arr;
// //     char *raw;
// //     for (int i = 0; 0 != (key = toml_key_in(toml, i)); i++)
// //     {
// //         if ((tab = toml_table_in(toml, key)))
// //         {
// //             int new_node_id = yaml_document_add_mapping(document, NULL, YAML_BLOCK_MAPPING_STYLE);
// //             toml_to_yaml(tab, document, new_node_id);  // Recursively process the table
// //             yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
// //         }
// //         else if ((arr = toml_array_in(toml, key)))
// //         {
// //             int new_node_id = yaml_document_add_sequence(document, NULL, YAML_BLOCK_SEQUENCE_STYLE);
// //             for (int j = 0; j < toml_array_nelem(arr); j++)
// //             {
// //                 if (toml_rtos(toml_raw_at(arr, j), &raw) == 0)
// //                 {
// //                     yaml_document_append_sequence_item(document, new_node_id, yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
// //                     free(raw);
// //                 }
// //             }
// //             yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
// //         }
// //         else if (toml_rtos(toml_raw_in(toml, key), &raw) == 0)
// //         {
// //             yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
// //             free(raw);
// //         }
// //     }
// // }
// -----------------------------------------------------------------------------------------------------------
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

void process_toml_file(const char *input_file, const char *output_file)
{
    FILE *fp = fopen(input_file, "r");
    if (!fp)
    {
        error("Failed to open toml file: ", strerror(errno));
    }

    char errbuf[256];
    toml_table_t *toml = toml_parse_file(fp, errbuf, sizeof(errbuf));
    if (!toml)
    {
        error("Failed to parse TOML: ", errbuf);
    }

    json_t *json = toml_to_json(toml);
    json_dump_file(json, output_file, JSON_INDENT(4));

    toml_free(toml);
    json_decref(json);
    fclose(fp);
}

// void convert_toml_to_yaml(const char *input_file, const char *output_file)
// {
//     FILE *fp = fopen(input_file, "r");
//     if (!fp)
//     {
//         error("Failed to open file: ", strerror(errno));
//     }

//     char errbuf[256];
//     toml_table_t *toml = toml_parse_file(fp, errbuf, sizeof(errbuf));
//     if (!toml)
//     {
//         error("Failed to parse TOML: ", errbuf);
//     }

//     yaml_document_t document;
//     yaml_document_initialize(&document, NULL, NULL, NULL, 1, 1);

//     int root = yaml_document_add_mapping(&document, NULL, YAML_BLOCK_MAPPING_STYLE);
//     toml_to_yaml(toml, &document, root);

//     FILE *outfile = fopen(output_file, "w");
//     if (!outfile)
//     {
//         error("Failed to open output file: ", strerror(errno));
//     }

//     yaml_emitter_t emitter;
//     yaml_emitter_initialize(&emitter);
//     yaml_emitter_set_output_file(&emitter, outfile);
//     yaml_emitter_dump(&emitter, &document);

//     toml_free(toml);
//     yaml_document_delete(&document);
//     yaml_emitter_delete(&emitter);
//     fclose(fp);
//     fclose(outfile);
// }

int main()
{
    // ---------------------------------Transform the TOML file to YAML----------------------------------------
    // convert_toml_to_yaml("static/toml/test.toml", "output.yaml");
    // ---------------------------------Transform the TOML file to YAML----------------------------------------
    // Only do this once
    
    // ---------------------------------Transform the TOML file to JSON----------------------------------------
    process_toml_file("eddie.toml", "output.json");
    return 0;
    // ---------------------------------Transform the TOML file to JSON----------------------------------------
}