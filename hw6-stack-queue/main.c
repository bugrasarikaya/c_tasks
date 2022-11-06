#include<stdio.h> //This program was developed in Visual Studio 2017.
#include<stdlib.h>
#include<string.h>
#include<conio.h>
struct node_queue {
	int data_queue;
	struct node_queue *next;
};
struct node_queue *front = NULL;
struct node_queue *rear = NULL;
struct node_stack {
	int data_stack;
	struct node_stack *next;
};
struct node_stack *head = NULL;
void enqueue(int data_queue) { //Inserts a node to the queue.
	struct node_queue *ptr;
	ptr = (struct node_queue *) malloc(sizeof(struct node_queue));
	if (ptr == NULL) {
		printf("ERROR: The pointed memory was not allocated successfully.");
	}
	else {
		if (front == NULL) {
			ptr->data_queue = data_queue;
			front = rear = ptr;
			ptr->next = NULL;
		}
		else {
			ptr->data_queue = data_queue;
			rear->next = ptr;
			rear = ptr;
			ptr->next = NULL;
		}
	}
}
void dequeue() { //Deletes first node in the queue.
	struct node_queue *ptr = front;
	front = front->next;
	ptr->next = NULL;
	free(ptr);
	if (front == NULL) rear = NULL;
}
void push(int data_stack) { //Inserts a node to the stack.
	struct node_stack *ptr;
	ptr = (struct node_stack *) malloc(sizeof(struct node_stack));
	if (ptr == NULL) {
		printf("ERROR: The pointed memory was not allocated successfully.");
	}
	else{
		if (head == NULL) {
			ptr->data_stack = data_stack;
			head = ptr;
			ptr->next = NULL;
		}
		else {
			ptr->data_stack = data_stack;
			ptr->next = head;
			head = ptr;
		}
	}
}
int pop() { //Deletes first node in the stack and returns first node's data.
	struct node_stack *ptr=head;
	int temp_data = ptr->data_stack;
	head = head->next;
	ptr->next = NULL;
	free(ptr);
	return temp_data;
}
void scan_odds() { //Scans odd numbers in the queue.
	struct node_queue *ptr_queue = front;
	while (ptr_queue != NULL) {
		if (ptr_queue->data_queue % 2 == 1) {
			push(ptr_queue->data_queue);
			ptr_queue->data_queue = -1;  //Replaces odd number with "-1" to remember for implementing reverse_odds function.
		}
		ptr_queue = ptr_queue->next;
	}
}
void reverse_odds() { //Reverse odd numbers' order in queue.
	struct node_queue *ptr_queue = front;
	struct node_stack *ptr_stack = head;
	while (ptr_queue != NULL) {
		if (ptr_queue->data_queue == -1) { //Detects odd numbers' places.
			ptr_queue->data_queue=pop(); //Inserts odd numbers' from stack (LIFO feauture assists reversing).
		}
		ptr_queue = ptr_queue->next;
	}
}
void display() { //Displays elements of the queue.
	struct node_queue *ptr_queue = front;
	while (ptr_queue != NULL) {
		printf("%d ", ptr_queue->data_queue);
		ptr_queue = ptr_queue->next;
	}
}
int display_odds_order() { //Displays order of the odd integers.
	struct node_queue *ptr_queue = front;
	int i = 1;
	while (ptr_queue != NULL) {
		if (ptr_queue->data_queue % 2 == 1) { //Detects odd numbers.
			if ((ptr_queue->data_queue / 10) % 10 > 0) printf(" %d", i++); //If the the number has two digits writes more blanks to align output text.
			else if((ptr_queue->data_queue / 10) % 10 == 0) printf("%d", i++);
		}
		else {
			if ((ptr_queue->data_queue / 10) % 10 > 0) printf("  ");
			else if ((ptr_queue->data_queue / 10) % 10 == 0) printf(" ");
		}
		ptr_queue = ptr_queue->next;
		printf(" ");
	}
	return i - 1; //Returns quantity of odd numbers.
}
void display_odds_order_reversely(int odd_count) { //Displays order of the odd integers reversely.
	struct node_queue *ptr_queue = front;
	int i = odd_count;
	while (ptr_queue != NULL) {
		if (ptr_queue->data_queue % 2 == 1) { //Detects odd numbers.
			if ((ptr_queue->data_queue / 10) % 10 > 0) printf(" %d", i--); //If the the number has two digits writes more blanks to align output text.
			else if ((ptr_queue->data_queue / 10) % 10 == 0) printf("%d", i--);
		}
		else {
			if ((ptr_queue->data_queue / 10) % 10 > 0) printf("  ");
			else if ((ptr_queue->data_queue / 10) % 10 == 0) printf(" ");
		}
		ptr_queue = ptr_queue->next;
		printf(" ");
	}
}
int count_commas(char *string) { //Counts commas in given string.
	int comma_counter=0;
	int len=strlen(string);
	for (int i = 0; i < len; i++) if (*(string + i) == ',') comma_counter++;
	return comma_counter;
}
void output(int line_length_1, int line_length, int line_count, int limit) { //Reorders queue and displays the output before and after.
	display();
	for (int i = 0; i < line_length_1 - line_length + 4; i++) printf(" "); //Writes more blanks for shorter lines to align output text.
	printf("Original %d. Queue\n", line_count);
	int odd_count = display_odds_order();
	for (int i = 0; i < line_length_1 - line_length + 4; i++) printf(" ");
	printf("Order of the Odd Integers\n\n");
	scan_odds();
	reverse_odds();
	display();
	for (int i = 0; i < line_length_1 - line_length + 4; i++) printf(" ");
	printf("Reordered %d. Queue\n", line_count);
	display_odds_order_reversely(odd_count);
	for (int i = 0; i < line_length_1 - line_length + 4; i++) printf(" ");
	printf("Order of the Odd Integers\n");
	if (line_count < limit) printf("--------------------------------------------------------\n");
}
void read_n_display() { //Reads data.txt and inserts it's datas to the struct array.
	FILE *fp;
	fopen_s(&fp, "data.txt", "r");
	if (fp == NULL) perror("ERROR");
	else {
		char line[46]; //46 is a random number for taking the lines of data.txt but it is bigger than the length of a line.
		int line_count = 0;
		int element_counter, comma_count, limit, line_length_1, line_length;
		char *token = NULL;
		char *context = NULL;
		do {
			fgets(line, sizeof line, fp);
			element_counter = 0;
			if (line_count != 0) comma_count = count_commas(line); //Counts commas in the line.
			if (line_count == 0) {
				limit = atoi(line); //First line contains the number of total line count in data.txt.
				line_count++;
			}
			else {
				if (line_count == 1) {
					line_length_1 = strlen(line);
					line_length_1--; //Decreasing for last '\n' character.
				}
				line_length = strlen(line);
				if (line_count != limit) line_length--; 
				token = strtok_s(line, ",", &context); //Separates line by comma for the first element (number) of the line.
				while (token!=NULL) {
					enqueue(atoi(token)); //Inserts elements into the queue.
					element_counter++;
					if (element_counter < comma_count) token = strtok_s(NULL, ",", &context); //Separates line by comma for elements (numbers) which are not the first element of the line.
					else if(element_counter = comma_count)token = strtok_s(NULL, "\n", &context); //Separates line by new line character for the last element of the line.
				}
				output(line_length_1, line_length, line_count, limit);
				while (front != NULL) dequeue(); //Deletes queue for inserting next line.
				line_count++;
			}
		} while (line_count < limit + 1);
		fclose(fp);
	}
}
void main() {
	read_n_display();
	_getch();
}
