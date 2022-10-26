/*
Name: Ali Sartaz Khan
Course: CSc 352
Description: Header file that contains all the functions that 
need to be implemented for a decision tree.
*/


typedef struct MQDecisionTreeNode { // node in the tree
    char text[50];
    int num_answers;
    char** answers;
    struct MQDecisionTreeNode* yes;
    struct MQDecisionTreeNode* no;
} MQDecisionTreeNode;


typedef struct MQDecisionTree { // the tree containing root
    MQDecisionTreeNode* root;
} MQDecisionTree;



/*
Method prints out the tree in string format

tree: MQDecisionTree pointer to the tree struct
*/
void MQ_print_tree(MQDecisionTree* tree);


/*
Method builds the structure of the tree using questions given using the file and returns the 
pointer of that tree.

file_name: name of the file containing all data
*/
MQDecisionTree* MQ_build_tree(char* file_name);

/*
Method populates the leaf nodes in the tree using the data from the file

tree: MQDecisionTree pointer to the tree struct
file_name: name of the file containing all data
*/
void MQ_populate_tree(MQDecisionTree* tree, char* file_name);


/*
Method iterates through the tree and frees all the nodes, including the tree pointer itself

tree: MQDecisionTree pointer to the tree struct
*/
void MQ_free_tree(MQDecisionTree* tree);


/*
Method iterates through a 2D char array and frees every inner array and also the
outter array.

questions_list: Array containing strings of questions
question_number: number of questions in the array
*/
void free_questions_list(char* *questions_list, int question_number);

/*
Returns the number of questions in the line using the delimiter
line: string containing questions
delimiter: Char used to determine how to identify a question.
*/
int find_number_of_questions(char* line, char delimiter);


/*
Method builds a 2D array containing a strings using a string of questions and number
of questions.
questions: String containing all the questions
questions_number: number of questions
*/
char** build_questions_list(char* questions, int question_number);