#include "stdlib.h"

#include "../../common/include/errors.h"
#include "../include/dump.h"

errors_t DumpTree(Tree_t *tree, const char *filename, const char *funcname, int line)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    FILE *dot_file = fopen(DOT_FILE_NAME, "w");
    if (!dot_file)
        return FILE_NULL_PTR;

    CHECK_ER(DrawTree(tree, dot_file));
    fclose(dot_file);

    char cmd[CMD_LEN] = {};
    char svg_file[SVG_FILE_LEN] = {};
    static int dump_num = 0;
    sprintf(cmd, "dot dump.dot -Tsvg -o svgs/dump%d.svg", dump_num);
    sprintf(svg_file, "svgs/dump%d.svg", dump_num);
    dump_num++;
    system(cmd);

    fprintf(tree->html_file, "\t\t<img\n\t\t\tsrc=\"%s\"\n\t\t\twidth=\"1500\"\n"
                             "\t\t\theight=\"1000\"/>\n\t\t<pre>\n\tFile:%s\n\tFunction:%s\n"
                             "\tLine:%d\n\t\t</pre>\n", svg_file, filename, funcname, line);

    return SUCCESS;
}

errors_t DrawTree(Tree_t *tree, FILE *dot_file)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);
    MY_ASSERT(dot_file, "Null pointer given as argument.", return NULL_PTR);

    fprintf(dot_file, "digraph\n{\n");

    fprintf(dot_file, "\trandir=\"TB\";\n\tnodesep=1;\n\tranksep=1.2;\n");

    fprintf(dot_file, "\tsubgraph cluster_tree\n\t{\n");
    fprintf(dot_file, "\tbgcolor=\"palegreen4:gray15\";\n\tstyle=radial;\n\tgradientangle=0;\n");
    fprintf(dot_file, "\t\tnode[shape=\"doubleoctagon\", height=0.7, color=palegreen4, fontcolor=\"#3f2512\","
            "style=filled, fillcolor=\"palegreen3:palegreen4\", style=radial, gradientangle=0, "
            "fontname=\"CMU Typewriter Text\", fontsize=17];\n");
    fprintf(dot_file, "\n\nedge[color=\"#170d07\", style=bold, fontname=\"CMU Typewriter Text\", "
            "arrowsize=1.5, fontsize=18];\n");
    //printf("%p\n", tree->root);
    CHECK_ER(DrawNode(tree->root, dot_file));

    fprintf(dot_file, "\t}\n}\n");

    return SUCCESS;
}

errors_t DrawNode(Node_t *node, FILE *dot_file)
{
    MY_ASSERT(node, "Null pointer given as argument.", return NULL_PTR);
    MY_ASSERT(dot_file, "Null pointer given as argument.", return NULL_PTR);

    static int depth = 1;
    depth++;

    if (node->left || node->right)
        fprintf(dot_file, "\t\t\"%s\";\n", node->data);
    else
        fprintf(dot_file, "\t\t\"%s\"[shape=invhouse, style=filled, fillcolor=\"brown1:brown4\","
                "style=radial, gradientangle=90, color=\"brown4\"];\n", node->data);
    if (node->parent)
    {
        if (node == node->parent->left)
            fprintf(dot_file, "\t\t\"%s\" -> \"%s\"[label=\"Нет\", weight=%d, arrowhead=\"ricurvelicurve\"];\n",
                    node->parent->data, node->data, depth);
        else
            fprintf(dot_file, "\t\t\"%s\" -> \"%s\"[label=\"Да\", weight=%d, arrowhead=\"licurvericurve\"];\n",
                    node->parent->data, node->data, depth);
    }

    if (node->left)
        CHECK_ER(DrawNode(node->left, dot_file));

    if (node->right)
        CHECK_ER(DrawNode(node->right, dot_file));

    if (node->right && node->left)
        fprintf(dot_file, "\t\t{rank=same; \"%s\"; \"%s\";}\n", node->left->data, node->right->data);
    else if (node->right || node->left)
        fprintf(dot_file, "\t\t{rank=same; \"%s\";}\n",
                (node->left) ? node->left->data : node->right->data);

    depth--;

    return SUCCESS;
}


