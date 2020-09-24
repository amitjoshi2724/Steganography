//Amit Joshi 9/27/2017 Period: 5
#include <math.h>
#include <string.h>
//
// Torbert, 16 Sept 2015
//
#include <stdio.h>
#include <stdlib.h>
//
typedef struct Node
{
   char symbol ;
	//
   int frequency ;
	//
   struct Node* left ;
   struct Node* right ;
	//
} TreeNode ;
//
char* codewords[256];
bool first = TRUE;
void copy_string(char target[], char source[]) {
   int i;
   for(i = 0; source[i] != '\0'; ++i){
      target[i] = source[i];
   }
   target[i] = '\0';
}
void save(char c, char route[]){
   codewords[c] = route;
}
void walk(TreeNode* node, char route[], int depth, FILE* fp){
   if(!first){
        printf("%s\n", codewords['m']);
   }
   if((*node).left == NULL && (*node).right == NULL){
      
      /*for(int i = 0; i < depth; i++){
         printf("\t");
      }*/
      if(first){
        codewords[(char)(*node).symbol] = route;
      
        first = FALSE;
      }
      printf("%c ", (char)(*node).symbol);
      printf("%s\n", route);
      
      
      fprintf(fp, "%c", (*node).symbol);
      fprintf(fp, "%s", route);
      fprintf(fp, "\n");
      return;
   }
   char left[depth+1];
   copy_string(left, route);
   walk((*node).left, strcat(left, "0"), depth + 1, fp);
   char right[depth+1];
   copy_string(right, route);
   walk((*node).right, strcat(right, "1"), depth + 1, fp);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
int main( int argc , char* argv[] )
{
   char* txt = "Hello World from Amit.";
   int freq[256] = {0};
   TreeNode* arr[256] = {NULL};
   for(int ip = 0; ip < strlen(txt); ip++){
      freq[txt[ip]] += 1;
   }
   int m = 0; 
   int n = 0;
   for(int i = 0; i < 256; i++){
      if(freq[i] > 0){
         m++;
      }
      n += freq[i];
      TreeNode* t = NULL ;
   		//
      t = (TreeNode*)malloc( sizeof(TreeNode) );
      (*t).symbol = i;
      t -> frequency = freq[i] ;
      t -> left = NULL;
      t -> right = NULL;
      arr[i] = t;
   		//printf( "%c\n" , t->symbol ) ;
   		//printf( "%d\n" , t->frequency ) ;
   
   }
   for (int i = 0; i < 256; ++i)
   {
      for (int j = i + 1; j < 256; ++j)
      {
         if ((*arr[i]).frequency < (*arr[j]).frequency)
         {
            TreeNode* a = arr[i];
            arr[i] = arr[j];
            arr[j] = a;
         }
      }
   }
   FILE* fp;
   fp = fopen("encoded_message.txt", "w");
   fprintf(fp, "%d", m);
   fprintf(fp, "\n");
   while(m > 1){
      for(int i = 0; i < m; i++){
         printf("%d ", (*arr[i]).frequency);
      }
      printf("\n");
      int min1 = m - 1;
      int min2 = m - 2;
      TreeNode* j = (TreeNode*)malloc( sizeof(TreeNode) );
      j -> left = arr[min1];
      j -> right = arr[min2];
      j -> frequency = (*arr[min1]).frequency + (*arr[min2]).frequency;
      arr[min2] = j;
      arr[min1] = NULL;
      m--;
      for (int i = 0; i < m; ++i)
      {
         for (int j = i + 1; j < m; ++j)
         {
            if ((*arr[i]).frequency < (*arr[j]).frequency)
            {
               TreeNode* a = arr[i];
               arr[i] = arr[j];
               arr[j] = a;
            }
         }
      }
   }
   for(int i = 0; i < m; i++){
      printf("%d ", (*arr[i]).frequency);
   }
   printf("\n");
   printf("made it outside\n");
   char route[] = "1";
   walk(arr[0], route, 0, fp);
   int y = 0;
   while(7 > 6){
      if(txt[y] == '\0'){
         break;
      }
      printf("%c", (char)txt[y]);
      printf("%s", codewords[(char)txt[y]]);
      fprintf(fp, "%s", codewords[(char)txt[y]]);
      y++;
   }
	//
   return 0 ;
}

//
// end of file
//
