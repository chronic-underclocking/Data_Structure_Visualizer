#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

enum Color {RED, BLACK};

struct Node
{
    int data;
    int ID; // Unique node ID, to be used in DOT visualization
    Color color;
    Node* parent;
    Node* left;
    Node* right;
};

class RBTree
{
private:

    Node* root;
    Node* leaf;

    int getMaxID(Node* node, int &mx)
    {
        if(node == leaf) return 0;
        mx = (mx > node -> ID  ? mx : node -> ID);
        getMaxID(node -> left, mx);
        getMaxID(node -> right, mx);
        return 0;
    }

    Node* minimum(Node* node)
    {
        while (node -> left != leaf)
        {
            node = node->left;
        }
        return node;
    }

    void printTree(std::string prefix, Node* node, bool isLeft)
    {
        if(node != leaf)
        {
            std::cout << prefix;
            std::cout << (isLeft ? "|---" : "!---" );
            std::cout << node -> data <<  ' ' << (node  -> color ? "BLACK" : "RED") <<"\n";//<< ' '<< node -> ID << "\n";

            printTree(prefix + (isLeft ? "|   " : "    "), node -> left, true);
            printTree(prefix + (isLeft ? "|   " : "    "), node -> right, false);
        }
    }

    void deleteTree(Node* node)
    {
        if (node == leaf) return;

        deleteTree(node -> left);
        deleteTree(node -> right);

        delete node;
    }

    void deleteRoot(Node** node_ref)
    {
        deleteTree(*node_ref);
        *node_ref = nullptr;
    }

    void leftRotate(Node* node)
    {
        Node* y = node -> right;
        node -> right = y -> left;
        if(y -> left != leaf)
        {
            y -> left -> parent = node;
        }
        y -> parent = node -> parent;
        if(node -> parent == nullptr)
        {
            root = y;
        }
        else if(node == node -> parent -> left)
        {
            node -> parent -> left = y;
        }
        else
        {
            node ->parent -> right = y;
        }
        y -> left = node;
        node -> parent = y;
    }

    void rightRotate(Node* node)
    {
        Node* y = node -> left;
        node -> left = y -> right;
        if(y -> right != leaf)
        {
            y -> right -> parent = node;
        }
        y -> parent = node -> parent;
        if(node -> parent == nullptr)
        {
            root = y;
        }
        else if(node == node -> parent -> right)
        {
            node -> parent -> right = y;
        }
        else
        {
            node -> parent -> left = y;
        }
        y -> right = node;
        node -> parent = y;
    }

    void transplant(Node* a, Node* b)
    {
        if(a -> parent == nullptr)
        {
            root = b;
        }
        else if(a == a -> parent -> left)
        {
            a -> parent -> left = b;
        }
        else
        {
            a -> parent -> right = b;
        }
        if(b != nullptr)
        {
            b -> parent = a -> parent;
        }
    }

    void generateDot()
    {
        std::ofstream dotFile("dotFiles\\rbtree.dot");
        dotFile << "strict graph G {\n"
                << "graph [ dpi = 500 ];\n"
                << "node [style = filled, fontcolor = white];\n";

        std::vector<Node*> vec;
        traverse(root, vec);
        for(auto node : vec)
        {
            dotFile << node -> ID << ' ' << "[label = " << node -> data << ", fillcolor = "
                    << (node -> color == RED ? "red" : "black") << "];\n";
        }
        bool root = true;
        for(auto node : vec)
        {
            if(root)
            {
                root = false;
                continue;
            }
            dotFile << node -> parent -> ID << "--" << node -> ID << ";\n";
        }
        dotFile << '}';
        vec.clear();
        dotFile.close();
    }

    void traverse(Node* node, std::vector<Node*> &vec)
    {
        if(node == leaf) return;
        vec.push_back(node);
        traverse(node -> left, vec);
        traverse(node -> right, vec);
    }

public:

    RBTree()
    {
        leaf = new Node;
        leaf -> color = BLACK;
        leaf -> left = nullptr;
        leaf -> right = nullptr;
        root = leaf;
    }

    ~RBTree()
    {
        Clear();
        delete leaf;
    }

    void Insert(int key)
    {
        Node* node = new Node;
        node -> data = key;
        int maxID = 0;
        getMaxID(root, maxID);
        node -> ID = maxID + 1;
        node -> color = RED;
        node -> parent = nullptr;
        node -> right = leaf;
        node -> left = leaf;

        if(root == leaf)
        {
            node -> color = BLACK;
            root = node;
            return;
        }

        Node* pos = root;
        Node* parental = nullptr;
        while(pos != leaf)
        {
            parental = pos;
            if(node -> data < pos -> data)
            {
                pos = pos -> left;
            }
            else
            {
                pos = pos -> right;
            }
        }

        node -> parent = parental;

        if(node -> data < parental -> data)
        {
            parental -> left = node;
        }

        else
        {
            parental -> right = node;
        }

        while(node != root && node -> parent -> color == RED)
        {
            if(node -> parent == node -> parent -> parent -> left)
            {
                Node* uncle = node -> parent -> parent -> right;
                if(uncle -> color ==  RED)
                {
                    node -> parent -> color = BLACK;
                    uncle -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    node = node -> parent -> parent;
                }

                else
                {
                    if(node == node -> parent -> right)
                    {
                        node = node -> parent;
                        leftRotate(node);
                    }
                    node -> parent -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    rightRotate(node -> parent -> parent);
                }
            }
            else
            {
                Node* uncle = node -> parent -> parent -> left;
                if(uncle -> color ==  RED)
                {
                    node -> parent -> color = BLACK;
                    uncle -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    node = node -> parent -> parent;
                }

                else
                {
                    if(node == node -> parent -> left)
                    {
                        node = node -> parent;
                        rightRotate(node);
                    }
                    node -> parent -> color = BLACK;
                    node -> parent -> parent -> color = RED;
                    leftRotate(node -> parent -> parent);
                }
            }
        }

        root -> color = BLACK;
    }

    void Delete(int key)
    {
        Node* searching = root;
        Node* node = leaf;
        Node* x;
        Node* y;
        while(searching != leaf)
        {
            if(searching -> data == key)
            {
                node = searching;
            }

            if(searching -> data >= key)
            {
                searching = searching -> left;
            }
            else
            {
                searching = searching -> right;
            }
        }

        if(node == leaf) return;

        y = node;
        Color y_color = y -> color;
        if(node -> left == leaf)
        {
            x = node -> right;
            transplant(node, node -> right);
        }
        else if(node -> right == leaf)
        {
            x = node -> left;
            transplant(node, node -> left);
        }
        else
        {
            y = minimum(node -> right);
            y_color = y -> color;
            x = y -> right;
            if(y -> parent == node)
            {
                x -> parent = y;
            }
            else
            {
                transplant(y, y -> right);
                y -> right = node -> right;
                y -> right -> parent = y;
            }
            transplant(node, y);
            y -> left = node -> left;
            y -> left -> parent = y;
            y -> color = node -> color;
        }

        delete node;

        if(y_color == BLACK)
        {
            Node* n;
            while(x != root && x -> color == BLACK)
            {
                if(x == x -> parent -> left)
                {
                    n = x -> parent -> right;

                    if(n -> color == RED)
                    {
                        n -> color = BLACK;
                        x -> parent -> color = RED;
                        leftRotate(x -> parent);
                        n = x -> parent -> right;
                    }

                    if(n -> left -> color == BLACK && n -> right -> color == BLACK)
                    {
                        n -> color = RED;
                        x = x -> parent;
                    }

                    else
                    {
                        if(n -> right -> color == BLACK)
                        {
                            n -> left -> color = BLACK;
                            n -> color = RED;
                            rightRotate(n);
                            n = x -> parent -> right;
                        }

                        n -> color = x -> parent -> color;
                        x -> parent-> color = BLACK;
                        n -> right-> color = BLACK;
                        leftRotate(x -> parent);
                        x = root;
                    }
                }

                else
                {
                    n = x -> parent -> left;
                    if(n -> color == RED)
                    {
                        n -> color = BLACK;
                        x ->parent -> color = RED;
                        rightRotate(x -> parent);
                        n = x -> parent -> left;
                    }

                    if(n -> right -> color == BLACK && n -> right -> color == BLACK)
                    {
                        n -> color = RED;
                        x = x -> parent;
                    }

                    else
                    {
                        if(n -> left -> color == BLACK)
                        {
                            n -> right -> color = BLACK;
                            n -> color = RED;
                            leftRotate(n);
                            n = x -> parent -> left;
                        }

                        n -> color = x -> parent -> color;
                        x -> parent -> color = BLACK;
                        n -> left -> color = BLACK;
                        rightRotate(x -> parent);
                        x = root;
                    }
                }
            }

            x -> color = BLACK;
        }
    }

    void Print()
    {
        //printTree("", root, false);
        generateDot();
        string filename = "dataStructures\\dot.py";
        string command = "python ";
        command += filename;
        system(command.c_str());
    }

    void Clear()
    {
        deleteRoot(&root);
        leaf = new Node;
        leaf -> color = BLACK;
        leaf -> left = nullptr;
        leaf -> right = nullptr;
        root = leaf;
    }
};


int main(int argc,char* argv[])
{
    RBTree tree;
    int choice;
    int num;
    do
    {
        cout << "1- Insert\n2- Delete\n3- Clear\n4- Exit\n\n";
        cout <<"Enter your choice: ";
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << "Enter number to insert: ";
                cin >> num;
                tree.Insert(num);
                tree.Print();
                break;
            case 2:
                cout << "Enter number to delete: ";
                cin >> num;
                tree.Delete(num);
                tree.Print();
                break;
            case 3:
                tree.Clear();
                break;
            default:
                break;
        }
    }
    while(choice != 4);

    return 0;
}
