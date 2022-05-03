#include "rbtree.h"
#include <stdio.h>
#include <malloc.h>


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  p->nil = NIL;
  p->root = NIL;
  NIL->color = RBTREE_BLACK;
  return p;
}

void rb_postorder_delete(node_t *root, rbtree *t)
{
    if (root == t->nil)
    {
        return;
    }
    rb_postorder_delete(root->left, t);
    rb_postorder_delete(root->right, t);
    free(root);
}

void delete_rbtree(rbtree *t)
{
    // TODO: reclaim the tree nodes's memory
    if (t == NULL)
    {
        return;
    }
    rb_postorder_delete(t->root, t);
    free(t->nil);
    free(t);
}


void left_rotate(rbtree *t, node_t *x)
{
    node_t *y;
    y = x->right;
    x->right = y->left;
    if (y->left != t->nil)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil)
    {
        t->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return;
}

void right_rotate(rbtree *t, node_t *x)
{
    node_t *y;
    y = x->left;
    x->left = y->right;
    if (y->right != t->nil)
    {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil)
    {
        t->root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    return;
}

node_t *rb_insert_Fixup(rbtree *t, node_t *z)
{
    node_t *uncle;
    while (z->parent->color == RBTREE_RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            uncle = z->parent->parent->right;
            //경우1
            if (uncle->color == RBTREE_RED)
            {
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            //경우2
            else {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(t, z);
                }
                //경우3
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        }
        //반대로
        else
        {
            uncle = z->parent->parent->left;
            //경우1
            if (uncle->color == RBTREE_RED)
            {
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            //경우2
            else {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(t, z);
                }
                //경우3
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
    return t->root;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

    node_t *parent = t->nil;
    node_t *p = t->root;
    //새 노드가 들어갈 자리 찾기 - p가 nil일 떄 끝 = parent의 자식으로 새 노드 들어감
    while (p != t->nil)
    {
        parent = p;
        if (key < p->key)
        {
            p = p->left;
        }
        else
        {
            p = p->right;
        }
    }
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    new_node->parent = parent;
    // 트리의 첫 노드일 때 - 루트가 된다
    if (parent == t->nil)
    {
        t->root = new_node;
        new_node->color = RBTREE_BLACK;
    }
    //찾은 부모노드보다 새로운 노드가 작을때 - 왼쪽 자식으로
    else if (key < parent->key)
    {
        parent->left = new_node;
        new_node->color = RBTREE_RED;
    }
    //크거나 같을때 - 오른쪽 자식으로
    else
    {
        parent->right = new_node;
        new_node->color = RBTREE_RED;
    }
    //새로운 노드 설정
    new_node->key = key;
    new_node->left = t->nil;
    new_node->right = t->nil;

    rb_insert_Fixup(t, new_node);
    return t->root;
    
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
      node_t *r = t->root;
    while (r != t->nil)
    {
        if (r->key == key)
            return r;
        else if (r->key < key)
            r = r->right;
        else
            r = r->left;
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // 트리 구성에서 left 쪽이 작은것 = left 쪽을 계속 탐색하여 작은값을 찾는다.
  // root -> left 를 기준으로 하여 left 로 내려간다. 더 작은 값이 나올때마다 해당하는 값을 기준으로 갱신.

  node_t *value = t->root;
  if (value == t -> nil)
  {
      return value;
  }
  while (value -> left != t->nil)
  {
      value = value -> left;
  }

  return value;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  // 트리 구성에서 right 쪽이 큰것 = right 쪽을 계속 탐색하여 큰값을 찾는다.
  // root -> right 를 기준으로 하여 right 로 내려간다. 더 큰 값이 나올때마다 해당하는 값을 기준으로 갱신.
  // 코드 자체는 min 에서 반대로

  node_t *value = t->root;
  if (value == t -> nil)
  {
      return value;
  }
  while (value -> right != t->nil)
  {
      value = value -> right;
  }

  return value;
}

// int rbtree_erase(rbtree *t, node_t *p) {
//   // TODO: implement erase
//   return 0;
// }

// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
//   // TODO: implement to_array
//   return 0;
// }