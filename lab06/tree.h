#include <iostream>
#include <set>
#include <algorithm>

class Tree{
    struct Node{
            int id;
            bool available;
            int height;
            Node* left, *right;

            Node(int i): id(i), available(true), left(nullptr), right(nullptr){}
            ~Node(){}

            void findRemove(int idElem, std::set<int> &ids){
                if(left != nullptr && left->id == idElem){
                    left->removeChilds(ids);
                    ids.erase(left->id);
                    delete left;
                    left = nullptr;
                }
                else if(right != nullptr && right->id == idElem){
                    right->removeChilds(ids);
                    ids.erase(right->id);
                    delete right;
                    right = nullptr;

                }
                else{
                    if(left != nullptr){
                        left->findRemove(idElem, ids);
                    }
                    if(right != nullptr){
                        right->findRemove(idElem, ids);
                    }
                }
            }
            
            void removeChilds(std::set<int> &ids){
                if(left != nullptr){
                    left->removeChilds(ids);
                    ids.erase(left->id);
                    delete left;
                    left = nullptr;

                }
                if(right != nullptr){
                    right->removeChilds(ids);
                    ids.erase(right->id);
                    delete right;
                    right = nullptr;

                }
            }

            void addElem(int idElem, int parent, std::set<int> &ids){
                if(id == parent){
                    if(left == nullptr){
                        left = new Node(idElem);
                    }
                    else{
                        right = new Node(idElem);
                    }
                    ids.insert(idElem);

                }
                else{
                    if(left != nullptr){
                        left->addElem(idElem, parent, ids);
                    }
                    if(right != nullptr){
                        right->addElem(idElem, parent, ids);
                    }
                }
            }

            int minHight(){
                if(left == nullptr || right == nullptr){
                    return 0;
                }
                int lHight = -1;
                int rHight = -1;
                if(left != nullptr && left->available == true){
                    lHight = left->minHight();
                }
                if(right != nullptr && right->available == true){
                    rHight = right->minHight();
                }

                if(rHight == -1 && lHight == -1){
                    available = false;
                    return -1;
                }
                else if(rHight  == -1){
                    return lHight + 1;
                }
                else if(lHight == -1){
                    return rHight + 1;
                }
                else{
                    return std::min(lHight, rHight) + 1;
                }
            }
            
            void notAvailable(int idElem){
                if(id == idElem){
                    available = false;
                }
                else{
                    if(left != nullptr){
                        left->notAvailable(idElem);
                    }
                    if(right != nullptr){
                        right->notAvailable(idElem);
                    }
                }
            }

            int idForMinH(int hight, int curHight){
                if(hight < curHight){
                    return -2;
                }
                else if(hight > curHight){
                    int curid = -2;
                    if(left != nullptr && left->available == true){
                        curid = left->idForMinH(hight, (curHight + 1));
                    }
                    if(right != nullptr && right->available == true && curid == -2){
                        curid = right->idForMinH(hight, (curHight + 1));
                    }
                    return curid;
                }
                else{
                    if(left == nullptr || right == nullptr){
                        return id;
                    }
                    return -2;
                }
            }
    };

    private:
        Node* root;
//        std::map<std::pair<>>
    public:
        std::set<int> ids;
        //std::set<int> notAvailable;
        Tree(){
            root = new Node(-1);
        } 

        bool exist(int id){
            if(ids.find(id) != ids.end()){
                return true;
            }
            return false;
        }

        void notAvailable(int id){
            root->notAvailable(id);
        }
        
        int findId(){
            int h = root->minHight();
            return root->idForMinH(h, 0);
        }

        void addElem(int id, int parent){
            root->addElem(id, parent, ids);
        }

         void Remove(int idElem){
            root->findRemove(idElem, ids);
         }
        ~Tree(){
            root->removeChilds(ids);
            delete root;
        }
};