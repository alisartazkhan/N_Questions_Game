/*
Name: Ali Sartaz Khan
Course: CSc 352
Description: C file that contains methods that run the M-Questions game
*/
#include "mq.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


/*
Method iterates recursively through the tree and asks questions depending on the level of the tree
and takes in input that decides which path of thr tree the program should take.

node: Pointer to the current node in the tree
questions_list: 2D array containing all the questions
index: index value of the questions_list
*/
void ask_questions(MQDecisionTreeNode* node, char* *questions_list, int index){
    char answer;
    if (strcmp(node->text, "NULL") == 0){ // leaf node
        // printf("Inside Leaf node!\n");
        if  (node->num_answers !=0){ 
            for (int i = 0; i < node->num_answers;i++){
                printf("is it a %s? (y/n)\n", node->answers[i]);
                scanf(" %c", &answer);
                if (answer == 'y'){printf("I guessed it!\n");return;}
            }
            printf("You got me :)\n");
        } 
        else{printf("You got me :)\n");}
        return;
    }

    printf("%s (y/n)\n", questions_list[index]);
    scanf(" %c", &answer);
   
    if (answer == 'y'){
        ask_questions(node->yes, questions_list, index+1);
        return;
    }
    if (answer == 'n'){
        ask_questions(node->no, questions_list, index+1);
        
    }

}

int main(int argc, char* argv[]){
    char* file_name = argv[1];
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
 
    MQDecisionTree* tree = MQ_build_tree(file_name);
 
    MQ_populate_tree(tree, file_name);

    ask_questions(tree->root, questions_list, 0);
 
    free_questions_list(questions_list, question_number);
    MQ_free_tree(tree);
    fclose(file);
    return 0;
}
