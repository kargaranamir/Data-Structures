#include<iostream>
#include<string>
#include <stack>
#include <fstream>

using namespace std;

struct book {
	string book_name;
	string author;
	int pub_year;
	int price;
};

book init_book(string book_name, string author, int pub_year, int price)
{
	book one_book;
	one_book.book_name = book_name;
	one_book.author = author;
	one_book.pub_year = pub_year;
	one_book.price = price;

	return one_book;
}

void print_book(book temp)
{
	cout << temp.book_name << "\t\t" << temp.author << "\t\t" << temp.pub_year << "\t:\t" << temp.price << endl;
}

bool first_is_smaller_than_second(book first, book second)
{
	if (first.pub_year < second.pub_year)
		return 1;
	else if (first.pub_year > second.pub_year)
		return 0;
	else
		if (first.book_name < second.book_name)
			return 1;
		else
			return 0;
}

struct book_node {
	book data;
	book_node *link;
};

struct book_Queue
{
	book_node *front = NULL, *rear = NULL;
};

void sortedBookInsert(book_Queue *q, string book_name, string author, int pub_year, int price)
{
	book new_book = init_book(book_name, author, pub_year, price);

	book_node *new_node = new book_node;
	new_node->data = new_book;

	book_node *current = q->front;

	// q is empty 
	if (current == NULL)
	{
		q->front = new_node;
		q->rear = new_node;
		q->rear->link = q->front;
	}

	// if it should be added before head
	else if (first_is_smaller_than_second(new_node->data, current->data))
	{
		q->rear->link = new_node;
		new_node->link = q->front;
		q->front = new_node;
	}

	// it should added somewhere else
	else
	{
		/* Locate the node before the point of insertion */
		while (current->link != q->front && first_is_smaller_than_second(current->link->data, new_node->data))
			current = current->link;

		if (current == q->rear)
			q->rear = new_node;
		new_node->link = current->link;
		current->link = new_node;
	}
}

book_node* search_book(book_Queue* q, string key)
{
	if (q->front == NULL)
		return NULL;
	book_node *curr = q->front, *prev = q->rear;
	while (curr->data.book_name != key)
	{
		if (curr->link == q->front)
		{
			cout << "Given book is not found in the list!!!" << endl;
			return NULL;
		}
		prev = curr;
		curr = curr->link;
	}
	return curr;
}

bool deleteBook(book_Queue *q, string key)
{
	if (q->front == NULL)
		return 0;

	// Find the required node 
	book_node *curr = q->front, *prev = q->rear;
	while (curr->data.book_name != key)
	{
		if (curr->link == q->front)
		{
			cout << "Given book is not found in the list!!!" << endl;
			return 0;
		}
		prev = curr;
		curr = curr->link;
	}

	// Check if node is only node 
	if (q->rear == q->front)
	{
		q->front = NULL;
		q->rear = NULL;
		free(curr);
		return 1;
	}

	// If more than one node, check if 
	// it is first node 
	if (curr == q->front)
	{
		q->front = curr->link;
		q->rear->link = q->front;
	}

	// check if node is last node 
	else if (curr == q->rear)
	{
		prev->link = q->front;
		q->rear = prev;
	}
	else
	{
		prev->link = curr->link;
	}
	free(curr);
	return 1;
}

int countNodes(book_Queue q)
{
	book_node *temp = q.front;
	int result = 0;
	if (q.front != NULL) {
		do {
			temp = temp->link;
			result++;
		} while (temp != q.front);
	}

	return result;
}

void printBookList(book_Queue q)
{
	book_node *temp = q.front;
	if (q.front != NULL)
	{
		do
		{
			print_book(temp->data);
			temp = temp->link;
		} while (temp != q.front);
	}

	cout << endl;
}

bool file_read(book_Queue* list)
{
	ifstream inputFile;

	inputFile.open("book_list.txt", ios::in);
	if (!inputFile)
	{
		return 0;
	}

	string book_name;
	string author;
	int year;
	int price;

	while (!inputFile.eof())
	{
		inputFile >> book_name >> author >> year >> price;
		if (book_name == "")
			return 0;
		sortedBookInsert(list, book_name, author, year, price);
	}
	inputFile.close();
	return 1;
}

bool file_write(book_Queue list)
{
	ofstream outputFile;
	outputFile.open("book_list.txt", ios::trunc);
	if (!outputFile)
	{
		return 0;
	}
	book_node *curr = list.front;
	if (list.front == list.rear)
	{
		if (list.front == NULL)
		{
			outputFile.close();
			return 0;
		}
		else
		{
			outputFile << curr->data.book_name << " " << curr->data.author << " " << curr->data.pub_year << " " << curr->data.price;
			return 1;
		}

	}
	while (curr->link != list.front)
	{
		outputFile << curr->data.book_name << " " << curr->data.author << " " << curr->data.pub_year << " " << curr->data.price << endl;
		curr = curr->link;
	}
	outputFile << curr->data.book_name << " " << curr->data.author << " " << curr->data.pub_year << " " << curr->data.price;
	outputFile.close();
	return 1;

}

//////////////////////////////////////////////////////book finished

struct human {
	int number;
	string name;
	stack <book> s;
};

struct human_node {
	human person;
	human_node *link;
};

struct human_Queue
{
	human_node *front = NULL, *rear = NULL;
};

struct all_people_in_bookstore {
	human_Queue *men = NULL;
	human_Queue *women = NULL;
	bool turn=1;//1 for man , 0 for woman
};

int faktor(human q)
{
	int sum=0;
	while (!q.s.empty())
	{
		cout << '\t' << q.s.top().book_name << ":\t" << q.s.top().price << endl;
		sum += q.s.top().price;
		q.s.pop();
	}
	cout << "sum for "<<q.name<<" = " << sum << endl;
	cout << "--------------------" << endl;
	return sum;
}

void enQueue(human_Queue *q, string name)
{
	//add to end
	human_node *temp = new human_node;
	if (q->front == NULL)
	{
		temp->person.number = 1;
	}
	else
	{
		temp->person.number = q->rear->person.number + 1;
	}
	temp->person.name = name;
	temp->link = NULL;

	if (q->front == NULL)
		q->front = temp;
	else
		q->rear->link = temp;

	q->rear = temp;
}

bool deQueue(human_Queue *q)
{
	//remove from first
	if (q->front == NULL)
	{
		printf("Queue is empty");
		return 0;
	}

	// If this is the last node to be deleted 
	human value; // Value to be dequeued 
	if (q->front == q->rear)
	{
		value = q->front->person;
		free(q->front);
		q->front = NULL;
		q->rear = NULL;
	}
	else  // There are more than one nodes 
	{
		human_node *temp = q->front;
		value = temp->person;
		q->front = q->front->link;
		free(temp);
	}

	return 1;
}

void addHumanToList(all_people_in_bookstore *people, string name, bool gender)
{
	if (gender == 1)
	{
		if (people->men == NULL)
			people->men = new human_Queue;
		enQueue(people->men, name);
	}
	else
	{
		if (people->women == NULL)
			people->women = new human_Queue;
		enQueue(people->women, name);
	}
}

void last_person_checkout(all_people_in_bookstore *all)
{
	bool done;
	int sum;
	if (all->turn == 1)
	{
		sum = faktor(all->men->front->person);
		done = deQueue(all->men);
	}
	else
	{
		sum = faktor(all->women->front->person);
		done = deQueue(all->women);
	}
	all->turn = !all->turn;
}

bool add_book_to_stack_human(all_people_in_bookstore *people, book_Queue* list, bool gender, int number, string key)
{
	book_node* currbook = search_book(list, key);
	if (currbook == NULL)
		return 0;
	human_Queue* q;
	if (gender == 1)
		q = people->men;
	else
		q = people->women;
	human_node *curr = q->front;
	if (q->front == NULL)
		return 0;
	if (q->front->person.number == number)
	{
		q->front->person.s.push(currbook->data);
	}
	curr = curr->link;
	while (curr->person.number != number)
	{
		curr = curr->link;
	}

	curr->person.s.push(currbook->data);
	deleteBook(list, key);
	return 1;
}

int main()
{
	book_Queue all_books;
	if (!file_read(&all_books))
		cerr << "some thing wrong during opening file!" << endl;

	printBookList(all_books);

	all_people_in_bookstore people;
	addHumanToList(&people, "amir", 1);
	addHumanToList(&people, "ali", 1);
	addHumanToList(&people, "mahed", 1);

	addHumanToList(&people, "zahra", 0);
	addHumanToList(&people, "maryam", 0);

	add_book_to_stack_human(&people, &all_books, 1, 2, "a");
	add_book_to_stack_human(&people, &all_books, 1, 2, "g");

	last_person_checkout(&people);
	last_person_checkout(&people);
	last_person_checkout(&people);

	if (file_write(all_books))
		cout << "Data written to file" << endl;
	system("pause");
	return 0;
 }