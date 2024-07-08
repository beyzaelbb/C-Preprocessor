#include <stdio.h>
#include <string.h>
#include "P_header.h"
#include <stdlib.h>

struct ArrayTable {
    char name[10];
    int dim;  // dim = 1 or 2
    char size1[10]; // dim1 size
    char size2[10]; // dim2 size
};
struct ArrayTable AT[20];  // max. 20 arrays stored 
int arrayCount = 0;

struct ParseTable {
    char oper[10]; // operation read copy etc
    char lhs[10];  // array before the = sign 
    char rhs1[10]; // first array after = if there is one 
    char rhs2[10]; // second array after = if there is one 
};
struct ParseTable PT; // obj of the struct

void lineParser(char* line) {
    // Clean PT Fields
    strcpy(PT.oper, "");
    strcpy(PT.lhs, "");
    strcpy(PT.rhs1, "");
    strcpy(PT.rhs2, "");

    // tokanizing the input according to given chars
    char* token = strtok(line, " .*+(),=<>\n");

    // Save tokanized elements from the line
    if (token != NULL) {
        strcpy(PT.oper, token);

        token = strtok(NULL, " .*+(),=<>\n");
        if (token != NULL) {
            strcpy(PT.lhs, token);
        }

        token = strtok(NULL, " .*+(),=<>\n");
        if (token != NULL) {
            strcpy(PT.rhs1, token);
        }

        token = strtok(NULL, " .*+(),=<>\n");
        if (token != NULL) {
            strcpy(PT.rhs2, token);
        }
    }
}

char* declare() {
    static char expandedLine[200];
    strcpy(expandedLine, "");

    // Fill the arraytable accordingly
    strcpy(AT[arrayCount].name, PT.lhs);
    AT[arrayCount].dim = (strlen(PT.rhs2) == 0) ? 1 : 2;
    strcpy(AT[arrayCount].size1, PT.rhs1);

    if (AT[arrayCount].dim == 2) {
        strcpy(AT[arrayCount].size2, PT.rhs2);
    } else {
        strcpy(AT[arrayCount].size2, "");
    }
    arrayCount++;


    // generated code
    if (AT[arrayCount-1].dim == 1) {
        sprintf(expandedLine, "int %s[%s];\n", AT[arrayCount-1].name, AT[arrayCount-1].size1);
    } else if (AT[arrayCount-1].dim == 2) {
        sprintf(expandedLine, "int %s[%s][%s];\n", AT[arrayCount-1].name, AT[arrayCount-1].size1, AT[arrayCount-1].size2);
    }

    return expandedLine;
}

char* read() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char arrayName[10];
    char fileName[50];
    int dim;
    char size1[10], size2[10];
 
    // Copy Array traits
    strcpy(arrayName, PT.lhs);
    strcpy(fileName, PT.rhs1);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, arrayName) == 0) {
            dim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dim == 2) {
                strcpy(size2, AT[i].size2);
            }
            break;
        }
    }

    sprintf(expandedLine, "FILE *file%s = fopen(\"../%s\", \"r\");\n", arrayName, fileName);

    if (dim == 1) {
        // 1D code generated
        sprintf(expandedLine + strlen(expandedLine), "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    fscanf(file%s, \"%%d\", &%s[i]);\n", arrayName, arrayName);
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    } else if (dim == 2) {
        // 2D code generated 
        sprintf(expandedLine + strlen(expandedLine), "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %s; j++) {\n", size2);
        sprintf(expandedLine + strlen(expandedLine), "        fscanf(file%s, \"%%d\", &%s[i][j]);\n", arrayName, arrayName);
        sprintf(expandedLine + strlen(expandedLine), "    }\n");
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    }

    sprintf(expandedLine + strlen(expandedLine), "fclose(file%s);\n", arrayName);

    return expandedLine;
}

char* copy() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char dest[10], src[10];
    int srcDim = 0, destDim = 0;
    char size1[10], size2[10];

    // Copy array traits
    strcpy(dest, PT.lhs);
    strcpy(src, PT.rhs1);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, src) == 0) {
            srcDim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (srcDim == 2) {
                strcpy(size2, AT[i].size2);
            }
        }
        if (strcmp(AT[i].name, dest) == 0) {
            destDim = AT[i].dim;
        }
    }

    // generate 1D and 2D codes 
    if (srcDim == 1) {
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    %s[i] = %s[i];\n", dest, src);
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    } else if (srcDim == 2) {
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %s; j++) {\n", size2);
        sprintf(expandedLine + strlen(expandedLine), "        %s[i][j] = %s[i][j];\n", dest, src);
        sprintf(expandedLine + strlen(expandedLine), "    }\n");
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    }

    return expandedLine;
}

char* initialize() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char arrayName[10];
    char initValue[10];
    int dim = 0;
    char size1[10], size2[10];

    // Copy array traits
    strcpy(arrayName, PT.lhs);
    strcpy(initValue, PT.rhs1); 

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, arrayName) == 0) {
            dim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dim == 2) {
                strcpy(size2, AT[i].size2);
            }
            break;
        }
    }
    // generate codes for dimentions 
    if (dim == 1) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    %s[i] = %s;\n", arrayName, initValue);
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    } else if (dim == 2) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %s; j++) {\n", size2);
        sprintf(expandedLine + strlen(expandedLine), "        %s[i][j] = %s;\n", arrayName, initValue);
        sprintf(expandedLine + strlen(expandedLine), "    }\n");
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    }

    return expandedLine;
}

char* print() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char arrayName[10];
    int dim = 0;
    char size1[10], size2[10];

    // Copy array traits
    strcpy(arrayName, PT.lhs);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, arrayName) == 0) {
            dim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dim == 2) {
                strcpy(size2, AT[i].size2);
            }
            break;
        }
    }

    if (dim == 1) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    printf(\"%%d \", %s[i]);\n", arrayName);
        sprintf(expandedLine + strlen(expandedLine), "}\n");
        sprintf(expandedLine + strlen(expandedLine), "printf(\"\\n\");\n");
    } else if (dim == 2) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %s; j++) {\n", size2);
        sprintf(expandedLine + strlen(expandedLine), "        printf(\"%%d \", %s[i][j]);\n", arrayName);
        sprintf(expandedLine + strlen(expandedLine), "    }\n");
        sprintf(expandedLine + strlen(expandedLine), "    printf(\"\\n\");\n");
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    }

    return expandedLine;
}

char* dotProduct() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char array1[10], array2[10];
    int dim1 = 0, dim2 = 0;
    char size1[10];

    // Copy Array treits
    strcpy(array1, PT.rhs1);
    strcpy(array2, PT.rhs2);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, array1) == 0) {
            dim1 = AT[i].dim;
            strcpy(size1, AT[i].size1);
        }
        if (strcmp(AT[i].name, array2) == 0) {
            dim2 = AT[i].dim;
        }
    }

    // Generate code for calculating the dot product
    sprintf(expandedLine, "P_dot = 0;\n");
    sprintf(expandedLine + strlen(expandedLine), "for (int i = 0; i < %s; i++) {\n", size1);
    sprintf(expandedLine + strlen(expandedLine), "    P_dot += %s[i] * %s[i];\n", array1, array2);
    sprintf(expandedLine + strlen(expandedLine), "}\n");

    return expandedLine;
}

char* addition() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char dest[10], src1[10], src2[10];
    int dimDest = 0, dimSrc1 = 0, dimSrc2 = 0;
    char size1[10], size2[10];

    // Copy Array Attributes
    strcpy(dest, PT.lhs);
    strcpy(src1, PT.rhs1);
    strcpy(src2, PT.rhs2);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, dest) == 0) {
            dimDest = AT[i].dim;
        }
        if (strcmp(AT[i].name, src1) == 0) {
            dimSrc1 = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dimSrc1 == 2) {
                strcpy(size2, AT[i].size2);
            }
        }
        if (strcmp(AT[i].name, src2) == 0) {
            dimSrc2 = AT[i].dim;
        }
    }


    if (dimDest == 1) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    %s[i] = %s[i] + %s[i];\n", dest, src1, src2);
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    } else if (dimDest == 2) {
        
        sprintf(expandedLine, "for (int i = 0; i < %s; i++) {\n", size1);
        sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %s; j++) {\n", size2);
        sprintf(expandedLine + strlen(expandedLine), "        %s[i][j] = %s[i][j] + %s[i][j];\n", dest, src1, src2);
        sprintf(expandedLine + strlen(expandedLine), "    }\n");
        sprintf(expandedLine + strlen(expandedLine), "}\n");
    }

    return expandedLine;
}

char* matrixMultiplication() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char result[10];
    char matrix1[10];
    char matrix2[10];

    // Copy Array traits
    strcpy(result, PT.lhs);
    strcpy(matrix1, PT.rhs1);
    strcpy(matrix2, PT.rhs2);

    // Get Dimensions
    int rows1 = 0, cols1 = 0, rows2 = 0, cols2 = 0;
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, matrix1) == 0) {
            rows1 = atoi(AT[i].size1);
            cols1 = atoi(AT[i].size2);
        }
        if (strcmp(AT[i].name, matrix2) == 0) {
            rows2 = atoi(AT[i].size1);
            cols2 = atoi(AT[i].size2);
        }
    }

    // Matrix multiplication logic
    sprintf(expandedLine + strlen(expandedLine), "for (int i = 0; i < %d; i++) {\n", rows1);
    sprintf(expandedLine + strlen(expandedLine), "    for (int j = 0; j < %d; j++) {\n", cols2);
    sprintf(expandedLine + strlen(expandedLine), "        %s[i][j] = 0;\n", result);
    sprintf(expandedLine + strlen(expandedLine), "        for (int k = 0; k < %d; k++) {\n", cols1);
    sprintf(expandedLine + strlen(expandedLine), "            %s[i][j] += %s[i][k] * %s[k][j];\n", result, matrix1, matrix2);
    sprintf(expandedLine + strlen(expandedLine), "        }\n");
    sprintf(expandedLine + strlen(expandedLine), "    }\n");
    sprintf(expandedLine + strlen(expandedLine), "}\n");

    return expandedLine;
}

char* sum() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char arrayName[10];
    int dim;
    char size1[10], size2[10];

    // Copy Array traits
    strcpy(arrayName, PT.lhs);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, arrayName) == 0) {
            dim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dim == 2) {
                strcpy(size2, AT[i].size2);
            }
            break;
        }
    }

    if (dim == 1) {
        sprintf(expandedLine, "P_sum = 0;\nfor (int i = 0; i < %s; ++i) {\n    P_sum += %s[i];\n}\n", size1, arrayName);
    } else if (dim == 2) {
        sprintf(expandedLine, "P_sum = 0;\nfor (int i = 0; i < %s; ++i) {\n    for (int j = 0; j < %s; ++j) {\n        P_sum += %s[i][j];\n    }\n}\n", size1, size2, arrayName);
    }

    return expandedLine;
}

char* aver() {
    static char expandedLine[500];
    strcpy(expandedLine, "");

    char arrayName[10];
    int dim;
    char size1[10], size2[10];
    int totalElements;

    // Copy Array traits
    strcpy(arrayName, PT.lhs);

    // find the wanted array in the AT and get sizes
    for (int i = 0; i < arrayCount; i++) {
        if (strcmp(AT[i].name, arrayName) == 0) {
            dim = AT[i].dim;
            strcpy(size1, AT[i].size1);
            if (dim == 2) {
                strcpy(size2, AT[i].size2);
            }
            break;
        }
    }

    // Calculate total number of elements
    if (dim == 1) {
        totalElements = atoi(size1);
    } else if (dim == 2) {
        totalElements = atoi(size1) * atoi(size2);
    }

    if (dim == 1) {
        sprintf(expandedLine, "P_aver = 0;\nfor (int i = 0; i < %s; ++i) {\n    P_aver += %s[i];\n}\nP_aver = %d;\n", size1, arrayName, totalElements);
    } else if (dim == 2) {
        sprintf(expandedLine, "P_aver = 0;\nfor (int i = 0; i < %s; ++i) {\n    for (int j = 0; j < %s; ++j) {\n        P_aver += %s[i][j];\n    }\n}\nP_aver = %d;\n", size1, size2, arrayName, totalElements);
    }

    return expandedLine;
}

int main(int args, char *argss[]) {
    FILE *infile = fopen(argss[1], "r");
    FILE *outfile = fopen("expanded.c", "w");
    char line[300];

    // error handling for if the files are null

    if (infile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        return 1;
    }

    while (fgets(line, sizeof(line), infile)) {
        // if the codes sees a whitespace move on
        int i = 0;
        while (line[i] == ' ') {
            i++;
        }

        // if the code sees @ parse the line and call the appropriate function
        if (line[i] == '@') {
            lineParser(line);
            char *expandedCode = NULL;
            if (strcmp(PT.oper, "@int") == 0) {
                expandedCode = declare();
            } else if (strcmp(PT.oper, "@read") == 0) {
                expandedCode = read();
            } else if (strcmp(PT.oper, "@copy") == 0) {
                expandedCode = copy();
            } else if (strcmp(PT.oper, "@init") == 0) {
                expandedCode = initialize();
            } else if (strcmp(PT.oper, "@print") == 0) {
                expandedCode = print();
            } else if (strcmp(PT.oper, "@dotp") == 0) {
                expandedCode = dotProduct();
            } else if (strcmp(PT.oper, "@add") == 0) {
                expandedCode = addition();
            } else if (strcmp(PT.oper, "@mmult") == 0) {
                expandedCode = matrixMultiplication();
            } else if (strcmp(PT.oper, "@sum") == 0) {
                expandedCode = sum();
            } else if (strcmp(PT.oper, "@aver") == 0) {
                expandedCode = aver();
            }
            if (expandedCode != NULL) {
                fprintf(outfile, "%s", expandedCode);
            }
        } else {
            fprintf(outfile, "%s", line);
        }
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}
