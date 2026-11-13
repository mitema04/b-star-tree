#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <ranges>
using namespace std;

class Data {
public:
    string id;
    string f_name, l_name, email;
    string id2;
    void print() const{
        cout << "ID: " << id << " First Name: " << f_name
            << " Last Name: " << l_name << " Email: " << email
            << " Foreign key: " << id2 << endl;
    }
    string print_in_file() const {
        string out=id+"|"+f_name+"|"+l_name+"|"+email+"|"+id2;
        return out;
    }
};

class Node {
public:
    bool isRoot;
    int numKeys;
    int minNum;
    int maxNum;
    int childrenNum;
    Data *keys[15];
    Node *father;
    Node *children[15];

    void print_Node() {
        if (father!=nullptr) {
            cout<<"Otac: ";
            for (int i=0; i<father->numKeys; i++)
            {
                cout << father->keys[i]->id << " ";
            }
            cout<<endl<<"Moji kljucevi: ";
            for (int i=0; i<numKeys; i++) {
                cout << keys[i]->id << " ";
            }
            cout<<endl;
        }
        else {
            cout<<"Otac: nema"<<endl;
            cout<<"Moji kljucevi: ";
            for (int i=0; i<numKeys; i++) {
                cout << keys[i]->id << " ";
            }
            cout<<endl;
        }
    }

    Node* find_right_bro() {
        Node *otac=this->father;
        if (otac==nullptr)
            return nullptr;
        int ind=0;
        for (int i=0; i<otac->childrenNum; i++) {
            if (otac->children[i]==this) {
                ind=i;
                break;
            }
        }
        if (ind==otac->childrenNum-1)
            return nullptr;
        else
            return otac->children[ind+1];
    }

    Node* find_left_bro() {
        Node *otac=this->father;
        if (otac==nullptr)
            return nullptr;
        int ind=0;
        for (int i=0; i<otac->childrenNum; i++) {
            if (otac->children[i]==this) {
                ind=i;
                break;
            }
        }
        if (ind==0)
            return nullptr;
        else
            return otac->children[ind-1];
    }

    void sortKeys() {
        for (int i = 0; i < numKeys-1; i++)
        {
            for (int j=i+1; j<numKeys; j++)
            {
                if (keys[j]->id< keys[i]->id)
                {
                    Data *temp=keys[i];
                    keys[i]=keys[j];
                    keys[j]=temp;
                }
            }
        }
    }
};

class Tree {
public:
    Node *root;
    int m;
    int minnumRoot;
    int maxnumRoot;
    int minnumNode;
    int maxnumNode;

    void split_root()
    {
        if (root==nullptr)
            return;
        Node *left_son=new Node();
        left_son->isRoot = false;
        left_son->numKeys=0;
        left_son->childrenNum=0;
        left_son->minNum=minnumNode;
        left_son->maxNum=maxnumNode;
        Node *right_son=new Node();
        right_son->isRoot = false;
        right_son->numKeys=0;
        right_son->childrenNum=0;
        right_son->minNum=minnumNode;
        right_son->maxNum=maxnumNode;

        vector<Node*>sons_list;

        // novonastali koren ce uvek imati 2 deteta i jedan kljuc
        Node *new_root=new Node();
        new_root->isRoot = true;
        new_root->numKeys = 1;
        new_root->father=nullptr;
        new_root->minNum = minnumRoot;
        new_root->maxNum = maxnumRoot;
        new_root->childrenNum = 2;

        int middle=floor(m/2);
        new_root->keys[0]=root->keys[middle];

        for (int i=0; i<root->childrenNum; i++)
        {
            sons_list.push_back(root->children[i]);
        }
        for (int i=0; i<middle; i++)
        {
            left_son->keys[i]=root->keys[i];
            left_son->numKeys++;
        }
        for (int i=middle+1; i<root->numKeys; i++)
        {
            right_son->keys[i-middle-1]=root->keys[i];
            right_son->numKeys++;
        }
        if (sons_list.size()>0) {
            for (int i=0; i<=middle; i++)
            {
                sons_list[i]->father=left_son;
                left_son->children[i]=sons_list[i];
                left_son->childrenNum++;
            }
            for (int i=middle+1; i<root->childrenNum; i++)
            {
                sons_list[i]->father=right_son;
                right_son->children[i-middle-1]=sons_list[i];
                right_son->childrenNum++;
            }
            /*
            for (int i=0; i<right_son->childrenNum; i++) {
                right_son->children[i]=right_son->children[i+1];
            }
            */
        }

        new_root->children[0]=left_son;
        new_root->children[1]=right_son;
        right_son->father=new_root;
        left_son->father=new_root;

        Node *temp=root;
        root=new_root;
        delete temp;
    }

    Node* find_invalid_root() {
        if (root==nullptr)
            return nullptr;
        vector<Node*> lista;
        lista.push_back(root);
        while(lista.size()>0) {
            Node* temp=lista.front();
            lista.erase(lista.begin());
            for (int i=0; i<temp->numKeys+1; i++)   // dece ima za 1 vise od kljuceva
            {
                if (temp->children[i]!=nullptr) {
                    lista.push_back(temp->children[i]);
                }
            }
            if (temp->numKeys == temp->maxNum) {
                return temp;
            }
        }
        return nullptr;
    }

    Node *find_invalid_root_for_update()
    {
        if (root==nullptr)
            return nullptr;
        vector<Node*> lista;
        lista.push_back(root);
        while(lista.size()>0) {
            Node* temp=lista.front();
            lista.erase(lista.begin());
            for (int i=0; i<temp->numKeys+1; i++)   // dece ima za 1 vise od kljuceva
            {
                if (temp->children[i]!=nullptr) {
                    lista.push_back(temp->children[i]);
                }
            }
            if (temp->numKeys == temp->maxNum || temp->numKeys<temp->minNum-1) {
                return temp;
            }
        }
        return nullptr;

    }
    Node* find_insert_root(Data* elem)
    {
        Node *temp=root;
        if (temp==nullptr) {
            cout<<"Jesam"<<endl;
            return temp;
        }
        else
        {
            while (true){
                if (temp->childrenNum==0)
                    break;
                for (int i=0; i<temp->numKeys; i++) {
                    if (temp->keys[i]->id>elem->id) {
                        temp=temp->children[i];
                        break;
                    }
                    if (i==temp->numKeys-1 && elem->id>temp->keys[i]->id){
                        temp=temp->children[i+1];
                        break;
                    }
                }
            }
        }
        return temp;
    }

    void overflow_right(Node *l_node, Node *r_node)   // ovo je moguce da ima gresku, ne proveravam za sad
    {
        Node *father=l_node->father;
        for (int i=0; i<father->childrenNum; i++) {
            if (father->children[i]==l_node)
            {
                Data *d=father->keys[i];
                for (int j=r_node->numKeys; j>0; j--) // prvo sve od desnog moramo da pomerimo da mozemo ovaj da ubacimo
                {
                    r_node->keys[j]=r_node->keys[j-1];
                }
                r_node->keys[0]=d;  // najdesnji od levog cvora postavljamo
                r_node->numKeys++;   // dodajemo ga
                father->keys[i]=l_node->keys[l_node->numKeys-1];

                l_node->keys[l_node->numKeys-1]=nullptr;
                l_node->numKeys--;

                //azuriranje niza pokazivaca na decu
                for (int j=r_node->childrenNum; j>0; j--) {  // pomeramo niz dece u desno
                    r_node->children[j]=r_node->children[j-1];
                }
                if (r_node->childrenNum!=0)
                    r_node->children[0]=l_node->children[l_node->childrenNum-1];
                if(r_node->children!=0 && r_node->children[0]!=nullptr)
                    r_node->children[0]->father=r_node;
                if (r_node->childrenNum!=0)
                    r_node->childrenNum++;
                if (l_node->childrenNum!=0)
                    l_node->children[l_node->childrenNum-1]=nullptr;
                if (l_node->childrenNum>0)
                    l_node->childrenNum--;
                break;

            }
        }
    }

    void overflow_left(Node *l_node, Node *r_node) {
        Node *father=r_node->father;
        for (int i=0; i<father->childrenNum; i++) {
            if (father->children[i]==r_node) {
                Data *d=father->keys[i-1];
                l_node->keys[l_node->numKeys]=d;  // najdesnji od levog cvora postavljamo
                l_node->numKeys++;   // dodajemo ga
                father->keys[i-1]=r_node->keys[0];

                //azuriranje niza kljuceva
                for (int j=0; j<r_node->numKeys-1; j++) {  // pomeramo niz kljuceva u levo
                    r_node->keys[j]=r_node->keys[j+1];
                }
                r_node->keys[r_node->numKeys-1]=nullptr;
                r_node->numKeys--;

                //azuriranje niza pokazivaca na decu
                l_node->children[l_node->childrenNum]=r_node->children[0];
                if(l_node->children[l_node->childrenNum]!=nullptr)
                    l_node->children[l_node->childrenNum]->father=l_node;
                if (l_node->childrenNum!=0)
                    l_node->childrenNum++;
                for (int j=0; j<r_node->childrenNum-1; j++) {  // pomeramo niz kljuceva u levo
                    r_node->children[j]=r_node->children[j+1];
                }
                if (r_node->childrenNum!=0)
                    r_node->children[r_node->childrenNum-1]=nullptr;
                if (r_node->childrenNum>0)
                    r_node->childrenNum--;
                break;

            }
        }
    }

    void two_to_three(Node *l_node, Node *r_node) {
        if (root==nullptr)
            return;
        Node *left_son=new Node();
        left_son->isRoot = false;
        left_son->numKeys=0;
        left_son->childrenNum=0;
        left_son->minNum=minnumNode;
        left_son->maxNum=maxnumNode;
        left_son->father=l_node->father;
        Node *right_son=new Node();
        right_son->isRoot = false;
        right_son->numKeys=0;
        right_son->childrenNum=0;
        right_son->minNum=minnumNode;
        right_son->maxNum=maxnumNode;
        right_son->father=l_node->father;
        Node *middle_son=new Node();
        middle_son->isRoot = false;
        middle_son->numKeys=0;
        middle_son->childrenNum=0;
        middle_son->minNum=minnumNode;
        middle_son->maxNum=maxnumNode;
        middle_son->father=l_node->father;

        vector<Data*>keys_list;
        vector<Node*>sons_list;

        // ovde pravimo niz kljuceva
        for (int i=0; i<l_node->numKeys; i++) {
            keys_list.push_back(l_node->keys[i]);
        }
        Data *fatherKey;
        int indexfather=0;
        Node *temp = l_node->father;
        for (int i=0; i<temp->childrenNum; i++) {
            if (temp->children[i]==l_node) {
                fatherKey=temp->keys[i];
                indexfather=i;
                keys_list.push_back(fatherKey);
                break;
            }
        }
        for (int i=0; i<r_node->numKeys; i++) {
            keys_list.push_back(r_node->keys[i]);
        }
        // ovde pravimo niz dece
        if (l_node->childrenNum!=0) {
            for (int i=0; i<l_node->childrenNum; i++) {
                sons_list.push_back(l_node->children[i]);
            }
        }
        if (r_node->childrenNum!=0) {
            for (int i=0; i<r_node->childrenNum; i++) {
                sons_list.push_back(r_node->children[i]);
            }
        }

        int index1=floor((2*m-2)/3);
        int index2=floor((4*m-1)/3);

        //namestanje kljuceva
        for (int i=0; i<index1; i++) {
            left_son->keys[i]=keys_list[i];
            left_son->numKeys++;
        }
        for (int i=index1+1; i<index2; i++) {
            middle_son->keys[i-index1-1]=keys_list[i];
            middle_son->numKeys++;
        }
        for (int i=index2+1; i<keys_list.size(); i++) {
            right_son->keys[i-index2-1]=keys_list[i];
            right_son->numKeys++;
        }
        // namestanje dece ako ih ima
        if (!sons_list.empty()) {
            for (int i=0; i<index1+1; i++) {
                sons_list[i]->father=left_son;
                left_son->children[i]=sons_list[i];
                left_son->childrenNum++;
            }
            for (int i=index1+1; i<index2+1; i++) {
                sons_list[i]->father=middle_son;
                middle_son->children[i-index1-1]=sons_list[i];
                middle_son->childrenNum++;
            }
            for (int i=index2+1; i<keys_list.size()+1; i++) {
                sons_list[i]->father=right_son;
                right_son->children[i-index2-1]=sons_list[i];
                right_son->childrenNum++;
            }
        }

        // kljuc koji je bio ocev a ucestvovao je u razdvajanju moramo da obrisemo i izvrsimo preraspodelu
        for (int i=indexfather; i<temp->numKeys; i++) {
            temp->keys[i]=temp->keys[i+1];
        }
        temp->keys[temp->numKeys-1]=nullptr;
        temp->numKeys--;
        // nakon sto smo kljuc uklonili, dodajemo zasigurno 2 nova, prvo na kraj pa sortiramo kljuceve
        temp->keys[temp->numKeys]=keys_list[index1];
        temp->numKeys++;
        temp->keys[temp->numKeys]=keys_list[index2];
        temp->numKeys++;
        temp->sortKeys();
        //a sada treba da azuriramo pokazivace za levog, srednjeg i desnog novog sina
        //moramo da se pomeramo u desno
        for (int i=temp->childrenNum; i>indexfather; i--) {
            temp->children[i]=temp->children[i-1];
        }
        temp->childrenNum++;
        temp->children[indexfather]=left_son;
        temp->children[indexfather+1]=middle_son;
        temp->children[indexfather+2]=right_son;
    }

    void three_to_two(Node *l_node, Node *m_node, Node *r_node) {
        // u ovom slucaju se ne gubi visina stabla
        // prvo cemo sve kljuceve da poguramo u jednu listu
        vector<Data*> keys_list;
        Node* father=l_node->father;
        Data *parentKey1;
        Data *parentKey2;
        for (int i=0; i<father->numKeys; i++) {
            if (father->children[i]==l_node)
                parentKey1=father->keys[i];
            if (father->children[i]==m_node) {
                parentKey2=father->keys[i];
                break;
            }
        }
        for (int i=0; i<l_node->numKeys; i++) {
            keys_list.push_back(l_node->keys[i]);
        }
        keys_list.push_back(parentKey1);
        for (int i=0; i<m_node->numKeys; i++) {
            keys_list.push_back(m_node->keys[i]);
        }
        keys_list.push_back(parentKey2);
        for (int i=0; i<r_node->numKeys; i++) {
            keys_list.push_back(r_node->keys[i]);
        }

        // sada cemo svu decu od tih kljuceva da poguramo u listu
        vector<Node*> sons_list;
        if (l_node->childrenNum!=0) {
            for (int i=0; i<l_node->childrenNum; i++) {
                sons_list.push_back(l_node->children[i]);
            }
            for (int i=0; i<m_node->childrenNum; i++) {
                sons_list.push_back(m_node->children[i]);
            }
            for (int i=0; i<m_node->childrenNum; i++) {
                sons_list.push_back(m_node->children[i]);
            }
        }
        Node *left=new Node();
        left->isRoot = false;
        left->minNum=minnumNode;
        left->maxNum=maxnumNode;
        left->father=l_node->father;
        left->numKeys=0;
        left->childrenNum=0;
        Node *right=new Node();
        right->isRoot = false;
        right->minNum=minnumNode;
        right->maxNum=maxnumNode;
        right->father=l_node->father;
        right->numKeys=0;
        right->childrenNum=0;
        int middle=(keys_list.size()-1)/2;
        Data *goUp=keys_list[middle];
        for (int i=0; i<middle; i++) {
            left->keys[i]=keys_list[i];
            left->numKeys++;
        }
        for (int i=middle+1; i<keys_list.size(); i++) {
            right->keys[i-middle-1]=keys_list[i];
            right->numKeys++;
        }
        if (!sons_list.empty()) {
            for(int i=0; i<=middle; i++) {
                left->children[i]=sons_list[i];
                left->childrenNum++;
            }
            for(int i=middle+1; i<=keys_list.size(); i++) {
                left->children[i-middle-1]=sons_list[i];
                left->childrenNum++;
            }
        }
        for (int i=0; i<father->numKeys; i++) {
            if (father->children[i]==l_node) {
                father->keys[i]=goUp;
                father->children[i]=left;
                father->children[i+1]=right;
                // treba da pomerimo ostatak niza kljuceva od oca na levo
                for (int j=i+1; j<father->numKeys; j++) {
                    father->keys[j]=father->keys[j+1];
                }
                // a treba i ostatak dece desno da pomerimo na levo i prevezemo sve lepo
                for (int j=i+2; j<father->childrenNum; j++) {
                    father->children[j]=father->children[j+1];
                }
                father->childrenNum--;
                break;
            }
        }
        father->numKeys--;

    }

    void two_to_one(Node *l_node, Node *r_node) {
        if (l_node->father->isRoot && l_node->father->numKeys==1)
            root=l_node;
        // prvo da u leve kljuceve pokupimo kljuceve od oca
        int pom;
        for(int i=0; i<l_node->father->numKeys; i++) {
            if (l_node->father->children[i]==l_node) {
                pom=i;
            }
        }
        for (int i=pom; i<l_node->father->numKeys; i++) {
            l_node->keys[l_node->numKeys]=l_node->father->keys[i];
            l_node->father->keys[i]=nullptr;
            l_node->father->numKeys--;
            l_node->numKeys++;
        }
        // onda da u leve kljuceve pokupimo kljuceve od brata
        for (int i=0; i<r_node->numKeys; i++) {
            l_node->keys[l_node->numKeys]=r_node->keys[i];
            r_node->keys[i]=nullptr;
            l_node->numKeys++;
        }
        // i sada da pokupimo jos decu od desnog brata
        for (int i=0; i<r_node->childrenNum; i++) {
            l_node->children[l_node->childrenNum]=r_node->children[i];
            r_node->children[i]->father=l_node;
            l_node->childrenNum++;
        }
        delete(r_node);
        l_node->father->children[l_node->father->childrenNum-1]=nullptr;
        l_node->father->childrenNum--;
        if (l_node->father->isRoot)
            l_node->father=nullptr;

    }

    void insert(Data *elem)
    {
        if (root==nullptr) {
            root = new Node();
            for (int i = 0; i < 15; i++) {
                root->keys[i] = nullptr;
                root->children[i] = nullptr;
            }
            root->father=nullptr;
            root->numKeys=0;
            root->minNum=minnumRoot;
            root->maxNum=maxnumRoot;
            root->isRoot=true;
            root->childrenNum=0;
            root->keys[root->numKeys]=elem;
            root->numKeys++;
            return;
        }

        Node *pom;
        // pronadjemo cvor gde hocemo da ubacimo element i ubacimo ga tu svakako
        pom=find_insert_root(elem);
        pom->keys[pom->numKeys]=elem;
        pom->numKeys++;
        pom->sortKeys();
        // sada trazimo cvor koji je prepunjen
        while (true)
        {
            Node *invalid=find_invalid_root();
            if (invalid==nullptr)
                return;
            // prvi slucaj, ako je nevalidan cvor koren, uvek se deli na levi, desni i koren
            // on se ne preliva nigde jer bracu nema
            if (invalid==root) {
                split_root();
            }
            else
            {   // drugi slucaj, pokusavamo prelivanje
                Node *r_bro=invalid->find_right_bro();
                Node *l_bro=invalid->find_left_bro();
                // prvo pokusavamo da se prelijemo desno
                if (r_bro!=nullptr && r_bro->numKeys<r_bro->maxNum-1) {
                    overflow_right(invalid, r_bro);
                }
                // onda pokusamo da se prelijemo levo
                else if (l_bro!=nullptr && l_bro->numKeys<l_bro->maxNum-1) {
                    overflow_left(l_bro, invalid);
                }
                else if (r_bro!=nullptr)
                {
                    two_to_three(invalid, r_bro);
                }
                else
                {
                    two_to_three(l_bro, invalid);
                }
            }
        }
    }

    void printTree()   // ispis po level orderu
    {
        cout<<endl;
        vector<Node*> ispis;
        if (root==nullptr) {
            cout<<"Stablo je prazno"<<endl<<endl;
            return;
        }
        ispis.push_back(root);
        while(ispis.size()>0) {
            Node* temp=ispis.front();
            ispis.erase(ispis.begin());
            for (int i=0; i<temp->numKeys+1; i++)   // dece ima za 1 vise od kljuceva
            {
                if (temp->children[i]!=nullptr) {
                    ispis.push_back(temp->children[i]);
                }
            }
            temp->print_Node();
            cout<<endl;
        }
    }

    void erase_tree() {
        vector<Node*> my_queue;
        if (root==nullptr) {
            cout<<"Stablo je prazno"<<endl<<endl;
            return;
        }
        my_queue.push_back(root);
        while(my_queue.size()>0) {
            Node* temp=my_queue.front();
            my_queue.erase(my_queue.begin());
            for (int i=0; i<temp->numKeys+1; i++)   // dece ima za 1 vise od kljuceva
            {
                if (temp->children[i]!=nullptr) {
                    my_queue.push_back(temp->children[i]);
                }
            }
            for (int i=0; i<temp->numKeys; i++) {
                delete(temp->keys[i]);
            }
            delete(temp);
            root=nullptr;
        }
        cout<<endl<<"Stablo uspesno obrisano!"<<endl<<endl;
    }

    void search_by_key(string key) {
        Node *temp=root;
        int br_koraka=1;
        int br_kljuceva=1;
        bool found=false;
        while (true) {
            for (int i=0; i<temp->numKeys+1; i++) {
                if (temp->keys[i]==nullptr || temp->keys[i]->id > key) {
                    temp=temp->children[i];
                    br_koraka++;
                    break;
                }
                else if (temp->keys[i]->id==key) {
                    found=true;
                    cout<<endl<<"PRONADJEN!"<<endl;
                    cout<<"Primarni kljuc: "<<temp->keys[i]->id<<endl;
                    cout<<"Ime: "<<temp->keys[i]->f_name<<endl;
                    cout<<"Prezime: "<<temp->keys[i]->l_name<<endl;
                    cout<<"Email: "<<temp->keys[i]->email<<endl;
                    cout<<"Strani kljuc: "<<temp->keys[i]->id2<<endl;
                    cout<<"Broj pristupa disku: "<<br_koraka<<endl;
                    cout<<"Broj predjenih kljuceva: "<<br_kljuceva<<endl<<endl;
                    break;
                }
                br_kljuceva++;
            }
            if (found==true) {
                break;
            }
            if (found==false && temp==nullptr) {
                cout<<endl<<"Dati kljuc ne postoji u stablu!"<<endl<<endl;
                return;
            }
        }
    }

    void search_by_name(string name) {
        cout<<endl;
        int br_koraka=0, br_kljuceva=0;
        bool found=false;
        vector<Node*> my_queue;
        if (root==nullptr) {
            cout<<"Stablo je prazno"<<endl<<endl;
            return;
        }
        my_queue.push_back(root);
        while(my_queue.size()>0) {
            Node* temp=my_queue.front();
            br_koraka++;
            my_queue.erase(my_queue.begin());
            for (int i=0; i<temp->numKeys+1; i++)   // dece ima za 1 vise od kljuceva
            {
                if (temp->children[i]!=nullptr) {
                    my_queue.push_back(temp->children[i]);
                }
            }
            for(int i=0; i<temp->numKeys; i++) {
                br_kljuceva++;
                if (temp->keys[i]->f_name==name) {
                    found=true;
                    cout<<"Primarni kljuc: "<<temp->keys[i]->id<<endl;
                    cout<<"Ime: "<<temp->keys[i]->f_name<<endl;
                    cout<<"Prezime: "<<temp->keys[i]->l_name<<endl;
                    cout<<"Email: "<<temp->keys[i]->email<<endl;
                    cout<<"Strani kljuc: "<<temp->keys[i]->id2<<endl<<endl;
                }
            }
        }
        if (!found)
            cout<<"Ime nije pronadjeno u bazi podataka!"<<endl;
        cout<<"Broj pristupa disku: "<<br_koraka<<endl;
        cout<<"Broj predjenih kljuceva: "<<br_kljuceva<<endl<<endl;
    }

    Data* return_by_key(string key, int *br_koraka, int *br_kljuceva) {
        Node *temp=root;
        (*br_koraka)++;
        while (true) {
            for (int i=0; i<temp->numKeys+1; i++) {
                if (temp->keys[i]!=nullptr)
                    (*br_kljuceva)++;
                if (temp->keys[i]==nullptr || temp->keys[i]->id > key) {
                    temp=temp->children[i];
                    (*br_koraka)++;
                    break;
                }
                else if (temp->keys[i]->id==key) {
                    return temp->keys[i];
                }
            }
            if (temp==nullptr) {
                return nullptr;
            }
        }
    }

    bool if_exists_delete_and_switch(string key) {
        Node* temp=root;
        bool found=false;
        while (true) {
            for (int i=0; i<temp->numKeys+1; i++) {
                if (temp->keys[i]==nullptr || temp->keys[i]->id > key) {
                    temp=temp->children[i];
                    break;
                }
                else if (temp->keys[i]->id==key) {
                    found=true;
                    // sada ako nismo u listu, trazimo sledbenika i menjamo se
                    // temp2 je zapravo taj nas list koji ce da se smanji
                    // ali prvo da vidimo dal smo mi list
                    if (temp->childrenNum==0) {
                        for (int j=i; j<temp->numKeys; j++) {
                            temp->keys[j]=temp->keys[j+1]; // pomeramo ceo niz u levo
                        }
                        temp->keys[temp->numKeys-1]=nullptr;
                        temp->numKeys--;
                        return true;
                    }
                    Node *temp2=temp->children[i+1];
                    while(temp2->childrenNum!=0) {
                        temp2=temp2->children[0];
                    }
                    Data *pom=temp2->keys[0];
                    temp2->keys[0]=temp->keys[i];
                    temp->keys[i]=pom;
                    for (int j=0; j<temp2->numKeys; j++) {
                        temp2->keys[j]=temp2->keys[j+1]; // pomeramo ceo niz u levo
                    }
                    temp2->keys[temp2->numKeys-1]=nullptr;
                    temp2->numKeys--;
                    return true;
                }
            }
            if (found==false && temp==nullptr) {
                return false;
            }
        }
    }

    void update_tree() {
        while (true)
        {
            Node *invalid=find_invalid_root_for_update();
            // napravicemo takodje da koren nikad ne narusava kriterijum
            if (invalid==nullptr)
                return;
            Node *first_l_bro=invalid->find_left_bro();
            Node *first_r_bro=invalid->find_right_bro();
            Node *second_l_bro=nullptr;
            Node *second_r_bro=nullptr;
            if (first_l_bro!=nullptr)
                second_l_bro=first_l_bro->find_left_bro();
            if (first_r_bro!=nullptr)
                second_r_bro=first_r_bro->find_right_bro();
            // ako moze da nam pozajmi prvi desni brat, nek pozajmi
            if (first_r_bro!=nullptr && first_r_bro->numKeys > first_r_bro->minNum-1) {
                overflow_left(invalid, first_r_bro);
            }
            // inace ako moze da nam pozajmi prvi levi brat, nek pozajmi
            else if (first_l_bro!=nullptr && first_l_bro->numKeys > first_l_bro->minNum-1){
                overflow_right(first_l_bro, invalid);
            }
            // ako ni to ne moze, nek pozajmi drugi desni
            else if (second_r_bro!=nullptr && second_r_bro->numKeys > second_r_bro->minNum-1) {
                overflow_left(first_r_bro, second_r_bro);
                overflow_left(invalid, first_r_bro);
            }
            // ako ni to ne moze nek pozajmi drugi levi
            else if (second_l_bro!=nullptr && second_l_bro->numKeys > second_l_bro->minNum-1) {
                overflow_right(second_l_bro, first_l_bro);
                overflow_right(first_l_bro, invalid);
            }
            // ako nema ko da zajmi pitamo se
            // prvo da li postoje levi i desni brat, da sa njima radimo spajanje
            else if (first_l_bro!=nullptr && first_r_bro!=nullptr){
                three_to_two(first_l_bro, invalid, first_r_bro);
            }
            // onda da li postoje dva desna
            else if (first_r_bro!=nullptr && second_r_bro!=nullptr) {
                three_to_two(invalid, first_r_bro, second_r_bro);
            }
            // onda da li postoje dva leva
            else if (first_l_bro!=nullptr && second_l_bro!=nullptr) {
                three_to_two(second_l_bro, first_l_bro, invalid);
            }
            // ako nista od toga nije ide spajanje sa jedinim preostalim bratom, desnim ili levim
            // ovo se desava samo za m=3 ja mislim
            else if (first_r_bro!=nullptr) {
                two_to_one(invalid, first_r_bro);
            }
            else
                two_to_one(first_l_bro, invalid);
        }
    }

};

void mainMenu() {
    cout<<"OPCIJE: "<< endl;
    cout<<"1-Ispis stabla"<<endl;
    cout<<"2-Dodavanje cvora"<<endl;
    cout<<"3-Brisanje cvora"<<endl;
    cout<<"4-Pretraga po kljucu"<<endl;
    cout<<"5-Pretraga po imenu"<<endl;
    cout<<"6-Pretraga na k kljuceva"<<endl;
    cout<<"7-Brisanje stabla"<<endl;
    cout<<"8-Izlaz"<<endl;
    cout<<"Vas izbor: ";
}

void main_insert(Tree *stablo) {
    Data *d=new Data();
    cout<<"Unesite primarni kljuc: ";
    cin>>d->id;
    cout<<"Unesite ime: ";
    cin>>d->f_name;
    cout<<"Unesite prezime: ";
    cin>>d->l_name;
    cout<<"Unesite email: ";
    cin>>d->email;
    cout<<"Unesite strani kljuc: ";
    cin>>d->id2;
    stablo->insert(d);
}

void main_search_by_key(Tree *stablo) {
    string key;
    cout<<"Unesite kljuc za pretragu: ";
    cin>>key;
    stablo->search_by_key(key);
}

void main_search_by_k_keys(Tree *stablo) {
    int k, br_koraka=0, br_kljuceva=0;
    vector <string>niz_kljuceva;
    cout<<"Unesite broj kljuceva: ";
    cin>>k;
    cout<<"Unesite kljuceve:"<<endl;
    cin.ignore();
    for (int i = 0; i < k; i++) {
        string line;
        getline(cin, line);
        niz_kljuceva.push_back(line);
    }
    vector<Data*> niz_podataka;
    for (int i=0; i<niz_kljuceva.size(); i++) {
        Data *pom=stablo->return_by_key(niz_kljuceva[i], &br_koraka, &br_kljuceva);
        if (pom)
            niz_podataka.push_back(pom);
    }
    // a sada da upisemo sve ovo u fajl
    ofstream outFile("izlaz.txt");
    if (!outFile) {
        cout<<"Fajl ne moze da se otvori"<<endl;
    }
    for (int i = 0; i < niz_podataka.size(); i++) {
        outFile<<niz_podataka[i]->print_in_file()<<endl;
    }
    outFile << "Broj koraka: " << br_koraka << endl;
    outFile << "Broj kljuceva: " << br_kljuceva << endl;
    outFile.close();
}

void main_search_by_name(Tree *stablo) {
    string name;
    cout<<"Unesite ime za pretragu: ";
    cin>>name;
    stablo->search_by_name(name);
}

void main_delete(Tree* stablo) {
    string key;
    cout<<"Primarni kljuc elementa za brisanje: ";
    cin>>key;
    bool exists_delete_key=stablo->if_exists_delete_and_switch(key);
    if (!exists_delete_key) {
        cout<<"Kljuc ne postoji u stablu!"<<endl;
        return;
    }
    stablo->update_tree();
    cout<<"Cvor uspesno obrisan!"<<endl;
}

int main() {
    int choice = 0;
    int choice2 = 0;
    vector<Data*> nizPodataka;
    int red=0;
    Tree *stablo=new Tree();
    cout << "POCETNI MENI" << endl;
    cout << "1 - Unos sa standardnog ulaza" << endl;
    cout << "2 - Unos iz datoteke" << endl;
    cout << "0 - Izlaz" << endl;
    cin >> choice;
    if (choice == 0) {
        cout << "Dovidjenja!" << endl;
        return 0;
    }
    if (choice == 2) {
        string naziv;
        cout << "Unesite ime datoteke" << endl;
        cin >> naziv;
        ifstream dat;
        dat.open(naziv);
        if (!dat.is_open()) {
            cout << "Datoteka ne postoji" << endl;
            return 0;
        }
        string linija;
        while (getline(dat, linija)) {
            if (linija.empty()) {
                continue;
            }
            Data *d=new Data();
            stringstream ss(linija);
            string temp;
            getline(ss, d->id, '|');
            getline(ss, d->f_name, '|');
            getline(ss, d->l_name, '|');
            getline(ss, d->email, '|');
            getline(ss, d->id2, '|');
            nizPodataka.push_back(d);
        }
        dat.close();
        cout<<"Unesite red stabla: ";
        cin>>red;
        stablo->root=nullptr;
        stablo->m=red;
        stablo->minnumRoot = 2;
        stablo->maxnumRoot = 2*floor((2*red-2)/3)+1;
        double pom=(double(2*red)-1)/3;
        stablo->minnumNode = ceil(pom);
        stablo->maxnumNode=red;
        for (int i=0; i<nizPodataka.size(); i++)
        {
            if (nizPodataka[i]!=nullptr) {
                stablo->insert(nizPodataka[i]);
            }
        }
        cout<<endl<<"Stablo uspesno kreirano!"<<endl<<endl;
    }
    while (true) {
        mainMenu();
        cin>>choice2;
        if (choice2==1) { // ispis stabla
            stablo->printTree();
        }
        else if (choice2==2) { // dodavanje novog cvora
            main_insert(stablo);
        }
        else if (choice2==3) { // brisanje cvora
            main_delete(stablo);
        }
        else if (choice2==4) { // pretraga po kljucu
            main_search_by_key(stablo);
        }
        else if (choice2==5) { // pretraga po imenu
            main_search_by_name(stablo);
        }
        else if (choice2==6) {
            main_search_by_k_keys(stablo);
        }
        else if (choice2==7) { // brisanje celog stabla
            stablo->erase_tree();
        }
        else if (choice2==8) { // izlaz
            cout<<endl<<"Dovidjenja!"<< endl;
            break;
        }
    }
    delete stablo;
    return 0;
}