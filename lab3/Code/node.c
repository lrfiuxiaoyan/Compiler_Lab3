#include "node.h"
Content *allContent = NULL;

//int extId = 0;

void insertContent(char* content){
	
	Content* tmp = malloc(sizeof(Content));
	Content *temp = allContent;
	strcpy(tmp->content, content);
	if(allContent == NULL) {
		allContent = tmp;
		allContent->next = NULL;
	}
	else{
		while(temp->next!= NULL){
			temp = temp->next;
		}
		temp->next = tmp;
	}
	tmp->next = NULL;
	//printf("Content = %s\n", content);
}


GrammarTree * createNode(char* idname, char* value,int line, enum treeType type, char* content){
	GrammarTree* node = (GrammarTree*)malloc(sizeof(GrammarTree));
//node->id = extId;
//extId++;
	strcpy(node->idName, idname);
	node->type = type;
	node->lineNum = line;
	node->childNode = NULL;
	node->siblingNodes = NULL;
	if(type == 2){
//		printf("%s AC = %s\n", node->idName, allContent->content);
		if(content) {
			strcpy(node->content, content);
		}
	}
	return node;
}

void insertNodes(GrammarTree * parent, GrammarTree *child){
	GrammarTree * tmp = parent->childNode;
	if(tmp == NULL){
		parent->childNode = child;
		return;
	}


	while(tmp->siblingNodes != NULL){
		tmp = tmp->siblingNodes;
	}
	tmp->siblingNodes = child;

	//printf("AC = %s\n", child->idName);
	
}
void printNodes(GrammarTree* node, int blankNum){
	if(node == NULL){return;}
	for(int i = 0; i < blankNum; i++){printf("  ");}//blank
	if(node->type == 0){
	 	printf("%s (%d)\n", node->idName, node->lineNum);
	}
	else if(node->type == 1){
	 	printf("%s\n", node->idName);
	}
	else{
		printf("%s:%s\n", node->idName, node->content);		
	}
	printNodes(node->childNode, blankNum+1);
	printNodes(node->siblingNodes, blankNum);
	return;
}
