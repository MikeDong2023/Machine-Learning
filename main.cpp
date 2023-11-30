#include <iostream>
#include <set>
#include "csvstream.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    //Error checking
    if(argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    try {csvstream trainFile(argv[1]);}
    catch(csvstream_exception const&) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    try {csvstream testFile(argv[2]);}
    catch(csvstream_exception const&) {
        cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }
}
class Classifier {
    private:
        set<string> allWords;
        map<string, int> numPostsContainingWord;
        map<string, int> numPostsWithLabel;
        /* The key is the label; the value is the word counts of its associated
         * posts.
         */
        map<string, map<string, int>> numPostsWithLabelContainingWord;
        int numPosts;

        /* MODIFIES: allWords, numPostsContainingWord,
         * and numPostsWithLabelContainingWord.
         * EFFECTS: Adds word to allWords and increments the number of posts
         * containing it and the number of posts with the same label containing
         * it if it hasn't already.
         */
        void processWord(string &word, set<string> &postVocab, string &postLabel) {
            allWords.insert(word);
            if(postVocab.find(word) == postVocab.end()) {
                numPostsContainingWord[word] += 1;
                postVocab.insert(word);
                numPostsWithLabelContainingWord[postLabel][word] += 1;
            }
        }
    public:
        /* REQUIRES: post is a map respresenting a Piazza post with 2 pairs:
         * one containing the label, and the other containing the content.
         * MODIFIES: numPosts, numPostsWithLabel, allWords, and
         * numPostsContainingWord
         * EFFECTS: Updates this based on the post passed in.
         */
        void processPost(std::map<string, string> &post) {
            ++numPosts;
            numPostsWithLabel[post["label"]] += 1;
            istringstream postBody(post["content"]);
            string word;
            set<string> postVocab;
            while(postBody >> word) {
                processWord(word, postVocab, post["label"]);
            }
        }
        int numUniqueWords() const {
            return allWords.size();
        }
        int calculateLikelihood(string &label, set<string> postVocab) const;
        string classifyPost(string &contents) const;
};