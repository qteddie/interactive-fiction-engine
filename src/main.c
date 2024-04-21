#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <toml.h>
#include <yaml.h>
#include <unistd.h>

static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1?msg1:"");
    exit(1);
}

void toml_to_yaml(toml_table_t *toml, yaml_document_t *document, int node)
{
    const char *key;
    toml_table_t *tab;
    toml_array_t *arr;
    char *raw;
    for (int i = 0; 0 != (key = toml_key_in(toml, i)); i++)
    {
        if ((tab = toml_table_in(toml, key)))
        {
            int new_node_id = yaml_document_add_mapping(document, NULL, YAML_BLOCK_MAPPING_STYLE);
            toml_to_yaml(tab, document, new_node_id);  // Recursively process the table
            yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
        }
        else if ((arr = toml_array_in(toml, key)))
        {
            int new_node_id = yaml_document_add_sequence(document, NULL, YAML_BLOCK_SEQUENCE_STYLE);
            for (int j = 0; j < toml_array_nelem(arr); j++)
            {
                if (toml_rtos(toml_raw_at(arr, j), &raw) == 0)
                {
                    yaml_document_append_sequence_item(document, new_node_id, yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
                    free(raw);
                }
            }
            yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), new_node_id);
        }
        else if (toml_rtos(toml_raw_in(toml, key), &raw) == 0)
        {
            yaml_document_append_mapping_pair(document, node, yaml_document_add_scalar(document, NULL, (yaml_char_t *)key, -1, YAML_PLAIN_SCALAR_STYLE), yaml_document_add_scalar(document, NULL, (yaml_char_t *)raw, -1, YAML_PLAIN_SCALAR_STYLE));
            free(raw);
        }
    }
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

    yaml_document_t document;
    yaml_document_initialize(&document, NULL, NULL, NULL, 1, 1);

    int root = yaml_document_add_mapping(&document, NULL, YAML_BLOCK_MAPPING_STYLE);
    toml_to_yaml(toml, &document, root);

    FILE *outfile = fopen("output.yaml", "w");
    if (!outfile)
    {
        error("Failed to open output file: ", strerror(errno));
    }

    yaml_emitter_t emitter;
    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, outfile);
    yaml_emitter_dump(&emitter, &document);

    toml_free(toml);
    yaml_document_delete(&document);
    yaml_emitter_delete(&emitter);
    fclose(fp);
    fclose(outfile);

    return 0;
}