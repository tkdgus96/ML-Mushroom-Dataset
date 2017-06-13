#define BLACK 1
#define RED 0
#include <stdio.h>
#include <stdlib.h>
int bh = 0;
int total = 0;
int tr = 0;
int nb=0;
int miss=0;
int delete=0;
int insert=0;

typedef struct Node*NodePtr;
struct Node
{
    int val;
    NodePtr p, left, right;
    int c;
};

NodePtr nil;

NodePtr node_alloc(int newval)
{
    NodePtr self = (NodePtr)malloc(sizeof(struct Node));
    self->val = newval;
    self->p = nil;
    self->left = nil;
    self->right = nil;
    //self->c = BLACK;
    return self;
}

NodePtr nil_init()
{
    nil = (NodePtr)malloc(sizeof(struct Node));
    nil->val = -1;
    nil->c = 1;
    nil->left = nil->right = NULL;
    
    return nil;
    
}
typedef struct RBT* RBTPtr;
struct RBT
{
    NodePtr root, NIL;
};


RBTPtr rbt_alloc()
{
    RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
    self->NIL = nil;
    self->root = self->NIL;
    return self;
}


void Left_Rotate(RBTPtr self, NodePtr n)
{
    NodePtr y;
    y = n->right;
    n->right = y->left;
    if (y->left != self->NIL)
        y->left->p = n;
    y->p = n->p;
    if (n->p == self->NIL)
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
    if (y->right != self->NIL)
        y->right->p = n;
    y->p = n->p;
    if (n->p == self->NIL)
        self->root = y;
    else if (n == n->p->left)
        n->p->left = y;
    else
        n->p->right = y;
    y->right = n;
    n->p = y;
}


void rbt_insert_fixup(RBTPtr self, NodePtr n)
{
    NodePtr y;
    while (n->p->c == RED)
    {
        if (n->p == n->p->p->left)
        {
            y = n->p->p->right;
            if (y!=self->NIL && y->c == RED)
            {
                n->p->c = BLACK;
                y->c = BLACK;
                n->p->p->c = RED;
                n = n->p->p;
            }
            else
            {
                if (n == n->p->right)
                {
                    n = n->p;
                    Left_Rotate(self, n);
                }
                n->p->c = BLACK;
                n->p->p->c = RED;
                Right_Rotate(self, n->p->p);
            }
        }
        else
        {
            y = n->p->p->left;
            if (y->c == RED)
            {
                n->p->c = BLACK;
                y->c = BLACK;
                n->p->p->c = RED;
                n = n->p->p;
            }
            else
            {
                if (n == n->p->left)
                {
                    n = n->p;
                    Right_Rotate(self, n);
                }
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
    NodePtr y = self->NIL;
    NodePtr x = self->root;
    while (x != self->NIL)
    {
        y = x;
        if (n->val < x->val)
            x = x->left;
        else
            x = x->right;
    }
    n->p = y;
    if (y == self->NIL)
        self->root = n;
    else if (n->val < y->val)
        y->left = n;
    else
        y->right = n;
    n->left = self->NIL;
    n->right = self->NIL;
    n->c = RED;
    rbt_insert_fixup(self, n);
}
void rbt_transplant(RBTPtr self, NodePtr u, NodePtr v)
{
    if (u->p == self->NIL)
        self->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}
NodePtr tree_min(RBTPtr self, NodePtr x)
{
    while (x->left != self->NIL)
        x = x->left;
    return x;
}
void rbt_delete_fixup(RBTPtr self, NodePtr n)
{
    NodePtr w;
    while (n != self->root && n->c == BLACK)
    {
        if (n == n->p->left)
        {
            w = n->p->right;
            if (w->c == RED)
            {
                w->c = BLACK;
                n->p->c = RED;
                Left_Rotate(self, n->p);
                w = n->p->right;
            }
            if (w->left->c == BLACK && w->right->c == BLACK)
            {
                w->c = RED;
                n = n->p;
            }
            else
            {
                if (w->right->c == BLACK)
                {
                    w->left->c = BLACK;
                    w->c = RED;
                    Right_Rotate(self, w);
                    w = n->p->right;
                }
                w->c = n->p->c;
                n->p->c = BLACK;
                w->right->c = BLACK;
                Left_Rotate(self, n->p);
                n = self->root;
            }
        }
        else
        {
            w = n->p->left;
            if (w->c == RED)
            {
                w->c = BLACK;
                n->p->c = RED;
                Right_Rotate(self, n->p);
                w = n->p->left;
            }
            if (w->right->c == BLACK && w->left->c == BLACK)
            {
                w->c = RED;
                n = n->p;
            }
            else
            {
                if (w->left->c == BLACK)
                {
                    w->right->c = BLACK;
                    w->c = RED;
                    Left_Rotate(self, w);
                    w = n->p->left;
                }
                w->c = n->p->c;
                n->p->c = BLACK;
                w->left->c = BLACK;
                Right_Rotate(self, n->p);
                n = self->root;
            }
        }
        
    }
    n->c = BLACK;
}
void rbt_delete(RBTPtr self, NodePtr n)
{
    NodePtr y, x;
    y = n;
    int y_original_color = y->c;
    if (n->left == self->NIL)
    {
        x = n->right;
        rbt_transplant(self, n, n->right);
    }
    else if (n->right == self->NIL)
    {
        x = n->left;
        rbt_transplant(self, n, n->left);
    }
    else
    {
        y = tree_min(self, n->right);
        y_original_color = y->c;
        x = y->right;
        if (y->p == n)
            x->p = y;
        else
        {
            rbt_transplant(self, y, y->right);
            y->right = n->right;
            y->right->p = y;
        }
        rbt_transplant(self, n, y);
        y->left = n->left;
        y->left->p = y;
        y->c = n->c;
    }
    if (y_original_color == BLACK)
        rbt_delete_fixup(self, x);
}
void rbt_print(RBTPtr self, NodePtr tree, int level)
{
    if (tree->right != self->NIL)
        rbt_print(self, tree->right, level + 1);
    for (int i = 0; i < level; i++)
        printf("   ");
    printf("%d-%s\n", tree->val, !(tree->c) ? "R" : "B");
    if (tree->left != self->NIL)
        rbt_print(self, tree->left, level + 1);
}
int rbt_count(RBTPtr self, NodePtr tree)
{
    if (tree==self->NIL)
        return 0;
    if (tree->left != self->NIL)
        return rbt_count(self, tree->left)+rbt_count(self, tree->right)+1;
    else
        return rbt_count(self, tree->left)+rbt_count(self, tree->left);
}


void rbt_bh(RBTPtr self, NodePtr tree)
{
    if (tree == self->NIL)
    {
        return;
    }
    else
    {
        if (tree->c == BLACK) {
            bh++;
            rbt_bh(self, tree->right);
        }
        else if (tree->c == RED)
            rbt_bh(self, tree->right);
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
/*
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
 */
NodePtr rbt_search(RBTPtr self, NodePtr x, int val)
{
    if (x == self->NIL || x->val == val)
        return x;
    else if (val <= x->val)
        return rbt_search(self,x->left, val);
    else
        return rbt_search(self,x->right, val);
}
void inorder_traversal(RBTPtr self, NodePtr root) {
    if (root != self->NIL) {
        inorder_traversal(self, root->left);
        printf("%d %s\n", root->val, !(root->c) ? "R" : "B");
        inorder_traversal(self, root->right);
    }
}
void rbt_counter(RBTPtr self, NodePtr root) {
    if (root != self->NIL) {
        rbt_counter(self, root->left);
        total++;
        rbt_counter(self, root->right);
    }
}



int main()
{
    nil = nil_init();
    RBTPtr rbt = rbt_alloc();
    FILE *fp;
    int num;
    fp = fopen("/Users/SK/desktop/test01.txt", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%d", &num);
        if (num > 0)
        {
            rbt_insert(rbt, node_alloc(num));
            insert++;
            //rbt_print(rbt,rbt->root,0);
        }
        else if (num < 0)
        {
            NodePtr node1 = rbt_search(rbt, rbt->root, num*-1);
            if (node1->val!=-1)
            {
                rbt_delete(rbt, node1);
                delete++;
            }
            else
                miss++;
        }
        else if (num == 0)
            break;
    }
    fclose(fp);
    //rbt_print(rbt, rbt->root, 0);
    rbt_bh(rbt, rbt->root);
    rbt_counter(rbt, rbt->root);
    printf("total = %d\n", total);
    printf("insert= %d\n", insert);
    printf("nb= %d\n", rbt_count(rbt, rbt->root));
    printf("delete= %d\n", delete);
    printf("miss= %d\n", miss);
    printf("bh = %d\n", bh);
    inorder_traversal(rbt, rbt->root);
    return 0;
}
