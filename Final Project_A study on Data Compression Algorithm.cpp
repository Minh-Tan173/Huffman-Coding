#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Defines the structure of the node in the Huffman tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

// Compare two nodes for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) 
    {
        return l->freq > r->freq;
    }
};

// Free memory for the Huffman tree
void freeHuffmanTree(Node* root) {
    if (root == nullptr) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

// Create a Huffman tree
Node* buildHuffmanTree(const string& text, unordered_map<char, int>& freq) 
{
    // Calculate the frequency of characters
    for (char ch : text) {
        freq[ch]++;
    }

    // Create priority queue
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (const auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build a Huffman tree
    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

// Generate Huffman encoding
void createHuffmanTable(Node* root, const string& str, unordered_map<char, string>& huffmanTable) 
{
    if (!root) return;

    if (!root->left && !root->right) 
    {
        huffmanTable[root->ch] = str;
    }
    createHuffmanTable(root->left, str + "0", huffmanTable);
    createHuffmanTable(root->right, str + "1", huffmanTable);
}

// Encode string
string encode(const string& text, const unordered_map<char, string>& huffmanTable) {
    string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanTable.at(ch);
    }
    return encodedString;
}

// Decode string
string decode(Node* root, const string& encodedString) {
    string decodedString = "";
    Node* currentNode = root;
    for (char bit : encodedString) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            decodedString += currentNode->ch;
            currentNode = root;
        }
    }
    return decodedString;
}

// Displays the Huffman encoding table
void printHuffmanTable(const unordered_map<char, string>& huffmanTable, const unordered_map<char, int>& freq) {
    cout << "+-------+--------+----------------+\n";
    cout << "| Char  | Freq   | Huffman Code   |\n";
    cout << "+-------+--------+----------------+\n";
    for (const auto& pair : huffmanTable) {
        cout << "|  " << setw(3) << pair.first << "  | " << setw(6) << freq.at(pair.first) << " | " << setw(14) << pair.second << " |\n";
    }
    cout << "+-------+--------+----------------+\n";
}

void printMenu() {
    cout << "====================================\n";
    cout << "|           HUFFMAN MENU           |\n";
    cout << "====================================\n";
    cout << "| 1. Enter the data to be encoded  |\n";
    cout << "| 2. Encoded result                |\n";
    cout << "| 3. Exit                          |\n";
    cout << "====================================\n";
    cout << "Enter the selection: ";
}

bool askReturnToMenu() {
    string answer;
    cout << endl << "Do you want to return to the menu? (yes/no): ";
    cin >> answer;
    return answer == "yes" || answer == "Yes" || answer == "YES";
}

int main() {
    string text;
    string encodedString;
    string decodedString;
    Node* root = nullptr;
    unordered_map<char, string> huffmanTable;
    unordered_map<char, int> freq;

    int choice;
    bool returnToMenu = true;

    while (returnToMenu) {
        system("cls"); // Clear the screen  
        printMenu();
        cin >> choice;
        cin.ignore(); // Ignore newline characters after entering a selection

        switch (choice) {
            case 1:
                system("cls"); // Clear the screen
                cout << "Input text: ";
                getline(cin, text);

                // Re-initialize variables
                if (root) {
                    freeHuffmanTree(root);
                }
                huffmanTable.clear();
                freq.clear();
                encodedString.clear();
                decodedString.clear();

                // Implement the Huffman algorithm
                root = buildHuffmanTree(text, freq); // Key: text, Value: freq
                createHuffmanTable(root, "", huffmanTable);
                encodedString = encode(text, huffmanTable);
                decodedString = decode(root, encodedString);

                cout << endl << "Encode completed" << endl;
                cout << "Original size: " << text.size() * 8 << " bits" << endl;
                cout << "Encoded size: " << encodedString.size() << " bits" << endl;
                returnToMenu = askReturnToMenu();
                break;

            case 2:
                system("cls"); // Clear the screen
                if (!root) {
                    cout << "You haven't entered the data for encoding!\n";
                } else {
                    cout << "Huffman encoding table:\n";
                    printHuffmanTable(huffmanTable, freq);
                    cout << endl << "Encoded string: " << encodedString << endl;
                    cout << endl << "Decoded string: " << decodedString << endl;
                }
                returnToMenu = askReturnToMenu();
                break;

            case 3:
                cout << "End the program.\n";
                returnToMenu = false;
                break;
            default:
                cout << "The selection is not valid. Please withdraw again.\n";
                returnToMenu = askReturnToMenu();
        }
    }

    if (root) {
        freeHuffmanTree(root);
    }

    return 0;
}
