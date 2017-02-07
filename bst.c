#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod {
	struct nod* left;
	struct nod* right;
	int value;
} node;


void WSTAW(node* element, node* tree)
{
	node* tempTree = tree;
	int inserted = 0;
	
	// no need for null check, as insertion will happen at
	// first matching null occurrence
	while(!inserted)
	{

		if(element->value < tempTree->value)
		{
			if(tempTree->left == NULL)
			{
				tempTree->left = element;
				inserted = 1;
			}
			else
			{
				tempTree = tempTree->left;
			}
			
		}
		else
		{
			if(tempTree->right == NULL)
			{
				tempTree->right = element;
				inserted = 1;
			}
			else
			{
				tempTree = tempTree->right;
			}
		}
	
	}
}


node* SZUKAJ(int searchValue, node* tree)
{
	node* tempTree = tree;
	
	while(tempTree != NULL)
	{
		if(searchValue == tempTree->value)
			return tempTree;

		if(searchValue < tempTree->value)
			tempTree = tempTree->left;
		else
			tempTree = tempTree->right;			

	}
	
	// searched value does not exist in a tree
	return NULL;
} 


void USUN(int deleteValue, node* tree)
{
	node* tempTree = tree;
	node* deletedElementParent = tree;
	
	// last path choosen (left = 1; right = 2)
	// allows us to null the deleted child pointer in parent node
	int childDirection = 0;
	
	while(tempTree != NULL)
	{
		if(deleteValue == tempTree->value)
		{
			node* elementToDelete = tempTree;

			// to-delete child is on the left -> move child-of-deleted-element or null to parent-of-deleted-element pointer
			if(childDirection == 1)
			{	
				// 2 children case
				if(tempTree->left != NULL && tempTree->right != NULL)
				{
					// select larger (right) child
					tempTree = tempTree->right;
					// if there is no left child, swap current with element to delete
					if(tempTree->left == NULL)
					{	
						// parent of deleted element now points to childs child
						deletedElementParent->left = tempTree;
						// set deleted element left as childs child left
						tempTree->left = elementToDelete->left;
						return;
					}
					
					// parent of leftmost element
					node* leafParent = tempTree;
					// find leftmost element
					while(tempTree->left != NULL)
					{
						// leftmost childs parent
						leafParent = tempTree;
						// leftmost child
						tempTree = tempTree->left;
					}


					deletedElementParent->left = tempTree;
					// delete leftmost element (leaf) from its parent
					leafParent->left = NULL;
					// set deleted element left child as leaf left child
					tempTree->left = elementToDelete->left;
					// set deleted element right child as leaf right child
					tempTree->right = elementToDelete->right;
					return;
				}
				
				
				// 1 or 0 children case
				// we have already checked that children count is 1 or 0, so this is legal
				if(tempTree->left != NULL)
					deletedElementParent-> left = tempTree->left;
				if(tempTree->right != NULL)
					deletedElementParent->left = tempTree->right;
				else
					deletedElementParent->left = NULL;
				
				return;
			}
			
			// to-delete child is on the right => move child-of-deleted-element or null to parent-of-deleted-element pointer
			if(childDirection == 2)
			{
				// 2 children case
				if(tempTree->left != NULL && tempTree->right != NULL)
				{
					// select larger (right) child
					tempTree = tempTree->right;
					// if there is no left child, swap current with element to delete
					if(tempTree->left == NULL)
					{	
						// parent of deleted element now points to childs child
						deletedElementParent->right = tempTree;
						// set deleted element left as childs child left
						tempTree->left = elementToDelete->left;
						return;
					}
					
					// parent of leftmost element
					node* leafParent = tempTree;
					// find leftmost element
					while(tempTree->left != NULL)
					{
						// leftmost childs parent
						leafParent = tempTree;
						// leftmost child
						tempTree = tempTree->left;
					}


					deletedElementParent->right = tempTree;
					// delete leftmost element (leaf) from its parent
					leafParent->left = NULL;
					// set deleted element left child as leaf left child
					tempTree->left = elementToDelete->left;
					// set deleted element right child as leaf right child
					tempTree->right = elementToDelete->right;
					return;
				}
				
				
				// 1 or 0 children case
				// we have already checked that children count is 1 or 0, so this is legal
				if(tempTree->left != NULL)
					deletedElementParent-> right = tempTree->left;
				if(tempTree->right != NULL)
					deletedElementParent->right = tempTree->right;
				else
					deletedElementParent->right = NULL;
				
				return;
			}
		}
		
		//save parent, as after next iteration we might
		// find element to delete
		deletedElementParent = tempTree;
		
		if(deleteValue < tempTree->value)
		{
			tempTree = tempTree->left;
			childDirection = 1;

		}
		else
		{
			tempTree = tempTree->right;	
			childDirection = 2;			
		}

	}
	
	// searched value doest not exist in a tree => cannot delete
	return;
}


void DRUKUJ(node* tree)
{
	node* tempTree = tree;
	
	if(tempTree == NULL)
		return;
	
	printf("%i ", tempTree->value);
	DRUKUJ(tempTree->left);
	DRUKUJ(tempTree->right);
}



void main()
{
	// ###################### INSERTIONS ########################################
	
	// prepare root element
	node root;
	root.left = NULL;
	root.right = NULL;
	root.value = 5;
	printf("\n## CREATED ROOT ELEMENT WITH VAL == %i\n", root.value);
	
	// insert element with val == 7
	node ele7;
	ele7.left = NULL;
	ele7.right = NULL;
	ele7.value = 7;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele7.value);
	WSTAW(&ele7, &root);
	
	// insert element with val == 3
	node ele3;
	ele3.left = NULL;
	ele3.right = NULL;
	ele3.value = 3;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele3.value);
	WSTAW(&ele3, &root);

	// insert element with val == 2
	node ele2;
	ele2.left = NULL;
	ele2.right = NULL;
	ele2.value = 2;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele2.value);
	WSTAW(&ele2, &root);
	
	// insert element with val == 6
	node ele6;
	ele6.left = NULL;
	ele6.right = NULL;
	ele6.value = 6;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele6.value);
	WSTAW(&ele6, &root);
	
	// insert element with val == 11
	node ele11;
	ele11.left = NULL;
	ele11.right = NULL;
	ele11.value = 11;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele11.value);
	WSTAW(&ele11, &root);
	
	// insert element with val == 10
	node ele10;
	ele10.left = NULL;
	ele10.right = NULL;
	ele10.value = 10;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele10.value);
	WSTAW(&ele10, &root);
	
	// insert element with val == 14
	node ele14;
	ele14.left = NULL;
	ele14.right = NULL;
	ele14.value = 14;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele14.value);
	WSTAW(&ele14, &root);
	
	// insert element with val == 4
	node ele4;
	ele4.left = NULL;
	ele4.right = NULL;
	ele4.value = 4;
	printf("## INSERTING ELEMENT WITH VAL == %i\n", ele4.value);
	WSTAW(&ele4, &root);

	
	
	

	// ###################### SEARCHING ########################################
	printf("\n\n");	
		
	// search and return node with val == 9 -- not in a tree => returns NULL
	int searchValue1 = 9;
	printf("\n## SEARCHING FOR AN ELEMENT WITH VAL == %i \n", searchValue1);
	node* searchResult1 = SZUKAJ(searchValue1, &root);
	if(searchResult1 != NULL)
		printf(">> found element. (value = %i) \n", searchResult1->value);
	else
		printf(">> cannot find element\n");	
	
	// search and return node with val == 7 -- in a tree => returns node with val == 7
	int searchValue2 = 7;
	printf("\n## SEARCHING FOR AN ELEMENT WITH VAL == %i \n", searchValue2);
	node* searchResult2 = SZUKAJ(searchValue2, &root);
	if(searchResult2 != NULL)
		printf(">> found element. (value = %i) \n", searchResult2->value);
	else
		printf(">> cannot find element\n");
	
	
	
	
	
	
	// ###################### DELETING ########################################
	printf("\n\n");
	
	// traverse tree in order
	printf("\n## PRINTING TREE IN-ORDER \n");
	DRUKUJ(&root);
	printf("\n");
	
	
	// delete node with val == 7 -- in a tree => deleted successfuly
	int deleteValue1 = 7;
	printf("\n\n## DELETING ELEMENT WITH VAL == %i \n", deleteValue1);
	USUN(deleteValue1, &root);
	if(SZUKAJ(deleteValue1, &root) == NULL)
		printf(">> success (element deleted OR not in a tree) \n");
	else
		printf(">> failure (element still in a tree) \n");
	
	// traverse tree in order
	printf("\n## PRINTING TREE IN-ORDER \n");
	DRUKUJ(&root);
	printf("\n");
	
	
	// delete node with val == 3 -- in a tree => deleted successfuly
	int deleteValue2 = 3;
	printf("\n\n## DELETING ELEMENT WITH VAL == %i \n", deleteValue2);
	USUN(deleteValue2, &root);
	if(SZUKAJ(deleteValue2, &root) == NULL)
		printf(">> success (element deleted OR not in a tree) \n");
	else
		printf(">> failure (element still in a tree) \n");
	
	// traverse tree in order
	printf("\n## PRINTING TREE IN-ORDER \n");
	DRUKUJ(&root);
	printf("\n");
	
	
	// delete node with val == 13 -- not in a tree => successful (not existed)
	int deleteValue3 = 12;
	printf("\n\n## DELETING ELEMENT WITH VAL == %i \n", deleteValue3);
	USUN(deleteValue3, &root);
	if(SZUKAJ(deleteValue3, &root) == NULL)
		printf(">> success (element deleted OR not in a tree) \n");
	else
		printf(">> failure (element still in a tree) \n");
	
	// traverse tree in order
	printf("\n## PRINTING TREE IN-ORDER \n");
	DRUKUJ(&root);
	printf("\n");
	
	
	return;
}