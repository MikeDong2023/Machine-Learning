#include <iostream>
#include <set>
#include <cmath>
#include "csvstream.hpp"
using namespace std;

class Classifier;

class Classifier {
    private:
        set<string> allWords;
        map<string, int> numPostsContainingWord;
        map<string, int> numPostsWithLabel;
        /* The key is the label; the value is the word counts of its associated
         * posts.
         */
        map<string, map<string, int>> numPostsWithLabelContainingWord;
        int numPosts = 0;//fixed?nope

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
        set<string> uniqueWords(const string &contents) {
            istringstream in(contents);
            string word;
            set<string> vocab;
            while(in >> word) {
                vocab.insert(word);
            }
            return vocab;
        }
    public:
        Classifier(csvstream &trainingData) {
            map<string, string> post;
            while(trainingData >> post) {
                processPost(post);
            }
        }
        vector<string> alphabetizedLabels() const {
            vector<string> labelVec;
            for(auto it = numPostsWithLabel.begin(); it != numPostsWithLabel.end();
             ++it) {
                labelVec.push_back(it->first);
            }
            sort(labelVec.begin(), labelVec.end());
            return labelVec;
        }
        vector<string> alphabetizedWords(const string &label) {
            set<string> vocab;
            for(auto it = numPostsWithLabelContainingWord[label].begin();
             it != numPostsWithLabelContainingWord[label].end(); ++it) {
                vocab.insert(it->first);
             }
             vector<string> words;
             for(const string &word : vocab) {
                words.push_back(word);
             }
             return words;
        }
        /* REQUIRES: post is a map respresenting a Piazza post with 2 pairs:
         * one containing the label, and the other containing the content.
         * MODIFIES: numPosts, numPostsWithLabel, allWords, and
         * numPostsContainingWord
         * EFFECTS: Updates this based on the post passed in.
         */
        void processPost(std::map<string, string> &post) {
            ++numPosts;
            //cout << numPosts << endl;
            numPostsWithLabel[post["tag"]] += 1;
            istringstream postBody(post["content"]);
            string word;
            set<string> postVocab;
            while(postBody >> word) {
                processWord(word, postVocab, post["tag"]);
            }
        }
        int numUniqueWords() const {
            return allWords.size();
        }
        int pussyCat() const {
            return numPosts;
        }
        int labelledPostsWithWord(const string &word) {
            return numPostsWithLabel[word];
        }
        int labelWordMap(const string &label, const string &word) {
            return numPostsWithLabelContainingWord[label][word];
        }
        double calculateLogPrior(const string &label) {
            //cout << numPostsWithLabel[label] << endl;
            //cout << numPosts << endl;
            return log(static_cast<double>(numPostsWithLabel[label])/numPosts);
        }
        
        double calculateLogLikelihood(const string &label, const string &word) {
            if(numPostsContainingWord[word] == 0 ) {
                return log(1/static_cast<double>(numPosts));
            }
            if(numPostsWithLabelContainingWord[label][word] == 0) {
                return log(static_cast<double>(numPostsContainingWord[word])/numPosts);
            }
            
            /*cout << numPostsWithLabelContainingWord[label][word] << endl;
            cout << numPostsWithLabel[label] << endl;
            cout << log(static_cast<double>(numPostsWithLabelContainingWord[label][word])/
                numPostsWithLabel[label]) << endl;

            cout << endl;*/
            return log(static_cast<double>(numPostsWithLabelContainingWord[label][word])/
                numPostsWithLabel[label]);
        }
        double calculateLikelihood(const string &label, const string &contents) {
            //Calculate log-prior [ln P(C)]
            double probability = calculateLogPrior(label);
            //cout << probability << endl;
            set<string> wordBag = uniqueWords(contents);
            for(const string &word : wordBag) {
               // cout << calculateLikelihood(label, word) << endl;
                probability += calculateLogLikelihood(label, word);
               // cout << probability << endl;
            }
            return probability;
        }
        
        string classifyPost(string &contents) {
            vector<string> labels = alphabetizedLabels();
            string likeliestLabel = labels[0];
            double maxLikelihood = calculateLikelihood(likeliestLabel, contents);
            
            //Loop through each label and find the one with the highest likelihood
            for(const string &label : labels) {
                
                if(calculateLikelihood(label, contents) > maxLikelihood) {
                    likeliestLabel = label;
                    maxLikelihood = calculateLikelihood(label, contents);
                }
            }
            return likeliestLabel;
        };
        
};
int main(int argc, char* argv[]) {
    //Error checking
    cout.precision(3);
    if(argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    if(argc == 4 && strcmp(argv[3], "--debug") != 0) {
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
    csvstream trainFile(argv[1]);
    Classifier sortBot(trainFile);
    if (argc == 4) {
        cout << "training data:" << endl;
        std::map<string,string> trainPrint;
        cout << "hi";
        while(trainFile >> trainPrint) {
            cout << "hello" ;
            cout << "  label = " << trainPrint["tag"] << ", content = ";
            cout << trainPrint["content"] << endl;
        }
    }
    cout << "trained on " << sortBot.pussyCat() << " examples" << endl;
    if(argc == 4) {
        cout << "vocabulary size = " << sortBot.numUniqueWords() << endl;
    }
    cout << "\n";
    vector<string> labels = sortBot.alphabetizedLabels();
    if(argc == 4) {
        cout << "classes: " << endl;
        for(const string &tag : labels) {
            cout << "  " << tag << ", " << sortBot.labelledPostsWithWord(tag) 
            << " examples, log-prior = " << sortBot.calculateLogPrior(tag) << endl;
        }
        cout << "classifier parameters:" << endl;
        for(const string &tag : labels) {
            vector<string> associatedWords = sortBot.alphabetizedWords(tag);
            for(string &word : associatedWords) {
                cout << "  " << tag << ":" << word << ", count = " << 
                sortBot.labelWordMap(tag, word) << ", log-likelihood = " 
                << sortBot.calculateLogLikelihood(tag, word) << endl;
            }
        }
        cout << "\n";
    }
    cout << "test data: "<< endl;
    csvstream testStream(argv[2]);
    int numCorrect = 0, numPosts = 0;
    std::map <string, string> testFile;
    while (testStream >> testFile) {
        ++numPosts;
        string predictedLabel = sortBot.classifyPost(testFile["content"]);
        if(predictedLabel == testFile["tag"]) ++numCorrect;
        cout << "  correct = " << testFile["tag"] << ", predicted = ";
        cout << predictedLabel << ", log-probability score = ";
        cout << sortBot.calculateLikelihood(predictedLabel, testFile["content"]) << endl;
        cout << "  contents = " << testFile["content"] << endl;//fixed no ouput 
        cout << endl;
    }
    cout << "performance: " << numCorrect << " / " << numPosts << " posts predicted correctly\n";
}