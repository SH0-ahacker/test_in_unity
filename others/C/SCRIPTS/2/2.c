#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

clock_t start, end;
extern int err_no; // globally holds the error no
int data_size; // holds the data set size
int attribute_size; // holds the attribute size
char *cross_table; // holds data set of cross table from .cxt file
int concept_count = 0; // holds generated concepts count

// local functions
void loadData(char *file_path);

void buildInitialConcept(char *obj, char *attr);

void computeConceptFrom(char *obj, char *attr, int attr_index);

void processConcept(char *obj, char *attr);

bool checkAttribute(int j, char *attr);

void makeExtent(char *extent, char *obj, int attr_index);

void makeIntent(char *intent, char *extent, int attr_index);

bool canonicity_test(char *attr, char *intent, int attr_index);

int main(int argc, char *argv[]) {
    loadData(argv[1]); // read data from file path

    char *ini_obj = (char *) malloc(data_size * sizeof(char)); // initial concept object list
    char *ini_attr = (char *) malloc(attribute_size * sizeof(char)); // initial concept attribute list
    buildInitialConcept(ini_obj, ini_attr); // make object and attribute list

    start = clock(); // start timing
    computeConceptFrom(ini_obj, ini_attr, 0); // invoke Close-by-One
    end = clock(); // stop timing

    printf("\nTotal Concepts : %d\n\n", concept_count);
    printf("execution time : %f seconds\n\n", ((double) (end - start) / CLOCKS_PER_SEC));

    // Free Memory
    free(cross_table);
    free(ini_obj);
    free(ini_attr);

    return 0;
}

// load data set file from given location
void loadData(char *file_path) {
    int err_num;
    FILE *file;
    if ((file = fopen(file_path, "rt")) == NULL) {
        err_num = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(err_num));
        exit(EXIT_FAILURE);
    } else {
        printf("\n~~~ Dataset Cross Table ~~~\n\n");

        char *buffer = NULL;
        size_t len = 0;
        ssize_t read;
        int line_count = 0;
        int data_read_count = 0;
        int obj_count = 0;
        int atr_count = 0;
        int x;
        while ((read = getline(&buffer, &len, file)) != -1) {
            // process lines
            if (buffer[0] == '\n') {
                // new line found
            } else {
                // skip first character on the .cxt file
                if (line_count != 0) {
                    if (line_count == 1) {
                        // data size found
                        data_size = atoi(buffer);
                    } else if (line_count == 2) {
                        // attribute size found
                        attribute_size = atoi(buffer);
                        cross_table = malloc(sizeof(char) * attribute_size * data_size); // allocate cross table
                    } else if (line_count > 2 && line_count <= (data_size + 2)) {
                        // read data set objects
                        obj_count++;
                    } else if (line_count > (2 + data_size) && line_count <= (2 + data_size + attribute_size)) {
                        // read attributes
                        atr_count++;
                        obj_count = 0; // reset obj count
                    } else if (line_count > (2 + data_size + attribute_size)) {
                        // read cross table
                        for (x = 0; x < attribute_size; x++) {
                            // check attribute present or not
                            if (buffer[x] == 'X') {
                                cross_table[(obj_count * attribute_size) + x] = '1'; // assign one when 'X'
                            } else {
                                cross_table[(obj_count * attribute_size) + x] = '0'; // assign zero when '.'
                            }
                            printf("%c", cross_table[(obj_count * attribute_size) + x]);
                        }
                        printf("\n");
                        obj_count++;
                    }
                }
                line_count++;
            }
        }
        fclose(file);
        free(buffer);
        printf("\n");
    }
}

// build up initial concept
// out: objects, attributes
void buildInitialConcept(char *obj, char *attr) {
    int i;
    int a;
    /**
     * assign objects
     * pass all objects into list, according to the theorem, (X)
     */
    for (i = 0; i < data_size; i++) {
        obj[i] = '1';
    }

    /**
     * assign attributes
     * set common attribute list for all objects on cross table (X up)
     */
    // go through attributes
    for (a = 0; a < attribute_size; a++) {
        bool status = true;
        // go through objects
        for (i = 0; i < data_size; i++) {
            if (cross_table[(i * attribute_size) + a] == '0') {
                status = false;
                break;
            }
        }
        if (status) {
            // attribute available for all objects, assign 1
            attr[a] = '1';
        } else {
            attr[a] = '0';
        }
    }
}

/**
 * Close-by-One Algorithm
 *
 * input :  1. object list
 *          2. attribute list
 *          3. current attribute index
 */
void computeConceptFrom(char *obj, char *attr, int attr_index) {
    // 1. Process Concept
    processConcept(obj, attr);
    // 2. go through attribute list
    int j;
    for (j = attr_index; j < attribute_size; j++) {
        // 3. check current attribute exist or not
        if (!checkAttribute(j, attr)) {
            // 4. make extent
            char extent[data_size];
            makeExtent(extent, obj, j);
            // 5. make intent
            char intent[attribute_size];
            makeIntent(intent, extent, j);
            // 6. do canonicity test
            if (canonicity_test(attr, intent, j)) {
                // 7. call computeConceptFrom
                computeConceptFrom(extent, intent, (j + 1));
            }
        }
    }
}

// store concept
void processConcept(char *obj, char *attr) {
//    printf("\n-------------------------------\n");
    printf("Concept - %d\n\n", concept_count);
/*    int i;
    // set objects details on concept lattice
    concept_lattice[concept_count].objects = malloc(sizeof(char) * data_size);
    printf("Object Set : ");
    strncpy(concept_lattice[concept_count].objects, obj, data_size);
    printf("%s ", concept_lattice[concept_count].objects);
    for (i = 0; i < data_size; i++) {
        concept_lattice[concept_count].objects[i] = obj[i];
        printf("%c ", obj[i]);
    }*/
    printf("\n");

    // set attribute details on concept latice
/*    concept_lattice[concept_count].attributes = (char *) malloc(sizeof(char) * attribute_size);
    printf("Attribute Set : ");
    strncpy(concept_lattice[concept_count].attributes, obj, attribute_size);
    for (i = 0; i < attribute_size; i++) {
        concept_latice[concept_count].attributes[i] = attr[i];
        printf("%c ", attr[i]);
    }
    printf("\n-------------------------------\n\n");*/
    concept_count++;
}

// check attribute contains on attribute list or not
bool checkAttribute(int j, char *attr) {
    bool status = true;
    if (attr[j] == '0') {
        status = false;
    }
    return status;
}

// make extent
void makeExtent(char *extent, char *obj, int attr_index) {
    int i, z;
//    printf("extent (attr : %d): ", attr_index);
    // go through cross table
    for (i = 0; i < data_size; i++) {
        extent[i] = '0'; // set default value
        if (cross_table[(i * attribute_size) + attr_index] == '1' && obj[i] != '0') {
            extent[i] = '1'; // set object index to extent list
        }
//        printf("%c ", extent[i]);
    }
//    printf("\n");
}

// make intent
void makeIntent(char *intent, char *extent, int attr_index) {
    int i, a;
    int empty_count = 0;
//    printf("intent (attr : %d): ", attr_index);
    // check extent is empty set
    for (i = 0; i < data_size; i++) {
        if (extent[i] == '0') {
            empty_count++;
        }
    }
    for (a = 0; a < attribute_size; a++) {
        // validate on empty extent set
        if (empty_count != data_size) {
            bool status = true;
            for (i = 0; i < data_size; i++) {
                // check extent available
                if (extent[i] != '0') {
                    // check related cross table index of current extent attribute availability
                    if (cross_table[(i * attribute_size) + a] != '1') {
                        status = false;
                        break;
                    }
                }
            }
            if (status) {
                intent[a] = '1';
            } else {
                intent[a] = '0';
            }
        } else {
            intent[a] = '1';
        }
//        printf("%c ", intent[a]);
    }
//    printf("\n");
}

// perform canonicity test
bool canonicity_test(char *attr, char *intent, int attr_index) {
    bool status = false;
    char set_1[attr_index];
    char set_2[attr_index];
    int set_1_c = 0;// holds set 1 found count
    int set_2_c = 0;// holds set 2 found count
    int i;
    // 1. check on atribute list
    for (i = 0; i < attr_index; i++) {
        // check attr set
        if (attr[i] == '1') {
            set_1_c++;
            set_1[i] = '1';
        } else {
            set_1[i] = '0';
        }

        // check intent set
        if (intent[i] == '1') {
            set_2_c++;
            set_2[i] = '1';
        } else {
            set_2[i] = '0';
        }
    }

    if (set_1_c == 0 && set_2_c == 0) {
        // both are empty set
        status = true;
    } else if ((set_1_c != 0 && set_2_c == 0) || (set_1_c == 0 && set_2_c != 0)) {
        // found some element(s) on either of set
        status = false;
    } else if (set_1_c == set_2_c) {
        // found element(s) on both sets
        for (i = 0; i < attr_index; i++) {
            if (set_1[i] != set_2[i]) {
                status = false;
                break;
            } else {
                status = true;
            }
        }
    }

    return status;
}
