struct node
{
    int key; // ключ узла
    char delta; // разность поддеревьев узла
    node* left;  // указатель на левое поддерево
    node* right; // указатель на правое поддерево

    node(int k) {key = k; left = right = nullptr; height = 1;}
};
