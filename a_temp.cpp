#include <iostream>
using namespace std;

class Node{

    public:

        Node();

        bool colour;
        int data;
        Node* parent;
        Node* leftchild;
        Node* rightchild;



};
Node::Node(){
    colour = 1;//1 means black, 0 means red
    data = 0; 
    parent = NULL;
    leftchild = NULL;
    rightchild = NULL;
}

class RBT{

    public:

        RBT();
        Node* NIL;
        Node* Root;
        void leftrotate(Node*);
        void rightrotate(Node*);
        void insert(Node*);
        void correction(Node*);
        void binarysearchseq(Node*);
        void printdata(Node*);

};
RBT::RBT(){
    Node*y  = new Node;
    NIL = y;
    Root = NIL; 
}

void RBT::leftrotate(Node* value){

    Node* y = value->rightchild;
    value->rightchild = y->leftchild;
    if(y->leftchild!=NIL){
        y->leftchild->parent = value;

    }
    y->parent = value->parent;
    if(value->parent==NIL){
        Root = y;
        
    }
    else if(value==value->parent->leftchild){
        value->parent->leftchild = y;
    }
    else{
        value->parent->rightchild = y;
    }
    y->leftchild = value;
    value->parent = y;


}
void RBT::rightrotate(Node* value){

    Node* y = value->leftchild;
    value->leftchild = y->rightchild;
    if(y->rightchild!=NIL){
        y->rightchild->parent = value;
    }
    y->parent = value->parent;
    if(value->parent==NIL){
        Root = y;

    }
    else if(value==value->parent->leftchild){
        value->parent->leftchild = y;
    }
    else{
        value->parent->rightchild = y;
    }
    y->rightchild = value;
    value->parent = y;



}
void RBT::correction(Node* value){

    if(value==Root){
        Root->colour = 1;
        return;
    }
    //when there is a conflict
    if(value->parent->colour==0){
        //grandfather exits because the parent colour is red, and parent cannot be root
        Node* father = value->parent;
        Node* grandfather = value->parent->parent;
        //when father is leftchild of grandfather
        if(grandfather->leftchild==father){
            Node* uncle = grandfather->rightchild;
            //uncle and father has same red colour
            if(grandfather->rightchild->colour==0){
                //lifting conflict upwards to grandfather
                father->colour = 1;
                grandfather->rightchild->colour = 1;
                grandfather->colour = 0;
                correction(grandfather);
            }
            else{
                //when uncle is black, and the value is right child of its father
                if(value->parent->rightchild==value){
                    leftrotate(father);
                    father = value;

                }
                //value is left child of it's father
                grandfather->colour = 0;
                father->colour = 1;
                rightrotate(grandfather);
                return;
                




            }

        }else{
            if(grandfather->leftchild->colour==0){
                father->colour = 1;
                grandfather->leftchild->colour = 1;
                grandfather->colour = 0;
                correction(grandfather);
            }
            else{

                if(value->parent->leftchild==value){
                    rightrotate(father);
                    father = value;
                }
                grandfather->colour = 0;
                father->colour = 1;
                leftrotate(grandfather);
                return;

            }

        }
        
    }

}
void RBT::insert(Node* value){

    value->leftchild = NIL;
    value->rightchild = NIL;
    value->parent = NIL;
    value->colour = 0;



    int data = value->data;
    

    Node* temp = Root;
    if(temp==NIL){
        Root = value;
        Root->colour = 1;

        return;
    }
    while(1){

        int val = temp->data;
        if(temp==NIL||temp==NULL){
            break;
        }
        if(data>=val){
            if(temp->rightchild!=NIL){
                temp = temp->rightchild;
                continue;

            }

            temp->rightchild = value;
            value->parent = temp;
            break;
        }
        else{
            if(temp->leftchild!=NIL){
                temp = temp->leftchild;
                continue;

            }
            temp->leftchild = value;
            value->parent = temp ;

            break;

        }

    }
    correction(value);
    
    




}

void RBT::printdata(Node* value){

    if(value==NULL||value==NIL){
        return;
    }
    printdata(value->leftchild);
    cout << value->data <<" ";
    printdata(value->rightchild);

}
int count = 0;
int* storagearray;

void RBT::binarysearchseq(Node* value){
    if(value==NULL||value==NIL){
        return;
    }
    // storing the root of all subtrees, when it encounter one
    storagearray[count++] = value->data;
    
    binarysearchseq(value->leftchild);
    binarysearchseq(value->rightchild);

}
int main(){

    RBT inputtree;

    int n;
    cin >> n;

    storagearray = new int[n];

    for(int i = 0;i<n;i++){
        int k;
        cin >> k;

        Node* tempnode = new Node;
        tempnode->data = k;
        inputtree.insert(tempnode);
    }
    
    inputtree.binarysearchseq(inputtree.Root);
    for(int i = 0;i<n-1;i++){
        cout << storagearray[i]<<" ";
    }
    if(n-1>=0){
        cout << storagearray[n-1]<<endl;

    }
   









    return 0;
}