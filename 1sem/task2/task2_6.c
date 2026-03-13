#include <stdio.h>
#include <stdlib.h>

typedef struct tnode {
  int elem;
  struct tnode *left;
  struct tnode *right;
} tnode;

void InsertToTree(tnode **tree, int elem) {
  
  if (*tree == NULL) {
    *tree = (tnode*)malloc(sizeof(tnode));
    (*tree)->elem = elem;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  }
  else {
    tnode *current = *tree;
    if (current->elem > elem) 
      InsertToTree(&(current->left), elem);
    else if (current->elem < elem)
      InsertToTree(&(current->right), elem);
  }
}

void RemoveFromTree(tnode **tree, int elem) {

  if (*tree == NULL) 
    return;
  
  tnode *current = *tree;

  if (current->elem > elem) 
    RemoveFromTree(&(current->left), elem);
  else if (current->elem < elem)
    RemoveFromTree(&(current->right), elem);
  
  else {
    if ((current->left == NULL) && (current->right == NULL)) {
      free(current);
      *tree = NULL;
    }
    else if (current->left == NULL) {
      *tree = current->right;
      free(current);
    }
    else if (current->right == NULL) {
      *tree = current->left;
      free(current);
    }

    else {
      tnode *minNode = current->right;
      tnode *minParent = current;
      
      while (minNode->left != NULL) {
        minParent = minNode;
        minNode = minNode->left;
      }
    
      current->elem = minNode->elem;
      if (minParent->left == minNode)
        minParent->left = minNode->right;
      else
        minParent->right = minNode->right;

      free(minNode);
    }
  }    
}

void FindInTree(tnode *tree, int elem) {
  if (tree == NULL) {
    printf("%d no\n", elem);
    return;
  }
  
  if (tree->elem > elem)
    FindInTree(tree->left, elem);
  else if (tree->elem < elem)
    FindInTree(tree->right, elem);
  else {
    printf("%d yes\n", elem);
  }
}  

void CleanTree(tnode **tree) {
  
  if (*tree == NULL)
    return;
  
  CleanTree(&((*tree)->left));
  CleanTree(&((*tree)->right));
  free(*tree);
  *tree = NULL;
}

int CountBalancedTrees(tnode *tree) {
    int count = 0;

    if (tree == NULL) {
        return 1;
    }

    if ((tree->right != NULL) && (tree->left != NULL) || ((tree->right == NULL) && (tree->left == NULL))) {
        count = 1;
    }
    
    return count + CountBalancedTrees(tree->left) + CountBalancedTrees(tree->right);

}

int main() {
  tnode *tree = NULL;
  char command;
  int elem;
  
  
  while (scanf(" %c", &command) != EOF) {
    if (command == '+' || command == '-' || command == '?') {
      if (scanf("%d", &elem) == 1) {
        switch (command) {
          case '+':
            InsertToTree(&tree, elem);
            break;
          case '-':
            RemoveFromTree(&tree, elem);
            break;
          case '?':
            FindInTree(tree, elem);
            break;
        }
      }
    }
  }

  printf("Колличество сбалансированных деревьев = %d\n ",CountBalancedTrees(tree));
  
  CleanTree(&tree);
  return 0;
}
