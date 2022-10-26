/*
Name: Ali Sartaz Khan
Description: C file that contains implementation of all the functions that 
in the header file
*/

#include "mq.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
Method prints whitespace.
spaces: number of spaces to be printed
*/
void print_spaces(int spaces){
    for (int i = 0; i < spaces; i++) {printf(" ");}
}

/*
Recursively prints all the nodes in the Decision Tree
node: Current node in the tree
spaces: number of whitespaces needed before printed
*/
void print_helper(MQDecisionTreeNode* node, int spaces){
    if (strcmp(node->text, "NULL") == 0){ // leaf node
        if  (node->num_answers !=0){ 
            printf("|");
            for (int i = 0; i < node->num_answers;i++){
                printf(" %s |", node->answers[i]);
            }
            printf("\n");
        } 
        else{printf("\n");}
        return;
    }

    printf("[%s]\n", node -> text);

    print_spaces(spaces);
    printf("-y-> ");
    print_helper(node -> yes, spaces+4);
    print_spaces(spaces);
    printf("-n-> ");
    print_helper(node -> no, spaces+4);


}


void MQ_print_tree(MQDecisionTree* tree){
    printf("[%s]\n", tree->root->text);
    printf("-y-> ");
    print_helper(tree-> root->yes, 4);
    printf("-n-> ");
    print_helper(tree-> root->no, 4);
}


int find_number_of_questions(char* line, char delimiter){
    int count = 0;
    for (int i = 0; line[i]; i++){
        if (line[i]==delimiter){
            count ++;
        }
    }

    return count;
}



char** build_questions_list(char* questions, int question_number){
    char* *questions_list = malloc(question_number*sizeof(char*)); // need to free
    char* token = strtok(questions, ",");
    questions_list[0] = calloc(strlen(token)+1,sizeof(char));// need to free
    strcpy(questions_list[0], token);
    int i =1;
   // loop through the string to extract all other tokens
   while(i<question_number) {
        // printf( "%s\n", token ); //printing each token
        token = strtok(NULL, ",");
        questions_list[i] = calloc(strlen(token)+1,sizeof(char));// need to free
        // questions_list[i] = token;
        strcpy(questions_list[i], token);
        
 
        i++;
   }

   return questions_list;

}


void free_questions_list(char* *questions_list, int question_number){

    for (int i = 0;i<question_number;i++){
        free(questions_list[i]);
    }
    free(questions_list);
}

/*
Method recursively Creates nodes in the tree
node: Pointer to current node
questions_number: number of questions
questions_list: 2D array containing all the questions
i: index number
*/
MQDecisionTreeNode* tree_insert_helper(MQDecisionTreeNode* node, int question_number, char* *questions_list, int i){
    if (i == question_number){ // leaf node
        // printf("%d\n", i); 
        node = malloc(sizeof(MQDecisionTreeNode)); // needs to free
        strcpy(node -> text, "NULL");
        node -> num_answers = 0;
        node -> answers = NULL;
        node -> yes = NULL;
        node -> no = NULL;

        return node;
    }
    if (node == NULL){
        // printf("%d\n", i);
        node = malloc(sizeof(MQDecisionTreeNode)); // needs to free
        strcpy(node -> text, questions_list[i]);
        node -> num_answers = 0;
        node -> answers = NULL;
        node -> yes = NULL;
        node -> no = NULL;
      
    }

    node -> yes = tree_insert_helper(node -> yes, question_number, questions_list, i+1);
    node -> no = tree_insert_helper(node -> no, question_number, questions_list, i+1);
    return node;

}

 
MQDecisionTree* MQ_build_tree(char* file_name){
    FILE* file = fopen(file_name, "r");
    if (file == NULL) { 
        fprintf(stderr, "error opening the file.\n");
        exit(1);
    }
    char number[100];
    fgets(number, 99, file);
    char questions[10000];
    fgets(questions, 9999, file);
    questions[strlen(questions)-1] = '\0'; // CHANGE THIS
    int question_number = find_number_of_questions(questions, '?');
    

    char* *questions_list = build_questions_list(questions, question_number); // need to free

    MQDecisionTree* tree = malloc(sizeof(MQDecisionTree));
    tree -> root = NULL;
    tree -> root =  tree_insert_helper(tree->root, question_number, questions_list, 0);
    free_questions_list(questions_list, question_number);
    fclose(file);
    return tree;

}


/*
Recusively iterates through the tree until it reachers the leaf node, and then fill in the appropriate 
answer for that node.
node: Pointer to current node
elements: number of elements
elem_list: 2D array containing all the elements
i: index number
*/
void populate_tree_helper(MQDecisionTreeNode* node, int elements, char* *elem_list, int i){

    if (strcmp(node->text, "NULL") == 0){ // leaf node
        if (node -> answers == NULL){
            node -> answers = malloc(100*sizeof(char*)); // NEED TO FREE
        }
        node -> answers[node->num_answers] = calloc(strlen(elem_list[0])+1, sizeof(char)); //need to free
        strcpy(node -> answers[node->num_answers], elem_list[0]);
        node -> num_answers +=1;
        return;
    }
    
    if (atoi(elem_list[i]) == 0){
        populate_tree_helper(node->no, elements, elem_list, i+1);
    } 
    else if (atoi(elem_list[i]) == 1){
        populate_tree_helper(node->yes, elements, elem_list, i+1);
    }
}


void MQ_populate_tree(MQDecisionTree* tree, char* file_name){
    FILE* file = fopen(file_name, "r");
    if (file == NULL) { 
        fprintf(stderr, "error opening the file.\n");
        exit(1);
    }
    char number[100];
    fgets(number, 99, file);
    int item_number = atoi(number);
    char questions[10000];
    fgets(questions, 9999, file);
    char line[1000];
    for(int i =0; i< item_number; i++){
        fgets(line, 999, file);
        // printf("%s", line);
        int elements = find_number_of_questions(line, ',')+1;
        char* *elem_list = build_questions_list(line, elements); // need to free
        populate_tree_helper(tree->root, elements, elem_list, 1);
        free_questions_list(elem_list, elements);
    }
    fclose(file);
}


/*
MEthod recursively iterates through the entire tree and free every node
and also frees arrays for the leaf nodes
node: Pointer to the current node
*/
void free_helper(MQDecisionTreeNode* node){
    if (node == NULL){return;}

    if (strcmp(node->text, "NULL") == 0){ // its a leaf node 
        int num = node -> num_answers;
        for (int i=0;i<num;i++){
            free(node->answers[i]);
        }
        free(node->answers);
        free(node);
        return;
    }

    free_helper(node->yes);
    free_helper(node->no);
    free(node);
}


void MQ_free_tree(MQDecisionTree* tree){
    
    free_helper(tree->root);
    free(tree);

}
