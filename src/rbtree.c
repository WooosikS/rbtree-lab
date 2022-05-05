#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  p -> nil = nil;
  p -> nil -> color = RBTREE_BLACK;
  p -> root = nil;
  return p;
}

// 전체 노드 삭제 (후위순위로)
void delete_postorder(rbtree *t, node_t *node) {
  if (node == t -> nil)
    return;
  delete_postorder(t, node -> left);
  delete_postorder(t, node -> right);
  node -> left = node -> right = node -> parent = NULL;
  free(node);
}


void delete_rbtree(rbtree *t) {
  delete_postorder(t, t -> root);
  free(t -> nil);
  free(t);
}


void left_rotate(rbtree *t, node_t * x){
  node_t *y = x  -> right;
  x  -> right = y -> left;

  if (y -> left != t -> nil){
    y -> left -> parent = x;
  }
  
  y -> parent = x -> parent;

  if (x -> parent == t -> nil){
    t -> root = y;
  }
  else if (x == x -> parent -> left){
    x -> parent -> left = y;
  }
  else{
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x  -> left;
  x  -> left = y -> right;

  if (y -> right != t -> nil){
    y -> right -> parent = x;
  }
  y -> parent = x -> parent;

  if (x -> parent == t -> nil){
    t -> root = y;
  }
  else if (x == x -> parent -> right){
    x -> parent -> right = y;
  }
  else{
    x -> parent -> left = y;
  }
  
  y -> right = x;
  x -> parent = y;
}

void rb_insert_fixup(rbtree *t, node_t * z){
  node_t * y;
  while(z -> parent -> color == RBTREE_RED){
    if (z -> parent == z -> parent -> parent -> left){
      y = z -> parent -> parent -> right;

      if (y -> color == RBTREE_RED){                   
        z -> parent -> color = RBTREE_BLACK;            //경우 1
        y -> color = RBTREE_BLACK;                      //경우 1
        z -> parent -> parent -> color = RBTREE_RED;    //경우 1
        z = z -> parent -> parent;                      //경우 1
      }
      else{
        if(z == z -> parent -> right){
          z = z -> parent;                              //경우 2
          left_rotate(t, z);                            //경우 2
        }
        z -> parent -> color = RBTREE_BLACK;            //경우 3
        z -> parent -> parent -> color = RBTREE_RED;    //경우 3
        right_rotate(t, z -> parent -> parent);         //경우 3
      }
    }else{
      y = z -> parent -> parent -> left;
      if (y -> color == RBTREE_RED){
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }else{
        if(z == z -> parent -> left){
          z = z -> parent;
          right_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }

  t -> root -> color = RBTREE_BLACK;
}

/* n개로 이루어진 RB-TREE에 대해 O(logn) 시간에 수행.
RB-TREE 특성을 만족함을 보장하기 위해 보조 프로시저 rb_insert_fixup을 호출해
노드의 색깔을 바꾸고 회전을 수행
*/
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t * x = t -> root;
  node_t * y = t -> nil;
  while (x != t -> nil){
    y = x;
    x = (x -> key > key) ? x -> left :  x -> right;
  }

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z -> key = key;
  z -> parent = y;

  if (y == t -> nil){
    t -> root = z;
  }
  else if (z -> key < y -> key){
    y -> left = z;
  }
  else{
    y -> right = z;
  }
  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;

  rb_insert_fixup(t, z);
  return t -> root;
}

node_t *search_node(const rbtree *t, node_t* root, const key_t key){
  if(root == t -> nil){
    return NULL;
  }
  else if(root -> key == key){
    return root;
  }
  else if (root -> key >key){
    return search_node(t, root -> left, key);
  }
  else{
    return search_node(t, root -> right, key);
  }
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  return search_node(t, t -> root, key);
}

node_t *tree_min(rbtree *t, node_t *x) {
  while (x -> left != t -> nil){
    x= x -> left;
  }
  return x;

}
node_t *tree_max(rbtree *t, node_t *x) {
  while (x -> right != t -> nil){
    x= x -> right;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *min_node = t -> root;
  
  while (min_node -> left != t -> nil){
    min_node = min_node -> left;
  }
  return min_node; 
}

node_t *rbtree_max(const rbtree *t) {
  node_t *max_node = t -> root;
  
  while (max_node -> right != t -> nil){
    max_node = max_node -> right;
  }
  return max_node; 
}

void rb_translant(rbtree *t, node_t *u, node_t *v){
  if (u -> parent == t -> nil){
    t -> root = v;
  }
  else if (u == u -> parent -> left){
    u -> parent -> left = v;
  }
  else u -> parent -> right = v;
    v -> parent = u -> parent;
}

void rb_delete_fixup(rbtree *t, node_t* x){
  node_t *w;
  while(x != t -> root && x -> color == RBTREE_BLACK){
    if(x == x -> parent -> left){                                                       //왼쪽 자식일 때
      w = x -> parent -> right;

      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;                                                      //경우 1
        x -> parent -> color = RBTREE_RED;                                              //경우 1
        left_rotate(t, x -> parent);                                                    //경우 1
        w = x -> parent -> right;                                                       //경우 1
      }
      if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;                                                        //경우 2
        x = x -> parent;                                                                //경우 2
      }
      else {
        
        if (w -> right -> color == RBTREE_BLACK){
          right_rotate(t, w);                                                           //경우 3
          w = x -> parent -> right;                                                     //경우 3
        }
        w -> color = x -> parent -> color;                                              //경우 4
        x -> parent -> color = RBTREE_BLACK;                                            //경우 4
        w -> right -> color = RBTREE_BLACK;                                             //경우 4
        left_rotate(t, x -> parent);                                                    //경우 4
        x = t -> root;                                                                  //경우 4
      }
    }
    else{                                                                               //오른쪽 자식일 때
      w = x -> parent -> left;
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }
      if(w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK){
        w -> color = RBTREE_RED;
        x = x -> parent;
      }
      else {
        if (w -> left -> color == RBTREE_BLACK){
          left_rotate(t, w);
          w = x -> parent -> left;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }
    }
  }
  x -> color = RBTREE_BLACK;
}

/*
n개로 이루어진 RB-TREE에 대해 O(logn) 시간에 수행.
*/
int rbtree_erase(rbtree *t, node_t *z) {
  if (z == NULL){
    return 0;
  }
  node_t *x;

  node_t *y = z;
  color_t y_color = y -> color;
  if (z -> left == t -> nil){
    x = z -> right;
    rb_translant(t, z, z -> right);
  }
  else if (z -> right == t -> nil){
    x = z -> left;
    rb_translant(t, z, z -> left);
  }
  else{
    y = tree_min(t, z -> right);
    y_color = y -> color;
    x = y -> right;

    if (y -> parent == z){
      x -> parent = y;
    }
    else{
      rb_translant(t, y, y -> right);
      y -> right = z -> right;
      y -> right -> parent = y;
    }

    rb_translant(t, z, y);
    y -> left = z -> left;
    y -> left -> parent = y;
    y -> color = z -> color;
  }

  if (y_color == RBTREE_BLACK) 
    rb_delete_fixup(t, x);
  
  free(z);
  return 0;
}

void inorder_to_array(const rbtree *t, node_t *root, int *pidx, key_t *arr){
  if (root != t -> nil){
    inorder_to_array(t, root -> left, pidx,arr);
    arr[(*pidx)++] = root -> key;
    inorder_to_array(t, root -> right, pidx,arr);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int idx = 0;
  int *pidx = &idx;
  inorder_to_array(t, t -> root, pidx, arr);
  return 0;
}