#define BLACK 1
#define RED 0
#include <stdio.h>
#include <stdlib.h>
int bh = 0;
int total = 0;
int tr = 0;

typedef struct Node*NodePtr;
struct Node
{
	int val;
	NodePtr p, left, right;
	int c;
};


NodePtr node_alloc(int newval)
{
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->p = NULL;
	self->left = NULL;
	self->right = NULL;
	self->c = BLACK;
	return self;
}


typedef struct RBT* RBTPtr;
struct RBT
{
	NodePtr root;
};


RBTPtr rbt_alloc()
{
	RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
	self->root = NULL;
	return self;
}


void Left_Rotate(RBTPtr self, NodePtr n)
{
	NodePtr y;
	y = n->right;
	n->right = y->left;
	if (y->left != NULL)
		y->left->p = n;
	y->p = n->p;
	if (n->p == NULL)
		self->root = y;
	else if (n == n->p->left)
		n->p->left = y;
	else
		n->p->right = y;
	y->left = n;
	n->p = y;
}


void Right_Rotate(RBTPtr self, NodePtr n)
{
	NodePtr y;
	y = n->left;
	n->left = y->right;
	if (y->right != NULL)
		y->right->p = n;
	y->p = n->p;
	if (n->p == NULL)
		self->root = y;
	else if (n == n->p->right)
		n->p->right = y;
	else
		n->p->left = y;
	y->right = n;
	n->p = y;
}


void rbt_insert_fixup(RBTPtr self, NodePtr n)
{
	NodePtr y;
	while (n->p != NULL && n->p->c == RED)
	{
		if (n->p == n->p->p->left)
		{
			y = n->p->p->right;
			if (y != NULL && y->c == RED)
			{
				n->p->c = BLACK;
				y->c = BLACK;
				n->p->p->c = RED;
				n = n->p->p;
			}
			else if (n == n->p->right)
			{
				n = n->p;
				Left_Rotate(self, n);
			}
			else
			{
				n->p->c = BLACK;
				n->p->p->c = RED;
				Right_Rotate(self, n->p->p);
			}
		}
		else if (n->p == n->p->p->right)
		{
			y = n->p->p->left;
			if (y != NULL && y->c == RED)
			{
				n->p->c = BLACK;
				y->c = BLACK;
				n->p->p->c = RED;
				n = n->p->p;
			}
			else if (n == n->p->left)
			{
				n = n->p;
				Right_Rotate(self, n);
			}
			else
			{
				n->p->c = BLACK;
				n->p->p->c = RED;
				Left_Rotate(self, n->p->p);
			}
		}
	}
	self->root->c = BLACK;
}


void rbt_insert(RBTPtr self, NodePtr n)
{
	NodePtr y = NULL;
	NodePtr x = self->root;
	while (x != NULL)
	{
		y = x;
		if (n->val < x->val)
			x = x->left;
		else
			x = x->right;
	}
	n->p = y;
	if (y == NULL)
		self->root = n;
	else if (n->val < y->val)
		y->left = n;
	else
		y->right = n;
	n->left = NULL;
	n->right = NULL;
	n->c = RED;
	rbt_insert_fixup(self, n);
}

void rbt_print(RBTPtr self, NodePtr tree, int level)
{
	if (tree->right != NULL)
		rbt_print(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	if (tree->c == BLACK)
	{
		printf("%d\n", tree->val);
		total = total + 1;
	}
	if (tree->c == RED)
	{
		printf("%d\n", tree->val);
		tr = tr + 1;
	}
	if (tree->left != NULL)
		rbt_print(self, tree->left, level + 1);
}


int rbt_bh(RBTPtr self, NodePtr tree, int bh)
{
	if (tree == NULL)
	{
		return bh;
	}
	else
	{
		if (tree->c == BLACK)
			rbt_bh(self, tree->right, bh + 1);
		else if (tree->c == RED)
			rbt_bh(self, tree->right, bh);
	}
}

struct node
{
	int data;
	struct node *next;
};
void push(struct node** head_ref, int new_data)
{
	struct node* new_node = (struct node*) malloc(sizeof(struct node));
	new_node->data = new_data;
	new_node->next = (*head_ref);
	(*head_ref) = new_node;
}

void deleteNode(struct node **head_ref, int key)
{
	struct node* temp = *head_ref, *prev;

	if (temp != NULL && temp->data == key)
	{
		*head_ref = temp->next;
		free(temp);
		return;
	}


	while (temp != NULL && temp->data != key)
	{
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) return;

	prev->next = temp->next;

	free(temp);
}
void inorder_traversal(NodePtr root) {
	if (root != NULL) {
		inorder_traversal(root->left);
		printf("%d\n ", root->val);
		inorder_traversal(root->right);
	}
}


int main()
{
	struct node* cur = NULL;
	struct node* head = NULL;
	RBTPtr rbt = rbt_alloc();
	FILE *fp;
	int num;
	fp = fopen("C:\\Users\\김상현\\Desktop\\새 폴더\\input.txt", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%d", &num);
		if (num >0)
			push(&head, num);
		else if (num < 0)
			deleteNode(&head, -1 * num);
		else if (num == 0)
			break;
	}
	cur = head;
	while (cur != NULL)
	{
		rbt_insert(rbt, node_alloc(cur->data));
		cur = cur->next;
	}
	fclose(fp);
	rbt_print(rbt, rbt->root, 0);
	printf("\n\n\n");
	printf("total = %d\n", total + tr);
	printf("nb = %d\n", total);
	printf("bh = %d\n", rbt_bh(rbt, rbt->root, bh));
	inorder_traversal(rbt->root);

	return 0;
}