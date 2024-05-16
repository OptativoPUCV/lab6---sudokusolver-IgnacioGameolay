#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
   //Verificar filas
   for (int fil = 0; fil < 9;fil++){
      int arr[10] = {0};
      for (int col = 0; col < 9; col++){
         int num = n->sudo[fil][col];
         if ((num != 0) && (arr[num] == 1)){
            return 0;
         }
         arr[num] = 1;
      }
   }
   
   //Verificar columnas
   for (int col = 0; col < 9; col++){
      int arr[10] = {0};
      for (int fil = 0; fil < 9;fil++){
         int num = n->sudo[fil][col];
         if ((num != 0) && (arr[num] == 1)){
            return 0;
         }
         arr[num] = 1;
      }
   }

   //Verificar submatrices
   for (int k = 0; k < 9; k += 3){
      for (int l = 0; l < 9; l += 3){
         int arr[10] = {0};
         for (int i = k; i < k+3; i++){
            for (int j = l; j < l+3; j++){
               int num = n->sudo[i][j];
               if ((num != 0) && (arr[num] == 1)){
                  return 0;
               }
               arr[num] = 1;
            }
         }
      }
   }
   return 1;
}


List* get_adj_nodes(Node* n){
   List* list = createList();
   int fil,col;

   //Buscar 1r casilla vac[ia]
   for (int i = 0; i < 9;i++){
      fil = i;
      for (int j = 0; j < 9; j++){
         col = j;
         if (n->sudo[i][j] == 0){
            break;
         }
      }
      if (n->sudo[fil][col] == 0){
         break;
      }
   }

   //Todas las casillas lenas
   if (fil == 9 && col == 9){
      return list;
   }

   //crear adjts.
   for (int num = 1; num <= 9; num++){
      Node* aux_node = copy(n); //Creo nuevo nodo
      aux_node->sudo[fil][col] = num;
      if(is_valid(aux_node)){
         pushBack(list, aux_node);
      } else {
         free(aux_node);
      }
   }
   
   return list;
}


int is_final(Node* n){
   for (int fil = 0; fil < 9;fil++){
      for (int col = 0; col < 9; col++){
         if (n->sudo[fil][col] == 0){
            return 0;
         }
      }
   }
   return 1;
}

/*
1-Cree un stack S (pila) e inserte el nodo.

2-Mientras el stack S no se encuentre vacío:

a) Saque y elimine el primer nodo de S.

b) Verifique si corresponde a un estado final, si es así retorne el nodo.

c) Obtenga la lista de nodos adyacentes al nodo.

d) Agregue los nodos de la lista (uno por uno) al stack S.

e) Libere la memoria usada por el nodo.

3-Si terminó de recorre el grafo sin encontrar una solución, retorne NULL.
*/
Node* DFS(Node* initial, int* cont){
   //1
   Stack* S = createStack();
   push(S, initial);

   //2
   while (!is_empty(S)){
      //a
      Node* current_node = top(S);
      pop(S);
      (*cont)++;
      //b
      if (is_final(current_node)){
         free(S);
         return current_node;
      }
      //c
      List* adj_nodes = get_adj_nodes(current_node);
      //d
      Node* aux= first(adj_nodes);
      while(aux){
         push(S, aux);
         aux = next(adj_nodes);
      }
      //e
      free(adj_nodes);
   }

   //3
   return NULL;
}

/*
void deep_first_search (node* I){
  Stack* S=createStack();
  push(S,I);
  while (size(S) != 0){
     node* n = pop(S);
     if is_visited(n) continue;
     
     visit(n);
     List* adj=get_adj_nodes(n);
     node* aux= first(adj);
     while(aux){
        push(S,aux);
        aux=next(adj);
     }
     free(n);
  }
}
*/

/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}
*/