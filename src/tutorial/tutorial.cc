#include "tutorial/tutorial.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>
namespace buzzdb {
namespace tutorial {

/// Your implementation go here
CommandExecutor::CommandExecutor() {}
bool CommandExecutor::isWordCharacter(char ch) {
  ch = std::tolower(ch);
  return (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch == '\'');
}

std::string CommandExecutor::toLower(const std::string& str) {
  std::string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
  return lowerStr;
}

std::string CommandExecutor::tokenize(const std::string& input,
                                      std::vector<std::string>& tokens) {
  tokens.clear();
  std::string currentWord;
  bool handleSlash = false;

  for (char ch : input) {
    if (ch == ' ') {
      if (!currentWord.empty()) {
        tokens.push_back(currentWord);
        // Check if the first word is "load"
        if (tokens.size() == 1 && currentWord == "load") {
          handleSlash = true;
        }

        currentWord.clear();
      }
    } else if (handleSlash && (ch == '/' || ch == '.')) {
      currentWord += ch;
    } else if (ch == '0' && currentWord.empty()) {
      currentWord.clear();
    } else if (isWordCharacter(ch)) {
      currentWord += std::tolower(ch);
    } else {
      return "ERROR: Invalid command";
    }
  }

  // Handle the last word, if there is one.
  if (!currentWord.empty()) {
    tokens.push_back(currentWord);
  }

  // Return a string as a demonstration.
  return "";
}

void CommandExecutor::indexWordsInFile(const std::string& content,
                                       int& wordCount) {
  bool inWord = false;
  std::string currentWord;

  for (char ch : content) {
    if (isWordCharacter(ch)) {
      if (!inWord) {
        inWord = true;
        currentWord.clear();
      }
      currentWord += std::tolower(ch);
    } else {
      if (inWord) {
        wordOccurrences[currentWord].push_back(wordCount);
        wordCount++;
        inWord = false;
      }
    }
  }
  // If file ends while still inside a word
  if (inWord) {
    wordOccurrences[currentWord].push_back(wordCount);
    wordCount++;
  }
}

std::string CommandExecutor::locateNthOccurrence(const std::string& word,
                                                 int n) {
  std::string lowerWord = toLower(word);  // Convert word to lowercase

  if (wordOccurrences.find(lowerWord) !=
      wordOccurrences.end()) {  // Check if the word exists in our map

    const auto& occurrences =
        wordOccurrences[lowerWord];  // Get the vector of occurrences

    if (n > 0 &&
        static_cast<size_t>(n) <=
            occurrences.size()) {  // Check if the nth occurrence exists
      int num = occurrences[n - 1] + 1;
      std::string str = std::to_string(num);
      return str;
    } else {
      return "No matching entry";
    }
  } else {
    return "No matching entry";
  }
}

bool CommandExecutor::isValidWord(const std::string& word) {
  for (char c : word) {
    if (!isWordCharacter(c)) {
      return false;  // return false if the character isn't a letter, number, or
                     // apostrophe
    }
  }
  return true;
}

std::string CommandExecutor::execute(std::string command) {
  std::vector<std::string> words;
  std::string tokenizeOutput = tokenize(command, words);
  if (words.empty() || !tokenizeOutput.empty()) {
    return "ERROR: Invalid command";
  } else {
    std::string action = words[0];
    if (action == "new") {
      wordOccurrences.clear();
      return "";
    } else if (action == "load") {
      if (words.size() != 2) {
        return "ERROR: Invalid command";
      } else {
        int number = 0;
        std::string myText;
        std::string fileName = words[1];
        std::ifstream MyReadFile(fileName);
        while (getline(MyReadFile, myText)) {
          indexWordsInFile(myText, number);
        }
        MyReadFile.close();
        return "";
      }
    } else if (action == "locate") {
      if (words.size() != 3) {
        return "ERROR: Invalid command";
      } else {
        std::string word = words[1];
        int n = std::stoi(words[2]);
        return locateNthOccurrence(word, n);
      }
    } else if (action == "end") {
      wordOccurrences.clear();
      std::unordered_map<std::string, std::vector<int>>().swap(wordOccurrences);
      return "";
    } else {
      return "ERROR: Invalid command";
    }
  }
}

}  // namespace tutorial
}  // namespace buzzdb
