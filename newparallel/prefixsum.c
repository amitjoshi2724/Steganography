#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef struct Node{
	struct Node* left;
	struct Node* right;
	int sum;
	int fromLeft;
	int l;
	int h;
} TreeNode;

void generateTree(TreeNode* root, int* input[]){

	int curLow = root.l;
	int curHigh = root.h;
	if(h-l == 1){
		return;
	}
	else{
		TreeNode* newLeft = (TreeNode*)malloc(sizeof(TreeNode));
		newLeft -> l = curLow;
		newLeft -> r = curLow + ((curHigh-curLow)/2);
		
		TreeNode* newRight = (TreeNode*)malloc(sizeof(TreeNode));
		newRight -> l = curLow + ((curHigh-curLow)/2);
		newRight -> r = curHigh
	}
}
int main(int argc, char* argv[]){
	int input[] = {6, 4, 16, 10, 16, 14, 2, 8};

	//pass 1
	TreeNode* root;
	root = (TreeNode*)malloc(sizeof(TreeNode));
	root -> l = 0;
	root -> h = 8;
	generateTree(root, input);

}
