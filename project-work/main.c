#include<stdio.h> //This program was developed in Visual Studio 2017.
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<conio.h>
int count_lines(char *directory) { //Counts lines in a given text file to determine array's size for dynamic allocation.
	FILE *fp;
	fopen_s(&fp, directory, "r");
	int line_count = 0;
	char chr;
	if (fp == NULL) perror("ERROR");
	else {
		chr = getc(fp);
		while (chr != EOF) {
			if (chr == '\n') line_count++;
			chr = getc(fp);
		}
		fclose(fp);
	}
	return line_count;
}
int find_max_length(char *directory, int line_count) { //Finds longest string length to determine array's size for minimizing dynamic allocated memory.
	int *length_list_array = (int *)malloc(sizeof(int)*line_count);
	FILE *fp;
	fopen_s(&fp, directory, "r");
	if (fp == NULL) perror("ERROR");
	else {
		int count = 0;
		char line[50]; //50 is a random number which is bigger than the longest password which is 39 in the 1 M passwords text file.
		char *token = NULL;
		char *context = NULL;
		while (fgets(line, sizeof line, fp) != NULL) {
			token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
			length_list_array[count++] = strlen(token);
		}
		fclose(fp);
	}
	int max = 0;
	for (int i = 0; i < line_count; i++) {
		if (length_list_array[i] > max) max = length_list_array[i];
	}
	free(length_list_array);
	return max;
}
void read_n_insert_into_array(char **list, char *directory, int string_length) { //Reads text file and inserts it's datas to the char array.
	FILE *fp;
	fopen_s(&fp, directory, "r");
	if (fp == NULL) perror("ERROR");
	else {
		char *line = (char *)malloc(sizeof(char)*(string_length + 2)); //Implements dynamic allocation in a sufficient size which is the result of "string_length + 1('\n') + 1('\0')".
		char *token = NULL;
		char *context = NULL;
		int count = 0;
		while (fgets(line, (string_length + 2), fp) != NULL) {
			token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
			strcpy_s(list[count++], strlen(token) + 1, token);
		}
		free(line);
		fclose(fp);
	}
}
void search_in_text_file(char *directory, int string_length) { //Displays search menu and implements search in 100000.txt.
	clock_t begin, end;
	do {
		printf("1. Search a password.\n");
		printf("2. Return to menu.\n");
		printf("Type an option: ");
		int option;
		int found;
		while (scanf_s("%d", &option) != 1) getchar(); //If user enters characters instead of integers, this getchar() discards characters.
		switch (option) {
		case 1:
			printf("Type a password: ");
			char string[50];
			int line_count = 0;
			found = 0;
			scanf_s("%s", string, sizeof string);
			begin = clock();
			FILE *fp;
			fopen_s(&fp, directory, "r");
			if (fp == NULL) perror("ERROR");
			else {
				char *line = (char *)malloc(sizeof(char)*(string_length + 2)); //Implements dynamic allocation in a sufficient size which is the result of "string_length + 1('\n') + 1('\0')".
				char *token = NULL;
				char *context = NULL;
				while (fgets(line, string_length + 2, fp) != NULL) {
					token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
					if (!strcmp(string, token)) { //Compares every password with given string.
						end = clock();
						printf("Found at line %d. ", line_count + 1);
						printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
						found = 1;
						break;
					}
					line_count++;
				}
				if (!found) printf("Not found.\n");
				break;
				free(line);
				fclose(fp);
			}
		case 2:
			return;
		default:
			printf("Invalid input.\n");
			break;
		}
	} while (1);
}
void search(char **list, int limit) { //Displays search menu and implements search in 2D char array for 100000.txt (Removed because of inserting text data into array before searching increases time and memory.).
	do {
		printf("1. Search a password.\n");
		printf("2. Return to menu.\n");
		printf("Type an option: ");
		int option;
		int found;
		while (scanf_s("%d", &option) != 1) getchar(); //If user enters characters instead of integers, this getchar() discards characters.
		switch (option) {
		case 1:
			printf("Type a password: ");
			char string[50];
			found = 0;
			scanf_s("%s", string, sizeof string);
			clock_t begin, end;
			begin = clock();
			for (int i = 0; i < limit; i++) {
				if (!strcmp(string, list[i])) { //Compares every password with given string.
					printf("Found at index %d. ", i);
					end = clock();
					printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
					found = 1;
					break;
				}
			}
			if (!found) printf("Not found.\n");
			break;
		case 2:
			return;
		default:
			printf("Invalid input.\n");
			break;
		}
	} while (1);
}
void display_test(char **list, int size) { //Test function for displaying array elements (passwords).
	for (int i = 0; i < size; i++) {
		printf("%s\n", list[i]);
	}
}
void free_array_2D_char(char **array, int size) { //Frees allocated memory of 2D array.
	for (int i = 0; i < size; i++) free(array[i]);
	free(array);
}
void search_objective(char *directory, int m_pwds_mode) {
	(!m_pwds_mode) ? printf("\n--------- Searching in Text File ---------\n") : printf("\n--------- Searching in Text File for 1000000.txt ---------\n");
	int line_count = count_lines(directory);
	int max_length = find_max_length(directory, line_count);
	search_in_text_file(directory, max_length);
}
void swap_strings(char **string_1, char **string_2) { //Swaps two strings.
	char *temp_string = *string_1;
	*string_1 = *string_2;
	*string_2 = temp_string;
}
int partition_in_array_ascending(char **list, int low, int high) { //Divides the array for implementing quick sort.
	int pivot_index = high;
	int i = (low - 1);
	for (int j = low; j < high; j++) {
		if (strcmp(list[j], list[pivot_index]) < 0) { //Moves low valuable string to the left side of array.
			i++;
			swap_strings(&list[i], &list[j]);
		}
	}
	swap_strings(&list[i + 1], &list[pivot_index]);
	return (i + 1);
}
void quick_sort_in_array_ascending(char **list, int low, int high) { //Quick sort function for sorting 2D char array in ascending order.
	if (low < high) {
		int pivot_index = partition_in_array_ascending(list, low, high);
		quick_sort_in_array_ascending(list, low, pivot_index - 1);
		quick_sort_in_array_ascending(list, pivot_index + 1, high);
	}
}
void sort_objective_output(char **ascending_array, char **descending_array, int line_count, int limit) { //Displays result of sort objective.
	char *gap = " ";
	char str[10];
	int max_rak_asc_length, max_rak_des_length, max_pwd_asc_length;
	max_rak_asc_length = max_rak_des_length = max_pwd_asc_length = 0;
	int rak_asc_length, rak_des_length, pwd_asc_length;
	for (int rank_asc = 1; rank_asc < limit + 1; rank_asc++) {
		_itoa_s(rank_asc, str, 10, 10);
		rak_asc_length = strlen(str);
		pwd_asc_length = strlen(ascending_array[rank_asc - 1]);
		if (rak_asc_length > max_rak_asc_length) max_rak_asc_length = rak_asc_length;
		if (pwd_asc_length > max_pwd_asc_length) max_pwd_asc_length = pwd_asc_length;
	}
	for (int rank_desc = line_count; rank_desc > line_count - limit; rank_desc--) {
		_itoa_s(rank_desc, str, 10, 10);
		rak_des_length = strlen(str);
		if (rak_des_length > max_rak_des_length) max_rak_des_length = rak_des_length;
	}
	for (int rank = 1; rank <= limit; rank++) {
		for (int option = 0; option < 4; option++) {
			switch (option % 4) {
			case 0: //Ranks in ascending order.
				printf("%d.", rank);
				_itoa_s(rank, str, 10, 10);
				rak_asc_length = strlen(str);
				for (int i = 0; i < max_rak_asc_length - rak_asc_length + 1; i++) printf("%s", gap);
				break;
			case 1: //Strongest passwords.
				printf("%s", ascending_array[rank - 1]);
				pwd_asc_length = strlen(ascending_array[rank - 1]);
				for (int i = 0; i < max_pwd_asc_length - pwd_asc_length + 5; i++) printf("%s", gap);
				break;
			case 2: //Ranks in descending order.
				_itoa_s(line_count - rank + 1, str, 10, 10);
				rak_des_length = strlen(str);
				for (int i = 0; i < max_rak_des_length - rak_des_length + 0; i++) printf("%s", gap);
				printf("%d.", line_count - rank + 1);
				_itoa_s(line_count - rank + 1, str, 10, 10);
				printf("%s", gap);
				break;
			case 3: //Weakest passwords.
				printf("%s", descending_array[rank-1]);
				printf("\n");
				break;
			}
		}
	}
}
void copy_arrays_reverse(char **array_dstn, char **array_src, int line_count) { //Copies data of an array to another array in reversely.
	for (int i = 0; i < line_count; i++) {
		strcpy_s(array_dstn[line_count - i - 1], strlen(array_src[i]) + 1, array_src[i]);
	}
}
void sort_objective(char *directory, int m_pwds_mode) {
	clock_t begin, end;
	(!m_pwds_mode) ? printf("\n--------- Quick Sort in Text File --------\n"): printf("\n--------- Quick Sort in Text File for 1000000.txt --------\n");
	begin = clock();
	int line_count = count_lines(directory);
	int max_length = find_max_length(directory, line_count);
	char **ascending_array = (char **)malloc(sizeof(char *) * line_count);
	for (int i = 0; i < line_count; i++) ascending_array[i] = (char *)malloc(sizeof(char) * (max_length + 1)); //Dynamic allocation of "array[10000][17]" for "10000.txt".
	read_n_insert_into_array(ascending_array, directory, max_length);
	printf("Implementing Quick Sort in char array...");
	quick_sort_in_array_ascending(ascending_array, 0, line_count - 1);
	printf(" COMPLETED\n");
	char **descending_array = (char **)malloc(sizeof(char *) * line_count);
	for (int i = 0; i < line_count; i++) descending_array[i] = (char *)malloc(sizeof(char) * (max_length + 1)); //Dynamic allocation of "descending_array[10000][17]" for "10000.txt".
	copy_arrays_reverse(descending_array, ascending_array, line_count);
	end = clock();
	sort_objective_output(ascending_array, descending_array, line_count, 10);
	free_array_2D_char(ascending_array, line_count);
	free_array_2D_char(descending_array, line_count);
	printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
	printf("Press any button to return menu.");
	_getch();
}
void reset_hash_table(char **hash_table, int hash_table_size) { //Prepares the hash_table before hashing.
	for (int i = 0; i < hash_table_size; i++) {
		hash_table[i][0] = '£'; //If first character of password is "£", this index is empty.
	}
}
uint32_t hash_function(const uint8_t *string, int k, int hash_table_size) { //This Jenkins's one_at_a_time hash function gives lass collusion count. Source is "https://en.wikipedia.org/wiki/Jenkins_hash_function", last visited at 13.1.2021.
	uint32_t  hash_key = 0;
	size_t i = 0;
	size_t  length = strlen(string);
	while (i != length) {
		hash_key += string[i++];
		hash_key += hash_key << 10;
		hash_key ^= hash_key >> 6;
	}
	hash_key += hash_key << 3;
	hash_key ^= hash_key >> 11;
	hash_key += hash_key << 15;
	hash_key = (hash_key + k) % hash_table_size;
	return hash_key;
}
void insert_to_hash_table(char *directory, int string_length, char **hash_table, uint32_t(*hash_function)(const uint8_t *, int, int), int *collision, int line_count, int hash_table_size) { //Inserts data into hash table.
	int hash_key;
	int k, not_found;
	int percent_factor = 0;
	int percent = line_count / 100;
	int i = 0;
	FILE *fp;
	fopen_s(&fp, directory, "r");
	if (fp == NULL) perror("ERROR");
	else {
		char *line = (char *)malloc(sizeof(char)*(string_length + 2)); //Implements dynamic allocation in a sufficient size which is the result of "string_length + 1('\n') + 1('\0')".
		char *token = NULL;
		char *context = NULL;
		while (fgets(line, string_length + 2, fp) != NULL) {
			token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
			k = 0; //This counter is for linear probing.
			not_found = 1;
			while (not_found) {
				hash_key = hash_function(token, k, hash_table_size);
				if (hash_table[hash_key][0] == '£') { //Checks index. If index is empty, it inserts data.
					strcpy_s(hash_table[hash_key], strlen(token) + 1, token);
					collision[hash_key] = k; //Saves collision count.
					not_found = 0;
					if (i == percent * percent_factor) printf("\rInserting data into hash table from text. %d%%", percent_factor++);
					if (i == line_count - 1) printf("\rInserting data into hash table from text. %%100");
					i++;
				}
				else {
					k++; //If index is not empty, it applies linear probing.
				}
			}
		}
		printf(" COMPLETED\n");
		free(line);
		fclose(fp);
	}
}
void search_in_hash_table(char **hash_table, uint32_t(*hash_function)(const uint8_t *, int, int), int hash_table_size) { //Displays search menu and implements search in hash table.
	clock_t begin, end;
	do {
		printf("1. Search a password.\n");
		printf("2. Return to menu.\n");
		printf("Type an option: ");
		int option;
		int found;
		while (scanf_s("%d", &option) != 1) getchar(); //If user enters characters instead of integers, this getchar() discards characters.
		switch (option) {
		case 1:
			printf("Type a password: ");
			char string[50];
			found = 0;
			scanf_s("%s", string, sizeof string);
			begin = clock();
			int hash_key;
			int k = 0; //This counter is for linear probing.
			while (k != hash_table_size) {
				hash_key = hash_function(string, k, hash_table_size);
				if (strcmp(hash_table[hash_key], string) == 0) {
					end = clock();
					printf("Found at index %d. Collision count is %d. ", hash_key, k);
					printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
					found = 1;
					break;
				}
				else {
					k++;
					found = 0;
				}
			}
			if (!found) printf("Not found.\n");
			break;
		case 2:
			return;
		default:
			printf("Invalid input.\n");
			break;
		}
	} while (1);
}
void report_collision(int *collision, int hash_table_size) { //Displays collision report.
	printf("Hash table's size is %d.\n", hash_table_size);
	int total = 0;
	for (int i = 0; i < hash_table_size; i++) {
		total = total + collision[i];
	}
	printf("Total collision count is %d.\n", total);
	int max_collision = collision[0];
	for (int i = 0; i < hash_table_size; i++) {
		if (collision[i] > max_collision) max_collision = collision[i];
	}
	printf("Maximum collision count is %d.\n", max_collision);
}
int check_prime_number(int number) { //Checkes prime number status.
	for (int i = 2; i <= number / 2; i++) if (number % i == 0) return 0;
	return 1;
}
int find_nearest_prime_number(int number) { //Finds nearest prime number.
	int low_value, high_value;
	for (int i = number; i > 1 / 2; i--) {
		if (check_prime_number(i)) { //Detect nearest low value.
			low_value = i;
			break;
		}
	}
	for (int i = number; ; i++) {
		if (check_prime_number(i)) { //Detect nearest high value.
			high_value = i;
			break;
		}
	}
	if (number - low_value < high_value - number) return low_value; //Detects the nearest one.
	else return high_value;
}
void hash_objective(char *directory, int m_pwds_mode) {
	clock_t begin, end;
	(!m_pwds_mode) ? printf("\n--------- Searching in Hash Table --------\n") : printf("\n--------- Searching in Hash Table for 1000000.txt --------\n");
	begin = clock();
	int line_count = count_lines(directory);
	int max_length = find_max_length(directory, line_count);
	int formula_result = ((line_count * 100) / 40); //"array_size*100/40" formula finds minimum hash_table size get minimum optimum collision count. Source is "https://stackoverflow.com/questions/22741966/how-to-choose-size-of-hash-table", last visited at 13.1.2021.
	int hash_table_size = find_nearest_prime_number(formula_result); //Assigns nearest prime number for formula result. It is 2503 for "1000.txt".
	char **hash_table = (char **)malloc(sizeof(char *) * hash_table_size);
	for (int i = 0; i < hash_table_size; i++) hash_table[i] = (char *)malloc(sizeof(char) * (max_length + 1)); //Dynamic allocation of "hash_table[2503][11]" for "1000.txt".
	reset_hash_table(hash_table, hash_table_size);
	int *collision = (int *)malloc(hash_table_size * sizeof(int)); //Dynamic allocation of "collision[2503]" for "1000.txt".
	for (int i = 0; i < hash_table_size; i++) collision[i] = 0; //Initializing of collision array.
	insert_to_hash_table(directory, max_length, hash_table, hash_function, collision, line_count, hash_table_size);
	end = clock();
	report_collision(collision, hash_table_size);
	printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
	search_in_hash_table(hash_table, hash_function, hash_table_size);
	free(collision);
	free_array_2D_char(hash_table, hash_table_size);
}
struct passwords { //Doubly linked list's node struct.
	char password[50];
	int score;
	struct passwords *next;
	struct passwords *prev;
};
int score_consecutiveness_status(char *password) { //Gives negative score according to count of consecutiveness statuses.
	int score = 0;
	int length = strlen(password);
	for (int i = 0; i < length - 1; i++) {
		if (*(password + i) >= 48 && *(password + i) <= 56) { //Statement of the number character which is between 0 and 8.
			if (*(password + i) + 1 == *(password + i + 1)) score--;
			else if (*(password + i) - 1 == *(password + i + 1)) score--;
		}
		else if (*(password + i) >= 65 && *(password + i) <= 89) { //Statement of the upper-case letter character between A and Y.
			if (*(password + i) + 1 == *(password + i + 1)) score--;
			else if (*(password + i) - 1 == *(password + i + 1)) score--;
		}
		else if (*(password + i) >= 97 && *(password + i) <= 121) { //Statement of the lower-case letter character between a and y.
			if (*(password + i) + 1 == *(password + i + 1)) score--;
			else if (*(password + i) - 1 == *(password + i + 1)) score--;
		}
	}
	if (!score) return 2; //If there is no consecutive characters, it gives 2 score.
	return score;
}
int score_password_length(char *password) { //Gives positive score as much as password length count.
	return strlen(password) * 2; //Gives 2 points for every character.
}
int score_transitions(char *password) { //Gives positive score as much as count of transitions to other character types(symbol, number, upper-case letter, lower-case letter).
	int score = 0;
	int length = strlen(password);
	for (int i = 0; i < length - 1; i++) {
		if ((*(password + i) >= 33 && *(password + i) <= 47) || (*(password + i) >= 58 && *(password + i) <= 64) || (*(password + i) >= 91 && *(password + i) <= 96) || (*(password + i) >= 123 && *(password + i) <= 126)) { //Statement of the symbol character.
			if (*(password + i + 1) >= 48 && *(password + i + 1) <= 57) score++; //Transition from symbol to number.
			else if (*(password + i + 1) >= 65 && *(password + i + 1) <= 90) score++; //Transition from symbol to upper-case letter.
			else if (*(password + i + 1) >= 97 && *(password + i + 1) <= 122) score++; //Transition from symbol to lower-case letter.
		}
		else if (*(password + i) >= 48 && *(password + i) <= 57) { //Statement of the number character.
			if ((*(password + i + 1) >= 33 && *(password + i + 1) <= 47) || (*(password + i + 1) >= 58 && *(password + i + 1) <= 64) || (*(password + i + 1) >= 91 && *(password + i + 1) <= 96) || (*(password + i + 1) >= 123 && *(password + i + 1) <= 126)) score++; //Transition from number to symbol.
			else if (*(password + i + 1) >= 65 && *(password + i + 1) <= 90) score++; //Transition from number to upper-case letter.
			else if (*(password + i + 1) >= 97 && *(password + i + 1) <= 122) score++; //Transition from number to lower-case letter.
		}
		else if (*(password + i) >= 65 && *(password + i) <= 90) { //Statement of the upper-case letter character.
			if ((*(password + i + 1) >= 33 && *(password + i + 1) <= 47) || (*(password + i + 1) >= 58 && *(password + i + 1) <= 64) || (*(password + i + 1) >= 91 && *(password + i + 1) <= 96) || (*(password + i + 1) >= 123 && *(password + i + 1) <= 126)) score++; //Transition from upper-case letter to symbol.
			else if (*(password + i + 1) >= 48 && *(password + i + 1) <= 57) score++; //Transition from upper-case letter character to number.
			else if (*(password + i + 1) >= 97 && *(password + i + 1) <= 122) score++; //Transition from upper-case letter to lower-case letter.
		}
		else if (*(password + i) >= 97 && *(password + i) <= 122) { //Statement of the lower-case letter character.
			if ((*(password + i + 1) >= 33 && *(password + i + 1) <= 47) || (*(password + i + 1) >= 58 && *(password + i + 1) <= 64) || (*(password + i + 1) >= 91 && *(password + i + 1) <= 96) || (*(password + i + 1) >= 123 && *(password + i + 1) <= 126)) score++; //Transition from lower-case letter to symbol.
			else if (*(password + i + 1) >= 48 && *(password + i + 1) <= 57) score++; //Transition from lower-case letter to number.
			else if (*(password + i + 1) >= 65 && *(password + i + 1) <= 90) score++; //Transition from lower-case letter to upper-case letter.
		}
	}
	return score;
}
int score_different_type_count(char *password) { //Gives positive score as much as different type count (It's maximum value is 4 because there is only 4 different type.).
	int score = 0;
	int type_symbol, type_number, type_upper_case_letter, type_lower_case_letter;
	type_symbol = type_number = type_upper_case_letter = type_lower_case_letter = 0;
	int length = strlen(password);
	for (int i = 0; i < length; i++) {
		if ((*(password + i) >= 33 && *(password + i) <= 47) || (*(password + i) >= 58 && *(password + i) <= 64) || (*(password + i) >= 91 && *(password + i) <= 96) || (*(password + i) >= 123 && *(password + i) <= 126)) { //Statement of the symbol character.
			if (!(type_symbol++)) score++;
		}
		else if (*(password + i) >= 48 && *(password + i) <= 57) { //Statement of the number character.
			if (!(type_number++)) score++;
		}
		else if (*(password + i) >= 65 && *(password + i) <= 90) { //Statement of the upper-case letter character.
			if (!(type_upper_case_letter++)) score++;
		}
		else if (*(password + i) >= 97 && *(password + i) <= 122) { //Statement of the lower-case letter character.
			if (!(type_lower_case_letter++)) score++;
		}
	}
	return score;
}
int score_consecutive_iteration_status(char *password) { //Gives negative score as much as consecutive iteration count.
	int score = 0;
	int length = strlen(password);
	for (int i = 0; i < length - 1; i++) if (*(password + i) == *(password + i + 1)) score--; //If current character is equal to next character, score decreases.
	if (!score) return 2; //If there is no consecutive characters, it gives 2 score.
	return score;
}
int score_all_types_availability(char *password) { //Gives 2 positive score if all types are available in password.
	int type_symbol, type_number, type_upper_case_letter, type_lower_case_letter;
	type_symbol = type_number = type_upper_case_letter = type_lower_case_letter = 0;
	int length = strlen(password);
	for (int i = 0; i < length; i++) {
		if ((*(password + i) >= 33 && *(password + i) <= 47) || (*(password + i) >= 58 && *(password + i) <= 64) || (*(password + i) >= 91 && *(password + i) <= 96) || (*(password + i) >= 123 && *(password + i) <= 126)) { //Statement of the symbol character.
			if (!type_symbol) type_symbol = 1;
		}
		else if (*(password + i) >= 48 && *(password + i) <= 57) { //Statement of the number character.
			if (!type_number) type_number = 1;
		}
		else if (*(password + i) >= 65 && *(password + i) <= 90) { //Statement of the upper-case letter character.
			if (!type_upper_case_letter) type_upper_case_letter = 1;
		}
		else if (*(password + i) >= 97 && *(password + i) <= 122) { //Statement of the lower-case letter character.
			if (!type_lower_case_letter) type_lower_case_letter = 1;
		}
	}
	if (type_symbol && type_number && type_upper_case_letter && type_lower_case_letter) return 2;
	else return 0;
}
void turn_to_lower_case(char *new_string, char *src_string) { //Turns string's upper-case letters to lower-case letters
	int str_length = strlen(src_string);
	strcpy_s(new_string, str_length + 1, src_string);
	for (int i = 0; i < str_length; i++) if (new_string[i] >= 65 && new_string[i] <= 90) new_string[i] = new_string[i] + 32;
}
void turn_to_lower_case_2d_array(char **array, int line_count) { //Turns 2D char array's upper-case letters to lower-case letters
	char temp_str[50];
	for (int i = 0; i < line_count; i++) {
		turn_to_lower_case(temp_str, array[i]);
		strcpy_s(array[i], strlen(temp_str) + 1, temp_str);
	}
}
int score_availability_in_dictionary(char *password, char **dictionary, int size) { //Gives negative score according to availibity of dictionary words in password.
	int score = 0;
	char low_pwd[50];
	turn_to_lower_case(low_pwd, password); //Turns password's letters to lower-case letter to find matches in the dictionary which has lower-case words.
	for (int i = 0; i < size; i++) {
		if (strstr(low_pwd, dictionary[i]) != NULL) score--;
	}
	if (!score) return 2; //If there is no consecutive characters, it gives 2 score.
	return score * 2; //Gives -2 points for every match.
}
int score_iteration_status(char *password) { //Gives negative score as much as iteration count.
	int score = 0;
	char temp_pwd[50];
	strcpy_s(temp_pwd, strlen(password) + 1, password);
	int length = strlen(temp_pwd);
	char c;
	for (int i = 0; i < length; i++) {
		if (temp_pwd[i] != '£') {  //Chosen characters will not be selected again.
			c = temp_pwd[i];
			for (int j = 0; j < length; j++) {
				if (j == i) continue;
				if (c == temp_pwd[j]) {
					score--;
					temp_pwd[j] = '£'; //"£" symbol is for specifying chosen characters to remember.
				}
			}
		}
	}
	if (!score) return 2; //If there is no consecutive characters, it gives 2 score.
	return score;
}
int score_blacklisted_status(char *password) { ////Gives negative score according to availibity of blacklisted words in password.
	int score = 0;
	char low_pwd[50];
	strcpy_s(low_pwd, strlen(password) + 1, password);
	char blacklisted[28][11] = { "qwe","qwer","qwert","qwerty","qwertyu","qwertyui","qwertyuio","qwertyuiop",
									"asd", "asdf", "asdfg", "asdfgh", "asdfghj", "asdfghjk", "asdfghjkl",
									"zxc", "zxcv", "zxcvb", "zxcvbn", "zxcvbnm",
									"wasd","esdf","rdfg","tfgh","yghj","uhjk","ijkl", 
									"thx" };
	for (int i = 0; i < 28; i++) {
		if (strstr(low_pwd, blacklisted[i]) != NULL) score--;
	}
	if (!score) return 2; //If there is no consecutive characters, it gives 2 score.
	return score * 2; //Gives -2 points for every match.
}
void arrange_string(char *des_string, char *src_string) { //Changes string's order to "number - upper-case letter - lower-case letter" order.
	strcpy_s(des_string, strlen(src_string) + 1, src_string);
	int str_length = strlen(des_string);
	char c;
	int j = 0;
	for (int i = 0; i < str_length; i++) {
		if (*(des_string + i) >= 48 && *(des_string + i) <= 57) { //Moves number character to left.
			for (int k = i; k > j; k--) {
				c = *(des_string + k);
				*(des_string + k) = *(des_string + k - 1);
				*(des_string + k - 1) = c;
			}
			j++;
		}
	}
	for (int i = j; i < str_length; i++) {
		if (*(des_string + i) >= 65 && *(des_string + i) <= 90) { //Moves upper-case letter character to left.
			for (int k = i; k > j; k--) {
				c = *(des_string + k);
				*(des_string + k) = *(des_string + k - 1);
				*(des_string + k - 1) = c;
			}
			j++;
		}
	}
	for (int i = j; i < str_length; i++) {
		if (*(des_string + i) >= 97 && *(des_string + i) <= 122) { //Moves lower-case letter character to left.
			for (int k = i; k > j; k--) {
				c = *(des_string + k);
				*(des_string + k) = *(des_string + k - 1);
				*(des_string + k - 1) = c;
			}
			j++;
		}
	}
}
int score_rearranged_password(char *password, char **dictionary, int size) { //Scores rearranged password by dececting hiding words, bad combinations and blacklisted passwords in different characters.
	int score = 0;
	if (score_transitions(password) > 0) {
		char arrenged_pwd[50];
		arrange_string(arrenged_pwd, password);
		score += score_consecutiveness_status(arrenged_pwd);
		score += score_availability_in_dictionary(arrenged_pwd, dictionary, size);
		score += score_blacklisted_status(arrenged_pwd);
	}
	return score;
}
int score_password(char *password, char **dictionary, int dictionary_line_count) { //Determines  total score of password.
	int total_score = 0;
	total_score += score_consecutiveness_status(password);
	total_score += score_password_length(password);
	total_score += score_transitions(password);
	total_score += score_different_type_count(password);
	total_score += score_consecutive_iteration_status(password);
	total_score += score_all_types_availability(password);
	total_score += score_availability_in_dictionary(password, dictionary, dictionary_line_count);
	total_score += score_iteration_status(password);
	total_score += score_blacklisted_status(password);
	total_score += score_rearranged_password(password, dictionary, dictionary_line_count);
	return total_score;
}
void score_linked_list(struct passwords **head, char **dictionary, int dictionary_lines_count) { //Adds password scores into linked list (Removed from program because it has a loop process which increases time in linked list.).
	struct passwords *ptr = *head;
	while (ptr != NULL) {
		ptr->score = score_password(ptr->password, dictionary, dictionary_lines_count);
		ptr = ptr->next;
	}
}
void insert_into_linked_list_from_text(struct passwords **head, struct passwords **last_node, char *string, char **dictionary, int dictionary_lines_count) { //Inserts data into the end of the linked list.
	struct passwords *node = (struct passwords *)malloc(sizeof(struct passwords));
	if (node == NULL) {
		printf("ERROR!");
	}
	else {
		strcpy_s(node->password, strlen(string) + 1, string);
		node->score = score_password(node->password, dictionary, dictionary_lines_count);
		if (*head == NULL) {
			node->prev = NULL;
			node->next = NULL;
			*head = node;
			*last_node = node;
		}
		else {
			struct passwords *temp = *last_node;
			/* Removed for working big files like 1 million passwords list because using loops for moving in linked list's nodes increases time.
			while (temp->next != NULL) {
				temp = temp->next;
			}
			*/
			temp->next = node;
			node->prev = temp;
			node->next = NULL;
			*last_node = node;
		}
	}
}
void free_linked_list(struct passwords **head) { //Deallocates linked list.
	while (*head != NULL) {
		struct passwords *ptr = *head;
		ptr = *head;
		*head = (*head)->next;
		free(ptr);
	}
}
void display_linked_list(struct passwords **head, int limit) { //Displays linked list data for testing.
	struct passwords *ptr = *head;
	int counter = limit;
	while (ptr != NULL) {
		printf("%s\n", ptr->password);
		ptr = ptr->next;
		counter--;
		if (!counter) break;
	}
}
void read_n_insert_into_linked_list_from_text(struct passwords **head, struct passwords ** last_node, char *directory, int string_length, char **dictionary, int dictionary_lines_count, int pwd_list_line_count) { //Reads data.txt and inserts it's datas to the struct array.
	FILE *fp;
	fopen_s(&fp, directory, "r");
	if (fp == NULL) perror("ERROR");
	else {
		char *line = (char *)malloc(sizeof(char)*(string_length + 2)); //Implements dynamic allocation to a sufficient size.
		char *token = NULL;
		char *context = NULL;
		while (fgets(line, (string_length + 2), fp) != NULL) {
			token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
			insert_into_linked_list_from_text(head, last_node, token, dictionary, dictionary_lines_count); //Inserts elements into the linked list.
		}
		fclose(fp);
		free(line);
	}
}
void swap_pointers(struct passwords **ptr_1, struct passwords **ptr_2) { //Swaps two pointers.
	struct passwords *temp = *ptr_1;
	*ptr_1 = *ptr_2;
	*ptr_2 = temp;
}

void swap_nodes(struct passwords **head, struct passwords **right, struct passwords **node_1, struct passwords **node_2) { //Swaps two nodes without changing nodes' data.
	if (*node_1 == *node_2) {
		return;
	}
	else if ((*node_1)->next != *node_2) { //The situation of non-consecutive nodes.
		struct passwords *ptr_node_1_next = (*node_1)->next;
		struct passwords *ptr_node_2_prev = (*node_2)->prev;
		if ((*node_1)->prev != NULL && (*node_2)->next != NULL) { //Swaps two nodes which are both not first and last node.
			(*node_1)->next = (*node_2)->next;
			(*node_1)->prev->next = *node_2;
			(*node_2)->prev = (*node_1)->prev;
			(*node_2)->next->prev = *node_1;
			(*node_1)->prev = ptr_node_2_prev;
			ptr_node_2_prev->next = *node_1;
			(*node_2)->next = ptr_node_1_next;
			ptr_node_1_next->prev = *node_2;
		}
		else if ((*node_1)->prev == NULL && (*node_2)->next != NULL) { //Swaps two nodes which one is first node.
			(*node_1)->next = (*node_2)->next;
			//(*node_1)->prev->next = *node_2; //A difference from first situation's processes.
			(*node_2)->prev = (*node_1)->prev;
			(*node_2)->next->prev = *node_1;
			(*node_1)->prev = ptr_node_2_prev;
			ptr_node_2_prev->next = *node_1;
			(*node_2)->next = ptr_node_1_next;
			ptr_node_1_next->prev = *node_2;
		}
		else if ((*node_1)->prev != NULL && (*node_2)->next == NULL) { //Swaps two nodes which one is last node.
			(*node_1)->next = (*node_2)->next;
			(*node_1)->prev->next = *node_2;
			(*node_2)->prev = (*node_1)->prev;
			//(*node_2)->next->prev = *node_1; //A difference from first situation's processes.
			(*node_1)->prev = ptr_node_2_prev;
			ptr_node_2_prev->next = *node_1;
			(*node_2)->next = ptr_node_1_next;
			ptr_node_1_next->prev = *node_2;
		}
		else if ((*node_1)->prev == NULL && (*node_2)->next == NULL) { //Swaps two nodes which one is first node and other one is last node.
			(*node_1)->next = (*node_2)->next;
			//(*node_1)->prev->next = *node_2; //A difference from first situation's processes.
			(*node_2)->prev = (*node_1)->prev;
			//(*node_2)->next->prev = node_1; //A difference from first situation's processes.
			(*node_1)->prev = ptr_node_2_prev;
			ptr_node_2_prev->next = *node_1;
			(*node_2)->next = ptr_node_1_next;
			ptr_node_1_next->prev = *node_2;
		}
	}
	else if ((*node_1)->next == *node_2) { //The situation of consecutive nodes.
		if ((*node_1)->prev != NULL && (*node_2)->next != NULL) { //Swaps two nodes which are both not first and last node.
			(*node_1)->next = (*node_2)->next;
			(*node_1)->prev->next = *node_2;
			(*node_2)->prev = (*node_1)->prev;
			(*node_2)->next->prev = *node_1;
			(*node_1)->prev = *node_2;
			(*node_2)->next = *node_1;
		}
		else if ((*node_1)->prev == NULL && (*node_2)->next != NULL) { //Swaps two nodes which one is first node.
			(*node_1)->next = (*node_2)->next;
			//(*node_1)->prev->next = *node_2; //A difference from first situation's processes.
			(*node_2)->prev = (*node_1)->prev;
			(*node_2)->next->prev = *node_1;
			(*node_1)->prev = *node_2;
			(*node_2)->next = *node_1;
		}
		else if ((*node_1)->prev != NULL && (*node_2)->next == NULL) { //Swaps two nodes which one is last node.
			(*node_1)->next = (*node_2)->next;
			(*node_1)->prev->next = *node_2;
			(*node_2)->prev = (*node_1)->prev;
			//(*node_2)->next->prev = *node_1; //A difference from first situation's processes.
			(*node_1)->prev = *node_2;
			(*node_2)->next = *node_1;
		}
		else if ((*node_1)->prev == NULL && (*node_2)->next == NULL) { //Swaps two nodes which one is first node and other one is last node (The situation of that linked list has only 2 nodes.).
			(*node_1)->next = (*node_2)->next;
			//(*node_1)->prev->next = *node_2; //A difference from first situation's processes.
			(*node_2)->prev = (*node_1)->prev;
			//(*node_2)->next->prev = *node_1; //A difference from first situation's processes.
			(*node_1)->prev = *node_2;
			(*node_2)->next = *node_1;
		}
	}
	if (*head == *node_1) *head = *node_2; //Protects the head pointer's target before the swapping.
	if (*right == *node_2)*right = *node_1; //Protects the right pointer's target before the swapping.
	swap_pointers(node_1, node_2);
}
void swap_nodes_data(struct passwords **node_1, struct passwords **node_2) { //Swaps nodes' data without changing nodes' next-prev statuses.
	int temp_int;
	temp_int = (*node_1)->score;
	(*node_1)->score = (*node_2)->score;
	(*node_2)->score = temp_int;
	char temp_str[50];
	strcpy_s(temp_str, strlen((*node_1)->password) + 1, (*node_1)->password);
	strcpy_s((*node_1)->password, strlen((*node_2)->password) + 1, (*node_2)->password);
	strcpy_s((*node_2)->password, strlen(temp_str) + 1, temp_str);
}
struct passwords *partition_in_linked_list(struct passwords **head, struct passwords **left, struct passwords **right) { //Divides the linked list for implementing quick sort.
	struct passwords *pivot_inner = *right;
	struct passwords *i = (*left)->prev;
	for (struct passwords *j = *left; j != *right; j = j->next) {
		if (j->score > pivot_inner->score) { //Moves low scored node to the left side of linked list.
			if (i == NULL) i = *left;
			else i = i->next;
			swap_nodes(head, right, &i, &j);
			//swap_nodes_data(&i, &j); //Removed because of my preference.
		}
	}
	if (i == NULL) i = *left;
	else i = i->next;
	swap_nodes(head, right, &i, &pivot_inner);
	//swap_nodes_data(&i, &pivot_inner); //Removed because of my preference.
	return i;
}
void quick_sort_in_linked_list(struct passwords **head, struct passwords **left, struct passwords **right) { //Quick sort function for sorting 2D char array in ascending order.
	if (*left != *right && *right != NULL && *left != (*right)->next) {
		struct passwords *pivot = partition_in_linked_list(head, left, right);
		quick_sort_in_linked_list(head, left, &(pivot->prev));
		quick_sort_in_linked_list(head, &(pivot->next), right);
	}
}
void linked_list_objective_output(struct passwords **head, struct passwords **last_node, int limit, int node_count, int pwd_list_line_count) { //Displays list objective's report output.
	char *gap = " ";
	struct passwords *ptr_1 = *head;
	struct passwords *ptr_2 = *last_node;
	char str_line_count[10];
	_itoa_s(pwd_list_line_count, str_line_count, 10, 10);
	int str_length = strlen(str_line_count);
	char *str = malloc(sizeof(char) * (str_length + 1));
	int max_rak_asc_length, max_rak_des_length, max_pwd_asc_length, max_pwd_des_length, max_scr_asc_length, max_scr_des_length;
	max_rak_asc_length = max_rak_des_length = max_pwd_asc_length = max_pwd_des_length = max_scr_asc_length = max_scr_des_length = 0;
	int rak_asc_length, rak_des_length, pwd_asc_length, pwd_des_length, scr_asc_length, scr_des_length;
	/* Removed for working big files like 1 million passwords list because using loops for moving in linked list's nodes increases time.
	while (ptr_2->next != NULL) {
		node_count++;
		ptr_2 = ptr_2->next;
	}
	*/
	struct passwords *temp_ptr = ptr_1;
	for (int rank_asc = 1; rank_asc < limit + 1; rank_asc++, temp_ptr = temp_ptr->next) {
		_itoa_s(rank_asc, str, str_length + 1, 10);
		rak_asc_length = strlen(str);
		pwd_asc_length = strlen(temp_ptr->password);
		_itoa_s(temp_ptr->score, str, str_length + 1, 10);
		scr_asc_length = strlen(str);
		if (rak_asc_length > max_rak_asc_length) max_rak_asc_length = rak_asc_length;
		if (pwd_asc_length > max_pwd_asc_length) max_pwd_asc_length = pwd_asc_length;
		if (scr_asc_length > max_scr_asc_length) max_scr_asc_length = scr_asc_length;
	}
	temp_ptr = ptr_2;
	for (int rank_des = node_count; rank_des > node_count - limit; rank_des--, temp_ptr = temp_ptr->prev) {
		pwd_des_length = strlen(temp_ptr->password);
		_itoa_s(temp_ptr->score, str, str_length + 1, 10);
		scr_des_length = strlen(str);
		_itoa_s(rank_des, str, str_length + 1, 10);
		rak_des_length = strlen(str);
		if (rak_des_length > max_rak_des_length) max_rak_des_length = rak_des_length;
		if (pwd_des_length > max_pwd_des_length) max_pwd_des_length = pwd_des_length;
		if (scr_des_length > max_scr_des_length) max_scr_des_length = scr_des_length;
	}
	for (int rank = 1; rank < limit + 1; rank++, ptr_1 = ptr_1->next, ptr_2 = ptr_2->prev) {
		for (int option = 0; option < 6; option++) {
			switch (option % 6) {
			case 0: //Ranks in ascending order.
				printf("%d.", rank);
				_itoa_s(rank, str, str_length + 1, 10);
				rak_asc_length = strlen(str);
				for (int i = 0; i < max_rak_asc_length - rak_asc_length + 1; i++) printf("%s", gap);
				break;
			case 1: //Strongest passwords.
				printf("%s", ptr_1->password);
				pwd_asc_length = strlen(ptr_1->password);
				for (int i = 0; i < max_pwd_asc_length - pwd_asc_length + 2; i++) printf("%s", gap);
				break;
			case 2: //Strongest passwords' scores.
				printf("%d", ptr_1->score);
				_itoa_s(ptr_1->score, str, str_length + 1, 10);
				scr_asc_length = strlen(str);
				for (int i = 0; i < max_scr_asc_length - scr_asc_length + 2; i++) printf("%s", gap);
				break;
			case 3: //Ranks in descending order.
				_itoa_s(node_count - rank + 1, str, str_length + 1, 10);
				rak_des_length = strlen(str);
				for (int i = 0; i < max_rak_des_length - rak_des_length + 0; i++) printf("%s", gap);
				printf("%d.", node_count - rank + 1);
				printf("%s", gap);
				break;
			case 4: //Weakest passwords.
				printf("%s", ptr_2->password);
				pwd_des_length = strlen(ptr_2->password);
				for (int i = 0; i < max_pwd_des_length - pwd_des_length + 2; i++) printf("%s", gap);
				break;
			case 5: //Weakest passwords' scores.
				_itoa_s(ptr_2->score, str, str_length + 1, 10);
				scr_des_length = strlen(str);
				for (int i = 0; i < max_scr_des_length - scr_des_length + 0; i++) printf("%s", gap);
				printf("%d", ptr_2->score);
				printf("\n");
				break;
			}
		}
	}
	free(str);
}
int merge_2d_arrays(char **dest_array, char **src_1_array, char **src_2_array, int src_1_line_count, int src_2_line_count) { //Merges two 2D char array with discarding duplicate elements.
	for (int i = 0; i < src_1_line_count; i++) strcpy_s(dest_array[i], strlen(src_1_array[i]) + 1, src_1_array[i]); //Inserts "src_1_array"s words firstly.
	int next_lines = src_1_line_count;
	int empty_lines = 0;
	int found;
	for (int i = 0; i < src_2_line_count; i++) {
		found = 0;
		for (int j = 0; j < src_1_line_count; j++) if (strcmp(src_2_array[i], src_1_array[j]) == 0) found = 1; //Detects the duplicate words.
		(found) ? empty_lines++ : strcpy_s(dest_array[next_lines++], strlen(src_2_array[i]) + 1, src_2_array[i]); //If the word is not a duplicate word, it inserts it into "dest_array".
	}
	for (int i = src_1_line_count + src_2_line_count -1; i > src_1_line_count + src_2_line_count - empty_lines - 1; i--)free(dest_array[i]); //Discards empty lines at the end of the array.
	int new_dest_array_lines = src_1_line_count + src_2_line_count - empty_lines;
	dest_array = (char **)realloc(dest_array, sizeof(char *) *new_dest_array_lines); //Reallocates the dictionary array's lines count.
	int max_line_length = 0;
	int line_length;
	for (int i = 0; i < new_dest_array_lines; i++) {
		line_length = strlen(dest_array[i]);
		if (line_length > max_line_length) max_line_length = line_length; //Determines max length of the new dictionary array's rows.
	}
	for (int i = 0; i < new_dest_array_lines; i++) dest_array[i] = (char *)realloc(dest_array[i], sizeof(char) * (max_line_length + 1)); //Reallocates the dictionary array's line length.
	return new_dest_array_lines;
}
struct pwd_list {
	char password[50];
	int score;
};
void insert_from_struct_array(struct passwords **head, struct passwords **last_node, char *string, int score) { //Inserts data into the end of the linked list.
	struct passwords *node = (struct passwords *)malloc(sizeof(struct passwords));
	if (node == NULL) {
		printf("ERROR!");
	}
	else {
		strcpy_s(node->password, strlen(string) + 1, string);
		node->score = score;
		if (*head == NULL) {
			node->prev = NULL;
			node->next = NULL;
			*head = node;
			*last_node = node;
		}
		else {
			struct passwords *temp = *last_node;
			/* Removed for working big files like 1 million passwords list because using loops for moving in linked list's nodes increases time.
			while (temp->next != NULL) {
				temp = temp->next;
			}
			*/
			temp->next = node;
			node->prev = temp;
			node->next = NULL;
			*last_node = node;
		}
	}
}
void insert_into_linked_list_from_struct_array(struct passwords **head, struct passwords **last_node, struct pwd_list **pwds, int pwd_list_line_count) { //Inserts sorted struct array elements into linked list.
	int k = 0;
	int percent = pwd_list_line_count / 100;
	for (int index = 0; index < pwd_list_line_count; index++) {
		insert_from_struct_array(head, last_node, (*pwds)[index].password, (*pwds)[index].score);
		if (index == percent * k) printf("\rInserting data into linked list from struct array. %d%%", k++);
		if (index == pwd_list_line_count - 1) printf("\rInserting data into linked list from struct array. %%100");
	}
	printf(" COMPLETED\n");
}
void swap_struct_array_data(struct pwd_list **pwds, int index_1, int index_2, int pwd_list_max_length) { //Swaps struct array data.
	char *temp_password = (char *)malloc(sizeof(char)*(pwd_list_max_length + 1));
	strcpy_s(temp_password, strlen((*pwds)[index_1].password) + 1, (*pwds)[index_1].password);
	strcpy_s((*pwds)[index_1].password, strlen((*pwds)[index_2].password) + 1, (*pwds)[index_2].password);
	strcpy_s((*pwds)[index_2].password, strlen(temp_password) + 1, temp_password); 
	free(temp_password);
	int temp_score=(*pwds)[index_1].score;
	(*pwds)[index_1].score=(*pwds)[index_2].score;
	(*pwds)[index_2].score=temp_score;
}
int partition_in_struct_array_iterative(struct pwd_list **pwds, int low, int high, int pwd_list_max_length){ //Divides struct array for Quick Sort.
	int pivot_value = (*pwds)[high].score;
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++) {
		if ((*pwds)[j].score > pivot_value) {
			i++;
			swap_struct_array_data(pwds, i, j, pwd_list_max_length);
		}
	}
	swap_struct_array_data(pwds, i + 1, high, pwd_list_max_length);
	return (i + 1);
}
void quick_sort_in_struct_array_iterative(struct pwd_list **pwds, int low, int high, int pwd_list_max_length){ //Implements Quick Sort by using iteration for struct array.
	printf("Implementing Quick Sort in struct array...");
	int *stack = malloc(sizeof(int)*(high - low + 1));
	int top = -1;
	stack[++top] = low;
	stack[++top] = high;
	while (top >= 0) {
		high = stack[top--];
		low = stack[top--];
		int pivot = partition_in_struct_array_iterative(pwds, low, high, pwd_list_max_length);
		if (pivot - 1 > low) {
			stack[++top] = low;
			stack[++top] = pivot - 1;
		}
		if (pivot + 1 < high) {
			stack[++top] = pivot + 1;
			stack[++top] = high;
		}
	}
	free(stack);
	printf(" COMPLETED\n");
}
void read_n_insert_into_struct_array_from_text(struct pwd_list **pwds, char *directory,int string_length, char **dictionary, int dictionary_lines_count, int pwd_list_line_count) { //Inserts data into struct array from text.
	FILE *fp;
	fopen_s(&fp, directory, "r");
	if (fp == NULL) perror("ERROR");
	else {
		char *line = (char *)malloc(sizeof(char)*(string_length + 2)); //Implements dynamic allocation in a sufficient size which is the result of "string_length + 1('\n') + 1('\0')".
		char *token = NULL;
		char *context = NULL;
		int index = 0;
		int k = 0;
		int percent = pwd_list_line_count / 100;
		while (fgets(line, (string_length + 2), fp) != NULL) {
			token = strtok_s(line, "\n", &context); //Discards new line character from each line of the text file.
			strcpy_s((*pwds)[index].password, strlen(token) + 1, token);
			(*pwds)[index].score= score_password(token, dictionary, dictionary_lines_count);
			if (index == percent * k) printf("\rScoring and inserting data into struct array from text. %d%%", k++);
			if (index == pwd_list_line_count - 1) printf("\rScoring and inserting data into struct array from text. %%100");
			index++;
		}
		printf(" COMPLETED\n");
		free(line);
		fclose(fp);
	}
}
void linked_list_objective(char *directory, int m_pwds_mode) {
	clock_t begin, end;
	(!m_pwds_mode) ? printf("\n----- Password Scores in Linked List -----\n") : printf("\n----- Password Scores in Linked List for 1000000.txt -----\n");
	begin = clock();
	struct passwords *head = NULL;
	struct passwords *last_node = head;
	int linuxwords_line_count = count_lines("dictionary\\linuxwords.txt"); //The words text's source is "https://users.cs.duke.edu/~ola/ap/linuxwords", last visited at 17.01.2021
	int linuxwords_max_length = find_max_length("dictionary\\linuxwords.txt", linuxwords_line_count);
	char **linuxwords_array = (char **)malloc(sizeof(char *) * linuxwords_line_count);
	for (int i = 0; i < linuxwords_line_count; i++) linuxwords_array[i] = (char *)malloc(sizeof(char) * (linuxwords_max_length + 1)); //Dynamic allocation of "linuxwords_array[45402][28]" for "linuxwords.txt".
	read_n_insert_into_array(linuxwords_array, "dictionary\\linuxwords.txt", linuxwords_max_length);
	turn_to_lower_case_2d_array(linuxwords_array, linuxwords_line_count);
	int nsfw_words_line_count = count_lines("dictionary\\nsfw_words.txt"); //The words text's source is "https://github.com/LDNOOBW/List-of-Dirty-Naughty-Obscene-and-Otherwise-Bad-Words/blob/master/en", last visited at 19.01.2021
	int nsfw_words_max_length = find_max_length("dictionary\\nsfw_words.txt", nsfw_words_line_count);
	char **nsfw_words_array = (char **)malloc(sizeof(char *) *nsfw_words_line_count);
	for (int i = 0; i < nsfw_words_line_count; i++) nsfw_words_array[i] = (char *)malloc(sizeof(char) * (nsfw_words_max_length + 1)); //Dynamic allocation of "nsfw_words_array[401][22]" for "nsfw_words.txt".
	read_n_insert_into_array(nsfw_words_array, "dictionary\\nsfw_words.txt", nsfw_words_max_length);
	turn_to_lower_case_2d_array(nsfw_words_array, nsfw_words_line_count);
	int dictionary_array_line_count = linuxwords_line_count + nsfw_words_line_count;
	int dictionary_array_max_length = (linuxwords_max_length > nsfw_words_max_length) ? linuxwords_max_length : nsfw_words_max_length;
	char **dictionary_array = (char **)malloc(sizeof(char *) *dictionary_array_line_count);
	for (int i = 0; i < dictionary_array_line_count; i++) dictionary_array[i] = (char *)malloc(sizeof(char) * (dictionary_array_max_length + 1)); //Dynamic allocation of "dictionary_array[45803][28]".
	int new_dict_array_lines=merge_2d_arrays(dictionary_array, linuxwords_array, nsfw_words_array, linuxwords_line_count, nsfw_words_line_count); //Loads dictionary by merging 2 words list.
	quick_sort_in_array_ascending(dictionary_array, 0, new_dict_array_lines - 1);
	int pwd_list_line_count = count_lines(directory); //Line count is equal to node number which is 1000 for "1000.txt".
	int pwd_list_max_length = find_max_length(directory, pwd_list_line_count);
	if (!m_pwds_mode) {
		read_n_insert_into_linked_list_from_text(&head, &last_node, directory, pwd_list_max_length, dictionary_array, new_dict_array_lines, pwd_list_line_count); //Inserts passwords into linked list from text file.
		quick_sort_in_linked_list(&head, &head, &last_node);
		end = clock();
		linked_list_objective_output(&head, &last_node, 10, pwd_list_line_count, pwd_list_line_count);
	}
	else {
		struct pwd_list *pwds = malloc(sizeof(struct pwd_list)*(pwd_list_line_count)); //Dynamic allocation of "pwd_list[999998]" for 1 M passwords.
		read_n_insert_into_struct_array_from_text(&pwds, directory, pwd_list_max_length, dictionary_array, new_dict_array_lines, pwd_list_line_count);
		quick_sort_in_struct_array_iterative(&pwds, 0, pwd_list_line_count - 1, pwd_list_max_length);
		insert_into_linked_list_from_struct_array(&head, &last_node, &pwds, pwd_list_line_count);
		end = clock();
		linked_list_objective_output(&head, &last_node, 10, pwd_list_line_count, pwd_list_line_count);
		free(pwds);
	}
	free_array_2D_char(linuxwords_array, linuxwords_line_count);
	free_array_2D_char(nsfw_words_array, nsfw_words_line_count);
	free_array_2D_char(dictionary_array, new_dict_array_lines);
	free_linked_list(&head);
	printf("Time: %f sec.\n", (double)(end - begin) / CLOCKS_PER_SEC);
	printf("Press any button to return menu.");
	_getch();
}
void score_string(char *password, char **dictionary, int dictionary_line_count) { //Displays a string's score for testing program.
	printf("Total score: %d", score_password(password, dictionary, dictionary_line_count));
	printf("\nPassword Length Score: %d", score_password_length(password));
	printf("\nConsecutiveness Status Score: %d", score_consecutiveness_status(password));
	printf("\nConsecutive Iteration Status Score: %d", score_consecutive_iteration_status(password));
	printf("\nIteration Status Score: %d", score_iteration_status(password));
	printf("\nTransitions Score: %d", score_transitions(password));
	printf("\nDifferent Type Count Score: %d", score_different_type_count(password));
	printf("\nAll Types Availability Score: %d", score_all_types_availability(password));
	printf("\nAvailability In Dictionary Score: %d", score_availability_in_dictionary(password, dictionary, dictionary_line_count));
	printf("\nBlacklisted Status Score: %d", score_blacklisted_status(password));
	printf("\nRearranged Password Score: %d", score_rearranged_password(password, dictionary, dictionary_line_count));
	printf("\n");
}
void score_given_password() {
	printf("\n------------- Score Password -------------\n");
	int linuxwords_line_count = count_lines("dictionary\\linuxwords.txt"); //The words text's source is "https://users.cs.duke.edu/~ola/ap/linuxwords", last visited at 17.01.2021
	int linuxwords_max_length = find_max_length("dictionary\\linuxwords.txt", linuxwords_line_count);
	char **linuxwords_array = (char **)malloc(sizeof(char *) * linuxwords_line_count);
	for (int i = 0; i < linuxwords_line_count; i++) linuxwords_array[i] = (char *)malloc(sizeof(char) * (linuxwords_max_length + 1)); //Dynamic allocation of "linuxwords_array[45402][28]" for "linuxwords.txt".
	read_n_insert_into_array(linuxwords_array, "dictionary\\linuxwords.txt", linuxwords_max_length);
	turn_to_lower_case_2d_array(linuxwords_array, linuxwords_line_count);
	int nsfw_words_line_count = count_lines("dictionary\\nsfw_words.txt"); //The words text's source is "https://github.com/LDNOOBW/List-of-Dirty-Naughty-Obscene-and-Otherwise-Bad-Words/blob/master/en", last visited at 19.01.2021
	int nsfw_words_max_length = find_max_length("dictionary\\nsfw_words.txt", nsfw_words_line_count);
	char **nsfw_words_array = (char **)malloc(sizeof(char *) *nsfw_words_line_count);
	for (int i = 0; i < nsfw_words_line_count; i++) nsfw_words_array[i] = (char *)malloc(sizeof(char) * (nsfw_words_max_length + 1)); //Dynamic allocation of "nsfw_words_array[401][22]" for "nsfw_words.txt".
	read_n_insert_into_array(nsfw_words_array, "dictionary\\nsfw_words.txt", nsfw_words_max_length);
	turn_to_lower_case_2d_array(nsfw_words_array, nsfw_words_line_count);
	int dictionary_array_line_count = linuxwords_line_count + nsfw_words_line_count;
	int dictionary_array_max_length = (linuxwords_max_length > nsfw_words_max_length) ? linuxwords_max_length : nsfw_words_max_length;
	char **dictionary_array = (char **)malloc(sizeof(char *) *dictionary_array_line_count);
	for (int i = 0; i < dictionary_array_line_count; i++) dictionary_array[i] = (char *)malloc(sizeof(char) * (dictionary_array_max_length + 1)); //Dynamic allocation of "dictionary_array[45803][28]".
	int new_dict_array_lines = merge_2d_arrays(dictionary_array, linuxwords_array, nsfw_words_array, linuxwords_line_count, nsfw_words_line_count); //Loads dictionary by merging 2 words list.
	quick_sort_in_array_ascending(dictionary_array, 0, new_dict_array_lines - 1);
	do {
		printf("1. Score a password.\n");
		printf("2. Return to menu.\n");
		printf("Type an option: ");
		int option;
		while (scanf_s("%d", &option) != 1) getchar(); //If user enters characters instead of integers, this getchar() discards characters.
		switch (option) {
		case 1:
			printf("Type a password: ");
			char password[100];
			scanf_s("%s", password, sizeof password);
			score_string(password, dictionary_array, new_dict_array_lines);
			break;
		case 2:
			free_array_2D_char(linuxwords_array, linuxwords_line_count);
			free_array_2D_char(nsfw_words_array, nsfw_words_line_count);
			free_array_2D_char(dictionary_array, new_dict_array_lines);
			return;
		default:
			printf("Invalid input.\n");
			break;
		}
	} while (1);
}
int main() {
	char *search_objective_directory = "10-million-password-list-top\\100000.txt";
	char *sort_objective_directory = "10-million-password-list-top\\10000.txt";
	char *hash_objective_directory = "10-million-password-list-top\\1000.txt";
	char *linked_list_directory = "10-million-password-list-top\\1000.txt";
	int m_pwds_mode = 0;
	do {
		(!m_pwds_mode) ? printf("------------------ Menu ------------------\n") :	printf("--------------- Menu - 1 Million Passwords ---------------\n");
		(!m_pwds_mode) ? printf("1. Search Objective\n") :							printf("1. Search Objective in 1000000.txt\n");
		(!m_pwds_mode) ? printf("2. Sort Objective\n") :							printf("2. Sort Objective in 1000000.txt\n");
		(!m_pwds_mode) ? printf("3. Hash Objective\n") :							printf("3. Hash Objective in 1000000.txt\n");
		(!m_pwds_mode) ? printf("4. Linked List Objective\n") :						printf("4. Linked List Objective in 1000000.txt\n");
		(!m_pwds_mode) ? printf("5. Enable 1 Million Passwords Mode\n") :			printf("5. Disable 1 Million Passwords Mode\n");
		printf("6. Score a password.\n");
		printf("7. Clear screen.\n");
		printf("8. Exit.\n");
		printf("Type an option: ");
		int option;
		while (scanf_s("%d", &option) != 1) getchar(); //If user enters characters instead of integers, this getchar() discards characters.
		switch (option) {
		case 1:
			search_objective(search_objective_directory, m_pwds_mode);
			printf("\n");
			break;
		case 2:
			sort_objective(sort_objective_directory, m_pwds_mode);
			printf("\n");
			break;
		case 3:
			hash_objective(hash_objective_directory, m_pwds_mode);
			printf("\n");
			break;
		case 4:
			linked_list_objective(linked_list_directory, m_pwds_mode);
			printf("\n");
			break;
		case 5:
			if (!m_pwds_mode) {
				search_objective_directory = "10-million-password-list-top\\1000000.txt";
				sort_objective_directory = "10-million-password-list-top\\1000000.txt";
				hash_objective_directory = "10-million-password-list-top\\1000000.txt";
				linked_list_directory = "10-million-password-list-top\\1000000.txt";
			m_pwds_mode = 1;
			}
			else {
				search_objective_directory = "10-million-password-list-top\\100000.txt";
				sort_objective_directory = "10-million-password-list-top\\10000.txt";
				hash_objective_directory = "10-million-password-list-top\\1000.txt";
				linked_list_directory = "10-million-password-list-top\\1000.txt";
				m_pwds_mode = 0;
			}
			printf("\n");
			break;
		case 6:
			score_given_password();
			printf("\n");
			break;
		case 7:
			system("cls");
			break;
		case 8:
			return 0;
		default:
			printf("Invalid input.\n");
			break;
		}
	} while (1);
}
